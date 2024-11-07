#include "Peer.h"
#include "allheader.h"
#include <mutex>

std::mutex cout_mutex;  // Mutex for thread-safe output

// Constructor initializes port and socket
Peer::Peer(string port) : port(port), ssl_sock(io_ctx, ssl_ctx) {}
    


// This sets up the P2P connection before running readMessage and sendMessage simultaneously
void Peer::startConnection() {  // Host function
    ssl_ctx.set_default_verify_paths();

    // This loads up the relevant SSL files
    try {
        ssl_ctx.use_certificate_chain_file("tcpdomain.pem");  // Server certificate, omitted by .gitignore for security reasons.
        ssl_ctx.use_private_key_file("tcpkey.pem", ssl::context::pem);  // Server private key omitted by .gitignore for security reasons.
        ssl_ctx.set_verify_mode(ssl::verify_none);  // No verification for simplicity
    }
    catch (std::exception& sslerror) {
        std::cerr << "Error loading certificates: " << sslerror.what() << endl;
    }

    tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), std::stoi(port)));
    cout << "Hello! Enter your name: ";
    getline(cin, name);
    cout << "Hi " << name << "! Waiting for peer on port " << port << "...\n";


    // Accepts connection from client, returns error message if unsuccessful

   try {
        acceptor.accept(ssl_sock.lowest_layer());
        cout << "Accepted! \n";

        
    }
    catch (const std::exception& error) {
        std::cerr << "Accept failed: " << error.what() << endl;

    }


    //ssl_sock.handshake(ssl::stream_base::server); //This line causes a runtime error

    cout << "Attempting handshake...\n";
    // This starts the handshake from the server side
    ssl_sock.async_handshake(ssl::stream_base::server, [this](const boost::system::error_code& error) {
        if (error) {
            std::cerr << "Handshake failed: " << error.message() << endl;
            //If handshake doesn't work, error message shows on screen
        }
        else {
            cout << "Peer connected! Start chatting...\n";

        }

        });
    
    
    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages
        
    readThread.join();
    sendThread.join();
    }

    


// This connects to an existing P2P chat as a client
void Peer::connectToSender(const string& host_ip) {  // Client function
    ssl_ctx.set_default_verify_paths();

    // This loads up the relevant SSL files
    try {
        ssl_ctx.use_certificate_chain_file("tcpdomain.pem");  // Certificate, omitted by .gitignore for security reasons.
        ssl_ctx.use_private_key_file("tcpkey.pem", ssl::context::pem);  // Private key, omitted by .gitignore for security reasons.
        ssl_ctx.set_verify_mode(ssl::verify_none);  // No verification for simplicity
    }
    catch (std::exception& sslerror) {
        std::cerr << "Error loading certificates: " << sslerror.what() << endl;
    }

    cout << "Enter your name: ";
    getline(std::cin, name);

    cout << "Resolving...\n";
    auto endpoints = resolver.resolve(host_ip, port);

    cout << "Connecting...\n";

    // Connects to the server, returns an error message if unsuccessful
    try {
        connect(ssl_sock.lowest_layer(), endpoints);
        cout << "Connected! \n";

    }
    catch (const std::exception& error) {
        std::cerr << "Connect failed: " << error.what() << endl;

    }

    //ssl_sock.handshake(ssl::stream_base::client); //This line causes a runtime error.

    cout << "Attempting handshake...\n";
    // This starts the handshake from the client side.
    ssl_sock.async_handshake(ssl::stream_base::client, [this](const boost::system::error_code& error) {
        if (error) {
            std::cerr << "Handshake failed: " << error.message() << endl;
            //If handshake doesn't work, error message shows on screen

        }
        else {
            cout << "Peer connected! Start chatting...\n";

        }

        });

    
    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages

    readThread.join();
    sendThread.join();

  }

// Handles sending messages with user's name as a prefix
void Peer::sendMessage() {
    while (true) {
        string message;
        getline(std::cin, message);

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            message = name + ": " + message + "\n";  // Format message with username
            boost::asio::write(ssl_sock, boost::asio::buffer(message));
        }
    }
}

// Handles receiving messages from the peer
void Peer::readMessage() {
    while (true) {
        streambuf buffer;
        boost::asio::read_until(ssl_sock, buffer, "\n");
        std::istream stream(&buffer);
        string message;
        getline(stream, message);

        std::lock_guard<std::mutex> lock(cout_mutex);
        cout << message << endl; // Print received message
    }
}
