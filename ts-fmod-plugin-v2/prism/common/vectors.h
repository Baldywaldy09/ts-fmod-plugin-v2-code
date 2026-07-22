#pragma once

#pragma pack(push, 1)
namespace prism
{
    class vector_2
    {
    public:
		float x;
		float y;
    };

    class vector_3
    {
    public:
        float x;
        float y;
        float z;
    };

	class vector_4
	{
	public:
		float w;
		float x;
		float y;
		float z;
	};

	struct matrix4x4_t {
		float m1, m2, m3, m4;
		float m5, m6, m7, m8;
		float m9, m10, m11, m12;
		float m13, m14, m15, m16;
	};

	class placement_t // Size: 0x0020
	{
	public:
		vector_3 position; //0x0000 (0x0c)
		int16_t chunk_x; //0x000C (0x02)
		int16_t chunk_z; //0x000E (0x02)
		vector_4 rotation; //0x0010 (0x10)
	};
	static_assert(sizeof(placement_t) == 0x20);
};
#pragma pack(pop)