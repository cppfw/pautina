#pragma once

#include <setka/tcp_socket.hpp>
#include <utki/span.hpp>

namespace pautina {

class connection
{
	friend class connection_thread;

	setka::tcp_socket socket;

public:
	connection(setka::tcp_socket&& socket);

	enum class state {
		receiving,
		sending
	};

private:
	state current_state = state::receiving;

	// stuff used in 'sending' state
	std::vector<uint8_t> data_to_send;
	size_t num_bytes_sent;

public:
	state state() const noexcept
	{
		return this->current_state;
	}

	void handle_received_data(utki::span<const uint8_t> data);

	void handle_all_data_sent();
};

} // namespace pautina
