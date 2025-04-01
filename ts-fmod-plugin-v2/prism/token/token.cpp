#include <vector>

#include "token.h"
using namespace prism_token_manager;

const std::vector<char> letters = {
    '\0', '0', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
    't', 'u', 'v', 'w', 'x', 'y', 'z', '_'
};


uint64_t pow_ul(int num) {
    uint64_t num2 = 1;
    for (int i = 0; num > i; i++) {
        num2 *= letters.size();
    }
    return num2;
}

int get_id_char(char letter) {
    for (size_t i = 0; i < letters.size(); i++) {
        if (letter == letters[i]) {
            return static_cast<int>(i);
        }
    }
    return 0;
}

ui_div divide(uint64_t num, uint64_t divider) {
    ui_div result;
    result.rem = num % divider;
    result.quot = num / divider;
    return result;
}

uint64_t prism_token_manager::string_to_token(const std::string& text) {
    uint64_t num = 0;
    int length = text.length();
    for (int i = 0; i < length; i++) {
        num += static_cast<uint64_t>(pow_ul(i) * get_id_char(std::tolower(text[i])));
    }
    return num;
}

std::string prism_token_manager::token_to_string(prism_token token) {
    std::string result;
    while (token != 0) {
        ui_div ul_div = divide(token, letters.size());
        token = ul_div.quot;
        result.push_back(letters[ul_div.rem]);
    }
    return result;
}