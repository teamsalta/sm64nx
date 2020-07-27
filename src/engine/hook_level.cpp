#include "sm64.h"
#include "hook_level.h"
#include <string.h>
#include "asset.h"
#include "hook_geo.h"
#include "hook_dl.h"
#include <unordered_map>

namespace sm64::hook::level
{
#define CMD_SIZE_SHIFT (sizeof(void*) >> 3)
#define CMD_PROCESS_OFFSET(offset) ((offset & 3) | ((offset & ~3) << CMD_SIZE_SHIFT))

	u64 cmdSizeShift()
	{
		return (sizeof(void*) >> 3);
	}

	u64 cmdProcessOffset(const u64 offset)
	{
		return ((offset & 3) | ((offset & ~3) << cmdSizeShift()));
	}

#define CMD_GET(type, offset) (*(type*)(CMD_PROCESS_OFFSET(offset) + (u8*)ip))

	void* cmdGet(u8* ip, u64 offset)
	{
		u8* r = ip + cmdProcessOffset(offset) + 6;
		return *(void**)r;
	}

	// These are equal
#define CMD_NEXT ((struct LevelCommand*)((u8*)ip + (ip->size << CMD_SIZE_SHIFT)))
#define NEXT_CMD ((struct LevelCommand*)((ip->size << CMD_SIZE_SHIFT) + (u8*)ip))

	Level::Level(const LevelCommand* level, XXHash64* s) : ip(level), start(level), state(s)
	{
	}

	void Level::load_and_execute()
	{
		// ip = (LevelCommand*)CMD_GET(void*, 12);
		ip = NEXT_CMD;
	}

	void Level::exit_and_execute()
	{
		auto targetAddr = (const LevelCommand*)CMD_GET(void*, 12);

		level::fingerprint(targetAddr, state);
		ip = nullptr;
	}

	void Level::exit()
	{
		hashOp();
		ip = nullptr;
	}

	void Level::sleep()
	{
		hashOp();
		// CMD_GET(s16, 2)
		ip = CMD_NEXT;
	}

	void Level::sleep2()
	{
		hashOp();
		// CMD_GET(s16, 2)

		ip = CMD_NEXT;
	}

	void Level::jump()
	{
		auto next = (LevelCommand*)CMD_GET(void*, 4);

		if(next >= start && next < ip)
		{
			ip = nullptr;
		}
		else
		{
			ip = next;
		}
	}

	void Level::jump_and_link()
	{
		// ip = (LevelCommand*)segmented_to_virtual(CMD_GET(void *, 4));
		ip = NEXT_CMD;
	}

	void Level::ret()
	{
		hashOp();
		ip = nullptr;
	}

	void Level::jump_and_link_push_arg()
	{
		hashOp();
		//*sStackTop++ = CMD_GET(s16, 2);
		ip = CMD_NEXT;
	}

	void Level::jump_repeat()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::loop_begin()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::loop_until()
	{
		// if(eval_script_op(CMD_GET(u8, 2), CMD_GET(s32, 4)) != 0)

		ip = CMD_NEXT;
	}

	void Level::jump_if()
	{
		/*if(eval_script_op(CMD_GET(u8, 2), CMD_GET(s32, 4)) != 0)
		{
			ip = (LevelCommand*)segmented_to_virtual(CMD_GET(void *, 8));
		}*/
		ip = CMD_NEXT;
	}

	void Level::jump_and_link_if()
	{
		/*if(eval_script_op(CMD_GET(u8, 2), CMD_GET(s32, 4)) != 0)
		{
			*sStackTop++ = (uintptr_t)NEXT_CMD;
			ip = (LevelCommand*)segmented_to_virtual(CMD_GET(void *, 8));
		}*/

		ip = CMD_NEXT;
	}

	void Level::skip_if()
	{
		/*if(eval_script_op(CMD_GET(u8, 2), CMD_GET(s32, 4)) == 0)
		{
			do
			{
				ip = CMD_NEXT;
			}
			while(ip->type == 0x0F || ip->type == 0x10);
		}*/

		ip = CMD_NEXT;
	}

	void Level::skip()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::skippable_nop()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::call()
	{
		/*typedef s32(*Func)(s16, s32);
		Func func = CMD_GET(Func, 4);
		sRegister = func(CMD_GET(s16, 2), sRegister);*/
		ip = CMD_NEXT;
	}

	void Level::call_loop()
	{
		// typedef s32(*Func)(s16, s32);
		// Func func = CMD_GET(Func, 4);

		ip = CMD_NEXT;
	}

	void Level::set_register()
	{
		hashOp();
		// sRegister = CMD_GET(s16, 2);
		ip = CMD_NEXT;
	}

	void Level::push_pool_state()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::pop_pool_state()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::load_to_fixed_address()
	{
		// load_to_fixed_pool_addr(CMD_GET(void *, 4), CMD_GET(void *, 8), CMD_GET(void *, 12));
		ip = CMD_NEXT;
	}

