#include "logfile.h"

/**********************
* CLogFile Constructor *
*---------------------**************************
* Opens the log file using the name passed in. *
***********************************************/
CLogFile::CLogFile(char *logFileName)
{
	logFile = fopen(logFileName, "w");
}


/*****************
* CLogFile Append *
*----------------****************************************************
* Appends messages to log file defined in constructor for debugging *
* and tracking program execution.									*
********************************************************************/
void CLogFile::Append(LPCSTR inString, ...)
{
	va_list Marker;
    char bufferString[1024];

	//Retreive system data and time
	GetLocalTime(&SysTime); 

	// Compile string sent from user
    va_start(Marker, inString);
    vsprintf(bufferString, inString, Marker);
    va_end(Marker);
	
	fprintf(logFile, "\n%i:%i:%i:%i - %s", 
		(int)SysTime.wHour,
		(int)SysTime.wMinute,
		(int)SysTime.wSecond,
		(int)SysTime.wMilliseconds,
		bufferString);
	fflush(logFile);
}

/*********************
* CLogFile Destructor *
*--------------------**
* Closes the logfile. *
**********************/
CLogFile::~CLogFile()
{
	fclose(logFile);
}
