package runtime;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class userTest {

	@Test
	public void testUser() throws Exception {
		user test = new user("gamma20142", "AA", 999999.99);
		assertNotNull(test);		
	}
	
	@Test 
	public void testBadUsername() throws Exception{
		try{
			user testBadUsername = new user("this name has more than fifteen characters","AA", 999999.99);
			fail();
		}
		catch(Exception e){}
	}
	
	@Test
	public void testBadUserType() throws Exception{
		try{
			user testBadUserType = new user("gamma20142", "AS", 999999.99);
			fail();
		}catch(Exception e){}
	}
	
	@Test
	public void testBadCredit() throws Exception{
		try{
			user testBadCredit = new user("gamma20142", "AA", 1234567.12);
			fail();
		}catch(Exception e){}
	}
	
	@Test
	public void testGetName() throws Exception {
		user test= new user("gamma20142","AA",999999.99);
		assertEquals("gamma20142",test.getName());
	}

	@Test
	public void testGetType() throws Exception {
		user test= new user("gamma20142","AA",999999.99);
		assertEquals("AA",test.getType());
	}

	@Test
	public void testGetWallet() throws Exception {
		user test= new user("gamma20142","AA",999999.99);
		assertEquals(999999.99,test.getWallet(),0.001);
	}

	@Test
	public void testUpdateWallet() throws Exception {
		user test= new user("gamma20142","AA",999999.99);
		test.updateWallet(12345.6);
		assertEquals(12345.6,test.getWallet(),0.001);
	}
	
	@Test
	public void testUpdateWallet1() throws Exception {
		user test= new user("gamma20142","AA",999999.99);
		test.updateWallet(1000000);
		assertEquals(-1, test.updateWallet(-10));
	}

}
