#include "tick.h"
#include "global_variables.h"
#include "hooks_core.h"
#include "common.h"
#include "memory.h"

struct Vector3 {
    float x;
    float y;
    float z;
};

using namespace global_variables::audio;
using namespace global_variables::cvar;
using namespace global_variables::truck;
bool start_bad = false; // cannot be global

Vector3 truckRot;
Vector3 cameraPos;
Vector3 cameraRot;
uint64_t cameraPosValuePtr;

game_actor_u* game_actor;
unk_interior* interior;
core_camera_u* core_camera;

uint64_t base_ctrl_ptr;
uint32_t game_actor_offset;
uint64_t unk_interior_ptr;
uint64_t core_camera_ptr;

uint64_t game_base;
navigation_voice_event* last_played;

fmod_manager* fmod_manager_instance_;
scs_log_t scs_log__;

bool should_engine_brake_sound_play(telemetry_data_t* telemetry_data)
{
    return telemetry_data->truck.motor_brake && telemetry_data->truck.effective_throttle < 0.05f && telemetry_data->truck.displayed_gear != 0 &&
        telemetry_data->truck.effective_clutch < 0.05f;
}

void tick::init_tick(scs_log_t scs_log, fmod_manager* fmod_manager_instance, uint64_t game_base_, uint64_t base_ctrl_ptr_, uint64_t unk_interior_ptr_, uint32_t game_actor_offset_, uint64_t core_camera_ptr_)
{
    scs_log__ = scs_log;

    game_base = game_base_;

    base_ctrl_ptr = base_ctrl_ptr_;
    game_actor_offset = game_actor_offset_;
    unk_interior_ptr = unk_interior_ptr_;
    core_camera_ptr = core_camera_ptr_;

    fmod_manager_instance_ = fmod_manager_instance;
}


