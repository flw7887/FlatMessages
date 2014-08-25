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

#define FMCLEN_BankTransaction	2938

typedef char FMCCOM_BankTransaction [2938] ;

#pragma pack(4)

typedef struct
{
	char	*version ;
	struct
	{
		char	*transaction_code ;
		int	*trans_jnlsno ;
		int	*response_code ;
		char	*response_desc ;
	} ResponseHeader ;
	struct
	{
		struct
		{
			char	*message_text ;
		} AddonMessages [ 3 ] ;
		struct
		{
			char	*title_text ;
			short	*page_no ;
			short	*page_size ;
		} TransactionDetailTitle ;
		struct
		{
			char	*trans_date ;
			char	*trans_time ;
			char	*outlet_id ;
			char	*card_no ;
			float	*trans_amount ;
		} TransactionDetails [ 10 ] ;
	} QueryTransactionDetails ;
} FMCREF_BankTransaction ;

_WINDLL_FUNC int FMCBIND_BankTransaction( FMCCOM_BankTransaction *p , long comlen , FMCREF_BankTransaction *pst );
_WINDLL_FUNC int FMCUNBIND_BankTransaction( FMCREF_BankTransaction *pst , FMCCOM_BankTransaction *p , long *p_comlen );

_WINDLL_FUNC int FMCINIT_BankTransaction( FMCCOM_BankTransaction *p , FMCREF_BankTransaction *pst );

#endif
