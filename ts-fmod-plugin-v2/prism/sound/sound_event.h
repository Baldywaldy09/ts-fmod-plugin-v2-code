#pragma once

#pragma pack(push, 1)

#include "fmod.h"

namespace prism
{
    typedef struct sound_event_t
    {
        char pad_0008[40];                            // 0x0008 (0x28)
        uint32_t is_playing;                          // 0x0030 (0x04) 1 = play, 2 = ?, 3 = pause, 4 = stopped
        char pad_0034[4];                             // 0x0034 (0x04)
        string_dyn_t soundref_path;                   // 0x0038 (0x18)
        char pad_0050[8];                             // 0x0050 (0x08)
        string_dyn_t soundref_content;                // 0x0058 (0x18)
        float volume;                                 // 0x0070 (0x04)
        float pitch;                                  // 0x0074 (0x04)
        PRISMFMOD_3D_ATTRIBUTES* fmod_3d_attributes; // 0x0078 (0x08)
        fmod_bank_t* bank;                            // 0x0080 (0x08)
        uint64_t fmod_event_instance_id;              // 0x0088 (0x08) ?
        uint64_t fmod_parameter_id;                   // 0x0090 (0x08) ?
        fmod_parameter_t* parameter_1;                // 0x0098 (0x08) ?
        char pad_00A0[16];                            // 0x00A0 (0x10)
        fmod_parameter_t* parameter_2;                // 0x00B0 (0x08) ?

        virtual void func_0();
    };
}

#pragma pack(pop)