void handle_volume(telemetry_data_t* telemetry_data)
{
    // Background Audio:
    int suspend_sound = prism::cvar::get_value(s_suspend_sound);
    if (suspend_sound == 1)
    {
        DWORD game_processID = GetCurrentProcessId();

        HWND hwnd = GetForegroundWindow();

        DWORD current_app_processId;
        GetWindowThreadProcessId(hwnd, &current_app_processId);

        if (game_processID != current_app_processId) fmod_manager_instance_->set_minimised(true);
        else fmod_manager_instance_->set_minimised(false);
    }
    else fmod_manager_instance_->set_minimised(false);


    // Handle Levels:
    const float master_volume = prism::cvar::get_value(s_master_volume);
    const float navigation_volume = prism::cvar::get_value(s_navigation_volume);
    const float music_volume = prism::cvar::get_value(s_ui_music_volume);
    const float engine_volume = prism::cvar::get_value(s_truck_engine_volume);
    const float turbo_volume = prism::cvar::get_value(s_truck_turbo_volume);
    const float exhaust_volume = prism::cvar::get_value(s_truck_exhaust_volume);
    const float effects_volume = prism::cvar::get_value(s_truck_effects_volume);
    const float interior_volume = prism::cvar::get_value(s_interior_volume);

    std::stringstream ss;
    std::ostringstream volume_stream;

    if (master_volume != current_master_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << master_volume;
        ss << "[ts-fmod-plugin-v2] Setting master volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("", master_volume);
        current_master_volume = master_volume;
    }

    if (navigation_volume != current_navigation_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << navigation_volume;
        ss << "[ts-fmod-plugin-v2] Setting navigation volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("game/navigation", navigation_volume);
        current_navigation_volume = navigation_volume;
    }

    if (music_volume != current_music_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << music_volume;
        ss << "[ts-fmod-plugin-v2] Setting menu music volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("game/ui_music", music_volume);
        current_music_volume = music_volume;
    }

    if (engine_volume != current_engine_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << engine_volume;
        ss << "[ts-fmod-plugin-v2] Setting engine volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("outside/exterior/truck_engine", engine_volume);
        current_engine_volume = engine_volume;
    }

    if (turbo_volume != current_turbo_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << turbo_volume;
        ss << "[ts-fmod-plugin-v2] Setting turbo volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("outside/exterior/truck_turbo", turbo_volume);
        current_turbo_volume = turbo_volume;
    }

    if (exhaust_volume != current_exhaust_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << exhaust_volume;
        ss << "[ts-fmod-plugin-v2] Setting exhaust volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("outside/exterior/truck_exhaust", exhaust_volume);
        current_exhaust_volume = exhaust_volume;
    }

    if (effects_volume != current_effects_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << effects_volume;
        ss << "[ts-fmod-plugin-v2] Setting effects volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("outside/exterior/truck_effects", effects_volume);
        current_effects_volume = effects_volume;
    }

    if (interior_volume != current_interior_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << interior_volume;
        ss << "[ts-fmod-plugin-v2] Setting interior volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        fmod_manager_instance_->set_bus_volume("cabin/interior", interior_volume);
        current_interior_volume = interior_volume;
    }

    if (game_actor != nullptr)
    {
        if (core_camera->vehicle_camera != nullptr)
        {
            if (core_camera->vehicle_camera->chase != nullptr)
            {
                if (game_actor->current_camera == 1 && !paused)
                {
                    float engine_volume_distance = engine_volume;
                    float turbo_volume_distance = turbo_volume;
                    float exhaust_volume_distance = exhaust_volume;
                    float effects_volume_distance = effects_volume;

                    float distance_from_truck = core_camera->vehicle_camera->chase->distance;
                    float min_distance = 4.0f;

                    auto adjust_volume = [&](float& volume_distance, float scaling_factor) {
                        if (volume_distance > 0) {
                            float factor = min_distance / distance_from_truck;
                            volume_distance *= 1 + (factor - 1) * scaling_factor;
                        }
                        else {
                            volume_distance = 0;
                        }

                        if (volume_distance < 0) { volume_distance = 0; }
                    };

                    if (distance_from_truck != min_distance) {
                        adjust_volume(engine_volume_distance, 0.55);
                        adjust_volume(turbo_volume_distance, 0.55);
                        adjust_volume(exhaust_volume_distance, 0.55);
                        adjust_volume(effects_volume_distance, 0.55);
                    }

                   // scs_log__(0, std::to_string(distance_from_truck).c_str());
                   // scs_log__(0, std::to_string(engine_volume_distance).c_str());

                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_engine", engine_volume_distance);
                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_turbo", turbo_volume_distance);
                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_exhaust", exhaust_volume_distance);
                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_effects", effects_volume_distance);
                }
                else
                {
                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_engine", engine_volume);
                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_turbo", turbo_volume);
                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_exhaust", exhaust_volume);
                    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_effects", effects_volume);
                }
            }
        }
    }
}

void handle_engine(telemetry_data_t* telemetry_data)
{
    fmod_manager_instance_->set_global_parameter("trans_rpm", telemetry_data->truck.engine_rpm);
    fmod_manager_instance_->set_global_parameter("park_brake", telemetry_data->truck.parking_brake);
    fmod_manager_instance_->set_event_parameter("engine/engine", "rpm", telemetry_data->truck.engine_rpm);
    fmod_manager_instance_->set_event_parameter("engine/exhaust", "rpm", telemetry_data->truck.engine_rpm);
    fmod_manager_instance_->set_event_parameter("engine/engine", "load", telemetry_data->truck.effective_throttle);
    fmod_manager_instance_->set_event_parameter("engine/engine", "engine_load", telemetry_data->truck.effective_throttle);
    fmod_manager_instance_->set_event_parameter("engine/exhaust", "load", telemetry_data->truck.effective_throttle);

    if (game_actor != nullptr)
    {
        air_pressure = game_actor->air_pressure;
        fmod_manager_instance_->set_global_parameter("air_pressure", air_pressure);

        const auto turbo_pressure = game_actor->turbo_pressure;
        if (turbo_pressure >= 0 && turbo_pressure <= 1)
        {
            fmod_manager_instance_->set_event_parameter("engine/turbo", "turbo", turbo_pressure);
        }

        const auto engine_state = game_actor->engine_state;
        if (engine_state != stored_engine_state)
        {
            if (!start_bad && engine_state > 0 && stored_engine_state == 0)
            {
                // engine is starting/running
                fmod_manager_instance_->set_event_parameter("engine/engine", "engine_state", engine_state);

                fmod_manager_instance_->set_event_parameter("engine/engine", "play", 1);
                fmod_manager_instance_->set_event_parameter("engine/exhaust", "play", 1);
                fmod_manager_instance_->set_event_state("engine/engine", true);
                fmod_manager_instance_->set_event_state("engine/exhaust", true);

                fmod_manager_instance_->set_event_parameter("engine/turbo", "play", 1);
                fmod_manager_instance_->set_event_state("engine/turbo", true);

                start_bad = false;
                engineRunning = true;
            }
            else if (engine_state == 0 || engine_state == 3) // engine is no longer running
            {
                fmod_manager_instance_->set_event_parameter("engine/engine", "play", 0);
                fmod_manager_instance_->set_event_parameter("engine/exhaust", "play", 0);
                fmod_manager_instance_->set_event_parameter("engine/turbo", "play", 0);
                start_bad = false;
                engineRunning = false;
            }
            stored_engine_state = engine_state;
        }

        fmod_manager_instance_->set_event_parameter("engine/engine", "brake",
            should_engine_brake_sound_play(telemetry_data) ? game_actor->engine_brake_state : 0.0f);

    }
}

