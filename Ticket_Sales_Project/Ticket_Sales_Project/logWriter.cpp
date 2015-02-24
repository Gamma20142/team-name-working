#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "logWriter.h"

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