#pragma once

#include "../unit.h"

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

	class game_actor_u : public unit_t // Size: 0x21B8
	{
	public:
		char pad_0010[8]; //0x0010 (0x08)
		class game_physics_vehicle_u* current_truck; //0x0018 (0x08)
		class visual_interior_u* visual_interior; //0x0020 (0x08)
		char pad_0028[8]; //0x0028 (0x08)
		void* hud_actor; //0x0030 (0x08) academy_game_actor_hud if academy
		char pad_0038[48]; //0x0038 (0x30)
		float N00000FE6; //0x0068 (0x04)
		float N00020750; //0x006C (0x04)
		char pad_0070[8]; //0x0070 (0x08)
		uint8_t engine_running; //0x0078 (0x01)
		char pad_0079[3]; //0x0079 (0x03)
		float engine_rpm; //0x007C (0x04)
		float air_pressure; //0x0080 (0x04)
		float brake_pressure; //0x0084 (0x04) for some reason this only works on the electric trucks????
		char pad_0088[4]; //0x0088 (0x04)
		float turbo_pressure; //0x008C (0x04)
		float retarder_level; //0x0090 (0x04) 0 = none ; 0.333 = lvl 1 ; 0.667 = lvl 2 ; 1.000 = max
		char pad_0094[20]; //0x0094 (0x14)
		void* history_stream_set; //0x00A8 (0x08)
		char pad_00B0[16]; //0x00B0 (0x10)
		void* history_manager; //0x00C0 (0x08)
		class _temp_game_trailer_actor_u* current_trailer; //0x00C8 (0x08)
		char pad_00D0[116]; //0x00D0 (0x74)
		int32_t current_camera; //0x0144 (0x04) 0 = dev ; 1 = orbit ; 2 = int ; 3 = front ; 4 = out window ; 5 = roof ; 6 = wheel ; 7 = top down ; 8 = ??? ; 9 = cinematic
		char pad_0148[8]; //0x0148 (0x08)
		int32_t engine_state; //0x0150 (0x04) 0 = not running ; 1 = starting ; 2 = running ; 3 = shutting down
		bool electrics_enabled; //0x0154 (0x01)
		char pad_0155[103]; //0x0155 (0x67)
		int32_t wiper_direction; //0x01BC (0x04) 0 = not moving ; 1 = up ; -1 = down
		int32_t wiper_stick_mode; //0x01C0 (0x04) 0 = off ; 1 = slow/auto ; 2 = medium ; 3 = fast
		int32_t wiper_mode; //0x01C4 (0x04) 0 = off ; 1 = slow ; 2 = medium ; 3 = fast
		char pad_01C8[8]; //0x01C8 (0x08)
		int32_t actor_state; //0x01D0 (0x04)
		int32_t aux_light_state; //0x01D4 (0x04)
		char pad_01D8[8]; //0x01D8 (0x08)
		float indicator_timer; //0x01E0 (0x04) force -100000000 to freeze
		char pad_01E4[4]; //0x01E4 (0x04)
		float wiper_position; //0x01E8 (0x04) 0 to 0.5 = moving up ; 0.5 to 1 = moving down
		char pad_01EC[304]; //0x01EC (0x130)
		float light_modes_button_gradual; //0x031C (0x04) 0 = off ; 0.5 = parking ; 1 = lowbeam
		float light_modes_button_instant; //0x0320 (0x04) 0 = off ; 0.5 = parking ; 1 = lowbeam
		float highbeams_button_gradual; //0x0324 (0x04)
		float highbeams_button_instant; //0x0328 (0x04)
		float light_horn_stick_gradual; //0x032C (0x04)
		float light_horn_stick_instant; //0x0330 (0x04)
		float indicator_stick_gradual; //0x0334 (0x04) 0 = left ; 0.5 = none ; 1 = right
		float indicator_stick_instant; //0x0338 (0x04) 0 = left ; 0.5 = none ; 1 = right
		float hazards_button_gradual; //0x033C (0x04)
		float hazards_button_instant; //0x0340 (0x04)
		float beacon_button_gradual; //0x0344 (0x04)
		float beacon_button_instant; //0x0348 (0x04)
		float key_state_gradual; //0x034C (0x04) 0 = nothing ; 0.330 = electrics ; 0.670 = engine running ; 1 = engine starting
		float key_state_instant; //0x0350 (0x04) 0 = nothing ; 0.330 = electrics ; 0.670 = engine running ; 1 = engine starting
		float handbrake_gradual; //0x0354 (0x04)
		float handbrake_instant; //0x0358 (0x04)
		char pad_035C[16]; //0x035C (0x10)
		float retarder_stick_gradual; //0x036C (0x04) 0 = none ; 0.333 = lvl 1 ; 0.667 = lvl 2 ; 1.000 = max
		float retarder_stick_instant; //0x0370 (0x04) 0 = none ; 0.333 = lvl 1 ; 0.667 = lvl 2 ; 1.000 = max
		float engine_brake_gradual; //0x0374 (0x04)
		float engine_brake_instant; //0x0378 (0x04)
		float gear_selector_gradual; //0x037C (0x04) 0 = reverse ; 0.5 = netural ; 1 = drive
		float gear_selector_instant; //0x0380 (0x04) 0 = reverse ; 0.5 = netural ; 1 = drive
		float axel_lift_button; //0x0384 (0x04)
		float trailer_axel_lift_button; //0x0388 (0x04)
		float diff_lock_switch; //0x038C (0x04) 0 = off ; 1 = on
		float wiper_speed_gradual; //0x0390 (0x04) 0 = off ; 0.333 = slow/auto ; 0.667 = medium ; 1 = fast
		float wiper_speed_instant; //0x0394 (0x04) 0 = off ; 0.333 = slow/auto ; 0.667 = medium ; 1 = fast
		float trailer_brake_gradual; //0x0398 (0x04) 0 = off ; 1 = on
		float trailer_brake_instant; //0x039C (0x04) 0 = off ; 1 = on
		char pad_03A0[4]; //0x03A0 (0x04)
		float trailer_air_stick_gradual; //0x03A4 (0x04)
		float trailer_air_stick_instant; //0x03A8 (0x04)
		int8_t left_window_state; //0x03AC (0x01) 0 = left open ; 1 = window down ; 2 = window up ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		int8_t left_window_button_state; //0x03AD (0x01) 0 = not moving ; 1 = window moving auto ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		char pad_03AE[2]; //0x03AE (0x02)
		float left_window_position; //0x03B0 (0x04) 0 = closed ; 1 = fully open
		float left_window_button_pressed_gradual; //0x03B4 (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		float left_window_button_pressed_instant; //0x03B8 (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		char pad_03BC[8]; //0x03BC (0x08)
		int8_t right_window_state; //0x03C4 (0x01) 0 = left open ; 1 = window down ; 2 = window up ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		int8_t right_window_button_state; //0x03C5 (0x01) 0 = not moving ; 1 = window moving auto ; 16 = moving down (manual + auto) ; 32 = moving up (manual + auto)
		char pad_03C6[2]; //0x03C6 (0x02)
		float right_window_position; //0x03C8 (0x04) 0 = closed ; 1 = fully open
		float right_window_button_pressed_gradual; //0x03CC (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		float right_window_button_pressed_instant; //0x03D0 (0x04) 1 = button down ; 0 = button up ; 0.5 = button not pressed
		char pad_03D4[6288]; //0x03D4 (0x1890)
		float rpm; //0x1C64 (0x04)
		float load; //0x1C68 (0x04)
		char pad_1C6C[1356]; //0x1C6C (0x54c)
	};
	static_assert(sizeof(game_actor_u) == 0x21B8);
}

#pragma pack(pop)