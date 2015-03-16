#include "userReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "helper.h"

using namespace std;

/*	USERREADER
/	written by	Ryan Morton - 100485285
/				Thomas Frantz - 100484424
/				Alvin Lee - 100484342
/
/	Manages the user information. 
*/


const double MAX_ACCOUNT_VALUE = 999999.99;
const int user_length = 15;

// read - reads in the user information from the user file and stores it in a vector called users
// return:   0 on success
//			-1 on file open failure\

int userReader::read(){
	ifstream reader("userInfo.txt");
	string input;
	if (reader.is_open()){
		getline(reader, input);
		while (input.compare("END") != 0){
			users.push_back(input);
			getline(reader, input);
		}
		return 0;
	}
	else{
		cout << "Unable to open file";
		return -1;
	}
}



// findUser - an internal function used to find the index of a user in the users vector
// input:	string user - the user we are looking for
// return:	 -1 if the user wasn't found
//			the index of the user if it was found

int userReader::findUser(string user){
	int index = 0;
	for (index = 0; index < users.size(); index++){
		if (helper::pad(user, user_length, " ", false).compare(users[index].substr(0, user_length)) == 0){
			return index;
		}
	}
	return -1;
}



// printUsers - an internal function used to check the data in the users vector

void userReader::printUsers(){
	for (int i = 0; i < users.size(); i++){
		cout << users[i];
		cout << endl;
	}
	cout << "\n";
}

// getType - gets the account type of a given user
// input:	string user - the username we want to know the account type for
// return:	"" - if the username is not found
//			the users type if the username is found

string userReader::getType(string user){
	int userIndex = findUser(user);
	if (userIndex == -1){ //user was not found
		return "";
	}
	return users[userIndex].substr(16, 2);
}

// auth - returns the amount of money in the specified user's account
// input:	string user - the username of the user we want to look for
// return: -1 if user not found
//			otherwise returns the amount of credit they have

double userReader::auth(string user){
	int userIndex = findUser(user);
	if (userIndex == -1){ //user was not found
		return -1;
	}
	else{
		string wallet = users[userIndex].substr(19, 9);
		return stod(wallet);
	}
}

// transferCredit - transfers credit between two accounts
// input:	string buyer - the buyer
//			string seller - the seller
//			double amount - the amount to be transferred
//			bool sale - true if it is a sale (buyer to seller), false if it is a refund (seller to buyer)
// return:	0 on success
//			-1 if either seller or buyer was not found
//			-2 if the buyer doesn't have enough credit to make a purchase
//			-3 if the seller doesn't have enough credit to make a refund
//			-4 if the seller will have too much credit to allow a purchase
//			-5 if the buyer will have too much credit to allow a refund

int userReader::transferCredit(string buyer, string seller,double amount, bool sale){
	int buyerIndex = findUser(buyer);
	int	sellerIndex = findUser(seller);
	if (buyerIndex == -1|| sellerIndex == -1){ 
		return -1;
	}

	double buyerCredit = stod(users[buyerIndex].substr(19, 9));
	double sellerCredit = stod(users[sellerIndex].substr(19, 9));

	if (sale){
		if (buyerCredit < amount){	
			return -2;
		}
		if (sellerCredit + amount > MAX_ACCOUNT_VALUE){
			return -4;
		}
		updateWallet(buyer, buyerCredit - amount);
		updateWallet(seller, sellerCredit + amount);
	}
	else{
		if (sellerCredit < amount){	
			return -3;
		}
		if (buyerCredit + amount > MAX_ACCOUNT_VALUE){
			return -5;
		}
		updateWallet(buyer, buyerCredit + amount);
		updateWallet(seller, sellerCredit - amount);
	}
	return 0;
}


// create - creates a new user
// input:	string user - the username of the new user
//			string type - the account type of the new user
//			double credit - the amount of credit the new user has
// return:	0 for success
//			-1 for username too long
//			-2 for invalid account type
//			-3 for invalid credit

int userReader::create(string user, string type, double credit){

	if (user.length() > user_length){
		return -1;
	}

	if (type != "AA" && type != "FS" && type != "BS" && type != "SS"){
		return -2;
	}
	string creditString = "";
	if (credit >= 0 && credit <= MAX_ACCOUNT_VALUE){
		users.push_back(helper::pad(user, user_length, " ", false) + " " + type + " " + helper::pad(helper::dtom(credit),9,"0",true));
	}
	else{
		return -3;
	}

	return 0;
}

// deleteUser - deletes a user from the system
// input:	string user - the username to be deleted
// return:	 0 on success
//			-1 if user was not found 

int userReader::deleteUser(string user){
	int userIndex = findUser(user);
	if (userIndex == -1){ 
		return -1;
	}
	users.erase(users.begin()+userIndex);
	return 0;
}

// updateWallet - updates a user's wallet value to a given amount
// input:	string user - the user's wallet we are adjusting
//			double value - the amount that the user's wallet is adjusted to
// return:	 0 on success
//			-1 if user was not found

int userReader::updateWallet(string user, double value){
	int userIndex = findUser(user);
	if (userIndex == -1){ 
		return -1;
	}
	string newString = users[userIndex].substr(0, 19) + helper::pad(helper::dtom(value),9,"0",true);
	users[userIndex] = newString;	
	return 0;
}

/*int main(){
	userReader user;
	user.read();
	user.printUsers();
	int test = user.auth("iodsadsa");
	cout << test;
	cout << endl;
	user.create("jimmy", "AA", 200);
	user.printUsers();
	user.deleteUser("philcollins");
	user.printUsers();
	user.updateWallet("jimmy", 900);
	user.printUsers();
	return 0;
}
*/