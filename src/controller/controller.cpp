#include "sm64.h"
#include "game/options.h"
#include "controller.h"

namespace sm64::hid
{
	State::State()
	{
		mouse_x	      = 0;
		mouse_y	      = 0;
		mouse_delta_x = 0;
		mouse_delta_y = 0;
		mouse_wheel   = 0;
		has_mouse     = false;

		reset();
	}

	void State::reset()
	{
		button	  = 0;
		stick_x	  = 0;
		stick_y	  = 0;
		errnum	  = 0;
		r_stick_x = 0;
		r_stick_y = 0;

		// mouse_x = 0;
		// mouse_y = 0;
		mouse_delta_x = 0;
		mouse_delta_y = 0;
		// mouse_wheel = 0;
		mouse_l	  = 0;
		mouse_r	  = 0;
		has_mouse = false;
	}

	Controller::Controller(bool isLocal) :
	    rawStickX(0), rawStickY(0), stickX(0), stickY(0), stickMag(0), buttonDown(0), buttonPressed(0), r_rawStickX(0), r_rawStickY(0), r_stickX(0), r_stickY(0), r_stickMag(0), m_isLocal(isLocal), m_state(), m_motorEnabled(false)
	{
	}

	void Controller::SendMotorEvent(short time, short level)
	{
	}

	void Controller::SendMotorDecay(short level)
	{
	}

	void Controller::ResetMotorPack()
	{
	}

	void Controller::SendMotorVib(int level)
	{
	}

	void Controller::update()
	{
	}

	void Controller::merge(const Controller& controller)
	{
		m_state.button |= controller.m_state.button;

		if(abs(m_state.stick_x) < abs(controller.m_state.stick_x))
		{
			m_state.stick_x = controller.m_state.stick_x;
		}

		if(abs(m_state.stick_y) < abs(controller.m_state.stick_y))
		{
			m_state.stick_y = controller.m_state.stick_y;
		}

		if(abs(m_state.r_stick_x) < abs(controller.m_state.r_stick_x))
		{
			m_state.r_stick_x = controller.m_state.r_stick_x;
		}

		if(abs(m_state.r_stick_y) < abs(controller.m_state.r_stick_y))
		{
			m_state.r_stick_y = controller.m_state.r_stick_y;
		}

		if(controller.hasMouse())
		{
			m_state.mouse_x = controller.m_state.mouse_x;
			m_state.mouse_y = controller.m_state.mouse_y;

			m_state.mouse_l = controller.m_state.mouse_l;
			m_state.mouse_r = controller.m_state.mouse_r;

			m_state.mouse_delta_x = controller.m_state.mouse_delta_x;
			m_state.mouse_delta_y = controller.m_state.mouse_delta_y;
		}

		m_state.has_mouse |= controller.m_state.has_mouse;
	}

	void Controller::resolveInputs()
	{
		rawStickX     = m_state.stick_x;
		rawStickY     = m_state.stick_y;
		r_rawStickX   = m_state.r_stick_x;
		r_rawStickY   = m_state.r_stick_y;
		buttonPressed = m_state.button & (m_state.button ^ buttonDown);
		// 0.5x A presses are a good meme
		buttonDown = m_state.button;

		if(sm64::config().game().mirror())
		{
			rawStickX   = -rawStickX;
			r_rawStickX = -r_rawStickX;
		}

		// reset the controller's x and y floats.
		this->stickX = 0;
		this->stickY = 0;

		// modulate the rawStickX and rawStickY to be the new f32 values by adding/subtracting 6.
		if(this->rawStickX <= -8)
		{
			this->stickX = this->rawStickX + 6;
		}

		if(this->rawStickX >= 8)
		{
			this->stickX = this->rawStickX - 6;
		}

		if(this->rawStickY <= -8)
		{
			this->stickY = this->rawStickY + 6;
		}

		if(this->rawStickY >= 8)
		{
			this->stickY = this->rawStickY - 6;
		}

		// calculate f32 magnitude from the center by vector length.
		this->stickMag = sqrtf(this->stickX * this->stickX + this->stickY * this->stickY);

		// magnitude cannot exceed 64.0f: if it does, modify the values appropriately to
		// flatten the values down to the allowed maximum value.
		if(this->stickMag > 64)
		{
			this->stickX *= 64 / this->stickMag;
			this->stickY *= 64 / this->stickMag;
			this->stickMag = 64;
		}

		this->r_stickX = 0;
		this->r_stickY = 0;

		if(this->r_rawStickX <= -8)
		{
			this->r_stickX = this->r_rawStickX + 6;
		}

		if(this->r_rawStickX >= 8)
		{
			this->r_stickX = this->r_rawStickX - 6;
		}

		if(this->r_rawStickY <= -8)
		{
			this->r_stickY = this->r_rawStickY + 6;
		}

		if(this->r_rawStickY >= 8)
		{
			this->r_stickY = this->r_rawStickY - 6;
		}

		// calculate f32 magnitude from the center by vector length.
		this->r_stickMag = sqrtf(this->r_stickX * this->r_stickX + this->r_stickY * this->r_stickY);

		// magnitude cannot exceed 64.0f: if it does, modify the values appropriately to
		// flatten the values down to the allowed maximum value.
		if(this->r_stickMag > 64)
		{
			this->r_stickX *= 64 / this->r_stickMag;
			this->r_stickY *= 64 / this->r_stickMag;
			this->r_stickMag = 64;
		}
	}

#ifdef ENABLE_MOUSE
	s64 Controller::mouse_x() const
	{
		return m_state.mouse_x * (sm64::config().camera().mousexInvert() ? -1 : 1) * sm64::config().camera().mousexScaler();
	}

	s64 Controller::mouse_y() const
	{
		return m_state.mouse_y * (sm64::config().camera().mouseyInvert() ? -1 : 1) * sm64::config().camera().mouseyScaler();
	}

	int Controller::mouse_delta_x() const
	{
		return m_state.mouse_delta_x;
	}

	int Controller::mouse_delta_y() const
	{
		return m_state.mouse_delta_y;
	}

	s64 Controller::mouse_wheel() const
	{
		return m_state.mouse_wheel;
	}

	u8 Controller::mouse_l() const
	{
		return m_state.mouse_l;
	}

	u8 Controller::mouse_r() const
	{
		return m_state.mouse_r;
	}
#endif
} // namespace sm64::hid
