#include "test.fmc.h"

int FMCBIND_BankTransaction( FMCCOM_BankTransaction *p , long comlen , FMCREF_BankTransaction *pst )
{
	char	*base = (char*)p ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	if( comlen != FMCLEN_BankTransaction )
		return -1;
	pst->version=(char*)base; base+=1;
		pst->ResponseHeader.transaction_code=(char*)base; base+=32+1;
		pst->ResponseHeader.trans_jnlsno=(int*)base; base+=4; (*(pst->ResponseHeader.trans_jnlsno))=ntohl(*(pst->ResponseHeader.trans_jnlsno));
		pst->ResponseHeader.response_code=(int*)base; base+=4; (*(pst->ResponseHeader.response_code))=ntohl(*(pst->ResponseHeader.response_code));
		pst->ResponseHeader.response_desc=(char*)base; base+=1024+1;
			for( index[2] = 0 ; index[2] < 3 ; index[2]++ )
			{
			pst->QueryTransactionDetails.AddonMessages[index[2]].message_text=(char*)base; base+=64+1;
			}
			pst->QueryTransactionDetails.TransactionDetailTitle.title_text=(char*)base; base+=64+1;
			pst->QueryTransactionDetails.TransactionDetailTitle.page_no=(short*)base; base+=2; (*(pst->QueryTransactionDetails.TransactionDetailTitle.page_no))=ntohs(*(pst->QueryTransactionDetails.TransactionDetailTitle.page_no));
			pst->QueryTransactionDetails.TransactionDetailTitle.page_size=(short*)base; base+=2; (*(pst->QueryTransactionDetails.TransactionDetailTitle.page_size))=ntohs(*(pst->QueryTransactionDetails.TransactionDetailTitle.page_size));
			for( index[2] = 0 ; index[2] < 10 ; index[2]++ )
			{
			pst->QueryTransactionDetails.TransactionDetails[index[2]].trans_date=(char*)base; base+=10+1;
			pst->QueryTransactionDetails.TransactionDetails[index[2]].trans_time=(char*)base; base+=10+1;
			pst->QueryTransactionDetails.TransactionDetails[index[2]].outlet_id=(char*)base; base+=6+1;
			pst->QueryTransactionDetails.TransactionDetails[index[2]].card_no=(char*)base; base+=20+1;
			pst->QueryTransactionDetails.TransactionDetails[index[2]].trans_amount=(float*)base; base+=4;
			}
	return 0;
}

int FMCUNBIND_BankTransaction( FMCREF_BankTransaction *pst , FMCCOM_BankTransaction *p , long *p_comlen )
{
	char	*base = (char*)p ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	if( p_comlen && (*p_comlen) < FMCLEN_BankTransaction )
		return -1;
	base+=1;
		base+=32+1;
		(*(int*)base)=htonl(*(pst->ResponseHeader.trans_jnlsno)); base+=4;
		(*(int*)base)=htonl(*(pst->ResponseHeader.response_code)); base+=4;
		base+=1024+1;
			for( index[2] = 0 ; index[2] < 3 ; index[2]++ )
			{
			base+=64+1;
			}
			base+=64+1;
			(*(short*)base)=htons(*(pst->QueryTransactionDetails.TransactionDetailTitle.page_no)); base+=2;
			(*(short*)base)=htons(*(pst->QueryTransactionDetails.TransactionDetailTitle.page_size)); base+=2;
			for( index[2] = 0 ; index[2] < 10 ; index[2]++ )
			{
			base+=10+1;
			base+=10+1;
			base+=6+1;
			base+=20+1;
			base+=4;
			}
	if( p_comlen )
		(*p_comlen) = FMCLEN_BankTransaction ;
	return 0;
}

int FMCINIT_BankTransaction( FMCCOM_BankTransaction *p , FMCREF_BankTransaction *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	memset( p , 0x00 , sizeof(FMCCOM_BankTransaction) );
	*(pst->version) = 1 ;
		*(pst->ResponseHeader.response_code) = 0 ;
		strcpy( pst->ResponseHeader.response_desc , "OK" );
			for( index[2] = 0 ; index[2] < 3 ; index[2]++ )
			{
			}
			strcpy( pst->QueryTransactionDetails.TransactionDetailTitle.title_text , "MY TITLE" );
			*(pst->QueryTransactionDetails.TransactionDetailTitle.page_no) = 1 ;
			for( index[2] = 0 ; index[2] < 10 ; index[2]++ )
			{
			strcpy( pst->QueryTransactionDetails.TransactionDetails[index[2]].outlet_id , "1001" );
			}
	return 0;
}
