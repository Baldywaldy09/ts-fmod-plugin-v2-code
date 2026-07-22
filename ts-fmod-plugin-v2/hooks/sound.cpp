
#include "sound.h"
#include "MinHook.h"
#include "../hooks_core.h"
#include "../prism/sound/sound_event.h"
#include <scssdk/scssdk.h>
#include "../fmod_manager.h"
#include "../bmem.h"

using namespace std;

static std::vector<std::thread> threads;

uintptr_t sound_event_update_address;
typedef bool(__fastcall* prism_sound_event_update_t) (
    const prism::sound_event_t* sound_event,
    bool stop,
    int64_t a3
);
prism_sound_event_update_t original_sound_event_update;

void scs_log(scs_log_type_t log_type, scs_string_t message) { hooks_core::g_hooks->log(log_type, message); }
namespace hooks
{
    string eventsToMute = "effects/reverse |";
    string customEvents = "engine/start_bad | interior/noise | effects/gear_grind | effects/gear_wrong | effects/air_brake | effects/hook_attach | effects/hook_detach | interior/system_warning1 | interior/system_warning2 | interior/system_warning3 |"; // sounds that the plugin needs to play from here:
    string whenStopped = "interior/noise |"; // sounds that should play only when last one has stopped playing

    void breakThreads()
    {
        scs_log(0, "breaking threads");

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    void playHorn(prism::sound_event_t* sound_event)
    {
        scs_log(0, "created");

        while (true)
        {
            hooks_core::g_hooks->get_fmod_manager()->set_event_state("horn/horn_01", true);

            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }

        breakThreads();
    }

    void startHornLoop(prism::sound_event_t* sound_event)
    {
        scs_log(0, "creating thread");
        threads.emplace_back([&]() { playHorn(sound_event); });
    }

    bool detoured_sound_event_update(prism::sound_event_t* sound_event, const bool stop, const int64_t a3)
    {
        string event;
        string soundRef = sound_event->soundref_content.string;

        if (soundRef.find('#') != string::npos) { event = soundRef.substr(soundRef.find('#')); event = event.erase(0, 1); }

        if (hooks_core::g_hooks->get_fmod_manager() == nullptr) {
            scs_log(2, "[ts-fmod-plugin-v2] `hooks_core::g_hooks->get_fmod_manager()` doesnt exist! Expect random behaviour");
            return original_sound_event_update(sound_event, stop, a3);
        }

        if (event.find("system_warning1") != string::npos) event = "interior/system_warning1";
        else if (event.find("system_warning2") != string::npos) event = "interior/system_warning2";
        else if (event.find("system_warning3") != string::npos) event = "interior/system_warning3";

        if (event.find("effects") != string::npos && (customEvents.find(event + " |") != string::npos || eventsToMute.find(event + " |") != string::npos)) // if its a truck effects sound and its a supported one
        {
            if (hooks_core::g_hooks->get_fmod_manager()->get_event(event.c_str())) // if the user has the sound
            {
                stringstream ss;
                ss << "Event: '" << event << "' was found. Muting...";
               // scs_log(0, ss.str().c_str());

                sound_event->volume = 0;
            }
        }

        if (event.find("horn/horn_") != string::npos)
        {
          //  startHornLoop(sound_event);

         //   sound_event->volume = 0;
        }

        if (customEvents.find(event + " |") != string::npos)
        {
            bool condition = (whenStopped.find(event + " |") != string::npos);
            hooks_core::g_hooks->get_fmod_manager()->set_event_state(event.c_str(), true, condition);

            // Needed to stop other engine sounds playing
            if (event == "engine/start_bad") start_bad = true;
        }

        return original_sound_event_update(sound_event, stop, a3);
    }

    bool sound::install()
    {
        sound_event_update_address = bmem::patternScan("4C 8B DC 49 89 6B 20 57 48");
        if (sound_event_update_address == NULL)
        {
            scs_log(2, "[ts-fmod-plugin-v2][sound::install] Could not find address for 'sound_event_update'");
            return false;
        }

        if (MH_CreateHook((LPVOID)sound_event_update_address, (LPVOID)detoured_sound_event_update, (LPVOID*)&original_sound_event_update) != MH_OK)
        {
            scs_log(2, "[ts-fmod-plugin-v2][sound::install] Could not create 'sound_event_update' hook");
            return false;
        }

        if (MH_EnableHook((LPVOID)sound_event_update_address) != MH_OK)
        {
            scs_log(2, "[ts-fmod-plugin-v2][sound::install] Could not enable 'sound_event_update' hook");
            return false;
        }
        installed_ = true;
        return true;
    }

    void sound::uninstall()
    {
        if (!installed_) return;
        if (MH_DisableHook((LPVOID)sound_event_update_address) != MH_OK)
        {
            scs_log(2, "[ts-fmod-plugin-v2][sound::uninstall] Could not disable 'sound_event_update' hook");
            return;
        }

        if (MH_RemoveHook((LPVOID)sound_event_update_address) != MH_OK)
        {
            scs_log(2, "[ts-fmod-plugin-v2][sound::uninstall] Could not remove 'sound_event_update' hook");
            return;
        }
        installed_ = false;

        // breakThreads();
    }
}