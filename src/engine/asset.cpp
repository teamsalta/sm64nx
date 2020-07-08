#include "sm64.h"
#include "asset.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#include <zstd.h>
#include <cstdlib>
#include "xxhash32.h"
#include "xxhash64.h"

#include <vector>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include <memory>

#include "hook_geo.h"
#include "hook_dl.h"
#include "hook_bhv.h"
#include "hook_texture.h"
#include "hook_sound.h"

#include "pc/nx.h"

#ifdef __SWITCH__
void initialize();
#endif

static std::string resolvePath(const char* path)
{
#ifdef __SWITCH__
	std::string result = "romfs:/";
#else
	std::string result = "romfs/";
#endif
	result += path;
	return result;
}

#define ASSET_MOUNT_MAGIC 0xf229924c86e8414f

struct MountEntry
{
	u8 type;
	u8 padding[3];
	u32 offset1;
	u64 junk;
	u64 ptrOffset;
	u64 hash;
};

static_assert(sizeof(MountEntry) == 0x20, "Mount Entry incorrect size");

struct MountHeader
{
	u64 magic;
	u32 entryCount;
	u32 size;
	MountEntry entries[1];

	bool isValid() const
	{
		return magic == ASSET_MOUNT_MAGIC;
	}
};

namespace sm64::asset
{
	u8* mount(MountHeader* func, u64 size);
}

static sm64::asset::Object* loadFileObject(const char* path)
{
	FILE* f = fopen(path, "rb");

	if(!f)
	{
		return nullptr;
	}

	fseek(f, 0, SEEK_END);

	u64 m_size = ftell(f);

	fseek(f, 0, SEEK_SET);

	if(!m_size)
	{
		fclose(f);
		return nullptr;
	}

	auto object = new sm64::asset::Object();

	if(!object)
	{
		fclose(f);
		return nullptr;
	}

	object->resize(m_size);

	fread(object->ptr(), 1, object->size(), f);
	fclose(f);

	auto ext = std::filesystem::path(path).extension();

	if(object->size() > sizeof(MountHeader))
	{
		auto header = (MountHeader*)object->ptr();

		if(header->isValid())
		{
			sm64::asset::mount(header, object->size());
			object->shift(header->size);
		}
	}

	return object;
}

namespace sm64
{
	Buffer::Buffer() : m_ptr(nullptr), m_size(0), m_allocSize(0), m_offset(0)
	{
	}

	Buffer::Buffer(const u64 sz) : m_ptr(nullptr), m_size(0), m_allocSize(0), m_offset(0)
	{
		resize(sz);
	}

	Buffer::~Buffer()
	{
		free();
	}

	void Buffer::shift(u64 offset)
	{
		m_offset = offset;
	}

	u8* Buffer::ptr() const
	{
		return m_ptr + m_offset;;
	}

	u64 Buffer::size() const
	{
		return m_size - m_offset;
	}

	void Buffer::free()
	{
		if(m_ptr)
		{
#ifdef _WIN32
			_aligned_free(m_ptr);
#else
			::free(m_ptr);
#endif
			m_ptr = nullptr;
		}
	}

	void Buffer::resize(const u64 sz, bool preserveData)
	{
		if(sz < m_allocSize)
		{
			m_size = sz;
			return;
		}

		m_allocSize = (sz / 0x4000) * 0x4000 + 0x4000;
#ifdef _WIN32
		u8* newAlloc = (u8*)_aligned_malloc(m_allocSize, 8);
#else
		u8* newAlloc = (u8*)aligned_alloc(8, m_allocSize);
#endif

		if(m_ptr)
		{
			//if(preserveData)
			{
				memcpy(newAlloc, m_ptr, m_size);
			}
			free();
		}

		m_ptr  = newAlloc;
		m_size = sz;
	}

	static size_t buffInSize  = 0;
	static size_t buffOutSize = 0;

	static u8* buffIn  = nullptr;
	static u8* buffOut = nullptr;

	void Buffer::append(const u8* buffer, const u64 sz)
	{
		u64 currentSize = m_size;

		resize(currentSize + sz);

		memcpy(m_ptr + currentSize, buffer, sz);
	}

	u64 Buffer::deflateZstd(const u8* buffer, const u64 sz)
	{
		if(!buffInSize)
		{
			buffInSize  = ZSTD_DStreamInSize();
			buffOutSize = ZSTD_DStreamOutSize();

			buffIn	= (u8*)malloc(buffInSize);
			buffOut = (u8*)malloc(buffOutSize);
		}

		u64 decompressedSize = ZSTD_getFrameContentSize(buffer, sz);

		resize(decompressedSize + buffOutSize, false);
		resize(0, false);

		ZSTD_inBuffer input = {buffer, sz, 0};
		ZSTD_outBuffer output = { m_ptr, m_allocSize + buffOutSize, 0 };
		

		ZSTD_DCtx* dctx = ZSTD_createDCtx();

		while(input.pos < input.size)
		{			
			size_t const ret      = ZSTD_decompressStream(dctx, &output, &input);

			if(ZSTD_isError(ret))
			{
				const char* err = ZSTD_getErrorName(ret);
				ZSTD_freeDCtx(dctx);
				return 0;
			}
			m_size += output.pos;
		}

		ZSTD_freeDCtx(dctx);

		return m_size;
	}

