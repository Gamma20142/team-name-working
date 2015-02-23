#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class userReader{
	vector<string> users;
public:
	userReader();
	int read();
	void printUsers();
	int auth(string user);
	int create(string user, string type, int credit);
	int deleteUser(string user);
	int updateWallet(string user, int value);
};

userReader::userReader(){

}

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


//returns -1 if user not found, otherwise returns the amount of credit they have
int userReader::auth(string user){
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
		return stoi(wallet);
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

int userReader::updateWallet(string user, int value){
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