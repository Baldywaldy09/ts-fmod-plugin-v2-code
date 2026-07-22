#include "tick.h"
#include "global_variables.h"
#include "hooks_core.h"
#include "common.h"
#include "memory.h"
#include "bmem.h"
#include <fmod/fmod_errors.h>

#include "prism\common\vectors.h"
#include "prism\common\arrays.h"
#include "prism\common\strings.h"
#include "prism\sound\sound_event.h"
#include "prism\sound\navigation.h"
#include "prism\vehicle_camera\vehicle_behind_rotation_camera.h"

#include "prism\actors\game_actor.h"
#include "prism\actors\game_ctrl.h"
#include "prism\actors\camera_manager.h"
#include "prism\actors\game_sound_data.h"

using namespace global_variables::audio;
using namespace global_variables::cvar;
using namespace global_variables::truck;
bool start_bad = false; // cannot be global
unsigned long long engineSoundEventPtr; // cannot be global

prism::vector_3 truckRot;
prism::vector_3 cameraPos;
prism::vector_3 cameraRot;

prism::navigation_sound_event* last_played;

fmod_manager* fmod_manager_instance_;
scs_log_t scs_log__;

bool should_engine_brake_sound_play(telemetry_data_t* telemetry_data)
{
    return telemetry_data->truck.motor_brake && telemetry_data->truck.effective_throttle < 0.05f && telemetry_data->truck.displayed_gear != 0 &&
        telemetry_data->truck.effective_clutch < 0.05f;
}

void tick::init_tick(scs_log_t scs_log, fmod_manager* fmod_manager_instance)
{
    scs_log__ = scs_log;

    fmod_manager_instance_ = fmod_manager_instance;
}

