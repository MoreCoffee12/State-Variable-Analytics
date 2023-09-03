
////////////////////////////////////////////////////
// This program serves as a test harness for
// the crank slider class
////////////////////////////////////////////////////
//
////////////////////////////////////////////////////
// Written by:	Brian Howard
// Date:		18 Dec 2001
////////////////////////////////////////////////////
//
// Minor updates
//
// Brian howard, September 2007
//
////////////////////////////////////////////////////
//
// Updated to Visual Studio 2008 Version 9.0.30729.1 
//            Microsoft .NET Framework
//            Version 3.5 SP1
//
// Brian Howard, May 2010
//
////////////////////////////////////////////////////
//
// Revised harness to test for additions to the
// CCrankSlider object
//
// 14 July 2010
// Brian Howard
//
////////////////////////////////////////////////////
//
// Revised harness to test for correct calculation of
// inertia load under crankshaft angular acceleration
//
// 27 Aug 2014
// Brian Howard
//
////////////////////////////////////////////////////



#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <oleauto.h>
#include <math.h>
#include "crankslider.h"

#define TEMPLINESIZE 1024
#define OUTPUTLINESIZE 4096

//display the members of the crankslider object
bool displaymembers( CCrankSlider *crankslidercurrent, FILE *stream )
{

    //local variables
    char outputline[OUTPUTLINESIZE];
    char templine[TEMPLINESIZE];
    int i;
    double dTemp;

    //initialize local variables
    strcpy_s(outputline, "");
    strcpy_s(templine, "" );
    i = 0;

	//Begin by outputting column headings
	strcpy_s( outputline, "Stroke (in), CrankRadius (in),ConnRod Length (in),RPM, Piston&Rod Weight (lbf), Crosshead Weight (lbf), Displacement(in),Velocity(in/s), Acceleration(in/s2), GasLoad (lbf), InertiaLoad (lbf), InertiaLoad with ang. acc.(lbf), CombinedLoad (lbf), Vertical Force (lbf), MainBearingRadialForce (lbf), MainBearingTangentForce(lbf), MainBearingTorque (ft-lb)," );
	strcat_s( outputline, "Stroke (cm), CrankRadius (cm),ConnRod Length (cm),RPM, Piston&Rod Weight (N), Crosshead Weight (N), Displacement(cm),Velocity(cm/s), Acceleration(cm/s2), GasLoad (N), InertiaLoad (N),  InertiaLoad with ang. acc. (N), CombinedLoad (N), Vertical Force (N), MainBearingRadialForce (N), MainBearingTangentForce (N), MainBearingTorque (Nm)," );
	strcat_s( outputline, "ConnRodAngleDegrees, ConnRodAngleRadians, CrankAngleDegrees, CrankAngleRadians, PistonAngleDegrees, RadiansPerSecond\n" );

     //output the formatted streams
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin withh USCS crank-slider dims
	printf( "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", crankslidercurrent->GetStroke_USCS(),
                                    crankslidercurrent->Getr_USCS(),
                                    crankslidercurrent->GetConnRodLength_USCS(),
                                    crankslidercurrent->GetRPM(),
                                    crankslidercurrent->GetPistonAndRodWeight_USCS(),
                                    crankslidercurrent->GetCrossheadWeight_USCS(),
                                    crankslidercurrent->GetSliderDisplacement_USCS(),
                                    crankslidercurrent->GetSliderVelocity_USCS(),
                                    crankslidercurrent->GetSliderAcceleration_USCS(),
                                    crankslidercurrent->GetSliderGasLoad_USCS(),
                                    crankslidercurrent->GetSliderInertiaLoad_USCS(),
                                    crankslidercurrent->GetSliderInertiaLoadWithAngAcc_USCS(),
                                    crankslidercurrent->GetSliderCombinedLoad_USCS(),
                                    crankslidercurrent->GetCrossheadVerticalForce_USCS(),
                                    crankslidercurrent->GetMainBearingRadialForce_USCS(),
                                    crankslidercurrent->GetMainBearingTangentForce_USCS(),
                                    crankslidercurrent->GetMainBearingTorque_USCS() );
     fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,", crankslidercurrent->GetStroke_USCS(),
                                    crankslidercurrent->Getr_USCS(),
                                    crankslidercurrent->GetConnRodLength_USCS(),
                                    crankslidercurrent->GetRPM(),
                                    crankslidercurrent->GetPistonAndRodWeight_USCS(),
                                    crankslidercurrent->GetCrossheadWeight_USCS(),
                                    crankslidercurrent->GetSliderDisplacement_USCS(),
                                    crankslidercurrent->GetSliderVelocity_USCS(),
                                    crankslidercurrent->GetSliderAcceleration_USCS(),
                                    crankslidercurrent->GetSliderGasLoad_USCS(),
                                    crankslidercurrent->GetSliderInertiaLoad_USCS(),
                                    crankslidercurrent->GetSliderInertiaLoadWithAngAcc_USCS(),
                                    crankslidercurrent->GetSliderCombinedLoad_USCS(),
                                    crankslidercurrent->GetCrossheadVerticalForce_USCS(),
                                    crankslidercurrent->GetMainBearingRadialForce_USCS(),
                                    crankslidercurrent->GetMainBearingTangentForce_USCS(),
                                    crankslidercurrent->GetMainBearingTorque_USCS() );

	//Now SI crank-slider dims
	printf( "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,%f,", crankslidercurrent->GetStroke_SI(),
                                    crankslidercurrent->Getr_SI(),
                                    crankslidercurrent->GetConnRodLength_SI(),
                                    crankslidercurrent->GetRPM(),
                                    crankslidercurrent->GetPistonAndRodWeight_SI(),
                                    crankslidercurrent->GetCrossheadWeight_SI(),
                                    crankslidercurrent->GetSliderDisplacement_SI(),
                                    crankslidercurrent->GetSliderVelocity_SI(),
                                    crankslidercurrent->GetSliderAcceleration_SI(),
                                    crankslidercurrent->GetSliderGasLoad_SI(),
                                    crankslidercurrent->GetSliderInertiaLoad_SI(),
                                    crankslidercurrent->GetSliderInertiaLoadWithAngAcc_SI(),
                                    crankslidercurrent->GetSliderCombinedLoad_SI(),
                                    crankslidercurrent->GetCrossheadVerticalForce_SI(),
                                    crankslidercurrent->GetMainBearingRadialForce_SI(),
                                    crankslidercurrent->GetMainBearingTangentForce_SI(),
                                    crankslidercurrent->GetMainBearingTorque_SI() );
	fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,", crankslidercurrent->GetStroke_SI(),
                                    crankslidercurrent->Getr_SI(),
                                    crankslidercurrent->GetConnRodLength_SI(),
                                    crankslidercurrent->GetRPM(),
                                    crankslidercurrent->GetPistonAndRodWeight_SI(),
                                    crankslidercurrent->GetCrossheadWeight_SI(),
                                    crankslidercurrent->GetSliderDisplacement_SI(),
                                    crankslidercurrent->GetSliderVelocity_SI(),
                                    crankslidercurrent->GetSliderAcceleration_SI(),
                                    crankslidercurrent->GetSliderGasLoad_SI(),
                                    crankslidercurrent->GetSliderInertiaLoad_SI(),
                                    crankslidercurrent->GetSliderInertiaLoadWithAngAcc_SI(),
                                    crankslidercurrent->GetSliderCombinedLoad_SI(),
                                    crankslidercurrent->GetCrossheadVerticalForce_SI(),
                                    crankslidercurrent->GetMainBearingRadialForce_SI(),
                                    crankslidercurrent->GetMainBearingTangentForce_SI(),
                                    crankslidercurrent->GetMainBearingTorque_SI() );

	//And finally, the angular position crank-slider dims
	printf( "%f, %f, %f, %f, ", crankslidercurrent->GetConnRodAngleDegrees(),
                                    crankslidercurrent->GetConnRodAngleRadians(),
                                    crankslidercurrent->GetCrankAngleDegrees(),
                                    crankslidercurrent->GetCrankAngleRadians() );
	fprintf( stream, "%f, %f, %f, %f, ", crankslidercurrent->GetConnRodAngleDegrees(),
							        crankslidercurrent->GetConnRodAngleRadians(),
							        crankslidercurrent->GetCrankAngleDegrees(),
							        crankslidercurrent->GetCrankAngleRadians() );

     //These critters are needed for torque/time calcs
     printf( "%f, %f\n", crankslidercurrent->GetPistonAngleDegrees(),
                                   crankslidercurrent->GetRadiansPerSecond() );
     fprintf( stream, " %f, %f\n", crankslidercurrent->GetPistonAngleDegrees(),
                                   crankslidercurrent->GetRadiansPerSecond() );

     //Reset the output line buffer
     strcpy_s( outputline, "" );

     //headers for the ambient pressure
     strcat_s( outputline, "Ambient Pressure (in2), Ambient Pressure (cm2)," );

     //These lines are for the chamber configuration
     //Begin by outputting column headings
     for( i=0; i<crankslidercurrent->GetPistonArea_MaxCount(); i++ )
     {
	     _itoa_s( ( i+1 ), templine, 10 );
         strcat_s( outputline, "Crank End Area " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (in2), " );
         strcat_s( outputline, "Head End Area " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (in2), " );
         strcat_s( outputline, "Crank End Bore " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (in), " );
         strcat_s( outputline, "Head End Bore " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (in), " );
         strcat_s( outputline, "Crank End Rod Dia. " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (in), " );
         strcat_s( outputline, "Head End Bore Dia." );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (in), " );
     }
    for( i=0; i<crankslidercurrent->GetPistonArea_MaxCount(); i++ )
    {
        _itoa_s( ( i+1 ), templine, 10 );
        strcat_s( outputline, "Crank End Area " );
        strcat_s( outputline, templine );
        strcat_s( outputline, " (cm2), " );
        strcat_s( outputline, "Head End Area" );
        strcat_s( outputline, templine );
        strcat_s( outputline, " (cm2), " );
         strcat_s( outputline, "Crank End Bore " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (cm), " );
         strcat_s( outputline, "Head End Bore " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (cm), " );
         strcat_s( outputline, "Crank End Rod Dia. " );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (cm), " );
         strcat_s( outputline, "Head End Bore Dia." );
         strcat_s( outputline, templine );
         strcat_s( outputline, " (cm), " );
    }
    strcat_s( outputline, "\n" );
    printf( outputline );
    fprintf(stream, outputline );

     //Reset the output line buffer
     strcpy_s( outputline, "" );

     //ambient pressure output
     if( crankslidercurrent->bGetAmbientPressure_USCS( &dTemp ) == false )
     {
         strcpy_s( outputline, "bGetAmbientPressure_USCS Failed." );
         printf( outputline );
	     fprintf(stream, outputline );
     }
     _gcvt_s( templine, TEMPLINESIZE, dTemp, 10 ); 
     strcat_s( outputline, templine );
     strcat_s( outputline, ", " );

     if( crankslidercurrent->bGetAmbientPressure_SI( &dTemp ) == false )
     {
         strcpy_s( outputline, "bGetAmbientPressure_SI Failed." );
         printf( outputline );
	     fprintf(stream, outputline );
     }
     _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
     strcat_s( outputline, templine );
     strcat_s( outputline, ", " );

     //Piston areas
     for( i=0; i<crankslidercurrent->GetPistonArea_MaxCount(); i++ )
     {
          _gcvt_s( templine, TEMPLINESIZE, crankslidercurrent->GetPistonAreaCrankEnd_USCS( (i+1) ), 10 ); 
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          _gcvt_s( templine, TEMPLINESIZE,crankslidercurrent->GetPistonAreaHeadEnd_USCS( (i+1) ), 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetCylinderBoreHeadEndDiameter_USCS( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetCylinderBoreCrankEndDiameter_USCS( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetPistonRodHeadEndDiameter_USCS( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetPistonRodCrankEndDiameter_USCS( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
     }
     for( i=0; i<crankslidercurrent->GetPistonArea_MaxCount(); i++ )
     {
          _gcvt_s( templine, TEMPLINESIZE, crankslidercurrent->GetPistonAreaCrankEnd_SI( (i+1) ), 10 );
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          _gcvt_s( templine, TEMPLINESIZE, crankslidercurrent->GetPistonAreaHeadEnd_SI( (i+1) ), 10 );
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetCylinderBoreHeadEndDiameter_SI( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetCylinderBoreCrankEndDiameter_SI( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetPistonRodHeadEndDiameter_SI( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          crankslidercurrent->bGetPistonRodCrankEndDiameter_SI( (i+1) , &dTemp );
          _gcvt_s( templine, TEMPLINESIZE, dTemp, 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
     }
     strcat_s( outputline, "\n" );
     printf( outputline );
	 fprintf(stream, outputline );

     //reset the output line buffer
     strcpy_s( outputline, "" );
     
     //spaces to keep data aligned as these columns are
     //reserved for the ambient pressures
     strcat_s( outputline, ",,");

     //Chamber pressure output
     for( i=0; i<crankslidercurrent->GetPistonArea_MaxCount(); i++ )
     {
          _gcvt_s( templine, TEMPLINESIZE, crankslidercurrent->GetPressureAbsCrankEnd_USCS( (i+1) ), 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          _gcvt_s( templine, TEMPLINESIZE, crankslidercurrent->GetPressureAbsHeadEnd_USCS( (i+1) ), 10);
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
     }
     for( i=0; i<crankslidercurrent->GetPistonArea_MaxCount(); i++ )
     {
          _gcvt_s( templine, TEMPLINESIZE, crankslidercurrent->GetPressureAbsCrankEnd_SI( (i+1) ), 10 );
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
          _gcvt_s( templine, TEMPLINESIZE, crankslidercurrent->GetPressureAbsHeadEnd_SI( (i+1) ), 10 );
          strcat_s( outputline, templine );
          strcat_s( outputline, ", " );
     }
     strcat_s( outputline, "\n" );
     printf( outputline );
	 fprintf(stream, outputline );

     //success
	 return true;
}

// house-keeping and close-out tasks
bool CloseOut(CCrankSlider *crankslidercurrent,
              CCrankSlider *crankslidernext,
              FILE *stream)
{
    //Free all elements in the crank slider objects
	delete crankslidercurrent;
	delete crankslidernext;

	//Document destruction of object
	printf("CCrankSlider Destructed.\n" );
	fprintf(stream, "CCrankSlider Destructed.\n" );

    //close the output file
	fclose( stream );

    return true;
}

bool bInsertLocalTime ( FILE *stream, int *iCurrentTickCount, int *iPreviousTickCount )
{

    //local variables
    SYSTEMTIME SysTime;
    int iLapsedTicks;

    GetLocalTime( &SysTime );
    *iPreviousTickCount = *iCurrentTickCount;
    *iCurrentTickCount = GetTickCount();
    iLapsedTicks = ( *iCurrentTickCount - *iPreviousTickCount );
    fprintf(stream, "%i:%02i:%02i:%003i\n",
    (int)SysTime.wHour,
    (int)SysTime.wMinute,
    (int)SysTime.wSecond,
    (int)SysTime.wMilliseconds);
    fprintf(stream, "Number of milliseconds: %i\n", iLapsedTicks );

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
	class CCrankSlider *crankslidercurrent;
	class CCrankSlider *crankslidernext;
	char outputline[OUTPUTLINESIZE];
	double testvalue;
	double dTemp;
    double dtemp02;
    double dExpected;
	double xarray[10];
	double yarray[10];
	double r;
	int iCurrentTickCount;
    int iPreviousTickCount;
    errno_t err;

	//initialize local variables
    i = 0;
    j = 0;
	stream = NULL;
	crankslidercurrent = NULL;
	crankslidernext = NULL;
    strcpy_s(outputline, "");
	testvalue = 3.14159;
	dTemp = 0;
    dtemp02 = 0;
    dExpected = 0.0;
	for(i=0;i<10;i++)
	{
		xarray[i] = 0;
		yarray[i]= 0;
	}
	r = 0;
	iCurrentTickCount = GetTickCount();
    iPreviousTickCount = GetTickCount();

	/////////////////////////////////////////////////////////////////////
    // The remainder of this program serves as a container and harness
	// to test the object members and methods.  Tests also
	// include exception handling
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// Test Sequence
	//	- Verify class instantiation
	//	- Verify member initialization
	//	- Verify method performance
	/////////////////////////////////////////////////////////////////////

	//Open the log file
	err = fopen_s( &stream, "CCrankSliderHarness.csv" , "w" );
    if( err != 0 )
    {
        printf ( "Failed to open CCrankSliderHarness.csv for write\n" );
        return;
    }

	//Intro line
	strcpy_s( outputline, "CCrankSlider Test Harness.\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Instantiate the new object
	crankslidercurrent	= new CCrankSlider();

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }


    //Document instantiation of class
    if( crankslidercurrent == NULL )
    {
        return;
    }
    else
    {
        strcpy_s( outputline, "CCrankSlider Instantiated.\n" );
        printf( outputline );
        fprintf(stream, outputline );
    }

	/////////////////////////////////////////////////////////////////////
    // Documemt member values as found on class instantiation
	/////////////////////////////////////////////////////////////////////
	if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    /////////////////////////////////////////////////////////////////////
    // We'll load a negative crank radius length to see if error is
    // caught using USCS.
    //
    // Revision, 28 August 2014
    // Restructured test to focus only on the "SetStroke_USCS" method.
    /////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_USCS( -10 ) )
	{
		strcpy_s( outputline, "SetStroke_USCS caught the -10 stroke length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}
	else
	{
		strcpy_s( outputline, "SetStroke_USCS didn't catch the -10 stroke length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    /////////////////////////////////////////////////////////////////////
    // We'll load a negative crank radius length to see if error is
    // caught using SI.
    /////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_SI( -10 ) )
	{
		strcpy_s( outputline, "SetStroke_SI caught the -10 stroke length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}
	else
	{
		strcpy_s( outputline, "SetStroke_SI didn't catch the -10 stroke length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load a negative conn rod angle length to see if error is
	// caught.
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetConnRodLength_USCS( -25 ) )
	{
		strcpy_s( outputline, "SetConnRodLength_USCS caught the -25 conn rod length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}
	else
	{
		strcpy_s( outputline, "SetConnRodLength_USCS didn't catch the -25 conn rod length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load stroke that is longer that the connrod to see if error 
	// is caught.  
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_USCS ( 25 ) )
    {
		strcpy_s( outputline, "SetStroke_USCS failed to load a valid length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 10 ) )
    {
		strcpy_s( outputline, "SetConnRodLength_USCS failed to load a valid length.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->CalcSliderDisplacement() )
	{
		strcpy_s( outputline, "CalcSliderDisplacement caught the 25/5 r over l.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}
	else
	{
		strcpy_s( outputline, "CalcSliderDisplacement didn't catch the 25/5 r over l.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load a negative RPM for the beginning of the revolution and
    // be sure that is caught.
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_USCS ( 10 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 25 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( crankslidercurrent->SetRPM( -100 ) )
	{
		strcpy_s( outputline, "CalcSliderDisplacement failed to catch the negative RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}
	else
	{
		strcpy_s( outputline, "CalcSliderDisplacement caught the negative RPM at the beginning of the revolution\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load a negative RPM for the ending of the revolution and
    // be sure that is caught.
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_USCS ( 10 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement failed to load a valid stroke while checking for negative RPm at end of revolution\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 25 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement failed to load a valid stroke while checking for negative RPm at end of revolution\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( crankslidercurrent->SetRPM01( -100 ) )
	{
		strcpy_s( outputline, "CalcSliderDisplacement failed to catch the negative RPM for the second revolution\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "CalcSliderDisplacement caught the negative RPM at the ending of the revolution\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    /////////////////////////////////////////////////////////////////////
    // Revision - 3 Sep 2014
    // Added this test to validate piston angle functions
    /////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetPistonAngleDegrees( 90 ))
    {
		strcpy_s( outputline, "SetPistonAngleDegrees failed to set an angle of 90 degrees (it should have succeeded).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    dTemp = crankslidercurrent->GetPistonAngleDegrees();
    if( dTemp < 89.999999 || dTemp > 90.0000001 )
    {
		strcpy_s( outputline, "GetPistonAngleRadians should have returned 90 degrees, but failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    dTemp = crankslidercurrent->GetPistonAngleRadians();
    if( dTemp < 1.57079 || dTemp > 1.57080 )
    {
		strcpy_s( outputline, "GetPistonAngleRadians should have returned pi/2 radians, but failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    /////////////////////////////////////////////////////////////////////
    // In U.S. Customary Units (USCS) the ambient pressure is bounded
    // between 0, a vacuum, and 20 psig, a very deep hole.  The following
    // section of code validates that these bounds are checked.
    /////////////////////////////////////////////////////////////////////
    dTemp = 20.001;
    if( crankslidercurrent->bSetAmbientPressure_USCS( dTemp ) == false )
	{
		fprintf(stream, "bSetAmbientPressure_USCS caught the %f incorrect ambient pressure.\n", dTemp );
	}
	else
	{
        fprintf(stream, "bSetAmbientPressure_USCS failed to catch the %f incorrect ambient pressure.\n", dTemp );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}
    dTemp = -0.0001;
    if( crankslidercurrent->bSetAmbientPressure_USCS( dTemp ) == false )
	{
		fprintf(stream, "bSetAmbientPressure_USCS caught the %f incorrect ambient pressure.\n", dTemp );
	}
	else
	{
        fprintf(stream, "bSetAmbientPressure_USCS failed to catch the %f incorrect ambient pressure.\n", dTemp );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}


    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load a good set of data and make sure everything is working
    // Begin by checking displacement. 
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 36 );
    crankslidercurrent->SetConnRodLength_USCS( 47.26 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetRPM( 227 );
	
    // Did the calculation succeed?
    if( crankslidercurrent->CalcSliderDisplacement() == false )
    {
		strcpy_s( outputline, "CalcSliderDisplacement failed on a valid data set.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
	
    // Is the value right?  See "CrankDynamics_Euler.doc" for the 
    // derivation of the expected value.
    dExpected = 7.018260;
    dTemp = crankslidercurrent->GetSliderDisplacement_USCS();
    if( dTemp > ( dExpected * 1.001 ) )
    {
		printf( "GetSliderDisplacement_USCS() returned incorrect value." );
		fprintf(stream, "GetSliderDisplacement_USCS() returned %f which is greater than the expected value of %f.\n", dTemp, dExpected );

        if( !displaymembers( crankslidercurrent, stream ) )
	    {
		    strcpy_s( outputline, "Member output failed.\n" );
		    printf( outputline );
		    fprintf(stream, outputline );
            CloseOut( crankslidercurrent, crankslidernext, stream );
		    return;
	    }
        
        return;
    }
    if( dTemp < ( dExpected * 0.999 ) )
    {
		printf( "GetSliderDisplacement_USCS() returned incorrect value." );
		fprintf(stream, "GetSliderDisplacement_USCS() returned %f which is less than the expected value of %f.\n", dTemp, dExpected );
        
        if( !displaymembers( crankslidercurrent, stream ) )
	    {
		    strcpy_s( outputline, "Member output failed.\n" );
		    printf( outputline );
		    fprintf(stream, outputline );
            CloseOut( crankslidercurrent, crankslidernext, stream );
		    return;
	    }
        
        return;
    }

    //success
    strcpy_s( outputline, "Displacement calculation succeded\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // Now for slider velocity
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 36 );
    crankslidercurrent->SetConnRodLength_USCS( 47.26 );
    crankslidercurrent->SetCrankAngleDegrees( 3 );
    crankslidercurrent->SetRPM( 227 );
	if( !crankslidercurrent->CalcSliderVelocity() ||
          ( ( crankslidercurrent->GetSliderVelocity_USCS() > -30 ) || (crankslidercurrent->GetSliderVelocity_USCS() < -31 ) ) )
	{
		strcpy_s( outputline, "CalcSliderVelocity_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Velocity (Should be real close to 30.91 in/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // Now for slider acceleration
	/////////////////////////////////////////////////////////////////////
	if( !crankslidercurrent->CalcSliderAcceleration() ||
          ( ( crankslidercurrent->GetSliderAcceleration_USCS() < -14014.0 ) || (crankslidercurrent->GetSliderAcceleration_USCS() > -14012.0 ) ) )
	{
        strcpy_s( outputline, "CalcSliderAcceleration_USCS failed when it should have succeeded.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAcceleration_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}
	else
	{
		strcpy_s( outputline, "Acceleration (Should be real close to 14013.0 in/s/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // Now for slider acceleration at 340 degrees (another quadrant)
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetCrankAngleDegrees( 340 );
	if( !crankslidercurrent->CalcSliderAcceleration() ||
          ( ( crankslidercurrent->GetSliderAcceleration_USCS() < -12612.0 ) || (crankslidercurrent->GetSliderAcceleration_USCS() > -12610.0 ) ) )
	{
        strcpy_s( outputline, "CalcSliderAcceleration_USCS failed when it should have succeeded.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        fprintf( stream, "Acceleration (Should be real close to 12611 in/s/s) is: %f\n", crankslidercurrent->GetSliderAcceleration_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
     }
	else
	{
        strcpy_s( outputline, "Acceleration (Should be real close to 12611 in/s/s) \n" );
        printf( outputline );
        fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    //restore to 3 degrees for the remainder of the tests
    crankslidercurrent->SetCrankAngleDegrees( 3 );

	/////////////////////////////////////////////////////////////////////
    // Crank slider acceleration with crankshaft angular acceleration. 
    // Test is based on K-9580, CVX Pascagoula, 0 degrees and 
    // ignore the piston angle
    //
    // Revision, 27 Aug 2014
    // Added test
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetPistonAngleDegrees( 0 ))
    {
		strcpy_s( outputline, "SetPistonAngleDegrees failed to set an angle of 0 degrees (it should have succeeded).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetStroke_USCS ( 8.5 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetStroke_USCS failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 17 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetConnRodLength_USCS failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( 0 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetCrankAngleDegrees failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM( 437 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM01( 387 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM01 failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    // Call the caclulation, document the results.
	if( !crankslidercurrent->CalcSliderAccelerationWithAngAcc() ||
          ( ( crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() < -11126 ) || (crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() > -11125 ) ) )
	{
        strcpy_s( outputline, "CalcSliderAccelerationWithAngAcc_USCS failed when it should have succeeded.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAcceleration_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}
	else
	{
		strcpy_s( outputline, "Reciprocating acceleration for crankshaft angular acceleration (Should be real close to -11125.5 in/s/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // Crank slider acceleration with crankshaft angular acceleration. 
    // Test is based on K-9580, CVX Pascagoula, 0 degrees and with
    // the correct piston angle (270 degrees).
    //
    // Revision, 27 Aug 2014
    // Added test
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetPistonAngleDegrees( 270 ))
    {
		strcpy_s( outputline, "SetPistonAngleDegrees failed to set an angle of 270 degrees (it should have succeeded).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetStroke_USCS ( 8.5 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetStroke_USCS failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 17 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetConnRodLength_USCS failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( 0 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetCrankAngleDegrees failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM( 437 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM01( 387 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM01 failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    // Call the calculation, make sure it works without error
	if( !crankslidercurrent->CalcSliderAccelerationWithAngAcc() )
	{
        strcpy_s( outputline, "CalcSliderAccelerationWithAngAcc() failed when it should have succeeded (CVX data, 0 degrees with piston angle).\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}

    // Call the calculation, document the results.
	if(  ( crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() < -9325.33 ) || (crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() > -9325.30 ) )
	{
        strcpy_s( outputline, "CalcSliderAccelerationWithAngAcc_USCS failed when it should have succeeded (CVX data, 0 degrees with piston angle).\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}
	else
	{
		strcpy_s( outputline, "Reciprocating acceleration for crankshaft angular acceleration (Should be real close to -9325.31 in/s/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

    ////////////////////////////////////////////////////////////////////
    // Crank slider acceleration with crankshaft angular acceleration. 
    // Test is based on K-9580, CVX Pascagoula, 45 degrees and 
    // a piston angle of 0.  This doesn't match the System 1 configuration,
    // but it is useful for testing.
    //
    // Revision, 27 Aug 2014
    // Added test
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetPistonAngleDegrees( 0 ))
    {
		strcpy_s( outputline, "SetPistonAngleDegrees failed to set an angle of 0 degrees (it should have succeeded).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetStroke_USCS ( 8.5 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetStroke_USCS failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 17 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetConnRodLength_USCS failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( 45 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetCrankAngleDegrees failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM( 437 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM01( 387 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM01 failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    // Call the calculation, it should succeed
	if( !crankslidercurrent->CalcSliderAccelerationWithAngAcc() )
	{
        strcpy_s( outputline, "CalcSliderAccelerationWithAngAcc() failed when it should have succeeded ( K-9580, CVX Pascagoula, 45 degrees with valid piston angle).\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}

    // This checks that the results of the calculation were accurate.
	if( ( crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() < -6031.9 ) || (crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() > -6031.7 ) )
	{
        strcpy_s( outputline, "GetSliderAccelerationWithAngAcc_USCS failed when it should have succeeded ( K-9580, CVX Pascagoula, 45 degrees with valid piston angle).\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAcceleration_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}
	else
	{
		strcpy_s( outputline, "Reciprocating acceleration for crankshaft angular acceleration (Should be real close to -6031.83 in/s/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

    ////////////////////////////////////////////////////////////////////
    // Crank slider acceleration with crankshaft angular acceleration. 
    // Test is based on K-9580, CVX Pascagoula, 45 degrees and 
    // the correct piston angle.
    //
    // Revision, 27 Aug 2014
    // Added test
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetPistonAngleDegrees( 270 ))
    {
		strcpy_s( outputline, "SetPistonAngleDegrees failed to set an angle of 270 degrees (it should have succeeded).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetStroke_USCS ( 8.5 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetStroke_USCS failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 17 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetConnRodLength_USCS failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( 45 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetCrankAngleDegrees failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM( 437 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetRPM01( 387 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM01 failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    // Call the calculation, it should succeed
	if( !crankslidercurrent->CalcSliderAccelerationWithAngAcc() )
	{
        strcpy_s( outputline, "CalcSliderAccelerationWithAngAcc() failed when it should have succeeded ( K-9580, CVX Pascagoula, 45 degrees with valid piston angle).\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}

    // This checks that the results of the calculation were accurate.
	if( ( crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() < -5007.61 ) || (crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() > -5007.59 ) )
	{
        strcpy_s( outputline, "GetSliderAccelerationWithAngAcc_USCS failed when it should have succeeded ( K-9580, CVX Pascagoula, 45 degrees with valid piston angle).\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
	}
	else
	{
		strcpy_s( outputline, "Reciprocating acceleration for crankshaft angular acceleration (Should be real close to -5007.6 in/s/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

    ////////////////////////////////////////////////////////////////////
    // Crank slider acceleration with crankshaft angular acceleration. 
    // Test is based on K-9580, CVX Pascagoula, 45 degrees, but
    // modified so that both RPM's are the same. 
    //
    // Revision, 29 Aug 2014
    // Added test
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_USCS ( 8.5 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetStroke_USCS failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 17 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetConnRodLength_USCS failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( 45 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetCrankAngleDegrees failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetRPM( 437 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetRPM01( 437 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM01 failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }

    // Call the caclulation, document the results.
	if( !crankslidercurrent->CalcSliderAccelerationWithAngAcc() ||
          ( ( crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() < -6330.1 ) || (crankslidercurrent->GetSliderAccelerationWithAngAcc_USCS() > -6329.8 ) ) )
	{
        strcpy_s( outputline, "CalcSliderAccelerationWithAngAcc_USCS failed when it should have succeeded checking same RPM values.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAcceleration_USCS() );
        return;
	}
	else
	{
		strcpy_s( outputline, "Reciprocating acceleration for crankshaft angular acceleration (Should be real close to -6329.99 in/s/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    ////////////////////////////////////////////////////////////////////
    // Crank slider acceleration with crankshaft angular acceleration. 
    // Test is based on K-9580, CVX Pascagoula, 45 degrees, but in
    // SI units to verify that function.  Piston angle is ignored.
    //
    // Revision, 28 Aug 2014
    // Added test
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetPistonAngleDegrees( 0 ))
    {
		strcpy_s( outputline, "SetPistonAngleDegrees failed to set an angle of 0 degrees (it should have succeeded, CVX Pascagoula test in SI ignoring units).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetStroke_SI ( 21.59 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetStroke_SI failed to load a valid stroke\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_SI( 43.18 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetConnRodLength_SI failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetCrankAngleDegrees( 45 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetCrankAngleDegrees failed to load a valid connecting rod length\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetRPM( 437 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetRPM01( 387 ) )
    {
		strcpy_s( outputline, "CalcSliderDisplacement->SetRPM01 failed to load a valid RPM\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }

    // Call the caclulation, document the results.
	if( !crankslidercurrent->CalcSliderAccelerationWithAngAcc() ||
          ( ( crankslidercurrent->GetSliderAccelerationWithAngAcc_SI() < -15320.9 ) || (crankslidercurrent->GetSliderAccelerationWithAngAcc_SI() > -15320.7 ) ) )
	{
        strcpy_s( outputline, "CalcSliderAccelerationWithAngAcc or GetSliderAccelerationWithAngAcc_SI failed when it should have succeeded ( K-9580, CVX Pascagoula, 45 degrees, SI Units).\n" );
        printf( outputline );
        fprintf(stream, outputline );
        fprintf( stream, "Acceleration is: %f\n", crankslidercurrent->GetSliderAcceleration_USCS() );
        return;
	}
	else
	{
		strcpy_s( outputline, "Reciprocating acceleration for crankshaft angular acceleration (Should be real close to -15320.8 cm/s/s) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    // Insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    /////////////////////////////////////////////////////////////////////
    // check that default piston areas can be loaded into the object.
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetPistonAreaCrankEnd_USCS( 1.00 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( 1.00 );
	if( ! ( ( crankslidercurrent->GetPistonAreaCrankEnd_USCS() ==  1.0 ) && ( crankslidercurrent->GetPistonAreaHeadEnd_USCS() ==  1.0 ) ) )
	{
        strcpy_s( outputline, "SetPistonAreaCrankEnd_USCS failed, in default access, when it should have succeeded.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
	}
	else
	{
		strcpy_s( outputline, "Piston areas.  Both the crank end and head end should be 1.00\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // check that additional piston areas can be loaded into the object
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetPistonAreaCrankEnd_USCS( 2.00, 2 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( 2.00, 2 );
	if( ! ( ( crankslidercurrent->GetPistonAreaCrankEnd_USCS( 2 ) ==  2.0 ) && ( crankslidercurrent->GetPistonAreaHeadEnd_USCS( 2 ) ==  2.0 ) ) )
	{
        strcpy_s( outputline, "SetPistonAreaCrankEnd_USCS failed, in adding additional areas, when it should have succeeded.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
	}
	else
	{
		strcpy_s( outputline, "Piston areas.  Both the crank end and head end on the second set should be 2.00\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // check that default piston pressures can be loaded into the object 
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( 8.88 + 14.7 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( 8.88 + 14.7 );
	if( ! ( ( crankslidercurrent->GetPressureAbsCrankEnd_USCS() ==  23.58 ) && ( crankslidercurrent->GetPressureAbsHeadEnd_USCS() ==  23.58 ) ) )
	{
		strcpy_s( outputline, "SetPressureCrankEnd_USCS failed, in default access, when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}
	else
	{
		strcpy_s( outputline, "Piston pressure.  Both the crank end and head end should be 23.58\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // check that additional piston areas can be loaded into the object 
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( 16.16, 2 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( 16.16, 2 );
	if( ! ( ( crankslidercurrent->GetPressureAbsCrankEnd_USCS( 2 ) ==  16.16 ) && ( crankslidercurrent->GetPressureAbsHeadEnd_USCS( 2 ) ==  16.16 ) ) )
	{
		strcpy_s( outputline, "SetPressureCrankEnd_USCS failed, in adding additional areas, when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}
	else
	{
		strcpy_s( outputline, "Piston areas.  Both the crank end and head end on the second set should be 16.16\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // And slider inertia load
	/////////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_USCS ( 36 ) )
    {
		strcpy_s( outputline, "In the slider inertia load check,SetStroke_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    crankslidercurrent->SetConnRodLength_USCS( 47.26 );
    crankslidercurrent->SetCrankAngleDegrees( 3 );
    crankslidercurrent->SetRPM( 227 );
    crankslidercurrent->SetRPM01( 227 );
    crankslidercurrent->SetPistonAndRodWeight_USCS( 664.0 + 590.0 );
    crankslidercurrent->SetCrossheadWeight_USCS( 1106.0 );
    //This is a bit of a hidden test, but by iterating through
    //the function about a grillion times, we can estimate how
    //long each calculation takes and see if changes made to the
    //object improve performance
	strcpy_s( outputline, "Beginning CalcSliderInertiaLoad Performance Test Section.\n" );
	printf( outputline );
	fprintf(stream, outputline );

    //the inertia
    for(i=0;i<10000000;i++)
    {
      crankslidercurrent->CalcSliderInertiaLoad();
    }
	if( !crankslidercurrent->CalcSliderInertiaLoad() ||
          ( ( crankslidercurrent->GetSliderInertiaLoad_USCS() > 85585.0 ) || (crankslidercurrent->GetSliderInertiaLoad_USCS() < 85582.0 ) ) )
	{
		strcpy_s( outputline, "CalcSliderInertiaLoad_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}
	else
	{
		strcpy_s( outputline, "Inertia Load (Should be real close to 85584.0 lbf) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
     }

	/////////////////////////////////////////////////////////////////////
    // Slider inertia load using the K-9580 CVX Pascagoula data at 45
    // degrees and allowing for angular acceleration of the crankshaft 
    // in USCS.
    //
    // Revision, 28 August 2014
    // Add this test
	/////////////////////////////////////////////////////////////////////
    fprintf(stream, "\n" );
    if( !crankslidercurrent->SetStroke_USCS ( 8.5 ) )
    {
		strcpy_s( outputline, "In the slider inertia load check with angular acceleration, SetStroke_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 17 ) )
    {
		strcpy_s( outputline, "In the slider inertia load check with angular acceleration, SetConnRodLength_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
    }
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetRPM( 437 );
    crankslidercurrent->SetRPM01( 387 );
    crankslidercurrent->SetPistonAndRodWeight_USCS( 500.0 + 100.0 );
    crankslidercurrent->SetCrossheadWeight_USCS( 148 );

    // Call the calculation and see if it succeded.
    if( !crankslidercurrent->CalcSliderInertiaLoadWithAngAcc() )
	{
		strcpy_s( outputline, "CalcSliderInertiaLoadWithAngAcc failed when it should have succeeded, crankshaft with angular acceleration (K-9580 CVX Pascagoula data at 45 degrees).\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}
    // Bounds checking
    if( crankslidercurrent->GetSliderInertiaLoadWithAngAcc_USCS() > 11676.6  || crankslidercurrent->GetSliderInertiaLoadWithAngAcc_USCS() < 11676.4 )
	{
		strcpy_s( outputline, "GetSliderInertiaLoadWithAngAcc_USCS failed when it should have succeeded, crankshaft with angular acceleration (K-9580 CVX Pascagoula data at 45 degrees).\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}
	else
	{
		strcpy_s( outputline, "Inertia Load (Should be real close to 11676.5 lbf) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
     }

    ////////////////////////////////////////////////////////////////////
    // Crank slider inertia load with crankshaft angular acceleration. 
    // Test is based on Waveform ID 4, but with the second RPM at 100.
    // Crank angle is set for 282 degrees
    // In the MS Access application these parameters resulted in
    // an overflow error.
    //
    // Revision, 29 Aug 2014
    // Added test
	/////////////////////////////////////////////////////////////////////
    fprintf(stream, "\n" );
    if( !crankslidercurrent->SetStroke_USCS ( 18 ) )
    {
		strcpy_s( outputline, "In the slider inertia load check with angular acceleration, SetStroke_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    if( !crankslidercurrent->SetConnRodLength_USCS( 47.26 ) )
    {
		strcpy_s( outputline, "In the slider inertia load check with angular acceleration, SetConnRodLength_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
    crankslidercurrent->SetCrankAngleDegrees( 282 );
    crankslidercurrent->SetRPM( 277 );
    crankslidercurrent->SetRPM01( 100 );
    crankslidercurrent->SetPistonAndRodWeight_USCS( 300.0 + 954 );
    crankslidercurrent->SetCrossheadWeight_USCS( 1106 );

    // Call the calculation and see if it succeded.
    if( !crankslidercurrent->CalcSliderInertiaLoadWithAngAcc() )
	{
		strcpy_s( outputline, "CalcSliderInertiaLoadWithAngAcc failed when it should have succeeded, crankshaft with angular acceleration (Waveform ID 4).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
    // Bounds checking
    if( crankslidercurrent->GetSliderInertiaLoadWithAngAcc_USCS() > 3716.4  || crankslidercurrent->GetSliderInertiaLoadWithAngAcc_USCS() < 3716.2 )
	{
		strcpy_s( outputline, "GetSliderInertiaLoadWithAngAcc_USCS failed when it should have succeeded, crankshaft with angular acceleration (Waveform ID 4).\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Inertia Load (Should be real close to 3716.3 lbf) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

	/////////////////////////////////////////////////////////////////////
    // reset the chamber configuration 
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 10 );
    crankslidercurrent->SetConnRodLength_USCS( 25 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetRPM( 227 );
    crankslidercurrent->SetRPM01( 227 );
    crankslidercurrent->SetPistonAndRodWeight_USCS( 664.0 + 590.0 );
    crankslidercurrent->SetCrossheadWeight_USCS( 1106.0 );
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( true );
    crankslidercurrent->SetPistonAreaCrankEnd_USCS( crankslidercurrent->CalcPistonArea( 19.00, 5.00) , 1 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( crankslidercurrent->CalcPistonArea( 19.00, 0.00) , 1 );
    crankslidercurrent->SetPistonAreaCrankEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( 311.28, 1 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( 642.22, 1 );
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( 0.00, 2 );
    if( crankslidercurrent->bSetAmbientPressure_USCS( 14.696 ) == false )
    {
        strcpy_s( outputline, "bSetAmbientPressure_USCS failed when it should have succeeded.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

	/////////////////////////////////////////////////////////////////////
    // reset the chamber configuration and test gas load calculation
	/////////////////////////////////////////////////////////////////////
    //This is a bit of a hidden test, but by iterating through
    //the function about a grillion times, we can estimate how
    //long each calculation takes and see if changes made to the
    //object improve performance
	strcpy_s( outputline, "Beginning CalcSliderGasLoad Performance Test Section.\n" );
	printf( outputline );
	fprintf(stream, outputline );

     //the loop
     for(i=0;i<10000000;i++)
     {
          crankslidercurrent->CalcSliderGasLoad();
     }
     if( !crankslidercurrent->CalcSliderGasLoad() ||
          ( ( crankslidercurrent->GetSliderGasLoad_USCS() > -99650.0 ) || (crankslidercurrent->GetSliderGasLoad_USCS() < -99660.0 ) ) )
	{
		strcpy_s( outputline, "CalcSliderGasLoad_USCS failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Gas Load (Should be real close to -99654.0 lbf) \n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
     if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

    /////////////////////////////////////////////////////////////////////
    // We'll load a good set of data for the conn rod angle calculation 
    // for a throw on a CCW rotating crankshaft on the right side of the
    // compressor.
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 10 );
    crankslidercurrent->SetConnRodLength_USCS( 25 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( true );
	if( !crankslidercurrent->CalcConnRodAngle() )
	{
		strcpy_s( outputline, "CalcConnRodAngle failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Connecting Rod Angle, CCW rotation, Right side of the machine\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //validate results
    dTemp = crankslidercurrent->GetConnRodAngleDegrees();
    if( ( dTemp < 8.13 ) || ( dTemp > 8.14 ) ) 
    {
		strcpy_s( outputline, "CalcConnRodAngle failed bounds checking.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        fprintf( stream, "CalcConnRodAngle (Should be real close to +8.13 deg) is: %f\n", dTemp);
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // For a throw on a CW rotating crankshaft on the left side of the
    // compressor. 
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 10 );
    crankslidercurrent->SetConnRodLength_USCS( 25 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetCCW( false );
    crankslidercurrent->SetRight( false );
	if( !crankslidercurrent->CalcConnRodAngle() )
	{
		strcpy_s( outputline, "CalcConnRodAngle failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Connecting Rod Angle, CW rotation, Left side of the machine\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //validate results
    dTemp = crankslidercurrent->GetConnRodAngleDegrees();
    if( ( dTemp < 8.13 ) || ( dTemp > 8.14 ) ) 
    {
		strcpy_s( outputline, "CalcConnRodAngle failed bounds checking.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        fprintf( stream, "CalcConnRodAngle (Should be real close to +8.13 deg) is: %f\n", dTemp);
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // For a throw on a CW rotating crankshaft on the right side of the
    // compressor.
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 10 );
    crankslidercurrent->SetConnRodLength_USCS( 25 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetCCW( false );
    crankslidercurrent->SetRight( true );
	if( !crankslidercurrent->CalcConnRodAngle() )
	{
		strcpy_s( outputline, "CalcConnRodAngle failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Connecting Rod Angle, CW rotation, Right side of the machine\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //validate results
    dTemp = crankslidercurrent->GetConnRodAngleDegrees();
    if( ( dTemp > -8.13 ) || ( dTemp < -8.14 ) ) 
    {
		strcpy_s( outputline, "CalcConnRodAngle failed bounds checking.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        fprintf( stream, "CalcConnRodAngle (Should be real close to -8.13 deg) is: %f\n", dTemp);
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // For a throw on a CCW rotating crankshaft on the left side of the 
    // compressor. 
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 10 );
    crankslidercurrent->SetConnRodLength_USCS( 25 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( false );
	if( !crankslidercurrent->CalcConnRodAngle() )
	{
		strcpy_s( outputline, "CalcConnRodAngle failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Connecting Rod Angle, CCW rotation, Left side of the machine\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //validate results
    dTemp = crankslidercurrent->GetConnRodAngleDegrees();
    if( ( dTemp > -8.13 ) || ( dTemp < -8.14 ) ) 
    {
		strcpy_s( outputline, "CalcConnRodAngle failed bounds checking.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        fprintf( stream, "CalcConnRodAngle (Should be real close to -8.13 deg) is: %f\n", dTemp);
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // Now for crosshead vertical force
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 10 );
    crankslidercurrent->SetConnRodLength_USCS( 25 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( false );
	if( !crankslidercurrent->CalcCrossheadVerticalForce() ||
          ( ( crankslidercurrent->GetCrossheadVerticalForce_USCS() < 12487.0 ) || (crankslidercurrent->GetCrossheadVerticalForce_USCS() > 12489.0 ) ) )
	{
        strcpy_s( outputline, "CalcSliderAcceleration_USCS failed when it should have succeeded.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        fprintf( stream, "Crosshead Vertical Force (Should be real close to 12488 lbf) is: %f\n", crankslidercurrent->GetCrossheadVerticalForce_USCS() );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
     }
	else
	{
		fprintf( stream, "Crosshead Vertical Force (Should be real close to 12488 lbf) is: %f \n", crankslidercurrent->GetCrossheadVerticalForce_USCS() );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // This crosshead vertical force test case was added because of 
    // "spikes" seen on the waveform.  The data originates from
    // waveformID #148
    //
    // B. Howard
    // 7 February 2011
    //
	/////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Begin debugging test case for WaveformID 148.\n" );
	printf( outputline );
	fprintf(stream, outputline );
    
    //configure the chamber
    crankslidercurrent->SetStroke_USCS ( 18 );
    crankslidercurrent->SetConnRodLength_USCS( 47.26 );
    crankslidercurrent->SetRPM( 277 );
    crankslidercurrent->SetPistonAreaCrankEnd_USCS( 263.89378290156, 1 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( 283.528736986498, 1 );
    crankslidercurrent->SetPistonAreaCrankEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( ( -3.573899 + 14.7), 1 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( ( 0.064396 + 14.7 ), 1 );
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( 0.00, 2 );
    crankslidercurrent->bSetAmbientPressure_USCS( 14.7 );
    crankslidercurrent->SetPistonAndRodWeight_USCS( 954);
    crankslidercurrent->SetCrossheadWeight_USCS( 1106 );
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( false );

    crankslidercurrent->SetCrankAngleDegrees( 243.5 );

	//call the calculation method
    if( !crankslidercurrent->CalcCrossheadVerticalForce() )
    {
        strcpy_s( outputline, "CalcSliderAcceleration_USCS failed when it should have succeeded.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //retrieve the value
    dTemp = crankslidercurrent->GetCrossheadVerticalForce_USCS();

    //boundary test the value
    if( ( dTemp < -4102.66 ) || ( dTemp > -4068.46 ) )
	{
        strcpy_s( outputline, "CalcSliderAcceleration_USCS failed bounds testing for Waveform ID 148 debug case.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        fprintf( stream, "Crosshead Vertical Force (Should be between -4102.66 and -4068.46 lbf) is: %f\n", dTemp );
        displaymembers( crankslidercurrent, stream );   
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }
	else
	{
		fprintf( stream, "Crosshead Vertical Force (Should be between -4102.66 and -4068.46 lbf) is: %f \n", dTemp );
	}
    
    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

    //document completion of the test
    strcpy_s( outputline, "End debugging test case for WaveformID 148.\n" );
	printf( outputline );
	fprintf(stream, outputline );

	/////////////////////////////////////////////////////////////////////
    // We'll load a good set of data for the radial force at the main
	// bearing on the right side.
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetStroke_USCS ( 10 );
    crankslidercurrent->SetConnRodLength_USCS( 25 );
    crankslidercurrent->SetRPM( 227 );
    crankslidercurrent->SetCrankAngleDegrees( 45 );
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( true );

    crankslidercurrent->SetPistonAreaCrankEnd_USCS( crankslidercurrent->CalcPistonArea( 19.00, 5.00) , 1 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( crankslidercurrent->CalcPistonArea( 19.00, 0.00) , 1 );
    crankslidercurrent->SetPistonAreaCrankEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPistonAreaHeadEnd_USCS( 0.00, 2 );

    crankslidercurrent->SetPressureAbsCrankEnd_USCS( 311.28, 1 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( 642.22, 1 );
    crankslidercurrent->SetPressureAbsCrankEnd_USCS( 0.00, 2 );
    crankslidercurrent->SetPressureAbsHeadEnd_USCS( 0.00, 2 );

    crankslidercurrent->SetPistonAndRodWeight_USCS( 664.0 + 590.0 );
    crankslidercurrent->SetCrossheadWeight_USCS( 1106.0 );

    if( crankslidercurrent->bSetAmbientPressure_USCS( 14.696 ) == false )
    {
        strcpy_s( outputline, "bSetAmbientPressure_USCS failed when it should have succeeded.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }




	if( !crankslidercurrent->CalcMainBearingRadialForce() ||
          ( ( crankslidercurrent->GetMainBearingRadialForce_USCS() > -52982 ) || ( crankslidercurrent->GetMainBearingRadialForce_USCS() < -52986 ) ) )
	{
		strcpy_s( outputline, "CalcMainBearingRadialLoad failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Radial Force, CCW, Right Side of the machine\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load a good set of data for the tangent force at the main
	// bearing.	
	/////////////////////////////////////////////////////////////////////
	if( !crankslidercurrent->CalcMainBearingTangentForce() )
	{
		strcpy_s( outputline, "CalcMainBearingTangentForce failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Tangent Force\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load a good set of data for the torque at the main 
	// bearing, CCW Right side. 
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( true );
	if( !crankslidercurrent->CalcMainBearingTorque() )
	{
		strcpy_s( outputline, "CalcMainBearingTorque failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Torque, CCW Right Side\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // We'll load a good set of data for the torque at the main
	// bearing, CCW Left side.
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( false );
	if( !crankslidercurrent->CalcMainBearingTorque() )
	{
		strcpy_s( outputline, "CalcMainBearingTorque failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		strcpy_s( outputline, "Torque, CCW Left Side\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

     //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // Check the angle to time conversion functions
    // Begin with the default object configuration.
    // It should succeed.
    /////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( false );
    if( !crankslidercurrent->SetPistonAngleDegrees( 90 ))
    {
		strcpy_s( outputline, "SetPistonAngleDegrees failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
    }
	if( ( crankslidercurrent->CalcTDCsec() < 0 ) )
	{
		strcpy_s( outputline, "CalcTDCsec failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		printf( "Check of CalcTDCsec with default initialization values.  Should be 15 seconds: %f\n", crankslidercurrent->CalcTDCsec() );
		fprintf(stream, "Check of CalcTDCsec with default initialization values.  Should be 15 seconds: %f\n", crankslidercurrent->CalcTDCsec() );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}

	/////////////////////////////////////////////////////////////////////
    // Check the angle to time conversion methods.
    // Now change to another RPM.
	/////////////////////////////////////////////////////////////////////
    crankslidercurrent->SetRPM( 1000 );
    crankslidercurrent->SetCCW( true );
    crankslidercurrent->SetRight( false );
    crankslidercurrent->SetPistonAngleDegrees( 90 );
	if( ( crankslidercurrent->CalcTDCsec() < 0 ) )
	{
		strcpy_s( outputline, "CalcTDCsec failed when it should have succeeded.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
		return;
	}
	else
	{
		printf( "Check of CalcTDCsec with default initialization values.  Should be 0.015 seconds: %f\n", crankslidercurrent->CalcTDCsec() );
		fprintf(stream, "Check of CalcTDCsec with default initialization values.  Should be 15 seconds: %f\n", crankslidercurrent->CalcTDCsec() );
	}

    //insert time value
    if ( bInsertLocalTime ( stream, &iCurrentTickCount, &iPreviousTickCount ) == false )
    {
        strcpy_s( outputline, "Failed to insert local time.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }

    //report out latest object contents
    if( !displaymembers( crankslidercurrent, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }

    ///////////////////////////////////////////////////////////////////
    // Bore Diameters
    ///////////////////////////////////////////////////////////////////
    if( crankslidercurrent->bGetCylinderBoreHeadEndDiameter_USCS( -1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetCylinderBoreHeadEndDiameter_USCS failed to trap a negative index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetCylinderBoreHeadEndDiameter_SI( 2, &dTemp ) )
    {
		strcpy_s( outputline, "bGetCylinderBoreHeadEndDiameter_SI failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 2.54 || dTemp > 2.54 )
    {
		strcpy_s( outputline, "bGetCylinderBoreHeadEndDiameter_SI failed bounds check for initized values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( crankslidercurrent->bSetCylinderBoreHeadEndDiameter_USCS( 1, -10.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreHeadEndDiameter_USCS failed to trap a negative bore.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( crankslidercurrent->bSetCylinderBoreHeadEndDiameter_USCS( -1, 10.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreHeadEndDiameter_USCS failed to trap a negative index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bSetCylinderBoreHeadEndDiameter_USCS( 1, 10.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreHeadEndDiameter_USCS failed with valid index and bore values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetCylinderBoreHeadEndDiameter_USCS( 1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetCylinderBoreHeadEndDiameter_USCS failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 10.00 || dTemp > 10.00 )
    {
		strcpy_s( outputline, "bGetCylinderBoreHeadEndDiameter_USCS failed bounds check for chamber 1, bore of 10.00 in.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }


    if( crankslidercurrent->bGetCylinderBoreCrankEndDiameter_USCS( -1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetCylinderBoreCrankEndDiameter_USCS failed to trap a negative index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetCylinderBoreCrankEndDiameter_SI( 2, &dTemp ) )
    {
		strcpy_s( outputline, "bGetCylinderBoreCrankEndDiameter_SI failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 2.54 || dTemp > 2.54 )
    {
		strcpy_s( outputline, "bGetCylinderBoreCrankEndDiameter_SI failed bounds check for initized values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( crankslidercurrent->bSetCylinderBoreCrankEndDiameter_USCS( 1, -11.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreCrankEndDiameter_USCS failed to trap a negative bore.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( crankslidercurrent->bSetCylinderBoreCrankEndDiameter_USCS( -1, 11.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreCrankEndDiameter_USCS failed to trap a negative index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bSetCylinderBoreCrankEndDiameter_USCS( 1, 11.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreCrankEndDiameter_USCS failed with valid index and bore values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetCylinderBoreCrankEndDiameter_USCS( 1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetCylinderBoreCrankEndDiameter_USCS failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 11.00 || dTemp > 11.00 )
    {
		strcpy_s( outputline, "bGetCylinderBoreCrankEndDiameter_USCS failed bounds check for chamber 1, bore of 11.00 in.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }


    ///////////////////////////////////////////////////////////////////
    // Rod Diameters
    ///////////////////////////////////////////////////////////////////
    if( crankslidercurrent->bGetPistonRodHeadEndDiameter_USCS( -1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetPistonRodHeadEndDiameter_USCS failed to trap a negative index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetPistonRodHeadEndDiameter_SI( 2, &dTemp ) )
    {
		strcpy_s( outputline, "bGetPistonRodHeadEndDiameter_SI failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 0.254 || dTemp > 0.254 )
    {
		strcpy_s( outputline, "bGetPistonRodHeadEndDiameter_SI failed bounds check for initized values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( crankslidercurrent->bSetPistonRodHeadEndDiameter_USCS( 1, -2.00 ) )
    {
		strcpy_s( outputline, "bSetPistonRodHeadEndDiameter_USCS failed to trap a negative diameter.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( crankslidercurrent->bSetPistonRodHeadEndDiameter_USCS( -1, 2.00 ) )
    {
		strcpy_s( outputline, "bSetPistonRodHeadEndDiameter_USCS failed to trap a negative index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bSetPistonRodHeadEndDiameter_USCS( 1, 2.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreHeadEndDiameter_USCS failed with valid index and diameter values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetPistonRodHeadEndDiameter_USCS( 1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetPistonRodHeadEndDiameter_USCS failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 2.00 || dTemp > 2.00 )
    {
		strcpy_s( outputline, "bGetPistonRodHeadEndDiameter_USCS failed bounds check for chamber 1, bore of 2.00 in.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bSetPistonRodCrankEndDiameter_USCS( 1, 2.00 ) )
    {
		strcpy_s( outputline, "bSetCylinderBoreCrankEndDiameter_USCS failed with valid index and diameter values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetPistonRodCrankEndDiameter_USCS( 1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetPistonRodCrankEndDiameter_USCS failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 2.00 || dTemp > 2.00 )
    {
		strcpy_s( outputline, "bGetPistonRodCrankEndDiameter_USCS failed bounds check for chamber 1, diameter of 2.00 in.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }


    if( crankslidercurrent->bGetPistonRodCrankEndDiameter_USCS( -1, &dTemp ) )
    {
		strcpy_s( outputline, "bGetPistonRodCrankEndDiameter_USCS failed to trap a negative index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( !crankslidercurrent->bGetPistonRodCrankEndDiameter_SI( 2, &dTemp ) )
    {
		strcpy_s( outputline, "bGetPistonRodCrankEndDiameter_SI failed with a valid index.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }
    if( dTemp < 0.254 || dTemp > 0.254 )
    {
		strcpy_s( outputline, "bGetPistonRodCrankEndDiameter_SI failed bounds check for initized values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }

    ///////////////////////////////////////////////////////////////////
    // Displaced Volume
    ///////////////////////////////////////////////////////////////////
    if( !crankslidercurrent->SetStroke_USCS( 0.9 ) )
    {
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
	    return;
    }

    if( ( crankslidercurrent->GetDisplacedVolume_USCS( 1.5, 0.2 ) > 1.56215694 ) 
        && ( crankslidercurrent->GetDisplacedVolume_USCS( 1.5, 0.2 ) < 1.56215696 ) )
    {
		strcpy_s( outputline, "Displaced Volume_USCS Default OK.\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }
    else
    {
		strcpy_s( outputline, "Displaced Volume_USCS initialization failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        CloseOut( crankslidercurrent, crankslidernext, stream );
        return;
    }


    ///////////////////////////////////////////////////////////////////
	// End-of-test regime tasks
    ///////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "****************Harness tests successfully completed.********************\n" );
	printf( outputline );
	fprintf(stream, outputline );

    //call the closeout function
    CloseOut( crankslidercurrent, crankslidernext, stream );

	//All done
	return;
}
