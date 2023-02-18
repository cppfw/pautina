#include "http_server.hpp"

using namespace pautina;

http_server::http_server(configuration config) :
	wait_set(2),
	accept_socket(config.port)
{
	this->wait_set.add(this->accept_socket, opros::ready::read); // TODO: use gaurd?
	this->wait_set.add(this->queue, opros::ready::read);
}

http_server::~http_server()
{
	this->wait_set.remove(this->queue);
	this->wait_set.remove(this->accept_socket);
}

void http_server::quit()
{
	this->queue.push_back([this]() {
		this->quit_flag = true;
	});
}

void http_server::run()
{
	std::array<opros::event_info, 2> triggered;
	while (!this->quit_flag) {
		auto num_triggered = this->wait_set.wait(triggered);

		for(unsigned i = 0; i != num_triggered; ++i){
			auto& w = triggered[i];

			if (w.object == &this->queue) {
				while (auto proc = this->queue.pop_front()) {
					proc.operator()();
				}
				continue;
			}

			// if we get here, then socket is ready to read

			auto socket = this->accept_socket.accept();

			LOG([](auto&o){o << "connection accepted" << std::endl;})
			this->spawn_thread(std::move(socket));
		}
	}
}

void http_server::spawn_thread(setka::tcp_socket&& socket){
	auto& thread = this->threads.emplace_back(*this, std::move(socket));
	thread.owner_iter = std::prev(this->threads.end());
	thread.start();
}
