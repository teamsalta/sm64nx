#pragma once
#include "sm64.h"
#include "xxhash64.h"
#include "graph_node.h"

namespace sm64::hook::dl
{
	u64 fingerprint(const Gfx* gfx, XXHash64* state);
	u64 fingerprint(const Gfx* gfx);

	Gfx* apply(Gfx* gfx);
	// Gfx* mount(Gfx* func, u64 size);
	GraphNodeFunc apply(GraphNodeFunc func);
} // namespace sm64::hook::dl
