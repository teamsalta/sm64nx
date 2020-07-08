#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/area.h"
#include "game/level_update.h"
#include "menu/level_select_menu.h"

#include "levels/scripts.h"
#include "levels/menu/header.h"

#include "actors/common1.h"

#include "make_const_nonconst.h"
#include "levels/intro/header.h"

#define FORCE_DEMO

const LevelScript level_intro_entry_1[] = {
    INIT_LEVEL(),
    FIXED_LOAD(/*loadAddr*/ _goddardSegmentStart, /*romStart*/ _goddardSegmentRomStart, /*romEnd*/ _goddardSegmentRomEnd),
    LOAD_RAW(/*seg*/ 0x13, _behaviorSegmentRomStart, _behaviorSegmentRomEnd),
    LOAD_MIO0(/*seg*/ 0x07, _intro_segment_7SegmentRomStart, _intro_segment_7SegmentRomEnd),
    ALLOC_LEVEL_POOL(),

    AREA(/*index*/ 1, intro_geo_0002D0),
    END_AREA(),

    FREE_LEVEL_POOL(),
    LOAD_AREA(/*area*/ 1),
    CALL(/*arg*/ 0, /*func*/ LevelProc_8016F508), // play its me mario sound
    SLEEP(/*frames*/ 75),
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_INTO_COLOR, /*time*/ 16, /*color*/ 0x00, 0x00, 0x00),
    SLEEP(/*frames*/ 16),
    CMD2A(/*forwardSpeed*/ 1),
    CLEAR_LEVEL(),
    SLEEP(/*frames*/ 2),
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _introSegmentRomStart, _introSegmentRomEnd, level_intro_entry_2),
};

const LevelScript level_intro_entry_2[] = {
	INIT_LEVEL(),
	BLACKOUT(/*active*/ TRUE),
	FIXED_LOAD(/*loadAddr*/ _goddardSegmentStart, /*romStart*/ _goddardSegmentRomStart, /*romEnd*/ _goddardSegmentRomEnd),
#ifndef FORCE_DEMO
	LOAD_MARIO_HEAD(/*loadHeadID*/ REGULAR_FACE),
#endif
	LOAD_RAW(/*seg*/ 0x13, _behaviorSegmentRomStart, _behaviorSegmentRomEnd),
	LOAD_MIO0_TEXTURE(/*seg*/ 0x0A, _title_screen_bg_mio0SegmentRomStart, _title_screen_bg_mio0SegmentRomEnd),
	ALLOC_LEVEL_POOL(),

#ifndef FORCE_DEMO
	AREA(/*index*/ 1, intro_geo_00035C),
	END_AREA(),
#endif

	FREE_LEVEL_POOL(),
	SLEEP(/*frames*/ 2),
	BLACKOUT(/*active*/ FALSE),
#ifndef FORCE_DEMO
	LOAD_AREA(/*area*/ 1),
#endif
	CLEAR_DEMO_PTR(), // we need to do this or else file select will be tainted with inputs
	GET_OR_SET(/*op*/ OP_GET, /*var*/ 5),
	JUMP_IF(/*op*/ OP_EQ, /*arg*/ 1, script_intro_L1), // was start pressed when demo ended last time?
	SET_MENU_MUSIC(/*seq*/ 0x0002),
	TRANSITION(/*transType*/ WARP_TRANSITION_FADE_FROM_STAR, /*time*/ 20, /*color*/ 0x00, 0x00, 0x00),
	SLEEP(/*frames*/ 20),
#ifndef FORCE_DEMO
	CALL_LOOP(/*arg*/ 1, /*func*/ LevelProc_8016F508), // mario face loop
	JUMP_IF(/*op*/ OP_EQ, /*arg*/ 100, script_intro_L1),
	JUMP_IF(/*op*/ OP_EQ, /*arg*/ 101, script_intro_L2),
	ADV_DEMO(), // advance the demo ID manually with a new command which sets the level as script register
	JUMP(script_intro_L4),
#else
	ADV_DEMO(), // advance the demo ID manually with a new command which sets the level as script register
	JUMP(script_intro_L4), // go to ingame
#endif
};

const LevelScript level_intro_entry_3[] = {
    JUMP(level_intro_entry_2),
};

