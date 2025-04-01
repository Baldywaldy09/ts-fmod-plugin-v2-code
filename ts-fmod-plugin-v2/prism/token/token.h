#pragma once
#include <string>

#define prism_token uint64_t

namespace prism_token_manager
{
    struct ui_div {
        uint64_t rem;  // remainder
        uint64_t quot; // quotient
    };

    prism_token string_to_token(const std::string& text);
    std::string token_to_string(prism_token token);
}