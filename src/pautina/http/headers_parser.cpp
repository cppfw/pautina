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

using namespace pautina::http;

std::string_view headers_parser::parse_skip_spaces(std::string_view str)
{
	auto i = str.begin();
	for (; i != str.end(); ++i) {
		auto c = *i;
		if (c != ' ') {
			this->cur_state = this->state_after_skiping_spaces;
			break;
		}
	}
	str = str.substr(std::distance(str.begin(), i));
	return str;
}

std::string_view headers_parser::parse_name(std::string_view str)
{
	auto i = str.begin();
	for (; i != str.end(); ++i) {
		auto c = *i;

		if (c == ':') {
			this->header_name = utki::make_string(this->buf);
			this->buf.clear();

			this->cur_state = state::skip_spaces;
			this->state_after_skiping_spaces = state::value;
			++i;
			break;
		} else if (c == '\n' && this->buf.empty()) {
			// end of HTTP request header
			this->cur_state = state::end;
			break;
		}

		this->buf.push_back(c);
	}
	str = str.substr(std::distance(str.begin(), i));
	return str;
}

std::string_view headers_parser::parse_value(std::string_view str)
{
	auto i = str.begin();
	for (; i != str.end(); ++i) {
		auto c = *i;

		if (c == '\n') {
			this->headers.add(std::move(this->header_name), utki::make_string(this->buf));
			this->buf.clear();
			this->cur_state = state::name;
			++i;
			break;
		}

		this->buf.push_back(c);
	}
	str = str.substr(std::distance(str.begin(), i));
	return str;
}

std::string_view headers_parser::feed(std::string_view str)
{
	while (!str.empty()) {
		switch (this->cur_state) {
			case state::skip_spaces:
				str = this->parse_skip_spaces(str);
				break;
			case state::name:
				str = this->parse_name(str);
				break;
			case state::value:
				str = this->parse_value(str);
				break;
			case state::end:
				return str;
		}
	}
	return str;
}
