package runtime;

import static org.junit.Assert.*;

import org.junit.Test;

public class transactionRTest {
	
	
	// constructor has no parameters and catch statement can't be reached unless
	// file is deleted, all that can be done is call the method as is
	@Test
	public void testTransactionR() {

		transactionR testR = new transactionR();
		
		assertEquals(false, testR.isRead());
		
	}

	// this provides statement and decision coverage, there is no loop to test
	@Test
	public void testReadLine() {

		transactionR testR = new transactionR();
		
		String testString = testR.readLine();
		
		//reading until file is empty will reach both the catch statement and the
		//inside of the if statement
		while(!testString.equals("END")){
			
			testString = testR.readLine();
			
		}
		
		assertEquals(true, testR.isRead());
		
	}

	// method has no parameters and only 1 line, just calling it provides statement
	// and decision coverage, there is also no loop

	@Test
	public void testIsRead() {

		transactionR testR = new transactionR();
		assertEquals(false, testR.isRead());
		
	}

}
