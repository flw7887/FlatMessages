#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.fmc.h"

#include "test.fmc.LOG.c"

int test()
{
	FMCCOM_BankTransaction	com ;
	FMCREF_BankTransaction	ref ;
	long			comlen ;
	
	memset( & com , 0x00 , sizeof(FMCCOM_BankTransaction) );
	
	memset( & ref , 0x00 , sizeof(FMCREF_BankTransaction) );
	comlen = FMCLEN_BankTransaction ;
	FMCBIND_BankTransaction( & com , comlen , & ref );
	*(ref.version) = 1 ;
	strcpy( ref.ResponseHeader.transaction_code , "P0101" );
	*(ref.ResponseHeader.trans_jnlsno) = 12345678 ;
	*(ref.ResponseHeader.response_code) = 0 ;
	strcpy( ref.ResponseHeader.response_desc , "OK" );
	
	strcpy( ref.QueryTransactionDetails.AddonMessages[1].message_text , "Check channel pass" );
	strcpy( ref.QueryTransactionDetails.AddonMessages[0].message_text , "Check account pass" );
	
	strcpy( ref.QueryTransactionDetails.TransactionDetailTitle.title_text , "DETAIL TITLE" );
	*(ref.QueryTransactionDetails.TransactionDetailTitle.page_no) = 1 ;
	*(ref.QueryTransactionDetails.TransactionDetailTitle.page_size) = 2 ;
	
	strcpy( ref.QueryTransactionDetails.TransactionDetails[0].trans_date , "2014-01-01" );
	strcpy( ref.QueryTransactionDetails.TransactionDetails[0].trans_time , "08:01:01" );
	strcpy( ref.QueryTransactionDetails.TransactionDetails[0].outlet_id , "1001" );
	strcpy( ref.QueryTransactionDetails.TransactionDetails[0].card_no , "603367123412341234" );
	*(ref.QueryTransactionDetails.TransactionDetails[0].trans_amount) = 100.00 ;
	
	strcpy( ref.QueryTransactionDetails.TransactionDetails[1].trans_date , "2014-02-02" );
	strcpy( ref.QueryTransactionDetails.TransactionDetails[1].trans_time , "08:02:02" );
	strcpy( ref.QueryTransactionDetails.TransactionDetails[1].outlet_id , "2002" );
	strcpy( ref.QueryTransactionDetails.TransactionDetails[1].card_no , "603367123412341234" );
	*(ref.QueryTransactionDetails.TransactionDetails[1].trans_amount) = 200.00 ;
	
	FMCUNBIND_BankTransaction( & ref , & com , & comlen );
	
	memset( & ref , 0x00 , sizeof(FMCREF_BankTransaction) );
	FMCBIND_BankTransaction( & com , comlen , & ref );
	
	FMCLOG_BankTransaction( & ref );
	
	FMCINIT_BankTransaction( & com , & ref );
	
	FMCLOG_BankTransaction( & ref );
	
	return 0;
}

int main()
{
	return -test();
}
