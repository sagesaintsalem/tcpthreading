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
	 
	io_context io_ctx; 
	string name;
	tcp::socket socket{ io_ctx };
	tcp::resolver resolver{ io_ctx };
	void setPort(string portNumber) { port = portNumber; }

	inline void startConnection();

	inline void connectToSender();
	
	inline void sendMessage();

	inline void readMessage();
};