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

#include "headers_parser.hpp"

#include <utki/string.hpp>

#include "util.hxx"

using namespace httpmodel;

utki::span<const uint8_t> headers_parser::parse_name(utki::span<const uint8_t> data)
{
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		if (c == ':') {
			this->header_name = utki::make_string(this->buf);
			this->buf.clear();

			this->cur_state = state::skip_spaces;
			this->state_after_skiping_spaces = state::value;
			++i;
			break;
		} else if (c == '\n' && (this->buf.empty() || (this->buf.size() == 1 && this->buf[0] == '\r'))) {
			// end of HTTP request header
			this->cur_state = state::end;
			++i;
			break;
		}

		this->buf.push_back(c);
	}
	data = data.subspan(std::distance(data.begin(), i));
	return data;
}

utki::span<const uint8_t> headers_parser::parse_value(utki::span<const uint8_t> data)
{
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		if (c == '\n') {
			if (!this->buf.empty() && this->buf.back() == '\r') {
				this->buf.pop_back();
			}
			this->headers.add(std::move(this->header_name), utki::make_string(this->buf));
			this->buf.clear();
			this->cur_state = state::name;
			++i;
			break;
		}

		this->buf.push_back(c);
	}
	data = data.subspan(std::distance(data.begin(), i));
	return data;
}

utki::span<const uint8_t> headers_parser::feed(utki::span<const uint8_t> data)
{
	while (!data.empty()) {
		switch (this->cur_state) {
			case state::skip_spaces:
				ASSERT(!data.empty())
				data = parse_skip_spaces(data);
				if (!data.empty()) {
					// spaces skipped
					this->cur_state = this->state_after_skiping_spaces;
				}
				break;
			case state::name:
				data = this->parse_name(data);
				break;
			case state::value:
				data = this->parse_value(data);
				break;
			case state::end:
				return data;
		}
	}
	return data;
}
