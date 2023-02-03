#pragma once

#include <list>

#include <nitki/queue.hpp>
#include <nitki/thread.hpp>
#include <opros/wait_set.hpp>
#include <setka/tcp_socket.hpp>

#include "connection.hpp"

namespace pautina {

class http_server;

class connection_thread : public nitki::thread
{
	friend class http_server;

	http_server& owner;
	std::list<connection_thread>::iterator owner_iter;

	nitki::queue queue;

	opros::wait_set wait_set;

	bool quit_flag = false;

	std::unique_ptr<pautina::connection> connection;

public:
	connection_thread(http_server& owner, setka::tcp_socket&& socket);
	~connection_thread() override;

	void run() override;

	void quit();
};

} // namespace pautina
