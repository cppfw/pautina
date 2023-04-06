#include "http_server.hpp"

#include "http_connection.hpp"

using namespace pautina;

http_server::http_server(const configuration& config) :
	server(config)
{}

std::unique_ptr<connection> http_server::spawn_connection(setka::tcp_socket&& socket)
{
	return std::make_unique<http_connection>(std::move(socket));
}
