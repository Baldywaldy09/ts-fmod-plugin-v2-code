#pragma once
#include <cstdint>
#include <cstring>
#include <string>



#include "common/unit.h"
#include "common/arrays.h"
#include "common/strings.h"
#include "common/pointers.h"
#include "common/vectors.h"

#include "token/token.h"

#include "ui/ui.h"

#include "pointer_base/pointer_base.h"

#include "actors/game_sound_data.h"
#include "actors/game_ctrl.h"
#include "actors/game_actor.h"

#include "sound/fmod.h"
#include "sound/navigation.h"



namespace prism
{
	class initialization
	{
	private:
		static void* scs_log;

		static void found_addr(const char* name, uint64_t address, bool function);
		static void missing_addr(const char* name, bool function);

		template <typename ClassType>
		static bool load_class(const char* name, const char* pattern, int offset, ClassType& class_type);
		
		template <typename FuncType>
		static bool load_function(const char* name, const char* pattern, int offset, FuncType& function);

	public:
		enum error_level_enum
		{
			ignore,		// Missing functions will not show a warning in console and will log as "[plugin_name][prism] Found function '{func}' at: '0x0000000000000'" // always returns true 
			warning,	// Missing functions will show a warning like: "[plugin_name][prism] Could not find function '{func}'!" // always returns true 
			error,		// Missing functions will show a error like: "[plugin_name][prism] Could not find function '{func}'!" // always returns true 
			hault		// Missing functions will show a error like: "[plugin_name][prism] Could not find function '{func}'!" // returns false if failed 
		};

		enum log_destination_enum
		{
			prism_console,		// All messages will be logged into the in-game console
			windows_console,	// All messages will be logged with std::cout // does NOT make the window for you
			local_file			// All messages get logged into '/plugins/{plugin_name}_baldy_prism.log'
		};

		static bool init_functions(error_level_enum error_level, log_destination_enum log_destination, bool log_info, bool error_in_console, std::string plugin_name);
	};
};