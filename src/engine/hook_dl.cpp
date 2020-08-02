#include "sm64.h"
#include "asset.h"
#include "hook_dl.h"
#include "xxhash64.h"

#include "game/camera.h"
#include "game/level_geo.h"
#include "game/mario_misc.h"
#include "game/object_helpers.h"
#include "game/behavior_actions.h"
#include "game/mario_actions_cutscene.h"
#include "menu/star_select.h"
#include "game/screen_transition.h"
#include "game/geo_misc.h"
#include "menu/file_select.h"
#include "game/moving_texture.h"
#include "game/object_helpers2.h"
#include "game/paintings.h"
#include "menu/intro_geo.h"

Gfx* geo_rotate_coin(s32 callContext, GraphNode* node, Mat4* c);

static GraphNodeFunc g_graphCallbacks[] = {
    nullptr,
    (GraphNodeFunc)geo_act_selector_strings,
    (GraphNodeFunc)geo_camera_fov,
    (GraphNodeFunc)geo_camera_main,
    (GraphNodeFunc)geo_cannon_circle_base,
    (GraphNodeFunc)geo_enfvx_main,
    (GraphNodeFunc)EndingBGDraw,
    (GraphNodeFunc)ArajinWave,
    (GraphNodeFunc)ArajinInit,
    (GraphNodeFunc)SelRoomEffect,
    (GraphNodeFunc)geo_file_select_strings_and_menu_cursor,
    (GraphNodeFunc)geo_mario_hand_foot_scaler,
    (GraphNodeFunc)geo_mario_head_rotation,
    (GraphNodeFunc)geo_mario_rotate_wing_cap_wings,
    (GraphNodeFunc)geo_mario_tilt_torso,
    (GraphNodeFunc)geo_rotate_coin,
    (GraphNodeFunc)geo_mirror_mario_backface_culling,
    (GraphNodeFunc)geo_mirror_mario_set_alpha,
    (GraphNodeFunc)geo_move_mario_part_from_parent,
    (GraphNodeFunc)geo_movtex_draw_colored,
    (GraphNodeFunc)geo_movtex_draw_colored_2_no_update,
    (GraphNodeFunc)geo_movtex_draw_colored_no_update,
    (GraphNodeFunc)geo_movtex_draw_nocolor,
    (GraphNodeFunc)geo_movtex_draw_water_regions,
    (GraphNodeFunc)geo_movtex_pause_control,
    (GraphNodeFunc)geo_movtex_update_horizontal,
    (GraphNodeFunc)geo_render_mirror_mario,
    (GraphNodeFunc)ControlShapeAnime,
    (GraphNodeFunc)ControlSwitchMap,
    (GraphNodeFunc)geo_switch_bowser_eyes,
    (GraphNodeFunc)geo_switch_mario_cap_effect,
    (GraphNodeFunc)geo_switch_mario_cap_on_off,
    (GraphNodeFunc)geo_switch_mario_eyes,
    (GraphNodeFunc)geo_switch_mario_hand,
    (GraphNodeFunc)geo_switch_mario_hand_grab_pos,
    (GraphNodeFunc)geo_switch_mario_stand_run,
    (GraphNodeFunc)geo_switch_peach_eyes,
    (GraphNodeFunc)geo_switch_tuxie_mother_eyes,
    (GraphNodeFunc)AlphaControl,
    (GraphNodeFunc)CtrlHand,
    (GraphNodeFunc)geo_wdw_set_initial_water_level,
    (GraphNodeFunc)Geo18_802764B0,
    (GraphNodeFunc)CtrlCoord,
    (GraphNodeFunc)Geo18_802A719C,
    (GraphNodeFunc)KopaProc1,
    (GraphNodeFunc)Geo18_802B7D44,
    (GraphNodeFunc)Geo18_802BA2B0,
    (GraphNodeFunc)WaveMove,
    (GraphNodeFunc)WaveInit,
    (GraphNodeFunc)Geo18_8030D93C,
    (GraphNodeFunc)Geo18_8030D9AC,
    (GraphNodeFunc)geo18_fade_transition,
    (GraphNodeFunc)geo18_game_over_tile,
    (GraphNodeFunc)geo18_intro_backdrop,
    (GraphNodeFunc)geo18_title_screen,
};

