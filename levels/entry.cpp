#include <ultra64.h>
#include "sm64.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "levels/intro/header.h"

#include "make_const_nonconst.h"
#include "engine/level_script.h"
#include "game/debug.h"

const LevelScript level_script_entry[] = {
    INIT_LEVEL(),
    SLEEP(/*frames*/ 2),
    BLACKOUT(/*active*/ FALSE),
    SET_REG(/*value*/ 0),
    EXECUTE(/*seg*/ 0x14, /*script*/ _introSegmentRomStart, /*scriptEnd*/ _introSegmentRomEnd, /*entry*/ level_intro_entry_1),
    JUMP(/*target*/ level_script_entry),
};

/*
#define EXTERN_MACRO(X) extern const MacroObject X[];

EXTERN_MACRO(bbh_seg7_macro_objs);
EXTERN_MACRO(bitdw_seg7_macro_objs);
EXTERN_MACRO(bitfs_seg7_macro_objs);
EXTERN_MACRO(bits_seg7_macro_objs);
EXTERN_MACRO(bob_seg7_macro_objs);
EXTERN_MACRO(castle_courtyard_seg7_macro_objs);
EXTERN_MACRO(castle_grounds_seg7_macro_objs);
EXTERN_MACRO(inside_castle_seg7_area_1_macro_objs);
EXTERN_MACRO(inside_castle_seg7_area_2_macro_objs);
EXTERN_MACRO(inside_castle_seg7_area_3_macro_objs);
EXTERN_MACRO(ccm_seg7_area_1_macro_objs);
EXTERN_MACRO(ccm_seg7_area_2_macro_objs);
EXTERN_MACRO(cotmc_seg7_macro_objs);
EXTERN_MACRO(ddd_seg7_area_1_macro_objs);
EXTERN_MACRO(ddd_seg7_area_2_macro_objs);
EXTERN_MACRO(hmc_seg7_macro_objs);
EXTERN_MACRO(jrb_seg7_area_1_macro_objs);
EXTERN_MACRO(jrb_seg7_area_2_macro_objs);
EXTERN_MACRO(lll_seg7_area_1_macro_objs);
EXTERN_MACRO(lll_seg7_area_2_macro_objs);
EXTERN_MACRO(pss_seg7_macro_objs);
EXTERN_MACRO(rr_seg7_macro_objs);
EXTERN_MACRO(sa_seg7_macro_objs);
EXTERN_MACRO(sl_seg7_area_1_macro_objs);
EXTERN_MACRO(sl_seg7_area_2_macro_objs);
EXTERN_MACRO(ssl_seg7_area_1_macro_objs);
EXTERN_MACRO(ssl_seg7_area_2_macro_objs);
EXTERN_MACRO(ssl_seg7_area_3_macro_objs);
EXTERN_MACRO(thi_seg7_area_1_macro_objs);
EXTERN_MACRO(thi_seg7_area_2_macro_objs);
EXTERN_MACRO(thi_seg7_area_3_macro_objs);
EXTERN_MACRO(totwc_seg7_macro_objs);
EXTERN_MACRO(ttc_seg7_macro_objs);
EXTERN_MACRO(ttm_seg7_area_1_macro_objs);
EXTERN_MACRO(ttm_seg7_area_2_macro_objs);
EXTERN_MACRO(ttm_seg7_area_3_macro_objs);
EXTERN_MACRO(ttm_seg7_area_4_macro_objs);
EXTERN_MACRO(vcutm_seg7_macro_objs);
EXTERN_MACRO(wdw_seg7_area_1_macro_objs);
EXTERN_MACRO(wdw_seg7_area_2_macro_objs);
EXTERN_MACRO(wf_seg7_macro_objs);
EXTERN_MACRO(wmotr_seg7_macro_objs);

namespace sm64
{
	EXPORT_MACRO(bbh_seg7_macro_objs);
	EXPORT_MACRO(bitdw_seg7_macro_objs);
	EXPORT_MACRO(bitfs_seg7_macro_objs);
	EXPORT_MACRO(bits_seg7_macro_objs);
	EXPORT_MACRO(bob_seg7_macro_objs);
	EXPORT_MACRO(castle_courtyard_seg7_macro_objs);
	EXPORT_MACRO(castle_grounds_seg7_macro_objs);
	EXPORT_MACRO(inside_castle_seg7_area_1_macro_objs);
	EXPORT_MACRO(inside_castle_seg7_area_2_macro_objs);
	EXPORT_MACRO(inside_castle_seg7_area_3_macro_objs);
	EXPORT_MACRO(ccm_seg7_area_1_macro_objs);
	EXPORT_MACRO(ccm_seg7_area_2_macro_objs);
	EXPORT_MACRO(cotmc_seg7_macro_objs);
	EXPORT_MACRO(ddd_seg7_area_1_macro_objs);
	EXPORT_MACRO(ddd_seg7_area_2_macro_objs);
	EXPORT_MACRO(hmc_seg7_macro_objs);
	EXPORT_MACRO(jrb_seg7_area_1_macro_objs);
	EXPORT_MACRO(jrb_seg7_area_2_macro_objs);
	EXPORT_MACRO(lll_seg7_area_1_macro_objs);
	EXPORT_MACRO(lll_seg7_area_2_macro_objs);
	EXPORT_MACRO(pss_seg7_macro_objs);
	EXPORT_MACRO(rr_seg7_macro_objs);
	EXPORT_MACRO(sa_seg7_macro_objs);
	EXPORT_MACRO(sl_seg7_area_1_macro_objs);
	EXPORT_MACRO(sl_seg7_area_2_macro_objs);
	EXPORT_MACRO(ssl_seg7_area_1_macro_objs);
	EXPORT_MACRO(ssl_seg7_area_2_macro_objs);
	EXPORT_MACRO(ssl_seg7_area_3_macro_objs);
	EXPORT_MACRO(thi_seg7_area_1_macro_objs);
	EXPORT_MACRO(thi_seg7_area_2_macro_objs);
	EXPORT_MACRO(thi_seg7_area_3_macro_objs);
	EXPORT_MACRO(totwc_seg7_macro_objs);
	EXPORT_MACRO(ttc_seg7_macro_objs);
	EXPORT_MACRO(ttm_seg7_area_1_macro_objs);
	EXPORT_MACRO(ttm_seg7_area_2_macro_objs);
	EXPORT_MACRO(ttm_seg7_area_3_macro_objs);
	EXPORT_MACRO(ttm_seg7_area_4_macro_objs);
	EXPORT_MACRO(vcutm_seg7_macro_objs);
	EXPORT_MACRO(wdw_seg7_area_1_macro_objs);
	EXPORT_MACRO(wdw_seg7_area_2_macro_objs);
	EXPORT_MACRO(wf_seg7_macro_objs);
	EXPORT_MACRO(wmotr_seg7_macro_objs);
}
*/