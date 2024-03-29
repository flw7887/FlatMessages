#include "fmc.h"

/*
 * fmc - FlatMessages compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int ClearLeft( char *str )
{
	char *pc = NULL ;
	long l;
	long len = strlen( str ) ;
	
	if( str == NULL )
		return -1;
	
	if( *str == '\0' )
		return -2;
	
	pc = str ;
	while(1)
	{
		if
		(
			*pc == ' '
			||
			*pc == '\t'
		)
		{
			;
		}
		else if
		(
			pc < str + len - 1
			&&
			(
				(unsigned char)(*pc) == 0xA1
				&&
				(unsigned char)(*(pc+1)) == 0xA1
			)
		)
		{
			pc += 1 ;
		}
		else
		{
			break;
		}
		
		pc++;
		
		if( pc == str + len )
			break;
	}
	
	if( pc != str )
	{
		len = strlen( pc );
		for( l = 0 ; l < len + 1 ; l++ )
			*( str + l ) = *( pc + l ) ;
	}
	
	return 0;
}

int ClearRight( char *str )
{
	char *pc = NULL ;
	long len;
	
	if( str == NULL )
		return -1;
	
	if( *str == '\0' )
		return -2;
	
	len = strlen( str ) ;
	pc = str + len - 1 ;
	while(1)
	{
		if
		(
			*pc == ' '
			||
			*pc == '\t'
		)
		{
			;
		}
		else if
		(
			pc > str
			&&
			(
				(unsigned char)(*pc) == 0xA1
				&&
				(unsigned char)(*(pc-1)) == 0xA1
			)
		)
		{
			pc -= 1 ;
		}
		else
		{
			*( pc + 1 ) = '\0' ;
			
			break;
		}
		
		if( pc == str )
		{
			(*pc) = '\0' ;
			
			break;
		}
		
		pc--;
	}
	
	return 0;
}

char *StringNoEnter( char *str )
{
	char	*ptr = NULL ;
	
	if( str == NULL )
		return NULL;
	
	for( ptr = str + strlen(str) - 1 ; ptr >= str ; ptr-- )
	{
		if( (*ptr) == '\r' || (*ptr) == '\n' )
			(*ptr) = '\0' ;
		else
			break;
	}
	
	return str;
}

void fprintab( FILE *fp , int depth )
{
	int		i ;
	
	for( i = 0 ; i < depth ; i++ )
	{
		fprintf( fp , "\t" );
	}
	
	return;
}

