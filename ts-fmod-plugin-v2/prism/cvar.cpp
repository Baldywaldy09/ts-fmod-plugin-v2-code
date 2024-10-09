// File: cvar.cpp
// Credit: Baldy09 | https://github.com/Baldywaldy09/

#include "cvar.h"
#include "../memory.h"
#include "../common.h"

bool initialised = false;

uint64_t unk_profile_base_ptr;

typedef char(__fastcall* set_value_t) (__int64 cvar_pointer, __int64 new_value);
set_value_t g_set_value;

typedef __int64(__fastcall* get_value_t) (__int64 cvar_pointer);
get_value_t g_get_value;

typedef __int64(__fastcall* register_value_t) (__int64 cvar_pointer);
register_value_t g_register;

typedef __int64(__fastcall* unregister_value_t) (__int64 cvar_pointer);
unregister_value_t g_unregister;

typedef __int64(__fastcall* g_store_t) (const char* file_path, int unk);
g_store_t g_store;

bool is_float(__int64 value) {
    // Float values can often have larger ranges and fractions
    // We assume anything larger than a certain range is a float
    return value > 1000000000LL || value < -1000000000LL ||
        std::fabs(static_cast<double>(value) - static_cast<int64_t>(value)) > 1e-6;
}

scs_log_t scs_log_;
int prism::cvar::init(scs_log_t scs_log__)
{
    scs_log_ = scs_log__;
    if (initialised) return SCS_RESULT_already_registered;

    const auto g_set_cvar_valueAddress = memory::get_address_from_pattern("48 89 5C 24 08 57 48 83 EC ?? 48 8B 81 18");
    if (g_set_cvar_valueAddress == NULL)
    {
        scs_log_(2, "[ts-fmod-plugin-v2][prism::cvar] Unable to find the pointer offset for the \"set_value\" function");
        return SCS_RESULT_generic_error;
    }
    g_set_value = (set_value_t)(g_set_cvar_valueAddress);

    const auto g_get_cvar_valueAddress = memory::get_address_from_pattern("40 57 48 83 EC ?? 48");
    if (g_get_cvar_valueAddress == NULL)
    {
        scs_log_(2, "[ts-fmod-plugin-v2][prism::cvar] Unable to find the pointer offset for the \"get_value\" function");
        return SCS_RESULT_generic_error;
    }
    g_get_value = (get_value_t)(g_get_cvar_valueAddress);


    const auto g_register_cvar_valueAddress = memory::get_address_from_pattern("4C 8D 81 A4");
    if (g_register_cvar_valueAddress == NULL)
    {
        scs_log_(2, "[ts-fmod-plugin-v2][prism::cvar] Unable to find the pointer offset for the \"register_value\" function");
        return SCS_RESULT_generic_error;
    }
    g_register = (register_value_t)(g_register_cvar_valueAddress);


    const auto g_unregister_cvar_valueAddress = memory::get_address_from_pattern("83 B9 ?? ?? ?? ?? ?? 75 ?? 48 8B");
    if (g_unregister_cvar_valueAddress == NULL)
    {
        scs_log_(2, "[ts-fmod-plugin-v2][prism::cvar] Unable to find the pointer offset for the \"unregister_value\" function");
        return SCS_RESULT_generic_error;
    }
    g_unregister = (unregister_value_t)(g_unregister_cvar_valueAddress);

    const auto g_store_address = memory::get_address_from_pattern("40 55 41 56 48 8D 6C 24 ?? 48 81 EC ?? ?? 00 00 80 3D ?? ?? ?? ?? ??");
    if (g_store_address == NULL)
    {
        scs_log_(2, "[ts-fmod-plugin-v2][prism::cvar] Unable to find the pointer offset for the \"store\" function");
        return SCS_RESULT_generic_error;
    }
    g_store = (g_store_t)(g_store_address);

    const auto unk_profile_base_instruction = memory::get_address_from_pattern("48 8B 0D ?? ?? ?? ?? 49 C7 C4 ?? ?? ?? ?? 4C 89 BD ?? ?? 00 00");
    if (unk_profile_base_instruction == NULL)
    {
        scs_log_(2, "[ts-fmod-plugin-v2][prism::cvar] Unable to find the pointer offset for the \"profile_base\"");
        return SCS_RESULT_generic_error;
    }
    unk_profile_base_ptr = unk_profile_base_instruction + *reinterpret_cast<int32_t*>(unk_profile_base_instruction + 3) + 7;
    
    const auto game_base = memory::get_game_base();

    std::stringstream ss;
    ss << "[ts-fmod-plugin-v2][prism::cvar] Found g_set_value: 'game_base+" << std::hex << (g_set_cvar_valueAddress - game_base)
        << "', g_get_value: 'game_base+" << std::hex << (g_get_cvar_valueAddress - game_base)
        << "', g_register: 'game_base+" << (g_register_cvar_valueAddress - game_base)
        << "', g_unregister: 'game_base+" << (g_unregister_cvar_valueAddress - game_base) 
        << "', g_store: 'game_base+" << (g_store_address - game_base)
        << "', profile_base: 'game_base+" << (unk_profile_base_ptr - game_base) << "'";

    scs_log_(0, ss.str().c_str());

    initialised = true;
	return SCS_RESULT_ok;
}

