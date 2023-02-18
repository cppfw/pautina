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