namespace sm64::hook::dl
{
	u64 fingerprint(const Gfx* gfx, XXHash64* state)
	{
		const Gfx* start = gfx;

		static u64 zero = 0;

		while(1)
		{
			const u8 cmd = (gfx->words.w0 >> 24) & 0xFF;

			switch(cmd)
			{
				case G_VTX:
					state->add(&gfx->words.w0, sizeof(gfx->words.w0));
					state->add((const void*)gfx->words.w1, ((gfx->words.w0 >> 12) & 0xFF) * 24);
					break;
					/*case G_MTX:
						state.add(&gfx->words.w0, sizeof(gfx->words.w0));
						state.add((const void*)gfx->words.w1, ((gfx->words.w0 >> 12) & 0xFF) * 24);
						break;*/
				case G_TRI1:
				case G_TRI2:
				case G_QUAD:
					state->add(&gfx->words.w0, sizeof(gfx->words.w0));
					state->add(&gfx->words.w1, sizeof(gfx->words.w1));
					break;
					/*case G_SETCIMG:
					case G_SETZIMG:
					case G_SETTIMG:
						break;*/
				case G_DL:
				{
					state->add(&gfx->words.w0, sizeof(gfx->words.w0));
					const Gfx* ip = (const Gfx*)gfx->words.w1;

					if(ip != start)
					{
						fingerprint(ip, state);
					}
					else
					{
						state->add(&zero, sizeof(zero));
					}

					u8 push_flag = _SHIFTR(gfx->words.w0, 16, 8);

					if(push_flag)
					{
						return state->hash();
					}
				}
				break;
				case G_ENDDL:
					state->add(&gfx->words.w0, sizeof(gfx->words.w0));
					state->add(&gfx->words.w1, sizeof(gfx->words.w1));
					return state->hash();
				default:
					state->add(&gfx->words.w0, sizeof(gfx->words.w0));
					state->add(&zero, sizeof(zero));
			}
			++gfx;
		}
		return state->hash();
	}

	u64 size(const Gfx* gfx)
	{
		auto start = (const u8*)gfx;

		while(1)
		{
			const u8 cmd = (gfx->words.w0 >> 24) & 0xFF;

			gfx++;

			if(cmd == G_ENDDL)
			{
				break;
			}
		}

		return (const u8*)gfx - start;
	}

	/*sm64::asset::Object export(const Gfx* src)
	{
		static u64 zeroes[2] = { 0, 0 };
		sm64::asset::Object obj;
		obj.resize(size(src));
		memcpy(obj.ptr, src, obj.size);

		auto gfx = (Gfx*)obj.ptr;

		sm64::asset::Object payload;

		while(1)
		{
			const u8 cmd = (gfx->words.w0 >> 24) & 0xFF;

			switch(cmd)
			{
				case G_VTX:
				{
					const u64 offset = payload.size;

					payload.append((const u8*)gfx->words.w1, ((gfx->words.w0 >> 12) & 0xFF) * 24);
					payload.align(0x10);

					gfx->words.w1 = sm64::ASSET_MAGIC_MASK & (obj.size + offset);
					break;
				}
				case G_MTX:
				{
					const u64 offset = payload.size;

					payload.append((const u8*)gfx->words.w1, sizeof(float[4][4]));
					payload.align(0x10);

					gfx->words.w1 = sm64::ASSET_MAGIC_MASK & (obj.size + offset);
					break;
				}
				case G_DL:
				{
					Gfx* ip = (Gfx*)gfx->words.w1;

					gfx->words.w1 = (uintptr_t)fingerprint(ip) & sm64::ASSET_HASH_MAGIC_MASK;


					u8 push_flag = _SHIFTR(gfx->words.w0, 16, 8);

					if(push_flag)
					{
						break;
					}
				}
				break;
				case G_ENDDL:
					break;
			}
			++gfx;
		}

		return obj;
	}*/

	u64 fingerprint(const Gfx* gfx)
	{
		XXHash64 state(0);
		return fingerprint(gfx, &state);
	}

	GraphNodeFunc apply(GraphNodeFunc func)
	{
		u16 index	    = sizeof(g_graphCallbacks) / sizeof(g_graphCallbacks[0]);
		const u64 funcIndex = (u64)func;
		if(funcIndex && funcIndex < index)
		{
			const auto r = g_graphCallbacks[funcIndex];
			return r;
		}

		return func;
	}

	Gfx* apply(Gfx* gfx)
	{
		if(!gfx)
		{
			return nullptr;
		}

		const auto hash = fingerprint(gfx);

		auto r = sm64::asset::load(hash);

		if(r)
		{
			return (Gfx*)r->ptr();
		}

		return gfx;
	}

	void mount(Gfx* gfx)
	{
		const Gfx* start = gfx;

		static u64 zero = 0;

		while(1)
		{
			const u8 cmd = (gfx->words.w0 >> 24) & 0xFF;

			switch(cmd)
			{
				/*case G_VTX:
					mount((Gfx*)gfx->words.w1);
					break;
				case G_TRI1:
				case G_TRI2:
				case G_QUAD:
					break;*/
				case G_DL:
				{
					Gfx* ip = apply((Gfx*)gfx->words.w1);

					if(ip != start)
					{
						mount(ip);
					}
					else
					{
					}

					u8 push_flag = _SHIFTR(gfx->words.w0, 16, 8);

					if(push_flag)
					{
						return;
					}
				}
				break;
				case G_ENDDL:
					return;
			}
			++gfx;
		}
		return;
	}
} // namespace sm64::hook::dl
