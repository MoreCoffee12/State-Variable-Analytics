
///////////////////////////////////////////////////////////////////
// This program serves as a test harness for
// the crank slider class
///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////
// Written by:	Brian Howard
// Date:		18 Dec 2001
///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////
// Revision Comment - 11 December 2006
//
// Amended error traping to capture bore less
// than or equal to zero
//
///////////////////////////////////////////////////////////////////
// Revision Comment - 13 July 2010
//
// Updated to visual studio 2008.  Added pressure curves analytics.
//
///////////////////////////////////////////////////////////////////

#define MAXCHARBUFFERLENGTH 2048
#define LOCALMAXPRESSUREARRAYSIZE 65535

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <oleauto.h>
#include <math.h>
#include "compexp.h"
#include "pressurecurveanalytics.h"
using namespace std;

bool displaymembers( CPressureCurveAnalytics *pcurveanalytics, FILE *stream )
{
    //local variables
    char outputline[MAXCHARBUFFERLENGTH];
    char templine[MAXCHARBUFFERLENGTH];
    char sTemp01[MAXCHARBUFFERLENGTH];
    bool bTemp;
    long lTemp;
    double dTemp01;
    double dTemp02;
    double dTemp03;
    double dTemp04;
    int i;
    
	//initialize local variables
    strcpy_s(outputline, "");
    strcpy_s(templine, "" );
    i = 0;

	//Begin by outputting column headings
	strcpy_s( outputline, MAXCHARBUFFERLENGTH, "Head End?,Pressure Sample Count,Clearance Percent,Comp. Ratio, Baro Pressure (USCS), Average K\n" );

    //output the formatted streams
	fprintf(stream, outputline );

    //acquire object values
    if( pcurveanalytics->bGetChamberEnd( &bTemp ) == false )
    {
        return false;
    }
    else
    {
        if( bTemp == true )
        {
            strcpy_s( sTemp01, "True" );
        }
        else
        {
            strcpy_s( sTemp01, "False" );
        }
    }
    
    if( pcurveanalytics->bGetPressureCurveSampleCount( &lTemp ) == false )
    {
        return false;
    }

    if( pcurveanalytics->bGetClearanceVolumePercent( &dTemp01 ) == false )
    {
        return false;
    }

    if( pcurveanalytics->bGetBarometricPressure_USCS( &dTemp02 ) == false )
    {
        return false;
    }

    if( pcurveanalytics->bGetCompressionRatio( &dTemp03 ) == false )
    {
        return false;
    }

    if( pcurveanalytics->bGetkAvg( &dTemp04 ) == false )
    {
        return false;
    }

    //output object values
    fprintf( stream, "%s, %i, %f, %f, %f, %f\n", 
							sTemp01,
                            lTemp,
                            dTemp01,
                            dTemp02, 
                            dTemp03,
                            dTemp04);

    //success
    return true;
}
bool displaymembers( CCompExpCurve *compexp, FILE *stream )
{

    //local variables
    char outputline[MAXCHARBUFFERLENGTH];
    char templine[MAXCHARBUFFERLENGTH];
    int i;
    
	//initialize local variables
    strcpy_s(outputline, "");
    strcpy_s(templine, "" );
    i = 0;

	//Begin by outputting column headings
	strcpy_s( outputline, MAXCHARBUFFERLENGTH, "Bore (in),Rod Diameter (in),Stroke (in),ConnRodLength (in),Clearance (%),Clearance (in3),Displaced Volume (in3),nComp,nExp,Suction Pressure (PSIA),Discharge Pressure (PSIA),CrankAngle, Pressure at CrankAngle (PSIA)," );
	strcat_s( outputline, MAXCHARBUFFERLENGTH, "Bore (cm),Rod Diameter (cm),Stroke (cm),ConnRodLength (cm),Clearance (%),Clearance (cm3),Displaced Volume (cm3),nComp,nExp,Suction Pressure (bar abs),Discharge Pressure (bar abs),CrankAngle, Pressure at CrankAngle (bar abs)," );
    strcat_s( outputline, MAXCHARBUFFERLENGTH, "Suction Leak, Discharge Leak \n" );
     
    //output the formatted streams
	fprintf(stream, outputline );

	//Output members.  Begin with USCS crank-slider dims
     fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,", 
							compexp->GetBoreDiameter_USCS(),
							compexp->GetRodDiameter_USCS(),
                                   compexp->GetStroke_USCS(),
                                   compexp->GetConnRodLength_USCS(),
							compexp->GetClearance_Percent(),
                                   compexp->GetClearance_USCS(),
                                   compexp->GetDisplacedVolume_USCS(),
                                   compexp->GetnComp(),
							compexp->GetnExp(),
                                   compexp->GetPressureAbsSuction_USCS(),
                                   compexp->GetPressureAbsDischarge_USCS(),
                                   compexp->GetCrankAngleDegrees(),
                                   compexp->GetPressureAbsCrankAngle_USCS());

	//Now SI crank-slider dims
	fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", 
							compexp->GetBoreDiameter_SI(),
							compexp->GetRodDiameter_SI(),
                                   compexp->GetStroke_SI(),
                                   compexp->GetConnRodLength_SI(),
							compexp->GetClearance_Percent(),
                                   compexp->GetClearance_SI(),
                                   compexp->GetDisplacedVolume_SI(),
                                   compexp->GetnComp(),
							compexp->GetnExp(),
                                   compexp->GetPressureAbsSuction_SI(),
                                   compexp->GetPressureAbsDischarge_SI(),
                                   compexp->GetCrankAngleDegrees(),
                                   compexp->GetPressureAbsCrankAngle_SI(),
                                   compexp->GetLeakSuction(),
                                   compexp->GetLeakDischarge());

     //success
	return true;
}