void handle_volume(telemetry_data_t* telemetry_data, prism::camera_manager_u* camera_manager)
{
    // Background Audio:

    int suspend_sound = s_suspend_sound->current_value_int;
    if (suspend_sound == 1)
    {
        DWORD game_processID = GetCurrentProcessId();

        HWND hwnd = GetForegroundWindow();

        DWORD current_app_processId;
        GetWindowThreadProcessId(hwnd, &current_app_processId);

        if (game_processID != current_app_processId) {
            fmod_manager_instance_->set_minimised(true);
            return;
        }
        else fmod_manager_instance_->set_minimised(false);
    }
    else fmod_manager_instance_->set_minimised(false);


    // Handle Levels:
    const float master_volume = s_master_volume->current_value_float;
    const float navigation_volume = s_navigation_volume->current_value_float;
    const float music_volume = s_ui_music_volume->current_value_float;
    const float engine_volume = s_truck_engine_volume->current_value_float;
    const float turbo_volume = s_truck_turbo_volume->current_value_float;
    const float exhaust_volume = s_truck_exhaust_volume->current_value_float;
    const float effects_volume = s_truck_effects_volume->current_value_float;
    const float interior_volume = s_interior_volume->current_value_float;

    fmod_manager_instance_->set_bus_volume("", master_volume);
    fmod_manager_instance_->set_bus_volume("game/navigation", navigation_volume);
    fmod_manager_instance_->set_bus_volume("game/ui_music", music_volume);
    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_engine", engine_volume);
    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_turbo", turbo_volume);
    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_exhaust", exhaust_volume);
    fmod_manager_instance_->set_bus_volume("outside/exterior/truck_effects", effects_volume);
    fmod_manager_instance_->set_bus_volume("cabin/interior", interior_volume);

    std::stringstream ss;
    std::ostringstream volume_stream;

    if (master_volume != current_master_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << master_volume;
        ss << "[ts-fmod-plugin-v2] Setting master volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        current_master_volume = master_volume;
    }

    if (navigation_volume != current_navigation_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << navigation_volume;
        ss << "[ts-fmod-plugin-v2] Setting navigation volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        current_navigation_volume = navigation_volume;
    }

    if (music_volume != current_music_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << music_volume;
        ss << "[ts-fmod-plugin-v2] Setting menu music volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        current_music_volume = music_volume;
    }

    if (engine_volume != current_engine_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << engine_volume;
        ss << "[ts-fmod-plugin-v2] Setting engine volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        current_engine_volume = engine_volume;
    }

    if (turbo_volume != current_turbo_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << turbo_volume;
        ss << "[ts-fmod-plugin-v2] Setting turbo volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        current_turbo_volume = turbo_volume;
    }

    if (exhaust_volume != current_exhaust_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << exhaust_volume;
        ss << "[ts-fmod-plugin-v2] Setting exhaust volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

        current_exhaust_volume = exhaust_volume;
    }

    if (effects_volume != current_effects_volume)
    {
        volume_stream.str("");
        ss.str("");

        volume_stream << std::fixed << std::setprecision(2) << effects_volume;
        ss << "[ts-fmod-plugin-v2] Setting effects volume to: " << volume_stream.str();
        scs_log__(0, ss.str().c_str());

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

    if (camera_manager)
    {
        if (camera_manager->current_camera == 1 && !paused)
        {
            prism::vehicle_behind_rotation_camera_u* chase_camera = nullptr;

            if (camera_manager->vehicle_cameras.size >= 2)
            {
                chase_camera = (prism::vehicle_behind_rotation_camera_u*)camera_manager->vehicle_cameras[1];

                if (std::string(chase_camera->get_unit_descriptor()->class_name->value) != "vehicle_behind_rotation_camera")
                {
                    scs_log__(2, "[ts-fmod-plugin-v2][Prism3D] camera_manager->vehicle_cameras[1] returned a invalid camera type!");


                    std::string got = std::string(chase_camera->get_unit_descriptor()->class_name->value);
                    scs_log__(2, ("[ts-fmod-plugin-v2][Prism3D] -> Got: 'prism::" + got + "' expected: 'prism::vehicle_behind_rotation_camera_u'").c_str());

                    chase_camera = nullptr;
                }
            }

            if (chase_camera)
            {
                float engine_volume_distance = engine_volume;
                float turbo_volume_distance = turbo_volume;
                float exhaust_volume_distance = exhaust_volume;
                float effects_volume_distance = effects_volume;

                float distance_from_truck = chase_camera->distance_from_root;
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
                    adjust_volume(engine_volume_distance, 0.45);
                    adjust_volume(turbo_volume_distance, 0.45);
                    adjust_volume(exhaust_volume_distance, 0.45);
                    adjust_volume(effects_volume_distance, 0.45);
                }

            //    scs_log__(0, std::to_string(distance_from_truck).c_str());
            //    scs_log__(0, std::to_string(engine_volume_distance).c_str());

                fmod_manager_instance_->set_bus_volume("outside/exterior/truck_engine", engine_volume_distance);
                fmod_manager_instance_->set_bus_volume("outside/exterior/truck_turbo", turbo_volume_distance);
                fmod_manager_instance_->set_bus_volume("outside/exterior/truck_exhaust", exhaust_volume_distance);
                fmod_manager_instance_->set_bus_volume("outside/exterior/truck_effects", effects_volume_distance);
            }
        }
    }
}

