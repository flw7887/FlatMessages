#include "test.fmc.h"

int FMCBIND_PersonalInfoList( FMCCOM_PersonalInfoList *p , long comlen , FMCREF_PersonalInfoList *pst )
{
	char	*base = (char*)p ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	if( comlen != FMCLEN_PersonalInfoList )
		return -1;
		for( index[1] = 0 ; index[1] < 37 ; index[1]++ )
		{
		pst->PersonalInfo[index[1]].id=(int*)base; base+=4; (*(pst->PersonalInfo[index[1]].id))=ntohl(*(pst->PersonalInfo[index[1]].id));
		pst->PersonalInfo[index[1]].name=(char*)base; base+=64+1;
		pst->PersonalInfo[index[1]].age=(char*)base; base+=1;
		pst->PersonalInfo[index[1]].gender=(char*)base; base+=1+1;
		pst->PersonalInfo[index[1]].phone_num=(int*)base; base+=4; (*(pst->PersonalInfo[index[1]].phone_num))=ntohl(*(pst->PersonalInfo[index[1]].phone_num));
		}
	return 0;
}

int FMCUNBIND_PersonalInfoList( FMCREF_PersonalInfoList *pst , FMCCOM_PersonalInfoList *p , long *p_comlen )
{
	char	*base = (char*)p ;
	int	index[10] = { 0 } ; index[0] = 0 ;
	if( p_comlen && (*p_comlen) < FMCLEN_PersonalInfoList )
		return -1;
		for( index[1] = 0 ; index[1] < 37 ; index[1]++ )
		{
		(*(int*)base)=htonl(*(pst->PersonalInfo[index[1]].id)); base+=4;
		base+=64+1;
		base+=1;
		base+=1+1;
		(*(int*)base)=htonl(*(pst->PersonalInfo[index[1]].phone_num)); base+=4;
		}
	if( p_comlen )
		(*p_comlen) = FMCLEN_PersonalInfoList ;
	return 0;
}

int FMCINIT_PersonalInfoList( FMCCOM_PersonalInfoList *p , FMCREF_PersonalInfoList *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
	memset( p , 0x00 , sizeof(FMCCOM_PersonalInfoList) );
		for( index[1] = 0 ; index[1] < 37 ; index[1]++ )
		{
		strcpy( pst->PersonalInfo[index[1]].gender , "M" );
		}
	return 0;
}
