#if !defined(DISABLE_SDL_CONTROLLER)

#include "sm64.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include "sdl.h"

#ifndef __SWITCH__
#define DEADZONE 20
#define RDEADZONE 20
#else
#define DEADZONE 0
#define RDEADZONE 0
#endif

static bool init_ok;

#define INITIAL_PEAK 0x8000

static int g_lstickX_peak = INITIAL_PEAK;
static int g_lstickY_peak = INITIAL_PEAK;
static int g_rstickX_peak = INITIAL_PEAK;
static int g_rstickY_peak = INITIAL_PEAK;

#ifdef DEBUG
#include <time.h>
extern struct Object* gMarioObject;
#endif

namespace sm64::hid
{
	namespace controller
	{
		class SDL : public Controller
		{
		public:
			SDL(SDL_GameController* controller) : Controller(), m_context(controller)
			{
			}

			static inline int8_t convertToByte(int value, int max)
			{
				int8_t result = value * 0x7F / max;

				return result;
			}

			static inline int8_t invert(const int8_t value)
			{
				if(value <= -128)
				{
					return 127;
				}

				return -value;
			}

			inline int16_t readAxis(SDL_GameControllerAxis axis)
			{
				return SDL_GameControllerGetAxis(m_context, axis);
			}

			inline int8_t stickLeftX()
			{
				auto value = readAxis(SDL_CONTROLLER_AXIS_LEFTX);

				if(abs(value) > g_lstickX_peak)
				{
					g_lstickX_peak = abs(value);
				}

				return convertToByte(value, g_lstickX_peak);
			}

			inline int8_t stickLeftY()
			{
				auto value = readAxis(SDL_CONTROLLER_AXIS_LEFTY);

				if(abs(value) > g_lstickY_peak)
				{
					g_lstickY_peak = abs(value);
				}

				return convertToByte(value, g_lstickY_peak);
			}

			inline int8_t stickRightX()
			{
				auto value = readAxis(SDL_CONTROLLER_AXIS_RIGHTX);

				if(abs(value) > g_rstickX_peak)
				{
					g_rstickX_peak = abs(value);
				}

				return convertToByte(value, g_rstickX_peak);
			}

			inline int8_t stickRightY()
			{
				auto value = readAxis(SDL_CONTROLLER_AXIS_RIGHTY);

				if(abs(value) > g_rstickY_peak)
				{
					g_rstickY_peak = abs(value);
				}

				return convertToByte(value, g_rstickY_peak);
			}

			void update() override
			{
				if(!init_ok || !m_context)
				{
					return;
				}

				SDL_GameControllerUpdate();

				if(m_context != NULL && !SDL_GameControllerGetAttached(m_context))
				{
					SDL_GameControllerClose(m_context);
					m_context = NULL;
				}

				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_START))
					m_state.button |= START_BUTTON;

				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
					m_state.button |= Z_TRIG;

				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
					m_state.button |= R_TRIG;

				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_A))
					m_state.button |= A_BUTTON;

				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_X))
					m_state.button |= B_BUTTON;
				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_B))
					m_state.button |= A_BUTTON;
				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_Y))
					m_state.button |= B_BUTTON;

				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
					m_state.button |= L_CBUTTONS;
				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
					m_state.button |= R_CBUTTONS;
				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_DPAD_UP))
					m_state.button |= U_CBUTTONS;
				if(SDL_GameControllerGetButton(m_context, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
					m_state.button |= D_CBUTTONS;



				if(sm64::config().camera().useClassicCamera())
				{
					int16_t leftx = SDL_GameControllerGetAxis(m_context, SDL_CONTROLLER_AXIS_LEFTX);
					int16_t lefty = SDL_GameControllerGetAxis(m_context, SDL_CONTROLLER_AXIS_LEFTY);
					int16_t rightx = SDL_GameControllerGetAxis(m_context, SDL_CONTROLLER_AXIS_RIGHTX);
					int16_t righty = SDL_GameControllerGetAxis(m_context, SDL_CONTROLLER_AXIS_RIGHTY);

					if(abs(righty) > 0x7000)
					{
						if(righty > 0)
						{
							m_state.button |= D_CBUTTONS;
						}
						else
						{
							m_state.button |= U_CBUTTONS;
						}
					}

					if(abs(rightx) > 0x7000)
					{
						if(rightx > 1)
						{
							m_state.button |= R_CBUTTONS;
						}
						else
						{
							m_state.button |= L_CBUTTONS;
						}
					}
				}

				int16_t ltrig = SDL_GameControllerGetAxis(m_context, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
				int16_t rtrig = SDL_GameControllerGetAxis(m_context, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

				if(ltrig > 30 * 256)
					m_state.button |= Z_TRIG;
				if(rtrig > 30 * 256)
					m_state.button |= R_TRIG;

				m_state.stick_x = stickLeftX();
				m_state.stick_y = invert(stickLeftY());
				m_state.r_stick_x = stickRightX();
				m_state.r_stick_y = stickRightY();

				uint32_t magnitude_sq = (uint32_t)(m_state.stick_x * m_state.stick_x) + (uint32_t)(m_state.stick_y * m_state.stick_y);

				if(magnitude_sq < (uint32_t)(DEADZONE * DEADZONE))
				{
					m_state.stick_x = 0;
					m_state.stick_y = 0;
				}

				magnitude_sq = (uint32_t)(m_state.r_stick_x * m_state.r_stick_x) + (uint32_t)(m_state.r_stick_y * m_state.r_stick_y);

				if(magnitude_sq < (uint32_t)(RDEADZONE * RDEADZONE))
				{
					m_state.r_stick_x = 0;
					m_state.r_stick_y = 0;
				}
			}
		protected:
			SDL_GameController* m_context;
		};
	}
	SDL::SDL()
	{
		if(SDL_Init(SDL_INIT_GAMECONTROLLER) != 0)
		{
			fprintf(stderr, "SDL init error: %s\n", SDL_GetError());
		}
	}

	SDL::~SDL()
	{
	}

	void SDL::scan(class Controllers* controllers)
	{
#ifdef __SWITCH__
		// swap A, B and X, Y to correct positions
		SDL_GameControllerAddMapping("53776974636820436F6E74726F6C6C65,Switch Controller,"
			"a:b0,b:b1,back:b11,"
			"dpdown:b15,dpleft:b12,dpright:b14,dpup:b13,"
			"leftshoulder:b6,leftstick:b4,lefttrigger:b8,leftx:a0,lefty:a1,"
			"rightshoulder:b7,rightstick:b5,righttrigger:b9,rightx:a2,righty:a3,"
			"start:b10,x:b2,y:b3");
#endif		

		init_ok = true;

		for(int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if(SDL_IsGameController(i))
			{
				auto context = SDL_GameControllerOpen(i);

				if(context)
				{
					auto controller = std::make_shared<controller::SDL>(context);
					m_controllers.push_back(controller);
					players().attach(controller, 0);
				}
			}
		}
	}
}

#endif
