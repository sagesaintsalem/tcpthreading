
#include "Peer.h"

void Peer::startConnection(){ // This sets up the P2P connection before running readMessage and sendMessage simultaneously.
	int acceptorPort = stoi(port); //Converts port string to int because tcp::endpoint only accepts int values as a second parameter.
	tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), acceptorPort)); //Accepts connections using IPv4 TCP protocol.
	cout << "Hello! Please enter your handle: ";
	getline(cin, name);
	cout << "Hi " << name << "! Waiting for peer...\n";
	tcp::socket socket(io_ctx);
	acceptor.accept(socket); //Wait for connection and store in socket
	cout << "Peer arrived! Chat away!\n";
	while (true) {
		std::thread readThread(&Peer::readMessage, this);
		std::thread sendThread(&Peer::sendMessage, this);
		io_ctx.stop();
		readThread.join();
		sendThread.join();
	}


	

}


void Peer::connectToSender() { // This connects to existing P2P chat before running readMessage and sendMessage simultaneously.
	tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", port); //Resolves host and service names into a list of endpoint entries.
	connect(socket, endpoints); // Connects to socket.
	cout << "Please enter your name: ";
	getline(cin, name);
	cout << "Welcome " << name << "!\n";
	while (true) {
		std::thread readThread(&Peer::readMessage, this);
		std::thread sendThread(&Peer::sendMessage, this);
		io_ctx.stop();
		readThread.join();
		sendThread.join();
	}

}


void Peer::sendMessage() {
	cout << "\n" << Peer::name << ": ";
	string message;
	std::getline(cin, message);
	message = "\n" + name + ": " + message + "\n";
	auto message_pointer = std::make_shared<string>(message);
	async_write(socket, buffer(*message_pointer), [message_pointer](boost::system::error_code error, size_t) {
		if (error) {
			cout << "Error sending message" << endl;
		}
		});
}

void Peer::readMessage() {
	async_read_until(socket, buffer_, "\n", [this](boost::system::error_code error, size_t len) {
		std::istream stream(&buffer_);
		string message;
		getline(stream, message);
		buffer_.consume(len);
		readMessage();
		});
}