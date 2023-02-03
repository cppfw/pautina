#include <iostream>

#include <pautina/http_server.hpp>

int main(int argc, const char** argv)
{
	pautina::http_server server({8080});

	server.run();

	return 0;
}