	namespace asset
	{
		struct Entry32
		{
			u32 xxhash;
			u32 length;
			u64 offset;
			u64 width;
			u64 height;
		};

		static_assert(sizeof(Entry32) == 32, "Entry32 incorrect size");

		enum class Type : u8
		{
			NONE = 0,
			TEXTURE,
			GEO,
			DISPLAY_LIST,
			BHV_SCRIPT,
			LEVEL_SCRIPT,
			SOUND_BIN = 8,
			SOUND_BANK,
			SOUND_TBL,
			SOUND_CTL,
			SOUND_MUSIC,
		};

		struct Entry64
		{
			u64 xxhash;
			u32 length;
			Type type;
			u8 flags[3];
			u64 offset;
			u64 width;
			u64 height;
			u64 padding[3];
		};

		static_assert(sizeof(Entry64) == 64, "Entry64 incorrect size");

		struct Header
		{
			u64 magic;
			u64 entryCount;
		};

		struct Header32 : public Header
		{
			const Entry32* lookup(const u64 xxhash)
			{
				u64 left  = 0;
				u64 right = entryCount;
				u64 i	  = right / 2;

				while(i < right)
				{
					const auto& entry = entries[i];

					if(entry.xxhash == xxhash)
					{
						return &entry;
					}

					if(xxhash > entry.xxhash)
					{
						if(left == i)
						{
							break;
						}
						left = i;
						i    = (left + right) / 2;
					}
					else
					{
						right = i;
						i     = (left + right) / 2;
					}
				}
				return nullptr;
			}

			Object& get(const u64 xxhash, Object& object, const u8* base)
			{
				auto entry = lookup(xxhash);

				if(entry)
				{
					object.resize(0);
					if(object.deflateZstd(base + entry->offset, entry->length))
					{
						object.width  = entry->width;
						object.height = entry->height;
					}
					else
					{
						object.resize(0);
						object.width  = 0;
						object.height = 0;
					}
					return object;
				}

				object.resize(0);
				return object;
			}

			Object& getIndex(const u64 i, Object& object, const u8* base)
			{
				if(i >= this->entryCount)
				{
					object.resize(0);
					return object;
				}

				auto& entry = this->entries[i];

				object.resize(0);
				if(object.deflateZstd(base + entry.offset, entry.length))
				{
					object.width  = entry.width;
					object.height = entry.height;
					object.hash   = entry.xxhash;
				}
				else
				{
					object.resize(0);
					object.width  = 0;
					object.height = 0;
					object.hash   = 0;
				}
				return object;
			}

			Entry32 entries[1];

			static bool is64Bit()
			{
				return false;
			}
		};

		struct Header64 : public Header
		{
			const Entry64* lookup(const u64 xxhash)
			{
				u64 left  = 0;
				u64 right = entryCount;
				u64 i	  = right / 2;

				while(i < right)
				{
					const auto& entry = entries[i];

					if(entry.xxhash == xxhash)
					{
						return &entry;
					}

					if(xxhash > entry.xxhash)
					{
						if(left == i)
						{
							break;
						}
						left = i;
						i    = (left + right) / 2;
					}
					else
					{
						right = i;
						i     = (left + right) / 2;
					}
				}
				return nullptr;
			}

			static Object& process(const Entry64& entry, Object& object)
			{
				switch(entry.type)
				{
					case Type::GEO:
						//hook::geo::mount((GeoLayout*)object.ptr, object.size);
						break;

					case Type::DISPLAY_LIST:
						//hook::dl::mount((Gfx*)object.ptr, object.size);
						break;
				}

				if(object.size() > sizeof(MountHeader))
				{
					auto header = (MountHeader*)object.ptr();

					if(header->isValid())
					{
						sm64::asset::mount(header, object.size());
						object.shift(header->size);
					}
				}

				return object;
			}

			Object& get(const u64 xxhash, Object& object, const u8* base)
			{
				auto entry = lookup(xxhash);

				if(entry)
				{
					object.resize(0);
					if(object.deflateZstd(base + entry->offset, entry->length))
					{
						object.width  = entry->width;
						object.height = entry->height;
					}
					else
					{
						object.resize(0);
						object.width  = 0;
						object.height = 0;
					}

					return process(*entry, object);
				}

				object.resize(0);
				return object;
			}

