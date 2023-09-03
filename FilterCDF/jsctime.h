// JSCTime.h: interface for the CJSCTime class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXTIMESTRING 20
#ifndef _CJSCTIME
#define _CJSCTIME

class CJSCTime  
{
public:

	// Members
     //////////////////////////////////////////////////////////////////////
	char	timestring[MAXTIMESTRING];
	int year;
	int day;
	int hour;
	int minute;
	int second;
	double abstime;

	// Methods
     //////////////////////////////////////////////////////////////////////
					CJSCTime();
	virtual ~CJSCTime();
	bool	ConvertText2Time();
	bool	ConvertTime2Text();

};

#endif

