#pragma once
#include "allheader.h"

class Sender {
public:
	boost::asio::io_context io_ctx;
	tcp::acceptor acceptor;
	tcp::socket socket;
		
};

