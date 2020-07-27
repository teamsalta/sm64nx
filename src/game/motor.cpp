#include "motor.h"
#include "controller/controllers.h"
#include "player/players.h"

void SendMotorEvent(short time, short level)
{
	for(auto& d : sm64::hid::controllers().drivers())
	{
		for(auto& c : d->controllers())
		{
			c->SendMotorEvent(time, level);
		}
	}
}

void SendMotorDecay(short level)
{
}

void ResetMotorPack()
{
	for(auto& d : sm64::hid::controllers().drivers())
	{
		for(auto& c : d->controllers())
		{
			c->ResetMotorPack();
		}
	}
}

void SendMotorVib(int level)
{
	for(auto& d : sm64::hid::controllers().drivers())
	{
		for(auto& c : d->controllers())
		{
			c->SendMotorVib(level);
		}
	}
}