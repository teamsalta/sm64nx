#pragma once

#include "types.h"
#include "controller.h"
#include <vector>
#include <memory>

namespace sm64::hid
{
	class Driver
	{
		public:
		Driver();
		virtual ~Driver();
		virtual const u64 size() const;
		virtual Controller& controller(const u64 index);
		virtual void update();
		virtual void scan(class Controllers* controllers);
		virtual bool defaultOnly()
		{
			return false;
		}

		std::vector<std::shared_ptr<Controller>>& controllers()
		{
			return m_controllers;
		}

		protected:
		std::vector<std::shared_ptr<Controller>> m_controllers;
	};

	class Controllers
	{
		public:
		Controllers();
		virtual ~Controllers();
		const u64 size() const;
		void update();
		void scan();
		std::vector<class Driver*>& drivers()
		{
			return m_drivers;
		}

		protected:
		std::vector<class Driver*> m_drivers;
	};

	Controllers& controllers();
} // namespace sm64::hid