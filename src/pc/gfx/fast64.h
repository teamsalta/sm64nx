#pragma once
#include "types.h"

struct GfxDimensions
{
	u32 width;
	u32 height;
	float aspect_ratio;
	float adjust_ratio;
};

extern GfxDimensions gfx_current_dimensions;

namespace sm64::gfx
{
	class Fast64
	{
	public:
		virtual void start_frame() = 0;
		virtual void end_frame() = 0;
		virtual void run_loop();
		virtual void run(Gfx *commands);

		static Fast64* createContext();
	};
}
