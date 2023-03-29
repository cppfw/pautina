#pragma once

#include <string_view>
#include <vector>
#include <map>

#include "http.hpp"

namespace pautina{

class http_parser{
    enum class state{
        idle,
        skip_spaces,
        method,
        path,
        // protocol,
        // header_name,
        // header_value,
        end
    };

    state cur_state = state::idle;
    state state_after_skiping_spaces;

    std::vector<char> buf;

    std::string_view parse_idle(std::string_view str);
    std::string_view parse_skip_spaces(std::string_view str);
    std::string_view parse_method(std::string_view str);
public:
    http::protocol protocol;
    http::method method;
    std::map<std::string, std::string> headers;

    /**
     * @brief Feed text portion to parse.
     * @param str - portion of text to parse.
     * @return std::string_view remained after parsing. It can be non-empty in case
     *     HTTP header end has been encountered in the middle of the fed data.
     * @throw std::invalid_argument in case of malformed HTTP header.
     */
    std::string_view feed(std::string_view str);

    /**
     * @brief Check if end of HTTP header is reached.
     * @return true if end of HTTP header is reached.
     * @return false otherwise.
     */
    bool is_end()const noexcept{
        return this->cur_state == state::end;
    }
};

}
