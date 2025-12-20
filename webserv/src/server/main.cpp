#include "Server.hpp"
#include <iostream>
#include <cstdlib>
#include <csignal>

// Global flag for graceful shutdown
volatile sig_atomic_t g_shutdown = 0;

void signalHandler(int signum) {
	if (signum == SIGINT || signum == SIGTERM) {
		std::cout << "\nShutdown signal received. Exiting gracefully..." << std::endl;
		g_shutdown = 1;
	}
}

int main(int argc, char** argv) {
	// Set up signal handlers for graceful shutdown
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGPIPE, SIG_IGN);  // Ignore broken pipe signals

	std::string config_file = "config/default.conf";

	if (argc > 1) {
		config_file = argv[1];
	}

	std::cout << "Starting webserv..." << std::endl;
	std::cout << "Configuration file: " << config_file << std::endl;

	try {
		Server server(config_file);
		server.run();
		std::cout << "Server stopped cleanly." << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

