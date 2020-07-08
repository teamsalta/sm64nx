#include "types.h"
#include "engine/asset.h"

static const void* readAnim()
{
	auto obj = sm64::asset::load(0x3d768436333cfc20);

	if(obj)
	{
		return obj->ptr();
	}

	return nullptr;
}

const void* marioAnim()
{
	static const void* buffer = readAnim();

	return buffer;
}