	void Level::load_raw()
	{
		// load_segment(CMD_GET(s16, 2), CMD_GET(void *, 4), CMD_GET(void *, 8), MEMORY_POOL_LEFT);
		ip = CMD_NEXT;
	}

	void Level::load_mio0()
	{
		// load_segment_decompress(CMD_GET(s16, 2), CMD_GET(void *, 4), CMD_GET(void *, 8));
		ip = CMD_NEXT;
	}

	void Level::load_mario_head()
	{
		// TODO: Fix these hardcoded sizes
		/*void *addr = main_pool_alloc(DOUBLE_SIZE_ON_64_BIT(0xE1000), MEMORY_POOL_LEFT);
		if(addr != NULL)
		{
			gdm_init(addr, DOUBLE_SIZE_ON_64_BIT(0xE1000));
			gd_add_to_heap(gZBuffer, sizeof(gZBuffer)); // 0x25800
			gd_add_to_heap(gFrameBuffer0, 3 * sizeof(gFrameBuffer0)); // 0x70800
			gdm_setup();
			gdm_maketestdl(CMD_GET(s16, 2));
		}
		else
		{
		}*/

		ip = CMD_NEXT;
	}

	void Level::load_mio0_texture()
	{
		// func_80278304(CMD_GET(s16, 2), CMD_GET(void *, 4), CMD_GET(void *, 8));
		ip = CMD_NEXT;
	}

	void Level::init_level()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::clear_level()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::alloc_level_pool()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::free_level_pool()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::begin_area()
	{
		// u8 areaIndex = CMD_GET(u8, 2);
		// auto geoLayoutAddr = (GeoLayout*)CMD_GET(void *, 4);

		ip = CMD_NEXT;
	}

	void Level::end_area()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::load_model_from_dl()
	{
		/*s16 val1 = CMD_GET(s16, 2) & 0x0FFF;
		s16 val2 = CMD_GET(u16, 2) >> 12;*/
		auto gfx = (const Gfx*)CMD_GET(void*, 4);
		dl::fingerprint(gfx, state);
		ip = CMD_NEXT;
	}

	void Level::load_model_from_geo()
	{
		// s16 arg0 = CMD_GET(s16, 2);
		auto geo = (const GeoLayout*)CMD_GET(void*, 4);
		geo::fingerprint(geo, state);

		ip = CMD_NEXT;
	}

	void Level::cmd_23()
	{
		/*s16 model = CMD_GET(s16, 2) & 0x0FFF;
		s16 arg0H = CMD_GET(u16, 2) >> 12;
		void *arg1 = CMD_GET(void *, 4);

		auto i = CMD_GET(s32, 8);*/

		ip = CMD_NEXT;
	}

	void Level::init_mario()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::place_object()
	{
		/*spawnInfo->startPos[0] = CMD_GET(s16, 4);
		spawnInfo->startPos[1] = CMD_GET(s16, 6);
		spawnInfo->startPos[2] = CMD_GET(s16, 8);

		spawnInfo->startAngle[0] = CMD_GET(s16, 10) * 0x8000 / 180;
		spawnInfo->startAngle[1] = CMD_GET(s16, 12) * 0x8000 / 180;
		spawnInfo->startAngle[2] = CMD_GET(s16, 14) * 0x8000 / 180;

		spawnInfo->areaIndex = sCurrAreaIndex;
		spawnInfo->activeAreaIndex = sCurrAreaIndex;

		spawnInfo->behaviorArg = CMD_GET(u32, 16);
		spawnInfo->behaviorScript = CMD_GET(void *, 20);*/

		ip = CMD_NEXT;
	}

	void Level::create_warp_node()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::create_instant_warp()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_terrain_type()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::create_painting_warp_node()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::cmd_3A()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::create_whirlpool()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_blackout()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_gamma()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_terrain_data()
	{
		// data = (Collision*)segmented_to_virtual(CMD_GET(void *, 4));
		// size = get_area_terrain_size(data) * sizeof(Collision);

		ip = CMD_NEXT;
	}

	void Level::set_rooms()
	{
		// stageScenes[sCurrAreaIndex].surfaceRooms = (s8*)segmented_to_virtual(CMD_GET(void *, 4));
		ip = CMD_NEXT;
	}

	void Level::set_macro_objects()
	{
		/*MacroObject *data = (MacroObject*)segmented_to_virtual(CMD_GET(void *, 4));
		s32 len = 0;
		while(data[len++] != 0x001E)
		{
			len += 4;
		}
		stageScenes[sCurrAreaIndex].macroObjects = (s16*)alloc_only_pool_alloc(sLevelPool, len * sizeof(MacroObject));
		memcpy(stageScenes[sCurrAreaIndex].macroObjects, data, len * sizeof(MacroObject));*/

		ip = CMD_NEXT;
	}

	void Level::SnOpenScene()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::cmd_2A()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_mario_start_pos()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::cmd_2C()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::cmd_2D()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_transition()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::nop()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::show_dialog()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_music()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::set_menu_music()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::cmd_38()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::get_or_set_var()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::advdemo()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	void Level::cleardemoptr()
	{
		hashOp();
		ip = CMD_NEXT;
	}

