#include "test.fmc.h"

#ifndef FUNCNAME_FMCLOG_BankTransaction
#define FUNCNAME_FMCLOG_BankTransaction	FMCLOG_BankTransaction
#endif

#ifndef PREFIX_FMCLOG_BankTransaction
#define PREFIX_FMCLOG_BankTransaction	printf( 
#endif

#ifndef NEWLINE_FMCLOG_BankTransaction
#define NEWLINE_FMCLOG_BankTransaction	"\n"
#endif

int FUNCNAME_FMCLOG_BankTransaction( FMCREF_BankTransaction *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	PREFIX_FMCLOG_BankTransaction "BankTransaction.version[%d]" NEWLINE_FMCLOG_BankTransaction , *(pst->version) );
		PREFIX_FMCLOG_BankTransaction "BankTransaction.ResponseHeader.transaction_code[%s]" NEWLINE_FMCLOG_BankTransaction , pst->ResponseHeader.transaction_code );
		PREFIX_FMCLOG_BankTransaction "BankTransaction.ResponseHeader.trans_jnlsno[%d]" NEWLINE_FMCLOG_BankTransaction , *(pst->ResponseHeader.trans_jnlsno) );
		PREFIX_FMCLOG_BankTransaction "BankTransaction.ResponseHeader.response_code[%d]" NEWLINE_FMCLOG_BankTransaction , *(pst->ResponseHeader.response_code) );
		PREFIX_FMCLOG_BankTransaction "BankTransaction.ResponseHeader.response_desc[%s]" NEWLINE_FMCLOG_BankTransaction , pst->ResponseHeader.response_desc );
			for( index[2] = 0 ; index[2] < 3 ; index[2]++ )
			{
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.AddonMessages[index[2]].message_text[%s]" NEWLINE_FMCLOG_BankTransaction , pst->QueryTransactionDetails.AddonMessages[index[2]].message_text );
			}
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetailTitle.title_text[%s]" NEWLINE_FMCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetailTitle.title_text );
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetailTitle.page_no[%hd]" NEWLINE_FMCLOG_BankTransaction , *(pst->QueryTransactionDetails.TransactionDetailTitle.page_no) );
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetailTitle.page_size[%hd]" NEWLINE_FMCLOG_BankTransaction , *(pst->QueryTransactionDetails.TransactionDetailTitle.page_size) );
			for( index[2] = 0 ; index[2] < 10 ; index[2]++ )
			{
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails[index[2]].trans_date[%s]" NEWLINE_FMCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails[index[2]].trans_date );
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails[index[2]].trans_time[%s]" NEWLINE_FMCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails[index[2]].trans_time );
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails[index[2]].outlet_id[%s]" NEWLINE_FMCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails[index[2]].outlet_id );
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails[index[2]].card_no[%s]" NEWLINE_FMCLOG_BankTransaction , pst->QueryTransactionDetails.TransactionDetails[index[2]].card_no );
			PREFIX_FMCLOG_BankTransaction "BankTransaction.QueryTransactionDetails.TransactionDetails[index[2]].trans_amount[%f]" NEWLINE_FMCLOG_BankTransaction , *(pst->QueryTransactionDetails.TransactionDetails[index[2]].trans_amount) );
			}
	return 0;
}
