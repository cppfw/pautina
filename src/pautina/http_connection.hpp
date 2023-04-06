#pragma once

#include "connection.hpp"

namespace pautina{

class http_connection : public connection{
public:
    http_connection(setka::tcp_socket&& socket);

};

}
