#pragma once
#include "allheader.h"

class Peer {
private:
	string port;
	tcp::resolver::results_type endpoints;
public:
	Peer(string port, string endpointhost) {
		setPort(port);
		setEndpoints(endpointhost, port);
	};
	io_context io_ctx;
	string name;
	tcp::socket socket{ io_ctx };
	tcp::resolver resolver{ io_ctx };
	void setPort(string portNumber) { port = portNumber; }


	void setEndpoints(string host, string port) {
		endpoints = resolver.resolve(host, port);
	}


	void startConnection() {
		io_ctx;
		setPort(port);
		int acceptorPort = stoi(port);
		tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), acceptorPort));
		cout << "Hello! Please enter your handle: ";
		string name;
		getline(cin, name);
		cout << "Hi " << name << "! Waiting for peer...\n";
		tcp::socket socket(io_ctx);
		acceptor.accept(socket);
		cout << "Peer arrived! Chat away!\n";
	}

	void connectToSender() {
		io_ctx;
		resolver;
		socket;
		boost::asio::connect(socket, endpoints);
		cout << "Please enter your name: ";
		getline(cin, name);
		cout << "Welcome " << name << "! \n";
	}
	
	/*void sendMessage(string message) {
		boost::system::error_code error;
		cout << "\n" << name << ": ";
		string clientMessage;
		std::getline(cin, clientMessage);
		clientMessage = "\n" + name + ": " + clientMessage;
		boost::asio::write(socket, boost::asio::buffer(clientMessage), error);
	}*/

};