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
		token_t param_name; //0x0010 (0x08)
		float value; //0x0018 (0x04)

		std::string param_name_string() { return prism_token_manager::token_to_string(param_name); } // Translate the param name into a normal string
	};
	static_assert(sizeof(fmod_parameter_t) == 0x1C);
	
	typedef struct FMOD_VECTOR
	{
		float x;
		float y;
		float z;
	} FMOD_VECTOR;

	typedef struct PRISMFMOD_3D_ATTRIBUTES
    {
        FMOD_VECTOR position; //0x0000 (0x0c)
        char pad_000C[4]; //0x000C (0x04)
        FMOD_VECTOR velocity; //0x0010 (0x0c)
        FMOD_VECTOR forward; //0x001C (0x0c)
        FMOD_VECTOR up; //0x0028 (0x0c)
    };
	
	typedef struct fmod_bank_t
    {
        char pad_0000[16];      // 0x0000 (0x10)
        string_dyn_t bank_path; // 0x0010 (0x18)

        virtual void func_0();
    };
}

#pragma pack(pop)