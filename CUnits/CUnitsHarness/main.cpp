
/////////////////////////////////////////////////////////////////////////
// This program serves as a test harness for 
// the unit conversion class
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
// Written by:	Brian Howard
// Date:		01 Jan 2002	
/////////////////////////////////////////////////////////////////////////
//
// Updated to Visual Studio 2008 Microsoft Visual Studio 2008
//            Version 9.0.30729.1 SP
//            Microsoft .NET Framework
//            Version 3.5 SP1
//
// Brian Howard, May 2010
//
/////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "units.h"

using namespace std;

#define OUTPUTLINESIZE 2048

bool displaymembers( CUnits *units, FILE *stream )
{

	//local variables
    char outputline[OUTPUTLINESIZE];

	//initialize local variables
	strcpy_s(outputline, "");

    /////////////////////////////////////////////////////////////////////////
	// Begin by outputting the classes and starting with class 00
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Class 00 - Units of length\n" );
	printf( outputline );
	fprintf(stream, outputline );
	strcpy_s( outputline, "Unit, Class, Multiplier, Offset\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin with cm
	printf( "cm, %f, %f, %f\n", units->cm[0], units->cm[1], units->cm[2] ); 
	fprintf( stream, "cm, %f, %f, %f\n", units->cm[0], units->cm[1], units->cm[2] ); 

	//now inches (in) 
	printf( "in, %f, %f, %f\n", units->in[0], units->in[1], units->in[2] ); 
	fprintf( stream, "in, %f, %f, %f\n", units->in[0], units->in[1], units->in[2] ); 

	//now meters (m)
	printf( "m, %f, %f, %f\n", units->m[0], units->m[1], units->m[2] ); 
	fprintf( stream, "m, %f, %f, %f\n", units->m[0], units->m[1], units->m[2] ); 

    /////////////////////////////////////////////////////////////////////////
	// Continue with with class 01
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Class 01 - Units of Area\n" );
	printf( outputline );
	fprintf(stream, outputline );
	strcpy_s( outputline, "Unit, Class, Multiplier, Offset\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin with square centimeters (cm2)
	printf( "cm2, %f, %f, %f\n", units->cm2[0], units->cm2[1], units->cm2[2] ); 
	fprintf( stream, "cm2, %f, %f, %f\n", units->cm2[0], units->cm2[1], units->cm2[2] ); 

	//now square inches (in2) 
	printf( "in2, %f, %f, %f\n", units->in2[0], units->in2[1], units->in2[2] ); 
	fprintf( stream, "in2, %f, %f, %f\n", units->in2[0], units->in2[1], units->in2[2] ); 

    /////////////////////////////////////////////////////////////////////////
	// Continue with with class 02
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Class 02 - Units of Volume\n" );
	printf( outputline );
	fprintf(stream, outputline );
	strcpy_s( outputline, "Unit, Class, Multiplier, Offset\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin with cubic centimeters (cm3)
	printf( "cm3, %f, %f, %f\n", units->cm3[0], units->cm3[1], units->cm3[2] ); 
	fprintf( stream, "cm3, %f, %f, %f\n", units->cm3[0], units->cm3[1], units->cm3[2] ); 

	//now cubic inches (in3) 
	printf( "in3, %f, %f, %f\n", units->in3[0], units->in3[1], units->in3[2] ); 
	fprintf( stream, "in3, %f, %f, %f\n", units->in3[0], units->in3[1], units->in3[2] ); 

	//now cubic feet (ft3) 
	printf( "ft3, %f, %f, %f\n", units->ft3[0], units->ft3[1], units->ft3[2] ); 
	fprintf( stream, "ft3, %f, %f, %f\n", units->ft3[0], units->ft3[1], units->ft3[2] ); 

    /////////////////////////////////////////////////////////////////////////
    // Continue with with class 03
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Class 03 - Units of Temperature\n" );
	printf( outputline );
	fprintf(stream, outputline );
	strcpy_s( outputline, "Unit, Class, Multiplier, Offset\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin with Rankine (R)
	printf( "R, %f, %f, %f\n", units->R[0], units->R[1], units->R[2] ); 
	fprintf( stream, "R, %f, %f, %f\n", units->R[0], units->R[1], units->R[2] ); 

	//now Kelvin (K) 
	printf( "K, %f, %f, %f\n", units->K[0], units->K[1], units->K[2] ); 
	fprintf( stream, "K, %f, %f, %f\n", units->K[0], units->K[1], units->K[2] ); 

    /////////////////////////////////////////////////////////////////////////
    // Continue with with class 04
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Class 04 - Units of Pressure\n" );
	printf( outputline );
	fprintf(stream, outputline );
	strcpy_s( outputline, "Unit, Class, Multiplier, Offset\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin with pound-force per square inch (PSIA)
	printf( "PSIA, %f, %f, %f\n", units->PSIA[0], units->PSIA[1], units->PSIA[2] ); 
	fprintf( stream, "PSIA, %f, %f, %f\n", units->PSIA[0], units->PSIA[1], units->PSIA[2] ); 

	//now bar (bar) 
	printf( "bar, %f, %f, %f\n", units->bar[0], units->bar[1], units->bar[2] ); 
	fprintf( stream, "bar, %f, %f, %f\n", units->bar[0], units->bar[1], units->bar[2] ); 

    /////////////////////////////////////////////////////////////////////////
	//Continue with with class 05
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Class 05 - Units of mass\n" );
	printf( outputline );
	fprintf(stream, outputline );
	strcpy_s( outputline, "Unit, Class, Multiplier, Offset\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin with gram (g)
	printf( "g, %f, %f, %f\n", units->g[0], units->g[1], units->g[2] ); 
	fprintf( stream, "g, %f, %f, %f\n", units->g[0], units->g[1], units->g[2] ); 

	//now pound-mass (lb) 
	printf( "lb, %f, %f, %f\n", units->lb[0], units->lb[1], units->lb[2] ); 
	fprintf( stream, "lb, %f, %f, %f\n", units->lb[0], units->lb[1], units->lb[2] ); 

    /////////////////////////////////////////////////////////////////////////
    // Continue with with class 06
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Class 06 - Units of torque\n" );
	printf( outputline );
	fprintf(stream, outputline );
	strcpy_s( outputline, "Unit, Class, Multiplier, Offset\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Output members.  Begin with USCS (ftlb)
	printf( "ftlb, %f, %f, %f\n", units->ftlb[0], units->ftlb[1], units->ftlb[2] ); 
	fprintf( stream, "ftlb, %f, %f, %f\n", units->ftlb[0], units->ftlb[1], units->ftlb[2] ); 

	//now pound-mass (Nm) 
	printf( "Nm, %f, %f, %f\n", units->Nm[0], units->Nm[1], units->Nm[2] ); 
	fprintf( stream, "Nm, %f, %f, %f\n", units->Nm[0], units->Nm[1], units->Nm[2] ); 

	//success
	return true;
}

//main subroutine.  returns nothing to user
void main ()
{
	//local variables
	int	i;
	int	j;
	FILE *stream;
	class CUnits *units;
	char outputline[OUTPUTLINESIZE];
	double testvalue;
	double dtemp;
	double xarray[10];
	double yarray[10];
	double r;
    errno_t err;

	//initialize local variables
	i = 0;
	j = 0;
	stream = NULL;
	units = NULL;
	strcpy_s(outputline, "");
	testvalue = 3.14159;
	dtemp = 0;
	for(i=0; i<10; i++)
	{
		xarray[i] = 0;
		yarray[i] = 0;
	}
	r = 0;

    /////////////////////////////////////////////////////////////////////////
    // The remainder of this program serves as a container and harness
	// to test the object members and methods.  Tests also
	// includes exception handling.	
    /////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////
	// Test Sequence
	//	- Verify class instantiation
	//	- Verify member initialization	
	//	- Verify method performance
    /////////////////////////////////////////////////////////////////////////
	
	//Open the log file
	err = fopen_s( &stream, "CUnitsHarness.csv", "w" );
    if( err != 0 )
    {
		return;
    }

	//Intro line
	strcpy_s( outputline, "CUnits Test Harness.\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Instantiate specific instance
	units	= new CUnits();

	//Document instantiation of class
	if( units == NULL )
		return;
	else
	{
		strcpy_s( outputline, "CUnits Instantiated.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

    /////////////////////////////////////////////////////////////////////////
    // Document member values as found on class instatiation.
    /////////////////////////////////////////////////////////////////////////
	if( !displaymembers( units, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    /////////////////////////////////////////////////////////////////////////
    // Verify basic conversions
    /////////////////////////////////////////////////////////////////////////
	strcpy_s( outputline, "Now performing basic conversions\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//length		- class 00 units
	printf( "1.00 in = %f cm\n", units->convert( units->in, units->cm ) );
	fprintf( stream, "1.00 in = %f cm\n", units->convert( units->in, units->cm ) );

	printf( "1.00 cm = %f in\n", units->convert( units->cm, units->in ) );
	fprintf( stream, "1.00 cm = %f in\n", units->convert( units->cm, units->in ) );

	printf( "1.00 in = %f ft\n", units->convert( units->in, units->ft ) );
	fprintf( stream, "1.00 in = %f ft\n", units->convert( units->in, units->ft ) );

	printf( "1.00 ft = %f in\n", units->convert( units->ft, units->in ) );
	fprintf( stream, "1.00 ft = %f in\n", units->convert( units->ft, units->in ) );

	printf( "1.00 ft = %f cm\n", units->convert( units->ft, units->cm ) );
	fprintf( stream, "1.00 ft = %f cm\n", units->convert( units->ft, units->cm ) );

	printf( "1.00 ft = %f m\n", units->convert( units->ft, units->m ) );
	fprintf( stream, "1.00 ft = %f m\n", units->convert( units->ft, units->m ) );

     //area			- class 02 units
	printf( "1.00 in2 = %f ft2\n", units->convert( units->in2, units->ft2 ) );
	fprintf( stream, "1.00 in2 = %f ft2\n", units->convert( units->in2, units->ft2 ) );

	printf( "1.00 ft2 = %f in2\n", units->convert( units->ft2, units->in2 ) );
	fprintf( stream, "1.00 ft2 = %f in2\n", units->convert( units->ft2, units->in2 ) );

	//Volume	- class 02 units
	printf( "1.00 in3 = %f ft3\n", units->convert( units->in3, units->ft3 ) );
	fprintf( stream, "1.00 in3 = %f ft3\n", units->convert( units->in3, units->ft3 ) );

	printf( "1.00 in3 = %f cm3\n", units->convert( units->in3, units->cm3 ) );
	fprintf( stream, "1.00 in3 = %f cm3\n", units->convert( units->in3, units->cm3 ) );

	printf( "1.00 in3 = %f m3\n", units->convert( units->in3, units->m3 ) );
	fprintf( stream, "1.00 in3 = %f m3\n", units->convert( units->in3, units->m3 ) );

	printf( "1.00 ft3 = %f in3\n", units->convert( units->ft3, units->in3 ) );
	fprintf( stream, "1.00 ft3 = %f in3\n", units->convert( units->ft3, units->in3 ) );

	printf( "1.00 ft3 = %f cm3\n", units->convert( units->ft3, units->cm3 ) );
	fprintf( stream, "1.00 ft3 = %f cm3\n", units->convert( units->ft3, units->cm3 ) );

	printf( "1.00 ft3 = %f m3\n", units->convert( units->ft3, units->m3 ) );
	fprintf( stream, "1.00 ft3 = %f m3\n", units->convert( units->ft3, units->m3 ) );

	//Pressure		- class 04 units
	printf( "1.00 PSIA = %f bar\n", units->convert( units->PSIA, units->bar ) );
	fprintf( stream, "1.00 PSIA = %f bar\n", units->convert( units->PSIA, units->bar ) );

	//mass			- class 05 units
	printf( "1.00 lb = %f g\n", units->convert( units->lb, units->g ) );
	fprintf( stream, "1.00 lb = %f g\n", units->convert( units->lb, units->g ) );

	printf( "1.00 lb = %f kg\n", units->convert( units->lb, units->kg ) );
	fprintf( stream, "1.00 lb = %f kg\n", units->convert( units->lb, units->kg ) );

	//torque		- class 06 units
	printf( "1.00 ft-lb = %f N-m\n", units->convert( units->ftlb, units->Nm ) );
	fprintf( stream, "1.00 ft-lb = %f N-m\n", units->convert( units->ftlb, units->Nm ) );

	printf( "1.00 N-m = %f ft-lb\n", units->convert( units->Nm, units->ftlb ) );
	fprintf( stream, "1.00 N-m = %f ft-lb\n", units->convert( units->Nm, units->ftlb ) );

	//force		- class 07 units
	printf( "1.00 lbf = %f N\n", units->convert( units->lbf, units->N ) );
	fprintf( stream, "1.00 lbf = %f N\n", units->convert( units->lbf, units->N ) );

	printf( "1.00 N = %f lbf\n", units->convert( units->N, units->lbf ) );
	fprintf( stream, "1.00 N = %f lbf\n", units->convert( units->N, units->lbf ) );

	//energy	- class 08 units
	printf( "1.00 lbf_ft = %f BTU\n", units->convert( units->lbf_ft, units->BTU ) );
	fprintf( stream, "1.00 lbf_ft = %f BTU\n", units->convert( units->lbf_ft, units->BTU ) );

	printf( "1.00 lbf_ft = %f J\n", units->convert( units->lbf_ft, units->J ) );
	fprintf( stream, "1.00 lbf_ft = %f J\n", units->convert( units->lbf_ft, units->J ) );

	printf( "1.00 lbf_ft = %f kJ\n", units->convert( units->lbf_ft, units->kJ ) );
	fprintf( stream, "1.00 lbf_ft = %f kJ\n", units->convert( units->lbf_ft, units->kJ ) );

	printf( "1.00 BTU = %f lbf_ft\n", units->convert( units->BTU, units->lbf_ft ) );
	fprintf( stream, "1.00 BTU = %f lbf_ft\n", units->convert( units->BTU, units->lbf_ft ) );

	printf( "1.00 BTU = %f J\n", units->convert( units->BTU, units->J ) );
	fprintf( stream, "1.00 BTU = %f J\n", units->convert( units->BTU, units->J ) );

	printf( "1.00 BTU = %f kJ\n", units->convert( units->BTU, units->kJ ) );
	fprintf( stream, "1.00 BTU = %f kJ\n", units->convert( units->BTU, units->kJ ) );

	printf( "1.00 radian = %f degree\n", units->convert( units->radians, units->degrees ) );
	fprintf( stream, "1.00 radian = %f degree\n", units->convert( units->radians, units->degrees ) );

	printf( "1.00 degree = %f radians\n", units->convert( units->degrees, units->radians ) );
	fprintf( stream, "1.00 degree = %f radians\n", units->convert( units->degrees, units->radians )  );

	/********************************************************************
	* End-of-program housekeeping functions								*
	*********************************************************************/
	//Free all elements in the strand
	delete units;

	//Document destruction of object
	printf("CUnits Destructed.\n" );
	fprintf(stream, "CUnits Destructed.\n" );

	//close the output file
	fclose( stream );

	//All done
	return;
}
