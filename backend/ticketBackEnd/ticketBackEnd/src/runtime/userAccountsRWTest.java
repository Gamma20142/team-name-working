package runtime;

import static org.junit.Assert.*;

import java.io.IOException;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

public class userAccountsRWTest {

	//our file is userInfo.txt, so if it is found then the RW will be created properly
	@Test
	public void testUserAccountsRW() {
		userAccountsRW test = new userAccountsRW();
		assertTrue(test.isFileFound());
	}

	@Test
	public void testGetUsers() throws Exception {
		userAccountsRW testRW = new userAccountsRW();
		user[] userList;
		userList=testRW.getUsers();
		assertEquals("gamma20142     ",userList[0].getName());
	}

	//to test this we are reading in the current userInfo file and then just rewriting it back
	@Test
	public void testWriteUsers() throws Exception {
		userAccountsRW testRW = new userAccountsRW(); 
		user[] userList;
		userList=testRW.getUsers();
		int result= testRW.writeUsers(userList);
		assertEquals(0, result);
	}
	
}



