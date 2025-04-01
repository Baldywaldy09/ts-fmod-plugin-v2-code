#include <iostream>
#include <fstream>
#include <sstream>

#include "prism.h"
#include "__patterns.h"
#include "../bmem.h"

std::string plugin_name;
bool log_info = false;
bool error_in_console = false;
prism::initialization::error_level_enum error_level;
prism::initialization::log_destination_enum log_destination;

bool has_logged = false;
void prism::initialization::found_addr(const char* name, uint64_t addr, bool function)
{
    std::string type = function ? "function" : "class";

    std::stringstream ss;
    ss << "[prism] Found " << type << " '" << name << "' at '" << std::uppercase << std::hex << addr << "'" << std::endl;

    if (log_destination == initialization::log_destination_enum::prism_console) {}
      //  global::scs_log(0, ss.str().c_str());

    else if (log_destination == initialization::log_destination_enum::windows_console)
        std::cout << "<INFO>  | " << "[" << plugin_name << "]" << ss.str();

    else if (log_destination == initialization::log_destination_enum::local_file) {
        std::ofstream file;
        if (!has_logged) file.open("plugins/" + plugin_name + "_B-Prism3D.log", std::ios::out | std::ios::trunc);
        else file.open("plugins/" + plugin_name + "_B-Prism3D.log", std::ios::out | std::ios::app);

        file << "<INFO>  | " << ss.str();
        has_logged = true;
    }
}

void prism::initialization::missing_addr(const char* name, bool function)
{
    if (error_level == initialization::error_level_enum::ignore) return;
     
    std::string type = function ? "function" : "class";

    std::stringstream ss;
    ss << "[prism] Could not find " << type << " '" << name << "'" << std::endl;

    if (error_level == initialization::error_level_enum::error || error_level == initialization::error_level_enum::hault)
        type = "<ERROR> | ";
    else if (error_level == initialization::error_level_enum::warning)
        type = "<WARN>  | ";

    if (log_destination == initialization::log_destination_enum::prism_console || error_in_console) {
        int level = -1;

        if (type == "<ERROR> | ") level = 2;
        else if (type == "<WARN>  | ") level = 1;

        //global::scs_log(level, ss.str().c_str());
    }

    else if (log_destination == initialization::log_destination_enum::windows_console)
        std::cout << type << "[" << plugin_name << "]" << ss.str();

    else if (log_destination == initialization::log_destination_enum::local_file) {
        std::ofstream file;
        if (!has_logged) file.open("plugins/" + plugin_name + "_B-Prism3D.log", std::ios::out | std::ios::trunc);
        else file.open("plugins/" + plugin_name + "_B-Prism3D.log", std::ios::out | std::ios::app);

        file << type << ss.str() ;
        has_logged = true;
    }
}

template <typename FuncType>
bool prism::initialization::load_function(const char* name, const char* pattern, int offset, FuncType& function) {
    uint64_t func_address = (bmem::getAddressFromPattern(pattern) + offset);

    if (!bmem::is_address_valid(func_address)) {
        missing_addr(name, true);
        if (error_level == error_level_enum::hault) return false;
    }
    else {
        found_addr(name, func_address, true);
    }

    function = reinterpret_cast<FuncType>(func_address);
    return true;
}

template <typename ClassType>
bool prism::initialization::load_class(const char* name, const char* pattern, int offset, ClassType& class_type) {
    uint64_t instruction_address = (bmem::getAddressFromPattern(pattern) + offset);
    uint64_t class_address = bmem::relativeToAbsolute<uint64_t>(instruction_address, 3, 7);

    if (!bmem::is_address_valid(class_address)) {
        missing_addr(name, false);
        if (error_level == error_level_enum::hault) return false;
    }
    else {
        found_addr(name, class_address, false);
    }

    class_type = reinterpret_cast<ClassType>(class_address);
    return true;
}

bool prism::initialization::init_functions(error_level_enum _error_level, log_destination_enum _log_destination, bool _log_info, bool _error_in_console, std::string _plugin_name) {
    error_level     = _error_level;
    log_destination = _log_destination;
    log_info        = _log_info;
    error_in_console = _error_in_console;
    plugin_name     = _plugin_name;

    bmem::init("currentproc");

    // = = = = = prism::ui = = = = = //
    if (!load_function("prism::ui::load_window", load_window_pattern, 0, prism::ui::load_window)) return false;
    if (!load_function("prism::ui::update_value", update_value_pattern, 0, prism::ui::update_value)) return false;
    // = = = = = prism::ui = = = = = //
    
    // = = = = = prism::pointer_base = = = = = //
    if (!load_class("prism::pointer_base", pointer_base_pattern, 0, prism::pointer_base)) return false;
    // = = = = = prism::pointer_base = = = = = //

    return true;
}
