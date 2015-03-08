#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "logWriter.h"

/*	LOGWRITER
/	written by	Ryan Morton - 100485285
/				Thomas Frantz - 100484424
/				Alvin Lee - 100484342
/
/	logwriter writes information to the daily transaction file
/
*/

// writeIn - writes to the daily transaction file
// input: string message
// returns -1 on file open failure
// returns 0 on success

int logWriter::writeIn(string message){
	ofstream writer;
	writer.open("DailyTransactionFile.txt", std::ios_base::app);
	if (writer.is_open()){
		writer << message;
		writer << endl;
		writer.close();
		return 0;
	}
	else{
		return -1;
	}
}

/*int main(){
	logWriter test;
	test.writeIn("this is a test");
	return 0;
}*/