#include "types.h"
#include "keyboard.h"

#include "pc/configfile.h"

#if !defined(DISABLE_SDL_CONTROLLER)
#include <SDL2/SDL.h>
#endif
#include "player/players.h"

#define STICK_X_LEFT 0x10000
#define STICK_X_RIGHT 0x20000
#define STICK_X_DOWN 0x80000
#define STICK_X_UP 0x40000

namespace sm64::hid
{
	namespace controller
	{
		class Keyboard : public Controller
		{
			public:
			Keyboard() : Controller()
			{
				int i = 0;

				set_keyboard_mapping(i++, STICK_X_UP, configKeyStickUp);
				set_keyboard_mapping(i++, STICK_X_LEFT, configKeyStickLeft);
				set_keyboard_mapping(i++, STICK_X_DOWN, configKeyStickDown);
				set_keyboard_mapping(i++, STICK_X_RIGHT, configKeyStickRight);
				set_keyboard_mapping(i++, A_BUTTON, configKeyA);
				set_keyboard_mapping(i++, B_BUTTON, configKeyB);
				set_keyboard_mapping(i++, Z_TRIG, configKeyZ);
				set_keyboard_mapping(i++, U_CBUTTONS, configKeyCUp);
				set_keyboard_mapping(i++, L_CBUTTONS, configKeyCLeft);
				set_keyboard_mapping(i++, D_CBUTTONS, configKeyCDown);
				set_keyboard_mapping(i++, R_CBUTTONS, configKeyCRight);
				set_keyboard_mapping(i++, R_TRIG, configKeyR);
				set_keyboard_mapping(i++, START_BUTTON, configKeyStart);
			}

			bool hasMouse() const
			{
				return true;
				return m_state.has_mouse;
			}

			int keyboard_buttons_down;

			int keyboard_mapping[13][2];

			int keyboard_map_scancode(int scancode)
			{
				int ret = 0;
				for(size_t i = 0; i < sizeof(keyboard_mapping) / sizeof(keyboard_mapping[0]); i++)
				{
					if(keyboard_mapping[i][0] == scancode)
					{
						ret |= keyboard_mapping[i][1];
					}
				}
				return ret;
			}

			bool on_key_down(int scancode)
			{
				int mapped = keyboard_map_scancode(scancode);
				keyboard_buttons_down |= mapped;
				return mapped != 0;
			}

			bool on_key_up(int scancode)
			{
				int mapped = keyboard_map_scancode(scancode);
				keyboard_buttons_down &= ~mapped;
				return mapped != 0;
			}

			void on_all_keys_up()
			{
				keyboard_buttons_down = 0;
			}

			void set_keyboard_mapping(int index, int mask, int scancode)
			{
				keyboard_mapping[index][0] = scancode;
				keyboard_mapping[index][1] = mask;
			}

			void enableMouse()
			{
				this->state().has_mouse = true;
			}

			void update()
			{
				int count  = 0;
				auto state = SDL_GetKeyboardState(&count);
				for(size_t i = 0; i < sizeof(keyboard_mapping) / sizeof(keyboard_mapping[0]); i++)
				{
					const auto scancode = keyboard_mapping[i][0];

					if(scancode < count)
					{
						if(state[scancode])
						{
							if(keyboard_mapping[i][1] > 0xFFFF)
							{
								switch(keyboard_mapping[i][1])
								{
									case STICK_X_DOWN:
										m_state.stick_y = -128;
										break;
									case STICK_X_UP:
										m_state.stick_y = 127;
										break;
									case STICK_X_LEFT:
										m_state.stick_x = -128;
										break;
									case STICK_X_RIGHT:
										m_state.stick_x = 127;
										break;
								}
							}
							else
							{
								this->state().button |= keyboard_mapping[i][1];
							}
						}
					}
				}

#ifdef ENABLE_MOUSE
				m_state.mouse_delta_x = 0;
				m_state.mouse_delta_y = 0;

				auto buttons = SDL_GetRelativeMouseState(&m_state.mouse_delta_x, &m_state.mouse_delta_y);
				this->enableMouse();

				if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					m_state.button |= B_BUTTON;
				}

				if(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					m_state.button |= A_BUTTON;
				}

				m_state.mouse_x += m_state.mouse_delta_x * 4;
				m_state.mouse_y += m_state.mouse_delta_y * 4;
#endif
			}
		};
	} // namespace controller

	Keyboard::Keyboard() : Driver()
	{
	}

	Keyboard::~Keyboard()
	{
	}

	void Keyboard::scan(class Controllers* controllers)
	{
		if(!size())
		{
#ifdef ENABLE_MOUSE
			SDL_SetRelativeMouseMode(SDL_TRUE);
#endif
			auto controller = std::make_shared<controller::Keyboard>();
#ifdef ENABLE_MOUSE

			controller->enableMouse();
#endif
			m_controllers.push_back(controller);
			players().attach(controller, 0);
		}
	}

	void Keyboard::update()
	{
		for(auto& keyboard : m_controllers)
		{
			((controller::Keyboard*)keyboard.get())->update();
		}
	}
} // namespace sm64::hid
