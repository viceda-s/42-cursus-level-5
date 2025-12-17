
#include "Server.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

int main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << "Usage ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int port = atoi(av[1]);
	std::string password = av[2];

	try {
		Server server(port, password);
		server.run();
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}