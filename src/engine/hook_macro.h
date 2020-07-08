#pragma once
#include "sm64.h"
#include "xxhash64.h"

namespace sm64::hook::macro
{
	enum Id : u64
	{
		NONE = 0,
		bbh_seg7_macro_objs = 0xa720dadfbd500b30,
		bitdw_seg7_macro_objs = 0x0f6e825b56ccdbaf,
		bitfs_seg7_macro_objs = 0x5a549950a7974338,
		bits_seg7_macro_objs = 0x14d5899230baca12,
		bob_seg7_macro_objs = 0x349e55139d7bba3c,
		castle_courtyard_seg7_macro_objs = 0x34c49d19df5b996e,
		castle_grounds_seg7_macro_objs = 0x630c1889ce934ec8,
		inside_castle_seg7_area_1_macro_objs = 0xee00a185a29f1213,
		inside_castle_seg7_area_2_macro_objs = 0x67f3bd08ad6f3457,
		inside_castle_seg7_area_3_macro_objs = 0x1d3c4d20dab9e2dc,
		ccm_seg7_area_1_macro_objs = 0xba410e61b8703596,
		ccm_seg7_area_2_macro_objs = 0x17ab8067d3599ba7,
		cotmc_seg7_macro_objs = 0xd4375d95c881cb27,
		ddd_seg7_area_1_macro_objs = 0xb51d284f54134d29,
		ddd_seg7_area_2_macro_objs = 0xeb2fd5e64e4faa23,
		hmc_seg7_macro_objs = 0x4500561879b3f1f0,
		jrb_seg7_area_1_macro_objs = 0x72fb46f180c35b01,
		jrb_seg7_area_2_macro_objs = 0x12bfd3b3a98a7528,
		lll_seg7_area_1_macro_objs = 0xd8a2e8dc73521c32,
		lll_seg7_area_2_macro_objs = 0xe5023b6b13c6b163,
		pss_seg7_macro_objs = 0x0fa3d82d8b949c24,
		rr_seg7_macro_objs = 0xd90471b2d431d2a5,
		sa_seg7_macro_objs = 0xc6f045aeef6ccc0a,
		sl_seg7_area_1_macro_objs = 0xa07788eae4b0f3bf,
		sl_seg7_area_2_macro_objs = 0xf02973cadb527904,
		ssl_seg7_area_1_macro_objs = 0x02f1592fb2395db7,
		ssl_seg7_area_2_macro_objs = 0x735993fb0405b439,
		ssl_seg7_area_3_macro_objs = 0xef46db3751d8e999,
		thi_seg7_area_1_macro_objs = 0x088850c6667a4882,
		thi_seg7_area_2_macro_objs = 0xba87fef18e0d91c0,
		thi_seg7_area_3_macro_objs = 0x649f78946af676f5,
		totwc_seg7_macro_objs = 0xaf0ec4e43b70f36d,
		ttc_seg7_macro_objs = 0x5db1d56e9343e959,
		ttm_seg7_area_1_macro_objs = 0xe59e4b730a7cb4d2,
		ttm_seg7_area_2_macro_objs = 0x76184a576831e089,
		ttm_seg7_area_3_macro_objs = 0xe4cab405a46f3f00,
		ttm_seg7_area_4_macro_objs = 0xe55b3fe8d88ddf2d,
		vcutm_seg7_macro_objs = 0xc8bd78070ad67724,
		wdw_seg7_area_1_macro_objs = 0xce3d3963edceb09e,
		wdw_seg7_area_2_macro_objs = 0x7ff86c8bea086e36,
		wf_seg7_macro_objs = 0x2010e28db35b478e,
		wmotr_seg7_macro_objs = 0x75c0c578c89fe96b,
	};

	void reg(const MacroObject* macros, u64 hash);
	u64 fingerprint(const MacroObject* macros);
	u64 fingerprint(const MacroObject* macros, XXHash64* state);
	const MacroObject* apply(const MacroObject* macros, const Id id);
}