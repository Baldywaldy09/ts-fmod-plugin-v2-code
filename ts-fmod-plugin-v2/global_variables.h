#pragma once
#include "prism/cvar.h"

namespace global_variables
{

    namespace audio
    {
        inline float current_master_volume = 0.f;
        inline float current_navigation_volume = 0.f;
        inline float current_music_volume = 0.f;
        inline float current_engine_volume = 0.f;
        inline float current_turbo_volume = 0.f;
        inline float current_exhaust_volume = 0.f;
        inline float current_effects_volume = 0.f;
        inline float current_interior_volume = 0.f;

        inline bool menu_music = false;
        inline bool navigation = false;
    };

    namespace cvar
    {
        inline prism::cvar::pointer s_master_volume;
        inline prism::cvar::pointer s_truck_engine_volume;
        inline prism::cvar::pointer s_truck_engine_mute;
        inline prism::cvar::pointer s_truck_turbo_volume;
        inline prism::cvar::pointer s_truck_turbo_mute;
        inline prism::cvar::pointer s_truck_exhaust_volume;
        inline prism::cvar::pointer s_truck_exhaust_mute;
        inline prism::cvar::pointer s_truck_effects_volume;
        inline prism::cvar::pointer s_interior_volume;
        inline prism::cvar::pointer s_interior_mute;
        inline prism::cvar::pointer s_ui_music_volume;
        inline prism::cvar::pointer s_ui_music_mute;
        inline prism::cvar::pointer s_navigation_volume;
        inline prism::cvar::pointer s_navigation_mute;
        inline prism::cvar::pointer g_voice_navigation;
        inline prism::cvar::pointer g_voice_navigation_pack;
        inline prism::cvar::pointer g_hardcore_simulation;
        inline prism::cvar::pointer s_reverse_enabled;
        inline prism::cvar::pointer s_suspend_sound;
    };

    namespace truck
    {
        inline std::string lastTruckName = "None";
        inline uint32_t stored_engine_state = 0;
        inline byte indicator_stick_state = false;
        inline bool was_indicator_light_on = false;
        inline bool was_park_brake_on = false;
        inline uint32_t prev_retarder_level = 0;
        inline bool high_beams_enabled = false;
        inline bool wipers_enabled = false;
        inline float hazard_warning_state = 0.0f;
        inline float light_horn_state = 0.0f;
        inline float light_stick_state = 0.0f;
        inline float wipers_stick_state = 0.0f;
        inline float last_left_window_pos = 0.f;
        inline float last_right_window_pos = 0.f;
        inline float air_pressure = 0.f;
        inline bool is_air_pressure_warning_on = true;
        inline bool is_window_moving = false;
        inline bool is_left_window_button_active = false;
        inline bool is_right_window_button_active = false;
        inline bool was_break_on = false;
        inline bool should_play_break_sound = false;
        inline bool should_play_trailer_connect_sound = false;
        inline bool should_play_trailer_disconnect_sound = false;
        inline bool paused = true;
        inline bool engineRunning = false;
        inline int lastGear = 0;
    };
};