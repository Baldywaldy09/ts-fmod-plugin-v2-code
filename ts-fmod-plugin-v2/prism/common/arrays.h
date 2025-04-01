#pragma once

#pragma pack(push, 1)
namespace prism
{
	class array_dyn_t // Size: 0x0020
	{
		char pad_0000[8]; //0x0000 (0x08)
	public:
		void* ptr_to_array; //0x0008 (0x08)
		uint64_t array_size; //0x0010 (0x08)
		uint64_t array_capacity; //0x0018 (0x08)
	};
	static_assert(sizeof(array_dyn_t) == 0x20);
};
#pragma pack(pop)