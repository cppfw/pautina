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

public:
	state state() const noexcept
	{
		return this->current_state;
	}

	void handle_received_data(utki::span<const uint8_t> data);

	std::vector<uint8_t> get_data_to_send();
};

} // namespace pautina
