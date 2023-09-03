// FilterCDF1.h: interface for the CFilterCDF class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsctime.h"

#define MAXSTRING 8388608
#ifndef _CFILTERCDF
#define _CFILTERCDF

class CFilterCDF  
{
public:

	// Memebers
	////////////////////////////////////////////////////////////////////////
	CJSCTime	jsctime;

	// Methods
	////////////////////////////////////////////////////////////////////////
	CFilterCDF();
	virtual ~CFilterCDF();

     bool bGetColCount( int *iColCount );
     bool bGetColAsDoubleArray( double *dCDFArray, long lMaxCDFArrayCount, long lCDFColumn, long *lRowsFound );
     bool bGetRowAsDoubleArray( double *dCDFArray, long lMaxCDFArrayCount, long lCDFRow, long *lColsFound );
     bool bGetRecordAsString( long lCol, long lRow, char *pszRecordString );
     bool bGetMaxStringCount( int *iMaxString );
     bool bGetRowCount( int *iRowCount );
     bool bSetStringData (char *szString );


     int GetCols(char *pstrFileName);
	int GetRows(char *pstrFileName, int ColCount);

	bool	IsJSCTime(char *pstrFileName);
	bool	GetJSCTime(char *pstrFileName, double *abstimearray);

	bool	LoadData(char *pstrFileName, int RecNo);
	bool	LoadData(char *pstrFileName, double *cdfarray, int RecCount);
	bool	LoadData(char *pstrFileName, 
							double *timearray, 
							double *cdfarray, 
							int RecCount);
	bool	LoadData(char *pstrFileName, 
							double *timearray, 
							double *cdfarray);


private:

	// members
	////////////////////////////////////////////////////////////////////////
	FILE	*stream;
	char	szField[2048];
     char szCSVString[MAXSTRING];
     long lCSVStringLength;
     long lColCount;
     long lRowCount;

     // methods
	////////////////////////////////////////////////////////////////////////
     bool bCharacterizeString();
};

#endif
