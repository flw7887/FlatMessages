#include "test.fmc.h"

int FMCBIND_TestMessage( FMCCOM_TestMessage *p , long comlen , FMCREF_TestMessage *pst )
{
	char	*base = (char*)p ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	if( comlen != FMCLEN_TestMessage )
		return -1;
	pst->version=(char*)base; base+=1;
	pst->name=(char*)base; base+=32+1;
	pst->oper_number=(short*)base; base+=2; (*(pst->oper_number))=ntohs(*(pst->oper_number));
	pst->library_card_number=(int*)base; base+=4; (*(pst->library_card_number))=ntohl(*(pst->library_card_number));
	pst->amount=(float*)base; base+=4;
	pst->balance=(double*)base; base+=8;
	pst->remark=(char*)base; base+=128+1;
	return 0;
}

int FMCUNBIND_TestMessage( FMCREF_TestMessage *pst , FMCCOM_TestMessage *p , long *p_comlen )
{
	char	*base = (char*)p ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	if( p_comlen && (*p_comlen) < FMCLEN_TestMessage )
		return -1;
	base+=1;
	base+=32+1;
	(*(short*)base)=htons(*(pst->oper_number)); base+=2;
	(*(int*)base)=htonl(*(pst->library_card_number)); base+=4;
	base+=4;
	base+=8;
	base+=128+1;
	if( p_comlen )
		(*p_comlen) = FMCLEN_TestMessage ;
	return 0;
}

int FMCINIT_TestMessage( FMCCOM_TestMessage *p , FMCREF_TestMessage *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	memset( p , 0x00 , sizeof(FMCCOM_TestMessage) );
	return 0;
}
