#pragma once
#include "../token/token.h"

#pragma pack(push, 1)

namespace prism
{
	class fmod_parameter_t // Size: 0x001C
	{
	public:
		fmod_parameter_t* parameter; //0x0000 (0x08)
		fmod_parameter_t* parameter_2; //0x0008 (0x08)
		prism_token param_name; //0x0010 (0x08)
		float value; //0x0018 (0x04)

		std::string param_name_string() { return prism_token_manager::token_to_string(param_name); } // Translate the param name into a normal string
	};
	static_assert(sizeof(fmod_parameter_t) == 0x1C);
}

#pragma pack(pop)