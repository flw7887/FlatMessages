#include "fmc.h"

/*
 * fmc - FlatMessages compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int ReadFmcFile( struct CommandParameter *pcmdparam , int depth , int *p_offset , char *fmc_pathfilename , int flag , FILE *fp_fmc , long lineno , struct MessageInfo *pmsginfo )
{
	char			filebuffer[ 4096 + 1 ] ;
	char			tmp[ 64 + 1 ] ;
	char			tmp2[ 64 + 1 ] ;
	int			count ;
	int			message_len ;
	
	char			include_pathfilename[ 256 + 1 ] ;
	
	struct FieldInfo	*field = NULL ;
	struct MessageInfo	*message = NULL ;
	
	if( depth > FMC_MAXDEPTH - 1 )
	{
		fprintf( stderr , "*** ERROR : : too many sub messages\n" );
		return -1;
	}
	
	while(1)
	{
		memset( filebuffer , 0x00 , sizeof(filebuffer) );
		if( fgets( filebuffer , sizeof(filebuffer) , fp_fmc ) == NULL )
			break;
		
		lineno++;
		
		ClearLeft( filebuffer );
		ClearRight( filebuffer );
		StringNoEnter( filebuffer );
		if( STRCMP( filebuffer , == , "" ) )
			continue;
		
		switch( flag )
		{
			case 0 :
				memset( tmp , 0x00 , sizeof(tmp) );
				count = sscanf( filebuffer , "%s" , tmp ) ;
				if( STRCMP( tmp , == , "MESSAGE" ) )
				{
					message = (struct MessageInfo *)malloc( sizeof(struct MessageInfo) ) ;
					if( message == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					
					memset( tmp , 0x00 , sizeof(tmp) );
					memset( tmp2 , 0x00 , sizeof(tmp2) );
					memset( message , 0x00 , sizeof(struct MessageInfo) );
					count = sscanf( filebuffer , "%s %s %s %d" , tmp , message->message_name , tmp2 , & (message->array_size) ) ;
					if( count == 2 && STRCMP( tmp , == , "MESSAGE" ) )
					{
						fprintab( stdout , depth ); printf( "MESSAGE %s\n" , message->message_name );
					}
					else if( count == 4 && STRCMP( tmp , == , "MESSAGE" ) && STRCMP( tmp2 , == , "ARRAY" ) )
					{
						fprintab( stdout , depth ); printf( "MESSAGE %s ARRAY %d\n" , message->message_name , message->array_size );
					}
					else
					{
						fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , fmc_pathfilename , lineno , filebuffer );
						return -1;
					}
					
					message_len = ReadFmcFile( pcmdparam , depth , p_offset , fmc_pathfilename , 1 , fp_fmc , lineno , message ) ;
					if( message_len < 0 )
						return message_len;
					
					(*p_offset) += message_len ;
					pmsginfo->next_message = message ;
					pmsginfo->message_length += message_len ;
					
					continue;
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%ld : head line [%s] invalid\n" , fmc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				break;
				
			case 1 :
				if( STRCMP( filebuffer , != , "{" ) )
				{
					fprintf( stderr , "*** ERROR : %s:%ld : second line [%s] invalid\n" , fmc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				flag = 2 ;
				
				break;
				
			case 2 :
				
				if( STRCMP( filebuffer , == , "}" ) )
				{
					if( pmsginfo->array_size == 0 )
						return pmsginfo->message_length;
					else
						return pmsginfo->message_length * pmsginfo->array_size ;
				}
				
				memset( tmp , 0x00 , sizeof(tmp) );
				memset( include_pathfilename , 0x00 , sizeof(include_pathfilename) );
				sscanf( filebuffer , "%s %s" , tmp , include_pathfilename );
				if( STRCMP( tmp , == , "INCLUDE" ) )
				{
					FILE	*include_fp_fmc = NULL ;
					include_fp_fmc = fopen( include_pathfilename , "r" ) ;
					if( include_fp_fmc == NULL )
					{
						fprintf( stderr , "*** ERROR : %s:%ld : file[%s] can't read\n" , fmc_pathfilename , lineno , include_pathfilename );
						return -1;
					}
					message_len = ReadFmcFile( pcmdparam , depth , p_offset , include_pathfilename , 2 , include_fp_fmc , 0 , pmsginfo ) ;
					fclose( include_fp_fmc );
					if( message_len < 0 )
						return message_len;
					pmsginfo->message_length += message_len ;
					
					continue;
				}
				if( STRCMP( tmp , == , "MESSAGE" ) )
				{
					message = (struct MessageInfo *)malloc( sizeof(struct MessageInfo) ) ;
					if( message == NULL )
					{
						fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
						return -1;
					}
					
					memset( tmp , 0x00 , sizeof(tmp) );
					memset( tmp2 , 0x00 , sizeof(tmp2) );
					memset( message , 0x00 , sizeof(struct MessageInfo) );
					count = sscanf( filebuffer , "%s %s %s %d" , tmp , message->message_name , tmp2 , & (message->array_size) ) ;
					if( count == 2 && STRCMP( tmp , == , "MESSAGE" ) )
					{
						fprintab( stdout , depth + 1 ); printf( "MESSAGE %s\n" , message->message_name );
					}
					else if( count == 4 && STRCMP( tmp , == , "MESSAGE" ) && STRCMP( tmp2 , == , "ARRAY" ) )
					{
						fprintab( stdout , depth + 1 ); printf( "MESSAGE %s ARRAY %d\n" , message->message_name , message->array_size );
					}
					else
					{
						fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , fmc_pathfilename , lineno , filebuffer );
						return -1;
					}
					
					message_len = ReadFmcFile( pcmdparam , depth + 1 , p_offset , fmc_pathfilename , 1 , fp_fmc , lineno , message ) ;
					if( message_len < 0 )
						return message_len;
					
					if( pmsginfo->sub_message_list == NULL )
					{
						pmsginfo->sub_message_list = message ;
						pmsginfo->last_sub_message = message ;
					}
					else
					{
						pmsginfo->last_sub_message->next_message = message ;
						pmsginfo->last_sub_message = message ;
					}
					
					pmsginfo->message_length += message_len ;
					
					continue;
				}
				
				field = (struct FieldInfo *)malloc( sizeof(struct FieldInfo) ) ;
				if( field == NULL )
				{
					fprintf( stderr , "*** ERROR : : alloc failed , errno[%d]\n" , errno );
					return -1;
				}
				memset( field , 0x00 , sizeof(struct FieldInfo) );
				
				memset( tmp , 0x00 , sizeof(tmp) );
				count = sscanf( filebuffer , "%s %d %s %s %[^\n]" , field->field_type , & (field->field_length) , field->field_name , tmp , field->init_default ) ;
				if(	(
						STRCMP( field->field_type , == , "INT" )
						&&
						( field->field_length == 1 || field->field_length == 2 || field->field_length == 4 )
					)
					||
					(
						STRCMP( field->field_type , == , "FLOAT" )
						&&
						( field->field_length == 4 || field->field_length == 8 )
					)
					||
					(
						STRCMP( field->field_type , == , "STRING" )
						&&
						field->field_length > 0
					)
				)
				{
					;
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , fmc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				if( STRCMP( tmp , == , "" ) )
				{
				}
				else if( STRCMP( tmp , == , "DEFAULT" ) )
				{
					if( strlen(field->init_default) > 2 && field->init_default[0] == '\"' && field->init_default[strlen(field->init_default)-1] == '\"' )
					{
						memmove( field->init_default , field->init_default + 1 , strlen(field->init_default) - 2 );
						field->init_default[strlen(field->init_default)-2] = '\0' ;
					}
				}
				else
				{
					fprintf( stderr , "*** ERROR : %s:%ld : line [%s] invalid\n" , fmc_pathfilename , lineno , filebuffer );
					return -1;
				}
				
				field->field_offset = (*p_offset) ;
				
				fprintab( stdout , depth ); printf( "	%s %d %s\n" , field->field_type , field->field_length , field->field_name );
				
				pmsginfo->message_length += field->field_length ;
				(*p_offset) += field->field_length ;
				if( STRCMP( field->field_type , == , "STRING" ) )
				{
					pmsginfo->message_length++;
					(*p_offset)++;
				}
				
				if( pmsginfo->field_list == NULL )
				{
					pmsginfo->field_list = field ;
				}
				else
				{
					pmsginfo->last_field->next_field = field ;
				}
				pmsginfo->last_field = field ;
				
				break;
		}
	}
	
	if( pmsginfo->array_size == 0 )
		return pmsginfo->message_length;
	else
		return pmsginfo->message_length * pmsginfo->array_size ;
}
