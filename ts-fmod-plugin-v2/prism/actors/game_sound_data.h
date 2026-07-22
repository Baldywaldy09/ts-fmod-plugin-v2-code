#pragma once

#include "../sound/fmod.h"
#include "../sound/navigation.h"
#include "../actors/game_ctrl.h"

#pragma pack(push, 1)

namespace prism
{
	class game_sound_data_u // Size: 0x21A1
	{
	public:
		char pad_0000[304]; //0x0000 (0x130)
		bool is_cam_interior; //0x0130 (0x01) is the camera inside (true when looking outside)
		bool is_cam_inside_interior; //0x0131 (0x01) is the camera inside (false when looking outside)
		char pad_0132[2]; //0x0132 (0x02)
		float camera_rotation_in_cabin; //0x0134 (0x04)
		float cabin_out; //0x0138 (0x04)
		char pad_013C[4]; //0x013C (0x04)
		class fmod_parameter_t* cabin_type; //0x0140 (0x08)
		class fmod_parameter_t* cabin_rot; //0x0148 (0x08)
		class fmod_parameter_t* cabin_out_t; //0x0150 (0x08)
		vector_2 window_state; //0x0158 (0x08) first=left second=right
		class fmod_parameter_t* wnd_left; //0x0160 (0x08)
		class fmod_parameter_t* wnd_right; //0x0168 (0x08)
		float daytime_value; //0x0170 (0x04)
		char pad_0174[172]; //0x0174 (0xac)
		class navigation_sound_event navigation_events[36]; //0x0220 (0x5a0)
		class string_dyn_t navigation_bank_path; //0x07C0 (0x20)
		class navigation_sound_event* playing_navi_sound; //0x07E0 (0x08)
		char pad_07E8[5160]; //0x07E8 (0x1428)
		vector_3 cam_position; //0x1C10 (0x0c)
		char pad_1C1C[124]; //0x1C1C (0x7c)
		bool echo; //0x1C98 (0x01)
		char pad_1C99[1288]; //0x1C99 (0x508)
	};
	static_assert(sizeof(game_sound_data_u) == 0x21A1);
}

#pragma pack(pop)