#include "connection_thread.hpp"

using namespace pautina;

namespace {
constexpr const size_t waitset_size = 2;
} // namespace

connection_thread::connection_thread(setka::tcp_socket&& socket) :
	wait_set(waitset_size),
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
	std::vector<opros::waitable*> triggered(waitset_size);
	while (!this->quit_flag) {
		ASSERT(this->wait_set.size() <= triggered.size())
		size_t num_triggered = this->wait_set.wait(triggered);
		ASSERT(num_triggered <= triggered.size())

		for (size_t i = 0; i != num_triggered; ++i) {
			auto waitable = triggered[i];
			ASSERT(waitable)

			// check if it is a procedure queue
			if (waitable == static_cast<opros::waitable*>(&this->queue)) {
				ASSERT(this->queue.flags().get(opros::ready::read))
				while (auto proc = this->queue.pop_front()) {
					proc.operator()();
				}
				ASSERT(!this->queue.flags().get(opros::ready::read))
				continue;
			}

			// if we get here, then the triggered waitable is a tcp_socket
			auto c = reinterpret_cast<pautina::connection*>(waitable->user_data);

			switch (c->state()) {
				case connection::state::receiving:
					{
						ASSERT(c->socket.flags().get(opros::ready::read))

						constexpr const size_t receive_buffer_size = 0x1000; // 4kb
						std::array<uint8_t, receive_buffer_size> buf;

						while (size_t num_bytes_received = c->socket.receive(buf)) {
							ASSERT(num_bytes_received > 0)
							c->handle_received_data(utki::make_span(buf.data(), num_bytes_received));
							if (c->state() == connection::state::sending) {
								// state has changed to 'sending'
								this->wait_set.change(c->socket, opros::ready::write);
								break;
							}
						}

						// read flag should be cleared by the receive() call
						ASSERT(!c->socket.flags().get(opros::ready::read))
					}
					break;
				case connection::state::sending:
					{
						ASSERT(c->socket.flags().get(opros::ready::write))
						// TODO:
					}
					break;
			}
		}
	}
}
