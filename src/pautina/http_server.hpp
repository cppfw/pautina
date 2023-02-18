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

#include <string>
#include <list>

#include <nitki/queue.hpp>
#include <setka/tcp_server_socket.hpp>

#include "connection_thread.hpp"

namespace pautina {

class http_server
{
	nitki::queue queue;

	opros::wait_set wait_set;

	setka::tcp_server_socket accept_socket;

	volatile bool quit_flag = false;

	std::list<connection_thread> threads;

	void spawn_thread(setka::tcp_socket&& socket);
public:
	struct configuration{
		uint16_t port = 80;
	};

	http_server(configuration config);
	~http_server();

	void add(std::vector<std::string> path, std::function<void()> handler);

	void run();

	void quit();
};

} // namespace pautina
