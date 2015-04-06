//
// Holds all of the recurring methods used by all the other programs
//

package runtime;

public class helper {

	public static ticket[] addTicket(ticket[] oldArray, ticket newTicket){
		ticket[] newArray = new ticket[oldArray.length + 1];
		
		for(int index = 0; index < oldArray.length; index++){			
			newArray[index] = oldArray[index];
		}
		
		newArray[oldArray.length] = newTicket;
		return newArray;	
	}
	
	public static user[] addUser(user[] oldArray, user newUser){
		
		user[] newArray = new user[oldArray.length + 1];
		
		for(int index = 0; index < oldArray.length; index++){
			
			newArray[index] = oldArray[index];
		}
		
		newArray[oldArray.length] = newUser;
		return newArray;	
	}
	
	public static user[] deleteUser(user[] users, String name){
		
		int address = findUser(users, name);
		
		if(address == -1)
			return users; // user couldnt be found ... return old array
	
		user[] newArray = new user[users.length - 1];
		
		for(int index = 0; index < newArray.length; index++){
			if(index >= address)
				newArray[index] = users[index+1];
			else
				newArray[index] = users[index];	
		}
		return newArray;	
	}
	
	public static int findUser(user[] users, String name){
		int address = -1;
		
		for(int index = 0; index < users.length; index++){
			
			if(users[index].getName().equals(name))
				address = index;		
		}
		return address;		
	}
	
	public static int findTicket(ticket[] tickets, String name){
		int address = -1;
		
		for(int index = 0; index < tickets.length; index++){
			if(tickets[index].getName().equals(name))
				address = index;
		}
		return address;
	}

	public static String padInt(int num, int length){
	
		String numAsString = "" + num;
		String result = "";
		
		for(int index = 0; index < (length - numAsString.length()); index++){
			result += "0";
		}
		
		result += numAsString;
		return result;	
	}
	
	//format is always 2 decimal places
	public static String padDouble(double num, int length){
		
		String numAsString = "" + num;
		String[] parts = numAsString.split("\\."); //0 is before decimal, 1 is after
		String result = "";
		
		//add leading 0s before decimal place
		for(int index = 0; index < (length - parts[0].length() - 3); index++){
			result += "0";	
		}
		result += parts[0];
		result += ".";
		result += parts[1];
		
		for(int index = 0; index < (length - result.length()); index++){
			result += "0";	
		}
		return result;	
	}
}
