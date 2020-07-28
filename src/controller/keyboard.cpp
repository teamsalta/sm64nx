#include "types.h"
#include "keyboard.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <rapidjson/istreamwrapper.h>
#include "pc/configfile.h"
#include <fstream>
#include <unordered_map>

#if !defined(DISABLE_SDL_CONTROLLER)
#include <SDL2/SDL.h>
#endif
#include "player/players.h"
#include "game/debug.h"

#define STICK_X_LEFT 0x10000
#define STICK_X_RIGHT 0x20000
#define STICK_X_DOWN 0x80000
#define STICK_X_UP 0x40000

bool saveJson(rapidjson::Document& doc, const std::string& jsonFilePath)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	if (!doc.Accept(writer))
	{
		return false;
	}

	std::ofstream out(jsonFilePath.c_str(), std::ofstream::trunc);
	out << buffer.GetString();

	return true;
}

namespace sm64::hid
{
	namespace controller
	{
		static const char* getInputName(int input)
		{
			switch (input)
			{
			case STICK_X_UP: return "STICK_X_UP";
			case STICK_X_LEFT: return "STICK_X_LEFT";
			case STICK_X_DOWN: return "STICK_X_DOWN";
			case STICK_X_RIGHT: return "STICK_X_RIGHT";
			case A_BUTTON: return "A_BUTTON";
			case B_BUTTON: return "B_BUTTON";
			case Z_TRIG: return "Z_TRIG";
			case U_CBUTTONS: return "U_CBUTTONS";
			case L_CBUTTONS: return "L_CBUTTONS";
			case D_CBUTTONS: return "D_CBUTTONS";
			case R_CBUTTONS: return "R_CBUTTONS";
			case R_TRIG: return "R_TRIG";
			case START_BUTTON: return "START_BUTTON";
			}
			return "";
		}

		static int getInputValue(const std::string& input)
		{
			if (input == "STICK_X_UP") return STICK_X_UP;
			if (input == "STICK_X_LEFT") return STICK_X_LEFT;
			if (input == "STICK_X_DOWN") return STICK_X_DOWN;
			if (input == "STICK_X_RIGHT") return STICK_X_RIGHT;
			if (input == "A_BUTTON") return A_BUTTON;
			if (input == "B_BUTTON") return B_BUTTON;
			if (input == "Z_TRIG") return Z_TRIG;
			if (input == "U_CBUTTONS") return U_CBUTTONS;
			if (input == "L_CBUTTONS") return L_CBUTTONS;
			if (input == "D_CBUTTONS") return D_CBUTTONS;
			if (input == "R_CBUTTONS") return R_CBUTTONS;
			if (input == "R_TRIG") return R_TRIG;
			if (input == "START_BUTTON") return START_BUTTON;

			return 0;
		}

		class Keyboard : public Controller
		{
		public:
			Keyboard() : Controller()
			{
				memset(m_lastKeyState, 0, sizeof(m_lastKeyState));
				m_keyBindings[SDL_SCANCODE_W] = STICK_X_UP;
				m_keyBindings[SDL_SCANCODE_A] = STICK_X_LEFT;
				m_keyBindings[SDL_SCANCODE_S] = STICK_X_DOWN;
				m_keyBindings[SDL_SCANCODE_D] = STICK_X_RIGHT;
				m_keyBindings[SDL_SCANCODE_SPACE] = A_BUTTON;
				m_keyBindings[SDL_SCANCODE_F] = B_BUTTON;
				m_keyBindings[SDL_SCANCODE_LSHIFT] = Z_TRIG;
				m_keyBindings[SDL_SCANCODE_C] = Z_TRIG;
				m_keyBindings[SDL_SCANCODE_UP] = U_CBUTTONS;
				m_keyBindings[SDL_SCANCODE_LEFT] = L_CBUTTONS;
				m_keyBindings[SDL_SCANCODE_DOWN] = D_CBUTTONS;
				m_keyBindings[SDL_SCANCODE_RIGHT] = R_CBUTTONS;
				m_keyBindings[SDL_SCANCODE_V] = R_TRIG;
				m_keyBindings[SDL_SCANCODE_RSHIFT] = R_TRIG;
				m_keyBindings[SDL_SCANCODE_RETURN] = START_BUTTON;
				m_keyBindings[SDL_SCANCODE_ESCAPE] = START_BUTTON;

#ifndef __SWITCH__
				loadKeyBindings();
#endif
			}

