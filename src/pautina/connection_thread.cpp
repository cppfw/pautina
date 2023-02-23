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

#include "connection_thread.hpp"

#include "http_server.hpp"

using namespace pautina;

connection_thread::connection_thread(http_server& owner, setka::tcp_socket&& socket) :
	nitki::loop_thread(1),
	owner(owner),
	connection(std::make_unique<pautina::connection>(std::move(socket)))
{
	this->wait_set.add(this->connection->socket, opros::ready::read);

	this->connection->socket.user_data = this->connection.get();
}

connection_thread::~connection_thread()
{
	this->wait_set.remove(this->connection->socket);
	LOG([](auto& o) {
		o << "connection_thread destroyed" << std::endl;
	})
}

std::optional<uint32_t> connection_thread::on_loop(utki::span<opros::event_info> triggered)
{
	for (auto& t : triggered) {
		if (!t.object->user_data) {
			// waitbale is a nitki::queue, skip it
			continue;
		}

		// waitable is a tcp_socket
		// std::cout << "socket ready: " << t.flags << std::endl;

		// check for error condition
		if (t.flags.get(opros::ready::error)) {
			LOG([](auto& o) {
				o << "socket error" << std::endl;
			})
			this->owner.reclaim_thread(*this);
			return {};
		}

		auto c = reinterpret_cast<pautina::connection*>(t.object->user_data);

		switch (c->state()) {
			case connection::state::receiving:
				{
					ASSERT(t.flags.get(opros::ready::read))

					constexpr const size_t receive_buffer_size = 0x1000; // 4kb
					std::array<uint8_t, receive_buffer_size> buf;

					try {
						bool first_read = true;
						while (size_t num_bytes_received = c->socket.receive(buf)) {
							first_read = false;
							ASSERT(num_bytes_received > 0)
							c->handle_received_data(utki::make_span(buf.data(), num_bytes_received));
							if (c->state() == connection::state::sending) {
								// state has changed to 'sending'
								this->wait_set.change(c->socket, opros::ready::write);
								break;
							}
						}
						if (first_read) {
							// zero bytes received right after the object has become ready to read,
							// this means that the connection was reset by peer
							LOG([](auto& o) {
								o << "connection reset by peer, quit thread!" << std::endl;
							})
							// destroy thread
							this->owner.reclaim_thread(*this);
							break;
						}
					} catch (std::exception& e) {
						LOG([&](auto& o) {
							o << "std::exception caught while receiving data from socket: " << e.what() << std::endl;
						})
					}
				}
				break;
			case connection::state::sending:
				{
					ASSERT(t.flags.get(opros::ready::write))

					ASSERT(c->num_bytes_sent < c->data_to_send.size())

					auto span = utki::make_span(
						c->data_to_send.data() + c->num_bytes_sent,
						c->data_to_send.size() - c->num_bytes_sent
					);

					size_t num_bytes_sent = c->socket.send(span);

					ASSERT(num_bytes_sent <= span.size())

					if (num_bytes_sent == span.size()) {
						c->data_to_send.clear();
						c->num_bytes_sent = 0;

						c->handle_all_data_sent();
						if (c->state() == connection::state::receiving) {
							// state has changed to 'receiving'
							this->wait_set.change(c->socket, opros::ready::read);
						}
					} else {
						c->num_bytes_sent += num_bytes_sent;
					}
				}
				break;
		}
	}

	return {};
}
