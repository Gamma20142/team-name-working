package runtime;

public class ticket {
	
	private String name;
	private String seller;
	
	private int quantity;
	
	private double price;
	
	
	public ticket(String n, String s, int q, double p) throws Exception{
	
		//test for invalid inputs, throw an exception if one is found
		if (n.length()>25 || n.length() < 0){
			System.out.println("ERROR: Name \'" + n +"\' is an invalid name.");
			throw new Exception();
		}	
		
		if (s.length()>15 || s.length() < 0){
			System.out.println("ERROR: Username \'" + s + "\' is an invalid username.");
			throw new Exception();
		}
		
		if (q > 100 || q < 0){
			System.out.println("ERORR:" + q + " is an invalid quantity.");
			throw new Exception();
		}
	
		if (p>=1000 ||  p < 0){
			System.out.println("ERROR: " + p + " is an invalid price.");
			throw new Exception();			
		}
	
		
		name = n;
		seller = s;	
		quantity = q;
		price = p;
		
	}
	
	
	//return the event's name
	public String getName(){
		
		return name;
		
	}
	
	
	//return the event's seller
	public String getSeller(){
		
		return seller;
		
	}
	
	
	//return the number of tickets left
	public int getQuantity(){
		
		return quantity;
		
	}
	
	
	//return the event's name
	public double getPrice(){
		
		return price;
		
	}
	
	
	
	// only allows there to be between 0 and 999 tickets
	public int updateQuantity(int newQ){
		
		if(newQ < 0 || newQ > 999)
			return -1;
		else{
			
			quantity = newQ;
			return 0;
			
		}
		
	}

}
