#pragma once

#include "server.hpp"

namespace pautina {

class http_server : public server
{
public:
	struct configuration : public server::configuration {};

	http_server(const configuration& config);

	std::unique_ptr<connection> spawn_connection(setka::tcp_socket&& socket) override;

	void add(std::vector<std::string> path, std::function<void()> handler);
};

} // namespace pautina
