#pragma once
#include "allheader.h"

class Sender {
public:
	boost::asio::io_context io_ctx;
	tcp::acceptor acceptor;
	tcp::socket socket;
		
};

int main() {
	boost::asio::io_context io_ctx;
	tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), 1337));
	tcp::socket socket(io_ctx);
	return 0;
}