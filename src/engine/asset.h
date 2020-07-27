#pragma once

#include <memory>

namespace sm64
{
	static const u64 ASSET_MAGIC_MASK      = 0xFFEF464F00000000;
	static const u64 ASSET_HASH_MAGIC_MASK = 0xFEEF464F00000000;

	class Buffer
	{
		public:
		Buffer();
		Buffer(const u64 sz);
		virtual ~Buffer();
		void resize(const u64 size, bool preserveData = true);
		void free();
		void append(const u8* buffer, const u64 size);
		u64 deflateZstd(const u8* buffer, const u64 size);

		u8* ptr() const;
		u64 size() const;
		void shift(u64 offset);

		protected:
		u8* m_ptr;
		u64 m_size;
		u64 m_allocSize;
		u64 m_offset;
	};

	namespace asset
	{
		class Object : public Buffer
		{
			public:
			Object();
			Object(const u64 xxhash);
			virtual ~Object();
			u64 align(const u64 alignment = 0x10);

			bool isValid() const;

			u64 hash;
			u64 width;
			u64 height;
		};

		Object* load(const u64 xxhash, const u64 mask = 0xFFFFFFFFFFFFFFFF);
		Object* load(const u8* buffer, u64 size);
	} // namespace asset

	bool fileDump(const char* fileName, const void* buffer, u64 sz);

} // namespace sm64
