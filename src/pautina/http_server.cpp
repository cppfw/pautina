#include "http_server.hpp"

using namespace pautina;

http_server::http_server() :
	wait_set(2)
{
	this->wait_set.add(this->accept_socket, opros::ready::read);
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
	while (!this->quit_flag) {
		this->wait_set.wait();

		if (this->queue.flags().get(opros::ready::read)) {
			while (auto proc = this->queue.pop_front()) {
				proc.operator()();
			}
		}

		if (this->accept_socket.flags().get(opros::ready::read)) {
			auto socket = this->accept_socket.accept();

			std::cout << "connection accepted" << std::endl;
			// TODO: accept connection
		}
	}
}
