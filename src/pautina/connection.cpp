#include "connection.hpp"

using namespace pautina;

connection::connection(setka::tcp_socket&& socket) :
	socket(std::move(socket))
{}

void connection::handle_all_data_sent()
{
	this->current_state = state::receiving;
}

void connection::handle_received_data(utki::span<const uint8_t> data){
	LOG([](auto&o){o << "connection::handle_received_data(): " << std::endl;})
	// TODO:
}
