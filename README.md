# XML-Parser
This is Simple XML file Parser implemented in C.
Note: This require gcc compiler installed

Description:
Parser API's:
SET: Function to write data into XML file. Refer Test.c for usage details.
GET: Function to get data from XML file. Refer Test.c for usage details.

Steps:
1. Change PWD to this folder. 
2. Execute Makefile using "make" command.
3. Execute parser using "./parser xml.xml" command.


File Details:
parser.c: API's SET and GET are implemented here.
Test.c: This file illustrates the usage of SET and GET API's. This file can be replaced with user written file but Makefile should be updated.
parser: This is the executable that get generated after executing "make" command.  This should be passed with XML file name as an argument
xml.xml: This can be replaced with user xml file. User file should be passed in this command "./parser UserXMLfilename.xml"





