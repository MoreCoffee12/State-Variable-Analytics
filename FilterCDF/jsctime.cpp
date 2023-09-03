// JSCTime.cpp: implementation of the CJSCTime class.
//
//////////////////////////////////////////////////////////////////////

#include "JSCTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJSCTime::CJSCTime()
{
	//Initialize class members
	strcpy_s(timestring, "");
	year	= 0;
	day = 0;
	hour = 0;
	minute = 0;
	second = 0;
	abstime = 0;

}

CJSCTime::~CJSCTime()
{

}

//////////////////////////////////////////////////////////////////////
// This function converts the string to years, days, 
// etc. and also calculates the absolute time.
//////////////////////////////////////////////////////////////////////
bool CJSCTime::ConvertText2Time()
{
	//local variables
	char	text[16];
     errno_t err;

	//Initialize local variables
	err = strcpy_s(text, "");

	//try the conversion of year
	err = strncpy_s(text, timestring, 4);
     if( err !=0 )
          return false;
	year	= atoi(text);

	//validate the year
	if( year < 1990 )
		return false;

	//try the conversion of day
	err = strncpy_s(text, _countof(text), timestring+5, 3);
     if( err !=0 )
          return false;
	day = atoi(text);

	//validate the day
	if( ( day < 1 ) || ( day > 365 ) )
		return false;

	//try the conversion of hours
	err = strncpy_s(text, _countof(text), timestring+9, 2);
     if( err !=0 )
          return false;
	hour	= atoi(text);

	//validate the year
	if( ( hour < 0 ) || ( hour > 24 ) )
		return false;

	//try the conversion of minutes
	err = strncpy_s(text, _countof(text), timestring+12, 2);
     if( err !=0 )
          return false;
	minute = atoi(text);

	//validate the year
	if( ( minute < 0 ) || ( minute > 60 ) )
		return false;

	//try the conversion of minutes
	err = strncpy_s(text, _countof(text), timestring+15, 2);
     if( err !=0 )
          return false;
	second = atoi(text);

	//validate the year
	if( ( second < 0 ) || ( second > 60 ) )
		return false;

	//calculate the absolute time
	abstime = (double)(second + (minute * 60)+ (hour * 3600) + (day * 24 * 3600) + ((year-1990) * 365 * 24 * 3600));

	//success
	return true;
}

//////////////////////////////////////////////////////////////////////
// This method converts absolute time to years,
// days, etc and to the string
//////////////////////////////////////////////////////////////////////
bool CJSCTime::ConvertTime2Text()
{
	//local variables
	div_t div_result;
	char temp[MAXTIMESTRING];
     errno_t err;

	//Parse out the year
	div_result = div((int)abstime, (365 * 24 * 3600));
	year	= div_result.quot + 1990;
	err = _itoa_s(year, timestring, MAXTIMESTRING, 10);
     if( err !=0 )
          return false;

	//Parse out the day
	div_result	= div(div_result.rem, (24 * 3600));
	day			= div_result.quot;
	err = _itoa_s(day, temp, MAXTIMESTRING, 10);
     if( err !=0 )
          return false;
	strcat_s(timestring, "/");
	if( day<100 )
		strcat_s(timestring, "0");
	if( day<10 )
		strcat_s(timestring, "0");
	strcat_s(timestring, temp);
	
	//Parse out the hour
	div_result = div(div_result.rem, 3600);
	hour	= div_result.quot;
	err = _itoa_s(hour, temp, MAXTIMESTRING, 10);
     if( err !=0 )
          return false;
	strcat_s(timestring, ":");
	if( hour<10 )
		strcat_s(timestring, "0");
	strcat_s(timestring, temp);
	
	//Parse out the minute
	div_result	= div(div_result.rem, 60);
	minute		= div_result.quot;
	err = _itoa_s(minute, temp, MAXTIMESTRING, 10);
     if( err !=0 )
          return false;
	strcat_s(timestring, ":");
	if( minute<10 )
		strcat_s(timestring, "0");
	strcat_s(timestring, temp);
	
	err = _itoa_s(div_result.rem, temp, MAXTIMESTRING, 10);
     if( err !=0 )
          return false;
	strcat_s(timestring, ":");
	if( div_result.rem<10 )
		strcat_s(timestring, "0");
	strcat_s(timestring, temp);
	
	//success
	return true;
}
