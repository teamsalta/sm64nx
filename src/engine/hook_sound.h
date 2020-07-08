#pragma once
#include "types.h"

namespace sm64::hook::sound
{
	enum class Id : u64
	{
		gSoundDataADSR = 0x5a37c2a179f2cc9c,
		gSoundDataRaw = 0x8777c5b1b90e10f8,
		gMusicData = 0xcb0fbb49530b8b40,
		gBankSetsData = 0x2ef7f8f848ffcdca
	};
}
