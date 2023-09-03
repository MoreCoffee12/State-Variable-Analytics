
////////////////////////////////////////////////////
//
// Test harness for plane and solids stress 
// calculation 
//
// Brian howard, September 2007
//
////////////////////////////////////////////////////
//
// Updated to include stress-strain converstions
//
// Brian Howard, October 2007
//
////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <oleauto.h>
#include <math.h>
#include "planestress.h"
#include "solid.h"

//check to see if two numbers are really close to each other
bool smalldiff( double d1, double d2 )
{
    if( ( d1 - d2 ) < 0.0000000000000 )
        return true;
    else
        return false;
}

//display the members of the solid strain object
bool displaymembers( CSolid *solid, FILE *stream )
{

    //local variables
    char outputline[2048];
    double dtemp, dtemp1, dtemp2, dtemp3;

    //output introductory sentence
    strcpy( outputline, "Outputting members of the solid strain object\n" );
	
    //output the formatted streams
    printf( outputline );
	fprintf(stream, outputline );

    //output header descriptions for the first row of the stress tensor
    strcpy( outputline, "NormalStress_xx, ShearStress_xy, ShearStress_xz\n" );
	fprintf(stream, outputline );

    //retrieve values from the object and send to output stream
    if( !solid->GetNormalStress_xx( &dtemp ) )
    {
        strcpy( outputline, "GetNormal_xx Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStress_xy( &dtemp ) )
    {
        strcpy( outputline, "GetShear_xy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStress_xz( &dtemp ) )
    {
        strcpy( outputline, "GetShear_xz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //output the second row of the stress tensor
    strcpy( outputline, "ShearStress_yx, NormalStress_yy, ShearStress_yz\n");
	fprintf(stream, outputline );
    if( !solid->GetShearStress_xy( &dtemp ) )
    {
        strcpy( outputline, "GetShear_xy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetNormalStress_yy( &dtemp ) )
    {
        strcpy( outputline, "GetNormal_yy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStress_yz( &dtemp ) )
    {
        strcpy( outputline, "GetShear_yz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //output the third row of the stress tensor
    strcpy( outputline, "ShearStress_zx, ShearStress_zy, NormalStress_zz\n");
	fprintf(stream, outputline );
    if( !solid->GetShearStress_xz( &dtemp ) )
    {
        strcpy( outputline, "GetShear_xz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStress_yz( &dtemp ) )
    {
        strcpy( outputline, "GetShear_yz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetNormalStress_zz( &dtemp ) )
    {
        strcpy( outputline, "GetNormal_zz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //output the first row of the strain tensor
    strcpy ( outputline, "NormalStrain_xx, ShearStrain_xy, ShearStrain_xz\n" );
	fprintf(stream, outputline );
    if( !solid->GetNormalStrain_xx( &dtemp ) )
    {
        strcpy( outputline, "GetNormalStrain_xx Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStrain_xy( &dtemp ) )
    {
        strcpy( outputline, "GetShearStrain_xy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStrain_xz( &dtemp ) )
    {
        strcpy( outputline, "GetShearStrain_xz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //output the second row of the strain tensor
    strcpy ( outputline, "ShearStrain_yx, NormalStrain_yy, ShearStrain_yz\n" );
	fprintf(stream, outputline );

    if( !solid->GetShearStrain_xy( &dtemp ) )
    {
        strcpy( outputline, "GetShearStrain_xy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetNormalStrain_yy( &dtemp ) )
    {
        strcpy( outputline, "GetNormalStrain_yy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStrain_yz( &dtemp ) )
    {
        strcpy( outputline, "GetShearStrain_yz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //output the third row of the strain tensor
    strcpy( outputline, "ShearStrain_zx, ShearStrain_zy, NormalStrain_zz\n" );
	fprintf(stream, outputline );

    if( !solid->GetShearStrain_xz( &dtemp ) )
    {
        strcpy( outputline, "GetShearStrain_xz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetShearStrain_yz( &dtemp ) )
    {
        strcpy( outputline, "GetShearStrain_yz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetNormalStrain_zz( &dtemp ) )
    {
        strcpy( outputline, "GetNormalStrain_zz Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //output the strain invariants
    strcpy (outputline, "StrainInvariant_1, StrainInvariant_2, StrainInvariant_3\n" );
	fprintf(stream, outputline );
    if( !solid->GetStrainInvariants( &dtemp1, &dtemp2, &dtemp3 ) )
    {
        strcpy( outputline, "GetStrainInvariants Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%#10.20f, %#10.20f, %#10.20f\n", dtemp1, dtemp2, dtemp3 );

    //output principal stresses
    strcpy (outputline, "Principal_1, Principal_2, Principal_3\n" );
	fprintf(stream, outputline );
    if( !solid->GetPrincipalStress_1( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipalStress_1 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetPrincipalStress_2( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipalStress_2 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetPrincipalStress_3( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipalStress_3 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //output principal strains
    strcpy (outputline, "Strain_1, Strain_2, Strain_3\n" );
	fprintf(stream, outputline );
    if( !solid->GetPrincipalStrain_1( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipalStrain_1 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%#10.20f,", dtemp );
    if( !solid->GetPrincipalStrain_2( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipalStrain_2 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%#10.20f,", dtemp );
    if( !solid->GetPrincipalStrain_3( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipalStrain_3 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%#10.20f\n", dtemp );

    //output maximum shear and material properties
    strcpy (outputline, "ShearStress_max, Modulus, Poisson's Ratio\n" );
	fprintf(stream, outputline );
    if( !solid->GetShearStress_max( &dtemp ) )
    {
        strcpy( outputline, "GetShearStress_max Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetModulus( &dtemp ) )
    {
        strcpy( outputline, "GetModulus Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !solid->GetPoissonsRatio( &dtemp ) )
    {
        strcpy( outputline, "GetPoissonsRatio Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //success
    return true;
}

//display the members of the plane stress object
bool displaymembers( CPlaneStress *planestress, FILE *stream )
{
    //local variables
    char outputline[2048];
    double dtemp;

    //output introductory sentence
    strcpy( outputline, "Outputting members of the plane stress object\n" );
	
    //output the formatted streams
    printf( outputline );
	fprintf(stream, outputline );

    //output header descriptions
    strcpy( outputline, "Normal_xx, Normal_yy, Shear_xy, Principal_1, Principal_2, Shear_max\n" );
	fprintf(stream, outputline );

    //retrieve values from the object and send to output stream
    if( !planestress->GetNormalStress_xx( &dtemp ) )
    {
        strcpy( outputline, "GetNormal_xx Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !planestress->GetNormalStress_yy( &dtemp ) )
    {
        strcpy( outputline, "GetNormal_yy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !planestress->GetShearStress_xy( &dtemp ) )
    {
        strcpy( outputline, "GetShear_xy Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !planestress->GetPrincipal_1( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipal_1 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !planestress->GetPrincipal_2( &dtemp ) )
    {
        strcpy( outputline, "GetPrincipal_2 Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f,", dtemp );
    if( !planestress->GetShear_max( &dtemp ) )
    {
        strcpy( outputline, "GetShear_max Failed.\n" );
	    fprintf(stream, outputline );
        return false;
    }
    fprintf( stream, "%f\n", dtemp );

    //success
    return true;
}

// house-keeping and close-out tasks
bool CloseOut(CPlaneStress *planestress,
              CSolid *solid,
              FILE *stream)
{
    //free the elements in the plane stress object
    delete planestress;
	printf("CPlaneStress Destructed.\n" );
	fprintf(stream, "CPlaneStress Destructed.\n" );

    //free the elements in the solid strain object
    delete solid;
	printf("CSolid Destructed.\n" );
	fprintf(stream, "CSolid Destructed.\n" );

    //close the output file
	fclose( stream );

    return true;
}

//main test harness
void main ()
{
	//local variables
    int	i;
	int	j;
	FILE *stream;
	class CPlaneStress *planestress;
    class CSolid *solid;
    char outputline[2048];
	double testvalue;
	double dtemp;
    double dtemp02;
    double normal_xx, normal_yy, normal_zz, shear_xy, shear_xz, shear_yz;
	double xarray[10];
	double yarray[10];
	double r;
    SYSTEMTIME SysTime;
	int CurrentTickCount;
    int PreviousTickCount;
    int LapsedTicks;

	//initialize local variables
    i = 0;
    j = 0;
	stream = NULL;
	planestress = NULL;
    strcpy(outputline, "");
	testvalue = 3.14159;
	dtemp = 0;
    dtemp02 = 0;
	for(i=0;i<10;i++)
	{
		xarray[i] = 0;
		yarray[i] = 0;
	}
	r = 0;
	CurrentTickCount = GetTickCount();
    PreviousTickCount = GetTickCount();
    LapsedTicks = ( CurrentTickCount - PreviousTickCount);

    ///////////////////////////////////////////////////////////////////////
	// The remainder of this program serves as a container and harness 
	// to test the object members and methods.  Tests also
	// include exception handling.	
    ///////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////
	// Test Sequence
	//	- Verify class instantiation
	//	- Verify member initialization
	//	- Verify method performance	
    ///////////////////////////////////////////////////////////////////////
	
	//Open the log file
	if( (stream = fopen( "CSolidMechanics.csv" , "w")) == NULL)
		return;

	//Intro line
	strcpy( outputline, "CSolidMechanics Test Harness.\n" );
	printf( outputline );
	fprintf(stream, outputline );

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


    ///////////////////////////////////////////////////////////////////
    // begin plane stress test section.  Instantiate the plane stress 
    // and solid strain objects.  Document default values
    //////////////////////////////////////////////////////////////////
    planestress = new CPlaneStress;
    displaymembers( planestress, stream );
    solid = new CSolid;
    displaymembers( solid, stream );

    ///////////////////////////////////////////////////////////////////
    // Load the values for the plane stress test case.  Reference ME556, HW #1,
    // problem 1.31
    //////////////////////////////////////////////////////////////////
	strcpy( outputline, "Testing plane stress principal and maximum shear stress calculations\n" );
	fprintf(stream, outputline );
    
    if( !planestress->SetNormalStress_xx( 220.0 ) )
    {
	    strcpy( outputline, "SetNormalStress_xx Failed.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !planestress->SetNormalStress_yy( 50.0 ) )
    {
	    strcpy( outputline, "SetNormalStress_yy Failed.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !planestress->SetShearStress_xy( -20.0) )
    {
	    strcpy( outputline, "SetShearStress_xy Failed.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !planestress->CalcPrincipalsMaxShear() )
    {
	    strcpy( outputline, "CalcPrincipalsMaxShear.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    displaymembers( planestress, stream );

    ///////////////////////////////////////////////////////////////////
    // Begin testing for the solid strain object member
    ///////////////////////////////////////////////////////////////////
    // check that error trapping captures a negative modulus
    ///////////////////////////////////////////////////////////////////
    if( solid->SetModulus( -207000000 ) )
    {
	    strcpy( outputline, "solid->SetModulus Failed to trap a negative modulus.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    else
    {
	    strcpy( outputline, "solid->SetModulus successfully trapped a negative modulus.\n" );
	    fprintf(stream, outputline );
    }
    // Now load a positive modulus
    if( !solid->SetModulus( 207000000 ) )
    {
	    strcpy( outputline, "solid->SetModulus failed to load a positive modulus.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    ///////////////////////////////////////////////////////////////////
    // check that error trapping captures a negative Poisson's ratio
    ///////////////////////////////////////////////////////////////////
    if( solid->SetPoissonsRatio( -0.3 ) )
    {
	    strcpy( outputline, "solid->SetPoissonsRatio Failed to trap a negative value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    else
    {
	    strcpy( outputline, "solid->SetSetPoissonsRatio successfully trapped a negative value.\n" );
	    fprintf(stream, outputline );
    }
    // Now load a positive Poisson's ratio
    if( !solid->SetPoissonsRatio( 0.3 ) )
    {
	    strcpy( outputline, "solid->SetPoissonsRatio failed to load a positive value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    // Now load the stress data.  Reference HW #2, Problem 2.28
    ///////////////////////////////////////////////////////////////////
    
    //these values will get used again for calculation check
    normal_xx = 220.0;
    normal_yy = 77.0;
    normal_zz = 154.0;
    shear_xy = 110.0;
    shear_xz = 66.0;
    shear_yz = 55.0;

    if( !solid->SetNormalStress_xx( normal_xx ) )
    {
	    strcpy( outputline, "solid->SetNormalStress_xx failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetShearStress_xy( shear_xy ) )
    {
	    strcpy( outputline, "solid->SetShearStress_xy failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetShearStress_xz( shear_xz ) )
    {
	    strcpy( outputline, "solid->SetShearStress_xy failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetNormalStress_yy( normal_yy ) )
    {
	    strcpy( outputline, "solid->SetNormalStress_xy failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetShearStress_yz( shear_yz ) )
    {
	    strcpy( outputline, "solid->SetShearStress_yz failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetNormalStress_zz( normal_zz ) )
    {
	    strcpy( outputline, "solid->SetNormalStress_zz failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetModulus( (double)207000 ) )
    {
	    strcpy( outputline, "solid->SetModulus failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetPoissonsRatio( (double)0.3 ) )
    {
	    strcpy( outputline, "solid->SetPoissonsRatio failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }

    //document loaded values
	strcpy( outputline, "Data loaded into strain solid object.\n" );
	fprintf(stream, outputline );
    displaymembers( solid, stream );

    //convert the stress field to a strain field and validate results
    if( !solid->CalcStrainFromStress() )
    {
	    strcpy( outputline, "solid->CalcStrainFromStress failed to convert.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetNormalStrain_xx( &dtemp );
    if( ( dtemp < 0.0007280192 ) || ( dtemp > 0.0007280194 ) )
    {
	    strcpy( outputline, "solid->CalcStrainFromStress failed to calculate NormalStrain_xx correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetNormalStrain_yy( &dtemp );
    if( ( dtemp < -0.0001700484 ) || ( dtemp > -0.0001700482 ) )
    {
	    strcpy( outputline, "solid->CalcStrainFromStress failed to calculate NormalStrain_yy correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetNormalStrain_zz( &dtemp );
    if( ( dtemp < 0.0003135265 ) || ( dtemp > 0.0003135267 ) )
    {
	    strcpy( outputline, "solid->CalcStrainFromStress failed to calculate NormalStrain_zz correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetShearStrain_xy( &dtemp );
    if( ( dtemp < 0.0006908212 ) || ( dtemp > 0.0006908213 ) )
    {
	    strcpy( outputline, "solid->CalcStrainFromStress failed to calculate ShearStrain_xy correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetShearStrain_xz( &dtemp );
    if( ( dtemp < 0.0004144927 ) || ( dtemp > 0.0004144928 ) )
    {
	    strcpy( outputline, "solid->CalcStrainFromStress failed to calculate ShearStrain_xz correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetShearStrain_yz( &dtemp );
    if( ( dtemp < 0.0003454106 ) || ( dtemp > 0.0003454107 ) )
    {
	    strcpy( outputline, "solid->CalcStrainFromStress failed to calculate ShearStrain_yz correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }

    //To Do - test for success on principal stress and strain calculations
	strcpy( outputline, "Stress field to strain field succeeded; display the results.\n" );
	fprintf(stream, outputline );
    displaymembers( solid, stream );

	//reset the stress tensor and see if we can convert the strain
    //back to the original stress tensor
    strcpy( outputline, "Reset the stress tensor.\n" );
	fprintf(stream, outputline );

    if( !solid->SetNormalStress_xx( (double)1 ) )
    {
	    strcpy( outputline, "solid->SetNormalStress_xx failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetShearStress_xy( (double)2 ) )
    {
	    strcpy( outputline, "solid->SetShearStress_xy failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetShearStress_xz( (double)3 ) )
    {
	    strcpy( outputline, "solid->SetShearStress_xy failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetNormalStress_yy( (double)4 ) )
    {
	    strcpy( outputline, "solid->SetNormalStress_xy failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetShearStress_yz( (double)5 ) )
    {
	    strcpy( outputline, "solid->SetShearStress_yz failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    if( !solid->SetNormalStress_zz( (double)6 ) )
    {
	    strcpy( outputline, "solid->SetNormalStress_zz failed to load value.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    displaymembers( solid, stream );

    //make the calculation and output the results
    //convert the stress field to a strain field and validate results
    if( !solid->CalcStressFromStrain() )
    {
	    strcpy( outputline, "solid->CalcStressFromStrain failed to convert.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }

    //check the calculations
    solid->GetNormalStress_xx( &dtemp );
    if( dtemp != normal_xx )
    {
	    strcpy( outputline, "solid->CalcStressFromStrain failed to calculate NormalStress_xx Correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetNormalStress_yy( &dtemp );
    if( dtemp != normal_yy )
    {
	    strcpy( outputline, "solid->CalcStressFromStrain failed to calculate NormalStress_yy Correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetNormalStress_zz( &dtemp );
    if( dtemp != normal_zz )
    {
	    strcpy( outputline, "solid->CalcStressFromStrain failed to calculate NormalStress_zz Correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetShearStress_xy( &dtemp );
    if( smalldiff( dtemp,shear_xy ) )
    {
	    strcpy( outputline, "solid->CalcStressFromStrain failed to calculate ShearStress_xy Correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetShearStress_xz( &dtemp );
    if( smalldiff( dtemp,shear_xz ) )
    {
	    strcpy( outputline, "solid->CalcStressFromStrain failed to calculate ShearStress_xz Correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }
    solid->GetShearStress_yz( &dtemp );
    if( smalldiff( dtemp,shear_yz ) )
    {
	    strcpy( outputline, "solid->CalcStressFromStrain failed to calculate ShearStress_yz Correctly.\n" );
	    printf( outputline );
	    fprintf(stream, outputline );
        CloseOut( planestress, solid, stream );
        return;
    }

    displaymembers( solid, stream );

    ///////////////////////////////////////////////////////////////////
	// End-of-test regime tasks
    ///////////////////////////////////////////////////////////////////
	strcpy( outputline, "****************Harness tests successfully completed.********************\n" );
	printf( outputline );
	fprintf(stream, outputline );

    //call the closeout function
    CloseOut( planestress, solid, stream );
   
	//All done
	return;
}
