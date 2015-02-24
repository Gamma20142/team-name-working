#include "ticketReader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

//Error codes for buy:
// 0 is success
//-1 is show not found
//-2 is not enough tickets remaining to complete transaction
int ticketReader::buy(string event, int quantity){
	for (int i = event.length(); i < 25; i++){
		event += " ";
	}
	int index = 0;
	for (index = 0; index < tickets.size(); index++){
		if (event.compare(tickets[index].substr(0, 25)) == 0)
			break;
	}
	if (index == tickets.size()){
		return -1;
	}

	int numAvailable = stoi(tickets[index].substr(42, 3));
	if (numAvailable < quantity){
		return -2;
	}

	int numRemaining = numAvailable - quantity;

	string newEntry = tickets[index].substr(0, 42);
	for (int i = to_string(numRemaining).length(); i < 3; i++){
		newEntry += "0";
	}
	newEntry += to_string(numRemaining);
	newEntry += tickets[index].substr(45, 7);

	tickets[index] = newEntry;
	return 0;
}

void ticketReader::printTickets(){
	for (int i = 0; i < tickets.size(); i++){
		cout << tickets[i];
		cout << endl;
	}
	cout << "\n";
}

//returns error codes for invalid entries
// 0 for success
// -1 for eventname too long
// -2 for invalid account type
// -3 for invalid price
// -4 for invalid quantity
int ticketReader::sell(string event, double price, int quantity, string user){
	if (event.length() > 25)
		return -1;
	else{
		for (int i = event.length(); i < 25; i++){
			event += " ";
		}
	}
	for (int i = user.length(); i < 15; i++){
			user += " ";
	}
		
	string priceString = "";
	if (price >= 0 && price < 1000){
		int dollar = floor(price);
		double cent = price - dollar;
		string cents = to_string(cent).substr(2, 2);
		string converted = to_string(dollar) + "." + cents;

		for (int i = converted.length(); i < 6; i++){
			priceString += "0";
		}
		priceString += converted;
	}
	else{
		return -3;
	}

	string quantityString = "";
	if (quantity > 0 && quantity <= 100){
		string converted = to_string(quantity);

		for (int i = converted.length(); i < 3; i++){
			quantityString += "0";
		}
		quantityString += converted;
	}
	else{
		return -4;
	}
	tickets.push_back(event + " " + user + " " + quantityString + " " + priceString);
	return 0;
}

int ticketReader::read(){
	ifstream reader("ticketInfo.txt");
	string input;
	if (reader.is_open()){
		getline(reader, input);
		while (input.compare("END") != 0){
			tickets.push_back(input);
			getline(reader, input);
		}
		return 0;
	}
	else{
		cout << "Unable to open file";
		return -1;
	}
}

vector<string> ticketReader::getEvents(){
	return tickets;
}

/*int main(){
	ticketReader ticket;
	ticket.read();
	ticket.printTickets();
	ticket.buy("Blink 182", 3);
	ticket.printTickets();
	ticket.sell("Alvin and the Chipmunks", 11.75, 70, "jimmy");
	ticket.printTickets();
	return 0;
}*/