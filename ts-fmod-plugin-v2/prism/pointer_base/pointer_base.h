#pragma once

#include "../actors/game_sound_data.h"
#include "../actors/game_ctrl.h"

#pragma pack(push, 1)

namespace prism
{
	class pointer_base_u                          // Size: 0x01A8
	{
	public:
		class game_sound_data_u* game_sound_data; //0x0000 (0x08)
		class game_ctrl_u* game_ctrl;             //0x0008 (0x08)
		char pad_0010[56];                        //0x0010 (0x38)
		void* sector_manager;                     //0x0048 (0x08)
		void* unk_gamedata;                       //0x0050 (0x08)
		char pad_0058[8];                         //0x0058 (0x08)
		class economy_u* economy;                 //0x0060 (0x08)
		void* game_traffic;                       //0x0068 (0x08)
		void* journey_events_manager;             //0x0070 (0x08)
		char pad_0078[8];                         //0x0078 (0x08)
		class camera_manager_u* camera_manager;   //0x0080 (0x08)
		class screen_ctrl* screen_ctrl;           //0x0088 (0x08)
		char pad_0090[40];                        //0x0090 (0x28)
		void* authentication_manager;             //0x00B8 (0x08)
		void* replay_manager;                     //0x00C0 (0x08)
		char pad_00C8[8];                         //0x00C8 (0x08)
		void* trigger_manager;                    //0x00D0 (0x08)
		void* cutscene_manager;                   //0x00D8 (0x08)
		char pad_00E0[72];                        //0x00E0 (0x48)
		void* oversize_manager;                   //0x0128 (0x08)
		char pad_0130[16];                        //0x0130 (0x10)
		void* radio;                              //0x0140 (0x08)
		char pad_0148[8];                         //0x0148 (0x08)
		void* license_plate_manager;              //0x0150 (0x08)
		char pad_0158[72];                        //0x0158 (0x48)
		void* traffic_sign_manager;               //0x01A0 (0x08)
	};
	static_assert(sizeof(pointer_base_u) == 0x1A8);

	inline pointer_base_u* pointer_base;
}

#pragma pack(pop)