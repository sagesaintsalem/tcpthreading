
#include "Peer.h"

void Peer::startConnection(){
	int acceptorPort = stoi(port);
	tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), acceptorPort)); //Accepts connections
	cout << "Hello! Please enter your handle: ";
	getline(cin, name);
	cout << "Hi " << name << "! Waiting for peer...\n";
	tcp::socket socket(io_ctx);
	acceptor.accept(socket); //Wait for connection and store in socket
	cout << "Peer arrived! Chat away!\n";
	std::thread readThread(&Peer::readMessage, this);
	std::thread sendThread(&Peer::sendMessage, this);


	io_ctx.stop();
	readThread.join();
	sendThread.join();

}


void Peer::connectToSender() {
	tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", port);
	connect(socket, endpoints);
	cout << "Please enter your name: ";
	getline(cin, name);
	cout << "Welcome " << name << "!\n";
	std::thread readThread(&Peer::readMessage, this);
	std::thread sendThread(&Peer::sendMessage, this);
	io_ctx.stop();
	readThread.join();
	sendThread.join();

}


void Peer::sendMessage() {
	cout << "\n" << Peer::name << ": ";
	string message;
	std::getline(cin, message);
	message = "\n" + name + ": " + message + "\n";
	auto message_pointer = std::make_shared<string>(message);
	/*boost::asio::write(socket, boost::asio::buffer(message), error);*/
	async_write(socket, buffer(*message_pointer), [message_pointer](boost::system::error_code error, size_t) {
		if (error) {
			cout << "Error sending message" << endl;
		}
		});
}

void Peer::readMessage() {
	//std::array<char, 240> buf{};
	/*size_t len = socket.read_some(boost::asio::buffer(buf), error);
	cout.write(buf.data(), len);
	cout << "\n";*/
	async_read_until(socket, buffer_, "\n", [this](boost::system::error_code error, size_t len) {
		std::istream stream(&buffer_);
		string message;
		getline(stream, message);
		buffer_.consume(len);
		readMessage();
		});
}