#pragma once

#include <cstddef>
#include "../common/vectors.h"
#include "../sound/navigation.h"

#pragma pack(push, 1)

namespace prism
{
	// Offsets for ETS2/ATS 1.61 (Windows) — sound_library_t
	class game_sound_data_u
	{
	public:
		char pad_0000[0x148]; //0x0000
		bool is_cam_interior; //0x0148 true when looking outside from cabin
		bool is_cam_inside_interior; //0x0149 false when looking outside
		char pad_014A[2]; //0x014A
		float camera_rotation_in_cabin; //0x014C
		float cabin_out; //0x0150
		char pad_0154[0x1C]; //0x0154
		vector_2 window_state; //0x0170 first=left second=right
		char pad_0178[0x18]; //0x0178
		float daytime_value; //0x0190
		char pad_0194[0x674]; //0x0194 → 0x0808
		class navigation_sound_event* playing_navi_sound; //0x0808
		char pad_0810[0x14B0]; //0x0810 → 0x1CC0
		bool echo; //0x1CC0
	};
	static_assert(offsetof(game_sound_data_u, is_cam_interior) == 0x0148);
	static_assert(offsetof(game_sound_data_u, window_state) == 0x0170);
	static_assert(offsetof(game_sound_data_u, daytime_value) == 0x0190);
	static_assert(offsetof(game_sound_data_u, playing_navi_sound) == 0x0808);
	static_assert(offsetof(game_sound_data_u, echo) == 0x1CC0);
}

#pragma pack(pop)
