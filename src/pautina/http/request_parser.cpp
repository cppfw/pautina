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

#include "request_parser.hpp"

#include <locale>
#include <sstream>
#include <stdexcept>

#include <utki/string.hpp>

using namespace pautina;

utki::span<const uint8_t> request_parser::parse_skip_spaces(utki::span<const uint8_t> data)
{
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);
		if (c != ' ') {
			this->cur_state = this->state_after_skiping_spaces;
			break;
		}
	}
	data = data.subspan(std::distance(data.begin(), i));
	return data;
}

utki::span<const uint8_t> request_parser::parse_method(utki::span<const uint8_t> data)
{
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		if (c == ' ') {
			// method name read
			this->request.method = http::method_from_string(utki::make_string_view(this->buf));

			this->buf.clear();

			this->cur_state = state::skip_spaces;
			this->state_after_skiping_spaces = state::path;
			++i;
			break;
		}

		this->buf.push_back(std::toupper(c, std::locale::classic()));
	}
	data = data.subspan(std::distance(data.begin(), i));
	return data;
}

utki::span<const uint8_t> request_parser::parse_path(utki::span<const uint8_t> data)
{
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		if (c == ' ') {
			this->request.path = utki::make_string(this->buf);
			this->buf.clear();
			this->cur_state = state::skip_spaces;
			this->state_after_skiping_spaces = state::protocol;
			++i;
			break;
		}

		this->buf.push_back(c);
	}
	data = data.subspan(std::distance(data.begin(), i));
	// std::cout << "parse_path(): data = " << data << std::endl;
	return data;
}

utki::span<const uint8_t> request_parser::parse_protocol(utki::span<const uint8_t> data)
{
	// std::cout << "parse_protocol(): data = " << data << std::endl;
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		if (c == '\n') {
			this->request.protocol = http::protocol_from_string(utki::make_string_view(this->buf));

			this->buf.clear();

			this->cur_state = state::headers;
			++i;
			break;
		}

		this->buf.push_back(c);
	}
	data = data.subspan(std::distance(data.begin(), i));
	return data;
}

utki::span<const uint8_t> request_parser::parse_body(utki::span<const uint8_t> data)
{
	// TODO:
	return data;
}

utki::span<const uint8_t> request_parser::feed(utki::span<const uint8_t> data)
{
	while (!data.empty()) {
		switch (this->cur_state) {
			case state::skip_spaces:
				data = this->parse_skip_spaces(data);
				break;
			case state::method:
				data = this->parse_method(data);
				break;
			case state::path:
				data = this->parse_path(data);
				// std::cout << "feed(): data = " << data << std::endl;
				break;
			case state::protocol:
				// std::cout << "feed(): data = " << data << std::endl;
				data = this->parse_protocol(data);
				break;
			case state::headers:
				data = this->headers_parser.feed(data);
				if (this->headers_parser.is_end()) {
					this->request.headers = std::move(this->headers_parser.headers);
					this->check_required_headers();

					// TODO: decide if there should be body following headers
					this->cur_state = state::end;

					return data;
				}
				break;
			case state::body:
				data = this->parse_body(data);
				break;
			case state::end:
				return data;
		}
	}
	return data;
}

void request_parser::check_required_headers()
{
	if (this->request.protocol >= http::protocol::http_1_1) {
		if (!this->request.headers.get(http::to_string(http::header::host))) {
			std::stringstream ss;
			ss << "HTTP/1.1+ request protocol requires 'Host' header, which is missing";
			throw std::invalid_argument(ss.str());
		}
	}
}
