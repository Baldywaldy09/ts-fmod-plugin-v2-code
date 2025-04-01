#pragma once

#include "../actors/game_actor.h"

#pragma pack(push, 1)

namespace prism
{
	class game_ctrl_u : public unit_t // Size: 0x2D80
	{
	public:
		char pad_0010[80];                        //0x0010 (0x50)
		void* game_data;                          //0x0060 (0x08)
		void* game_traffiu;                       //0x0068 (0x08)
		char pad_0070[8];                         //0x0070 (0x08)
		void* core_interior_glass_confgu;         //0x0078 (0x08)
		void* core_interior_glass_config2;        //0x0080 (0x08)
		class map_data_u* map_data;               //0x0088 (0x08)
		void* mirror_data;                        //0x0090 (0x08)
		char pad_0098[40];                        //0x0098 (0x28)
		prism::string_dyn_t timezone;             //0x00C0 (0x18)
		char pad_00D8[312];                       //0x00D8 (0x138)
		prism::array_dyn_t cameras;               //0x0210 (0x20)
		prism::array_dyn_t trailers;              //0x0230 (0x20)
		prism::array_dyn_t trailer_defs;          //0x0250 (0x20)
		prism::array_dyn_t traffic_vehicle_types; //0x0270 (0x20)
		prism::array_dyn_t traffic_trailer_types; //0x0290 (0x20)
		prism::array_dyn_t addon_hookups;         //0x02B0 (0x20)
		void* license_plate_manager;              //0x02D0 (0x08)
		void* driver_plate_manager;               //0x02D8 (0x08)
		void* player_tag_manager;                 //0x02E0 (0x08)
		void* traffic_sign_manager;               //0x02E8 (0x08)
		char pad_02F0[840];                       //0x02F0 (0x348)
		void* trigger_manager;                    //0x0638 (0x08)
		void* camera_manager;                     //0x0640 (0x08)
		void* cutscene_manager;                   //0x0648 (0x08)
		void* journey_events_manager;             //0x0650 (0x08)
		char pad_0658[16];                        //0x0658 (0x10)
		void* core_camera;                        //0x0668 (0x08)
		void* environment;                        //0x0670 (0x08)
		void* oversize_manager;                   //0x0678 (0x08)
		char pad_0680[48];                        //0x0680 (0x30)
		prism::game_ctrl_u* game_ctrl;            //0x06B0 (0x08)
		char pad_06B8[16];                        //0x06B8 (0x10)
		void* eye_state_u;                        //0x06C8 (0x08)
		char pad_06D0[8496];                      //0x06D0 (0x2130)
		prism::game_ctrl_u* game_ctrl2;           //0x2800 (0x08)
		char pad_2808[544];                       //0x2808 (0x220)
		void* physics_data_u;                     //0x2A28 (0x08)
		void* physics_interior_camera_data;       //0x2A30 (0x08)
		void* force_feedback_data;                //0x2A38 (0x08)
		void* vehicle_damage_data;                //0x2A40 (0x08)
		void* vehicle_service_data;               //0x2A48 (0x08)
		void* sector_manager;                     //0x2A50 (0x08)
		char pad_2A58[800];                       //0x2A58 (0x320)
		prism::game_actor_u* game_actor;          //0x2D78 (0x08)
	};
	static_assert(sizeof(prism::game_ctrl_u) == 0x2D80);
}

#pragma pack(pop)