bool engine_failed = false;
int engineRetryCount = 0;
void handle_engine(telemetry_data_t* telemetry_data, prism::game_ctrl_u* game_ctrl)
{
    fmod_manager_instance_->set_global_parameter("trans_rpm", telemetry_data->truck.engine_rpm);
    fmod_manager_instance_->set_global_parameter("park_brake", telemetry_data->truck.parking_brake);
    fmod_manager_instance_->set_event_parameter("engine/engine", "rpm", telemetry_data->truck.engine_rpm);
    fmod_manager_instance_->set_event_parameter("engine/exhaust", "rpm", telemetry_data->truck.engine_rpm);
    fmod_manager_instance_->set_event_parameter("engine/engine", "load", telemetry_data->truck.effective_throttle);
    fmod_manager_instance_->set_event_parameter("engine/engine", "engine_load", telemetry_data->truck.effective_throttle);
    fmod_manager_instance_->set_event_parameter("engine/exhaust", "load", telemetry_data->truck.effective_throttle);

    prism::game_actor_u* game_actor = game_ctrl ? game_ctrl->game_actor : nullptr;
    if (game_actor != nullptr)
    {
        //scs_log__(0, std::to_string(game_actor->wiper_position).c_str());

        fmod_manager_instance_->set_global_parameter("air_pressure", game_actor->air_pressure);

        if (game_actor->turbo_pressure >= 0 && game_actor->turbo_pressure <= 1)
        {
            fmod_manager_instance_->set_event_parameter("engine/turbo", "turbo", game_actor->turbo_pressure);
        }

        if (game_actor->engine_state != stored_engine_state)
        {
            if (!start_bad && game_actor->engine_state > 0 && stored_engine_state == 0)
            {
                // engine is starting/running
                fmod_manager_instance_->set_event_parameter("engine/engine", "engine_state", game_actor->engine_state);

                fmod_manager_instance_->set_event_parameter("engine/engine", "play", 1);
                fmod_manager_instance_->set_event_parameter("engine/exhaust", "play", 1);
                fmod_manager_instance_->set_event_parameter("engine/turbo", "play", 1);

                fmod_manager_instance_->set_event_state("engine/engine", true);
                fmod_manager_instance_->set_event_state("engine/exhaust", true);
                fmod_manager_instance_->set_event_state("engine/turbo", true);

                start_bad = false;
                engineRunning = true;
            }
            else if (game_actor->engine_state == 0 || game_actor->engine_state == 3) // engine is no longer running
            {
                fmod_manager_instance_->set_event_parameter("engine/engine", "play", 0);
                fmod_manager_instance_->set_event_parameter("engine/exhaust", "play", 0);
                fmod_manager_instance_->set_event_parameter("engine/turbo", "play", 0);

                start_bad = false;
                engineRunning = false;
            }
            stored_engine_state = game_actor->engine_state;
        }

        if (!start_bad && game_actor->engine_state == 2 && !engineRunning) {
            engine_failed = true;
            scs_log__(2, "[ts-fmod-plugin-v2] Failed to start engine! Forcing startup...");

            // engine is starting/running
            fmod_manager_instance_->set_event_parameter("engine/engine", "engine_state", game_actor->engine_state);

            fmod_manager_instance_->set_event_parameter("engine/engine", "play", 1);
            fmod_manager_instance_->set_event_parameter("engine/exhaust", "play", 1);
            fmod_manager_instance_->set_event_parameter("engine/turbo", "play", 1);

            fmod_manager_instance_->set_event_state("engine/engine", true);
            fmod_manager_instance_->set_event_state("engine/exhaust", true);
            fmod_manager_instance_->set_event_state("engine/turbo", true);

            start_bad = false;
            engineRunning = true;

            stored_engine_state = 2;
        }

        if (engine_failed && game_actor->engine_state == 2 && engineRunning)
        {
            scs_log__(0, "[ts-fmod-plugin-v2] Engine force start successfull!");
            engine_failed = false;
        }

        fmod_manager_instance_->set_event_parameter("engine/engine", "brake", should_engine_brake_sound_play(telemetry_data) ? game_actor->engine_brake_instant : 0.0f);
    }
}

