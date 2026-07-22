// File: cvar.cpp
// Credit: Baldy09 | https://github.com/Baldywaldy09/

#include "cvar.h"
#include "../bmem.h"
#include "../common.h"

bool initialised = false;

scs_log_t scs_log_;
int prism::cvar::init(scs_log_t scs_log__)
{
    scs_log_ = scs_log__;
    if (initialised) return SCS_RESULT_already_registered;

    initialised = true;

    return SCS_RESULT_ok;
}

prism::cvar::value* prism::cvar::get_pointer(const char* cvar_name)
{
    if (!initialised) return nullptr;

    std::ostringstream oss;
    for (size_t i = 0; cvar_name[i] != '\0'; ++i) {
        if (i > 0) oss << ' ';
        oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(cvar_name[i]) & 0xFF);
    }
    
    // These are needed to make sure we get the cvar address and nothing else that uses the cvar name
    std::string blank_bytes = " 00 00 00 00 00 00 00 00";

    std::string cvar_name_bytes = oss.str() + blank_bytes;

   // scs_log_(0, cvar_name);
   // scs_log_(0, cvar_name_bytes.c_str());

    uint64_t cvar_adrress = bmem::patternScan(cvar_name_bytes.c_str());
    if (cvar_adrress == NULL)
    {
        scs_log_(0, ("[prism::cvar] Cannot Find: " + std::string(cvar_name)).c_str());
    }

    if (!bmem::isAddressValid((cvar_adrress - 0x8))) {
        scs_log_(0, ("[prism::cvar] Cannot Find: " + std::string(cvar_name)).c_str());
    }

    std::stringstream ss;
    ss << std::hex << cvar_adrress - 0x8;
   // scs_log_(0, ss.str().c_str());
    
    return (prism::cvar::value*)(cvar_adrress - 0x8);
}

