#include "fmod_manager.h"
#include "prism/cvar.h"
#include "nlohmann/json.hpp"
#include "global_variables.h"
#include <fmod/fmod_errors.h>
#include "common.h"
using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace global_variables::cvar;

fmod_manager::fmod_manager(const scs_log_t scs_log) : scs_log_(scs_log) {}

fmod_manager::~fmod_manager()
{
    system_->release();
    CoUninitialize();
}

bool fmod_manager::load_bank(const std::filesystem::path& plugin_files_dir, std::string bank_name)
{
    FMOD::Studio::Bank* bank;
    auto bank_file_path = plugin_files_dir;

    if (bank_name.find(".bank") == std::string::npos)
    {
        bank_file_path.append(bank_name).concat(".bank");
    }
    else
    {
        bank_file_path.append(bank_name);

        size_t pos = 0;
        pos = bank_name.find(".bank", pos);
        bank_name = bank_name.replace(pos, std::string(".bank").length(), "");
    }

    const auto res = system_->loadBankFile(bank_file_path.generic_u8string().c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    if (res != FMOD_OK)
    {
        std::stringstream ss;
        ss << "[ts-fmod-plugin-v2] Could not load the bank file '" << bank_name << ".bank' in 'vehicle_sounds.json' file, " <<
            FMOD_ErrorString(res);
        scs_log_(2, ss.str().c_str());
        return false;
    }
    selected_bank_names_.push_back(bank_name);
    fmod_bank_map_[bank_name] = bank;
    std::stringstream ss;
    ss << "[ts-fmod-plugin-v2] Using sound bank: '" << bank_name << "'";
    scs_log_(SCS_LOG_TYPE_message, ss.str().c_str());

    init_channels_for_bank(plugin_files_dir, bank_name);
}

bool fmod_manager::unload_bank(std::string bank_name)
{
    auto bank = get_bank(bank_name.c_str());
    if (bank == nullptr) { scs_log_(1, "1"); return false; }

    auto it = std::find(selected_bank_names_.begin(), selected_bank_names_.end(), bank_name);
    selected_bank_names_.erase(it);

    auto guids_file_path = fs::current_path().append("plugins/ts-fmod-plugin-v2");
    guids_file_path.append(bank_name).concat(".bank.guids");

    std::ifstream guids_file(guids_file_path);
    std::string s_guid, channel_path;
    if (!guids_file.is_open()) { scs_log_(1, "2"); return false; }

    while (guids_file >> s_guid >> channel_path)
    {
        if (channel_path.find("event:/") != std::string::npos)
        {
            const auto channel_name = channel_path.substr(7, channel_path.size() - 7);

            const auto res = fmod_events_global_map_.find(channel_name);

            if (res == fmod_events_global_map_.end()) fmod_events_map_.erase(channel_name);
        }
        else if (channel_path.find("bus:/") != std::string::npos)
        {
            const auto bus_name = channel_path.substr(5, channel_path.size() - 5);

            const auto res = fmod_busses_global_map_.find(bus_name);

            if (res == fmod_busses_global_map_.end()) fmod_busses_map_.erase(bus_name);
        }
    }

    guids_file.close();
    bank->unload();
    return true;
}

bool fmod_manager::load_truck_banks(const std::filesystem::path& plugin_files_dir, std::string truckToLoad, std::string truckToUnLoad)
{
    auto selected_bank_file_path = plugin_files_dir;
    selected_bank_file_path.append("vehicle_sounds.json");

    if (!exists(selected_bank_file_path))
    {
        scs_log_(2, "[ts-fmod-plugin-v2] Could not find the 'vehicle_sounds.json' file");
        return false;
    }

    std::ifstream selected_bank_file(selected_bank_file_path);
    if (!selected_bank_file.is_open())
    {
        scs_log_(2, "[ts-fmod-plugin-v2] Could not read the 'vehicle_sounds.json' file");
        return false;
    }

    json root;
    selected_bank_file >> root;

    for (json truck : root["trucks"])
    {
        if (truck.value("name", "") == truckToUnLoad.c_str())
        {
            for (std::string bank_name : truck["files"])
            {
                size_t pos = bank_name.find(".bank");
                if (pos != std::string::npos) {
                    bank_name.replace(pos, std::string::npos, "");
                }

                unload_bank(bank_name);
            }
        }
    }

    for (json truck : root["trucks"])
    {
        if (truck.value("name", "") == truckToLoad)
        {
            for (std::string bank_name : truck["files"])
            {
                load_bank(plugin_files_dir, bank_name);
            }
        }
    }
    selected_bank_file.close();
}

void fmod_manager::check_events()
{
    if (fmod_events_map_.find("engine/engine") == fmod_events_map_.end())
    {
        scs_log_(SCS_LOG_TYPE_warning,
            "[ts-fmod-plugin-v2] Did not find an 'event:/engine/engine' event. You will not have engine sounds.");
    }
    if (fmod_events_map_.find("engine/exhaust") == fmod_events_map_.end())
    {
        scs_log_(SCS_LOG_TYPE_warning,
            "[ts-fmod-plugin-v2] Did not find an 'event:/engine/exhaust' event. You will not have exhaust sounds.");
    }
    if (fmod_events_map_.find("engine/turbo") == fmod_events_map_.end())
    {
        scs_log_(SCS_LOG_TYPE_warning,
            "[ts-fmod-plugin-v2] Did not find an 'event:/engine/turbo' event. You will not have turbo sounds.");
    }

    //check navigation vocies was loaded
    {
        std::stringstream navi_event_error_ss;
        size_t navigation_event_error_count = 0;

        const size_t navigation_events_count = 38;
        const char* navigation_events[]{
            "and_then_exit_left",
            "and_then_exit_right",
            "and_then_go_straight",
            "and_then_keep_left",
            "and_then_keep_right",
            "and_then_turn_left",
            "and_then_turn_right",
            "compound_exit_left",
            "compound_exit_right",
            "compound_go_straight",
            "compound_keep_left",
            "compound_keep_right",
            "compound_turn_left",
            "compound_turn_right",
            "exit_left",
            "exit_now",
            "exit_right",
            "finish",
            "go_straight",
            "keep_left",
            "keep_right",
            "prepare_exit_left",
            "prepare_exit_right",
            "prepare_turn_left",
            "prepare_turn_right",
            "recomputing",
            "roundabout_1",
            "roundabout_2",
            "roundabout_3",
            "roundabout_4",
            "roundabout_5",
            "roundabout_6",
            "speed_signal",
            "speed_warning",
            "start",
            "turn_left",
            "turn_right",
            "u_turn"
        };

        for (auto& navigation_event : navigation_events)
        {
            if (fmod_events_map_.find(navigation_event) == fmod_events_map_.end())
            {
                navi_event_error_ss << navigation_event << ",";
                navigation_event_error_count++;
            }
        }
        auto log = navi_event_error_ss.str();

        if (navigation_event_error_count == navigation_events_count)
        {
            scs_log_(SCS_LOG_TYPE_warning,
                "[ts-fmod-plugin-v2] Did not find any navigation event. You will not have navigation voices.");
        }
        else if (log.length() > 1)
        {
            log = log.erase(log.length() - 1);
            std::string err = "[ts-fmod-plugin-v2] Did not find an navigation event. You will not have ( " + log +
                " ) voices.";
            scs_log_(SCS_LOG_TYPE_warning,
                err.c_str());
        }
    }
}

void fmod_manager::mute_game_audio()
{
    if (get_event("engine/engine"))
    {
        prism::cvar::set_value(s_truck_engine_mute, "1");
        prism::cvar::set_value(s_truck_exhaust_mute, "1");
        prism::cvar::set_value(s_truck_turbo_mute, "1");
    }
    else
    {
        prism::cvar::set_value(s_truck_engine_mute, "0");
        prism::cvar::set_value(s_truck_exhaust_mute, "0");
        prism::cvar::set_value(s_truck_turbo_mute, "0");
    }

    if (get_event("interior/blinker_off")) prism::cvar::set_value(s_interior_mute, "1");
    else prism::cvar::set_value(s_interior_mute, "0");

    if (get_event("music/main_menu")) prism::cvar::set_value(s_ui_music_mute, "1");
    else
    {
        if (telemetry_data.menuMusic) prism::cvar::set_value(s_ui_music_mute, "0");
    }

    if (get_event("finish")) prism::cvar::set_value(s_navigation_mute, "1");
    else 
    {
        if (telemetry_data.navigation) prism::cvar::set_value(s_navigation_mute, "0");
    }

    prism::cvar::store(false, true, false);
}

void fmod_manager::unmute_game_audio()
{
    if (get_event("engine/engine"))
    {
        prism::cvar::set_value(s_truck_engine_mute, "0");
        prism::cvar::set_value(s_truck_exhaust_mute, "0");
        prism::cvar::set_value(s_truck_turbo_mute, "0");
    }

    if (get_event("interior/blinker_off")) prism::cvar::set_value(s_interior_mute, "0");

    if (get_event("music/main_menu")) prism::cvar::set_value(s_ui_music_mute, "0");

    if (get_event("finish")) prism::cvar::set_value(s_navigation_mute, "0");

    prism::cvar::store(false, true, false);
}


bool fmod_manager::init(bool ETS2)
{
    const auto co_init_res = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    if (co_init_res != S_OK && co_init_res != S_FALSE)
    {
        scs_log_(2, "[ts-fmod-plugin-v2] CoInitializeEx Failed");
        return false;
    }
    const auto plugin_files_dir = fs::current_path().append("plugins/ts-fmod-plugin-v2");
    auto res = FMOD::Studio::System::create(&system_);
    if (res != FMOD_OK)
    {
        scs_log_(2,
                 (std::string("[ts-fmod-plugin-v2] Could not create FMOD system, ") + FMOD_ErrorString(res)).c_str());
        return false;
    }
    res = system_->initialize(64, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
    if (res != FMOD_OK)
    {
        scs_log_(2,
                 (std::string("[ts-fmod-plugin-v2] Could not initialize FMOD, ") + FMOD_ErrorString(res)).c_str());
        return false;
    }
    res = system_->getCoreSystem(&core_system_);
    if (res != FMOD_OK)
    {
        scs_log_(2,
                 (std::string("[ts-fmod-plugin-v2] Could not load FMOD core system, ") + FMOD_ErrorString(res)).c_str());
        return false;
    }
    int driver_count;
    core_system_->getNumDrivers(&driver_count);
    if (res != FMOD_OK)
    {
        scs_log_(2,
                 (std::string("[ts-fmod-plugin-v2] Could not get FMOD driver count, ") + FMOD_ErrorString(res)).c_str());
        return false;
    }

    for (int i = 0; i < driver_count; ++i)
    {
        char device_name[256];
        res = core_system_->getDriverInfo(i, device_name, 256, nullptr, nullptr, nullptr, nullptr);
        if (res != FMOD_OK)
        {
            scs_log_(2,
                     (std::string("[ts-fmod-plugin-v2] Could not get FMOD driver[") + std::to_string(i) + "] " +
                         FMOD_ErrorString(res)).c_str());
            if (i == 0) return false; // Only fail if not able to get the default device
        }
        scs_log_(SCS_LOG_TYPE_message,
                 (std::string("[ts-fmod-plugin-v2] Found output device[") + std::to_string(i) + "] " + device_name).
                 c_str());
    }
    scs_log_(SCS_LOG_TYPE_message, std::string("[ts-fmod-plugin-v2] Selecting default output device (0)").c_str());

    FMOD::Studio::Bank* bank;

    auto master_bank_path = plugin_files_dir;
    if (ETS2) master_bank_path.append("SCS-Files/ETS2/master.bank");
    else master_bank_path.append("SCS-Files/ATS/master.bank");
    res = system_->loadBankFile(master_bank_path.generic_u8string().c_str(),
                                FMOD_STUDIO_LOAD_BANK_NORMAL,
                                &bank);
    if (res != FMOD_OK)
    {
        std::stringstream ss;
        ss << "[ts-fmod-plugin-v2] Could not load the bank file 'master.bank', " <<
            FMOD_ErrorString(res);
        scs_log_(2, ss.str().c_str());

        std::stringstream sss;
        sss << "[ts-fmod-plugin-v2] File should be at: " << master_bank_path;

        scs_log_(2, sss.str().c_str());
        return false;
    }

    std::string backupPath;
    if (ETS2) backupPath = "SCS-Files\\ETS2\\BACKUP_system_warning";
    else backupPath = "SCS-Files\\ATS\\BACKUP_system_warning";
    if (!load_bank(plugin_files_dir, backupPath)) return false; // Load backup sounds
    if (!load_truck_banks(plugin_files_dir, "global", "")) return false; // Load "global" sounds

    return true;
}

bool fmod_manager::init_channels_for_bank(const std::filesystem::path& plugin_files_dir, std::string bank_name)
{
    std::stringstream ss;
    ss << "[ts-fmod-plugin-v2] Loading the events and busses for '" << bank_name << "'";
    scs_log_(SCS_LOG_TYPE_message, ss.str().c_str());

    auto guids_file_path = plugin_files_dir;
    guids_file_path.append(bank_name).concat(".bank.guids");

    if (!exists(guids_file_path))
    {
        scs_log_(2, "[ts-fmod-plugin-v2] Could not find the '*.bank.guids' file");
        return false;
    }

    std::ifstream guids_file(guids_file_path);
    std::string s_guid, channel_path;
    if (!guids_file.is_open())
    {
        scs_log_(2, "[ts-fmod-plugin-v2] Could not read the '*.bank.guids' file");
        return false;
    }

    while (guids_file >> s_guid >> channel_path)
    {
        if (channel_path.find("event:/") != std::string::npos)
        {
            const auto channel_name = channel_path.substr(7, channel_path.size() - 7);
            if (fmod_events_map_.count(channel_name))
            {
                std::stringstream ss;
                ss << "[ts-fmod-plugin-v2] The event '" << channel_name << "' is already loaded.";
                scs_log_(SCS_LOG_TYPE_warning, ss.str().c_str());
                continue;
            }

            auto event = fmod_event(system_, s_guid);
            if (event.create_event_instance() != FMOD_OK)
            {
                std::stringstream ss;
                ss << "[ts-fmod-plugin-v2] Could not load event '" << channel_name << "'";
                scs_log_(2, ss.str().c_str());
                continue;
            }
            std::stringstream ss;
            ss << "[ts-fmod-plugin-v2] Loading event '" << channel_name << "'";
            scs_log_(SCS_LOG_TYPE_message, ss.str().c_str());
            add_event(channel_name.c_str(), event);
        }
        else if (channel_path.find("bus:/") != std::string::npos)
        {
            const auto bus_name = channel_path.substr(5, channel_path.size() - 5);
            if (fmod_busses_map_.count(bus_name))
            {
                continue;
            }

            auto guid = common::get_guid(s_guid);
            FMOD::Studio::Bus* bus;
            system_->getBusByID(&guid, &bus);
            add_bus(bus_name.c_str(), bus);
        }
    }
    guids_file.close();
    return true;
}

void fmod_manager::set_paused(const bool state)
{
    pause_bus("outside/exterior/truck_engine", state);
    pause_bus("outside/exterior/truck_exhaust", state);
    pause_bus("outside/exterior/truck_turbo", state);
    pause_bus("cabin/interior", state);

    pause_bus("outside", state);
    pause_bus("outside/exterior", state);
    pause_bus("exterior", state); // backward compatibility for 1.37 sound mods
    pause_bus("game/navigation", state);

    set_event_state("music/main_menu", state); // play menu music sound

    if (state)
    {
        if (get_event("engine/engine"))
        {
            prism::cvar::set_value(s_truck_engine_mute, "0");
            prism::cvar::set_value(s_truck_exhaust_mute, "0");
            prism::cvar::set_value(s_truck_turbo_mute, "0");
        }

        if (get_event("interior/blinker_off")) prism::cvar::set_value(s_interior_mute, "0");

        if (get_event("finish")) prism::cvar::set_value(s_navigation_mute, "0");

        prism::cvar::store(false, true, false);
    }
    else
    {
        mute_game_audio();
    }

}

std::vector<std::thread> threads;
bool shouldStop = false;
void fmod_manager::_set_minimised() 
{
    float master = prism::cvar::get_value(s_master_volume);

    do
    {
        master -= 0.01;
        set_bus_volume("", master);
        std::this_thread::sleep_for(std::chrono::milliseconds(18));

    } while (master > 0 && !shouldStop);

    if (!shouldStop)
    { 
        set_bus_volume("", 0);
    }
}

bool muted = true;
void fmod_manager::set_minimised(bool state)
{
    if (state && !muted)
    { 
        shouldStop = false;
        muted = true;

        threads.emplace_back(&fmod_manager::_set_minimised, this);
    }
    else if (!state && muted)
    {
        shouldStop = true;

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        set_bus_volume("", prism::cvar::get_value(s_master_volume));

        muted = false;
    }
}

FMOD::Studio::Bus* fmod_manager::get_bus(const char* name)
{
    const auto res = fmod_busses_map_.find(name);
    if (res == fmod_busses_map_.end())
    {
        return nullptr;
    }
    return res->second;
}

FMOD::Studio::Bank* fmod_manager::get_bank(const char* name)
{
    const auto res = fmod_bank_map_.find(name);
    if (res == fmod_bank_map_.end())
    {
        return nullptr;
    }
    return res->second;
}

std::unique_ptr<fmod_event> fmod_manager::get_event(const char* name)
{
    const auto res = fmod_events_map_.find(name);
    if (res == fmod_events_map_.end())
    {
        return nullptr;
    }
    return std::make_unique<fmod_event>(res->second);
}

void fmod_manager::add_event(const char* name, fmod_event event, bool global)
{
    fmod_events_map_.insert(std::make_pair(name, event));

    if (global) fmod_events_global_map_.insert(std::make_pair(name, event));
}

void fmod_manager::add_bus(const char* name, FMOD::Studio::Bus* bus, bool global)
{
    fmod_busses_map_.insert(std::make_pair(name, bus));

    if (global) fmod_busses_global_map_.insert(std::make_pair(name, bus));
}

FMOD_RESULT fmod_manager::update() const
{
    return system_->update();
}

FMOD_RESULT fmod_manager::set_global_parameter(const char* param_name, const float value) const
{
    return system_->setParameterByName(param_name, value);
}

FMOD_RESULT fmod_manager::set_event_parameter(const char* event_name, const char* param_name, const float value)
{
    const auto event = get_event(event_name);
    if (event == nullptr) return FMOD_ERR_EVENT_NOTFOUND;

    return event->set_parameter_by_name(param_name, value);
}

FMOD_RESULT fmod_manager::set_event_state(const char* event_name, const bool state, const bool only_when_event_stopped)
{
    const auto event = get_event(event_name);
    if (event == nullptr) return FMOD_ERR_EVENT_NOTFOUND;

    FMOD_STUDIO_PLAYBACK_STATE playback_state;
    event->get_playback_state(&playback_state);
    if (playback_state != FMOD_STUDIO_PLAYBACK_STOPPED && only_when_event_stopped) return FMOD_ERR_EVENT_ALREADY_LOADED;

    return state ? event->start() : event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}


void fmod_manager::set_camera_posrot(FMOD_3D_ATTRIBUTES* listener_attributes, FMOD_VECTOR* atten) {

    system_->setListenerAttributes(0, listener_attributes, atten);
}

FMOD_RESULT fmod_manager::set_event_3d_posrot(const char* event_name, float pos_x, float pos_y, float pos_z, float pitch, float yaw, float roll) {
    const auto event = get_event(event_name);
    if (event == nullptr) return FMOD_ERR_EVENT_NOTFOUND;

    FMOD_3D_ATTRIBUTES attributes = {};
    attributes.position.x = pos_x;
    attributes.position.y = pos_y;
    attributes.position.z = pos_z;

    attributes.forward = { 0.0f, 0.0f, 1.0f };
    attributes.up = { 0.0f, 1.0f, 0.0f };


    return event->set_3d_attributes(attributes);
}


FMOD_RESULT fmod_manager::set_bus_volume(const char* bus_name, const float value)
{
    auto* const bus = get_bus(bus_name);
    if (bus == nullptr) return FMOD_ERR_EVENT_NOTFOUND;

    return bus->setVolume(value);
}

FMOD_RESULT fmod_manager::pause_bus(const char* bus_name, const bool state)
{
    auto* const bus = get_bus(bus_name);
    if (bus == nullptr) return FMOD_ERR_EVENT_NOTFOUND;

    return bus->setPaused(state);
}