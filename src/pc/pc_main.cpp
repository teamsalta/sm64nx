#include <stdlib.h>

#include "sm64.h"

#include "game/memory.h"
#include "audio/external.h"

#include "gfx/renderer.h"

#include "configfile.h"
#include "engine/math_util.h"
#include "game/debug.h"

#include <cstdio>
#include <filesystem>

#include "nx.h"
#include "gfx/fast64.h"
#include "audio/audio_api.h"
#include "xxhash64.h"

GfxDimensions gfx_current_dimensions;

OSMesg D_80339BEC;
OSMesgQueue gSIEventMesgQueue;

s8 gResetTimer;
s8 D_8032C648;

extern AudioAPI audio_sdl;
AudioAPI* audio_api = nullptr;

sm64::gfx::Fast64* g_fast64 = nullptr;

void send_display_list(struct SPTask *spTask);
void game_loop_one_iteration(void);
void dispatch_audio_sptask(struct SPTask *spTask);

extern void game_init(void *arg);


void dispatch_audio_sptask(struct SPTask *spTask)
{
}

static uint8_t inited = 0;


void send_display_list(struct SPTask *spTask)
{
	if(!inited)
	{
		return;
	}
	g_fast64->run((Gfx *)spTask->task.t.data_ptr);
}

#define printf

static void save_config(void)
{
}

bool verifyIntegrity()
{
	const static u64 ROM_SIZE = 0x800000;
	bool hasRom = false, hasPak = false;
	auto buffer = malloc(ROM_SIZE);

	if(!buffer)
	{
		return false;
	}
	
	for(const auto& entry : std::filesystem::directory_iterator("."))
	{
		if(entry.is_regular_file())
		{
			if(!hasRom && entry.path().extension() == ".z64" && entry.file_size() == ROM_SIZE)
			{
				FILE* f = fopen(entry.path().string().c_str(), "rb");

				if(f)
				{
					fread(buffer, 1, ROM_SIZE, f);
					fclose(f);

					if(XXHash64::hash(buffer, ROM_SIZE, 0) == 0xdaf4de8a170fdcbb)
					{
						hasRom = true;
						break;
					}
				}
			}
		}
	}

	for(const auto& entry : std::filesystem::directory_iterator("romfs"))
	{
		if(entry.is_regular_file())
		{
			if(entry.path().filename().string() == "!!base.pak")
			{
				hasPak = true;
				break;
			}
		}
	}

	if (!hasRom)
	{
		sm64::log("error: unable to locate Z64 rom.\n");
	}

	if (!hasPak)
	{
		sm64::log("error: unable to locate romfs/!!base.pak\n");
	}

	free(buffer);
	return hasRom && hasPak;
}

void main_func(void)
{
	sm64::log("initializing app\n");

	if(!verifyIntegrity())
	{
		return;
	}

	static u64 pool[0x165000 / 8 / 4 * sizeof(void *)];
	main_pool_init(pool, pool + sizeof(pool) / sizeof(pool[0]));
	gEffectsMemoryPool = mem_pool_init(0x4000, MEMORY_POOL_LEFT);

	g_fast64 = sm64::gfx::Fast64::createContext();

	audio_api = &audio_sdl;
	if(!audio_api->init())
	{
		return;
	}

	audio_init();
	sound_init();

	game_init(NULL);

	inited = 1;


	g_fast64->run_loop();

}

#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
#include <windows.h>
#include <shellscalingapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	main_func();
	return 0;
}
#else
#ifdef __SWITCH__
void initialize();
#endif

extern "C"
{
	int main(int argc, char *argv[]);
}

int main(int argc, char *argv[])
{
#ifdef __SWITCH__
	initialize();
#endif
	main_func();
	return 0;
}
#endif