bool isRetarderActive = false;
void handle_truck_effects(telemetry_data_t* telemetry_data)
{

  //  scs_log__(0, ("rpm: " + std::to_string(telemetry_data->truck.engine_rpm)).c_str());

    fmod_manager_instance_->set_global_parameter("retarder", telemetry_data->truck.retarder_level);

    if (telemetry_data->truck.retarder_level > 0 &&
        telemetry_data->truck.effective_throttle < 0.09f &&
        !isRetarderActive
    )
    {
        fmod_manager_instance_->set_event_state("effects/retarder", true, true);
        fmod_manager_instance_->set_event_state("retarder/retarder", true, true);
        isRetarderActive = true;

        scs_log__(0, "Retarder");
    }
    else if (telemetry_data->truck.retarder_level == 0 && isRetarderActive)
    {
        fmod_manager_instance_->set_event_state("effects/retarder", false);
        fmod_manager_instance_->set_event_state("retarder/retarder", false);
        isRetarderActive = false;

        scs_log__(0, "No Retarder");
    }

    if (engineRunning)
    {
        if (telemetry_data->truck.displayed_gear != lastGear)
        {
            fmod_manager_instance_->set_event_state("effects/air_gear", true);
        }
    }

    lastGear = telemetry_data->truck.displayed_gear;

    int reverse_enabled = s_reverse_enabled->current_value_int;
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

void handle_interior(telemetry_data_t* telemetry_data, prism::game_ctrl_u* game_ctrl, prism::game_sound_data_u* sound_data)
{
    prism::game_actor_u* game_actor = game_ctrl ? game_ctrl->game_actor : nullptr;

    if (game_actor)
    {
        // Handle basic interior buttons
        const auto hazard_warning = game_actor->hazards_button_instant;
        if (!common::cmpf(hazard_warning, hazard_warning_state))
        {
            fmod_manager_instance_->set_event_state("interior/stick_hazard_warning", true);
            hazard_warning_state = hazard_warning;
        }

        const auto light_horn = game_actor->light_horn_stick_instant;
        if (!common::cmpf(light_horn, light_horn_state))
        {
            fmod_manager_instance_->set_event_state("interior/stick_light_horn", true);
            light_horn_state = light_horn;
        }

        auto stick_lights = game_actor->light_modes_button_instant;
        if (stick_lights != light_stick_state)
        {
            fmod_manager_instance_->set_event_state("interior/stick_lights", true);
            light_stick_state = stick_lights;
        }

        const auto wipers_stick = game_actor->wiper_speed_instant;
        if (!common::cmpf(wipers_stick, wipers_stick_state))
        {
            fmod_manager_instance_->set_event_state("interior/stick_wipers", true);
            wipers_stick_state = wipers_stick;
        }

        // Handle Wipers
        if (game_actor->wiper_direction == 1)
        {
            if (!wipers_moving_up)
            {
              //  scs_log__(0, "wiper moving up");

                fmod_manager_instance_->set_event_state("interior/wipers_up", true, true);
                fmod_manager_instance_->set_event_state("interior/wipers_down", false);
            }

            wipers_moving_up = true;
            wipers_moving_down = false;
        }
        else if (game_actor->wiper_direction == -1)
        {
            if (!wipers_moving_down)
            {
              //  scs_log__(0, "wiper moving down");

                fmod_manager_instance_->set_event_state("interior/wipers_down", true, true);
                fmod_manager_instance_->set_event_state("interior/wipers_up", false);
            }

            wipers_moving_down = true;
            wipers_moving_up = false;
        }
        else
        {
            fmod_manager_instance_->set_event_state("interior/wipers_up", false);
            fmod_manager_instance_->set_event_state("interior/wipers_down", false);

            wipers_moving_up = false;
            wipers_moving_down = false;
        }

        // Handle window movement
        if (game_actor->left_window_state > 2 || game_actor->right_window_state > 2)  // if its moving and not just reached the end
        {
            is_window_moving = true;
            fmod_manager_instance_->set_global_parameter("window_stop", 0.f);
            fmod_manager_instance_->set_event_state("interior/window_move", true, true);

            if (game_actor->left_window_state > 2 && game_actor->right_window_state < 2)
            {
                fmod_manager_instance_->set_event_3d_posrot("interior/window_move", -1.f, 0, 0);
            }
            else if (game_actor->right_window_state > 2 && game_actor->left_window_state < 2)
            {
                fmod_manager_instance_->set_event_3d_posrot("interior/window_move", 1.f, 0, 0);
            }
            else
            {
                fmod_manager_instance_->set_event_3d_posrot("interior/window_move", 0, 0, 0);
            }
        }
        else if (is_window_moving)
        {
            is_window_moving = false;
            fmod_manager_instance_->set_global_parameter("window_stop", 1.f);
            fmod_manager_instance_->set_event_state("interior/window_move", false);
        }

        // Handle window buttons

        // right
        if (game_actor->right_window_button_pressed_instant == 1.f || game_actor->right_window_button_pressed_instant == 0.f)
        {
            if (!is_right_window_button_active) {
                fmod_manager_instance_->set_event_state("interior/window_click", true, false);
                fmod_manager_instance_->set_event_3d_posrot("interior/window_click", 1.f, 0, 0);
            }

            is_right_window_button_active = true;
        }
        else if (is_right_window_button_active)
        {
            is_right_window_button_active = false;
        }

        // left
        if (game_actor->left_window_button_pressed_instant == 1.f || game_actor->left_window_button_pressed_instant == 0.f)
        {
            if (!is_left_window_button_active) {
                fmod_manager_instance_->set_event_state("interior/window_click", true, false);
                fmod_manager_instance_->set_event_3d_posrot("interior/window_click", -1.f, 0, 0);
            }

            is_left_window_button_active = true;
        }
        else if (is_left_window_button_active)
        {
            is_left_window_button_active = false;
        }
    }

    if (sound_data)
    {

        const auto window_pos = sound_data->window_state;
        fmod_manager_instance_->set_global_parameter("wnd_left", window_pos.x);
        fmod_manager_instance_->set_global_parameter("wnd_right", window_pos.y);

        if (sound_data->is_cam_inside_interior)
        {
            auto window_volume = 0.5;

            float leftWindowBoost = ((window_pos.x / 2) / 2) / 2;
            float rightWindowBoost = ((window_pos.y / 2) / 2) / 2;

            float finalVolume = window_volume + (leftWindowBoost + rightWindowBoost);

            finalVolume += 0.05; // game_data.sii: // Interior sound attenuation value (the outside sounds will have 5% of original volume)

            fmod_manager_instance_->set_bus_volume("outside", finalVolume);
            fmod_manager_instance_->set_bus_volume("outside/exterior", finalVolume);
            fmod_manager_instance_->set_bus_volume("exterior", finalVolume);

            if (common::has_arg("tsfmod_deadening")) {
                fmod_manager_instance_->set_effect("engine/engine", true);
                fmod_manager_instance_->set_effect("engine/exhaust", true);
                fmod_manager_instance_->set_effect("engine/turbo", true);
                fmod_manager_instance_->set_effect("engine/start_bad", true);
                fmod_manager_instance_->set_effect("effects/gear_grind", true);
                fmod_manager_instance_->set_effect("effects/gear_wrong", true);
                fmod_manager_instance_->set_effect("effects/air_brake", true);
                fmod_manager_instance_->set_effect("effects/hook_attach", true);
                fmod_manager_instance_->set_effect("effects/hook_detach", true);
            }
        }
        else
        {
            fmod_manager_instance_->set_bus_volume("outside", 1);
            fmod_manager_instance_->set_bus_volume("outside/exterior", 1);
            fmod_manager_instance_->set_bus_volume("exterior", 1);


            fmod_manager_instance_->set_effect("engine/engine", false);
            fmod_manager_instance_->set_effect("engine/exhaust", false);
            fmod_manager_instance_->set_effect("engine/turbo", false);
            fmod_manager_instance_->set_effect("engine/start_bad", false);
            fmod_manager_instance_->set_effect("effects/gear_grind", false);
            fmod_manager_instance_->set_effect("effects/gear_wrong", false);
            fmod_manager_instance_->set_effect("effects/air_brake", false);
            fmod_manager_instance_->set_effect("effects/hook_attach", false);
            fmod_manager_instance_->set_effect("effects/hook_detach", false);
        }

        if (sound_data->is_cam_interior)
        {
            fmod_manager_instance_->set_bus_volume("cabin/interior", current_interior_volume);

            fmod_manager_instance_->set_global_parameter("cabin_type", 1);
            fmod_manager_instance_->set_global_parameter("cabin_rot", sound_data->camera_rotation_in_cabin);

        }
        else
        {
            fmod_manager_instance_->set_bus_volume("cabin/interior", 0.f);

            fmod_manager_instance_->set_global_parameter("cabin_type", 0.f);
            fmod_manager_instance_->set_global_parameter("cabin_rot", 0.f);
        }

        fmod_manager_instance_->set_global_parameter("cabin_out", 0.f);
        fmod_manager_instance_->set_global_parameter("surr_type", sound_data->echo);
        fmod_manager_instance_->set_global_parameter("daytime", sound_data->daytime_value);


        if (sound_data->playing_navi_sound != nullptr && last_played != sound_data->playing_navi_sound)
        {
            fmod_manager_instance_->set_event_state(sound_data->playing_navi_sound->event, true, true);
        }
        last_played = sound_data->playing_navi_sound;
    }



    int hardcore_simulation = g_hardcore_simulation->current_value_int;
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
        fmod_manager_instance_->set_event_state("interior/stick_retarder", true);
        prev_retarder_level = telemetry_data->truck.retarder_level;
    }

    if (telemetry_data->truck.light_high_beam != high_beams_enabled)
    {
        fmod_manager_instance_->set_event_state("interior/stick_high_beam", true);
        high_beams_enabled = telemetry_data->truck.light_high_beam;
    }

}

