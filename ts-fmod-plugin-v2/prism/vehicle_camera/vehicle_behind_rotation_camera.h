#pragma once

#pragma pack(push, 1)

namespace prism
{
	class vehicle_behind_rotation_camera : public unit_t // Size: 0x0970
	{
	public:
		float yaw; //0x0010 (0x04)
		float pitch; //0x0014 (0x04)
		float roll; //0x0018 (0x04)
		float mouse_sensitivity; //0x001C (0x04)
		float camera_fov; //0x0020 (0x04)
		char pad_0024[4]; //0x0024 (0x04)
		float render_distance; //0x0028 (0x04)
		char pad_002C[12]; //0x002C (0x0c)
		float streach_x; //0x0038 (0x04)
		float streach_y; //0x003C (0x04)
		char pad_0040[172]; //0x0040 (0xac)
		float shake_anim_step; //0x00EC (0x04)
		float shake_anim_scale_min; //0x00F0 (0x04)
		float shake_anim_scale_max; //0x00F4 (0x04)
		class array_dyn_t shake_anim; //0x00F8 (0x20)
		char pad_0118[32]; //0x0118 (0x20)
		float hand_shake_speed; //0x0138 (0x04)
		char pad_013C[572]; //0x013C (0x23c)
		float distance_from_root_instant; //0x0378 (0x04)
		float distance_from_root; //0x037C (0x04)
		char pad_0380[1520]; //0x0380 (0x5f0)
	};
	static_assert(sizeof(vehicle_behind_rotation_camera) == 0x970);
}

#pragma pack(pop)