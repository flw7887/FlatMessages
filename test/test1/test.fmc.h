#ifndef _H__
#define _H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if ( defined __unix ) || ( defined __linux__ )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC
#endif
#elif ( defined _WIN32 )
#ifndef _WINDLL_FUNC
#define _WINDLL_FUNC		_declspec(dllexport)
#endif
#endif

#if ( defined __unix ) | ( defined __linux__ )
#include <arpa/inet.h>
#elif ( defined _WIN32 )
#include "winsock.h"
#endif

#define FMCLEN_TestMessage	181

typedef char FMCCOM_TestMessage [181] ;

#pragma pack(4)

typedef struct
{
	char	*version ;
	char	*name ;
	short	*oper_number ;
	int	*library_card_number ;
	float	*amount ;
	double	*balance ;
	char	*remark ;
} FMCREF_TestMessage ;

_WINDLL_FUNC int FMCBIND_TestMessage( FMCCOM_TestMessage *p , long comlen , FMCREF_TestMessage *pst );
_WINDLL_FUNC int FMCUNBIND_TestMessage( FMCREF_TestMessage *pst , FMCCOM_TestMessage *p , long *p_comlen );

_WINDLL_FUNC int FMCINIT_TestMessage( FMCCOM_TestMessage *p , FMCREF_TestMessage *pst );

#endif
