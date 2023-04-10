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

#include "connection.hpp"

#include <utki/string.hpp>

using namespace pautina::http;

connection::connection(setka::tcp_socket&& socket) :
	pautina::connection(std::move(socket))
{}

void connection::handle_received_data(utki::span<const uint8_t> data)
{
	ASSERT(!data.empty())

	LOG([&](auto& o) {
		o << "connection::handle_received_data(): " << utki::make_string(data) << std::endl;
	})

	if (this->requests.empty()) {
		this->requests.emplace_back();
	}

	while (!data.empty()) {
		ASSERT(!this->requests.empty())
		if (this->requests.back().is_end()) {
			this->requests.emplace_back();
		}

		try {
			data = this->requests.back().feed(data);
		} catch (std::invalid_argument& e) {
			LOG([&](auto& o) {
				o << "std::invalid_argument while parsing http request: " << e.what() << "\n";
				o << "data = " << utki::make_string(data) << std::endl;
			})
			this->set_can_receive_data(false);
			// TODO: bad request
			break;
		}
	}

	// TODO: handle parsed requests
}

void connection::handle_data_sent()
{
	// TODO: handle requests in queue
}
