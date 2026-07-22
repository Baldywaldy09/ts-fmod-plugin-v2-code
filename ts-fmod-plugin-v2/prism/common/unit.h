#pragma once

#include "strings.h"

#pragma pack(push, 1)
namespace prism
{
    class descriptor
    {
    public:
        prism::string* string;
    };


	class unit_t // Size: 0x0010
	{
		uint32_t N000001B5; //0x0008 (0x04)
		uint32_t N000001CF; //0x000C (0x04)
	public:
		virtual void destructor();
		virtual void destroy();
		virtual void clone();
		virtual void Function3();
		virtual void Function4();
		virtual descriptor* get_unit_descriptor();

	};
	static_assert(sizeof(unit_t) == 0x10);

};
#pragma pack(pop)