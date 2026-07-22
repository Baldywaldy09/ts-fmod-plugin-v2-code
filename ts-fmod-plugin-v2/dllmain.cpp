#define TSFMOD_API_VOID void __stdcall
#define TSFMOD_EXPORT extern "C" __declspec(dllexport)

#include "includes_windows.h"
#include "global_variables.h"
#include "telemetry_data.h"
#include "hooks_core.h"
#include <scssdk/scssdk_telemetry_channel.h>
#include "tick.h"
#include <scssdk/scssdk_telemetry.h>
#include "scssdk/eurotrucks2/scssdk_telemetry_eut2.h"
#include "sdk_stores.h"
#include "common.h"
#include "memory.h"
#include <windows.h>
#include <shellapi.h>

#include "prism_cvar/cvar.h"

#include "bmem.h"

namespace fs = std::filesystem;

using namespace global_variables::audio;
using namespace global_variables::cvar;
using namespace global_variables::truck;

scs_log_t scs_log;
fmod_manager* fmod_manager_instance;
hooks_core* g_hooks = nullptr;
telemetry_data_t telemetry_data;

scs_telemetry_register_for_channel_t register_for_channel = nullptr;
#define register_channel(name, index, type, trailer_or_truck, field) register_for_channel(name, index, SCS_VALUE_TYPE_##type, SCS_TELEMETRY_CHANNEL_FLAG_no_value, telemetry_store_##type, &telemetry_data.trailer_or_truck.field);

// ALL API FUNCTIONS MUST GO FROM A-Z TO KEEP RVA THE SAME

std::string currentTruckName = "NO_TRUCK";
TSFMOD_EXPORT TSFMOD_API_VOID a_getCurrentTruckName(char* buffer, size_t bufferSize)
{
    scs_log(0, "[ts-fmod-plugin-v2] API Function 'getCurrentTruckName' Called");
    strncpy_s(buffer, bufferSize, currentTruckName.c_str(), _TRUNCATE);
}

TSFMOD_EXPORT TSFMOD_API_VOID b_reloadVehicleSounds()
{
    scs_log(0, "[ts-fmod-plugin-v2] API Function 'reloadVehicleSounds' Called");


    scs_log(0, "[ts-fmod-plugin-v2] Reloading all global events ");
    fmod_manager_instance->load_truck_banks(fs::current_path().append("plugins/ts-fmod-plugin-v2"), "global", "global");

    fmod_manager_instance->check_events();
    fmod_manager_instance->mute_game_audio();

    // Force engine sound reload
    stored_engine_state = 0;



    scs_log(0, ("[ts-fmod-plugin-v2] Reloading all events for vehicle " + currentTruckName).c_str());
    fmod_manager_instance->load_truck_banks(fs::current_path().append("plugins/ts-fmod-plugin-v2"), currentTruckName, currentTruckName);

    fmod_manager_instance->check_events();
    fmod_manager_instance->mute_game_audio();

    // Force engine sound reload
    stored_engine_state = 0;


    scs_log(0, ("[ts-fmod-plugin-v2] Events and audio for vehicle: " + currentTruckName + " have been reloaded").c_str());
}

std::string lastTruckName = "NO_TRUCK";
SCSAPI_VOID handle_configuration(const scs_event_t event, const void* const event_info, const scs_context_t context)
{
    const struct scs_telemetry_configuration_t* const info = static_cast<const scs_telemetry_configuration_t*>(event_info);
    std::string id = info->id;

    if (id.compare(SCS_TELEMETRY_CONFIG_truck) == 0) {
        std::string truckBrand;
        std::string truckName;
        for (const scs_named_value_t* current = info->attributes; current->name; ++current) {
            const std::string name = current->name;
            if (name.compare(SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand) == 0) {
                truckBrand = static_cast<std::string>(current->value.value_string.value);
            }
            else if (name.compare(SCS_TELEMETRY_CONFIG_ATTRIBUTE_name) == 0) {
                truckName = static_cast<std::string>(current->value.value_string.value);
            }
        }

        if (truckName == "" && truckBrand == "")
        {
            currentTruckName = "NO_TRUCK";
            lastTruckName = "NO_TRUCK";
            return;
        }

        std::string truckFullName = truckBrand + " " + truckName;
        if (truckFullName == lastTruckName) return;

        scs_log(0, ("[ts-fmod-plugin-v2] Entered vehicle: " + truckFullName + " | Loading events").c_str());
        currentTruckName = truckFullName;

        fmod_manager_instance->load_truck_banks(fs::current_path().append("plugins/ts-fmod-plugin-v2"), truckFullName, lastTruckName);
        fmod_manager_instance->check_events();
        fmod_manager_instance->mute_game_audio();

        lastTruckName = truckFullName;
        scs_log(0, ("[ts-fmod-plugin-v2] Events and audio for vehicle: " + truckFullName + " have been loaded").c_str());
    }
}

