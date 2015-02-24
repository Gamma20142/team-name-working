#include "userReader.h"
#include <iostream>
#include <fstream>
#include <sstream>

//returns 0 on success, -1 on file open failure
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

void userReader::printUsers(){
	for (int i = 0; i < users.size(); i++){
		cout << users[i];
		cout << endl;
	}
	cout << "\n";
}

string userReader::getType(string user){
	for (int i = user.length(); i < 15; i++){
		user += " ";
	}
	int index = 0;
	for (index = 0; index < users.size(); index++){
		if (user.compare(users[index].substr(0, 15)) == 0)
			break;
	}
	if (index == users.size()){ //user was not found
		return "";
	}
	return users[index].substr(16, 2);
}

//returns -1 if user not found, otherwise returns the amount of credit they have
double userReader::auth(string user){
	for (int i = user.length(); i < 15; i++){
		user += " ";
	}
	int index = 0;
	for (index = 0; index < users.size(); index++){
		if (user.compare(users[index].substr(0, 15))==0)
			break;
	}
	if (index == users.size()){ //user was not found
		return -1;
	}
	else{
		string wallet = users[index].substr(19, 9);
		return stod(wallet);
	}
}

int userReader::transferCredit(string buyer, string seller,double amount, bool sale){
	for (int i = buyer.length(); i < 15; i++){
		buyer += " ";
	}
	for (int i = seller.length(); i < 15; i++){
		seller += " ";
	}
	int buyerIndex = -1;
	int sellerIndex = -1;
	for (int index = 0; index < users.size(); index++){
		if (buyer.compare(users[index].substr(0, 15)) == 0)
			buyerIndex = index;
		if (seller.compare(users[index].substr(0, 15)) == 0)
			sellerIndex = index;
	}
	if (buyerIndex == -1|| sellerIndex == -1){ //either seller or buyer was not found
		return -1;
	}

	double buyerCredit = stod(users[buyerIndex].substr(19, 9));
	double sellerCredit = stod(users[sellerIndex].substr(19, 9));

	if (sale){
		if (buyerCredit < amount){	//buyer doesn't have enough credit to do purchase
			return -2;
		}
		updateWallet(buyer, buyerCredit - amount);
		updateWallet(seller, sellerCredit + amount);
	}
	else{
		if (sellerCredit < amount){	//seller doesn't have enough credit to do refund
			return -3;
		}
		updateWallet(buyer, buyerCredit + amount);
		updateWallet(seller, sellerCredit - amount);
	}
}

//returns error codes for invalid entries
// 0 for success
// -1 for username too long
// -2 for invalid account type
// -3 for invalid credit
int userReader::create(string user, string type, int credit){
	
	if (user.length() > 15)
		return -1;
	else{
		for (int i = user.length(); i < 15; i++){
			user += " ";
		}
	}

	if (type != "AA" && type != "FS" && type != "BS" && type == "SS"){
		return -2;
	}
	string creditString = "";
	if (credit >= 0 && credit < 1000000){
		string converted = to_string(credit);
		
		for (int i = converted.length(); i < 9; i++){
			creditString += "0";
		}
		creditString += converted;
	}
	else{
		return -3;
	}
	users.push_back(user + " " + type + " " + creditString);
	return 0;
}

int userReader::deleteUser(string user){
	for (int i = user.length(); i < 15; i++){
		user += " ";
	}
	int index = 0;
	for (index = 0; index < users.size(); index++){
		if (user.compare(users[index].substr(0, 15)) == 0)
			break;
	}
	if (index == users.size()){ //user was not found
		return -1;
	}

	users.erase(users.begin()+index);
	return 0;
}

int userReader::updateWallet(string user, double value){
	for (int i = user.length(); i < 15; i++){
		user += " ";
	}
	int index = 0;
	for (index = 0; index < users.size(); index++){
		if (user.compare(users[index].substr(0, 15)) == 0)
			break;
	}
	if (index == users.size()){ //user was not found
		return -1;
	}
	string creditString = "";
	if (value >= 0 && value < 1000000){
		string converted = to_string(value);

		for (int i = converted.length(); i < 9; i++){
			creditString += "0";
		}
		creditString += converted;
	}
	string newString = users[index].substr(0, 19) + creditString;
	users[index] = newString;
		
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