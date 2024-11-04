#include "Peer.h"




static void client() {
	try {
		//boost::asio::io_context iocxt;

		//////Resolver is class template, it provides the ability to resolve a query to a list of endpoints.
		//tcp::resolver resolver{ iocxt };

		//tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "1337");

		//tcp::socket socket{ iocxt };

		//

		////Connect and set to receiving mode
		//boost::asio::connect(socket, endpoints);
		//cout << "Hello! Please enter your handle: ";
		//string name;
		//getline(cin, name);
		////string* nameptr = &name;
		//cout << "Hi " << name << "! Waiting for peer...\n";
		//cout << "Connected! Chat away!\n";

		Peer peer("1337", "127.0.0.1");
		peer.connectToSender();


		//While in receiving mode
		while (true) {
			std::array<char, 240> buf{};
			boost::system::error_code error;
			size_t len = peer.socket.read_some(boost::asio::buffer(buf), error);

			cout.write(buf.data(), len);
			cout << "\n";

			//Sending mode
			while (true) {
				cout << "\n" << peer.name << ": ";
				string clientMessage;
				std::getline(cin, clientMessage);
				clientMessage = "\n" + peer.name + ": " + clientMessage;

				boost::asio::write(peer.socket, boost::asio::buffer(clientMessage), error);
				//cout << "\n";
				//Back to receiving mode
				break;
			}


		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;


	}
}

