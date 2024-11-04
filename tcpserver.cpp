#include "Peer.cpp"




//int main(int argc, char** argv) {
//	if (argc == 2) {
//		string appName = argv[1];
//		if (appName == "server") {
//			server();
//			return 0;
//		}
//		else if (appName == "client") {
//			client();
//			return 0;
//		}
//	}
//	
//	cout << "Byeeee" << std::endl;
//	
//}

int main() {
	cout << "Welcome to Chataway! Please select from the following: " << endl;
	cout << "1. Host chat \n";
	cout << "2. Join chat \n";
	int choice;
	cin >> choice;
	cin.ignore();
	if (choice == 1) {
		Peer server("1337");
		server.startConnection();
	}
	else if (choice == 2) {
		Peer client("1337");
		client.connectToSender();
	}
	else {
		cout << "Key not found. Ending program. \n";
	}

}
