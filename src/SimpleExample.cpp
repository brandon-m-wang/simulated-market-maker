#include <chrono>
#include <iostream>

#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS
#include <asio.hpp>
#include <asio/ts/internet.hpp> // Network comms
#include <asio/ts/buffer.hpp>	// Movement of memory

int main()
{
	asio::error_code ec;

	// Create a context - platform specific interface ("asio instance")
	asio::io_context context;

	// Get address of the TCP endpoint defined by IP address and port
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

	// Create a socket
	asio::ip::tcp::socket socket(context);

	// Socket will connect to TCP endpoint
	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address: \n"
				  << ec.message() << std::endl;
	}

	if (socket.is_open()) 
	{
		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);

		size_t bytes = socket.available();
		std::cout << "Bytes available " << bytes << std::endl;

		if (bytes > 0) {
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

			for (auto c : vBuffer) {
				std::cout << c;
			}
		}

	}


	system("read -n 1 -s -p \"Press any key to continue...\"");
	return 0;
}