#include <string>
#include "helper.h"

/*	HELPER
/	written by	Ryan Morton - 100485285
/				Thomas Frantz - 100484424
/				Alvin Lee - 100484342
/
/	A couple of internal helper functions
*/


// pad - pads a string file with data on a specified end of a string
// inputs:  string s - the string that we want to be padded
//			int length - the length that we want the string to be padded to
//			string p - the string that we will be padding s with
//			bool front - true for padding the front of the string, false to pad the back of the string
// returns:	returnString - the final padded string
string helper::pad(string s, int length, string p, bool front){
	string returnString;
	if (front){
		for (int i = s.length(); i < length; i++){
			returnString += p;
		}
		returnString += s;
	}
	else{
		returnString = s;
		for (int i = s.length(); i < length; i++){
			returnString += p;
		}
	}

	return returnString;
}

// dtom - takes a double value and converts it to a currency format (double to money)
// inputs:	double amount - the value we are converting
// returns:	the converted amount formatted like a currency value

string helper::dtom(double amount){
	string dollars = to_string((int)amount);
	string cents = to_string(fmod(amount, 1.0)).substr(2, 2);
	return (dollars + "." + cents);
}
