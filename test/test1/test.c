#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.fmc.h"

#include "test.fmc.LOG.c"

int test()
{
	FMCCOM_TestMessage	com ;
	FMCREF_TestMessage	ref ;
	long			comlen ;
	
	/* init communication buffer */
	memset( & com , 0x00 , sizeof(FMCCOM_TestMessage) );
	
	/* client code */
	memset( & ref , 0x00 , sizeof(FMCREF_TestMessage) );
	FMCBIND_TestMessage( & com , FMCLEN_TestMessage , & ref );
	
	*(ref.version) = 1 ;
	strcpy( ref.name , "calvin" );
	*(ref.oper_number) = 12345 ;
	*(ref.library_card_number) = 1234567890 ;
	*(ref.amount) = 100.00 ;
	*(ref.balance) = 10000.00 ;
	strcpy( ref.remark , "my remark" );
	
	comlen = FMCLEN_TestMessage ;
	FMCUNBIND_TestMessage( & ref , & com , & comlen );
	
	/* ... client send communication buffer to server in '& com' with length 'comlen' ... */
	
	/* server code */
	memset( & ref , 0x00 , sizeof(FMCREF_TestMessage) );
	FMCBIND_TestMessage( & com , comlen , & ref );
	
	FMCLOG_TestMessage( & ref );
	
	return 0;
}

int main()
{
	return -test();
}