void handle_truck_effects(telemetry_data_t* telemetry_data)
{
    if (engineRunning)
    {
        if (telemetry_data->truck.displayed_gear != lastGear)
        {
            fmod_manager_instance_->set_event_state("effects/air_gear", true);
        }
    }

    lastGear = telemetry_data->truck.displayed_gear;

    int reverse_enabled = prism::cvar::get_value(s_reverse_enabled);
    if (reverse_enabled == 1)
    {
        if (telemetry_data->truck.light_reverse)
        {
            fmod_manager_instance_->set_event_state("effects/reverse", true, true);
        }
        else
        {
            fmod_manager_instance_->set_event_state("effects/reverse", false);
        }
    }
}

void handle_interior(telemetry_data_t* telemetry_data)
{
    if (game_actor != nullptr)
    {
        const auto hazard_warning = game_actor->hazard_warning_state;
        if (!common::cmpf(hazard_warning, hazard_warning_state))
        {
            fmod_manager_instance_->set_event_state("interior/stick_hazard_warning", true);
            hazard_warning_state = hazard_warning;
        }

        const auto light_horn = game_actor->light_horn_state;
        if (!common::cmpf(light_horn, light_horn_state))
        {
            fmod_manager_instance_->set_event_state("interior/stick_light_horn", true);
            light_horn_state = light_horn;
        }

        auto stick_lights = game_actor->light_switch_state;
        if (stick_lights != light_stick_state)
        {
            fmod_manager_instance_->set_event_state("interior/stick_lights", true);
            light_stick_state = stick_lights;
        }

        const auto wipers_stick = game_actor->wipers_state;
        if (!common::cmpf(wipers_stick, wipers_stick_state))
        {
            fmod_manager_instance_->set_event_state("interior/stick_wipers", true);
            wipers_stick_state = wipers_stick;
        }

        if (game_actor->wipers_position > 0 && game_actor->wipers_position < 0.5)
        {
            // scs calls `wipers_up` when its moving down

            fmod_manager_instance_->set_event_state("interior/wipers_up", false);
            fmod_manager_instance_->set_event_state("interior/wipers_down", true, true);
        }
        else if (game_actor->wipers_position >= 0.5)
        {
            fmod_manager_instance_->set_event_state("interior/wipers_down", false);
            fmod_manager_instance_->set_event_state("interior/wipers_up", true, true);
        }
        else
        {
            fmod_manager_instance_->set_event_state("interior/wipers_up", false);
            fmod_manager_instance_->set_event_state("interior/wipers_down", false);
        }

        if ((game_actor->is_left_window_moving != 0 || game_actor->is_right_window_moving != 0) &&
            (game_actor->left_window_moving_direction > 2 || game_actor->right_window_moving_direction > 2))
        {
            is_window_moving = true;
            fmod_manager_instance_->set_global_parameter("window_stop", 0.f);
            fmod_manager_instance_->set_event_state("interior/window_move", true, true);

            if (game_actor->is_left_window_moving != 0)
            {
                //fmod_manager_instance_->set_event_3d_posrot("interior/window_move", -1.f);
            }
            else if (game_actor->is_right_window_moving != 0)
            {
                //fmod_manager_instance_->set_event_3d_posrot("interior/window_move", 1.f);
            }
        }
        else if (is_window_moving)
        {
            is_window_moving = false;
            fmod_manager_instance_->set_global_parameter("window_stop", 1.0f);
            fmod_manager_instance_->set_event_state("interior/window_move", false);
        }

        if ((game_actor->left_window_state == 1.f && game_actor->left_window_btn_state == 1.f) ||
            (game_actor->left_window_state == 0.f && game_actor->left_window_btn_state == 0.f))
        {
            if (!is_left_window_button_active)
            {
                fmod_manager_instance_->set_event_state("interior/window_click", true, false);
            }
            is_left_window_button_active = true;
        }
        else if (is_left_window_button_active)
        {
            is_left_window_button_active = false;
        }
        if ((game_actor->right_window_state == 1.f && game_actor->right_window_btn_state == 1.f) ||
            (game_actor->right_window_state == 0.f && game_actor->right_window_btn_state == 0.f))
        {
            if (!is_right_window_button_active)
            {
                fmod_manager_instance_->set_event_state("interior/window_click", true, false);
            }
            is_right_window_button_active = true;
        }
        else if (is_right_window_button_active)
        {
            is_right_window_button_active = false;
        }
    }

    if (interior != nullptr)
    {
        const auto window_pos = interior->window_state;
        if (window_pos.x >= 0 && window_pos.x <= 1) fmod_manager_instance_->set_global_parameter("wnd_left", window_pos.x);
        if (window_pos.y >= 0 && window_pos.y <= 1) fmod_manager_instance_->set_global_parameter("wnd_right", window_pos.y);

        if (interior->is_camera_inside)
        {
            auto window_volume = 0.5;

            float leftWindowBoost = ((window_pos.x / 2) / 2) / 2;
            float rightWindowBoost = ((window_pos.y / 2) / 2) / 2;

            float finalVolume = window_volume + (leftWindowBoost + rightWindowBoost);

            fmod_manager_instance_->set_bus_volume("outside", finalVolume);
            fmod_manager_instance_->set_bus_volume("outside/exterior", finalVolume);
            fmod_manager_instance_->set_bus_volume("exterior", finalVolume);
        }
        else
        {
            fmod_manager_instance_->set_bus_volume("outside", 1);
            fmod_manager_instance_->set_bus_volume("outside/exterior", 1);
            fmod_manager_instance_->set_bus_volume("exterior", 1);

        }

        if (interior->interior_camera)
        {
            fmod_manager_instance_->set_bus_volume("cabin/interior", current_interior_volume);
        }
        else
        {
            fmod_manager_instance_->set_bus_volume("cabin/interior", 0);
        }

        fmod_manager_instance_->set_global_parameter("cabin_out", interior->cabin_out);
        fmod_manager_instance_->set_global_parameter("cabin_type", 1); // improves the cabin sound?? honestly im not sure but i think it does
        fmod_manager_instance_->set_global_parameter("cabin_rot", interior->camera_rotation_in_cabin);
        fmod_manager_instance_->set_global_parameter("surr_type", interior->should_have_echo);
        fmod_manager_instance_->set_global_parameter("daytime", interior->daytime);


        const auto now_playing_navigation_sound = interior->now_playing_navigation_sound;
        if (now_playing_navigation_sound != nullptr && last_played != now_playing_navigation_sound)
        {
            fmod_manager_instance_->set_event_state(now_playing_navigation_sound->event_name, true, true);
        }
        last_played = now_playing_navigation_sound;
    }



    int hardcore_simulation = prism::cvar::get_value(g_hardcore_simulation);
    if (hardcore_simulation == 1)
    {
        if (telemetry_data->truck.brake_air_pressure_warning && telemetry_data->truck.engine_enabled)
        {
            is_air_pressure_warning_on = true;
            fmod_manager_instance_->set_event_state("interior/air_warning", true, true);
        }
        else if (is_air_pressure_warning_on)
        {
            is_air_pressure_warning_on = false;
            fmod_manager_instance_->set_event_state("interior/air_warning", false);
        }
    }

    if ((telemetry_data->truck.light_lblinker || telemetry_data->truck.light_rblinker) && !was_indicator_light_on)
    {
        fmod_manager_instance_->set_event_state("interior/blinker_on", true);
        was_indicator_light_on = true;
    }
    else if (!telemetry_data->truck.light_lblinker && !telemetry_data->truck.light_rblinker && was_indicator_light_on)
    {
        fmod_manager_instance_->set_event_state("interior/blinker_off", true);
        was_indicator_light_on = false;
    }

    if (telemetry_data->truck.parking_brake != was_park_brake_on)
    {
        if (was_park_brake_on) fmod_manager_instance_->set_event_state("interior/stick_park_brake_off", true);
        else
        {
            fmod_manager_instance_->set_event_state("interior/stick_park_brake", true);
            if (air_pressure != 0)
            {
                fmod_manager_instance_->set_event_state("effects/air_brake", true);
            }
        }

        was_park_brake_on = telemetry_data->truck.parking_brake;
    }

    const byte current_blinker_stick = telemetry_data->truck.lblinker ? 1 : telemetry_data->truck.rblinker ? 2 : 0;
    // 1 if lblinker, 2 if rblinker, 0 if off
    if (current_blinker_stick != indicator_stick_state && current_blinker_stick != 0)
    {
        fmod_manager_instance_->set_event_state("interior/stick_blinker", true);
        indicator_stick_state = current_blinker_stick;
    }
    else if (current_blinker_stick == 0 && indicator_stick_state != 0)
    {
        fmod_manager_instance_->set_event_state("interior/stick_blinker_off", true);
        indicator_stick_state = 0;
    }

    if (telemetry_data->truck.retarder_level != prev_retarder_level)
    {
        fmod_manager_instance_->set_event_state("interior/stick_retarder", true, true);
        prev_retarder_level = telemetry_data->truck.retarder_level;
    }

    if (telemetry_data->truck.light_high_beam != high_beams_enabled)
    {
        fmod_manager_instance_->set_event_state("interior/stick_high_beam", true);
        high_beams_enabled = telemetry_data->truck.light_high_beam;
    }

}

