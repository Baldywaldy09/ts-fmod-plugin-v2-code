#pragma once

#include <cstddef>
#include "../actors/game_actor.h"

#pragma pack(push, 1)

namespace prism
{
	class game_ctrl_u
	{
	public:
		char pad_0000[0x31B0];
		prism::game_actor_u* game_actor;
	};
	static_assert(offsetof(game_ctrl_u, game_actor) == 0x31B0);
}

#pragma pack(pop)