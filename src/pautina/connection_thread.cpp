#include "connection_thread.hpp"

using namespace pautina;

connection_thread::connection_thread(setka::tcp_socket&& socket) :
	wait_set(2),
	connection(std::make_unique<pautina::connection>(std::move(socket)))
{
	this->wait_set.add(
		this->connection->socket,
		opros::ready::read
	); // TODO: what if adding fails, catch exception? Deinit gracefully.
	this->wait_set.add(this->queue, opros::ready::read);

	this->connection->socket.user_data = this->connection.get();
}

connection_thread::~connection_thread()
{
	this->wait_set.remove(this->queue);
	this->wait_set.remove(this->connection->socket);
}

void connection_thread::run()
{
	while (!this->quit_flag) {
		this->wait_set.wait();

		if (this->queue.flags().get(opros::ready::read)) {
			while (auto proc = this->queue.pop_front()) {
				proc.operator()();
			}
		}

		if (this->connection->socket.flags().get(opros::ready::read)) {
			if (this->connection->state() != connection::state::receiving) {
				// TODO: illegal state, handle the error
			}

			constexpr const size_t receive_buffer_size = 0x1000; // 4kb
			std::array<uint8_t, receive_buffer_size> buf;

			while (size_t num_bytes_received = this->connection->socket.receive(buf)) {
				ASSERT(num_bytes_received > 0)
				this->connection->handle_received_data(utki::make_span(buf.data(), num_bytes_received));
				if (this->connection->state() == connection::state::sending) {
					// state has changed to 'sending'
					this->wait_set.change(this->connection->socket, opros::ready::write);
					break;
				}
			}
		}

		if (this->connection->socket.flags().get(opros::ready::write)) {
			// TODO: write
		}
	}
}
