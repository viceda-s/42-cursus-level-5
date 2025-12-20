#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sys/poll.h>
#include <vector>
#include <map>

#define LISTEN_CONN 128
#define BUFFER_SIZE 8192

class Client;
class Config;
class Request;
class Response;

/**
 * @class Server
 * @brief Main HTTP server class handling connections and requests
 *
 * This class implements a non-blocking HTTP/1.1 server using poll()
 * for efficient I/O multiplexing. It manages multiple client connections,
 * processes HTTP requests, and serves responses according to the
 * configuration file.
 */
class Server {
private:
	Config* _config;
	int _server_fd;
	std::vector<struct pollfd> _poll_fds;
	std::map<int, Client*> _clients; // fd -> Client*
	std::map<int, std::string> _output_buffers; // Output buffers per client fd

public:
	/**
	 * @brief Construct a new Server object
	 * @param config_file Path to the configuration file
	 * @throws std::runtime_error if configuration parsing fails
	 */
	Server(const std::string& config_file);

	/**
	 * @brief Destroy the Server object and cleanup resources
	 */
	~Server();

	/**
	 * @brief Main event loop - runs until interrupted
	 *
	 * Handles all I/O events using poll(), processes client requests,
	 * and manages client connections.
	 */
	void run();

	/**
	 * @brief Check if shutdown signal was received
	 * @return true if server should shutdown
	 */
	static bool shouldShutdown();

private:
	// Socket setup
	void _setupSocket();
	void _acceptNewClient();
	void _handleClientData(int client_fd);
	void _setNonBlocking(int fd);

	// Request processing
	void _processClientRequest(int client_fd);
	void _handleRequest(int client_fd, Request& request);
	Response _buildResponse(const Request& request);

	// CGI handling
	void _handleCgiRequest(int client_fd, const Request& request);

	// Output handling
	void _sendToClient(int client_fd, const std::string& data);
	void _flushClientBuffer(int client_fd);

	// Client management
	void _removeClient(int client_fd);
	void _cleanupTimedOutClients();

	// Helper methods
	std::string _readFile(const std::string& path);
	std::string _getContentType(const std::string& path);
	bool _fileExists(const std::string& path);
};

#endif // SERVER_HPP

