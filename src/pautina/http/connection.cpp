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

connection::connection(setka::tcp_socket&& socket, const pautina::http::server& owner) :
	pautina::connection(std::move(socket)),
	owner(owner)
{}

void connection::handle_front_request()
{
	auto resp = this->owner.router.route(this->requests.front().request);
	this->requests.pop_front();

	// send the response
	LOG([&](auto& o) {
		o << "sending http response, status = " << to_string(resp.status) << std::endl;
	})
	this->send(resp.to_bytes_no_body());
	if (!resp.body.empty()) {
		this->send(std::move(resp.body));
	}

	// if not all requests have been handled wait with receiving more data
	if (!this->requests.empty() && this->requests.front().is_end()) {
		ASSERT(this->is_sending())
		this->set_can_receive_data(false);
	}
}

void connection::handle_data_received(utki::span<const uint8_t> data)
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
		ASSERT(!this->requests.back().is_end())

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

		if (this->requests.back().is_end()) {
			LOG([&](auto& o) {
				o << "http request parsed" << std::endl;
			})
			this->requests.emplace_back();
		}
	}

	ASSERT(!this->requests.empty())

	// handle parsed requests
	if (this->requests.front().is_end()) {
		if (!this->is_sending()) {
			// handling request will send the response, so we only want to
			// send something unless there is no sending in progress
			this->handle_front_request();
		}
	}
}

void connection::handle_all_data_sent()
{
	ASSERT(!this->is_sending())

	// handle requests in queue
	if (this->requests.empty() || !this->requests.front().is_end()) {
		this->set_can_receive_data(true);
	} else {
		ASSERT(!this->requests.empty())
		ASSERT(this->requests.front().is_end())

		this->handle_front_request();
	}
}
