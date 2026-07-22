#pragma once

#include "../unit.h"

#pragma pack(push, 1)

namespace prism
{
	class core_camera_u : public unit_t // Size: 0x00A0
	{
	public:
		char pad_0010[12]; //0x0010 (0x0c)
		float mouse_sensitivity; //0x001C (0x04)
		float fov; //0x0020 (0x04)
		float near_plane; //0x0024 (0x04)
		float far_plane; //0x0028 (0x04)
		char pad_002C[12]; //0x002C (0x0c)
		float h_fov; //0x0038 (0x04)
		float v_fov; //0x003C (0x04)
		class placement_t placement; //0x0040 (0x20)
		matrix4x4_t projection_matrix; //0x0060 (0x40)
	};
	static_assert(sizeof(core_camera_u) == 0xA0);

	class vehicle_behind_rotation_camera_u : public core_camera_u // Size: 0x0918
	{
	public:
		char pad_00A0[1080]; //0x00A0 (0x438)
		float distance_from_root_instant; //0x04D8 (0x04)
		float distance_from_root; //0x04DC (0x04)
		char pad_04E0[1080]; //0x04E0 (0x438)
	};
	static_assert(sizeof(vehicle_behind_rotation_camera_u) == 0x918);
}

#pragma pack(pop)