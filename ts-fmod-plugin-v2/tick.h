#pragma once

#include "hooks_core.h"
#include <scssdk/scssdk_telemetry_event.h>

namespace tick {
	void init_tick(scs_log_t scs_log, fmod_manager* fmod_manager_instance);
	SCSAPI_VOID telemetry_tick(const scs_event_t event, const void* const event_info, const scs_context_t context);
}; 