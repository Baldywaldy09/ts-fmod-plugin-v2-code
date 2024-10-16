// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include "global_variables.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <combaseapi.h>

#include <sstream>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <cassert>
#include <unordered_map>

#include "fmod/fmod_studio.hpp"
#include "fmod/fmod_errors.h"

#include "prism/string.h"

#include "common.h"
#include "memory.h"
#include "telemetry_data.h"
#include "memory_structure.h"
#include "core.h"
#include "hooks/sound.h"
#include "fmod_manager.h"

#include "scssdk/scssdk_telemetry.h"
#include "scssdk/eurotrucks2/scssdk_telemetry_eut2.h"

#endif //PCH_H
