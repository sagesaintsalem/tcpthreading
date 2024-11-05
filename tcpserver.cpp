#include "Peer.cpp"




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
