#include <iostream>

#include <pautina/http/server.hpp>

int main(int argc, const char** argv)
{
	uint16_t port = 8080;

	pautina::http::server server(
		{port},
		{}
	);

	std::cout << "app: server is listening on port " << port << std::endl;

	server.run();

	std::cout << "app: exit" << std::endl;

	return 0;
}
