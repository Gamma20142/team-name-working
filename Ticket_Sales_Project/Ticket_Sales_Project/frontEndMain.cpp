#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "userReader.h"
#include "ticketReader.h"
#include "logWriter.h"

using namespace std;

int main(){
	//read in user file
	userReader users;
	users.read();

	//read in tickets file
	ticketReader tickets;
	tickets.read();

	logWriter writer;

	bool loggedIn = false;
	string currentUser;
	double userWalletValue;

	string command;
	string commandCode;
	string logString;

	while (true){
		if (loggedIn){
			cout << "What would you like to do? ";
			cin >> command;
			if (command == "logout"){
				
				//makes the string for the daily transaction file
				string padUser = currentUser;
				for (int i = padUser.length(); i < 15; i++){
					padUser += " ";
				}

				string padWallet = "";
				for (int i = to_string(userWalletValue).length(); i < 9; i++){
					padWallet += "0";
				}
				padWallet += to_string(userWalletValue);
				commandCode = "00";
				logString = commandCode + " " + padUser + " " + users.getType(currentUser) + " " + padWallet.substr(0,9);
				writer.writeIn(logString);

				//sets the system back to logged out state
				loggedIn = false;
				currentUser = "";
				userWalletValue = 0;
				
			}
			else if (command == "create"){
				cout << "Please enter the new username: ";
				string newUser;
				cin >> newUser;
				cout << "Please enter the type of user (AA, FS, BS, SS): ";
				string userType;
				cin >> userType;
				if (users.create(newUser, userType, 0) == 0)		
					cout << "User created.";
				else
					cout << "Failed to create user";
				
				//daily transaction
				string padUser = newUser;
				for (int i = padUser.length(); i < 15; i++){
					padUser += " ";
				}

				commandCode = "01";
				logString = commandCode + " " + padUser + " " + userType + " " + "000000.00";
				writer.writeIn(logString);
			}
			else if (command == "delete"){
				cout << "Please enter the user to delete: ";
				string deleteUser;
				cin >> deleteUser;
				double deleteWallet = users.auth(deleteUser);
				string deleteType = users.getType(deleteUser);

				if (users.deleteUser(deleteUser) == -1){
					cout << "Could not find user.";
				}
				else{	//delete the user's tickets for sale
					vector<string> events = tickets.getEvents();
					for (int i = deleteUser.length(); i < 15; i++){
						deleteUser += " ";
					}
					int index = 0;
					for (index = 0; index < events.size(); index++){
						if (deleteUser.compare(events[index].substr(26, 15)) == 0){	//searching for the username
							int ticketsRemaining = stoi(events[index].substr(42, 3));
							tickets.buy(events[index].substr(0, 25), ticketsRemaining);
						}
							
					}
					//tickets.printTickets();
					cout << "User was deleted";
					//daily transaction
					string padWallet = "";
					for (int i = to_string(userWalletValue).length(); i < 9; i++){
						padWallet += "0";
					}
					padWallet += to_string(deleteWallet);
					commandCode = "02";
					logString = commandCode + " " + deleteUser + " " + deleteType + " " + padWallet.substr(0, 9);
					writer.writeIn(logString);
				}
			}
			else if (command == "sell"){
				cout << "What is the name of the event you are selling tickets for? ";
				string eventName;
				cin >> eventName;
				cout << "How much will each ticket sell for? ";
				double ticketPrice;
				cin >> ticketPrice;
				cout << "How many tickets are for sale? ";
				int numTickets;
				cin >> numTickets;
				tickets.sell(eventName, ticketPrice, numTickets, currentUser);
				//tickets.printTickets();
				//daily transaction
				string padEvent = eventName;
				for (int i = padEvent.length(); i < 25; i++){
					padEvent += " ";
				}
				string padSeller = currentUser;
				for (int i = padSeller.length(); i < 15; i++){
					padSeller += " ";
				}
				string padTickets = "";
				for (int i = to_string(numTickets).length(); i < 3; i++){
					padTickets += "0";
				}
				padTickets += to_string(numTickets);
				string padPrice = "";
				for (int i = to_string(ticketPrice).length(); i < 6; i++){
					padPrice += "0";
				}
				padPrice += to_string(ticketPrice);
				commandCode = "03";
				logString = commandCode + " " + padEvent + " " + padSeller + " " + padTickets + " " + padPrice;
				writer.writeIn(logString);
			}
			else if (command == "buy"){
				cout << "What event would you like to buy tickets for? ";
				string eventName;
				cin >> eventName;
				cout << "How many tickets would you like to purchase? ";
				int numTickets;
				cin >> numTickets;

				//find the event, the cost of each ticket
				vector<string> events = tickets.getEvents();
				for (int i = eventName.length(); i < 25; i++){
					eventName += " ";
				}
				int index = 0;
				string seller;
				for (index = 0; index < events.size(); index++){
					if (eventName.compare(events[index].substr(0, 25)) == 0){	//searching for the event
						int ticketsRemaining = stoi(events[index].substr(42, 3));
						double price = stod(events[index].substr(46,6));
						if (numTickets > ticketsRemaining){
							cout << "Not enough tickets.";
						}
						else{
							double cost = price*numTickets;
							if ( cost > userWalletValue)
								cout << "Not enough credit to complete transaction. Please add more credit first.";
							else{
								userWalletValue -= cost;
								seller = events[index].substr(26, 15);
								users.transferCredit(currentUser,seller,cost,true);	//update the buyers wallet
								tickets.buy(eventName, numTickets);										
								//daily transaction
								string padTickets = "";
								for (int i = to_string(numTickets).length(); i < 3; i++){
									padTickets += "0";
								}
								padTickets += to_string(numTickets);
								string padPrice = "";
								for (int i = to_string(price).length(); i < 6; i++){
									padPrice += "0";
								}
								padPrice += to_string(price);
								commandCode = "04";
								logString = commandCode + " " + eventName + " " + seller + " " + padTickets + " " + padPrice;
								writer.writeIn(logString);
							}
						}
					}
				}

			}
			else if (command == "refund"){
				cout << "Which buyer is getting a refund? ";
				string buyer;
				cin >> buyer;
				cout << "Which seller is issuing a refund? ";
				string seller;
				cin >> seller;
				cout << "How much credit is being refunded? ";
				double amount;
				cin >> amount;
				users.transferCredit(buyer, seller, amount, false);
				//TODO : daily transaction
				string padBuyer = buyer;
				for (int i = padBuyer.length(); i < 15; i++){
					padBuyer += " ";
				}
				string padSeller = seller;
				for (int i = padSeller.length(); i < 15; i++){
					padSeller += " ";
				}
				string padAmount = "";
				for (int i = to_string(amount).length(); i < 9; i++){
					padAmount += "0";
				}
				padAmount += to_string(userWalletValue);
				commandCode = "05";
				logString = commandCode + " " + padBuyer + " " + padSeller + " " + padAmount.substr(0,9);
				writer.writeIn(logString);
			}
			else if (command == "addCredit"){
				cout << "How much credit would you like to add? ";
				double amount;
				cin >> amount;
				if (amount > 1000){
					cout << "Cannot add more than $1000.00 credit at one time.";
				}
				else if ((userWalletValue + amount) > 999999.99){
					cout << "You cannot have that much credit in the system.";
				}
				else{
					userWalletValue += amount;
					users.updateWallet(currentUser, userWalletValue);
				}
				//daily transaction
				string padUser = currentUser;
				for (int i = padUser.length(); i < 15; i++){
					padUser += " ";
				}
				string padWallet = "";
				for (int i = to_string(userWalletValue).length(); i < 9; i++){
					padWallet += "0";
				}
				padWallet += to_string(userWalletValue);
				commandCode = "06";
				logString = commandCode + " " + padUser + " " + users.getType(currentUser) + " " + padWallet.substr(0, 9);
				writer.writeIn(logString);
			}
		}
		else
		{
			cout << "Please enter your login name: ";
			string username;
			cin >> username;
			userWalletValue = users.auth(username);
			if (userWalletValue == -1){
				cout << "Invalid login name.\n";
			}
			else{
				loggedIn = true;
				currentUser = username;
			}
		}
	}

}