SCSAPI_VOID telemetry_pause(const scs_event_t event, const void* const event_info, scs_context_t context)
{
    fmod_manager_instance->set_paused(event == SCS_TELEMETRY_EVENT_paused);
    paused = event == SCS_TELEMETRY_EVENT_paused;
}

// https://stackoverflow.com/questions/940707/how-do-i-programmatically-get-the-version-of-a-dll-or-exe-file
DWORD get_product_version()
{
    DWORD ver_handle = 0;
    UINT size = 0;
    LPBYTE lp_buffer = nullptr;
    char module_file_name[MAX_PATH];
    module_file_name[0] = '\0';
    GetModuleFileNameA(nullptr, module_file_name, sizeof(module_file_name));
    const DWORD ver_size = GetFileVersionInfoSizeA(module_file_name, &ver_handle);

    DWORD version = 0;

    if (ver_size != NULL)
    {
        const auto ver_data = new char[ver_size];

        if (GetFileVersionInfoA(module_file_name, ver_handle, ver_size, ver_data))
        {
            if (VerQueryValueA(ver_data, "\\", reinterpret_cast<void* *>(&lp_buffer), &size))
            {
                if (size)
                {
                    if (const auto ver_info = reinterpret_cast<VS_FIXEDFILEINFO*>(lp_buffer); ver_info->dwSignature == 0xfeef04bd)
                    {
                        version = (ver_info->dwFileVersionMS >> 0) & 0xffff;
                    }
                }
            }
        }
        delete[] ver_data;
    }
    return version;
}

