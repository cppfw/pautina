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

#include "parser.hpp"

#include <utki/string.hpp>

using namespace pautina::urlmodel;

utki::span<const uint8_t> parser::parse_scheme(utki::span<const uint8_t> data)
{
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		c = std::tolower(c, std::locale::classic());

		if (c == '/') {
			// start with path without scheme and authority
			this->cur_state = state::path;
			++i;
			break;
		} else if (c == ':') {
			// end of scheme
			this->uri.scheme = utki::make_string(this->buf);
			this->buf.clear();
			this->cur_state = state::authority;
			++i;
			break;
		}

		if (this->buf.empty()) {
			if (!std::isalpha(c, std::locale::classic())) {
				std::stringstream ss;
				ss << "urlmodel: first char of URL scheme is not alphabethic: " << c;
				throw std::invalid_argument(ss.str());
			}
		} else {
			if (!(std::isalpha(c, std::locale::classic()) || std::isdigit(c, std::locale::classic()) || c == '+'
				  || c == '-' || c == '.'))
			{
				std::stringstream ss;
				ss << "urlmodel: URL scheme contains forbidden character: " << c;
				throw std::invalid_argument(ss.str());
			}
		}

		this->buf.push_back(c);
	}
	data = data.subspan(std::distance(data.begin(), i));
	return data;
}

utki::span<const uint8_t> parser::parse_authority(utki::span<const uint8_t> data)
{
	// TODO:
	return nullptr;
}

utki::span<const uint8_t> parser::feed(utki::span<const uint8_t> data)
{
	while (!data.empty()) {
		switch (this->cur_state) {
			case state::scheme:
				data = this->parse_scheme(data);
				break;
			case state::authority:
				data = this->parse_authority(data);
				break;
			case state::end:
				return data;
			default: // TODO: remove default case
				break;
		}
	}
	return data;
}
