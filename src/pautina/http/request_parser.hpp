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

#include "headers_parser.hpp"
#include "request.hpp"

namespace pautina::http {

class request_parser
{
	enum class state {
		skip_spaces,
		method,
		path,
		protocol,
		headers,
		body,
		end
	};

	state cur_state = state::method;
	state state_after_skiping_spaces;

	std::vector<char> buf;

	utki::span<const uint8_t> parse_method(utki::span<const uint8_t> data);
	utki::span<const uint8_t> parse_path(utki::span<const uint8_t> data);
	utki::span<const uint8_t> parse_protocol(utki::span<const uint8_t> data);
	utki::span<const uint8_t> parse_body(utki::span<const uint8_t> data);

	std::string header_name; // for storing header name until header value is parsed

	http::headers_parser headers_parser;

	void check_required_headers();

	void set_state_after_headers();

	size_t num_body_bytes_expected; // holds number of body bytes remained to read when in state::body

public:
	http::request request;

	/**
	 * @brief Feed data portion to parse.
	 * @param data - portion of data to parse.
	 * @return span remained after parsing. It can be non-empty in case
	 *     HTTP header end has been encountered in the middle of the fed data.
	 * @throw std::invalid_argument in case of malformed HTTP header.
	 */
	utki::span<const uint8_t> feed(utki::span<const uint8_t> data);

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

} // namespace pautina::http
