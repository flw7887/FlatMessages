#include "fmc.h"

/*
 * fmc - FlatMessages compiler
 * author	: calvin
 * email	: calvinwilliams.c@gmail.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

int GenerateCCode_h( int depth , struct MessageInfo *first_pmsginfo , FILE *fp_fmc_h )
{
	struct MessageInfo	*pmsginfo = NULL ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_message )
	{
		if( depth == 0 )
		{
			if( sizeof(long) == 4 )
			{
				fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "#pragma pack(4)\n" );
			}
			else
			{
				fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "#pragma pack(8)\n" );
			}
			fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "\n" );
		}
		
		if( depth == 0 )
		{
			fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "typedef struct\n" );
		}
		else
		{
			fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "struct\n" );
		}
		fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "{\n" );
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintab( fp_fmc_h , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_length == 1 )
				{
					fprintf( fp_fmc_h , "	char	*%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_length == 2 )
				{
					fprintf( fp_fmc_h , "	short	*%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_h , "	int	*%s ;\n" , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_h , "	float	*%s ;\n" , pfldinfo->field_name );
				}
				else if( pfldinfo->field_length == 8 )
				{
					fprintf( fp_fmc_h , "	double	*%s ;\n" , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_fmc_h , "	char	*%s ;\n" , pfldinfo->field_name );
			}
		}
		
		if( pmsginfo->sub_message_list )
		{
			nret = GenerateCCode_h( depth + 1 , pmsginfo->sub_message_list , fp_fmc_h ) ;
			if( nret )
				return nret;
		}
		
		if( depth == 0 )
		{
			fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "} FMCREF_%s ;\n" , pmsginfo->message_name );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "} %s ;\n" , pmsginfo->message_name );
			}
			else
			{
				fprintab( fp_fmc_h , depth ); fprintf( fp_fmc_h , "} %s [ %d ] ;\n" , pmsginfo->message_name , pmsginfo->array_size );
			}
		}
	}
	
	return 0;
}

static int GenerateCCode_c_BIND( int depth , struct MessageInfo *first_pmsginfo , FILE *fp_fmc_c , char *up_pathname )
{
	struct MessageInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_message )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->message_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->message_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintab( fp_fmc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_length == 1 )
				{
					fprintf( fp_fmc_c , "	%s%s=(char*)base; base+=%d;\n" , pathname , pfldinfo->field_name , pfldinfo->field_length );
				}
				else if( pfldinfo->field_length == 2 )
				{
					fprintf( fp_fmc_c , "	%s%s=(short*)base; base+=%d; (*(%s%s))=ntohs(*(%s%s));\n" , pathname , pfldinfo->field_name , pfldinfo->field_length , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_c , "	%s%s=(int*)base; base+=%d; (*(%s%s))=ntohl(*(%s%s));\n" , pathname , pfldinfo->field_name , pfldinfo->field_length , pathname , pfldinfo->field_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_c , "	%s%s=(float*)base; base+=%d;\n" , pathname , pfldinfo->field_name , pfldinfo->field_length );
				}
				else if( pfldinfo->field_length == 8 )
				{
					fprintf( fp_fmc_c , "	%s%s=(double*)base; base+=%d;\n" , pathname , pfldinfo->field_name , pfldinfo->field_length );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_fmc_c , "	%s%s=(char*)base; base+=%d+1;\n" , pathname , pfldinfo->field_name , pfldinfo->field_length );
			}
		}
		
		if( pmsginfo->sub_message_list )
		{
			nret = GenerateCCode_c_BIND( depth + 1 , pmsginfo->sub_message_list , fp_fmc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_UNBIND( int depth , struct MessageInfo *first_pmsginfo , FILE *fp_fmc_c , char *up_pathname )
{
	struct MessageInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_message )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname  , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->message_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->message_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintab( fp_fmc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_length == 1 )
				{
					fprintf( fp_fmc_c , "	base+=%d;\n" , pfldinfo->field_length );
				}
				else if( pfldinfo->field_length == 2 )
				{
					fprintf( fp_fmc_c , "	(*(short*)base)=htons(*(%s%s)); base+=%d;\n" , pathname , pfldinfo->field_name , pfldinfo->field_length );
				}
				else if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_c , "	(*(int*)base)=htonl(*(%s%s)); base+=%d;\n" , pathname , pfldinfo->field_name , pfldinfo->field_length );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_c , "	base+=%d;\n" , pfldinfo->field_length );
				}
				else if( pfldinfo->field_length == 8 )
				{
					fprintf( fp_fmc_c , "	base+=%d;\n" , pfldinfo->field_length );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_fmc_c , "	base+=%d+1;\n" , pfldinfo->field_length );
			}
		}
		
		if( pmsginfo->sub_message_list )
		{
			nret = GenerateCCode_c_UNBIND( depth + 1 , pmsginfo->sub_message_list , fp_fmc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_c_INIT( int depth , struct MessageInfo *first_pmsginfo , FILE *fp_fmc_c , char *up_pathname )
{
	struct MessageInfo	*pmsginfo = NULL ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_message )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->message_name );
			}
			else
			{
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->message_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			if( STRCMP( pfldinfo->init_default , == , "" ) )
				continue;
			
			fprintab( fp_fmc_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_length == 1 )
				{
					fprintf( fp_fmc_c , "	*(%s%s) = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_length == 2 )
				{
					fprintf( fp_fmc_c , "	*(%s%s) = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_c , "	*(%s%s) = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_c , "	*(%s%s) = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
				else if( pfldinfo->field_length == 8 )
				{
					fprintf( fp_fmc_c , "	*(%s%s) = %s ;\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_fmc_c , "	strcpy( %s%s , \"%s\" );\n" , pathname , pfldinfo->field_name , pfldinfo->init_default );
			}
		}
		
		if( pmsginfo->sub_message_list )
		{
			nret = GenerateCCode_c_INIT( depth + 1 , pmsginfo->sub_message_list , fp_fmc_c , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_c , depth ); fprintf( fp_fmc_c , "	}\n" );
		}
	}
	
	return 0;
}

static int GenerateCCode_LOG_c( int depth , struct MessageInfo *grandfather_pmsginfo , struct MessageInfo *first_pmsginfo , FILE *fp_fmc_LOG_c , char *LOG_up_pathname , char *up_pathname )
{
	struct MessageInfo	*pmsginfo = NULL ;
	char			LOG_pathname[ 1024 + 1 ] ;
	char			pathname[ 1024 + 1 ] ;
	struct FieldInfo	*pfldinfo = NULL ;
	
	int			nret = 0 ;
	
	for( pmsginfo = first_pmsginfo ; pmsginfo ; pmsginfo = pmsginfo->next_message )
	{
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_LOG_c , depth ); fprintf( fp_fmc_LOG_c , "	for( index[%d] = 0 ; index[%d] < %d ; index[%d]++ )\n" , depth , depth , pmsginfo->array_size , depth );
			fprintab( fp_fmc_LOG_c , depth ); fprintf( fp_fmc_LOG_c , "	{\n" );
		}
		
		if( depth == 0 )
		{
			strcpy( LOG_pathname , LOG_up_pathname );
			strcpy( pathname , up_pathname );
		}
		else
		{
			if( pmsginfo->array_size == 0 )
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s." , LOG_up_pathname , pmsginfo->message_name );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s." , up_pathname , pmsginfo->message_name );
			}
			else
			{
				SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s%s[index[%d]]." , LOG_up_pathname , pmsginfo->message_name , depth );
				SNPRINTF( pathname , sizeof(pathname)-1 , "%s%s[index[%d]]." , up_pathname , pmsginfo->message_name , depth );
			}
		}
		
		for( pfldinfo = pmsginfo->field_list ; pfldinfo ; pfldinfo = pfldinfo->next_field )
		{
			fprintab( fp_fmc_LOG_c , depth );
			
			if( STRCMP( pfldinfo->field_type , == , "INT" ) )
			{
				if( pfldinfo->field_length == 1 )
				{
					fprintf( fp_fmc_LOG_c , "	PREFIX_FMCLOG_%s \"%s%s[%%d]\" NEWLINE_FMCLOG_%s , *(%s%s) );\n" , grandfather_pmsginfo->message_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->message_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_length == 2 )
				{
					fprintf( fp_fmc_LOG_c , "	PREFIX_FMCLOG_%s \"%s%s[%%hd]\" NEWLINE_FMCLOG_%s , *(%s%s) );\n" , grandfather_pmsginfo->message_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->message_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_LOG_c , "	PREFIX_FMCLOG_%s \"%s%s[%%d]\" NEWLINE_FMCLOG_%s , *(%s%s) );\n" , grandfather_pmsginfo->message_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->message_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "FLOAT" ) )
			{
				if( pfldinfo->field_length == 4 )
				{
					fprintf( fp_fmc_LOG_c , "	PREFIX_FMCLOG_%s \"%s%s[%%f]\" NEWLINE_FMCLOG_%s , *(%s%s) );\n" , grandfather_pmsginfo->message_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->message_name , pathname , pfldinfo->field_name );
				}
				else if( pfldinfo->field_length == 8 )
				{
					fprintf( fp_fmc_LOG_c , "	PREFIX_FMCLOG_%s \"%s%s[%%lf]\" NEWLINE_FMCLOG_%s , *(%s%s) );\n" , grandfather_pmsginfo->message_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->message_name , pathname , pfldinfo->field_name );
				}
			}
			else if( STRCMP( pfldinfo->field_type , == , "STRING" ) )
			{
				fprintf( fp_fmc_LOG_c , "	PREFIX_FMCLOG_%s \"%s%s[%%s]\" NEWLINE_FMCLOG_%s , %s%s );\n" , grandfather_pmsginfo->message_name , LOG_pathname , pfldinfo->field_name , grandfather_pmsginfo->message_name , pathname , pfldinfo->field_name );
			}
		}
		
		if( pmsginfo->sub_message_list )
		{
			nret = GenerateCCode_LOG_c( depth + 1 , grandfather_pmsginfo , pmsginfo->sub_message_list , fp_fmc_LOG_c , LOG_pathname , pathname ) ;
			if( nret )
				return nret;
		}
		
		if( pmsginfo->array_size > 0 )
		{
			fprintab( fp_fmc_LOG_c , depth ); fprintf( fp_fmc_LOG_c , "	}\n" );
		}
	}
	
	return 0;
}

int GenerateCCode( struct CommandParameter *pcmdparam , struct MessageInfo *pmsginfo , FILE *fp_fmc_h , FILE *fp_fmc_c , FILE *fp_fmc_LOG_c )
{
	char		LOG_pathname[ 64 + 1 ] ;
	int		nret = 0 ;
	
	fprintf( fp_fmc_h , "#ifndef _H_%s_\n" , pmsginfo->message_name );
	fprintf( fp_fmc_h , "#define _H_%s_\n" , pmsginfo->message_name );
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "#include <stdio.h>\n" );
	fprintf( fp_fmc_h , "#include <stdlib.h>\n" );
	fprintf( fp_fmc_h , "#include <string.h>\n" );
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "#if ( defined __unix ) || ( defined __linux__ )\n" );
	fprintf( fp_fmc_h , "#ifndef _WINDLL_FUNC\n" );
	fprintf( fp_fmc_h , "#define _WINDLL_FUNC\n" );
	fprintf( fp_fmc_h , "#endif\n" );
	fprintf( fp_fmc_h , "#elif ( defined _WIN32 )\n" );
	fprintf( fp_fmc_h , "#ifndef _WINDLL_FUNC\n" );
	fprintf( fp_fmc_h , "#define _WINDLL_FUNC		_declspec(dllexport)\n" );
	fprintf( fp_fmc_h , "#endif\n" );
	fprintf( fp_fmc_h , "#endif\n" );
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "#if ( defined __unix ) | ( defined __linux__ )\n" );
	fprintf( fp_fmc_h , "#include <arpa/inet.h>\n" );
	fprintf( fp_fmc_h , "#elif ( defined _WIN32 )\n" );
	fprintf( fp_fmc_h , "#include \"winsock.h\"\n" );
	fprintf( fp_fmc_h , "#endif\n" );
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "#define FMCLEN_%s	%d\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_length );
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "typedef char FMCCOM_%s [%d] ;\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_length );
	fprintf( fp_fmc_h , "\n" );
	
	nret = GenerateCCode_h( 0 , pmsginfo->next_message , fp_fmc_h ) ;
	if( nret )
		return nret;
	
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "_WINDLL_FUNC int FMCBIND_%s( FMCCOM_%s *p , long comlen , FMCREF_%s *pst );\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_h , "_WINDLL_FUNC int FMCUNBIND_%s( FMCREF_%s *pst , FMCCOM_%s *p , long *p_comlen );\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "_WINDLL_FUNC int FMCINIT_%s( FMCCOM_%s *p , FMCREF_%s *pst );\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_h , "\n" );
	fprintf( fp_fmc_h , "#endif\n" );
	
	fprintf( fp_fmc_c , "#include \"%s.h\"\n" , pcmdparam->pathfilename );
	fprintf( fp_fmc_c , "\n" );
	fprintf( fp_fmc_c , "int FMCBIND_%s( FMCCOM_%s *p , long comlen , FMCREF_%s *pst )\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_c , "{\n" );
	fprintf( fp_fmc_c , "	char	*base = (char*)p ;\n" );
	fprintf( fp_fmc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , FMC_MAXDEPTH );
	fprintf( fp_fmc_c , "	if( comlen != FMCLEN_%s )\n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_c , "		return -1;\n" );
	
	nret = GenerateCCode_c_BIND( 0 , pmsginfo->next_message , fp_fmc_c , "pst->" ) ;
	if( nret )
		return nret;
	
	fprintf( fp_fmc_c , "	return 0;\n" );
	fprintf( fp_fmc_c , "}\n" );
	
	fprintf( fp_fmc_c , "\n" );
	fprintf( fp_fmc_c , "int FMCUNBIND_%s( FMCREF_%s *pst , FMCCOM_%s *p , long *p_comlen )\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_c , "{\n" );
	fprintf( fp_fmc_c , "	char	*base = (char*)p ;\n" );
	fprintf( fp_fmc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , FMC_MAXDEPTH );
	fprintf( fp_fmc_c , "	if( p_comlen && (*p_comlen) < FMCLEN_%s )\n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_c , "		return -1;\n" );
	
	nret = GenerateCCode_c_UNBIND( 0 , pmsginfo->next_message , fp_fmc_c , "pst->" ) ;
	if( nret )
		return nret;
	
	fprintf( fp_fmc_c , "	if( p_comlen )\n" );
	fprintf( fp_fmc_c , "		(*p_comlen) = FMCLEN_%s ;\n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_c , "	return 0;\n" );
	fprintf( fp_fmc_c , "}\n" );
	
	fprintf( fp_fmc_c , "\n" );
	fprintf( fp_fmc_c , "int FMCINIT_%s( FMCCOM_%s *p , FMCREF_%s *pst )\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_c , "{\n" );
	fprintf( fp_fmc_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , FMC_MAXDEPTH );
	fprintf( fp_fmc_c , "	memset( p , 0x00 , sizeof(FMCCOM_%s) );\n" , pmsginfo->next_message->message_name );
	
	nret = GenerateCCode_c_INIT( 0 , pmsginfo->next_message , fp_fmc_c , "pst->" ) ;
	if( nret )
		return nret;
	
	fprintf( fp_fmc_c , "	return 0;\n" );
	fprintf( fp_fmc_c , "}\n" );
	
	fprintf( fp_fmc_LOG_c , "#include \"%s.h\"\n" , pcmdparam->pathfilename );
	fprintf( fp_fmc_LOG_c , "\n" );
	fprintf( fp_fmc_LOG_c , "#ifndef FUNCNAME_FMCLOG_%s\n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_LOG_c , "#define FUNCNAME_FMCLOG_%s	FMCLOG_%s\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_LOG_c , "#endif\n" );
	fprintf( fp_fmc_LOG_c , "\n" );
	fprintf( fp_fmc_LOG_c , "#ifndef PREFIX_FMCLOG_%s\n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_LOG_c , "#define PREFIX_FMCLOG_%s	printf( \n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_LOG_c , "#endif\n" );
	fprintf( fp_fmc_LOG_c , "\n" );
	fprintf( fp_fmc_LOG_c , "#ifndef NEWLINE_FMCLOG_%s\n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_LOG_c , "#define NEWLINE_FMCLOG_%s	\"\\n\"\n" , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_LOG_c , "#endif\n" );
	fprintf( fp_fmc_LOG_c , "\n" );
	fprintf( fp_fmc_LOG_c , "int FUNCNAME_FMCLOG_%s( FMCREF_%s *pst )\n" , pmsginfo->next_message->message_name , pmsginfo->next_message->message_name );
	fprintf( fp_fmc_LOG_c , "{\n" );
	fprintf( fp_fmc_LOG_c , "	int	index[%d] = { 0 } ; index[0] = 0 ;\n" , FMC_MAXDEPTH );
	
	SNPRINTF( LOG_pathname , sizeof(LOG_pathname)-1 , "%s." , pmsginfo->next_message->message_name );
	nret = GenerateCCode_LOG_c( 0 , pmsginfo->next_message , pmsginfo->next_message , fp_fmc_LOG_c , LOG_pathname , "pst->" ) ;
	if( nret )
		return nret;
	
	fprintf( fp_fmc_LOG_c , "	return 0;\n" );
	fprintf( fp_fmc_LOG_c , "}\n" );
	
	return 0;
}
