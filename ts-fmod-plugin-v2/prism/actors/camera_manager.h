#pragma once

#pragma pack(push, 1)

namespace prism
{
	class camera_manager_u : public unit_t // Size: 0x0140
	{
	public:
		int32_t current_camera;            //0x0010 (0x04)
		char pad_0014[28];                 //0x0014 (0x1c)
		array_dyn_t<> vehicle_cameras;     //0x0030 (0x28)
		char pad_0050[40];                 //0x0058 (0x28)
		class core_camera_u* core_camera;  //0x0080 (0x08)
		char pad_0080[184];                //0x0088 (0xb8)
	};
	static_assert(sizeof(camera_manager_u) == 0x140);
}

#pragma pack(pop)