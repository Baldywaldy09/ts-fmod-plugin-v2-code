#pragma once

#include "hooks_core.h"
#include "memory_structure.h"
#include <scssdk/scssdk_telemetry_event.h>

namespace tick {
	void init_tick(scs_log_t scs_log, fmod_manager* fmod_manager_instance, uint64_t game_base_, uint64_t base_ctrl_ptr_, uint64_t unk_interior_ptr_, uint32_t game_actor_offset_, uint64_t core_camera_ptr_);
	SCSAPI_VOID telemetry_tick(const scs_event_t event, const void* const event_info, const scs_context_t context);
}; 