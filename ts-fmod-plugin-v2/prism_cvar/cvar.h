// File: cvar.h
// Credit: Baldy09 | https://github.com/Baldywaldy09/

#pragma once

#include "../includes_windows.h"
#include <scssdk/scssdk.h>

namespace prism
{
	namespace cvar
	{
		using cvar_input_value = std::variant<float, int, const char*>; // cvar values are always a float, int or a string
		typedef __int64 pointer;

		// Functions:
		int init(scs_log_t scs_log_);
		prism::cvar::pointer get_pointer(const char* cvar_name);
		float get_value(prism::cvar::pointer cvar_pointer);
		__int64 set_value(prism::cvar::pointer cvar_pointer, cvar_input_value new_value);
		prism::cvar::pointer register_name(const char* cvar_name);
		__int64 unregister_name(prism::cvar::pointer cvar_pointer);
		void store(bool main_config, bool config_profile_local, bool config_profile);
	};
}