#include "sm64.h"
#include "hook_bhv.h"
#include "hook_dl.h"
#include "asset.h"
#include <unordered_map>

namespace sm64::hook::bhv
{
	extern BhvCallback jumpTable[547];

	static std::unordered_map<const BehaviorScript*, u64>* g_map;

	static std::unordered_map<const BehaviorScript*, u64>& map()
	{
		if(!g_map)
		{
			g_map = new std::unordered_map<const BehaviorScript*, u64>();
		}

		return *g_map;
	}

	void reg(const BehaviorScript* bhv, u64 hash)
	{
		map()[bhv] = hash;
	}

	const BehaviorScript* apply(const BehaviorScript* func, const Id id)
	{
		u64 hash;
		try
		{
			auto hash = id != Id::NONE ? (u64)id : map().at(func);

			auto r = sm64::asset::load(hash);

			if(r && r->isValid())
			{
				return (BehaviorScript*)r->ptr();
			}
		}
		catch(...)
		{
			return func;
		}

		return func;
	}

	BhvCallback function(BhvCallback func)
	{
		if((u64)func < 0x1000)
		{
			return jumpTable[(u64)func];
		}

		return func;
	}

	BehaviorScript* mount(BehaviorScript* func, u64 size)
	{
		return func;
	}
} // namespace sm64::hook::bhv
