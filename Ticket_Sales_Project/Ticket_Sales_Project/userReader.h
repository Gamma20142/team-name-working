#pragma once
#include <string>
#include <vector>

using namespace std;

class userReader{
private:	
	vector<string> users;
public:
	int read();
	void printUsers();
	string getType(string username);
	int transferCredit(string buyer, string seller,double amount, bool sale);
	double auth(string user);
	int create(string user, string type, int credit);
	int deleteUser(string user);
	int updateWallet(string user, double value);
};