void register_all_channels()
{
    // Truck Channels:
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_adblue, SCS_U32_NIL, float, truck, adblue)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_adblue_warning, SCS_U32_NIL, bool, truck, adblue_warning)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_battery_voltage, SCS_U32_NIL, float, truck, battery_voltage)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_battery_voltage_warning, SCS_U32_NIL, bool, truck, battery_voltage_warning)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_brake_air_pressure, SCS_U32_NIL, float, truck, brake_air_pressure)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_brake_air_pressure_emergency, SCS_U32_NIL, bool, truck, brake_air_pressure_emergency)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_brake_air_pressure_warning, SCS_U32_NIL, bool, truck, brake_air_pressure_warning)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_brake_temperature, SCS_U32_NIL, float, truck, brake_temperature)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_cabin_angular_acceleration, SCS_U32_NIL, fvector, truck, cabin_angular_acceleration)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_cabin_angular_velocity, SCS_U32_NIL, fvector, truck, cabin_angular_velocity)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_cabin_offset, SCS_U32_NIL, fplacement, truck, cabin_offset)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_cruise_control, SCS_U32_NIL, float, truck, cruise_control)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_dashboard_backlight, SCS_U32_NIL, float, truck, dashboard_backlight)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_differential_lock, SCS_U32_NIL, bool, truck, differential_lock)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_displayed_gear, SCS_U32_NIL, s32, truck, displayed_gear)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_brake, SCS_U32_NIL, float, truck, effective_brake)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_clutch, SCS_U32_NIL, float, truck, effective_clutch)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_steering, SCS_U32_NIL, float, truck, effective_steering)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_effective_throttle, SCS_U32_NIL, float, truck, effective_throttle)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_electric_enabled, SCS_U32_NIL, bool, truck, electric_enabled)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_enabled, SCS_U32_NIL, bool, truck, engine_enabled)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_gear, SCS_U32_NIL, s32, truck, engine_gear)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_engine_rpm, SCS_U32_NIL, float, truck, engine_rpm)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel, SCS_U32_NIL, float, truck, fuel)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel_average_consumption, SCS_U32_NIL, float, truck, fuel_average_consumption)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel_range, SCS_U32_NIL, float, truck, fuel_range)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel_warning, SCS_U32_NIL, bool, truck, fuel_warning)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_hazard_warning, SCS_U32_NIL, bool, truck, hazard_warning)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_head_offset, SCS_U32_NIL, fplacement, truck, head_offset)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_hshifter_selector, 0, bool, truck, hshifter_selector)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_hshifter_slot, SCS_U32_NIL, u32, truck, hshifter_slot)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_input_brake, SCS_U32_NIL, float, truck, input_brake)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_input_clutch, SCS_U32_NIL, float, truck, input_clutch)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_input_steering, SCS_U32_NIL, float, truck, input_steering)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_input_throttle, SCS_U32_NIL, float, truck, input_throttle)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_lblinker, SCS_U32_NIL, bool, truck, lblinker)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_lift_axle, SCS_U32_NIL, bool, truck, lift_axle)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_lift_axle_indicator, SCS_U32_NIL, bool, truck, lift_axle_indicator)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_aux_front, SCS_U32_NIL, u32, truck, light_aux_front)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_aux_roof, SCS_U32_NIL, u32, truck, light_aux_roof)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_beacon, SCS_U32_NIL, bool, truck, light_beacon)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_brake, SCS_U32_NIL, bool, truck, light_brake)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_high_beam, SCS_U32_NIL, bool, truck, light_high_beam)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_lblinker, SCS_U32_NIL, bool, truck, light_lblinker)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_low_beam, SCS_U32_NIL, bool, truck, light_low_beam)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_parking, SCS_U32_NIL, bool, truck, light_parking)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_rblinker, SCS_U32_NIL, bool, truck, light_rblinker)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_light_reverse, SCS_U32_NIL, bool, truck, light_reverse)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_local_angular_acceleration, SCS_U32_NIL, fvector, truck, local_angular_acceleration)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_local_angular_velocity, SCS_U32_NIL, fvector, truck, local_angular_velocity)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_local_linear_acceleration, SCS_U32_NIL, fvector, truck, local_linear_acceleration)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_local_linear_velocity, SCS_U32_NIL, fvector, truck, local_linear_velocity)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_motor_brake, SCS_U32_NIL, bool, truck, motor_brake)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_distance, SCS_U32_NIL, float, truck, navigation_distance)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_speed_limit, SCS_U32_NIL, float, truck, navigation_speed_limit)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_navigation_time, SCS_U32_NIL, float, truck, navigation_time)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_odometer, SCS_U32_NIL, float, truck, odometer)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_oil_pressure, SCS_U32_NIL, float, truck, oil_pressure)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_oil_pressure_warning, SCS_U32_NIL, bool, truck, oil_pressure_warning)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_oil_temperature, SCS_U32_NIL, float, truck, oil_temperature)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_parking_brake, SCS_U32_NIL, bool, truck, parking_brake)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_rblinker, SCS_U32_NIL, bool, truck, rblinker)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_retarder_level, SCS_U32_NIL, u32, truck, retarder_level)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_speed, SCS_U32_NIL, float, truck, speed)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_trailer_lift_axle, SCS_U32_NIL, bool, truck, trailer_lift_axle)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_trailer_lift_axle_indicator, SCS_U32_NIL, bool, truck, trailer_lift_axle_indicator)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_water_temperature, SCS_U32_NIL, float, truck, water_temperature)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_water_temperature_warning, SCS_U32_NIL, bool, truck, water_temperature_warning)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_cabin, SCS_U32_NIL, float, truck, wear_cabin)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_chassis, SCS_U32_NIL, float, truck, wear_chassis)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_engine, SCS_U32_NIL, float, truck, wear_engine)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_transmission, SCS_U32_NIL, float, truck, wear_transmission)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wear_wheels, SCS_U32_NIL, float, truck, wear_wheels)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_lift, 0, float, truck, wheel_lift)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_lift_offset, 0, float, truck, wheel_lift_offset)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_on_ground, 0, bool, truck, wheel_on_ground)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_rotation, 0, float, truck, wheel_rotation)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_steering, 0, float, truck, wheel_steering)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_substance, 0, u32, truck, wheel_substance)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_susp_deflection, 0, float, truck, wheel_susp_deflection)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wheel_velocity, 0, float, truck, wheel_velocity)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_wipers, SCS_U32_NIL, bool, truck, wipers)
    register_channel(SCS_TELEMETRY_TRUCK_CHANNEL_world_placement, SCS_U32_NIL, dplacement, truck, world_placement)

    // Trailer Channels:
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_cargo_damage, SCS_U32_NIL, float, trailer, cargo_damage)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_connected, SCS_U32_NIL, bool, trailer, connected)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_local_angular_acceleration, SCS_U32_NIL, fvector, trailer, local_angular_acceleration)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_local_angular_velocity, SCS_U32_NIL, fvector, trailer, local_angular_velocity)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_local_linear_acceleration, SCS_U32_NIL, fvector, trailer, local_linear_acceleration)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_local_linear_velocity, SCS_U32_NIL, fvector, trailer, local_linear_velocity)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wear_body, SCS_U32_NIL, float, trailer, wear_body)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wear_chassis, SCS_U32_NIL, float, trailer, wear_chassis)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wear_wheels, SCS_U32_NIL, float, trailer, wear_wheels)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift, 0, float, trailer, wheel_lift)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_lift_offset, 0, float, trailer, wheel_lift_offset)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_on_ground, 0, bool, trailer, wheel_on_ground)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_rotation, 0, float, trailer, wheel_rotation)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_steering, 0, float, trailer, wheel_steering)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_substance, 0, u32, trailer, wheel_substance)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_susp_deflection, 0, float, trailer, wheel_susp_deflection)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_wheel_velocity, 0, float, trailer, wheel_velocity)
    register_channel(SCS_TELEMETRY_TRAILER_CHANNEL_world_placement, SCS_U32_NIL, dplacement, trailer, world_placement)
}

