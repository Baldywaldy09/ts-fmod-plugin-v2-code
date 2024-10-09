#pragma once

#include "fmod_event.h"
#include "telemetry_data.h"

class fmod_manager
{
    telemetry_data_t telemetry_data;

    FMOD::System* core_system_ = nullptr;
    FMOD::Studio::System* system_ = nullptr;
    std::vector<std::string> selected_bank_names_;

    scs_log_t scs_log_;
    scs_string_t convertToScsString(const std::string& str) { return str.c_str(); }

    std::unordered_map<std::string, fmod_event> fmod_events_map_;
    std::unordered_map<std::string, fmod_event> fmod_events_global_map_;
    std::unordered_map<std::string, FMOD::Studio::Bus*> fmod_busses_map_;
    std::unordered_map<std::string, FMOD::Studio::Bus*> fmod_busses_global_map_;
    std::map<std::string, FMOD::Studio::Bank*> fmod_bank_map_;

    void _set_minimised();

    bool init_channels_for_bank(const std::filesystem::path& plugin_files_dir, std::string bank_name);
public:
    explicit fmod_manager(scs_log_t scs_log);
    ~fmod_manager();

    bool init(bool ETS2);

    void check_events();
    void mute_game_audio();
    void unmute_game_audio();
    bool load_bank(const std::filesystem::path& plugin_files_dir, std::string bank_name);
    bool load_truck_banks(const std::filesystem::path& plugin_files_dir, std::string truckToLoad, std::string truckToUnLoad);

    void add_event(const char* name, fmod_event event, bool global = false);
    void add_bus(const char* name, FMOD::Studio::Bus* bus, bool global = false);

    bool unload_bank(std::string bank_name);

    void set_camera_posrot(FMOD_3D_ATTRIBUTES* listener_attributes, FMOD_VECTOR* atten);

    FMOD::Studio::Bus* get_bus(const char* name);
    FMOD::Studio::Bank* get_bank(const char* name);
    std::unique_ptr<fmod_event> get_event(const char* name);

    void set_paused(bool state);
    void set_minimised(bool state);
    FMOD_RESULT update() const;
    FMOD_RESULT set_global_parameter(const char* param_name, float value) const;
    FMOD_RESULT set_event_parameter(const char* event_name, const char* param_name, float value);
    FMOD_RESULT set_event_state(const char* event_name, bool state, bool only_when_event_stopped = false);
    FMOD_RESULT set_event_3d_posrot(const char* event_name, float pos_x, float pos_y, float pos_z, float pitch, float yaw, float roll);
    FMOD_RESULT set_bus_volume(const char* bus_name, float value);
    FMOD_RESULT pause_bus(const char* bus_name, bool state);
};
