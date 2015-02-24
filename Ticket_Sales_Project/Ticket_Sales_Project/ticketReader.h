#pragma once
#include <string>
#include <vector>

using namespace std;

class ticketReader{
private:
	vector<string> tickets;
public:
	void printTickets();
	int buy(string event, int quantity);
	int sell(string event, double price, int quantity, string user);
	int read();
	vector<string> getEvents();
};