			Object& getIndex(const u64 i, Object& object, const u8* base)
			{
				if(i >= this->entryCount)
				{
					object.resize(0);
					return object;
				}

				auto& entry = this->entries[i];

				object.resize(entry.length);
				object.resize(0);
				if(object.deflateZstd(base + entry.offset, entry.length))
				{
					object.width  = entry.width;
					object.height = entry.height;
					object.hash   = entry.xxhash;
				}
				else
				{
					object.resize(0);
					object.width  = 0;
					object.height = 0;
					object.hash   = 0;
				}

				return process(entry, object);
			}

			Entry64 entries[1];

			static bool is64Bit()
			{
				return true;
			}
		};

		static const u64 MAGIC32 = 0xE8B675681037DE11;
		static const u64 MAGIC64 = 0xE8B675681037DE12;

		class IDatabase
		{
			public:
			virtual ~IDatabase()
			{
			}

			virtual Object* load(const u64 xxhash) = 0;
			virtual bool is64Bit() const	       = 0;

			virtual u64 size() const	       = 0;
			virtual Object* loadIndex(u64 i) const = 0;
		};

		template <class T> class Database : public Buffer, public IDatabase
		{
			public:
			Database(Buffer* buffer) : m_buffer(buffer), m_header((T*)(buffer->ptr()))
			{
				int y = 0;
			}

			Database() : m_buffer(nullptr), m_header(nullptr)
			{
			}

			Database(const std::string& path) : m_buffer(nullptr), m_header(nullptr)
			{
				load(path.c_str());
			}

			virtual ~Database()
			{
				if(m_buffer)
				{
					delete m_buffer;
					m_buffer = nullptr;
				}
			}

			Object* load(const u64 xxhash) override
			{
				if(!m_buffer || !m_buffer->ptr() || !m_header)
				{
					resize(0);
					return nullptr;
				}

				auto object = new Object();
				m_header->get(xxhash, *object, m_buffer->ptr());
				return object;
			}

			bool is64Bit() const override
			{
				return T::is64Bit();
			}

			u64 size() const override
			{
				if(m_header)
				{
					return m_header->entryCount;
				}
				return 0;
			}

			Object* loadIndex(u64 i) const override
			{
				if(!m_buffer || !m_buffer->ptr() || !m_header)
				{
					return nullptr;
				}

				auto object = new Object();
				m_header->getIndex(i, *object, m_buffer->ptr());
				return object;
			}

			protected:
			Buffer* m_buffer;
			T* m_header;
		};

		IDatabase* loadDb(const std::string& dbfile)
		{
			FILE* f = fopen(dbfile.c_str(), "rb");

			if(!f)
			{
				return nullptr;
			}

			fseek(f, 0, SEEK_END);

			u64 m_size = ftell(f);

			fseek(f, 0, SEEK_SET);

			if(!m_size)
			{
				fclose(f);
				return nullptr;
			}

			u64 allocSize = m_size;

			auto m_buffer = new Buffer(allocSize);

			if(!m_buffer)
			{
				fclose(f);
				return nullptr;
			}

			const u8* end = m_buffer->ptr() + m_size;
			u8* current   = m_buffer->ptr();

			while(current < end)
			{
				auto r = fread(current, 1, end - current, f);

				if(r <= 0)
				{
					fclose(f);
					return nullptr;
				}

				current += r;
			}

			fclose(f);

			auto header = (Header*)(m_buffer->ptr());

			if(header->magic == MAGIC32)
			{
				return new Database<Header32>(m_buffer);
			}
			else if(header->magic == MAGIC64)
			{
				return new Database<Header64>(m_buffer);
			}

			delete m_buffer;
			return nullptr;
		}

		class Databases
		{
			public:
			Databases()
			{
				std::vector<std::filesystem::path> paks;
#ifdef _MSC_VER
				loadDir("romfs/", paks);
#else
				loadDir("romfs:/", paks);
				loadDir("sdmc:/switch/sm64/", paks);
#endif

				std::sort(paks.begin(), paks.end());// , std::greater<std::filesystem::path>());

				for(const auto& path : paks)
				{
					auto db = std::unique_ptr<IDatabase>(loadDb(path.u8string()));

					for(u64 i = 0; i < db->size(); i++)
					{
						auto r = db->loadIndex(i);

						if(r && r->isValid())
						{
							m_map[r->hash] = std::move(std::unique_ptr<Object>(r));
						}
					}
				}

#ifdef _MSC_VER
				loadLooseDir("romfs", true);
#else
				loadLooseDir("sdmc:/switch/sm64/", true);
#endif
			}

			virtual ~Databases()
			{
			}

