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

#include <utki/span.hpp>

#include "url.hpp"

namespace pautina::urlmodel {

class parser
{
	enum class state {
		scheme,
		authority,
		path,
		query_name,
		query_value,
		fragment,
		end
	};

	state cur_state = state::scheme;

	std::vector<uint8_t> buf;

	utki::span<const uint8_t> parse_scheme(utki::span<const uint8_t> data);
	utki::span<const uint8_t> parse_authority(utki::span<const uint8_t> data);

public:
	urlmodel::uri uri;

	/**
	 * @brief Feed data portion to parse.
	 * @param data - portion of data to parse.
	 * @return span remained after parsing. It can be non-empty in case
	 *     URI end has been encountered in the middle of the fed data.
	 * @throw std::invalid_argument in case of malformed HTTP header.
	 */
	utki::span<const uint8_t> feed(utki::span<const uint8_t> data);

	/**
	 * @brief End of input data reached.
	 * This function tells parser that end of input data has been reached.
	 */
	void end_of_data();

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

} // namespace pautina::urlmodel
