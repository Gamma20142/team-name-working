//
// ticketsRW is the object responsible for 
// reading and writing the tickets file 
// file for the backEndMain
//

package runtime;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class ticketsRW {
	
	private BufferedReader reader;
	
	private boolean fileFound;
	private boolean read; // checks if file was already read
	//cant read more than once
	
	
	public ticketsRW(){
		
		try { // if it can't find the file set check boolean to false
			
			reader = new BufferedReader(new FileReader("ticketInfo.txt"));
			
			read = false;
			fileFound = true;
			
		} catch (FileNotFoundException e) {
			
			fileFound = false;
			
		//} catch (IOException e) {
			
			//fileFound = false;
			
		}
		
	}
	
	
	
	
	public ticket[] getTickets() throws Exception{
		
		ticket[] tickets = new ticket[0];
		
		if(fileFound && !read){
			
			boolean end = false;
			
			while(!end){
				String raw = reader.readLine();
				
				if (raw != null){
					if(raw.equals("END")){
						end = true;
					}
					else{
						if(!raw.equals("")){ // in case dummy double breaks are inside the list
							String name = raw.substring(0, 25);
							String seller = raw.substring(26, 41);
							int quantity = Integer.parseInt(raw.substring(42, 45));
							double price = Double.parseDouble(raw.substring(46));
							
							ticket current = new ticket(name, seller, quantity, price);
							tickets = helper.addTicket(tickets, current);			
						}
					}
				}
				
				
			}
			
			read = true;
			reader.close();
			
		}
		
		return tickets;
		
	}
	
	
	
	
	public int writeTickets(ticket[] tickets) throws IOException{
		
		BufferedWriter writer = new BufferedWriter(new FileWriter("ticketInfo.txt"));
		
		for(int index = 0; index < tickets.length; index++){
			
			String line = tickets[index].getName() + " "
					+ tickets[index].getSeller() + " "
					+ helper.padInt(tickets[index].getQuantity(), 3) + " "
					+ helper.padDouble(tickets[index].getPrice(), 6); 
			
			try {
				
				writer.write(line);
				writer.newLine();
				
			} catch (IOException e) {
				
				System.out.println("Couldn't write line");
				
				return -1;
				
			}
			
		}
		
		
		try {
			
			writer.write("END");
			
		} catch (IOException e) {
			
			System.out.println("Couldn't write END");
			return -1;
			
		}
		
		writer.close();
		
		return 0;
		
	}
	

}
