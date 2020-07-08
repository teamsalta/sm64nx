#pragma once
#include <ultra64.h>
#include "sm64.h"
#include <vector>
#include "xxhash64.h"

namespace sm64::hook::level
{
	class Level
	{
	public:
		Level(const LevelCommand* level, XXHash64* s);
		u64 fingerprint();
		void hashOp();

		void load_and_execute();
		void exit_and_execute();
		void exit();
		void sleep();
		void sleep2();
		void jump();
		void jump_and_link();
		void ret();
		void jump_and_link_push_arg();
		void jump_repeat();
		void loop_begin();
		void loop_until();
		void jump_if();
		void jump_and_link_if();
		void skip_if();
		void skip();
		void skippable_nop();
		void call();
		void call_loop();
		void set_register();
		void push_pool_state();
		void pop_pool_state();
		void load_to_fixed_address();
		void load_raw();
		void load_mio0();
		void load_mario_head();
		void load_mio0_texture();
		void init_level();
		void clear_level();
		void alloc_level_pool();
		void free_level_pool();
		void begin_area();
		void end_area();
		void load_model_from_dl();
		void load_model_from_geo();
		void cmd_23();
		void place_object();
		void init_mario();
		void create_warp_node();
		void create_painting_warp_node();
		void create_instant_warp();
		void load_area();
		void cmd_2A();
		void set_mario_start_pos();
		void cmd_2C();
		void cmd_2D();
		void set_terrain_data();
		void set_rooms();
		void show_dialog();
		void set_terrain_type();
		void nop();
		void set_transition();
		void set_blackout();
		void set_gamma();
		void set_music();
		void set_menu_music();
		void cmd_38();
		void set_macro_objects();
		void cmd_3A();
		void create_whirlpool();
		void get_or_set_var();
		void advdemo();
		void cleardemoptr();


	protected:
		template<class T> T add(const T& n)
		{
			state->add(&n, sizeof(n));
			return n;
		}

		const LevelCommand* ip;
		const LevelCommand* start;
		XXHash64* state;
	};

	void reg(const LevelCommand* level, u64 hash);
	u64 fingerprint(const LevelCommand* level, XXHash64* state);
	u64 fingerprint(const LevelCommand* layout);
	LevelCommand* apply(LevelCommand* func);
	LevelCommand* mount(LevelCommand* func, u64 size);
}