			void loadDir(const std::filesystem::path& path, std::vector<std::filesystem::path>& paks, u8 depth = 0)
			{
				try
				{
					for(const auto& entry : std::filesystem::directory_iterator(path))
					{
						if(entry.is_regular_file())
						{
							if(entry.path().extension() == ".pak")
							{
								paks.push_back(entry.path());
							}
						}
						else if(entry.is_directory())
						{
							if(config().mods().isEnabled(entry.path().filename().generic_u8string()))
							{
								loadDir(entry.path(), paks, depth+1);
							}
						}
					}
				}
				catch(...)
				{
				}
			}

			void loadLooseDir(const std::filesystem::path& path, bool rootDir)
			{
				static std::unordered_map<std::string, u64> defaultFileMapping = { {"bank_sets", (u64)hook::sound::Id::gBankSetsData}, {"sequences.bin", (u64)hook::sound::Id::gMusicData}, {"sound_data.ctl", (u64)hook::sound::Id::gSoundDataADSR}, {"sound_data.tbl", (u64)hook::sound::Id::gSoundDataRaw} };
				try
				{
					for(const auto& entry : std::filesystem::directory_iterator(path))
					{
						if(entry.is_regular_file())
						{
							auto ext = entry.path().extension();
							if(ext == ".gfx" || ext == ".geo" || ext == ".bhv" || ext == ".lvl" || ext == ".mcr" || ext == ".bin")
							{
								std::string hash = "";
								auto s		 = entry.path().u8string();

								for(char c : s)
								{
									if(c == ']')
									{
										if(hash.length() == 16)
										{
											u64 xxhash64 = std::stoull(hash, nullptr, 16);

											m_map[xxhash64] = std::move(std::unique_ptr<Object>(loadFileObject(s.c_str())));
										}
									}
									else if(c == '[')
									{
										hash = "";
									}
									else
									{
										hash += c;
									}
								}
							}
							else if(defaultFileMapping.count(entry.path().filename().string()))
							{
								auto s = entry.path().u8string();
								m_map[defaultFileMapping[entry.path().filename().string()]] = std::move(std::unique_ptr<Object>(loadFileObject(s.c_str())));
							}
						}
						else if(entry.is_directory())
						{
							if(config().mods().isEnabled(entry.path().filename().generic_u8string()))
							{
								loadLooseDir(entry.path(), false);
							}
						}
					}
				}
				catch(...)
				{
				}
			}

			Object* load(const u64 xxhash)
			{
				return m_map[xxhash].get();
			}

			Object* loadPartial(u64 xxhash, const u64 mask)
			{
				xxhash = xxhash & mask;

				for(auto it = m_map.begin(); it != m_map.end(); ++it)
				{
					if((it->first & mask) == xxhash)
					{
						return it->second.get();
					}
				}
				return nullptr;
			}

			protected:
			std::unordered_map<u64, std::unique_ptr<Object>> m_map;
		};

		static Databases* g_dbs = nullptr;

		static Databases& db()
		{
			if(!g_dbs)
			{
				g_dbs = new Databases();
			}
			return *g_dbs;
		}

		Object::Object() : Buffer(), width(0), height(0)
		{
		}

		Object::Object(const u64 xxhash) : Buffer(), width(0), height(0)
		{
		}

		Object::~Object()
		{
		}

		static u8 g_zeroes[0x1000] = { 0 };

		u64 Object::align(const u64 alignment)
		{
			u64 bytes = m_size % alignment;

			if(bytes)
			{
				append(&g_zeroes[0], bytes);
			}

			return bytes;
		}

		bool Object::isValid() const
		{
			return size();
		}

		Object* load(const u64 xxhash, const u64 mask)
		{
			if(mask == 0xFFFFFFFFFFFFFFFF)
			{
				return db().load(xxhash);
			}
			else
			{
				return db().loadPartial(xxhash, mask);
			}
		}

		Object* load(const u8* buffer, u64 size)
		{
			u64 xxhash = XXHash64::hash(buffer, size, 0);
			return load(xxhash);
		}

		static const u64 ASSET_MAGIC_MASK = 0xFFEF464F00000000;

		u8* mount(MountHeader* func, u64 size)
		{
			auto ptr = (u8*)func;

			for(u64 i = 0; i < func->entryCount; i++)
			{
				auto& entry = func->entries[i];
				auto& v = *(u64*)(ptr + entry.offset1);

#ifdef DEBUG
				if((v & 0xFFFFFFFF00000000) != ASSET_MAGIC_MASK)
				{
					int y = 0;
				}
#endif
				v = (u64)(ptr + entry.ptrOffset);
			}
			return nullptr;
		}
	} // namespace asset

	bool fileDump(const char* fileName, const void* buffer, u64 sz)
	{
		FILE* f = fopen(fileName, "wb+");

		if(f)
		{
			fwrite(buffer, sz, 1, f);
			fclose(f);
			return true;
		}
		return false;
	}
} // namespace sm64
