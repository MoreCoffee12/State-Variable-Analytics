// JSCTime.h: interface for the CJSCTime class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifndef _CJSCTIME
#define _CJSCTIME

class CJSCTime  
{
public:

	/*Variables
	******************************************************************/
	char			timestring[20];
	int				year;
	int				day;
	int				hour;
	int				minute;
	int				second;
	double			abstime;

	/*Functions
	******************************************************************/
					CJSCTime();
	virtual			~CJSCTime();
	bool			ConvertText2Time();
	bool			ConvertTime2Text();

};

#endif

