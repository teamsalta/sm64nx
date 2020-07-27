#pragma once
#include "types.h"
#include "controller/controller.h"
#include <vector>
#include <memory>

namespace sm64
{
	class Player
	{
		public:
		Player();
		virtual ~Player();

		std::vector<std::shared_ptr<hid::Controller>>& controllers()
		{
			return m_controllers;
		}
		void attach(const std::shared_ptr<hid::Controller>& controller);
		void detachControllers();
		void update();
		hid::Controller& controller()
		{
			return m_controller;
		}

		protected:
		std::vector<std::shared_ptr<hid::Controller>> m_controllers;
		hid::Controller m_controller;
	};
} // namespace sm64