#pragma comment( linker, "/export:scs_telemetry_init=scs_telemetry_init" )
SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t* const params)
{
    std::stringstream ss;
    const auto* const version_params = reinterpret_cast<const scs_telemetry_init_params_v101_t*>(params);
    scs_log = version_params->common.log;

    ss << "[ts-fmod-plugin-v2] TS-FMOD-Plugin V2 / Improved - Plugin Version: '" << common::plugin_version << "' ";
    scs_log(0, ss.str().c_str());
    scs_log(0, "[ts-fmod-plugin-v2] Plugin Loading...");

    // Check game version:
    const auto game_version = get_product_version();
    if (game_version != common::supported_game_version)
    {
        ss.str("");
        ss << "[ts-fmod-plugin-v2] Detected game version " << game_version
            << " while plugin is made for version " << common::plugin_version <<
            ". The plugin will not load to prevent crashes.";
        scs_log(2, ss.str().c_str());

        HWND hwnd = GetForegroundWindow();

        std::wstringstream message;
        message << L"Unsupported game version detected!\n\nPlugin is made for version: " << common::plugin_version
            << L"\nYou are trying to use it on version: 1." << game_version
            << L"\n\nWould you like to check for a avaliable update?";

        int result = MessageBoxW(NULL, message.str().c_str(), L"TS-FMOD-Plugin V2 / Improved", MB_YESNO | MB_ICONERROR | MB_TOPMOST);
        if (result == IDYES)
        {
            ShellExecute(0, 0, L"https://github.com/Baldywaldy09/ts-fmod-plugin-v2/releases/latest", 0, 0, SW_SHOW);
        }

        return SCS_RESULT_generic_error;
    }
    // End //

    // Force version label
    const char* versionTXT = "plugins\\ts-fmod-plugin-v2\\version.tsfv";
    if (!fs::exists(versionTXT))
    {
        scs_log(1, "[ts-fmod-plugin-v2] Didnt find version txt! Creating...");
        std::ofstream outfile(versionTXT);

        if (outfile.is_open())
        {
            outfile << common::tsfv_version;
            outfile.close();
        }
        else {
            scs_log(1, "[ts-fmod-plugin-v2] Unable to open file 'version.tsfv' for writing");
        }
    }
    else
    {
        std::ofstream outfile(versionTXT, std::ios::trunc); // std::ios::trunc clears the file content
        if (outfile.is_open())
        {
            outfile << common::tsfv_version;
            outfile.close();
        }
        else
        {
            scs_log(1, "[ts-fmod-plugin-v2] Unable to open file 'version.tsfv' for writing");
        }
    }

    // Find Memory + cvar pointers
    scs_log(0, "[ts-fmod-plugin-v2] Searching memory... If this is one of the last messages in the log after a crash, try disabling this plugin.");

    scs_result_t cvar_result = prism::cvar::init(scs_log);
    if (cvar_result != SCS_RESULT_ok) return cvar_result;

    s_master_volume         = prism::cvar::get_pointer("s_master_volume");
    s_truck_engine_volume   = prism::cvar::get_pointer("s_truck_engine_volume");
    s_truck_engine_mute     = prism::cvar::get_pointer("s_truck_engine_mute");
    s_truck_exhaust_volume  = prism::cvar::get_pointer("s_truck_exhaust_volume");
    s_truck_exhaust_mute    = prism::cvar::get_pointer("s_truck_exhaust_mute");
    s_truck_turbo_volume    = prism::cvar::get_pointer("s_truck_turbo_volume");
    s_truck_turbo_mute      = prism::cvar::get_pointer("s_truck_turbo_mute");
    s_truck_effects_volume  = prism::cvar::get_pointer("s_truck_effects_volume");
    s_interior_volume       = prism::cvar::get_pointer("s_interior_volume");
    s_interior_mute         = prism::cvar::get_pointer("s_interior_mute");
    s_ui_music_volume       = prism::cvar::get_pointer("s_ui_music_volume");
    s_ui_music_mute         = prism::cvar::get_pointer("s_ui_music_mute");
    s_navigation_volume     = prism::cvar::get_pointer("s_navigation_volume");
    s_navigation_mute       = prism::cvar::get_pointer("s_navigation_mute");
    g_voice_navigation      = prism::cvar::get_pointer("g_voice_navigation");
    g_voice_navigation_pack = prism::cvar::get_pointer("g_voice_navigation_pack");
    g_hardcore_simulation   = prism::cvar::get_pointer("g_hardcore_simulation");
    s_reverse_enabled       = prism::cvar::get_pointer("s_reverse_enabled");
    s_suspend_sound         = prism::cvar::get_pointer("s_suspend_sound");
    // End //


    // Start FMOD
    std::string fullGameName = version_params->common.game_name;
    bool ETS2 = fullGameName.find("Euro") != std::string::npos;

    fmod_manager_instance = new fmod_manager(scs_log);
    if (!fmod_manager_instance->init(ETS2))
    {
        scs_log(2, "[ts-fmod-plugin-v2] Could not init fmod");
        return SCS_RESULT_generic_error;
    }
    // End //


    // Create hooks
    g_hooks = new hooks_core(scs_log, fmod_manager_instance);
    if (!g_hooks->init())
    {
        scs_log(2, "[ts-fmod-plugin-v2] Could not init g_hooks");
        return SCS_RESULT_generic_error;
    }
    // End //

    // Register SCS Items:
    const auto events_registered =
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_paused, telemetry_pause, &telemetry_data) == SCS_RESULT_ok) &&
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_started, telemetry_pause, &telemetry_data) == SCS_RESULT_ok) &&
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, handle_configuration, &telemetry_data) == SCS_RESULT_ok) &&
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_start, tick::telemetry_tick, &telemetry_data) == SCS_RESULT_ok) &&
        (version_params->register_for_event(SCS_TELEMETRY_EVENT_frame_end, tick::telemetry_tick, &telemetry_data) == SCS_RESULT_ok);

    if (!events_registered)
    {
        scs_log(2, "[ts-fmod-plugin-v2] Unable to register event callbacks");
        return SCS_RESULT_generic_error;
    }

    register_for_channel = version_params->register_for_channel;
    register_all_channels();
    // End //

    // Start Tick:
    scs_log(0, "[ts-fmod-plugin-v2] Starting tick function... If this is one of the last messages in the log after a crash, try disabling this plugin.");
    tick::init_tick(scs_log, fmod_manager_instance);
    scs_log(0, "[ts-fmod-plugin-v2] Started tick function");


    // check if they have any navigation sound mod. If yes also enable it in game so it actually works :)
    if (fmod_manager_instance->get_event("finish"))
    {
        global_variables::audio::navigation = true;
        g_voice_navigation->set_value("1");

        g_voice_navigation_pack->set_value("english_doug_uk");
    //    prism::cvar::store(false, true, false);
    }
    if (fmod_manager_instance->get_event("music/main_menu")) menu_music = true;

    fmod_manager_instance->mute_game_audio();
    fmod_manager_instance->set_event_state("music/main_menu", true);

    if (common::has_arg("tsfmod_nodeadening"))
    {
        scs_log(0, "[ts-fmod-plugin-v2] Sound deadening disabled");
    }
    else
        scs_log(0, "[ts-fmod-plugin-v2] Sound deadening enabled");



    scs_log(0, "[ts-fmod-plugin-v2] Plugin loaded");

    return SCS_RESULT_ok;
}

#pragma comment( linker, "/export:scs_telemetry_shutdown=scs_telemetry_shutdown" )
SCSAPI_VOID scs_telemetry_shutdown(void)
{
    fmod_manager_instance->set_event_parameter("engine/engine", "play", 0.f);
    fmod_manager_instance->set_event_parameter("engine/exhaust", "play", 0.f);
    fmod_manager_instance->set_event_parameter("engine/turbo", "play", 0.f);

    fmod_manager_instance->set_event_state("engine/engine", false);
    fmod_manager_instance->set_event_state("engine/turbo", false);
    fmod_manager_instance->set_event_state("engine/exhaust", false);

    fmod_manager_instance->update();

    // Allow the game audio to take over:
     fmod_manager_instance->unmute_game_audio(); // tmp make crash

     delete fmod_manager_instance;
     fmod_manager_instance = nullptr;

    //g_hooks->uninitialize();
    delete g_hooks;

    scs_log(0, "[ts-fmod-plugin-v2] Plugin Unloaded | You will not have custom sounds");
}