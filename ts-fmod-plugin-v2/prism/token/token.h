#pragma once
#include <string>

#define token_t uint64_t

namespace prism_token_manager
{
    struct ui_div {
        uint64_t rem;  // remainder
        uint64_t quot; // quotient
    };

    token_t string_to_token(const std::string& text);
    std::string token_to_string(token_t token);
}