#include "fmc.h"

/*
 * fmc - FlatMessages compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

static void version()
{
	printf( "fmc v1.0.0 - Flat Message Compiler\n" );
	printf( "Copyright by calvin<calvinwilliams.c@gmail.com> 2014\n" );
}

static void usage()
{
	printf( "USAGE : fmc -f .fmc [ -c ]\n" );
}

int main( int argc , char *argv[] )
{
	int			c ;
	struct CommandParameter	cmdparam ;
	
	if( argc == 1 )
	{
		usage();
		exit(0);
	}
	
	memset( & cmdparam , 0x00 , sizeof(struct CommandParameter) );
	for( c = 1 ; c < argc ; c++ )
	{
		if( strcmp( argv[c] , "-v" ) == 0 && 1 + 1 == argc )
		{
			version();
			exit(0);
		}
		else if( strcmp( argv[c] , "-f" ) == 0 && c + 1 < argc )
		{
			c++;
			cmdparam.pathfilename = argv[c] ;
		}
		else if( strcmp( argv[c] , "-c" ) == 0 )
		{
			cmdparam.output_c_flag = 1 ;
		}
		else
		{
			usage();
			exit(7);
		}
	}
	
	if( cmdparam.pathfilename == NULL )
	{
		usage();
		exit(7);
	}
	
	return -fmc( & cmdparam ) ;
}
