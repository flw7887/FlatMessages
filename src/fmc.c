#include "fmc.h"

/*
 * fmc - FlatMessages compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int fmc( struct CommandParameter *pcmdparam )
{
	FILE			*fp_fmc = NULL ;
	struct MessageInfo	message ;
	int			message_len ;
	int			offset ;
	
	char			output_pathfilename[ 256 + 1 ] ;
	
	int			nret = 0 ;
	
	fp_fmc = fopen( pcmdparam->pathfilename , "r" ) ;
	if( fp_fmc == NULL )
	{
		fprintf( stderr , "file[%s] can't read\n" , pcmdparam->pathfilename );
		return -1;
	}
	offset = 0 ;
	memset( & message , 0x00 , sizeof(struct MessageInfo) );
	message_len = ReadFmcFile( pcmdparam , 0 , & offset , pcmdparam->pathfilename , 0 , fp_fmc , 0 , & message ) ;
	fclose( fp_fmc );
	if( message_len < 0 )
		return -message_len;
	printf( "ok!\n" );
	
	if( pcmdparam->output_c_flag == 1 )
	{
		FILE	*fp_fmc_h = NULL ;
		FILE	*fp_fmc_c = NULL ;
		FILE	*fp_fmc_LOG_c = NULL ;
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.h" , pcmdparam->pathfilename );
		fp_fmc_h = fopen( output_pathfilename , "w" ) ;
		if( fp_fmc_h == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			return -1;
		}
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.c" , pcmdparam->pathfilename );
		fp_fmc_c = fopen( output_pathfilename , "w" ) ;
		if( fp_fmc_c == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			fclose( fp_fmc_h );
			return -1;
		}
		
		memset( output_pathfilename , 0x00 , sizeof(output_pathfilename) );
		SNPRINTF( output_pathfilename , sizeof(output_pathfilename)-1 , "%s.LOG.c" , pcmdparam->pathfilename );
		fp_fmc_LOG_c = fopen( output_pathfilename , "w" ) ;
		if( fp_fmc_LOG_c == NULL )
		{
			fprintf( stderr , "file[%s] can't write\n" , output_pathfilename );
			fclose( fp_fmc_h );
			return -1;
		}
		
		nret = GenerateCCode( pcmdparam , & message , fp_fmc_h , fp_fmc_c , fp_fmc_LOG_c ) ;
		if( nret )
			return -nret;
		
		fclose( fp_fmc_h );
		fclose( fp_fmc_c );
		fclose( fp_fmc_LOG_c );
	}
	
	return 0;
}
