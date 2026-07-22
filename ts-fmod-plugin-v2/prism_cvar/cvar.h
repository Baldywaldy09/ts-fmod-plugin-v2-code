// File: cvar.h
// Credit: Baldy09 | https://github.com/Baldywaldy09/

#pragma once

#include "../includes_windows.h"
#include <scssdk/scssdk.h>

namespace prism
{
	namespace cvar
	{
		class value // Size: 0x011C
		{
		public:
			char cvar_name[32];        //0x0008 (0x20)
			char default_value[100];   //0x0028 (0x64)
			char pad_008C[12];         //0x008C (0x0c)
			void* N000036D1;           //0x0098 (0x08)
			char pad_00A0[9];          //0x00A0 (0x09)
			bool N00003702;            //0x00A9 (0x01)
			char pad_00AA[7];          //0x00AA (0x07)
			char current_value[100];   //0x00B1 (0x64)
			char pad_0115[3];          //0x0115 (0x03)
			union {
				float current_value_float;
				int current_value_int;
				uint32_t current_value_raw;
			}; // 0x0118

			virtual void Function0();
			virtual char set_value(const char* new_value); // char __fastcall set_value(cvar_value this, const char* new_val)
			virtual void Function2();
			virtual void Function3();
			virtual void Function4();
			virtual void Function5();
			virtual void Function6();
			virtual void Function7();
		};

		int init(scs_log_t scs_log_);
		prism::cvar::value* get_pointer(const char* cvar_name);
	};
}