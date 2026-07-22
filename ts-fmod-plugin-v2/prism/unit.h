#pragma once

#include "common/strings.h"

namespace prism {
	class unit_descriptor_t // Size: 0x0050
	{
	public:
		prism::string* class_name;       //0x0000 (0x08)
		void* constructor;               //0x0008 (0x08)
		void* destructor;                //0x0010 (0x08)
		unit_descriptor_t* parent_class; //0x0018 (0x08)
		char pad_0020[16];               //0x0020 (0x10)
		uint64_t N00003430;              //0x0030 (0x08)
		uint64_t N00003632;              //0x0038 (0x08)
		char pad_0040[8];                //0x0040 (0x08)
		uint64_t unk_token;              //0x0048 (0x08)
	};
	static_assert(sizeof(unit_descriptor_t) == 0x50);

	class unit_t // Size: 0x0010
	{
	public:
		uint32_t unit_unk1; //0x0008 (0x04)
		uint32_t unit_unk2; //0x000C (0x04)

		virtual void destructor();
		virtual void destroy();
		virtual void clone();
		virtual void Function3();
		virtual void Function4();
		virtual unit_descriptor_t* get_unit_descriptor();
		virtual void Function6();
		virtual void set_attribute();
		virtual void pre_save();
		virtual void post_save();
		virtual void pre_load();
		virtual void post_load();
	};
	static_assert(sizeof(unit_t) == 0x10);
};
