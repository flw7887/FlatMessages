#include "test.fmc.h"

#ifndef FUNCNAME_FMCLOG_PersonalInfoList
#define FUNCNAME_FMCLOG_PersonalInfoList	FMCLOG_PersonalInfoList
#endif

#ifndef PREFIX_FMCLOG_PersonalInfoList
#define PREFIX_FMCLOG_PersonalInfoList	printf( 
#endif

#ifndef NEWLINE_FMCLOG_PersonalInfoList
#define NEWLINE_FMCLOG_PersonalInfoList	"\n"
#endif

int FUNCNAME_FMCLOG_PersonalInfoList( FMCREF_PersonalInfoList *pst )
{
	int	index[10] = { 0 } ; index[0] = 0 ;
		for( index[1] = 0 ; index[1] < 37 ; index[1]++ )
		{
		PREFIX_FMCLOG_PersonalInfoList "PersonalInfoList.PersonalInfo[index[1]].id[%d]" NEWLINE_FMCLOG_PersonalInfoList , *(pst->PersonalInfo[index[1]].id) );
		PREFIX_FMCLOG_PersonalInfoList "PersonalInfoList.PersonalInfo[index[1]].name[%s]" NEWLINE_FMCLOG_PersonalInfoList , pst->PersonalInfo[index[1]].name );
		PREFIX_FMCLOG_PersonalInfoList "PersonalInfoList.PersonalInfo[index[1]].age[%d]" NEWLINE_FMCLOG_PersonalInfoList , *(pst->PersonalInfo[index[1]].age) );
		PREFIX_FMCLOG_PersonalInfoList "PersonalInfoList.PersonalInfo[index[1]].gender[%s]" NEWLINE_FMCLOG_PersonalInfoList , pst->PersonalInfo[index[1]].gender );
		PREFIX_FMCLOG_PersonalInfoList "PersonalInfoList.PersonalInfo[index[1]].phone_num[%d]" NEWLINE_FMCLOG_PersonalInfoList , *(pst->PersonalInfo[index[1]].phone_num) );
		}
	return 0;
}
