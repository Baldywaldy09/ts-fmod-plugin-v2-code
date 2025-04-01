#pragma once

#pragma pack(push, 1)

namespace prism
{
	class navigation_sound_event // Size: 0x0028
	{
		char pad_0004[8]; //0x0004 (0x04)
	public:
		const char* event; //0x0008 (0x08)
		char pad_0014[16]; //0x0014 (0x0c)
		void* sound_instance; //0x0020 (0x08)
	};
	static_assert(sizeof(navigation_sound_event) == 0x28);
}

#pragma pack(pop)