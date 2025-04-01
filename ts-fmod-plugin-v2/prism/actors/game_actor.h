#pragma once

#pragma pack(push, 1)

namespace prism
{
	enum window_state
	{
		window_not_fully_open = 0,
		window_open = 1,
		window_closed = 2,
		window_moving_down_auto = 272,
		window_moving_up_auto = 288,
		window_reached_bottom = 4097,
		window_moving_down_manual = 4112,
		window_reached_top = 8194,
		window_moving_up_manual = 8224,
	};

	class game_actor_u : public unit_t // Size: 0x02E2
	{
	public:
		char pad_0010[8];                          //0x0010 (0x08)
		void* game_physics_vehicle;                //0x0018 (0x08)
		void* visual_interior;                     //0x0020 (0x08)
		void* game_actor_hud;                      //0x0028 (0x08)
		char pad_0030[64];                         //0x0030 (0x40)
		bool engine_running;                       //0x0070 (0x01)
		char pad_0071[3];                          //0x0071 (0x03)
		float engine_rpm;                          //0x0074 (0x04)
		float air_pressure;                        //0x0078 (0x04)
		float brake_pressure;                      //0x007C (0x04) for some reason this only works on the electric trucks????
		float turbo_pressure;                      //0x0080 (0x04)
		float retarder_level;                      //0x0084 (0x04) 0 = none ; 0.333 = lvl 1 ; 0.667 = lvl 2 ; 1.000 = max
		char pad_0088[24];                         //0x0088 (0x18)
		void* history_stream_set;                  //0x00A0 (0x08)
		char pad_00A8[16];                         //0x00A8 (0x10)
		void* history_manager;                     //0x00B8 (0x08)
		void* game_trailer_actor;                  //0x00C0 (0x08)
		char pad_00C8[88];                         //0x00C8 (0x58)
		int32_t current_camera;                    //0x0120 (0x04) 0 = dev ; 1 = orbit ; 2 = int ; 3 = front ; 4 = out window ; 5 = roof ; 6 = wheel ; 7 = top down ; 8 = ??? ; 9 = cinematic
		char pad_0124[8];                          //0x0124 (0x08)
		int32_t engine_state;                      //0x012C (0x04) 0 = not running ; 1 = starting ; 2 = running ; 3 = shutting down
		bool electrics_enabled;                    //0x0130 (0x01)
		char pad_0131[99];                         //0x0131 (0x63)
		int32_t wiper_direction;                   //0x0194 (0x04) 0 = not moving ; 1 = up ; -1 = down
		int32_t wiper_stick_mode;                  //0x0198 (0x04) 0 = off ; 1 = slow/auto ; 2 = medium ; 3 = fast
		int32_t wiper_mode;                        //0x019C (0x04) 0 = off ; 1 = slow ; 2 = medium ; 3 = fast
		char pad_01A0[20];                         //0x01A0 (0x14)
		int32_t light_state;                       //0x01B4 (0x04)
		int32_t aux_light_state;                   //0x01B8 (0x04)
		char pad_01BC[16];                         //0x01BC (0x10)
		float wiper_position;                      //0x01CC (0x04) 0 to 0.5 = moving up ; 0.5 to 1 = moving down
		char pad_01D0[72];                         //0x01D0 (0x48)
		float light_modes_button_gradual;          //0x0218 (0x04) 0 = off ; 0.5 = parking ; 1 = lowbeam
		float light_modes_button_instant;          //0x021C (0x04) 0 = off ; 0.5 = parking ; 1 = lowbeam
		float highbeams_button_gradual;            //0x0220 (0x04)
		float highbeams_button_instant;            //0x0224 (0x04)
		float light_horn_stick_gradual;            //0x0228 (0x04)
		float light_horn_stick_instant;            //0x022C (0x04)
		float indicator_stick_gradual;             //0x0230 (0x04) 0 = left ; 0.5 = none ; 1 = right
		float indicator_stick_instant;             //0x0234 (0x04) 0 = left ; 0.5 = none ; 1 = right
		float hazards_button_gradual;              //0x0238 (0x04)
		float hazards_button_instant;              //0x023C (0x04)
		float beacon_button_gradual;               //0x0240 (0x04)
		float beacon_button_instant;               //0x0244 (0x04)
		float key_state_gradual;                   //0x0248 (0x04) 0 = nothing ; 0.330 = electrics ; 0.670 = engine running ; 1 = engine starting
		float key_state_instant;                   //0x024C (0x04) 0 = nothing ; 0.330 = electrics ; 0.670 = engine running ; 1 = engine starting
		float handbrake_gradual;                   //0x0250 (0x04)
		float handbrake_instant;                   //0x0254 (0x04)
		float handbrake_electric_gradual;          //0x0258 (0x04) 0 = brake releasing ; 0.5 = mid/neutral/return ; 1 = brake applied
		float handbrake_electric_instant;          //0x025C (0x04) 0 = brake releasing ; 0.5 = mid/neutral/return ; 1 = brake applied
		float retarder_stick_gradual;              //0x0260 (0x04) 0 = none ; 0.333 = lvl 1 ; 0.667 = lvl 2 ; 1.000 = max
		float retarder_stick_instant;              //0x0264 (0x04) 0 = none ; 0.333 = lvl 1 ; 0.667 = lvl 2 ; 1.000 = max
		float engine_brake_gradual;                //0x0268 (0x04)
		float engine_brake_instant;                //0x026C (0x04)
		float gear_selector_gradual;               //0x0270 (0x04) 0 = reverse ; 0.5 = netural ; 1 = drive
		float gear_selector_instant;               //0x0274 (0x04) 0 = reverse ; 0.5 = netural ; 1 = drive
		float axel_lift_button;                    //0x0278 (0x04)
		float trailer_axel_lift_button;            //0x027C (0x04)
		float diff_lock_switch;                    //0x0280 (0x04) 0 = off ; 1 = on
		float wiper_speed_gradual;                 //0x0284 (0x04) 0 = off ; 0.333 = slow/auto ; 0.667 = medium ; 1 = fast
		float wiper_speed_instant;                 //0x0288 (0x04) 0 = off ; 0.333 = slow/auto ; 0.667 = medium ; 1 = fast
		float trailer_brake_gradual;               //0x028C (0x04) 0 = off ; 1 = on
		float trailer_brake_instant;               //0x0290 (0x04) 0 = off ; 1 = on
		float trailer_attached;                    //0x0294 (0x04) 0 = no trailer ; 1 = trailer
		float trailer_air_stick_gradual;           //0x0298 (0x04)
		float trailer_air_stick_instant;           //0x029C (0x04)
		int8_t left_window_state;                  //0x02A0 (0x01) 0 = left open ; 1 = window down ; 2 = window up ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		int8_t left_window_button_state;           //0x02A1 (0x01) 0 = not moving ; 1 = window moving auto ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		char pad_02A2[2];                          //0x02A2 (0x02)
		float left_window_position;                //0x02A4 (0x04) 0 = closed ; 1 = fully open
		float left_window_button_pressed_gradual;  //0x02A8 (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		float left_window_button_pressed_instant;  //0x02AC (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		char pad_02B0[8];                          //0x02B0 (0x08)
		int8_t right_window_state;                 //0x02B8 (0x01) 0 = left open ; 1 = window down ; 2 = window up ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		int8_t right_window_button_state;          //0x02B9 (0x01) 0 = not moving ; 1 = window moving auto ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		char pad_02BA[2];                          //0x02BA (0x02)
		float right_window_position;               //0x02BC (0x04) 0 = closed ; 1 = fully open
		float right_window_button_pressed_gradual; //0x02C0 (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		float right_window_button_pressed_instant; //0x02C4 (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		char pad_02C8[25];                         //0x02C8 (0x19)
		bool game_not_paused;                      //0x02E1 (0x01)
	};
	static_assert(sizeof(game_actor_u) == 0x2E2);
}

#pragma pack(pop)