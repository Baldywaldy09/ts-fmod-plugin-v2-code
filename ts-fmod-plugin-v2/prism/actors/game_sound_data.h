#pragma once

#include "../sound/fmod.h"
#include "../sound/navigation.h"
#include "../actors/game_ctrl.h"

#pragma pack(push, 1)

namespace prism
{
	class game_sound_data_u                                 // Size: 0x1C59
	{
	public:
		char pad_0000[280];                                 //0x0000 (0x118)
		bool is_cam_interior;                               //0x0118 (0x01) is the camera inside (true when looking outside)
		bool is_cam_inside_interior;                        //0x0119 (0x01) is the camera inside (false when looking outside)
		char pad_011A[2];                                   //0x011A (0x02)
		float camera_rotation_in_cabin;                     //0x011C (0x04)
		float cabin_out;                                    //0x0120 (0x04)
		char pad_0124[4];                                   //0x0124 (0x04)
		class fmod_parameter_t* cabin_type;                 //0x0128 (0x08)
		class fmod_parameter_t* cabin_rot;                  //0x0130 (0x08)
		class fmod_parameter_t* cabin_out_t;                //0x0138 (0x08)
		vector_2 window_state;                               //0x0140 (0x08) first=left second=right
		class fmod_parameter_t* wnd_left;                   //0x0148 (0x08)
		class fmod_parameter_t* wnd_right;                  //0x0150 (0x08)
		float daytime_value;                                //0x0158 (0x04)
		char pad_015C[4];                                   //0x015C (0x04)
		class fmod_parameter_t* daytime;                    //0x0160 (0x08)
		char pad_0168[160];                                 //0x0168 (0xa0)
		class navigation_sound_event navigation_events[36]; //0x0208 (0x5a0)
		class string_dyn_t navigation_bank_path;            //0x07A8 (0x18)
		class navigation_sound_event* playing_navi_sound;   //0x07C0 (0x08)
		char pad_07C8[5128];                                //0x07C8 (0x1408)
		vector_3 cam_position;                               //0x1BD0 (0x0c)
		char pad_1BDC[124];                                 //0x1BDC (0x7c)
		bool echo;                                          //0x1C58 (0x01)
	};
	static_assert(sizeof(game_sound_data_u) == 0x1C59);
}

#pragma pack(pop)