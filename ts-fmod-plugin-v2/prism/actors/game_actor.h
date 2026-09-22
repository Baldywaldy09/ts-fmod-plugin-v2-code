#pragma once

#include <cstddef>
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

	// Offsets for ETS2/ATS 1.61 (Windows)
	class game_actor_u : public unit_t
	{
	public:
		char pad_0010[0x19C]; //0x0010
		int32_t current_camera; //0x01AC 0 = dev ; 1 = orbit ; 2 = int ; ...
		char pad_01B0[8]; //0x01B0
		int32_t engine_state; //0x01B8 0 = not running ; 1 = starting ; 2 = running ; 3 = shutting down
		bool electrics_enabled; //0x01BC
		char pad_01BD[0x6F]; //0x01BD
		int32_t wiper_direction; //0x022C 0 = not moving ; 1 = up ; -1 = down
		int32_t wiper_stick_mode; //0x0230
		char pad_0234[0x28]; //0x0234
		float wiper_position; //0x025C
		char pad_0260[0xF0]; //0x0260
		float air_pressure; //0x0350
		char pad_0354[0x2C]; //0x0354
		float turbo_pressure; //0x0380
		char pad_0384[0x10]; //0x0384
		float light_modes_button_gradual; //0x0394 0 = off ; 0.5 = parking ; 1 = lowbeam
		float light_modes_button_instant; //0x0398
		float highbeams_button_gradual; //0x039C
		float highbeams_button_instant; //0x03A0
		float light_horn_stick_gradual; //0x03A4
		float light_horn_stick_instant; //0x03A8
		float indicator_stick_gradual; //0x03AC
		float indicator_stick_instant; //0x03B0
		float hazards_button_gradual; //0x03B4
		float hazards_button_instant; //0x03B8
		float beacon_button_gradual; //0x03BC
		float beacon_button_instant; //0x03C0
		char pad_03C4[8]; //0x03C4
		float handbrake_gradual; //0x03CC
		float handbrake_instant; //0x03D0
		char pad_03D4[0x18]; //0x03D4
		float engine_brake_gradual; //0x03EC
		float engine_brake_instant; //0x03F0
		char pad_03F4[0x14]; //0x03F4
		float wiper_speed_gradual; //0x0408
		float wiper_speed_instant; //0x040C
		char pad_0410[0x24]; //0x0410
		int8_t left_window_state; //0x0434 moving direction / end state
		int8_t left_window_button_state; //0x0435
		char pad_0436[2]; //0x0436
		float left_window_position; //0x0438 0 = closed ; 1 = fully open
		float left_window_button_pressed_gradual; //0x043C
		float left_window_button_pressed_instant; //0x0440
		char pad_0444[8]; //0x0444
		int8_t right_window_state; //0x044C
		int8_t right_window_button_state; //0x044D
		char pad_044E[2]; //0x044E
		float right_window_position; //0x0450
		float right_window_button_pressed_gradual; //0x0454
		float right_window_button_pressed_instant; //0x0458
	};
	static_assert(offsetof(game_actor_u, engine_state) == 0x01B8);
	static_assert(offsetof(game_actor_u, air_pressure) == 0x0350);
	static_assert(offsetof(game_actor_u, turbo_pressure) == 0x0380);
	static_assert(offsetof(game_actor_u, light_modes_button_instant) == 0x0398);
	static_assert(offsetof(game_actor_u, engine_brake_instant) == 0x03F0);
	static_assert(offsetof(game_actor_u, wiper_speed_instant) == 0x040C);
	static_assert(offsetof(game_actor_u, left_window_state) == 0x0434);
	static_assert(offsetof(game_actor_u, right_window_button_pressed_instant) == 0x0458);
}

#pragma pack(pop)
