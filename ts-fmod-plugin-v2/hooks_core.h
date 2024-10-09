#pragma once

#include "fmod_manager.h"

namespace hooks
{
    class hook;
}

class hooks_core
{
    scs_log_t scs_log_;
    fmod_manager* fmod_manager_instance_;
    std::vector<hooks::hook*> installed_hooks_;

public:
    static hooks_core* g_hooks;

    hooks_core(const scs_log_t scs_log, fmod_manager* fmod_manager_instance);
    ~hooks_core();
    bool init();
    void tick();
    void uninitialize() const;

    void log(scs_log_type_t log_type, scs_string_t message) const;
    fmod_manager* get_fmod_manager() const; // Changed function name to avoid conflict
};
