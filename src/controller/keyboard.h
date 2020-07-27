#pragma once
#include "controllers.h"

namespace sm64::hid
{
	class Keyboard : public Driver
	{
		public:
		Keyboard();
		virtual ~Keyboard();
		void scan(class Controllers* controllers) override;
		void update() override;
		bool defaultOnly() override
		{
			return true;
		}

		protected:
	};
} // namespace sm64::hid