const LevelScript level_intro_entry_4[] = {
    INIT_LEVEL(),
    LOAD_RAW(/*seg*/ 0x13, _behaviorSegmentRomStart, _behaviorSegmentRomEnd),
    LOAD_MIO0_TEXTURE(/*seg*/ 0x0A, _title_screen_bg_mio0SegmentRomStart, _title_screen_bg_mio0SegmentRomEnd),
    LOAD_MIO0(/*seg*/ 0x07, _debug_level_select_mio0SegmentRomStart, _debug_level_select_mio0SegmentRomEnd),
    FIXED_LOAD(/*loadAddr*/ _goddardSegmentStart, /*romStart*/ _goddardSegmentRomStart, /*romEnd*/ _goddardSegmentRomEnd),
    ALLOC_LEVEL_POOL(),

    AREA(/*index*/ 1, intro_level_geo),
    END_AREA(),

    FREE_LEVEL_POOL(),
    LOAD_AREA(/*area*/ 1),
    SET_MENU_MUSIC(/*seq*/ 0x0002),
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_FROM_COLOR, /*time*/ 16, /*color*/ 0xFF, 0xFF, 0xFF),
    SLEEP(/*frames*/ 16),
    CALL_LOOP(/*arg*/ 3, /*func*/ LevelProc_8016F508), // input loop level select
    JUMP_IF(/*op*/ OP_EQ, /*arg*/ -1, script_intro_L5),
    JUMP(script_intro_L3),
};

// These should be static, but C doesn't allow non-sized forward declarations of static arrays

const LevelScript script_intro_L1[] = {
    STOP_MUSIC(/*fadeOutTime*/ 0x00BE),
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_INTO_COLOR, /*time*/ 16, /*color*/ 0xFF, 0xFF, 0xFF),
    SLEEP(/*frames*/ 16),
    CLEAR_LEVEL(),
    SLEEP(/*frames*/ 2),
    SET_REG(/*value*/ 16),
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _menuSegmentRomStart, _menuSegmentRomEnd, level_main_menu_entry_1),
};

const LevelScript script_intro_L2[] = {
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_INTO_COLOR, /*time*/ 16, /*color*/ 0xFF, 0xFF, 0xFF),
    SLEEP(/*frames*/ 16),
    CLEAR_LEVEL(),
    SLEEP(/*frames*/ 2),
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _introSegmentRomStart, _introSegmentRomEnd, level_intro_entry_4),
};

const LevelScript script_intro_L3[] = {
    STOP_MUSIC(/*fadeOutTime*/ 0x00BE),
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_INTO_COLOR, /*time*/ 16, /*color*/ 0xFF, 0xFF, 0xFF),
    SLEEP(/*frames*/ 16),
    CLEAR_LEVEL(),
    SLEEP(/*frames*/ 2),
    EXIT_AND_EXECUTE(/*seg*/ 0x15, _scriptsSegmentRomStart, _scriptsSegmentRomEnd, level_main_scripts_entry),
};

const LevelScript script_intro_L4[] = {
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_INTO_COLOR, /*time*/ 16, /*color*/ 0xFF, 0xFF, 0xFF),
    SLEEP(/*frames*/ 16),
    CLEAR_LEVEL(),
    SLEEP(/*frames*/ 2),
    EXIT_AND_EXECUTE(/*seg*/ 0x15, _scriptsSegmentRomStart, _scriptsSegmentRomEnd, level_main_scripts_entry),
};

const LevelScript script_intro_L5[] = {
    STOP_MUSIC(/*fadeOutTime*/ 0x00BE),
    TRANSITION(/*transType*/ WARP_TRANSITION_FADE_INTO_COLOR, /*time*/ 16, /*color*/ 0x00, 0x00, 0x00),
    SLEEP(/*frames*/ 16),
    CLEAR_LEVEL(),
    SLEEP(/*frames*/ 2),
    EXIT_AND_EXECUTE(/*seg*/ 0x14, _introSegmentRomStart, _introSegmentRomEnd, level_intro_entry_1),
};

#include "game/debug.h"
namespace sm64
{
	EXPORT_LEVEL(level_intro_entry_1);
	EXPORT_LEVEL(level_intro_entry_2);
	EXPORT_LEVEL(level_intro_entry_3);
	EXPORT_LEVEL(level_intro_entry_4);
	EXPORT_LEVEL(script_intro_L1);
	EXPORT_LEVEL(script_intro_L2);
	EXPORT_LEVEL(script_intro_L3);
	EXPORT_LEVEL(script_intro_L4);
	EXPORT_LEVEL(script_intro_L5);
}
