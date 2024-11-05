#pragma once
#include "allheader.h"

class Peer {
private:
	string port;
	streambuf buffer_; 
public:
	Peer(string port) {
		setPort(port);
	};
	 
	io_context io_ctx; // Provides input/output functionality.
	string name;
	tcp::socket socket{ io_ctx };
	tcp::resolver resolver{ io_ctx }; // Connects to existing connection.
	void setPort(string portNumber) { port = portNumber; } // Sets the port number for use in startConnection and connectToSender.

	inline void startConnection(); //Body of this function and the next three are in Peer.cpp.

	inline void connectToSender();
	
	inline void sendMessage();

	inline void readMessage();
};