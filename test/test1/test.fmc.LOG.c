#include "test.fmc.h"

#ifndef FUNCNAME_FMCLOG_TestMessage
#define FUNCNAME_FMCLOG_TestMessage	FMCLOG_TestMessage
#endif

#ifndef PREFIX_FMCLOG_TestMessage
#define PREFIX_FMCLOG_TestMessage	printf( 
#endif

#ifndef NEWLINE_FMCLOG_TestMessage
#define NEWLINE_FMCLOG_TestMessage	"\n"
#endif

int FUNCNAME_FMCLOG_TestMessage( FMCREF_TestMessage *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	PREFIX_FMCLOG_TestMessage "TestMessage.version[%d]" NEWLINE_FMCLOG_TestMessage , *(pst->version) );
	PREFIX_FMCLOG_TestMessage "TestMessage.name[%s]" NEWLINE_FMCLOG_TestMessage , pst->name );
	PREFIX_FMCLOG_TestMessage "TestMessage.oper_number[%hd]" NEWLINE_FMCLOG_TestMessage , *(pst->oper_number) );
	PREFIX_FMCLOG_TestMessage "TestMessage.library_card_number[%d]" NEWLINE_FMCLOG_TestMessage , *(pst->library_card_number) );
	PREFIX_FMCLOG_TestMessage "TestMessage.amount[%f]" NEWLINE_FMCLOG_TestMessage , *(pst->amount) );
	PREFIX_FMCLOG_TestMessage "TestMessage.balance[%lf]" NEWLINE_FMCLOG_TestMessage , *(pst->balance) );
	PREFIX_FMCLOG_TestMessage "TestMessage.remark[%s]" NEWLINE_FMCLOG_TestMessage , pst->remark );
	return 0;
}
