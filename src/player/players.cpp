#include "players.h"

namespace sm64
{
	Players g_players;

	Players& players()
	{
		return g_players;
	}

	Player& player(const u64 i)
	{
		return g_players[i];
	}

	Players::Players() : m_players(), m_size(0)
	{
	}

	const u64 Players::size() const
	{
		return m_size;
	}

	void Players::update()
	{
		for(auto& player : m_players)
		{
			player.update();
		}
	}

	void Players::attach(const std::shared_ptr<hid::Controller>& controller, const u8 playerId)
	{
		if(playerId == MAX_PLAYERS)
		{
			m_players[m_size++].attach(controller);
		}
		else
		{
			m_players[playerId].attach(controller);
			m_size = MAX(playerId+1, m_size);
		}
	}
}