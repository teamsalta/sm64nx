#include "tas.h"
#include <stdio.h>
#include "player/players.h"

namespace sm64::hid
{
	namespace controller
	{
		class Tas : public Controller
		{
		public:
			Tas() : Controller()
			{
				fp = fopen("cont.m64", "rb");

				if(fp != NULL)
				{
					uint8_t buf[0x400];
					fread(buf, 1, sizeof(buf), fp);
				}
			}

			virtual ~Tas()
			{
				if(fp)
				{
					fclose(fp);
				}
			}

			void update()
			{
				if(fp != NULL)
				{
					u8 bytes[4] = { 0 };
					fread(bytes, 1, 4, fp);
					m_state.button = (bytes[0] << 8) | bytes[1];
					m_state.stick_x = bytes[2];
					m_state.stick_y = bytes[3];
				}
			}

		protected:
			FILE* fp;
		};
	}

	Tas::Tas() : Driver()
	{
	}

	Tas::~Tas()
	{
	}

	void Tas::scan(class Controllers* controllers)
	{
		if(!size())
		{
			auto controller = std::make_shared<controller::Tas>();
			m_controllers.push_back(controller);
			players().attach(controller, 0);
		}
	}

}
