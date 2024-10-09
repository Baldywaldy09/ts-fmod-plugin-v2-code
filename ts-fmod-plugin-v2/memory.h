#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <algorithm>

namespace memory
{
    struct pattern_byte
    {
        pattern_byte() : ignore(true) {}

        explicit pattern_byte(const std::string& byte_string, const bool ignore_this = false) : ignore(ignore_this),
            data(string_to_uint8(byte_string)) {}

        bool ignore;
        uint8_t data;

    private:
        static uint8_t string_to_uint8(const std::string& str)
        {
            std::istringstream iss(str);
            uint32_t ret;

            if (iss >> std::hex >> ret) return static_cast<uint8_t>(ret);

            return 0;
        }
    };

    static uint64_t game_base;
    static uint64_t image_size;

    static void init(const char* moduleName)
    {
        if (moduleName == "eurotrucks2.exe" || moduleName == "amtrucks.exe") { moduleName = nullptr; }

        game_base = reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName));
        const auto header = reinterpret_cast<const IMAGE_DOS_HEADER*>(game_base);
        const auto nt_header = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(header) + header->e_lfanew);
        image_size = nt_header->OptionalHeader.SizeOfImage;
    }

    static uint64_t get_game_base(const char* moduleName = "eurotrucks2.exe")
    {
        init(moduleName);

        return game_base;
    }

    static uint64_t get_address_from_pattern(const std::string pattern, const char* moduleName = "eurotrucks2.exe")
    {
        init(moduleName);

        std::vector<pattern_byte> p;
        std::istringstream iss(pattern);
        std::string w;

        while (iss >> w)
        {
            if (w[0] == '?')
            {
                // Wildcard
                p.emplace_back();
            }
            else if (w.length() == 2 && isxdigit(w[0]) && isxdigit(w[1]))
            {
                // Hex
                p.emplace_back(w);
            }
            else  return NULL;
        }

        for (uint64_t i = 0; i < image_size; i++)
        {
            auto current_byte = reinterpret_cast<uint8_t*>(game_base + i);
            auto found = true;

            for (size_t ps = 0; ps < p.size(); ps++)
            {
                if (p[ps].ignore == false && current_byte[ps] != p[ps].data)
                {
                    found = false;
                    break;
                }
            }

            if (found) return reinterpret_cast<uint64_t>(current_byte);
        }

        return NULL;
    }
}