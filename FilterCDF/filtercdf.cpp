// FilterCDF.cpp: implementation of the CFilterCDF class.
//
// This class supports basic interface to comma delimited files
//////////////////////////////////////////////////////////////////////
//
// Revision, 2 Sep 2023
// Updated for safe string handling; updated to Visual Studio 2022
// B. Howard
//
//////////////////////////////////////////////////////////////////////

#include "filtercdf.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilterCDF::CFilterCDF()
{
	//Initialize object members
    stream = NULL;
    strcpy_s( szField, sizeof(szField), "" );
    strcpy_s( szCSVString, sizeof(szCSVString), "" );
    lCSVStringLength = strlen( szCSVString );
    lColCount = 0;
    lRowCount = 0;

}

CFilterCDF::~CFilterCDF()
{

}

//////////////////////////////////////////////////////////////////////
// Update charasteristics of CSV data
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bCharacterizeString()
{
     //locals
	char	s=',';
	int k;
	int ch;
	int RecNo;
     long lCSVStringIndex;
	bool CommaFlag;
	bool bEndOfLineFlag;

     //initialize locals
     lCSVStringIndex = -1;
	k = 0;
     RecNo = 0;
	CommaFlag	= true;
	bEndOfLineFlag = false;

     //loop through the string
     do
	{

		do
		{
               lCSVStringIndex++;
               ch = szCSVString[lCSVStringIndex];

			//Check to see if it is a comma or end of line
			if(((char)ch == ',') || ((char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
			{
				RecNo++;
				CommaFlag = false;
				szField[k] = '\0';
				if( ( (char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
                    {
					//is this the first end of line flag?
                         if( !bEndOfLineFlag )
                         {    
                              bEndOfLineFlag = true;
                              lColCount = RecNo;
                         }
                    }

                    k = 0;
			}
			else
			{
				szField[k]	= (char)ch;
				k=k+1;
                    if ( k > _countof( szField ) )
                         return false;
			}

		}   while( CommaFlag && ( szCSVString[lCSVStringIndex] != '\0' ) );
		
	}  while( szCSVString[lCSVStringIndex] != '\0' );

     //success
     if( lColCount > 0 )
          lRowCount =  ( RecNo / lColCount );
     else
          lRowCount = 0;
    
     return true;

}


//////////////////////////////////////////////////////////////////////
// Get the column values as double from the CSV string
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bGetColAsDoubleArray( double *dCDFArray, 
                                      long lMaxCDFArrayCount, 
                                      long lCDFColumn,
                                      long *lRowsFound )
{

    //locals
	char s=',';
	int k;
	int ch;
     int chNext;
	int RecNo;
     long lCSVStringIndex;
     long lCurrentColumn;
     long lCDFArrayIndex;
     long lCurrentRow;
	bool CommaFlag;

     //initialize locals
     lCSVStringIndex = -1;
	k = 0;
     RecNo = 0;
     lCurrentColumn = 0;
     lCDFArrayIndex = 0;
     lCurrentRow = 0;
     CommaFlag	= true;

     //loop through the string
     do
	{

		do
		{
               lCSVStringIndex++;
               ch = szCSVString[lCSVStringIndex];

			//Check to see if it is a comma or end of line
			if(((char)ch == ',') || ((char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
			{
				RecNo++;
				CommaFlag = false;
				szField[k] = '\0';
                    lCurrentColumn++;

                    //if this is the column of interest add it to the array
                    if( ( lCurrentColumn == lCDFColumn ) && ( lCDFArrayIndex < lMaxCDFArrayCount ) )
                    {
                         dCDFArray[lCDFArrayIndex] = atof( szField );
                         lCDFArrayIndex++;
                    }

				//if end of line, increment row pointer
                    if( ( (char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
                    {
					//increment row pointer, reset column pointer
                         lCurrentRow++;
                         lCurrentColumn = 0;

                         //check to see if the next character is a 10 or 13 and skip it too
                         chNext = szCSVString[ ( lCSVStringIndex + 1 ) ];
                         if( ( chNext == 10 ) || ( chNext == 13 ) )
                              lCSVStringIndex++;
                    }

                    strcpy_s( szField, "" );
                    k = 0;

                    
			}
			else
			{
				szField[k] = (char)ch;
				k=k+1;
			}

		}   while( CommaFlag && ( szCSVString[lCSVStringIndex] != '\0' ) && ( k <= _countof(szField) ) );
		
	}  while( szCSVString[lCSVStringIndex] != '\0' );

     //success
     *lRowsFound = lCDFArrayIndex;
     return true;

}

//////////////////////////////////////////////////////////////////////
// Get the column count for a string
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bGetColCount( int *iColCount )
{

     *iColCount =  CFilterCDF::lColCount;
     return true;

}

//////////////////////////////////////////////////////////////////////
// Get the maximum string length for parsing.
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bGetMaxStringCount( int *iMaxString )
{
     *iMaxString = MAXSTRING;
     return true;
}

//////////////////////////////////////////////////////////////////////
// Get the column values as double from the CSV string
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bGetRecordAsString( long lCol, long lRow, char *pszRecordString )
{

     //locals
	char	s=',';
	int k;
	int ch;
     int chNext;
	int RecNo;
     long lCSVStringIndex;
     long lCurrentColumn;
     long lCDFArrayIndex;
     long lCurrentRow;
	bool CommaFlag;
     errno_t err;

     //initialize locals
     lCSVStringIndex = -1;
	k = 0;
     RecNo = 0;
     lCurrentColumn = 0;
     lCDFArrayIndex = 0;
     lCurrentRow = 1;
     CommaFlag	= true;

     //loop through the string
     do
	{

		do
		{
               lCSVStringIndex++;
               ch = szCSVString[lCSVStringIndex];

			//Check to see if it is a comma or end of line
			if(((char)ch == ',') || ((char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
			{
				RecNo++;
				CommaFlag = false;
				szField[k] = '\0';
                    lCurrentColumn++;

                    //if this is the column and row of interest?
                    if( ( lCurrentColumn == lCol ) && ( lCurrentRow == lRow ) )
                    {
                         err = strcpy_s( pszRecordString, _countof( szField ), szField );
                         if( err == 0 )
                              return true;
                         else
                              return false;
                    }

				//if end of line, increment row pointer
                    if( ( (char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
                    {
					//increment row pointer
                         lCurrentRow++;
                         lCurrentColumn = 0;

                         //check to see if the next character is a 10 or 13 and skip it too
                         chNext = szCSVString[ ( lCSVStringIndex + 1 ) ];
                         if( ( chNext == 10 ) || ( chNext == 13 ) )
                              lCSVStringIndex++;
                    }

                    strcpy_s( szField, "" );
                    k = 0;

                    
			}
			else
			{
				szField[k] = (char)ch;
				k=k+1;
			}

		}   while( CommaFlag && ( szCSVString[lCSVStringIndex] != '\0' ) && ( k <= _countof(szField) ) );
		
	}  while( szCSVString[lCSVStringIndex] != '\0' );

     //success
     return true;

}
//////////////////////////////////////////////////////////////////////
// Get the row values as double from the CSV string
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bGetRowAsDoubleArray( double *dCDFArray, 
                                      long lMaxCDFArrayCount, 
                                      long lCDFRow,
                                      long *lColsFound )
{

     //locals
	char	s=',';
	int k;
	int ch;
     int chNext;
	int RecNo;
     long lCSVStringIndex;
     long lCurrentColumn;
     long lCDFArrayIndex;
     long lCurrentRow;
	bool CommaFlag;

     //initialize locals
     lCSVStringIndex = -1;
	k = 0;
     RecNo = 0;
     lCurrentColumn = 0;
     lCDFArrayIndex = 0;
     lCurrentRow = 1;
     CommaFlag	= true;

     //loop through the string
     do
	{

		do
		{
               lCSVStringIndex++;
               ch = szCSVString[lCSVStringIndex];

			//Check to see if it is a comma or end of line
			if(((char)ch == ',') || ((char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
			{
				RecNo++;
				CommaFlag = false;
				szField[k] = '\0';
                    lCurrentColumn++;

                    //if this is the row of interest add it to the array
                    if( ( lCurrentRow == lCDFRow ) && ( lCDFArrayIndex < lMaxCDFArrayCount ) )
                    {
                         dCDFArray[lCDFArrayIndex] = atof( szField );
                         lCDFArrayIndex++;
                    }

				//if end of line, increment row pointer
                    if( ( (char)ch == '\n') || ( ch == 13 ) || ( (char)ch == '\0' ) )
                    {
					//increment row pointer, reset column pointer
                         lCurrentRow++;
                         lCurrentColumn = 0;

                         //check to see if the next character is a 10 or 13 and skip it too
                         chNext = szCSVString[ ( lCSVStringIndex + 1 ) ];
                         if( ( chNext == 10 ) || ( chNext == 13 ) )
                              lCSVStringIndex++;
                    }

                    strcpy_s( szField, "" );
                    k = 0;

                    
			}
			else
			{
				szField[k] = (char)ch;
				k=k+1;
			}

		}   while( CommaFlag && ( szCSVString[lCSVStringIndex] != '\0' ) && ( k <= _countof(szField) ) );
		
	}  while( szCSVString[lCSVStringIndex] != '\0' );


     //success
     *lColsFound = lCDFArrayIndex;
     return true;

}

//////////////////////////////////////////////////////////////////////
// Get the row count for a string
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bGetRowCount( int *iRowCount )
{

     *iRowCount =  CFilterCDF::lRowCount;
     return true;

}

//////////////////////////////////////////////////////////////////////
// Set the string for parsing.
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::bSetStringData( char *szString )
{
     errno_t err;
     
     //validate string length
     if( strlen( szString ) >= MAXSTRING )
          return false;

     //copy the string into the object and update
     //associated members
     err = strcpy_s( szCSVString, szString );
     lCSVStringLength = strlen( szCSVString );

     //check for errors
     if( err != 0 )
          return false;

     //update string characteristics
     if ( !bCharacterizeString() )
          return false;

     //success
     return true;
}
//////////////////////////////////////////////////////////////////////
// This method returns the number of columns in a
// comma seperated value (csv) file.
//////////////////////////////////////////////////////////////////////
int CFilterCDF::GetCols(char *pstrFileName)
{

	//local variables
	char	s=',';
	int k;
	int iFile;
	int ch;
	int RecNo;
	bool CommaFlag;
	bool EndOfLineFlag;
     errno_t err;

	//initialize local variables
	k = 0;
	iFile = 0;
	ch = 0;
	RecNo = 0;
	CommaFlag	= true;
	EndOfLineFlag = true;

	//Open the file and count columns
	err = fopen_s( &stream, pstrFileName , "r" );
	//looks like we had a problem and need to bail out
     if( err != 0)
	{
		RecNo	= -1;
		return RecNo;
	}

	//loop through the file
     do
	{

		do
		{
			ch = fgetc( stream );

			//Check to see if it is a comma or end of line
			if(((char)ch == ',') || ((char)ch == '\n') || ( ch == 13 ) )
			{
				RecNo++;
				CommaFlag = false;
				szField[k] = '\0';
				if( ( (char)ch == '\n') || ( ch == 13 ) )
					EndOfLineFlag = false;
			}
			else
			{
				szField[k]	= (char)ch;
				k=k+1;
			}


		}   while(CommaFlag && (feof (stream) == 0));
		
	}  while(EndOfLineFlag && feof (stream) == 0);

	iFile = fclose(stream);

	return RecNo;

}

//////////////////////////////////////////////////////////////////////
// Given a file name, this function checks to see
// if the data present might be JSC time format.
// If so a true is returned.
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::IsJSCTime(char *pstrFileName)
{

	//local variables
	char	s=',';
	int darrayindex;
	int k;
	int iFile;
	int ch;
	bool	CommaFlag;
	CJSCTime	jsctime;
     errno_t err;


	//Initialize local variables
	darrayindex = 0;
	k = 0;
	iFile = 0;
	CommaFlag	= true;

	//Open the file and check for success
	err = fopen_s( &stream, pstrFileName , "r"); 
     if( err != 0 )
		return false;

	//We made it.  Now begin reading in the
	//information from the first entry
	do
	{
		ch = fgetc( stream );

		//Check to see if it is a comma
		if(((char)ch == ',') || ((char)ch == '\n') )
		{
			CommaFlag = false;
			szField[k] = '\0';

		}
		else
		{
			szField[k] = (char)ch;
			k=k+1;
		}
	}   while(CommaFlag && (feof (stream) == 0));

	//Check to see what knocked us out of the loop
	//if eof, then unexpected error.  Let the calling
	//program know
	if( feof (stream) != 0)
		return false;

	//... and we have cdf value.
	err = strcpy_s(jsctime.timestring, szField);
     if( err != 0 )
          return false;

	//attempt to convert the string to a time.
	//If this function call fails, we know there
	//is a problem.
	if( !jsctime.ConvertText2Time() )
		return false;

	//close the file	
	iFile = fclose(stream);

	//success
	return true;

}

//////////////////////////////////////////////////////////////////////
// Given the number of columns, this function returns
// the number of rows in a comma seperated value
// (csv) file.
//////////////////////////////////////////////////////////////////////
int CFilterCDF::GetRows(char *pstrFileName, int ColCount)
{

	char	s=',';
	int k;
	int iFile;
	int ch;
	int RecNo=0;
	bool	CommaFlag;
	bool	EndOfLineFlag=true;
     errno_t err;

	//Stash the data to the file
     err = fopen_s ( &stream, pstrFileName , "r" );
     if( err == 0 )
	{
	
		do
		{

			CommaFlag	= true;
			k = 0;
			do
			{
				ch = fgetc( stream );

				//Check to see if it is a comma
				if(((char)ch == ',') || ((char)ch == '\n') || ( ch == 13 ) )
				{
					RecNo++;
					CommaFlag = false;
					szField[k] = '\0';
					if((char)ch == '\n')
						EndOfLineFlag = false;
				}
				else
				{
					szField[k] = (char)ch;
					k=k+1;
				}


			}   while(CommaFlag && (feof (stream) == 0));
			
		}  while( feof (stream) == 0 );
	}
	else
	{	
		return -1;
	}

	iFile = fclose(stream);

	return int( (double)RecNo/(double)ColCount );

}

//////////////////////////////////////////////////////////////////////
// This method retrieves data from the CDF file
// Overload specific:
// - retrieves specific record
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::LoadData(char *pstrFileName, int RecNo)
{

	char	s=',';
	int j;
	int k;
	int iFile;
	int ch;
	bool	CommaFlag;
     errno_t err;

	//Stash the data to the file
	err = fopen_s( &stream, pstrFileName , "r"  );
     if( err  == 0 )
	{
	
		for(j=0; j<RecNo; j++)
		{

			CommaFlag	= true;
			k			= 0;
			do
			{
				ch = fgetc( stream );

				//Check to see if it is a comma or end of line
				if(((char)ch == ',') || ((char)ch == '\n') )
				{
					CommaFlag = false;
					szField[k] = '\0';
				}
				else
				{
					szField[k]	= (char)ch;
					k=k+1;
				}


			}   while(CommaFlag && (feof (stream) == 0));

			//Check to see what knocked us out of the loop
			//if eof, then unexpected error.  Let the calling
			//program know
			if( feof (stream) != 0)
				return false;
		}
	}
	else
	{	
		return false;
	}

	iFile = fclose(stream);

	//success
	return true;

}


//////////////////////////////////////////////////////////////////////
// This methods retrieves data from the CDF file	 
// Overload specific:						
// - retrieves all records and places each item in   
//   a array of type double							 
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::LoadData(char *pstrFileName, 
							double *cdfarray,
							int RecCount)
{

	//local variables
	char	s=',';
	int j;
	int k;
	int iFile;
	int ch;
	bool CommaFlag;
	char	*stopstring;
     errno_t err;

	//Initialize local variables
	j = 0;
	k = 0;
	iFile = 0;
	ch = 0;
	CommaFlag	= true;
	stopstring = NULL;

	//Initialize object variables
	strcpy_s( szField, "" );

	//Attempt to open the file for reading
	err = fopen_s( &stream, pstrFileName, "r" );
     if( err != 0  )
		return false;
	
	//Outer loop to read the file
	for(j=0; j<RecCount; j++)
	{

		do
		{
			ch = fgetc( stream );

			//Check to see if it is a comma or end of line
			if( ( (char)ch == ',' ) || ( (char)ch == '\n' )  || ( ch == 13 ) )
			{
				CommaFlag = false;
				szField[k] = '\0';
			}
			else
			{
				szField[k]	= (char)ch;
				k=k+1;
			}


		}   while( CommaFlag && ( feof( stream ) == 0 ) );

		//Check to see what knocked us out of the loop
		//if eof, then unexpected error.  Let the calling
		//program know
		if( feof( stream ) != 0)
			return false;

		//... and we have cdf value
		cdfarray[j]		= strtod( szField, &stopstring );

		//reset the variables in the loop
		strcpy_s( szField, "" );
		CommaFlag	= true;
		k = 0;

	}

	//close the file
	iFile = fclose(stream);

	//success
	return true;

}

//////////////////////////////////////////////////////////////////////
// This method fills the arrays from the file in		
// pstrFileName.  By default the first column of data is
// the timearray (or more generally, the independant	
// data set.  The remainder of the columns are defined	
// as the cdfarray (or the dependant variable arrays.
// The RecCount argument defines the maximum number of 
// records.
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::LoadData(char *pstrFileName, 
							double *timearray, 
							double *cdfarray, 
							int RecCount)
{

	//local variables
	char s=',';
	int timeindex;
	int darrayindex;
	int j;
	int k;
	int iFile;
	int ch;
	bool	CommaFlag;
	bool	IndepCol;
	bool	JSCTimeFormat;
	char	*stopstring;
	CJSCTime jsctime;
     errno_t err;

	//Initialize local variables
	timeindex	= 0;
	darrayindex = 0;
	j = 0;
	k = 0;
	iFile = 0;
	CommaFlag	= true;
	IndepCol = true;
	JSCTimeFormat = false;
	stopstring = NULL;

	//Inspect file contents
	JSCTimeFormat	= IsJSCTime( pstrFileName );

	//initialize object variables
	strcpy_s( szField, "" );
	
	//Open the file and check for success
	err = fopen_s( &stream, pstrFileName , "r" ); 
     if( err != 0 )
		return false;

	//We made it.  Now begin reading in the
	//information
	for( j=0; j<RecCount; j++ )
	{

		CommaFlag	= true;
		k			= 0;
		do
		{
			ch = fgetc( stream );

			//Check to see if it is a comma
			if( ( (char)ch == ',' ) || ( (char)ch == '\n' ) || ( ch == 13 ) )
			{
				CommaFlag	= false;
				szField[k]	= '\0';

			}
			else
			{
				szField[k]	= (char)ch;
				k=k+1;
			}

		}   while(CommaFlag && (feof( stream ) == 0));

		//Check to see what knocked us out of the loop
		//if eof, then unexpected error.  Let the calling
		//program know
		if( feof( stream ) != 0)
			return false;

		//... and we have cdf value.  If the IndepCol
		//is true, then save it to the JSCTime object
		//also check to see if it a newline character se
		//we can reset the flag
		if( IndepCol )
		{
			//If we have to convert with JSC time, do it.
			//Otherwise, just a standard conversion to the
			//independent array
			if( JSCTimeFormat )
			{
				strcpy_s(jsctime.timestring, szField);
				if( !jsctime.ConvertText2Time())
					return false;
				timearray[timeindex] = jsctime.abstime;
				timeindex++;
				IndepCol		= false;
			}
			else
			{
				timearray[timeindex] = strtod(szField, &stopstring);
				timeindex++;
				IndepCol		= false;
			}
		}
		else
		{
			cdfarray[darrayindex] = strtod(szField, &stopstring);
			darrayindex++;
			if( ( (char)ch == '\n') || ( ch == 13 ) )
				IndepCol	= true;
		}
	}

	//close the file	
	iFile = fclose(stream);

	//success
	return true;

}

//////////////////////////////////////////////////////////////////////
// This method fills the arrays from the file in		
// pstrFileName.  By default the first column of data is
// the timearray (or more generally, the independant	
// data set.  The remainder of the columns are defined	
// as the cdfarray (or the dependant variable arrays.
//////////////////////////////////////////////////////////////////////
bool CFilterCDF::LoadData(char *pstrFileName, 
							double *timearray, 
							double *cdfarray)
{
	//local variables
	char	s=',';
	int timeindex;
	int			darrayindex;
	int			j;
	int			k;
	int			iFile;
	int			ch;
	bool		CommaFlag;
	bool		JSCTimeFormat;
	bool		IndepCol;
	char		*stopstring;
	CJSCTime	jsctime;
     errno_t err;

	//Initialize local variables
	timeindex		= 0;
	darrayindex		= 0;
	j				= 0;
	k				= 0;
	iFile			= 0;
	CommaFlag		= true;
	IndepCol	= true;
	JSCTimeFormat	= false;
	
	//Inspect file contents
	JSCTimeFormat	= IsJSCTime( pstrFileName );

	//Open the file and check for success
     err = fopen_s( &stream, pstrFileName , "r"); 
     if( err != 0 )
		return false;

	//We made it.  Now begin reading in the
	//information
	do
	{

		CommaFlag	= true;
		k			= 0;
		do
		{
			ch = fgetc( stream );

			//Check to see if it is a comma
			if(((char)ch == ',') || ((char)ch == '\n') )
			{
				CommaFlag = false;
				szField[k] = '\0';

			}
			else
			{
				szField[k]	= (char)ch;
				k=k+1;
			}

		}   while(CommaFlag && (feof (stream) == 0));

		//Check to see what knocked us out of the loop
		//if eof, then in this case, expected results
		if( feof (stream) != 0)
		{
			return true;
			//close the file	
			iFile = fclose(stream);
		}

		//... and we have cdf value.  If the IndepCol
		//is true, then save it to the JSCTime object
		//also check to see if it a newline character se
		//we can reset the flag
		if( IndepCol )
		{
			//If we have to convert with JSC time, do it
			//otherwise, just a standard conversion to the
			//independent array
			if( JSCTimeFormat )
			{
				strcpy_s(jsctime.timestring, szField);
				if( !jsctime.ConvertText2Time())
					return false;
				timearray[timeindex] = jsctime.abstime;
				timeindex++;
				IndepCol		= false;
			}
			else
			{
				timearray[timeindex] = strtod(szField, &stopstring);
				timeindex++;
				IndepCol		= false;
			}
		}
		else
		{
			cdfarray[darrayindex] = strtod(szField, &stopstring);
			darrayindex++;
			if((char)ch == '\n')
				IndepCol	= true;
		}

		//increment the counter
		j++;

	} while (feof (stream) == 0);

	//close the file	
	iFile = fclose(stream);

	//success
	return true;

}