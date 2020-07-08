#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

#include "common1.h"
#include "group6.h"

#include "monty_mole/geo.inc.c"
#include "ukiki/geo.inc.c"
#include "fwoosh/geo.inc.c"

#include "game/debug.h"

namespace sm64::geo
{
	EXPORT_GEO(monty_mole_geo);
	EXPORT_GEO(ukiki_geo);
	EXPORT_GEO(fwoosh_geo);
}