bool reeferRunning = false;
bool reeferStarted = false;
void handle_reefer(telemetry_data_t* telemetry_data)
{
    if (telemetry_data->trailer.connected)
    {
        if (!reeferRunning)
        {
            if (!reeferStarted)
            {
                fmod_manager_instance_->set_event_state("reefer_up", true);
                reeferStarted = true;
            }


            if (fmod_manager_instance_->is_event_playing("reefer_up") == 2)
            {
                fmod_manager_instance_->set_event_state("reefer_traffic", true);
                fmod_manager_instance_->set_global_parameter("is_reefer_on", 1);
                fmod_manager_instance_->set_global_parameter("is_owned_trailer", 1);

                reeferRunning = true;
            }
        }
    }
    else
    {
        if (reeferRunning)
        {
            fmod_manager_instance_->set_event_state("reefer_traffic", false);
            fmod_manager_instance_->set_event_state("reefer_down", true);
            reeferRunning = false;
            reeferStarted = false;
        }
    }
}

static prism::game_ctrl_u* game_ctrl{};
static prism::camera_manager_u* camera_manager{};
static prism::game_sound_data_u* sound_data{};
SCSAPI_VOID tick::telemetry_tick(const scs_event_t event, const void* const event_info, const scs_context_t context)
{
    telemetry_data_t* telemetry_data = (telemetry_data_t*)context;

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


    if (!game_ctrl)
    {
        bmem::setModule("current");

        uint64_t game_ctrl_ptr_instruction = bmem::patternScan("48 8B 0D ?? ?? ?? ?? 0F 57 C0 48 8B D0");
        uint64_t game_ctrl_ptr = bmem::relativeToAbsolute(game_ctrl_ptr_instruction, 3, 7);

        game_ctrl = *(prism::game_ctrl_u**)game_ctrl_ptr;
    }


    if (!camera_manager)
    {
        uint64_t camera_manager_ptr_instruction = bmem::patternScan("48 8B 05 ?? ?? ?? ?? 41 FF CE");
        uint64_t camera_manager_ptr = bmem::relativeToAbsolute(camera_manager_ptr_instruction, 3, 7);

        camera_manager = *(prism::camera_manager_u**)camera_manager_ptr;
    }


    if (!sound_data)
    {
        uint64_t sound_data_ptr_instruction = bmem::patternScan("48 8B 1D ?? ?? ?? ?? 48 8B F1 48 8B 83 ?? ?? ?? ?? 48 8D 8B");
        uint64_t sound_data_ptr = bmem::relativeToAbsolute(sound_data_ptr_instruction, 3, 7);

        sound_data = *(prism::game_sound_data_u**)sound_data_ptr;
    }


    handle_volume(telemetry_data, camera_manager);
    handle_engine(telemetry_data, game_ctrl);
    handle_truck_effects(telemetry_data);
   // handle_reefer(telemetry_data);
    handle_interior(telemetry_data, game_ctrl, sound_data);


   // fmod_manager_instance_->set_event_state("retarder/retarder", true, true);
    fmod_manager_instance_->update();
}