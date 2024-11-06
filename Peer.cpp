#include "Peer.h"
#include "allheader.h"
#include <mutex>

std::mutex cout_mutex;  // Mutex for thread-safe output

// Constructor initializes port and socket
Peer::Peer(std::string port) : port(port), socket(io_ctx) {}

// This sets up the P2P connection before running readMessage and sendMessage simultaneously
void Peer::startConnection() {  // Host function
    tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), std::stoi(port)));
    std::cout << "Hello! Enter your name: ";
    std::getline(std::cin, name);
    std::cout << "Hi " << name << "! Waiting for peer on port " << port << "...\n";

    acceptor.accept(socket); // Accepts connection from client
    std::cout << "Peer connected! Start chatting...\n";

    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages

    readThread.join();
    sendThread.join();
}

// This connects to an existing P2P chat as a client
void Peer::connectToSender(const string& host_ip) {  // Client function
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);

    auto endpoints = resolver.resolve(host_ip, port);
    boost::asio::connect(socket, endpoints); // Connect to the host
    std::cout << "Connected to host at " << host_ip << " on port " << port << ". Start chatting...\n";

    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages

    readThread.join();
    sendThread.join();
}

// Handles sending messages with user's name as a prefix
void Peer::sendMessage() {
    while (true) {
        std::string message;
        std::getline(std::cin, message);

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            message = name + ": " + message + "\n";  // Format message with username
            boost::asio::write(socket, boost::asio::buffer(message));
        }
    }
}

// Handles receiving messages from the peer
void Peer::readMessage() {
    while (true) {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");
        std::istream stream(&buffer);
        std::string message;
        std::getline(stream, message);

        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << message << std::endl; // Print received message
    }
}
