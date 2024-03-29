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

#include <deque>

#include <httpmodel/request_parser.hpp>
#include <tcpserver/connection.hpp>

#include "server.hpp"

namespace pautina {

class connection : public tcpserver::connection
{
	std::deque<httpmodel::request_parser> requests;

	const pautina::server& owner;

	void handle_front_request();

	httpmodel::response handle_request(const httpmodel::request& req);

	bool keep_alive = false;

public:
	connection(setka::tcp_socket&& socket, const pautina::server& owner);

	void handle_data_received(utki::span<const uint8_t> data) override;

	void handle_all_data_sent() override;
};

} // namespace pautina