//main test harness
void main ()
{
	//local variables
	int	i;
	int	j;
	FILE *stream;
	class CCompExpCurve *compexp;
    class CPressureCurveAnalytics *pcurveanalytics;
	char outputline[MAXCHARBUFFERLENGTH];
    string line;
    double testvalue;
	double dTemp;
    double dTemp02;
	double xarray[10];
	double yarray[10];
    double dPressureCurve[LOCALMAXPRESSUREARRAYSIZE];
    double dVolArray[LOCALMAXPRESSUREARRAYSIZE];
	double r;
    SYSTEMTIME SysTime;
	int CurrentTickCount;
    int PreviousTickCount;
    int LapsedTicks;
    long lArraySize;
    errno_t err;

	//initialize local variables
	i = 0;
	j = 0;
	stream = NULL;
	compexp = NULL;
	strcpy_s(outputline, "");
	testvalue = 3.14159;
	dTemp = 0;
    dTemp02 = 0;
	for(i=0;i<10;i++)
	{
		xarray[i] = 0;
		yarray[i] = 0;
	}
    r = 0;
    CurrentTickCount = GetTickCount();
    PreviousTickCount = GetTickCount();
    LapsedTicks = ( CurrentTickCount - PreviousTickCount);


    ///////////////////////////////////////////////////////////////////
	// The remainder of this program serves as a container and harness
	// to test the object members and methods.  Tests also			  
    // include exception handling	
    ///////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
	// Test Sequence
	//	- Verify class instantiation
	//	- Verify member initialization	
	//	- Verify method performance
    ///////////////////////////////////////////////////////////////////
	
	//Open the log file
    err = fopen_s(&stream, "CCompExpCurveHarness.csv" , "w");
	if( err != 0 )
    {
	    strcpy_s( outputline, "Failed to open output fle.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
		return;
    }

	//Intro line
	strcpy_s( outputline, "CCompExpCurve Test Harness.\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Assemble the object
	compexp	= new CCompExpCurve();

    //insert time value
    GetLocalTime( &SysTime );
    PreviousTickCount = CurrentTickCount;
    CurrentTickCount = GetTickCount();
    LapsedTicks = ( CurrentTickCount - PreviousTickCount );
    fprintf(stream, "%i:%02i:%02i:%003i\n", 
    (int)SysTime.wHour,
    (int)SysTime.wMinute,
    (int)SysTime.wSecond,
    (int)SysTime.wMilliseconds);
    fprintf(stream, "Number of milliseconds: %i\n", LapsedTicks ); 


     //Document instantiation of class
	if( compexp == NULL )
	{
		strcpy_s( outputline, "CCompExpCurve Failed to Instantiate.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}
	else
	{
		strcpy_s( outputline, "CCompExpCurve Instantiated.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    ///////////////////////////////////////////////////////////////////
    // Documemt member values as found on class instatiation
    ///////////////////////////////////////////////////////////////////
	if( !displaymembers( compexp, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    ///////////////////////////////////////////////////////////////////
    // Verify member instatiated with expected default values
    ///////////////////////////////////////////////////////////////////
    // Begin with the cylinder bore diameter
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->GetBoreDiameter_USCS() == 1.5 ) && ( compexp->GetBoreDiameter_SI() == ( 1.5 * 2.54) ) )
    {
        strcpy_s( outputline, "Bore Diameter Default OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
		strcpy_s( outputline, "Bore Diameter initialization failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // piston rod diameter
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->GetRodDiameter_USCS() == 0.2 ) && ( compexp->GetRodDiameter_SI() == ( 0.2 * 2.54) ) )
    {
	    strcpy_s( outputline, "Rod Diameter Default OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
	    strcpy_s( outputline, "Rod Diameter initialization failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    //Clearance
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->GetClearance_Percent() == 0.1 ) && ( compexp->GetClearance_USCS() > 0.156215694 ) && ( compexp->GetClearance_USCS() < 0.156215696 ) )
    {
		strcpy_s( outputline, "Clearance Percent Default OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
        strcpy_s( outputline, "Clearance Percent initialization failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
	// Verify ability to set active end of the cylinder 
    ///////////////////////////////////////////////////////////////////
    if( compexp->bSetHeadEnd( true, true ) )
    {
		strcpy_s( outputline, "HeadEnd set OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "HeadEnd set failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to catch discharge pressures less than 0
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetPressureAbsDischarge_USCS( -14.696 ) ) )
    {
		strcpy_s( outputline, "SetPressureAbsDischarge_USCS error trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetPressureAbsDischarge_USCS failed to trap errors.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal discharge pressures 
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->SetPressureAbsDischarge_USCS( 644.6 ) ) )
    {
		strcpy_s( outputline, "SetPressureAbsDischarge_USCs loading data OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetPressureAbsDischarge_USCS failed to set pressure.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to catch suction pressures less than 0 
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetPressureAbsSuction_USCS( -14.696 ) ) )
    {
		strcpy_s( outputline, "SetPressureAbsSuction_USCS error trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetPressureAbsSuction_USCS failed to trap errors.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal suction pressures 
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->SetPressureAbsSuction_USCS( 270.2 ) ) )
    {
		strcpy_s( outputline, "SetPressureAbsSuction_USCS loading data OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetPressureAbsSuction_USCS failed to set pressure.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    //////////////////////////////////////////////////////////////////////
 	// Verify ability to catch bore diameters less than 0
    //////////////////////////////////////////////////////////////////////
    if( !( compexp->SetBoreDiameter_USCS( -19.00 ) ) )
    {
		strcpy_s( outputline, "SetBoreDiameter_USCS error trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetBoreDiameter_USCS failed to trap errors.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
	// Verify ability to catch bore diameters equal to 0
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetBoreDiameter_USCS( 0.00 ) ) )
    {
		strcpy_s( outputline, "SetBoreDiameter_USCS error trapping for bore = 0 OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetBoreDiameter_USCS failed to trap errors for bore = 0.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal bore diameters 
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->SetBoreDiameter_USCS( 19.00 ) ) )
    {
		strcpy_s( outputline, "SetBoreDiameter_USCS loading data OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
     }
     else
     {
		strcpy_s( outputline, "SetBoreDiameter_USCS failed to set bore diameter.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to catch rod diameters less than 0 
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetRodDiameter_USCS( -5.00 ) ) )
    {
		strcpy_s( outputline, "SetRodDiameter_USCS error trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetRodDiameter_USCS failed to trap errors.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal rod diameter
    ///////////////////////////////////////////////////////////////////
     if( ( compexp->SetRodDiameter_USCS( 5.00 ) ) )
     {
		strcpy_s( outputline, "SetRodDiameter_USCS loading data OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
     }
     else
     {
		strcpy_s( outputline, "SetRodDiameter_USCS failed to set bore diameter.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
     }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to trap clerance percentages less than 0
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetClearance_Percent( -0.533 ) ) )
    {
		strcpy_s( outputline, "SetClearance_Percent trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetClearance_Percent trapping failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal clerance 
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->SetClearance_Percent( 0.533 ) ) && ( compexp->GetClearance_Percent() == 0.533 ) )
    {
		strcpy_s( outputline, "SetClearance_Percent OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetClearance_Percent failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to trap ncomp less than 0 
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetnComp( -1.40231 ) ) )
    {
		strcpy_s( outputline, "SetnComp trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetnComp trapping failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal ncomp  
    ///////////////////////////////////////////////////////////////////
     if( ( compexp->SetnComp( 1.40231 ) ) && ( compexp->GetnComp() == 1.40231 ) )
     {
		strcpy_s( outputline, "SetnComp OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
     }
     else
     {
		strcpy_s( outputline, "SetnComp failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
     }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to trap nexp less than 0 
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetnExp( -1.019 ) ) )
    {
		strcpy_s( outputline, "SetnExp trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetnExp trapping failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal nexp
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->SetnExp( 1.019 ) ) && ( compexp->GetnExp() == 1.019 ) )
    {
		strcpy_s( outputline, "SetnExp OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetnExp failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to trap leaksuction less than 0
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetLeakSuction( -0.10 ) ) )
    {
		strcpy_s( outputline, "SetLeakSuction trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetLeakSuction trapping failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal leak suction
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->SetLeakSuction( 0.10 ) ) && ( compexp->GetLeakSuction() == 0.10 ) )
    {
		strcpy_s( outputline, "SetLeakSuction OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
          displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetLeakSuction failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to trap leakdischarge less than 0
    ///////////////////////////////////////////////////////////////////
    if( !( compexp->SetLeakDischarge( -0.10 ) ) )
    {
		strcpy_s( outputline, "SetLeakDischarge trapping OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetLeakDischarge trapping failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to set normal leak discharge 
    ///////////////////////////////////////////////////////////////////
    if( ( compexp->SetLeakDischarge( 0.0 ) ) && ( compexp->GetLeakDischarge() == 0.0 ) )
    {
		strcpy_s( outputline, "SetLeakDischarge OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "SetLeakDischarge failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Load a real set of data and verify performance at TDC
    ///////////////////////////////////////////////////////////////////
    compexp->SetStroke_USCS ( 36 );
    compexp->SetConnRodLength_USCS( 47.26 );
    compexp->SetBoreDiameter_USCS( 19.00 );
    compexp->SetRodDiameter_USCS( 0.00 );
    compexp->SetCrankAngleDegrees( 0.00 );
    compexp->SetClearance_Percent( 0.533 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsDischarge_USCS() ) )
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd TDC OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd TDC failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Load a real set of data and verify performance at 14 
    // degrees ATDC. 
    ///////////////////////////////////////////////////////////////////
    compexp->SetStroke_USCS ( 36 );
    compexp->SetConnRodLength_USCS( 47.26 );
    compexp->SetBoreDiameter_USCS( 19.00 );
    compexp->SetRodDiameter_USCS( 0.00 );
    compexp->SetCrankAngleDegrees( 14.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() < 620.36 ) 
        && ( compexp->GetPressureAbsCrankAngle_USCS() > 620.34 ) )
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 14 degrees OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 14 degrees failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        fclose( stream );
        delete compexp;
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Make sure the suction valve really opens.
    ///////////////////////////////////////////////////////////////////
    compexp->SetStroke_USCS ( 36 );
    compexp->SetConnRodLength_USCS( 47.26 );
    compexp->SetBoreDiameter_USCS( 19.00 );
    compexp->SetRodDiameter_USCS( 0.00 );
    compexp->SetCrankAngleDegrees( 111.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsSuction_USCS() ) )
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 111 degrees (suction valve open) OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 111 degrees ( suction valve open) failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify Head End Pressures at bottom dead center (BDC)
    ///////////////////////////////////////////////////////////////////
    compexp->SetStroke_USCS ( 36 );
    compexp->SetConnRodLength_USCS( 47.26 );
    compexp->SetBoreDiameter_USCS( 19.00 );
    compexp->SetRodDiameter_USCS( 0.00 );
    compexp->SetCrankAngleDegrees( 180.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsSuction_USCS() ) )
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 180 degrees OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 180 degrees failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Load a real set of data and verify performance at 194 
    // degrees ATDC.
    ///////////////////////////////////////////////////////////////////
    compexp->SetStroke_USCS ( 36 );
    compexp->SetConnRodLength_USCS( 47.26 );
    compexp->SetBoreDiameter_USCS( 19.00 );
    compexp->SetRodDiameter_USCS( 0.00 );
    compexp->SetCrankAngleDegrees( 194.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() < 272.51 )
        && ( compexp->GetPressureAbsCrankAngle_USCS() > 272.50 ) )
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 194 degrees OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 194 degrees failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify Discharge valve opens up 
    ///////////////////////////////////////////////////////////////////
    compexp->SetStroke_USCS ( 36.00 );
    compexp->SetConnRodLength_USCS( 47.26 );
    compexp->SetBoreDiameter_USCS( 19.00 );
    compexp->SetRodDiameter_USCS( 0.00 );
    compexp->SetCrankAngleDegrees( 342.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsDischarge_USCS() ) )
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 342 degrees (discharge valve opens) OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at HeadEnd 342 degrees (discharge valve opens) failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Load a real set of data and verify performance at TDC for the
    // crank end.
    ///////////////////////////////////////////////////////////////////
    compexp->bSetHeadEnd( false, true );
    compexp->SetStroke_USCS ( 36.00 );
    compexp->SetConnRodLength_USCS( 47.26 );
    compexp->SetBoreDiameter_USCS( 19.00 );
    compexp->SetRodDiameter_USCS( 5.00 );
    compexp->SetCrankAngleDegrees( 0.00 );
    compexp->SetClearance_Percent( 0.106 );
    compexp->SetnComp( 1.40283 );
    compexp->SetnExp( 1.14858 );
    compexp->SetPressureAbsSuction_USCS( 310.85 );
    compexp->SetPressureAbsDischarge_USCS( 588.46 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsSuction_USCS() ) )
    {
		strcpy_s( outputline, "CalcPressure at Crank End at TDC OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at Crank End at TDC failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // now the crank end at 14 degrees ATDC
    ///////////////////////////////////////////////////////////////////
    compexp->SetCrankAngleDegrees( 14.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() < 319.10 ) 
        && ( compexp->GetPressureAbsCrankAngle_USCS() > 319.08 ) )
    {
		strcpy_s( outputline, "CalcPressure at Crank End 14 degrees OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at Crank End 14 degrees failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Make sure the crank end discharge valve really opens 
    ///////////////////////////////////////////////////////////////////
    compexp->SetCrankAngleDegrees( 86.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsDischarge_USCS() ) )
    {
		strcpy_s( outputline, "CalcPressure at Crank End 86 degrees (discharge valve open) OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at Crank End 86 degrees (discharge valve open) failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify Crank End Pressures at bottom dead center (BDC)
    ///////////////////////////////////////////////////////////////////
    compexp->SetCrankAngleDegrees( 180.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsDischarge_USCS() ) )
    {
		strcpy_s( outputline, "CalcPressure at Crank End 180 degrees OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at Crank End 180 degrees failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Load a real set of data and verify performance at 194 
    // degrees ATDC.
    ///////////////////////////////////////////////////////////////////
    compexp->SetCrankAngleDegrees( 194.00 );
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() < 534.46 )
          && ( compexp->GetPressureAbsCrankAngle_USCS() > 534.44 ) )
    {
		strcpy_s( outputline, "CalcPressure at Crank End 194 degrees OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at Crank End 194 degrees failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify crank end suction valve opens up 
    ///////////////////////////////////////////////////////////////////
   compexp->SetCrankAngleDegrees( 342.00 );
   if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsSuction_USCS() ) )
   {
		strcpy_s( outputline, "CalcPressure at Crank End 342 degrees (suction valve opens) OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
    }
    else
    {
		strcpy_s( outputline, "CalcPressure at Crank End 342 degrees (suction valve opens) failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

     /********************************************************************
	* Run the numbers with a leak in the suction valve                  *
     *********************************************************************/
     compexp->bSetHeadEnd( true, true );
     compexp->SetCrankAngleDegrees( 342.00 );
     compexp->SetLeakSuction( 0.10 );
     compexp->SetLeakDischarge( 0.00 );
     j=(int)compexp->GetCrankAngleDegrees();
     if( ( compexp->bCalcPressure() ) && ( compexp->CalcPressureLeak() ) )
     {
          for(i=0;i<j;i++)
          {
               compexp->SetCrankAngleDegrees( (double)i );
               compexp->bCalcPressure();
               dTemp     = compexp->GetPressureAbsCrankAngle_USCS();
               compexp->CalcPressureLeak();
               dTemp02   = compexp->GetPressureAbsCrankAngle_USCS();
               fprintf( stream, "%i,%f,%f\n", i, dTemp, dTemp02 );
          }
     }

     fprintf( stream, "------------------------\n");

    ///////////////////////////////////////////////////////////////////
    // Run the numbers with a leak in the suction valve on the crank end *
    ///////////////////////////////////////////////////////////////////
    compexp->bSetHeadEnd( false, true );
    compexp->SetCrankAngleDegrees( 342.00 );
    compexp->SetLeakSuction( 0.10 );
    compexp->SetLeakDischarge( 0.00 );
    j=(int)compexp->GetCrankAngleDegrees();
    if( ( compexp->bCalcPressure() ) && ( compexp->CalcPressureLeak() ) )
    {
        for(i=0;i<j;i++)
        {
            compexp->SetCrankAngleDegrees( (double)i );
            compexp->bCalcPressure();
            dTemp = compexp->GetPressureAbsCrankAngle_USCS();
            compexp->CalcPressureLeak();
            dTemp02 = compexp->GetPressureAbsCrankAngle_USCS();
            fprintf( stream, "%i,%f,%f\n", i, dTemp, dTemp02 );
        }
    }

    ///////////////////////////////////////////////////////////////////
    // Reload the object with default values
    ///////////////////////////////////////////////////////////////////
    delete compexp;
    compexp	= new CCompExpCurve();
    compexp->SetBoreDiameter_USCS( 8.00 );
    compexp->SetStroke_USCS( 18.00 );
    compexp->SetConnRodLength_USCS( 47.255 );
    compexp->SetRodDiameter_USCS( 5.00 );
    compexp->SetPressureAbsSuction_USCS( 1200.00 );
    compexp->SetPressureAbsDischarge_USCS( 2800.00 );
    compexp->SetLeakSuction( 0.8 );

    //verify the head end suction pressure
    compexp->bSetHeadEnd( true, true );
    compexp->SetCrankAngleDegrees( 180 );
    compexp->bCalcPressure();
    if( ( compexp->bCalcPressure() ) && ( compexp->GetPressureAbsCrankAngle_USCS() == compexp->GetPressureAbsSuction_USCS() ) )
    {
        strcpy_s( outputline, "CalcPressure at HeadEnd 180 degrees OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        displaymembers( compexp, stream );
        for(i=0;i<360;i++)
        {
            compexp->SetCrankAngleDegrees( (double)i );
            compexp->bCalcPressure();
            dTemp = compexp->GetPressureAbsCrankAngle_USCS();
            compexp->CalcPressureLeak();
            dTemp02   = compexp->GetPressureAbsCrankAngle_USCS();
            fprintf( stream, "%i,%f,%f\n", i, dTemp, dTemp02 );
        }
    }
    else
    {
        strcpy_s( outputline, "CalcPressure at HeadEnd 180 degrees failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    //Verify ability set suction valve close angle captures
    //invalid close angle
    if( compexp->bSetSuctionValveCloseAngle( -1.00 ) )
    {
        strcpy_s( outputline, "bSetSuctionValveCloseAngle failed to trap a negative angle.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    //Verify ability to set suction valve close angle
    if( !compexp->bSetSuctionValveCloseAngle( 180.000 ) )
    {
        strcpy_s( outputline, "bSetSuctionValveCloseAngle failed on a value of 0.00.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }

    //Verify ability to set suction valve close angle
    if( !compexp->bGetSuctionValveCloseAngle( &dTemp ) )
    {
        strcpy_s( outputline, "bGetSuctionValveCloseAngle failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        delete compexp;
        fclose( stream );
        return;
    }
    if( dTemp < 180 || dTemp > 180 )
    {
		strcpy_s( outputline, "bGetSuctionValveCloseAngle failed bounds check.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //testing of CCompExp complete, free memory
    delete compexp;
    strcpy_s( outputline, "***************CCompExp tests successfully completed.********************\n" );
    printf( outputline );
    fprintf(stream, outputline );

    ///////////////////////////////////////////////////////////////////
    // code below this line tests the pressure curve analytics
    // object
    ///////////////////////////////////////////////////////////////////
    // Document and verify instatiation
    ///////////////////////////////////////////////////////////////////
    pcurveanalytics	= new CPressureCurveAnalytics();
    displaymembers( pcurveanalytics, stream );

    ///////////////////////////////////////////////////////////////////
    // Verfiy methods
    ///////////////////////////////////////////////////////////////////
    // Begin with configuration of head end chamber
    ///////////////////////////////////////////////////////////////////
    if( pcurveanalytics->bSetChamberEnd( true ) == true )
    {
		strcpy_s( outputline, "bSetChamberEnd OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
		strcpy_s( outputline, "bSetChamberEnd failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    //verify that the array size methods operate correctly
    ///////////////////////////////////////////////////////////////////
    if( pcurveanalytics->bSetPressureCurveSampleCount( -2 ) )
    {
		strcpy_s( outputline, "bSetPressureCurveSampleCount failed to trap a negative array count.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    if( !pcurveanalytics->bSetPressureCurveSampleCount( 360 ) )
    {
		strcpy_s( outputline, "bSetPressureCurveSampleCount failed to load a valid array count of 360.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( !pcurveanalytics->bGetPressureCurveSampleCount( &lArraySize ) )
    {
		strcpy_s( outputline, "bSetPressureCurveSampleCount failed to retrieve valid array count.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( lArraySize < 360 || lArraySize > 360 )
    {
		strcpy_s( outputline, "bSetPressureCurveSampleCount failed bounds check.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    ///////////////////////////////////////////////////////////////////
    // Verify ability to load and unload curves
    ///////////////////////////////////////////////////////////////////

    // Open the first test file.  This data originiates from time stamp
    // 06MAR2007 11:37:08 on cylinder serial number A09535-4, crank end
    ifstream myfile("PressureCurveAnalytics_Test01.txt");

    if( myfile.is_open() )
    {
        lArraySize = 0;
        while ( !myfile.eof())
        {
            getline (myfile,line);
            if( line.length() > 3 )
            {
                dPressureCurve[lArraySize] = atof( line.c_str() );
                lArraySize++;
                if( lArraySize > LOCALMAXPRESSUREARRAYSIZE )
                {
 		            strcpy_s( outputline, "Test file size exceeds maximum array size\n" );
		            printf( outputline );
		            fprintf(stream, outputline );
                }
            }

        }
    }
    else
    {
 		strcpy_s( outputline, "Failed to open PressureCurveAnalytics_Test01.txt for input.\n" );
		printf( outputline );
		fprintf(stream, outputline );
   }

    //close the input file
    myfile.close();
    
    //load the data, verify method traps bad barometric pressure
    if( pcurveanalytics->bLoadPressureCurveGage_USCS( dPressureCurve, -0.001, lArraySize) == false )
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS correctly trapped a negative barometric pressure.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS failed to trap a negative barometric pressure.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( pcurveanalytics->bLoadPressureCurveGage_USCS( dPressureCurve, 100, lArraySize) == false )
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS correctly trapped a large positive barometric pressure.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS failed to trap a large positive barometric pressure.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //load the data with valid presssure and barometric pressure 
    if( pcurveanalytics->bLoadPressureCurveGage_USCS( dPressureCurve, 14.600, lArraySize) == true )
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS successfully loaded data for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS failed to load data for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    if( !pcurveanalytics->bSetChamberEnd( false ) )
    {
		strcpy_s( outputline, "bSetChamberEnd failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //verify effected members
    if( !pcurveanalytics->bGetCompressionRatio( &dTemp ) )
    {
		strcpy_s( outputline, "bGetCompressionRatio failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( dTemp > 1.737 || dTemp < 1.735 )
    {
		strcpy_s( outputline, "bGetCompressionRatio returned out of range value for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //set mechancial parameters
    if( !pcurveanalytics->SetStroke_USCS( 12.590 ) )
    {
		strcpy_s( outputline, "SetStroke_USCS failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( 32.280 ) )
    {
		strcpy_s( outputline, "SetConnRodLength_USCS failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //verify that the object catches a negative kAvg
    if( pcurveanalytics->bGetClearanceVolumePercent( -1.00, &dTemp ) )
    {
		strcpy_s( outputline, "bGetClearanceVolume failed to trap a negative kAvg for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    //input kAvg for this test case
    if( !pcurveanalytics->bGetClearanceVolumePercent( 1.416, &dTemp ) )
    {
		strcpy_s( outputline, "bGetClearanceVolumePercent failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( dTemp > 0.148 || dTemp < 0.146 )
    {
		strcpy_s( outputline, "bGetClearanceVolumePercent calculation failed bounds testing PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //setup the machine geometry
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, 21.21 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreCrankEndDiameter_USCS failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, 21.21 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreHeadEndDiameter_USCS failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, 4.33 ) )
    {
		strcpy_s( outputline, "bSetPistonRodCrankEndDiameter_USCS failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, 0.00 ) )
    {
		strcpy_s( outputline, "bSetPistonRodHeadEndDiameter_USCS failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //verify displaced volume calculations
    if( !pcurveanalytics->bGetDisplacedVolumeArray( dVolArray, LOCALMAXPRESSUREARRAYSIZE ) )
    {
		strcpy_s( outputline, "bGetDisplacedVolumeArray failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( dVolArray[0] < 4890.83 || dVolArray[0] > 4890.85 )
    {
		strcpy_s( outputline, "bGetDisplacedVolumeArray calculation failed bounds testing PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //Retrieve the suction valve closure point
    if( !pcurveanalytics->bGetCrankAngleAtSuctionValveClosure( &dTemp ) )
    {
		strcpy_s( outputline, "bGetCrankAngleAtSuctionValveClosure failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( dTemp < 61.8 || dTemp > 62.2 )
    {
		strcpy_s( outputline, "bGetCrankAngleAtSuctionValveClosure calculation failed bounds testing PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //document results
    displaymembers( pcurveanalytics, stream );

    // Open the first test file.  This data originiates from time stamp
    // 07DEC2006 06:02:25 on cylinder serial number A09535-4, head end
    ifstream myfile02("PressureCurveAnalytics_Test02.txt");

    if( myfile02.is_open() )
    {
        lArraySize = 0;
        while ( !myfile02.eof())
        {
            getline (myfile02,line);
            if( line.length() > 3 )
            {
                dPressureCurve[lArraySize] = atof( line.c_str() );
                lArraySize++;
                if( lArraySize > LOCALMAXPRESSUREARRAYSIZE )
                {
 		            strcpy_s( outputline, "Test file size exceeds maximum array size\n" );
		            printf( outputline );
		            fprintf(stream, outputline );
                }
            }

        }
    }
    else
    {
 		strcpy_s( outputline, "Failed to open PressureCurveAnalytics_Test02.txt for input.\n" );
		printf( outputline );
		fprintf(stream, outputline );
   }

    //close the input file
    myfile02.close();

    //load the data with valid presssure and barometric pressure 
    if( pcurveanalytics->bLoadPressureCurveGage_USCS( dPressureCurve, 14.600, lArraySize) == true )
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS successfully loaded data for PressureCurveAnalytics_Test02.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
		strcpy_s( outputline, "bLoadPressureCurveGage_USCS failed to load data for PressureCurveAnalytics_Test02.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //this is the head end chamber
    if( !pcurveanalytics->bSetChamberEnd( true ) )
    {
		strcpy_s( outputline, "bSetChamberEnd failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //Retrieve the suction valve closure point
    if( !pcurveanalytics->bGetCrankAngleAtSuctionValveClosure( &dTemp ) )
    {
		strcpy_s( outputline, "bGetCrankAngleAtSuctionValveClosure failed for PressureCurveAnalytics_Test01.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }
    if( dTemp < 250 || dTemp > 255 )
    {
		strcpy_s( outputline, "bGetCrankAngleAtSuctionValveClosure calculation failed bounds testing PressureCurveAnalytics_Test02.txt.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete pcurveanalytics;
        fclose( stream );
        return;
    }

    //Testing of CPressureCurveAnalytics complete, free memory
    delete pcurveanalytics;

    ///////////////////////////////////////////////////////////////////
    // End-of-program housekeeping functions
    ///////////////////////////////////////////////////////////////////
    strcpy_s( outputline, "****************Harness tests successfully completed.********************\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //Document destruction of object
    printf("CCompExpCurve Destructed.\n" );
    fprintf(stream, "CCompExpCurve Destructed.\n" );

    //close the output file
    fclose( stream );

    //All done
    return;
}