bool gotPointer = false;
SCSAPI_VOID tick::telemetry_tick(const scs_event_t event, const void* const event_info, const scs_context_t context)
{
    telemetry_data_t* telemetry_data = (telemetry_data_t*)context;

    const auto base_ctrl_address = *reinterpret_cast<uint64_t*>(base_ctrl_ptr);
    const auto game_actor_ptr = base_ctrl_address + game_actor_offset;
    game_actor = *reinterpret_cast<game_actor_u**>(game_actor_ptr);

    interior = *reinterpret_cast<unk_interior**>(unk_interior_ptr);

    core_camera = *reinterpret_cast<core_camera_u**>(core_camera_ptr);

    if (fmod_manager_instance_ == nullptr)
    {
        scs_log__(2, "[ts-fmod-plugin-v2] Fatal Error! cannot get 'fmod_manager_instance_'!");
        return;
    }

    if (telemetry_data == nullptr)
    {
        scs_log__(2, "[ts-fmod-plugin-v2] Fatal Error! cannot get 'telemetry_data'!");
        return;
    }

    bool development = false;

    if (development)
    {
        std::stringstream ss;
        ss << "game_actor: " << std::hex << game_actor;
        ss << "\ncurrent_camera: " << game_actor->current_camera;

        size_t offset = (reinterpret_cast<size_t>(&(game_actor->current_camera)) - reinterpret_cast<size_t>(game_actor));
        ss << "\ncurrent_camera offset: " << std::hex << offset;
        scs_log__(0, ss.str().c_str());
    }

    handle_volume(telemetry_data); 
    handle_engine(telemetry_data);
    handle_truck_effects(telemetry_data);
    handle_interior(telemetry_data);

    fmod_manager_instance_->update(); 
}