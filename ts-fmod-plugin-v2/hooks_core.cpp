#include "hooks_core.h"

#include <MinHook.h>
#include <string>

#include "hooks/sound.h"
#include "fmod_manager.h"

hooks_core* hooks_core::g_hooks = nullptr;

hooks_core::hooks_core(const scs_log_t scs_log, fmod_manager* fmod_manager_instance) : scs_log_(scs_log), fmod_manager_instance_(fmod_manager_instance)
{
    g_hooks = this;
}

hooks_core::~hooks_core()
{
    uninitialize();
}

void hooks_core::tick()
{
}

bool hooks_core::init()
{
    if (MH_Initialize() != MH_OK)
    {
        return false;
    }

    const auto sound_hook = new hooks::sound();

    if (!sound_hook->install())
    {
        return false;
    }
    installed_hooks_.emplace_back(sound_hook);

    return true;
}

void hooks_core::uninitialize() const
{
    for (const auto& installed_hook : installed_hooks_)
    {
        installed_hook->uninstall();
        delete installed_hook;
    }
    MH_Uninitialize();
}

void hooks_core::log(const scs_log_type_t log_type, const scs_string_t message) const
{
    scs_log_(log_type, message);
}

fmod_manager* hooks_core::get_fmod_manager() const
{
    return fmod_manager_instance_;
}