	static void (Level::*jumpTable[])() = {
	    /*00*/ &Level::load_and_execute,
	    /*01*/ &Level::exit_and_execute,
	    /*02*/ &Level::exit,
	    /*03*/ &Level::sleep,
	    /*04*/ &Level::sleep2,
	    /*05*/ &Level::jump,
	    /*06*/ &Level::jump_and_link,
	    /*07*/ &Level::ret,
	    /*08*/ &Level::jump_and_link_push_arg,
	    /*09*/ &Level::jump_repeat,
	    /*0A*/ &Level::loop_begin,
	    /*0B*/ &Level::loop_until,
	    /*0C*/ &Level::jump_if,
	    /*0D*/ &Level::jump_and_link_if,
	    /*0E*/ &Level::skip_if,
	    /*0F*/ &Level::skip,
	    /*10*/ &Level::skippable_nop,
	    /*11*/ &Level::call,
	    /*12*/ &Level::call_loop,
	    /*13*/ &Level::set_register,
	    /*14*/ &Level::push_pool_state,
	    /*15*/ &Level::pop_pool_state,
	    /*16*/ &Level::load_to_fixed_address,
	    /*17*/ &Level::load_raw,
	    /*18*/ &Level::load_mio0,
	    /*19*/ &Level::load_mario_head,
	    /*1A*/ &Level::load_mio0_texture,
	    /*1B*/ &Level::init_level,
	    /*1C*/ &Level::clear_level,
	    /*1D*/ &Level::alloc_level_pool,
	    /*1E*/ &Level::free_level_pool,
	    /*1F*/ &Level::begin_area,
	    /*20*/ &Level::end_area,
	    /*21*/ &Level::load_model_from_dl,
	    /*22*/ &Level::load_model_from_geo,
	    /*23*/ &Level::cmd_23,
	    /*24*/ &Level::place_object,
	    /*25*/ &Level::init_mario,
	    /*26*/ &Level::create_warp_node,
	    /*27*/ &Level::create_painting_warp_node,
	    /*28*/ &Level::create_instant_warp,
	    /*29*/ &Level::SnOpenScene,
	    /*2A*/ &Level::cmd_2A,
	    /*2B*/ &Level::set_mario_start_pos,
	    /*2C*/ &Level::cmd_2C,
	    /*2D*/ &Level::cmd_2D,
	    /*2E*/ &Level::set_terrain_data,
	    /*2F*/ &Level::set_rooms,
	    /*30*/ &Level::show_dialog,
	    /*31*/ &Level::set_terrain_type,
	    /*32*/ &Level::nop,
	    /*33*/ &Level::set_transition,
	    /*34*/ &Level::set_blackout,
	    /*35*/ &Level::set_gamma,
	    /*36*/ &Level::set_music,
	    /*37*/ &Level::set_menu_music,
	    /*38*/ &Level::cmd_38,
	    /*39*/ &Level::set_macro_objects,
	    /*3A*/ &Level::cmd_3A,
	    /*3B*/ &Level::create_whirlpool,
	    /*3C*/ &Level::get_or_set_var,
	    /*3D*/ &Level::advdemo,
	    /*3E*/ &Level::cleardemoptr,
	};

	void Level::hashOp()
	{
		state->add(ip, ip->size << CMD_SIZE_SHIFT);
	}

	u64 Level::fingerprint()
	{
		while(ip != NULL)
		{
			if(ip->type >= sizeof(jumpTable) / sizeof(jumpTable[0]))
			{
				break;
			}
			auto func = jumpTable[ip->type];
			(this->*func)();
		}

		return state->hash();
	}

	static std::unordered_map<const LevelCommand*, u64>* g_map;

	static std::unordered_map<const LevelCommand*, u64>& map()
	{
		if(!g_map)
		{
			g_map = new std::unordered_map<const LevelCommand*, u64>();
		}

		return *g_map;
	}

	void reg(const LevelCommand* level, u64 hash)
	{
		map()[level] = hash;
	}

	u64 fingerprint(const LevelCommand* level, XXHash64* state)
	{
		return 0;
		Level l(level, state);
		return l.fingerprint();
	}

	u64 fingerprint(const LevelCommand* layout)
	{
		XXHash64 state(0);
		return fingerprint(layout, &state);
	}

	LevelCommand* apply(LevelCommand* func)
	{
		u64 hash;
		try
		{
			auto hash = map().at(func);

			auto r = sm64::asset::load(hash);

			if(r && r->isValid())
			{
				return (LevelCommand*)r->ptr();
			}
		}
		catch(...)
		{
			return func;
		}

		return func;
	}

	LevelCommand* mount(LevelCommand* func, u64 size)
	{
		return func;
		// return (LevelCommand*)asset::mount((u8*)func, size);
	}
} // namespace sm64::hook::level
