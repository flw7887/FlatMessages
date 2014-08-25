#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "test.fmc.h"
#include "test.fmc.LOG.c"

int press( long press_count )
{
	FMCCOM_PersonalInfoList	com ;
	FMCREF_PersonalInfoList	ref ;
	long			comlen ;
	long			c ;
	long			t1 , t2 , dt ;
	
	time( & t1 );
	
	memset( & com , 0x00 , sizeof(FMCCOM_PersonalInfoList) );
	comlen = sizeof(FMCCOM_PersonalInfoList) ;
	memset( & ref , 0x00 , sizeof(FMCREF_PersonalInfoList) );
	FMCBIND_PersonalInfoList( & com , comlen , & ref );
	FMCINIT_PersonalInfoList( & com , & ref );
	
	*(ref.PersonalInfo[36].id) = 1001 ;
	strcpy( ref.PersonalInfo[36].name , "Calvin" );
	*(ref.PersonalInfo[36].age) = 35 ;
	*(ref.PersonalInfo[36].phone_num) = 12345678 ;
	
	for( c = 0 ; c < press_count ; c++ )
	{
		FMCUNBIND_PersonalInfoList( & ref , & com , & comlen );
		memset( & ref , 0x00 , sizeof(FMCREF_PersonalInfoList) );
		FMCBIND_PersonalInfoList( & com , comlen , & ref );
	}
	
	FUNCNAME_FMCLOG_PersonalInfoList( & ref );
	
	time( & t2 );
	dt = t2 - t1 ;
	
	printf( "sizeof(FMCCOM_PersonalInfoList)[%d]\n" , sizeof(FMCCOM_PersonalInfoList) );
	printf( "EPLASE %ld seconds\n" , dt );
	
	return 0;
}

static void usage()
{
	printf( "USAGE : test press_count\n" );
}

int main( int argc , char *argv[] )
{
	if( argc != 1 + 1 )
	{
		usage();
		exit(7);
	}
	
	return -press( atol(argv[1]) );
}
