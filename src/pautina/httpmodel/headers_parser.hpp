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

#include <string>
#include <string_view>
#include <vector>

#include <utki/span.hpp>

#include "headers.hpp"

namespace pautina::httpmodel {

class headers_parser
{
	enum class state {
		skip_spaces,
		name,
		value,
		end
	};

	state cur_state = state::name;
	state state_after_skiping_spaces;

	std::vector<char> buf; // for storing currently parsed string

	utki::span<const uint8_t> parse_name(utki::span<const uint8_t> data);
	utki::span<const uint8_t> parse_value(utki::span<const uint8_t> data);

	std::string header_name; // for storing header name until header value is parsed

public:
	httpmodel::headers headers;

	/**
	 * @brief Feed data portion to parse.
	 * @param data - portion of data to parse.
	 * @return span remained after parsing. It can be non-empty in case
	 *     HTTP headers end has been encountered in the middle of the fed data.
	 * @throw std::invalid_argument in case of malformed HTTP headers.
	 */
	utki::span<const uint8_t> feed(utki::span<const uint8_t> data);

	/**
	 * @brief Check if end of HTTP headers is reached.
	 * @return true if end of HTTP headers is reached.
	 * @return false otherwise.
	 */
	bool is_end() const noexcept
	{
		return this->cur_state == state::end;
	}
};

} // namespace pautina::httpmodel
