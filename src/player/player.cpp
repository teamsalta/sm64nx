#include "player.h"

namespace sm64
{
	Player::Player()
	{
	}

	Player::~Player()
	{
	}

	void Player::attach(const std::shared_ptr<hid::Controller>& controller)
	{
		m_controllers.push_back(controller);
	}

	void Player::detachControllers()
	{
		m_controllers.resize(0);
	}

	void Player::update()
	{
		m_controller.state().reset();

		for(auto& controller : m_controllers)
		{
			controller->state().reset();
			controller->update();

			m_controller.merge(*controller);
		}
		m_controller.resolveInputs();
	}
} // namespace sm64
