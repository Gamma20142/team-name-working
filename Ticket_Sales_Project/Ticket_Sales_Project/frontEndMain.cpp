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

const int MAX_TICKETS_BUY = 4;
const int MAX_TICKETS_SELL = 100;
const int MAX_TICKET_PRICE = 1000;
const double MAX_ACCOUNT_VALUE = 999999.99;
const int user_length = 15;
const int event_length = 25;

int main(){
	//read in user file
	userReader users;
	users.read();

	//read in tickets file
	ticketReader tickets;
	tickets.read();

	//create the log writer
	logWriter writer;

	//current user information
	bool loggedIn = false;
	string currentUser;
	double userWalletValue;
	string userType;


	string command;
	string commandCode;
	string logString;


	//output stream for testing
	ofstream output;
	output.open("output.txt", ofstream::out | ofstream::trunc);



	while (true){
		cin.clear();
		if (loggedIn){
			cout << "What would you like to do? ";
			output << "What would you like to do? \n";
			getline(cin,command);
			// LOGOUT
			if (command == "logout"){
				//makes the string for the daily transaction file
				commandCode = "00";
				logString = commandCode + " " + helper::pad(currentUser, user_length, " ",false) + " " + users.getType(currentUser) 
					+ " " + helper::pad(helper::dtom(userWalletValue),9,"0",true);
				writer.writeIn(logString);

				//sets the system back to logged out state
				loggedIn = false;
				currentUser = "";
				userWalletValue = 0;
			}
			//QUIT - LOGOUT THEN QUIT INSTEAD OF GOING BACK TO MAIN MENU
			else if (command == "quit"){
				//makes the string for the daily transaction file
				commandCode = "00";
				logString = commandCode + " " + helper::pad(currentUser, user_length, " ", false) + " " + users.getType(currentUser) 
					+ " " + helper::pad(helper::dtom(userWalletValue), 9, "0", true);
				writer.writeIn(logString);
				return 0;
			}
			// CREATE
			else if (command == "create"){
				
				if (userType == "AA"){


					bool properInput = false;
					string newUser;
					string newType;

					//get the username
					do{
						cout << "Please enter the new username: ";
						output << "Please enter the new username: \n";
						getline(cin, newUser);
						if (newUser.length() > user_length){
							cout << "Username is too long.\n";
							output << "Username is too long.\n";
						}
						else if (users.auth(newUser) != -1){
							cout << "Username already exits.\n";
							output << "Username already exits.\n";
						}
						else
							properInput = true;
					} while (properInput == false);

					properInput = false;

					//get the type
					do{
						cout << "Please enter the type of user (AA, FS, BS, SS): ";
						output << "Please enter the type of user (AA, FS, BS, SS): \n";
						getline(cin, newType);
						if (newType != "AA" && newType != "FS" && newType != "BS" && newType != "SS"){
							cout << "Invalid user type.\n";
							output << "Invalid user type.\n";
						}
						else
							properInput = true;
					} while (properInput == false);

					//create the user
					if (users.create(newUser, newType, 0) == 0){

						//daily transaction
						commandCode = "01";
						logString = commandCode + " " + helper::pad(newUser, user_length, " ", false) + " " + newType 
							+ " " + helper::pad(helper::dtom(0.0), 9, "0", true);
						writer.writeIn(logString);

						cout << "User created.\n";
						output << "User created.\n";
					}
					else
						cout << "Failed to create user\n";
						output << "Failed to create user\n";
				}
				else{
					cout << "Please log in as an administrator to create an account.\n";
					output << "Please log in as an administrator to create an account.\n";
				}
			}
			// DELETE
			else if (command == "delete"){
				if (userType == "AA"){
					cout << "Please enter the user to delete: ";
					output << "Please enter the user to delete: \n";
					string deleteUser;
					getline(cin, deleteUser);
					double deleteWallet = users.auth(deleteUser);
					string deleteType = users.getType(deleteUser);

					if (deleteUser == currentUser){
						cout << "Cannot delete currently logged in user.\n";
						output << "Cannot delete currently logged in user.\n";
					}
					else if (users.deleteUser(deleteUser) == -1){
						cout << "Could not find user.\n";
						output << "Could not find user.\n";
					}
					else{	//delete the user's tickets for sale
						vector<string> events = tickets.getEvents();

						int index = 0;
						for (index = 0; index < events.size(); index++){
							if (helper::pad(deleteUser, user_length, " ", false).compare(events[index].substr(26, user_length)) == 0){	//searching for the username
								int ticketsRemaining = stoi(events[index].substr(42, 3));
								tickets.buy(events[index].substr(0, event_length), ticketsRemaining);
							}
						}
						cout << "User was deleted\n";
						output << "User was deleted\n";
						//daily transaction
						commandCode = "02";
						logString = commandCode + " " + helper::pad(deleteUser, user_length, " ", false) + " " + deleteType + " " 
							+ helper::pad(helper::dtom(deleteWallet), 9, "0", true);
						writer.writeIn(logString);
					}
				}
				else{
					cout << "Please log in as an administrator to delete an account.\n";
					output << "Please log in as an administrator to delete an account.\n";
				}
			}
			// SELL
			else if (command == "sell"){
				if (userType != "BS"){
					bool properInput = false;
					string eventName;
					string ticketPriceString;
					string numTicketsString;
					double ticketPrice;
					int numTickets;					
					
					//get the event name
					do{
						cout << "What is the name of the event you are selling tickets for? ";
						output << "What is the name of the event you are selling tickets for? \n";
						getline(cin, eventName);
						if (eventName.length() > event_length){
							cout << "The event name is too long.\n";
							output << "The event name is too long.\n";
						}
						else
							properInput = true;
					} while (properInput == false);
					
					properInput = false;

					//get the price
					do{
						try{
							cout << "How much will each ticket sell for? ";
							output << "How much will each ticket sell for? \n";
							getline(cin, ticketPriceString);

							ticketPrice = stod(ticketPriceString);
							if (ticketPrice >= MAX_TICKET_PRICE){
								cout << "Ticket price is too high.\n";
								output << "Ticket price is too high.\n";
							}
							else
								properInput = true;
							}
							catch (invalid_argument e){
								cout << "That is not a valid number.\n";
								output << "That is not a valid number.\n";
							}
						} while (properInput == false);

					properInput = false;

					//get the number of tickets
					do{
						try{
							cout << "How many tickets are for sale? ";
							output << "How many tickets are for sale? \n";
							getline(cin, numTicketsString);
							numTickets = stoi(numTicketsString);
							if (numTickets > MAX_TICKETS_SELL){
								cout << "The maximum number of tickets is 100.\n";
								output << "The maximum number of tickets is 100.\n";
							}
							else
								properInput = true;
						}
						catch (invalid_argument e){
							cout << "That is not a valid number.\n";
							output << "That is not a valid number.\n";
						}
					} while (properInput == false);
	
					tickets.sell(eventName, ticketPrice, numTickets, currentUser);
					cout << "Event created.\n";
					output << "Event created.\n";
					
					//daily transaction
					commandCode = "03";
					logString = commandCode + " " + helper::pad(eventName, event_length, " ", false) + " " + helper::pad(currentUser, user_length, " ", false) + " "
						+ helper::pad(to_string(numTickets), 3, "0", true) + " " + helper::pad(helper::dtom(ticketPrice), 6, "0", true);
					writer.writeIn(logString);
				}
				else{
					cout << "This account is not allowed to sell tickets. Please log in as a seller account.\n";
					output << "This account is not allowed to sell tickets. Please log in as a seller account.\n";
				}
			}
			// BUY
			else if (command == "buy"){
				if (userType != "SS"){
					string eventName;
					string numTicketsString;
					string seller;
					int numTickets;
					int ticketsRemaining;
					double price;
					vector<string> events = tickets.getEvents();
				
					bool foundEvent = false;
					cout << "What event would you like to buy tickets for? ";
					output << "What event would you like to buy tickets for? \n";
					getline(cin, eventName);

					cout << "What is the sellers name?";
					output << "What is the sellers name?\n";
					getline(cin, seller);

					int index = 0;
					for (index = 0; index < events.size(); index++){
						if (helper::pad(eventName, event_length, " ", false).compare(events[index].substr(0, event_length)) == 0){	//searching for the event
							if (helper::pad(seller, user_length, " ", false).compare(events[index].substr(26, user_length)) == 0){
								ticketsRemaining = stoi(events[index].substr(42, 3));
								price = stod(events[index].substr(46, 6));
								foundEvent = true;
							}
						}
					}
					if (foundEvent == false){
						cout << "Either that event doesn't exist or that seller isn't selling tickets to that show.\n";
						output << "Either that event doesn't exist or that seller isn't selling tickets to that show.\n";
					}
					else{
						bool properInput = false;
						do{
							try{
								cout << "How many tickets would you like to purchase? ";
								output << "How many tickets would you like to purchase? \n";
								getline(cin, numTicketsString);
								numTickets = stoi(numTicketsString);
								if (numTickets > MAX_TICKETS_BUY && userType!="AA"){
									cout << "Can't buy more than 4 tickets at once.\n";
									output << "Can't buy more than 4 tickets at once.\n";
								}
								else
									properInput = true;
							}
							catch (invalid_argument e){
								cout << "That is not a valid number.\n";
								output << "That is not a valid number.\n";
							}
						} while (properInput == false);

						if (numTickets > ticketsRemaining){
							cout << "Not enough tickets.\n";
							output << "Not enough tickets.\n";
						}
						else{
							double cost = price*numTickets;
							if (cost > userWalletValue){
								cout << "Not enough credit to complete transaction. Please add more credit first.\n";
								output << "Not enough credit to complete transaction. Please add more credit first.\n";
							}
							else{
								userWalletValue -= cost;

								users.transferCredit(currentUser, seller, cost, true);	//update the buyers wallet
								tickets.buy(eventName, numTickets);
								cout << "Tickets purchased.\n";
								output << "Tickets purchased.\n";
								//daily transaction
								commandCode = "04";
								logString = commandCode + " " + helper::pad(eventName, event_length, " ", false) + " " + helper::pad(seller, user_length, " ", false) + " "
									+ helper::pad(to_string(numTickets), 3, "0", true) + " " + helper::pad(helper::dtom(price), 6, "0", true);
								writer.writeIn(logString);
							}
						}
					}
				}
				else{
					cout << "This account is not allowed to buy tickets. Please log in as a buyer account.\n";
					output << "This account is not allowed to buy tickets. Please log in as a buyer account.\n";
				}
			}
			// REFUND
			else if (command == "refund"){
				if (userType == "AA"){
					string buyer;
					string seller;

					string amountString;
					double amount;
					bool foundUser = false;
					
					do{
						cout << "Which buyer is getting a refund? ";
						output << "Which buyer is getting a refund? \n";
						getline(cin, buyer);
						if (users.auth(buyer) == -1){
							cout << "That user does not exist\n";
							output << "That user does not exist\n";
						}
						else
							foundUser = true;
					} while (foundUser == false);

					foundUser = false;

					do{
						cout << "Which seller is issuing a refund? ";
						output << "Which seller is issuing a refund? \n";
						getline(cin, seller);
						if (users.auth(seller) == -1){
							cout << "That user does not exist.\n";
							output << "That user does not exist.\n";
						}
						else
							foundUser = true;
					} while (foundUser == false);

					bool properInput = false;
					do{
						cout << "How much credit is being refunded? ";
						output << "How much credit is being refunded? \n";
						getline(cin, amountString);
						try{
							amount = stod(amountString);
							properInput = true;
						}
						catch (invalid_argument e){
							cout << "That is not a valid number.\n";
							output << "That is not a valid number.\n";
						}
					} while (properInput == false);

					users.transferCredit(buyer, seller, amount, false);
					cout << "Refund complete.\n";
					output << "Refund complete.\n";
					// daily transaction
					commandCode = "05";
					logString = commandCode + " " + helper::pad(buyer, user_length, " ", false) + " " + helper::pad(seller, user_length, " ", false) 
						+ " " + helper::pad(helper::dtom(amount), 9, "0", true);
					writer.writeIn(logString);
				}
				else{
					cout << "Please log in as an administrator to issue a refund.\n";
					output << "Please log in as an administrator to issue a refund.\n";
				}
			}
			else if (command == "addCredit"){

				string creditUser;
				double amount;

				if (userType == "AA"){
					bool foundUser = false;
					do{
						cout << "Which user account would you like to add credit to? ";
						output << "Which user account would you like to add credit to? \n";
						getline(cin, creditUser);
						if (users.auth(creditUser) == -1){
							cout << "Cannot find that user.\n";
							output << "Cannot find that user.\n";
						}
						else
							foundUser = true;
					} while (foundUser == false);
				}
				else{
					creditUser = currentUser;
				}

				bool properInput = false;
				do{
					cout << "How much credit would you like to add? ";
					output << "How much credit would you like to add? \n";
					string amountString;
					getline(cin, amountString);
					try{
						amount = stod(amountString);
						properInput = true;
					}
					catch (invalid_argument e){
						cout << "That is not a valid number.\n";
						output << "That is not a valid number.\n";
					}
				} while (properInput == true);

				if (amount > 1000){
					cout << "Cannot add more than $1000.00 credit at one time.\n";
					output << "Cannot add more than $1000.00 credit at one time.\n";
				}
				else if ((users.auth(creditUser) + amount) > MAX_ACCOUNT_VALUE){
					cout << "You cannot have that much credit in the system.\n";
					output << "You cannot have that much credit in the system.\n";
				}
				else{
					if (creditUser == currentUser)
						userWalletValue += amount;
					users.updateWallet(creditUser, users.auth(creditUser)+amount);
					cout << "Credit added.\n";
					output << "Credit added.\n";

					//daily transaction
					commandCode = "06";
					logString = commandCode + " " + helper::pad(creditUser, user_length, " ", false) + " " + users.getType(creditUser)
						+ " " + helper::pad(helper::dtom(users.auth(creditUser) + amount), 9, "0", true);
					writer.writeIn(logString);
				}
			}
			else if (command == "help"){
				if (userType == "AA"){
					cout << "create - creates a new user in the system.\n";
					cout << "delete - delete a user from the system.\n";
					cout << "buy - purchase tickets to an event.\n";
					cout << "sell - post tickets to an event for sale.\n";
					cout << "refund - issue a refund from a seller to a buyer\n";
					cout << "addCredit - add credit to a user's account.\n";
					cout << "logout - logout of the system.\n";
					cout << "quit - logout of the system and exit.\n";


					output << "create - creates a new user in the system.\n";
					output << "delete - delete a user from the system.\n";
					output << "buy - purchase tickets to an event.\n";
					output << "sell - post tickets to an event for sale.\n";
					output << "refund - issue a refund from a seller to a buyer\n";
					output << "addCredit - add credit to a user's account.\n";
					output << "logout - logout of the system.\n";
					output << "quit - logout of the system and exit.\n";

				}
				else if (userType == "FS"){
					cout << "buy - purchase tickets to an event.\n";
					cout << "sell - post tickets to an event for sale.\n";
					cout << "addCredit - add credit to a user's account.\n";
					cout << "logout - logout of the system.\n";
					cout << "quit - logout of the system and exit.\n";

					output << "buy - purchase tickets to an event.\n";
					output << "sell - post tickets to an event for sale.\n";
					output << "addCredit - add credit to a user's account.\n";
					output << "logout - logout of the system.\n";
					output << "quit - logout of the system and exit.\n";



				}
				else if (userType == "SS"){
					cout << "sell - post tickets to an event for sale.\n";
					cout << "addCredit - add credit to a user's account.\n";
					cout << "logout - logout of the system.\n";
					cout << "quit - logout of the system and exit.\n";

					output << "sell - post tickets to an event for sale.\n";
					output << "addCredit - add credit to a user's account.\n";
					output << "logout - logout of the system.\n";
					output << "quit - logout of the system and exit.\n";



				}
				else if (userType == "BS"){
					cout << "buy - purchase tickets to an event.\n";
					cout << "addCredit - add credit to a user's account.\n";
					cout << "logout - logout of the system.\n";
					cout << "quit - logout of the system and exit.\n";


					output << "buy - purchase tickets to an event.\n";
					output << "addCredit - add credit to a user's account.\n";
					output << "logout - logout of the system.\n";
					output << "quit - logout of the system and exit.\n";

				}
			}
			else{
				string errorString = "\"" + command + "\" is not a valid command. Type help to bring up a list of possible commands.\n";
				cout << errorString;
				output << errorString;
			}
		}
		else
		{
			cout << "Please enter your login name: ";
			output << "Please enter your login name: \n";
			string username;
			getline(cin,username);

			if(username == "quit"){

				cout << "Ready to shut down. . . .\n";
				output << "Ready to shut down. . . .\n";

				system("Pause");

				exit(0);


			}


			userWalletValue = users.auth(username);
			if (userWalletValue == -1){
				cout << "Invalid login name.\n";
				output << "Invalid login name.\n";
			}
			else{
				loggedIn = true;
				currentUser = username;
				userType = users.getType(username);
			}
		}
	}


	output.close();


}
