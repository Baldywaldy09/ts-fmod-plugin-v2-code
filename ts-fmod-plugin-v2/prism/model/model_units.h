#pragma once

#pragma pack(push, 1)

namespace prism
{
	class model_object_u : public unit_t // Size: 0x0640
	{
	public:
		vector_3 position; //0x0010 (0x0c)
		char pad_001C[20]; //0x001C (0x14)
		vector_3 position_2; //0x0030 (0x0c)
		char pad_003C[20]; //0x003C (0x14)
		void* needed_to_render; //0x0050 (0x08)
		char pad_0058[8]; //0x0058 (0x08)
		void* related_to_its_childeren; //0x0060 (0x08)
		char pad_0068[72]; //0x0068 (0x48)
		class array_dyn_t materials; //0x00B8 (0x20)
		void* model_desc_u; //0x00D8 (0x08)
		void* model_geom_u; //0x00E0 (0x08)

		virtual void one_Function0();
		virtual void one_Function1();
		virtual void one_renderunk();
		virtual void one_Function3();
		virtual void two_Function0();
		virtual void two_Function1();
		virtual void two_Function2();
		virtual void two_Function3(void* unk, void* unk2);
		virtual void three_Function0();
		virtual void three_Function1();
		virtual void three_Function2();
		virtual void three_Function3();
		virtual void four_Function0();
		virtual void four_Function1();
		virtual void four_Function2();
		virtual void four_Function3();
	};
}

#pragma pack(pop)