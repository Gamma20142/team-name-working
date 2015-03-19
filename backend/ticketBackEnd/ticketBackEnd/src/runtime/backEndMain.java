//
// this is the main runnable program which will
// take in all the data from the user accounts,
// tickets file, and daily transaction file and
// update the user accounts and daily transaction 
// files as necessary
//
// To run the program from the command line navigate to the src folder and type:
// java runtime.backEndMain


package runtime;

import java.io.IOException;

public class backEndMain {
	
	public static void main(String[] args) throws IOException{
		
		userAccountsRW userReaderWriter = new userAccountsRW();
		ticketsRW ticketReaderWriter = new ticketsRW();
		transactionR transactions = new transactionR();
		
		user[] users = userReaderWriter.getUsers();
		ticket[] tickets = ticketReaderWriter.getTickets();
		
		boolean done = false;
		
		// handle each transaction one at a time
		while(!done){
			
			String currentTransaction = transactions.readLine();
			
			if(currentTransaction.equals("END")){
				
				done = true;
				
			}
			else if(currentTransaction.substring(0, 2).equals("00")){ //logout
				
				String name = currentTransaction.substring(3, 18);
				
				double newWalletValue = Double.parseDouble(currentTransaction.substring(22));
				
				int userAddress = helper.findUser(users, name);
				
				if(userAddress != -1){
					
					users[userAddress].updateWallet(newWalletValue);
					
				}
				
				
				System.out.println("Logout from "
						+ currentTransaction.substring(3, 18));
				
			}
			else if(currentTransaction.substring(0, 2).equals("01")){ //create user
				
				String name = currentTransaction.substring(3, 18);
				String type = currentTransaction.substring(19, 21);
				
				double wallet = 0.0; //assume empty wallet
				
				user newUser = new user(name, type, wallet);
				
				users = helper.addUser(users, newUser);
				
				System.out.println("Added user " + name);
				
			}
			else if(currentTransaction.substring(0, 2).equals("02")){ // delete user
				
				String name = currentTransaction.substring(3, 18);
				
				user[] placeholder = users;
				
				users = helper.deleteUser(users, name);
				
				if(placeholder.length == users.length) // user couldnt be found or deleted
					System.out.println("Could not find user (to delete) " + name);
				else //user found and deleted
					System.out.println("Deleted user " + name);
				
				
			}
			else if(currentTransaction.substring(0, 2).equals("03")){ //put tickets for sale
				
				String event = currentTransaction.substring(3, 28);
				String seller = currentTransaction.substring(29, 44);
				
				int quantity = Integer.parseInt(currentTransaction.substring(45, 48));
				
				double price = Double.parseDouble(currentTransaction.substring(49));
				
				
				ticket newTicket = new ticket(event, seller, quantity, price);
				
				tickets = helper.addTicket(tickets, newTicket);
				
				System.out.println("Created new event " + event);
				
				
			}
			else if(currentTransaction.substring(0, 2).equals("04")){ // buy tickets
				
				String event = currentTransaction.substring(3, 28);
				String seller = currentTransaction.substring(29, 44);
				
				int quantity = Integer.parseInt(currentTransaction.substring(45, 48));
				
				double price = Double.parseDouble(currentTransaction.substring(49));
				
				
				int eventAddress = helper.findTicket(tickets, event);
				int sellerAddress = helper.findUser(users, seller);
				
				
				if(eventAddress == -1 || sellerAddress == -1){
					
					System.out.println("Event or Seller don't exist, can't"
							+ " complete buy transaction:\n" + event + "\n" 
							+ seller);
					
				}
				else{
					
					double total = price*quantity;
					
					if(users[sellerAddress].getWallet() + total > 999999.99){
						
						System.out.println("Seller has too much credit"
								+ " to complete purchase");
						
					}
					else if(tickets[eventAddress].getQuantity() < quantity){
						
						System.out.println("Not enough tickets for sale.");
						
					}
					else{
						
						users[sellerAddress].updateWallet(users[sellerAddress].getWallet() + total);
						
						tickets[eventAddress].updateQuantity(tickets[eventAddress].getQuantity() - quantity);
						
						System.out.println("Sold tickets belonging to  " + seller);
						
					}
					
				}
				
			}
			else if(currentTransaction.substring(0, 2).equals("05")){ // refund
				
				String buyer = currentTransaction.substring(3, 18);
				String seller = currentTransaction.substring(19, 34);
				
				int buyerAddress = helper.findUser(users, buyer);
				int sellerAddress = helper.findUser(users, seller);
				
				double credit = Double.parseDouble(currentTransaction.substring(35));
				
				if(buyerAddress == -1 || sellerAddress == -1){
					
					System.out.println("One of these users don't exist, can't"
							+ " complete refund transaction:\n" + buyer + "\n" 
							+ seller);
					
				}
				else{
					
					if(users[buyerAddress].getWallet() + credit > 999.99){
						
						System.out.println("Buyer has too much credit"
								+ " to complete the transaction");
						
					}
					else if(users[sellerAddress].getWallet() - credit < 0.0){
						
						System.out.println("Seller doesn't have enough credit"
								+ " to complete the transaction");
						
					}
					else{
						
						users[buyerAddress].updateWallet(users[buyerAddress].getWallet() + credit);
						users[sellerAddress].updateWallet(users[sellerAddress].getWallet() - credit);
						
						System.out.println("Refund transaction completed.");
						
					}
					
				}
				
			}
			else if(currentTransaction.substring(0, 2).equals("06")){ //addcredit
				
				String name = currentTransaction.substring(3, 18);
				double newWalletValue = Double.parseDouble(currentTransaction.substring(22));
				
				
				int address = helper.findUser(users, name);
				
				
				if(address == -1)
					System.out.println("Could not find user (to add credit) " + name);
				else{
					
					users[address].updateWallet(newWalletValue);
					
					System.out.println("Updated wallet for user " + name);
					
				}
				
			}
			
		}
		
		//update both info files
		ticketReaderWriter.writeTickets(tickets);
		
		userReaderWriter.writeUsers(users);
		
	}
	
}
