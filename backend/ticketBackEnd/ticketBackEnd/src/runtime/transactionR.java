//
// transactionR is the object responsible for 
// reading the daily transaction file for the
// backEndMain
//


package runtime;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class transactionR {
	
	private BufferedReader reader;
	
	private boolean fileFound;
	private boolean read; // checks if file was already read
	//cant read more than once
	
	public transactionR(){
		
		try { // if it can't find the file set check boolean to false
			
			reader = new BufferedReader(new FileReader("DailyTransactionFile.txt"));
			
			read = false;
			fileFound = true;
			
		} catch (FileNotFoundException e) {
			
			fileFound = false;
			
		}
		
	}
	
	public String readLine(){
		
		String result = "";
		
		try {
			result = reader.readLine();
		} catch (IOException e) {
			
			read = true;
			
		}
		
		if(result == null){
			
			result = "END";
			read = true;
			
		}
		
		return result;
		
	}
	
	public boolean isRead(){
		return read;	
	}
}
