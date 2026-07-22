#pragma once

#include "../actors/camera_manager.h"
#include "../actors/game_sound_data.h"
#include "../actors/game_ctrl.h"

#pragma pack(push, 1)

namespace prism
{
	class pointer_base_u                          // Size: 0x01A8
	{
	public:
		prism::game_sound_data_u* game_sound_data;     //0x0000 (0x08)
		prism::game_ctrl_u* game_ctrl;                 //0x0008 (0x08)
		char pad_0010[56];                             //0x0010 (0x38)
		void* sector_manager;                          //0x0048 (0x08)
		void* unk_gamedata;                            //0x0050 (0x08)
		char pad_0058[8];                              //0x0058 (0x08)
		class economy_u* economy;                      //0x0060 (0x08)
		void* game_traffic;                            //0x0068 (0x08)
		camera_manager_u* camera_manager;              //0x0078 (0x08)
	};

	inline pointer_base_u* pointer_base;
}

#pragma pack(pop)