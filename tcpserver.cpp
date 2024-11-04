#include "allheader.h"
#include "listener.cpp"
#include "sender.cpp"
#include "Peer.h"




int main(int argc, char** argv) {
	if (argc == 2) {
		string appName = argv[1];
		if (appName == "server") {
			server();
			return 0;
		}
		else if (appName == "client") {
			client();
			return 0;
		}
	}
	
	cout << "Byeeee" << std::endl;
	
}

