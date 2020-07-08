#include "sm64.h"
#include "hook_macro.h"
#include "asset.h"
#include <unordered_map>
#include "level_misc_macros.h"

namespace sm64::hook::macro
{
	static std::unordered_map<const MacroObject*, u64>* g_macroMap;
	static std::unordered_map<u64, const MacroObject*>* g_idMap;

	static std::unordered_map<const MacroObject*, u64>& macroMap()
	{
		if(!g_macroMap)
		{
			g_macroMap = new std::unordered_map<const MacroObject*, u64>();
		}

		return *g_macroMap;
	}

	static std::unordered_map<u64, const MacroObject*>& idMap()
	{
		if(!g_idMap)
		{
			g_idMap = new std::unordered_map<u64, const MacroObject*>();
		}

		return *g_idMap;
	}

	void reg(const MacroObject* macros, u64 hash)
	{
		macroMap()[macros] = hash;
		idMap()[hash]	   = macros;
	}

	u64 fingerprint(const MacroObject* macros)
	{
		XXHash64 state(0);
		return fingerprint(macros, &state);
	}

	u64 fingerprint(const MacroObject* macros, XXHash64* state)
	{
		auto ptr = macros;

		while(ptr && *ptr != MACRO_OBJECT_END())
		{
			state->add(ptr, sizeof(*ptr));
			ptr++;
		}

		return state->hash();
	}

	const MacroObject* apply(const MacroObject* macro, const Id id)
	{
		u64 hash;
		if(id == Id::NONE)
		{
			try
			{
				auto hash = macroMap().at((const MacroObject*)macro);

				auto r = sm64::asset::load(hash);

				if(r && r->isValid())
				{
					return (MacroObject*)r->ptr();
				}
			}
			catch(...)
			{
				return macro;
			}
		}
		else
		{
			u64 nid = (u64)id;
			auto r	= sm64::asset::load(nid);

			if(r && r->isValid())
			{
				return (MacroObject*)r->ptr();
			}

			try
			{
				auto func = idMap().at(nid);
				return func;
			}
			catch(...)
			{
			}
		}

		return macro;
	}
} // namespace sm64::hook::macro
