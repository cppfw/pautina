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

#include "util.hxx"

using namespace pautina::httpmodel;

utki::span<const uint8_t> request_parser::parse_method(utki::span<const uint8_t> data)
{
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		if (c == ' ') {
			// method name read
			this->request.method = httpmodel::method_from_string(utki::make_string_view(this->buf));

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

utki::span<const uint8_t> request_parser::parse_protocol(utki::span<const uint8_t> data)
{
	// std::cout << "parse_protocol(): data = " << data << std::endl;
	auto i = data.begin();
	for (; i != data.end(); ++i) {
		auto c = char(*i);

		if (c == '\n') {
			if (!this->buf.empty() && this->buf.back() == '\r') {
				// remove trailing carriage return, as it came from DOS line ending
				this->buf.pop_back();
			}

			this->request.protocol = httpmodel::protocol_from_string(utki::make_string_view(this->buf));

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
	auto num_bytes_to_read = data.size() <= this->num_body_bytes_expected ? data.size() : this->num_body_bytes_expected;
	std::copy(data.begin(), utki::next(data.begin(), num_bytes_to_read), std::back_inserter(this->request.body));
	this->num_body_bytes_expected -= num_bytes_to_read;
	if (this->num_body_bytes_expected == 0) {
		this->cur_state = state::end;
	}
	return data.subspan(num_bytes_to_read);
}

void request_parser::set_state_after_headers()
{
	// According to spec, server SHOULD read and forward message body on any request,
	// even if the message body is not assumed by the request method.
	// See section 4.3 of https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html

	auto content_length_header = this->request.headers.get(httpmodel::to_string(httpmodel::header::content_length));
	if (!content_length_header.has_value()) {
		// TODO: implement support of "Transfer-Encoding" header and
		//   "multipart/byteranges" mediatype.
		//   See section 4.4 of https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html

		// no body assumed
		this->cur_state = state::end;
		return;
	}

	ASSERT(content_length_header.has_value())
	utki::string_parser p(content_length_header.value());
	this->num_body_bytes_expected = p.read_number<size_t>();
	this->request.body.reserve(this->num_body_bytes_expected);
	this->cur_state = state::body;
}

void request_parser::check_required_headers()
{
	if (this->request.protocol >= httpmodel::protocol::http_1_1) {
		if (!this->request.headers.get(httpmodel::to_string(httpmodel::header::host))) {
			std::stringstream ss;
			ss << "HTTP/1.1+ request protocol requires 'Host' header, which is missing";
			throw std::invalid_argument(ss.str());
		}
	}
}

utki::span<const uint8_t> request_parser::feed(utki::span<const uint8_t> data)
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
			case state::method:
				data = this->parse_method(data);
				break;
			case state::path:
				data = this->url_parser.feed(data);

				if (this->url_parser.is_end()) {
					this->request.path = std::move(this->url_parser.url);
					this->cur_state = state::skip_spaces;
					this->state_after_skiping_spaces = state::protocol;
				}
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
					this->set_state_after_headers();
				} else {
					ASSERT(data.empty())
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
