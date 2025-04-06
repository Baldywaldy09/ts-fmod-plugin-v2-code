#pragma once

// prism::ui //
#define load_window_pattern "4C 8B DC 49 89 5B ?? 49 89 73 ?? 49 89 53 ?? 57 41 54 41 55 41 56 41 57"
#define update_value_pattern "48 89 5C 24 ?? 57 48 83 EC ?? 8B DA 48 8B F9 F6 C2 ?? 74"

// prism::pointer_base //
#define pointer_base_pattern "48 8B 3D ?? ?? ?? ?? 38 87 ?? ?? 00 00"

// prism::spawn_model
#define spawn_model_pattern "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 40 80 BC 24 80 00 00 00 00 49 8B F1 4D 8B F8 48 8B EA 4C 8B F1" // BAD