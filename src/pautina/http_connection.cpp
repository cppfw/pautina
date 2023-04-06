#include "http_connection.hpp"

using namespace pautina;

http_connection::http_connection(setka::tcp_socket&& socket) :
    connection(std::move(socket))
{}
