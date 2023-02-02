#pragma once

#include <functional>
#include <string>

#include <nitki/queue.hpp>
#include <setka/tcp_server_socket.hpp>

namespace pautina {

class http_server
{
	setka::tcp_server_socket accept_socket;

	nitki::queue queue;

	opros::wait_set wait_set;

	volatile bool quit_flag = false;

public:
	http_server();
	~http_server();

	void add(std::vector<std::string> path, std::function<void()> handler);

	void run();

	void quit();
};

} // namespace pautina
