#include "connection.hpp"

using namespace pautina;

connection::connection(setka::tcp_socket&& socket) :
	socket(std::move(socket))
{}
