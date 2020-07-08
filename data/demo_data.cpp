#include "types.h"
#include "engine/asset.h"


static const void* readAnim()
{
	auto obj = sm64::asset::load(0xf6eee390de43389b);

	if(obj)
	{
		return obj->ptr();
	}

	return nullptr;
}

const void* demoInputs()
{
	static const void* buffer = readAnim();

	return buffer;
}

