#pragma once

#include "scssdk/scssdk_value.h"

struct truck_data_t {
    float adblue;
    float adblue_average_consumption;
    bool adblue_warning;
    float battery_voltage;
    bool battery_voltage_warning;
    float brake_air_pressure;
    bool brake_air_pressure_emergency;
    bool brake_air_pressure_warning;
    float brake_temperature;
    scs_value_fvector_t cabin_angular_acceleration;
    scs_value_fvector_t cabin_angular_velocity;
    scs_value_fplacement_t cabin_offset;
    float cruise_control;
    float dashboard_backlight;
    bool differential_lock;
    int32_t displayed_gear;
    float effective_brake;
    float effective_clutch;
    float effective_steering;
    float effective_throttle;
    bool electric_enabled;
    bool engine_enabled;
    int32_t engine_gear;
    float engine_rpm;
    float fuel;
    float fuel_average_consumption;
    float fuel_range;
    bool fuel_warning;
    bool hazard_warning;
    scs_value_fplacement_t head_offset;
    bool hshifter_selector;
    uint32_t hshifter_slot;
    float input_brake;
    float input_clutch;
    float input_steering;
    float input_throttle;
    bool lblinker;
    bool lift_axle;
    bool lift_axle_indicator;
    uint32_t light_aux_front;
    uint32_t light_aux_roof;
    bool light_beacon;
    bool light_brake;
    bool light_high_beam;
    bool light_lblinker;
    bool light_low_beam;
    bool light_parking;
    bool light_rblinker;
    bool light_reverse;
    scs_value_fvector_t local_angular_acceleration;
    scs_value_fvector_t local_angular_velocity;
    scs_value_fvector_t local_linear_acceleration;
    scs_value_fvector_t local_linear_velocity;
    bool motor_brake;
    float navigation_distance;
    float navigation_speed_limit;
    float navigation_time;
    float odometer;
    float oil_pressure;
    bool oil_pressure_warning;
    float oil_temperature;
    bool parking_brake;
    bool rblinker;
    uint32_t retarder_level;
    float speed;
    bool trailer_lift_axle;
    bool trailer_lift_axle_indicator;
    float water_temperature;
    bool water_temperature_warning;
    float wear_cabin;
    float wear_chassis;
    float wear_engine;
    float wear_transmission;
    float wear_wheels;
    float wheel_lift;
    float wheel_lift_offset;
    bool wheel_on_ground;
    float wheel_rotation;
    float wheel_steering;
    uint32_t wheel_substance;
    float wheel_susp_deflection;
    float wheel_velocity;
    bool wipers;
    scs_value_dplacement_t world_placement;
};

struct trailer_data_t {
    float cargo_damage;
    bool connected;
    scs_value_fvector_t local_angular_acceleration;
    scs_value_fvector_t local_angular_velocity;
    scs_value_fvector_t local_linear_acceleration;
    scs_value_fvector_t local_linear_velocity;
    float wear_body;
    float wear_chassis;
    float wear_wheels;
    float wheel_lift;
    float wheel_lift_offset;
    bool wheel_on_ground;
    float wheel_rotation;
    float wheel_steering;
    uint32_t wheel_substance;
    float wheel_susp_deflection;
    float wheel_velocity;
    scs_value_dplacement_t world_placement;

};

struct telemetry_data_t {
    truck_data_t truck;
    trailer_data_t trailer;

    bool navigation;
    bool menuMusic;
};