			void loadKeyBindings()
			{
				try
				{
					std::ifstream ifs("keyboard1.bindings.json", std::ifstream::in);


					if (ifs.is_open())
					{
						rapidjson::IStreamWrapper isw(ifs);
						rapidjson::Document d;
						d.ParseStream(isw);

						if (d.IsObject())
						{
							for (auto itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr)
							{
								if (itr->name.IsString() && itr->value.IsString())
								{
									auto key = SDL_GetScancodeFromName(itr->name.GetString());
									auto value = getInputValue(itr->value.GetString());								

									if (key != SDL_SCANCODE_UNKNOWN && value)
									{
										m_keyBindings[key] = value;
									}
									else
									{
										sm64::log("could not bind key: \"%s\" -> \"%s\"\n", itr->value.GetString(), itr->name.GetString());
									}
								}
							}
						}
					}
				}
				catch (...)
				{
				}
			}

			void saveKeyBindings()
			{
				try
				{
					rapidjson::Document d;
					d.SetObject();

					rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

					for (const auto i : m_keyBindings)
					{
						rapidjson::Value value(getInputName(i.second), allocator);
						rapidjson::Value key(SDL_GetScancodeName(i.first), allocator);
						d.AddMember(key, value, allocator);
					}

					saveJson(d, "keyboard1.bindings.json");
				}
				catch (...)
				{
				}
			}

			void clearRebindMode()
			{
			}

			void resetRebinds()
			{
			}


			bool hasMouse() const
			{
				return true;
				return m_state.has_mouse;
			}

			int keyboard_buttons_down;

			int keyboard_map_scancode(SDL_Scancode scancode)
			{
				if (m_keyBindings.count(scancode))
				{
					return m_keyBindings[scancode];
				}
				return 0;
			}

			bool on_key_down(SDL_Scancode scancode)
			{
				int mapped = keyboard_map_scancode(scancode);
				keyboard_buttons_down |= mapped;
				return mapped != 0;
			}

			bool on_key_up(SDL_Scancode scancode)
			{
				int mapped = keyboard_map_scancode(scancode);
				keyboard_buttons_down &= ~mapped;
				return mapped != 0;
			}

			void on_all_keys_up()
			{
				keyboard_buttons_down = 0;
			}

			void enableMouse()
			{
				this->state().has_mouse = true;
			}

			bool canRebind(SDL_Scancode scancode, int input)
			{
				if (m_keyBindings.count(scancode) == 0)
				{
					return true;
				}

				auto replacingInput = m_keyBindings[scancode];
				u64 count = 0;

				for (auto i : m_keyBindings)
				{
					if (i.second == replacingInput)
					{
						count++;
					}
				}

				return count != 1;
			}

			bool updateRebind(int input) override
			{
				int count = 0;
				auto state = SDL_GetKeyboardState(&count);
				u64 changed = 0;

				for (int i = 0; i < MIN(MAX_KEY_STATE, count); i++)
				{
					m_lastKeyState[i] = (state[i] ^ m_lastKeyState[i]) & state[i];
				}

				for (int i = 0; i < MIN(MAX_KEY_STATE, count); i++)
				{
					if (m_lastKeyState[i] && canRebind((SDL_Scancode)i, input))
					{
  						m_keyBindings[(SDL_Scancode)i] = input;
						changed++;
						saveKeyBindings();
					}
				}

				memcpy(m_lastKeyState, state, MIN(MAX_KEY_STATE, count));
				return changed != 0;
			}

			void update()
			{
				int count = 0;
				auto state = SDL_GetKeyboardState(&count);
				for (const auto x : m_keyBindings)
				{
					const auto scancode = x.first;
					const auto input = x.second;

					if (scancode < count)
					{
						if (state[scancode])
						{
							if (input > 0xFFFF)
							{
								switch (input)
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
								this->state().button |= input;
							}
						}
					}
				}

#ifdef ENABLE_MOUSE
				m_state.mouse_delta_x = 0;
				m_state.mouse_delta_y = 0;

				auto buttons = SDL_GetRelativeMouseState(&m_state.mouse_delta_x, &m_state.mouse_delta_y);
				this->enableMouse();

				if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					m_state.button |= B_BUTTON;
				}

				if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
				{
					m_state.button |= A_BUTTON;
				}

				m_state.mouse_x += m_state.mouse_delta_x * 4;
				m_state.mouse_y += m_state.mouse_delta_y * 4;
#endif
				memcpy(m_lastKeyState, state, MIN(MAX_KEY_STATE, count));
			}

			protected:
				std::unordered_map<SDL_Scancode, int> m_keyBindings;
				u8 m_lastKeyState[MAX_KEY_STATE];
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
		if (!size())
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
		for (auto& keyboard : m_controllers)
		{
			((controller::Keyboard*)keyboard.get())->update();
		}
	}
} // namespace sm64::hid
