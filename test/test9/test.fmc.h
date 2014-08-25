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

#define FMCLEN_PersonalInfoList	2812

typedef char FMCCOM_PersonalInfoList [2812] ;

#pragma pack(4)

typedef struct
{
	struct
	{
		int	*id ;
		char	*name ;
		char	*age ;
		char	*gender ;
		int	*phone_num ;
	} PersonalInfo [ 37 ] ;
} FMCREF_PersonalInfoList ;

_WINDLL_FUNC int FMCBIND_PersonalInfoList( FMCCOM_PersonalInfoList *p , long comlen , FMCREF_PersonalInfoList *pst );
_WINDLL_FUNC int FMCUNBIND_PersonalInfoList( FMCREF_PersonalInfoList *pst , FMCCOM_PersonalInfoList *p , long *p_comlen );

_WINDLL_FUNC int FMCINIT_PersonalInfoList( FMCCOM_PersonalInfoList *p , FMCREF_PersonalInfoList *pst );

#endif
