#include "controllers.h"
#include "tas.h"
#include "keyboard.h"
#include "sdl.h"
#include <stdexcept>

namespace sm64::hid
{
	static Controllers* g_controllers;

	Controllers& controllers()
	{
		if(!g_controllers)
		{
			g_controllers = new Controllers();
		}
		return *g_controllers;
	}

	Driver::Driver()
	{
	}

	Driver::~Driver()
	{
	}

	const u64 Driver::size() const
	{
		return m_controllers.size();
	}

	Controller& Driver::controller(const u64 index)
	{
		if(index >= m_controllers.size())
		{
			throw std::runtime_error("invalid controller index");
		}
		return *m_controllers[index];
	}

	void Driver::update()
	{
		for(auto& controller : m_controllers)
		{
			controller->state().reset();
			controller->update();
			controller->resolveInputs();
		}
	}

	void Driver::scan(class Controllers* controllers)
	{
	}

	Controllers::Controllers()
	{
		m_drivers.push_back(new SDL());

#ifdef ENABLE_MOUSE
		m_drivers.push_back(new Keyboard());
#endif
		m_drivers.push_back(new Tas());
	}

	Controllers::~Controllers()
	{
	}

	const u64 Controllers::size() const
	{
		u64 count = 0;

		for(auto& driver : m_drivers)
		{
			count += driver->size();
		}

		return count;
	}

	void Controllers::update()
	{
		for(auto& driver : m_drivers)
		{
			driver->update();
		}
	}

	void Controllers::scan()
	{
		u64 found = 0;
		for(auto& driver : m_drivers)
		{
			if(!driver->defaultOnly() || !found)
			{
				driver->scan(this);
				found += driver->size();
			}
		}
	}
} // namespace sm64::hid