prism::cvar::pointer prism::cvar::get_pointer(const char* cvar_name)
{
    if (!initialised) return NULL;

    std::ostringstream oss;
    for (size_t i = 0; cvar_name[i] != '\0'; ++i) {
        if (i > 0) oss << ' ';
        oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(cvar_name[i]) & 0xFF);
    }
    
    // These are needed to make sure we get the cvar address and nothing else that uses the cvar name
    std::string blank_bytes = " 00 00 00 00";

    std::string cvar_name_bytes = oss.str() + blank_bytes;

   // scs_log_(0, cvar_name_bytes.c_str());

    uint64_t cvar_adrress = memory::get_address_from_pattern(cvar_name_bytes);
    if (cvar_adrress == NULL)
    {
        scs_log_(0, ("[prism::cvar] Cannot Find: " + std::string(cvar_name)).c_str());
    }

    __int64 cvar_pointer = (__int64)cvar_adrress;
    return cvar_pointer;
}

// Cannot get string values
// Always returns float even if int value
float prism::cvar::get_value(prism::cvar::pointer cvar_pointer)
{
    if (!initialised) return NULL;

    __int64 raw_result = g_get_value(cvar_pointer);

    if (is_float(raw_result))
    {
        float result;
        std::memcpy(&result, &raw_result, sizeof(result));

        return result;
    }
    else return (int)raw_result;
}

// Can set: float, int and string values
__int64 prism::cvar::set_value(prism::cvar::pointer cvar_pointer, cvar_input_value new_value)
{
    if (!initialised) return NULL;

    const char* new_value_str = nullptr;
    std::visit([&new_value_str](auto&& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, float>) {
            static std::string temp_str = std::to_string(value);
            new_value_str = temp_str.c_str();
        }
        else if constexpr (std::is_same_v<T, int>) {
            static std::string temp_str = std::to_string(value);
            new_value_str = temp_str.c_str();
        }
        else if constexpr (std::is_same_v<T, const char*>) {
            new_value_str = value;
        }
    }, new_value);

    std::string cvar_name(reinterpret_cast<const char*>(cvar_pointer));

    return g_set_value(cvar_pointer, (__int64)new_value_str);
}

// dont work.. idk why 
prism::cvar::pointer prism::cvar::register_name(const char* cvar_name)
{
    if (!initialised) return NULL;

    size_t length = strlen(cvar_name) + 1;

    size_t total_size = length + 272 + sizeof(unsigned int);

    char* memory = new char[total_size];

    strcpy_s(memory, total_size, cvar_name);

    __int64 cvar_pointer = reinterpret_cast<__int64>(memory);

    g_register(cvar_pointer);

    return cvar_pointer;
}

__int64 prism::cvar::unregister_name(prism::cvar::pointer cvar_pointer)
{
    if (!initialised) return NULL;

    return g_unregister(cvar_pointer);
}

void prism::cvar::store(bool main_config, bool config_profile_local, bool config_profile)
{
    // 0 = global config // main_config
    // 1 = local profile config // config_profile_local
    // 2 = profile config // config_profile

    const char* profile_name = *reinterpret_cast<const char**>(
        *reinterpret_cast<uint64_t*>(
            *reinterpret_cast<uint64_t*>(
                *reinterpret_cast<uint64_t*>(unk_profile_base_ptr) + 0xBC0) + 0x0) + 0x18) + 0x0;

    std::ostringstream oss;
    for (size_t i = 0; profile_name[i] != '\0'; ++i) {
        oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(profile_name[i]) & 0xFF);
    }

    if (main_config) { g_store("/home/config.cfg", 0); }
    if (config_profile_local) { g_store(("/home/profiles/" + oss.str() + "/config_local.cfg").c_str(), 1); }
    if (config_profile) { g_store(("/home/profiles/" + oss.str() + "/config.cfg").c_str(), 2); }
}