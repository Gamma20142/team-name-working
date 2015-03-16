to add a test to the master:

	-copy/paste the GenericSpecificTest folder with
	the name of the new test

	-add that folder name to the reportDirectories.txt

	-add the call to that folder's test.bat in the 
	masterTester.bat (copy/paste all 3 lines)

	-edit the expected.txt and testCommands.txt files inside
	that test so the test will carry out what you want



notes:

	-the reportDirectories.txt must always end with the 
	terminate line.

	-DO NOT edit the test.bat for each test

	-inside the masterTester.bat the lines above and below
	each call control the directory cursor, therefore you
	must always copy/paste that block of 3 lines when adding
	a new test