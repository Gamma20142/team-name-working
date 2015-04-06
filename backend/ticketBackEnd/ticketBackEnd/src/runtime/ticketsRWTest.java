package runtime;

import static org.junit.Assert.*;

import java.io.IOException;

import org.junit.Test;

public class ticketsRWTest {
	
	//NOTE that the only what to test the constructor is to run it and see if it
	// throws an error or not
	@Test
	public void testTicketsRW() {
		
		ticketsRW testRW = new ticketsRW();
		assertNotNull(testRW);
	}
	
	
	//NOTE getTickets has no parameters but by reading the whole file both statement
	// coverage and decision coverage are provided, the loop can only be executed
	// the same number of times are there is lines plus one
	@Test
	public void testGetTickets() throws Exception {

		ticketsRW testRW = new ticketsRW();
		
		ticket testList[];
		
		testList = testRW.getTickets();
		
		assertEquals("Celine Dion Show         ", testList[0].getName());
		
	}
	
	
	
	//NOTE in order to not ruin the ticketInfo file the file will be read that wrote
	//back in the same state
	//NOTE by writing the whole file the test provides statement and decision coverage
	//not including the test statements
	//the loop will only execute the same number of times as there are ticket objects

	@Test
	public void testWriteTickets() throws Exception {

		ticketsRW testRW = new ticketsRW();
		
		ticket testList[];
		
		testList = testRW.getTickets();
		
		int result = testRW.writeTickets(testList);
		
		assertEquals(0, result);
		
	}

}
