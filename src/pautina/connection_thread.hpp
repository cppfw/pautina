#pragma once

#include <nitki/queue.hpp>
#include <nitki/thread.hpp>
#include <opros/wait_set.hpp>
#include <setka/tcp_socket.hpp>

#include "connection.hpp"

namespace pautina {

class connection_thread : public nitki::thread
{
	nitki::queue queue;

	opros::wait_set wait_set;

	bool quit_flag = false;

	std::unique_ptr<pautina::connection> connection;

public:
	connection_thread(setka::tcp_socket&& socket);
	~connection_thread();

	void run() override;
};

} // namespace pautina
