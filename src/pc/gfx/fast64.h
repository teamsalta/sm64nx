#pragma once
#include "types.h"
#include <mutex>

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
		virtual void physics_loop();
		virtual void run(Gfx *commands);

		static Fast64* createContext();

	protected:
		std::mutex m_mutex;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_nextFrameTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_currentFrameStartTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_lastFrameTime;
		std::chrono::duration<u64, std::micro> m_lastFrameDuration;
		std::chrono::duration<u64, std::micro> m_lastSwapDuration;
		std::chrono::microseconds m_refreshRate;
	};
}
