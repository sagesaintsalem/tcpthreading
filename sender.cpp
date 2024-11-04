#include "allheader.h"
#include "Peer.h"


static void server()
{
	try {
		//Makes context object that Boost uses to keep info on your input/output for system, provides core I/O functionality. 
		//boost::asio::io_context io_ctx;

		//Accepts connections
		//tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), 1337));


		//Wait for connection and store in socket
		while (true) {
			/*cout << "Hello! Please enter your handle: ";
			string name;
			getline(cin, name);
			cout << "Hi " << name << "! Waiting for peer...\n";
			tcp::socket socket(io_ctx);
			acceptor.accept(socket);
			cout << "Peer arrived! Chat away!\n";*/
			Peer sender("1337", "127.0.0.1");
			sender.startConnection();

			//While there is a connection
			while (true) {
				cout << "\n" << sender.name << ": ";

				boost::system::error_code error;

				string hello;
				std::getline(cin, hello);
				hello = "\n" + sender.name + ": " + hello;
				boost::asio::write(sender.socket, boost::asio::buffer(hello), error);





				//While in receiving mode
				while (true) {
					std::array<char, 240> buf{};
					size_t len = sender.socket.read_some(boost::asio::buffer(buf), error);
					std::cout.write(buf.data(), len);
					cout << "\n";
					//Back to sending mode
					break;
				}
			}
		
		}
	}
catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}


}


