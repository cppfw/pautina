#include <iostream>

#include <pautina/http_server.hpp>

int main(int argc, const char** argv)
{
	pautina::http_server server({8080});

	std::cout << "app: server is running" << std::endl;

	server.run();

	std::cout << "app: exit" << std::endl;

	return 0;
}
