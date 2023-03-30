/*
MIT License

Copyright (c) 2023 Ivan Gagis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "http.hpp"

namespace pautina {

class http_parser
{
	enum class state {
		skip_spaces,
		method,
		path,
		protocol,
		header_name,
		header_value,
		end
	};

	state cur_state = state::method;
	state state_after_skiping_spaces;

	std::vector<char> buf;

	std::string_view parse_skip_spaces(std::string_view str);
	std::string_view parse_method(std::string_view str);
	std::string_view parse_path(std::string_view str);
	std::string_view parse_protocol(std::string_view str);
	std::string_view parse_header_name(std::string_view str);
	std::string_view parse_header_value(std::string_view str);

	std::string header_name; // for storing header name until header value is parsed

public:
	http::protocol protocol;
	http::method method;
	std::string path;
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
	bool is_end() const noexcept
	{
		return this->cur_state == state::end;
	}
};

} // namespace pautina
