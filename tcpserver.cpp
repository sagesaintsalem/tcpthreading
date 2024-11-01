#include "allheader.h"
#include "listener.cpp"
#include "sender.cpp"
#include <map>




int main(int argc, char** argv) {
	if (argc == 2) {
		std::string appName = argv[1];
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

