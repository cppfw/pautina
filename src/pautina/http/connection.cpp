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

using namespace std::string_literals;

namespace {
const std::string keep_alive_header_value = "keep-alive"s;
const std::string close_header_value = "close"s;
} // namespace

connection::connection(setka::tcp_socket&& socket, const pautina::http::server& owner) :
	pautina::connection(std::move(socket)),
	owner(owner)
{}

httpmodel::response connection::handle_request(const httpmodel::request& req)
{
	auto connection_header = req.headers.get(httpmodel::to_string(httpmodel::header::connection));

	if (req.protocol == httpmodel::protocol::http_1_0) {
		// check if connection is persistent
		// Persistent connection spec: https://datatracker.ietf.org/doc/html/rfc7230#section-6.3
		if (connection_header.has_value() && connection_header.value() == keep_alive_header_value) {
			this->keep_alive = true;
		} else {
			this->keep_alive = false;
		}
	} else {
		ASSERT(req.protocol >= httpmodel::protocol::http_1_1)

		if (!req.headers.get(httpmodel::to_string(httpmodel::header::host))) {
			// HTTP/1.1+ request protocol requires 'Host' header, which is missing
			return {req, httpmodel::status::http_400_bad_request};
		}

		// check if connection is persistent
		// Persistent connection spec: https://datatracker.ietf.org/doc/html/rfc7230#section-6.3
		if (!connection_header.has_value() || connection_header.value() == close_header_value) {
			this->keep_alive = false;
		} else {
			this->keep_alive = true;
		}
	}

	return this->owner.router.route(req);
}

void connection::handle_front_request()
{
	ASSERT(!this->requests.empty())
	ASSERT(this->requests.front().is_end())

	auto resp = this->handle_request(this->requests.front().request);
	this->requests.pop_front();

	// Handle persistent connection.
	// In case client requested persistent connection, the server must append "Connection" header.
	// Persistent connection spec: https://datatracker.ietf.org/doc/html/rfc7230#section-6.3
	if (resp.protocol == httpmodel::protocol::http_1_0) {
		if (this->keep_alive) {
			// append "Connection" header, replacing existing one
			resp.headers.add(httpmodel::header::connection, std::string(keep_alive_header_value));
		}
	} else {
		ASSERT(resp.protocol >= httpmodel::protocol::http_1_1)
		if (!this->keep_alive) {
			// append "Connection" header, replacing existing one
			resp.headers.add(httpmodel::header::connection, std::string(close_header_value));
		}
	}

	// send the response
	LOG([&](auto& o) {
		o << "sending http response, status = " << to_string(resp.status) << std::endl;
	})
	this->send(resp.to_bytes_no_body());
	if (!resp.body.empty()) {
		// TODO: add Content-Length header?
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
			this->disconnect();
			return;
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

	// If the connection is not persistent, then close it after response has been sent.
	if (!this->keep_alive) {
		this->disconnect();
		return;
	}

	// handle requests in queue
	if (this->requests.empty() || !this->requests.front().is_end()) {
		this->set_can_receive_data(true);
	} else {
		ASSERT(!this->requests.empty())
		ASSERT(this->requests.front().is_end())

		this->handle_front_request();
	}
}
