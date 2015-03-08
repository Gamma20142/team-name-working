#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "userReader.h"
#include "ticketReader.h"
#include "logWriter.h"
#include "helper.h"

using namespace std;

/*	TICKET FILE SYSTEM
/	written by	Ryan Morton - 100485285
/				Thomas Frantz - 100484424
/				Alvin Lee - 100484342
/
/	This is the main user interface for the ticket system. The user can login and logout, and 
/	issue the following commands by typing them when prompted: create, delete, buy, sell, refund, addCredit
/
/	create - creates a user
/	delete - deletes a user
/	buy - buys tickets to an existing event
/	sell - place new tickets up for sale in the system
/	refund - issues a refund from a seller to a buyer
/	addCredit - adds credit to the currently logged in user (for now)
*/

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
			// LOGOUT
			if (command == "logout"){
				//makes the string for the daily transaction file
				commandCode = "00";
				logString = commandCode + " " + helper::pad(currentUser, 15, " ",false) + " " + users.getType(currentUser) + " " + helper::pad(helper::dtom(userWalletValue),9,"0",true);
				writer.writeIn(logString);

				//sets the system back to logged out state
				loggedIn = false;
				currentUser = "";
				userWalletValue = 0;
			}
			// CREATE
			else if (command == "create"){
				cout << "Please enter the new username: ";
				string newUser;
				cin >> newUser;
				cout << "Please enter the type of user (AA, FS, BS, SS): ";
				string userType;
				cin >> userType;
				if (users.create(newUser, userType, 0) == 0)		
					cout << "User created.\n";
				else
					cout << "Failed to create user\n";
				
				//daily transaction
				commandCode = "01";
				logString = commandCode + " " + helper::pad(newUser,15," ",false) + " " + userType + " " + helper::pad(helper::dtom(0.0),9,"0",true);
				writer.writeIn(logString);
			}
			// DELETE
			else if (command == "delete"){
				cout << "Please enter the user to delete: ";
				string deleteUser;
				cin >> deleteUser;
				double deleteWallet = users.auth(deleteUser);
				string deleteType = users.getType(deleteUser);

				if (users.deleteUser(deleteUser) == -1){
					cout << "Could not find user.\n";
				}
				else{	//delete the user's tickets for sale
					vector<string> events = tickets.getEvents();
	
					int index = 0;
					for (index = 0; index < events.size(); index++){
						if (helper::pad(deleteUser,15," ",false).compare(events[index].substr(26, 15)) == 0){	//searching for the username
							int ticketsRemaining = stoi(events[index].substr(42, 3));
							tickets.buy(events[index].substr(0, 25), ticketsRemaining);
						}
					}
					cout << "User was deleted\n";
					//daily transaction
					commandCode = "02";
					logString = commandCode + " " + helper::pad(deleteUser, 15, " ", false) + " " + deleteType + " " + helper::pad(helper::dtom(deleteWallet), 9, "0", true);
					writer.writeIn(logString);
				}
			}
			// SELL
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
				cout << "Event created.\n";
				//daily transaction
				commandCode = "03";
				logString = commandCode + " " + helper::pad(eventName,25," ",false) + " " + helper::pad(currentUser,15," ",false) + " " 
					+ helper::pad(to_string(numTickets),3,"0",true) + " " + helper::pad(helper::dtom(ticketPrice),6,"0",true);
				writer.writeIn(logString);
			}
			// BUY
			else if (command == "buy"){
				cout << "What event would you like to buy tickets for? ";
				string eventName;
				cin >> eventName;
				cout << "How many tickets would you like to purchase? ";
				int numTickets;
				cin >> numTickets;

				//find the event, the cost of each ticket
				vector<string> events = tickets.getEvents();

				int index = 0;
				string seller;
				for (index = 0; index < events.size(); index++){
					if (helper::pad(eventName,25," ",false).compare(events[index].substr(0, 25)) == 0){	//searching for the event
						int ticketsRemaining = stoi(events[index].substr(42, 3));
						double price = stod(events[index].substr(46,6));
						if (numTickets > ticketsRemaining){
							cout << "Not enough tickets.";
						}
						else{
							double cost = price*numTickets;
							if ( cost > userWalletValue)
								cout << "Not enough credit to complete transaction. Please add more credit first.\n";
							else{
								userWalletValue -= cost;
								seller = events[index].substr(26, 15);
								users.transferCredit(currentUser,seller,cost,true);	//update the buyers wallet
								tickets.buy(eventName, numTickets);		
								cout << "Tickets purchased.\n";
								//daily transaction
								commandCode = "04";
								logString = commandCode + " " + helper::pad(eventName,25," ",false) + " " + seller + " " 
									+ helper::pad(to_string(numTickets),3,"0",true) + " " + helper::pad(helper::dtom(price),6,"0",true);
								writer.writeIn(logString);
							}
						}
					}
				}

			}
			// REFUND
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
				cout << "Refund complete.";
				// daily transaction
				commandCode = "05";
				logString = commandCode + " " + helper::pad(buyer,15," ",false) + " " + helper::pad(seller,15," ",false) + " " + helper::pad(helper::dtom(amount),9,"0",true);
				writer.writeIn(logString);
			}
			else if (command == "addCredit"){
				cout << "How much credit would you like to add? ";
				double amount;
				cin >> amount;
				if (amount > 1000){
					cout << "Cannot add more than $1000.00 credit at one time.\n";
				}
				else if ((userWalletValue + amount) > 999999.99){
					cout << "You cannot have that much credit in the system.\n";
				}
				else{
					userWalletValue += amount;
					users.updateWallet(currentUser, userWalletValue);
					cout << "Credit added.\n";
				}
				//daily transaction
				commandCode = "06";
				logString = commandCode + " " + helper::pad(currentUser,15," ",false) + " " + users.getType(currentUser) + " " + helper::pad(helper::dtom(userWalletValue),9,"0",true);
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