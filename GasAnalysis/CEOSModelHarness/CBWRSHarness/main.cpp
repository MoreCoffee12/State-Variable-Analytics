
////////////////////////////////////////////////////////////////////////
// This program serves as a test harness for
// the equation of state (EOS) model
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
// Written by:	Brian Howard
// Date:		11 Jan 2002
////////////////////////////////////////////////////////////////////////
//
// Revision, 27 Nov 2011    
// Revised for safe string handling, added Silane gas testing
// B. Howard
//
////////////////////////////////////////////////////////////////////////
//
// Revision, 2 Sep 2023
// Upgraded to Visual Studio 2022; more revisions for safe string 
// handling.
// B. Howard
// 
////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <windows.h>
#include "bwrs.h"
#include "filtercdf.h"

#define FLUIDCOUNT 25

// Revision 2 Sep 2023, from CBWRS *eos to std::unique_ptr<CBWRS>
bool check_V_PT_USCS(std::unique_ptr<CBWRS>& eos, FILE *stream,
                     char *SourceFile,
                     double uncert)
{

    //local variables
    char outputline[2048];
    double fluidarray[FLUIDCOUNT];
    double xarray[1024];
    double T;
    double P;
    double Vpub;
    double V;
    int i;
    int j;
    int Cols;
    // Revision 2 Sep 2023, was class CFilterCDF filterCDF;
    auto filterCDF = std::make_unique<CFilterCDF>();

    //initialize variables
    i = 0;
    j = 0;
    T = 459.67;
    P = 0;
    V = 0;
    Vpub = 0;
    Cols = 0;
    for(i=0;i<1024;i++)
    {
        xarray[i] = 0;
    }

    // Check local array size compared to eos array size
    i = eos->GetFluidCount();
    j = sizeof( fluidarray ) / sizeof( *fluidarray );
    if( i != j ) 
    {
        strcpy_s( outputline, "the array, fluidarray, in check_V_PT_USCS is a different size than the fluidarray in the eos.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        return false;
    }

    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //Set the fluid index
    fluidarray[0] = 0.500;
    fluidarray[2] = 0.500;

    //Output fluid name
    fprintf( stream, "V_PT Properties Check\n" );

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    fprintf(stream, "Pressure, Temperature, Sp. Vol (pub), Sp. Vol (calc)\n"  );
    fprintf(stream, "(psia),(R),(ft3/lbm), (ft3/lbm)\n"  );

    //iterate through the dataset and find the pressure
    // Revision 2 Sep 2023, access via pointer instead of the object
    Cols = filterCDF->GetCols( SourceFile );
    j = filterCDF->GetRows( SourceFile, (int)Cols );
    filterCDF->LoadData( SourceFile, xarray, (i*j) );

    //evaluate each test point
    for( i = 0; i < j; i++ )
    {

        Vpub = ( xarray[ 0 + ( i * Cols )] );
        P = ( xarray[1 + ( i * Cols )] );
        T = ( xarray[ 2 + ( i * Cols )] );
        V = eos->GetV_TP_USCS( T, P );

        //Output the results
        fprintf( stream, "%f, %f, %f, %f\n", P, T, Vpub, V ); 

        //Validate the pressure calculation
        if( ( V < ( Vpub * ( 1 - uncert ) ) ) || 
            ( V > (Vpub * ( 1 + uncert ) ) ) )
        {
            return false;
        }

    }

    //done
    return true;
}

// Revision 2 Sep 2023, from CBWRS *eos to std::unique_ptr<CBWRS>
bool check_P_VS_USCS(std::unique_ptr<CBWRS>& eos, FILE *stream,
                     int FluidIndex, 
                     char *SourceFile,
                     double uncert)
{

    //local variables
    char outputline[2048];
    double fluidarray[FLUIDCOUNT];
    double xarray[1024];
    double V;
    double S;
    double Ppub;
    double P;
    int i;
    int j;
    int Cols;
    // Revision 2 Sep 2023, was class CFilterCDF filterCDF;
    auto filterCDF = std::make_unique<CFilterCDF>();

    //initialize variables
    i = 0;
    j = 0;
    V = 0;
    S = 0;
    P = 0;
    Ppub = 0;
    Cols = 0;
    for(i=0;i<1024;i++)
    {
        xarray[i] = 0;
    }

    // Check local array size compared to eos array size
    i = eos->GetFluidCount();
    j = sizeof( fluidarray ) / sizeof( *fluidarray );
    if( i != j ) 
    {
        strcpy_s( outputline, "the array, fluidarray, in check_P_VS_USCS is a different size than the fluidarray in the eos.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        return false;
    }

    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
    {
        fluidarray[i] = 0;
    }

    //Set the fluid index
    fluidarray[ ( FluidIndex - 1 )] = 1.00;

    //Output fluid name
    fprintf( stream, "P_VS Properties Check\n" );
    fprintf( stream, eos->GetName( FluidIndex ));
    fprintf( stream, "\n" );

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    fprintf(stream, "Sp. Vol,Entropy, Pressure (pub), Pressure (calc)\n"  );
    fprintf(stream, "(ft3/lbm),BTU/(lb-R),(psia), (psia)\n"  );

    //iterate through the dataset and find the pressure
    // Revision 2 Sep 2023, access via pointer instead of the object
    Cols = filterCDF->GetCols( SourceFile );
    j = filterCDF->GetRows( SourceFile, (int)Cols );
    filterCDF->LoadData( SourceFile, xarray, (i*j) );

    //evaluate each test point
    for( i = 0; i < j; i++ )
    {

        V = ( xarray[( i * Cols )] );
        S = ( xarray[ 1 + ( i * Cols )] );
        Ppub = ( xarray[ 2 + ( i * Cols )] );
        P = eos->GetP_VS_USCS( V, S );

        //Output the results
        fprintf( stream, "%f, %f, %f, %f\n", V, S, Ppub, P ); 

        //Validate the pressure calculation
        if( ( P < ( Ppub * ( 1 - uncert ) ) ) || 
            ( P > (Ppub * ( 1 + uncert ) ) ) )
        {
            return false;
        }

    }

    //done
    return true;
}

// Revision 2 Sep 2023, from CBWRS *eos to std::unique_ptr<CBWRS>
bool check_T_SP_USCS(std::unique_ptr<CBWRS>& eos, FILE *stream,
                     int FluidIndex, 
                     char *SourceFile,
                     double uncert)
{

    //local variables
    char outputline[2048];
    double fluidarray[FLUIDCOUNT];
    double xarray[1024];
    double T;
    double S;
    double Tpub;
    double P;
    int i;
    int j;
    int Cols;
    // Revision 2 Sep 2023, was class CFilterCDF filterCDF;
    auto filterCDF = std::make_unique<CFilterCDF>();

    //initialize variables
    i = 0;
    j = 0;
    T = 0;
    S = 0;
    P = 0;
    Tpub = 0;
    Cols = 0;
    for(i=0;i<1024;i++)
    {
        xarray[i] = 0;
    }

    // Check local array size compared to eos array size
    i = eos->GetFluidCount();
    j = sizeof( fluidarray ) / sizeof( *fluidarray );
    if( i != j ) 
    {
        strcpy_s( outputline, "the array, fluidarray, in check_T_SP_USCS is a different size than the fluidarray in the eos.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        return false;
    }

    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
    {
        fluidarray[i] = 0;
    }

    //Set the fluid index
    fluidarray[ ( FluidIndex - 1 )] = 1.00;

    //Output fluid name
    fprintf( stream, "T_SP Properties Check\n" );
    fprintf( stream, eos->GetName( FluidIndex ));
    fprintf( stream, "\n" );

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    fprintf(stream, "Entropy, Pressure , Temperature (pub), Temperature (calc)\n"  );
    fprintf(stream, "BTU/(lb-R),(psia), (R), (R)\n"  );

    //iterate through the dataset and find the pressure
    // Revision 2 Sep 2023, access via pointer instead of the object
    Cols = filterCDF->GetCols( SourceFile );
    j = filterCDF->GetRows( SourceFile, (int)Cols );
    filterCDF->LoadData( SourceFile, xarray, (i*j) );

    //evaluate each test point
    for( i = 0; i < j; i++ )
    {

        S = ( xarray[( i * Cols )] );
        P = ( xarray[ 1 + ( i * Cols )] );
        Tpub = ( xarray[ 2 + ( i * Cols )] );
        T = eos->GetT_SP_USCS( S, P );

        //Output the results
        fprintf( stream, "%f, %f, %f, %f\n", S, P, Tpub, T ); 

        //Validate the pressure calculation
        if( ( T < ( Tpub * ( 1 - uncert ) ) ) || 
            ( T > (Tpub * ( 1 + uncert ) ) ) )
        {
            return false;
        }

    }

    //done
    return true;
}

// Revision 2 Sep 2023, from CBWRS *eos to std::unique_ptr<CBWRS>
bool bCheckVaporPressure_SI(std::unique_ptr<CBWRS>& eos, FILE *stream,
                            int FluidIndex, 
                            char *SourceFile,
                            double uncert)
{

    //local variables
    int i;
    int j;
    int Cols;
    char outputline[2048];
    double xarray[1024];
    double fluidarray[FLUIDCOUNT];
    double T;
    double VapPress;
    double VapPressPub;
    double Vf;
    double VfPub;
    double Vg;
    double VgPub;
    double hf;
    double hfPub;
    // Revision 2 Sep 2023, was class CFilterCDF filterCDF;
    auto filterCDF = std::make_unique<CFilterCDF>();

    //initialize variables
    i = 0;
    j = 0;
    Cols = 0;
    T = 0;
    VapPress = 0;
    VapPressPub = 0;
    Vg = 0;
    VgPub = 0;
    for(i=0;i<1024;i++)
    {
        xarray[i] = 0;
    }

    // Check local array size compared to eos array size
    i = eos->GetFluidCount();
    j = sizeof( fluidarray ) / sizeof( *fluidarray );
    if( i != j ) 
    {
        strcpy_s( outputline, "the array, fluidarray, in bCheckVaporPressure_SI is a different size than the fluidarray in the eos.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        return false;
    }

    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
    {
        fluidarray[i] = 0;
    }

    //Set the fluid index
    fluidarray[ ( FluidIndex - 1 )] = 1.00;

    //Output fluid name
    fprintf( stream, "Vapor Pressure SI Properties Check\n" );
    fprintf( stream, eos->GetName( FluidIndex ));
    fprintf( stream, "\n" );

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    fprintf(stream, "Temperature,VaporPressure (pub),VaporPressure (calc), Vf (pub), Vf (calc), Vg (pub), Vg (calc), hf (pub), hf (calc)\n"  );
    fprintf(stream, "(K),(bar),(bar), (cm3/g), (cm3/g), (cm3/g), (cm3/g), (kJ/kg), (kJ/kg)\n"  );

    //iterate through the temperatures and find the 
    //vapor pressure properties
    //Open the test point set input file.  
    // Revision 2 Sep 2023, access via pointer instead of the object
    Cols = filterCDF->GetCols( SourceFile );
    j = filterCDF->GetRows( SourceFile, (int)Cols );
    filterCDF->LoadData( SourceFile, xarray, (i*j) );

    //evaluate each test point
    for( i = 0; i < j; i++ )
    {

        T = ( xarray[( i * Cols )] );
        VapPressPub = ( xarray[ 1 + ( i * Cols )] );
        VfPub = ( xarray[ 2 + ( i * Cols )] );
        VgPub = ( xarray[ 3 + ( i * Cols )] );
        hfPub = ( xarray[ 4 + ( i * Cols )] );
        VapPress = eos->GetVaporPressure_T_SI( T );
        Vf = eos->GetSatLiqV_T_SI( T );
        Vg = eos->GetSatVapV_T_SI( T );
        hf = eos->GetSatLiqH_T_SI( T );

        fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f, %f\n", T, 
            VapPressPub,
            VapPress,
            VfPub,
            Vf,
            VgPub,
            Vg,
            hfPub,
            hf );

        //Validate the Vapor Pressure calculation
        if( ( VapPress < ( VapPressPub * ( 1 - uncert ) ) ) || 
            ( VapPress > (VapPressPub * ( 1 + uncert ) ) ) )
        {
            return false;
        }

        //Validate the liquid specific volume calculation
        if( ( Vf < ( VfPub * ( 1 - uncert ) ) ) || 
            ( Vf > (VfPub * ( 1 + uncert ) ) ) )
        {
            return false;
        }

        //Validate the vapor specific volume calculation
        if( ( Vg < ( VgPub * ( 1 - uncert ) ) ) || 
            ( Vg > (VgPub * ( 1 + uncert ) ) ) )
        {
            return false;
        }

    }

    //done
    return true;
}

// Revision 2 Sep 2023, from CBWRS *eos to std::unique_ptr<CBWRS>
bool bCheckError(std::unique_ptr<CBWRS>& eos, FILE *stream, char *location )
{

    //local variables
    int i;
    char outputline[2048];

    //initialize local variables
    i = 0;
    strcpy_s(outputline, "");

    //check for errors
    if( eos->GetMessageCount() > 0 )
    {
        for( i=0; i<eos->GetMessageCount(); i++ )
        {
            //flag trouble for those messages with a priority less than 20
            if( eos->GetMessagePriority( i+1 ) < 20 )
            {
                printf( "Message %i (Priority %i): %s\n, %s\n", (i+1),
                    eos->GetMessagePriority( (i+1) ),
                    eos->GetMessageMain( i+1 ),
                    eos->GetMessageAux( i+1 ));
                fprintf( stream, "Message %i (Priority %i): %s\n, %s\n", (i+1),
                    eos->GetMessagePriority( (i+1) ),
                    eos->GetMessageMain( i+1 ),
                    eos->GetMessageAux( i+1 ));
                return false;
            }
        }

    }

    //clear the error handler
    eos->ClearMessages();

    //done
    return true;
}

// Revision 2 Sep 2023, from CBWRS *eos to std::unique_ptr<CBWRS>
bool displaymembers(std::unique_ptr<CBWRS>& eos, FILE *stream )
{

    //local variables
    char outputline[2048];
    int i;

    //initialize local variables
    strcpy_s(outputline, "");
    i = 0;

    //Begin by outputting total number of fluids in the model
    printf( "Total Fluid Count: %i\n", eos->GetFluidCount() );
    fprintf( stream, "Total Fluid Count: %i\n", eos->GetFluidCount() );

    //Output column headings - line 1
    strcpy_s( outputline, ",,,Secondary,,,Crit. Temp.,Crit. Temp., Crit. Pres., Crit. Pres.,Crit. Vol.,Crit. Vol.,Omega\n" );
    printf( outputline );
    fprintf( stream, outputline );

    //Output column headings - line 2
    strcpy_s( outputline, "TPGL Index,Formula,Name,Name,CAS #,Mo. Wt.,(R),(K),(psia),(bar),(ft3/mol),(cm3/mol),-\n" );
    printf( outputline );
    fprintf( stream, outputline );

    //Output members.
    for( i=1; i <= eos->GetFluidCount(); i++ )
    {
        printf( "%i,%s,%s,%s,%i-%i-%i,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetTPGLIndex( i ),
            eos->GetFormula( i ),
            eos->GetName( i ),
            eos->GetName_Alt01( i ),
            eos->GetCASNo( i )[2], 
            eos->GetCASNo( i )[1],
            eos->GetCASNo( i )[0],
            eos->GetMolWeight( i ),
            eos->GetTempCrit_USCS( i ),
            eos->GetTempCrit_SI( i),
            eos->GetPresCrit_USCS( i ),
            eos->GetPresCrit_SI( i ),
            eos->GetVolCrit_USCS( i ),
            eos->GetVolCrit_SI( i ),
            eos->GetEcc( i ));
        fprintf( stream, "%i,%s,%s,%s,%i-%i-%i,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetTPGLIndex( i ),
            eos->GetFormula( i ),
            eos->GetName( i ),
            eos->GetName_Alt01( i ),
            eos->GetCASNo( i )[2], 
            eos->GetCASNo( i )[1],
            eos->GetCASNo( i )[0],
            eos->GetMolWeight( i ),
            eos->GetTempCrit_USCS( i ),
            eos->GetTempCrit_SI( i),
            eos->GetPresCrit_USCS( i ),
            eos->GetPresCrit_SI( i ),
            eos->GetVolCrit_USCS( i ),
            eos->GetVolCrit_SI( i ),
            eos->GetEcc( i ));
    }

    //success
    return true;
}

// Revision 2 Sep 2023, from CBWRS *eos to std::unique_ptr<CBWRS>
bool displayBWRSParams(std::unique_ptr<CBWRS>& eos, FILE *stream )
{

    //local variables
    char outputline[2048];
    int i;

    //initialize local variables
    strcpy_s(outputline, sizeof(outputline), "");
    i = 0;

    //Output column headings - line 1
    strcpy_s(outputline, sizeof(outputline), "Fluid Formula, Name, Bo,Ao,Co,Do,Eo,b,a,d,alpha,c,gamma\n" );
    printf( outputline );
    fprintf( stream, outputline );

    //Output members.
    for( i=1; i <= eos->GetFluidCount(); i++ )
    {
        printf( "%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetFormula( i ),
            eos->GetName( i ),
            eos->GetBo( i ),
            eos->GetAo( i ),
            eos->GetCo( i ),
            eos->GetDo( i ),
            eos->GetEo( i ),
            eos->Getb( i ),
            eos->Geta( i ),
            eos->Getd( i ),
            eos->GetAlpha( i ),
            eos->Getc( i ),
            eos->GetGamma( i )); 
        fprintf( stream, "%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetFormula( i ),
            eos->GetName( i ),
            eos->GetBo( i ), 
            eos->GetAo( i ),
            eos->GetCo( i ),
            eos->GetDo( i ),
            eos->GetEo( i ), 
            eos->Getb( i ),
            eos->Geta( i ),
            eos->Getd( i ),
            eos->GetAlpha( i ),
            eos->Getc( i ),
            eos->GetGamma( i )); 
    }

    //success
    return true;
}

// main subroutine. 
// The returns no results to the user instead saving the results to a file.
void main ()
{

    //local variables
    bool bTemp;
    int i;
    int j;
    FILE *stream;
    // Revision, 2 Sep 2023, moved from stack to heap
    //class CBWRS *eos;
    std::unique_ptr<CBWRS> eos = std::make_unique<CBWRS>();
    const std::string str_filename = "CBWRSHarness.csv";
    char outputline[2048];
    double testvalue;
    double dTemp;
    double dTemp01;
    double dTemp02;
    double dTemp03;
    double temp;
    double press;
    double ent;
    double enth;
    // Revision, 2 Sep 2023, moved from stack to heap
    std::vector<double> xarray(1024);
    std::vector<double> yarray(1024);
    double r;
    double fluidarray[FLUIDCOUNT];
    double uncert;
    SYSTEMTIME SysTime;
    int CurrentTickCount;
    int PreviousTickCount;
    int LapsedTicks;

    //initialize local variables
    bTemp = false;
    i  = 0;
    j = 0;
    stream = NULL;
    strcpy_s(outputline, sizeof(outputline), "");
    testvalue = 3.14159;
    dTemp = 0;
    dTemp02 = 0;
    temp = 0;
    press = 0;
    ent = 0;
    enth = 0;
    for(i=0;i<1024;i++)
    {
        xarray[i] = 0.0;
        yarray[i] = 0;
    }
    r = 0;
    uncert = 0.05;
    CurrentTickCount = GetTickCount64();
    PreviousTickCount = GetTickCount64();
    LapsedTicks = ( CurrentTickCount - PreviousTickCount);

    /********************************************************************
    * The remainder of this program serves as a container and harness   *
    * to test the object members and methods.  Tests also               *
    * include exception handling                                        *
    *********************************************************************/

    /********************************************************************
    * Test Sequence                                                     *
    *	- Verify class instantiation                                    *
    *	- Verify member initialization                                  *
    *	- Verify method performance                                     *
    *********************************************************************/

    // Open the log file
    // Revision, 2 Sep 2023, changed to fopen_s, added error handling
    errno_t err = fopen_s(&stream, str_filename.c_str(), "w");
    if (err != 0 || stream == nullptr)
    {
        strcpy_s(outputline, sizeof(outputline), "Failed to open the log file.\n");
        printf(outputline);
        return;
    }

    // Intro line
    strcpy_s(outputline, sizeof(outputline), "CEOSModel Test through CBWRS Test Harness.\n" );
    printf( outputline );
    fprintf(stream, outputline );

    // Assemble the class
    // Revision, 2 Sep 2023, moved from stack to heap, std::unique_ptr so 
    // no need to instantiate class
    // eos	= new CBWRS();

    // Document instantiation of class
    if( eos == NULL )
    {
        strcpy_s(outputline, sizeof(outputline), "CBWRS Failed to Instantiate.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }
    else
    {
        strcpy_s(outputline, sizeof(outputline), "CBWRS Instantiated.\n" );
        printf( outputline );
        fprintf(stream, outputline );
    }

    /********************************************************************
    * Documemt member values as found on class instatiation				*
    *********************************************************************/
    if( !displaymembers( eos, stream ) )
    {
        strcpy_s(outputline, sizeof(outputline), "Member output failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    strcpy_s(outputline, sizeof(outputline), "\n" );
    printf( outputline );
    fprintf(stream, outputline );

    ////////////////////////////////////////////////////////////////////////
    // Check local array size compared to eos array size
    ////////////////////////////////////////////////////////////////////////
    i = eos->GetFluidCount();
    j = sizeof( fluidarray ) / sizeof( *fluidarray );
    if( i != j ) 
    {
        strcpy_s(outputline, sizeof(outputline), "the array, fluidarray, in main is a different size than the fluidarray in the eos.\n" );
        printf( outputline );
        fprintf( stream, outputline );
        eos.reset();
        return;
    }

    /********************************************************************
    * now we'll dive into the bwrs specific information                 *
    *********************************************************************/
    if( !displayBWRSParams( eos, stream ) )
    {
        strcpy_s(outputline, sizeof(outputline), "member output failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    /********************************************************************
    * The next series of tests checks the EOS component by component    *
    *********************************************************************/

    /********************************************************************
    * Calculate pressure at constant volume as temperature changes.     *
    * This is not so much a thermodynamic function as it is a way to    *
    * check the BWRS equation of state which describes pressure as a    *
    * function of specific molar volume and temperature, i.e. P(V,T).   *
    *********************************************************************/
    //Clear the message queue
    eos->ClearMessages();

    //Loop through the fluids in the engine
    for( j=0; j<eos->GetFluidCount(); j++ )
    {
        //Clear the array
        for( i=0; i<eos->GetFluidCount(); i++ )
            fluidarray[i]		= 0;

        //Set the fluid for methane
        fluidarray[j]			= 1.00;

        //Set the variables in the EOS object
        eos->SetMixtureData( fluidarray );
        eos->SetMaxIterations( 200 );
        eos->SetPrecision ( 0.000001 );

        //check for errors
        if( !bCheckError( eos, stream, "Failed to load fluid at P(T,V)\n" ) )
            return;

        //Set the headers.  Begin with the fluid name
        printf( "Name:,%s\n", eos->GetName( j+1 ) );
        fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

        //parameters
        strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,Spec. Vol.\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //units
        strcpy_s(outputline, sizeof(outputline), "(PSIA),(R),(ft3/lb)\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //Keep the volume steady and vary temperature
        for( dTemp = eos->GetTmin_USCS( j + 1 ); 
            dTemp < eos->GetTmax_USCS( j + 1 ); 
            dTemp = ( dTemp + 100 ) )
        {
            printf( "%f,%f,%f\n", 
                eos->GetP_VT_USCS( 0.03710, dTemp ), 
                dTemp, 
                0.03710 );
            fprintf( stream, "%f,%f,%f\n", 
                eos->GetP_VT_USCS( 0.03710, dTemp ), 
                dTemp, 
                0.03710 );
            //check for errors
            if( !bCheckError( eos, stream, "Failed to load fluid at P(T,V)\n" ) )
                return;
        }
    }

    /********************************************************************
    * The next series of tests checks the EOS component by component in *
    * SI units.                                                         *
    *********************************************************************/
    //Loop through the fluids in the engine
    for( j=0; j<eos->GetFluidCount(); j++ )
    {
        //Clear the array
        for( i=0; i<eos->GetFluidCount(); i++ )
            fluidarray[i] = 0;

        //Set the fluid
        fluidarray[j] = 1.00;

        //Set the variables in the EOS object
        eos->SetMixtureData( fluidarray );
        eos->SetMaxIterations( 200 );
        eos->SetPrecision ( 0.000001 );

        //check for errors
        if( !bCheckError( eos, stream, "Failed to load fluid at P(T,V)\n" ) )
            return;

        //Set the headers.  Begin with fluid name
        printf( "Name:,%s\n", eos->GetName( j+1 ) );
        fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

        //parameters
        strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,Spec. Vol.\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //units
        strcpy_s(outputline, sizeof(outputline), "(bar),(K),(cm3/g)\n" );
        printf( outputline );
        fprintf(stream, outputline );

        for( dTemp = eos->GetTmin_SI( j + 1 ); 
            dTemp < eos->GetTmax_SI( j + 1 ); 
            dTemp = ( dTemp + 100 ) )
        {
            printf( "%f,%f,%f\n", 
                eos->GetP_VT_SI( 251.0, dTemp ), 
                dTemp, 
                251.0 );
            fprintf( stream, "%f,%f,%f\n", 
                eos->GetP_VT_SI( 251.0, dTemp ), 
                dTemp, 
                251.0 );
            //check for errors
            if( !bCheckError( eos, stream, "Failed to load fluid at P(T,V)\n" ) )
                return;
        }
    }

    /********************************************************************
    * The next series of tests checks the EOS                           *
    * solver component by component starting with the specific volume   *
    * calculation                                                       *
    *********************************************************************/
    //Before checking the method, we'll check to see that exceptions
    //are handled
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid index to methane
    fluidarray[0] = 1.00;

    //clear the error handler
    eos->ClearMessages();

    // Load invalid temperature.
    dTemp = eos->GetV_TP_USCS( -100, 800 );

    //check for errors.  We should have at least one
    if( eos->GetMessageCount() > 0 )
    {

        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetV_TP_USCS correctly trapped a negative temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //clear the error handler
        eos->ClearMessages();

    }
    else
    {
        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetV_TP_USCS failed to trap a negative temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //Load bad pressure
    dTemp = eos->GetV_TP_USCS( 700, -800 );

    //check for errors.  We should have at least one
    if( eos->GetMessageCount() > 0 )
    {

        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetV_TP_USCS correctly trapped a negative pressure\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //clear the error handler
        eos->ClearMessages();

    }
    else
    {
        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetV_TP_USCS failed to trap a negative pressure\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //Loop through the fluids in the engine
    for( j=0; j<eos->GetFluidCount(); j++ )
    {
        //Clear the array
        for( i=0; i<eos->GetFluidCount(); i++ )
        {
            fluidarray[i] = 0.0;
        }

        // Set the fluid for current fluid index.
        fluidarray[j] = 1.00;

        // Set the variables in the EOS object.
        eos->SetMixtureData( fluidarray );
        eos->SetMaxIterations( 200 );
        eos->SetPrecision ( 0.000001 );

        // Check for errors in the message handler queue.
        bTemp = bCheckError( eos, stream, "Failed to load fluid when checking the solver\n" );
        if( bTemp == false )
        {
            strcpy_s(outputline, sizeof(outputline), "bCheckError Failed\n" );
            printf( outputline );
            fprintf(stream, outputline );
            eos.reset();
            return;
        }

        // Set the headers
        printf( "Name:,%s\n", eos->GetName( j+1 ) );
        fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

        // Parameters
        strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,SpecificVolume\n" );
        printf( outputline );
        fprintf(stream, outputline );

        // Units
        strcpy_s(outputline, sizeof(outputline), "(PSIA),(R),(ft3/lb)\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //pressure loop
        dTemp = eos->GetPresCrit_mx_USCS();
        bTemp = bCheckError( eos, stream, "Failed to GetPresCrit_mx_USCS\n" );
        if( bTemp == false )
        {
            strcpy_s(outputline, sizeof(outputline), "bCheckError Failed\n" );
            printf( outputline );
            fprintf(stream, outputline );
            return;
        }

        for( i = (int)( eos->GetPresCrit_mx_USCS() / 3 ) ; i< 1100; i=( i + 100 ) )
        {
            //temperature loop
            for( dTemp = eos->GetTmin_USCS( j + 1 ); 
                dTemp < eos->GetTmax_USCS( j + 1 ); 
                dTemp = ( dTemp + 20 ) )
            {
                printf( "%f,%f,%f\n", 
                    (double)i,
                    dTemp, 
                    eos->GetV_TP_USCS( dTemp, (double)i ) );
                fprintf( stream, "%f,%f,%f\n", 
                    (double)i,
                    dTemp, 
                    eos->GetV_TP_USCS( dTemp, (double)i ) );

                //check for errors
                if( !bCheckError( eos, stream, "Failed to GetV_TP_USCS check\n" ) )
                    return;
            }
        }
    }

    /********************************************************************
    * The next series of tests checks the EOS                           *
    * solver specific volume calculation in SI units                    *
    *********************************************************************/
    //Loop through the fluids in the engine
    for( j=0; j<eos->GetFluidCount(); j++ )
    {
        //Clear the array
        for( i=0; i<eos->GetFluidCount(); i++ )
            fluidarray[i]		= 0;

        //Set the fluid for current fluid index
        fluidarray[j]			= 1.00;

        //Set the variables in the EOS object
        eos->SetMixtureData( fluidarray );
        eos->SetMaxIterations( 200 );
        eos->SetPrecision ( 0.000001 );

        //check for errors
        if( !bCheckError( eos, stream, "Failed to load fluid when checking the solver\n" ) )
            return;

        //Set the headers
        printf( "Name:,%s\n", eos->GetName( j+1 ) );
        fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

        //parameters
        strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,SpecificVolume\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //units
        strcpy_s(outputline, sizeof(outputline), "(bar),(K),(cm3/kg)\n" );
        printf( outputline );
        fprintf(stream, outputline );

        for( dTemp = eos->GetTmin_SI( j + 1 ); 
            dTemp < eos->GetTmax_SI( j+ 1 ); 
            dTemp = ( dTemp + 20 ) )
        {
            printf( "%f,%f,%f\n", 
                (double)1,
                dTemp, 
                eos->GetV_TP_SI( dTemp, 1 ) );
            fprintf( stream, "%f,%f,%f\n", 
                (double)1,
                dTemp, 
                eos->GetV_TP_SI( dTemp, 1 ) );

            //check for errors
            if( !bCheckError( eos, stream, "Failed to GetV_TP_SI check\n" ) )
                return;
        }
    }

    /********************************************************************
    * The next series of tests checks the EOS entropy                   *
    * solver component by component                                     *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for air
    fluidarray[18]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //check for errors
    if( !bCheckError( eos, stream, "Failed to load fluid when checking the solver\n" ) )
        return;

    //Set the headers
    printf( "Name:,%s\n", eos->GetName( j+1 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

    strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,entropy\n" );
    printf( outputline );
    fprintf(stream, outputline );

    for( i=100; i<400; i=i+20 )
    {
        dTemp	= ( (double)i + 459.67 );
        printf( "%f,%f,%f\n", 
            (double)1,
            dTemp, 
            eos->GetS_TP_USCS( dTemp, 1 ) );
        fprintf( stream, "%f,%f,%f\n", 
            1.0,
            dTemp, 
            eos->GetS_TP_USCS( dTemp, 1 ) );
    }

    /********************************************************************
    * The next series of tests checks the EOS entropy in SI units       *
    * solver component by component                                     *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for water
    j=17;
    fluidarray[j]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    printf( "Name:,%s\n", eos->GetName( j+1 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

    strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,entropy\n" );
    printf( outputline );
    fprintf(stream, outputline );

    strcpy_s(outputline, sizeof(outputline), "(bar),(K),(J/g-K)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    for( i=300; i<400; i=i+20 )
    {
        dTemp	= ( (double)i );
        printf( "%f,%f,%f\n", 
            (double)1,
            dTemp, 
            eos->GetS_TP_SI( dTemp, 1 ) );
        fprintf( stream, "%f,%f,%f\n", 
            (double)1,
            dTemp, 
            eos->GetS_TP_SI( dTemp, 1 ) );
    }

    /********************************************************************
    * The next series of tests checks the EOS enthalpy departure        *
    * function in USCS                                                  *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for air
    fluidarray[18]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,enthalpy\n" );
    printf( outputline );
    fprintf(stream, outputline );

    strcpy_s(outputline, sizeof(outputline), "(PSIA),(K),(BTU/lb)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    printf( "%f,%f,%f\n", 
        (double)14.7,
        (double)559.67, 
        eos->GetH_TP_USCS( 559.67, 14.7 ) );
    fprintf( stream, "%f,%f,%f\n", 
        (double)14.7,
        (double)559.67, 
        eos->GetH_TP_USCS( 559.67, 14.7 ) );

    /********************************************************************
    * The next series of tests checks the EOS enthalpy departure		*
    * function in SI													*
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for water
    fluidarray[18]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    strcpy_s(outputline, sizeof(outputline), "Pressure,Temperature,enthalpy\n" );
    printf( outputline );
    fprintf(stream, outputline );

    strcpy_s(outputline, sizeof(outputline), "(bar),(K),(kJ/kg)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    printf( "%f,%f,%f\n", 
        (double)1,
        (double)90, 
        eos->GetH_TP_SI( 90, 1 ) );
    fprintf( stream, "%f,%f,%f\n", 
        (double)1,
        (double)90, 
        eos->GetH_TP_SI( 90, 1 ) );

    ////////////////////////////////////////////////////////////////////////
    // The next series of tests checks the vapor pressure function for
    // the fluid of interest in USCS units
    ////////////////////////////////////////////////////////////////////////
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for hexane
    fluidarray[7]			= 1.00;

    //Output fluid name
    fprintf( stream, eos->GetName( 8 ));
    fprintf( stream, "\n" );

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    strcpy_s(outputline, sizeof(outputline), "Temperature,VaporPressure,Vf, Vg, Hf, Hg, Sf, Sg\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //iterate through the temperatures and find the 
    //vapor pressures
    for( i = 0; i < 10; i++ )
    {

        dTemp	= ( ( 879.67 ) + ( (double)i ) );
        dTemp02   = eos->GetVaporPressure_T_USCS( dTemp );
        printf( "%f, %f, %f, %f, %f, %f, %f, %f\n",	dTemp, 
            dTemp02,
            eos->GetSatLiqV_T_USCS( dTemp ),
            eos->GetSatVapV_T_USCS( dTemp ),
            eos->GetSatLiqH_T_USCS( dTemp ),
            eos->GetSatVapH_T_USCS( dTemp ),
            eos->GetSatLiqS_T_USCS( dTemp ),
            eos->GetSatVapS_T_USCS( dTemp ) );
        fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f\n", dTemp, 
            dTemp02,
            eos->GetSatLiqV_T_USCS( dTemp ),
            eos->GetSatVapV_T_USCS( dTemp ),
            eos->GetSatLiqH_T_USCS( dTemp ),
            eos->GetSatVapH_T_USCS( dTemp ),
            eos->GetSatLiqS_T_USCS( dTemp ),
            eos->GetSatVapS_T_USCS( dTemp ) );
    }

    /********************************************************************
    * The next series of tests checks the vapor pressure function for   *
    * the fluid of interest as a function of pressure in USCS			*
    *********************************************************************/
    //Loop through the fluids in the engine
    for( j=0; j<1; j++ )
    {
        //Clear the array
        for( i=0; i<eos->GetFluidCount(); i++ )
            fluidarray[i]		= 0;

        //Set the fluid
        fluidarray[j]			= 1.00;

        //Set the headers
        printf( "Name:,%s\n", eos->GetName( j+1 ) );
        fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

        //Set the variables in the EOS object
        eos->SetMixtureData( fluidarray );
        eos->SetMaxIterations( 200 );
        eos->SetPrecision ( 0.000001 );

        //Set the headers
        strcpy_s(outputline, sizeof(outputline), "Pressure,Vapor Temperature, Vf, Vg, Hf, Hg, Sf, Sg\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //iterate through the pressures and find the 
        //vapor temperatures
        for( i = 0; i < 5; i++ )
        {

            dTemp	= ( ( 46.41 ) + ( (double)i / 10 ) );
            printf( "%f, %f, %f, %f, %f, %f, %f, %f\n",	dTemp, 
                eos->GetVaporTemperature_P_USCS( dTemp ),
                eos->GetSatLiqV_P_USCS( dTemp ),
                eos->GetSatVapV_P_USCS( dTemp ),
                eos->GetSatLiqH_P_USCS( dTemp ),
                eos->GetSatVapH_P_USCS( dTemp ),
                eos->GetSatLiqS_P_USCS( dTemp ),
                eos->GetSatVapS_P_USCS( dTemp ) );
            fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f\n", dTemp, 
                eos->GetVaporTemperature_P_USCS( dTemp ),
                eos->GetSatLiqV_P_USCS( dTemp ),
                eos->GetSatVapV_P_USCS( dTemp ),
                eos->GetSatLiqH_P_USCS( dTemp ),
                eos->GetSatVapH_P_USCS( dTemp ),
                eos->GetSatLiqS_P_USCS( dTemp ),
                eos->GetSatVapS_P_USCS( dTemp ) );
        }
    }

    ////////////////////////////////////////////////////////////////////////
    //
    // The next series of tests checks the vapor pressure function for
    // the pure methane component in SI units
    //
    ////////////////////////////////////////////////////////////////////////
    //
    // Data was take from Source 4 (Reynolds_ which uses a different EOS
    // to generate the data. However values should still match within 
    // 5% on all values
    //
    ////////////////////////////////////////////////////////////////////////
    bTemp = bCheckVaporPressure_SI( eos, stream, 1, 
        "Methane_VaporPressure_SI.tst",
        0.05);    
    if( bTemp == false )
    {

        

        //Document failure of test
        strcpy_s(outputline, sizeof(outputline), "Vapor pressure, SI, for methane failed.\n" );
        printf(outputline);
        fprintf(stream, outputline );

        //close the output file
        fclose( stream );

        //exit
        return; 
    }

    ////////////////////////////////////////////////////////////////////////
    //
    // The next series of tests checks the vapor pressure function for
    // the pure ammonia component in SI units
    //
    ////////////////////////////////////////////////////////////////////////
    //
    // Data was take from Source 4 (Reynolds_ which uses a different EOS
    // to generate the data. Plus this case uses the generic constants
    // based on eccentricy.  However values should still match within 
    // 12% on all properties
    //
    ////////////////////////////////////////////////////////////////////////
    if( !bCheckVaporPressure_SI( eos, stream, 17, 
        "Ammonia_VaporPressure_SI.tst",
        0.12) )
    {

        eos.reset();

        //Document failure of test
        strcpy_s(outputline, sizeof(outputline), "Vapor pressure, SI, for ammonia failed.\n" );
        printf(outputline);
        fprintf(stream, outputline );

        //close the output file
        fclose( stream );

        //exit
        return; 
    }

    /********************************************************************
    * The next series of tests checks the vapor pressure function for   *
    * the fluid of interest as a function of pressure in SI				*
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for hexane
    fluidarray[7]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Output fluid name
    fprintf( stream, eos->GetName( 8 ));
    fprintf( stream, "\n" );

    //Set the headers
    strcpy_s(outputline, sizeof(outputline), "Pressure,Vapor Temperature, Vf, Vg, Hf, Hg, Sf, Sg\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //Set the headers
    strcpy_s(outputline, sizeof(outputline), "(bar),(K), (cm3/g), (cm3/g), (kJ/kg), (kJ/kg), (kJ/kg-K), (kJ/kg-K)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //iterate through the pressures and find the 
    //vapor temperatures
    for( i = 0; i < 5; i++ )
    {

        dTemp	= ( ( 0.02 ) + ( (double)i / 100 ) );
        fprintf( stream, "%f, %f, %f, %f, %f, %f, %f, %f\n", dTemp, 
            eos->GetVaporTemperature_P_SI( dTemp ),
            eos->GetSatLiqV_P_SI( dTemp ),
            eos->GetSatVapV_P_SI( dTemp ),
            eos->GetSatLiqH_P_SI( dTemp ),
            eos->GetSatVapH_P_SI( dTemp ),
            eos->GetSatLiqS_P_SI( dTemp ),
            eos->GetSatVapS_P_SI( dTemp ) );
    }

    /********************************************************************
    * The next series of tests checks the EOS temperature as a function *
    * of entropy and pressure solver in USCS                            *
    *********************************************************************/
    //Before checking the method, we'll check to see that exceptions
    //are handled
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid index to methane
    fluidarray[0]			= 1.00;

    //clear the error handler
    eos->ClearMessages();

    //Load bad pressure
    dTemp = eos->GetT_SP_USCS( 2.369001468, -10 );

    //check for errors.  We should have at least one
    if( eos->GetMessageCount() > 0 )
    {

        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetT_SP_USCS correctly trapped a negative temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //include the error
        strcpy_s(outputline, sizeof(outputline), eos->GetMessageMain( 1 ) );
        printf( outputline );
        fprintf(stream, outputline );

        //eol character
        strcpy_s(outputline, sizeof(outputline), "\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //clear the error handler
        eos->ClearMessages();

    }
    else
    {
        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetT_SP_USCS failed to trap a negative temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //Load bad entropy
    dTemp = eos->GetT_SP_USCS( -2.369001468, 10 );

    //check for errors.  We should have at least one with a high priority.
    if( eos->GetMessageCount() > 0 )
    {

        //describe the result
        strcpy_s(outputline, sizeof(outputline), "GetT_SP_USCS correctly trapped a negative entropy\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //include the error
        strcpy_s(outputline, sizeof(outputline), eos->GetMessageMain( 1 ) );
        printf( outputline );
        fprintf(stream, outputline );

        //eol character
        strcpy_s(outputline, sizeof(outputline), "\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //clear the error handler
        eos->ClearMessages();

    }
    else
    {
        //parameters
        strcpy_s( outputline, "GetT_SP_USCS failed to trap a negative entropy\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for air
    fluidarray[18]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    printf( "Name:,%s\n", eos->GetName( j+1 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

    strcpy_s( outputline, "Pressure, Entropy, Temperature,\n" );
    printf( outputline );
    fprintf(stream, outputline );

    printf( "%f,%f,%f\n", 
        (double)1,
        2.521572, 
        eos->GetT_SP_USCS( 2.521572, 1 ) );
    fprintf( stream, "%f,%f,%f\n", 
        1.0,
        2.521572, 
        eos->GetT_SP_USCS( 2.521572, 1 ) );

    //The following mixture came from the PetroCanada mixture and gave the solver
    //some grief
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for petrocanada mixture
    fluidarray[0]			= 0.04;
    fluidarray[1]			= 0.02;
    fluidarray[2]			= 0.015;
    fluidarray[14]			= 0.005;
    fluidarray[15]			= 0.92;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Clear the message handler
    eos->ClearMessages();

    //Set the output file headers
    fprintf(stream, "PetroCanada Mixture Test\n" );
    fprintf(stream, "Pressure, Entropy, Temperature,\n" );

    //Pressure    
    dTemp01 = 1000.0;

    //Entropy at above pressure and 100 deg. F
    dTemp02 = 15.0348;

    //Make the calculation
    dTemp = eos->GetT_SP_USCS( dTemp02, dTemp01 );

    //check to see if any errors were generated
    if( eos->GetMessageCount() > 0 )
    {
        strcpy_s( outputline, "T_SP_UCSC failed unexpectedly using the gas mixture from PetroCanada\n" );
        printf( outputline );
        fprintf(stream, outputline );
        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s( outputline, "\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    //the calculation succeeded
    fprintf( stream, "%f,%f,%f\n", 
        dTemp01,
        dTemp02, 
        dTemp );

    //check for resonable answer
    if( ( dTemp > 560 ) || ( dTemp < 540 ) )
    {
        strcpy_s( outputline, "T_SP_UCSC failed to return reasonable value using the gas mixture from PetroCanada\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    //The following mixture came from the BP Whiting mixture and gave the solver
    //some grief
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for petrocanada mixture
    fluidarray[0]			= 0.1121;
    fluidarray[1]			= 0.0647;
    fluidarray[2]			= 0.0177;
    fluidarray[3]			= 0.0026;
    fluidarray[4]			= 0.0029;
    fluidarray[8]			= 0.0070;
    fluidarray[12]			= 0.0035;
    fluidarray[14]			= 0.0124;
    fluidarray[15]			= 0.7771;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Clear the message handler
    eos->ClearMessages();

    //Set the output file headers
    fprintf(stream, "BP Whiting Mixture Test\n" );
    fprintf(stream, "Pressure, Entropy, Temperature,\n" );

    //Suction pressure, psia    
    dTemp01 = 151.3;

    //Entropy at discharge pressure (336.9 psia) and 679.7 deg. R
    dTemp02 = 20.5908;

    //Make the calculation
    dTemp = eos->GetT_SP_USCS( dTemp02, dTemp01 );

    //check to see if any errors were generated
    if( eos->GetMessageCount() > 0 )
    {
        strcpy_s( outputline, "T_SP_UCSC failed unexpectedly using the gas mixture from BP Whiting\n" );
        printf( outputline );
        fprintf(stream, outputline );
        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s( outputline, "\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    //the calculation succeeded
    fprintf( stream, "%f,%f,%f\n", 
        dTemp01,
        dTemp02, 
        dTemp );

    //check for resonable answer
    if( ( dTemp > 635 ) || ( dTemp < 625 ) )
    {
        strcpy_s( outputline, "T_SP_UCSC failed to return reasonable value using the gas mixture from BP Whiting\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    /********************************************************************
    * The next series of tests checks the EOS temperature as a function *
    * of entropy and pressure solver in SI                              *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for water
    j						= 17;
    fluidarray[j]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //label the test
    fprintf(stream, "Testing T_SP_SI method now for water\n" );


    //Set the headers
    printf( "Name:,%s\n", eos->GetName( j+1 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

    strcpy_s(outputline, sizeof(outputline), "Pressure, Entropy, Temperature,\n" );
    printf( outputline );
    fprintf(stream, outputline );

    printf( "%f,%f,%f\n", 
        (double)0.035197,
        4.9225, 
        eos->GetT_SP_SI( 4.9225, 0.035197 ) );
    fprintf( stream, "%f,%f,%f\n", 
        0.035197,
        4.9225, 
        eos->GetT_SP_SI( 4.9225, 0.035197 ) );


    ////////////////////////////////////////////////////////////////////////
    //
    // Data was take from Source 2 (Starling) for this test case.  Values 
    // should match within 1% on all samples.
    //
    ////////////////////////////////////////////////////////////////////////
    bTemp =check_T_SP_USCS( eos, stream, 1, 
        "Methane_T_SP_USCS.tst",
        0.01); 
    if( bTemp == false )
    {

        eos.reset();

        //Document failure of test
        strcpy_s( outputline, "T_SP_USCS for methane failed.\n" );
        printf(outputline);
        fprintf(stream, outputline );

        //close the output file
        fclose( stream );

        //exit
        return; 
    }

    /********************************************************************
    * The next series of tests checks the EOS temperature as a function *
    * of enthalpy and pressure solver in USCS                           *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for air
    fluidarray[18]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    printf( "Name:,%s\n", eos->GetName( 18 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( 18 ) );

    strcpy_s( outputline, "Pressure, Enthalpy, Temperature,\n" );
    printf( outputline );
    fprintf(stream, outputline );

    printf( "%f,%f,%f\n", 
        (double)14.7,
        132.0636, 
        eos->GetT_HP_USCS( 132.0636, 14.7 ) );
    fprintf( stream, "%f,%f,%f\n", 
        14.7,
        132.0636, 
        eos->GetT_HP_USCS( 132.0636, 14.7 ) );

    /********************************************************************
    * The next series of tests checks the EOS temperature as a function *
    * of enthalpy and pressure solver in SI                             *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]		= 0;

    //Set the fluid for R-134a
    j						= 23;
    fluidarray[j]			= 1.00;

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    printf( "Name:,%s\n", eos->GetName( j + 1 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( j + 1 ) );

    strcpy_s( outputline, "Pressure, Enthalpy, Temperature\n" );
    printf( outputline );
    fprintf(stream, outputline );

    strcpy_s( outputline, "(bar), (kJ/kg), (K)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    printf( "%f,%f,%f\n", 
        (double)20,
        73.29879, 
        eos->GetT_HP_SI( 73.29879, 20 ) );
    fprintf( stream, "%f,%f,%f\n", 
        20.0,
        73.29879, 
        eos->GetT_HP_SI( 73.29879, 20 ) );

    /********************************************************************
    * Test the ability to find enthalpy as a function of temperature    *
    * and entropy                                                       *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]            = 0;

    //Clear the message handler
    eos->ClearMessages();

    //Set the fluid for methane
    j                        = 0;
    fluidarray[j]            = 1.00;

    //At 100 degrees F and 14.7 PSIA S=2.4640 (Ref. 1 atm)
    //BTU/lb-R and H=280.68 BTU/lb
    //Lets see if we get the same result

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    printf( "Calculate H_TS For Name: %s\n", eos->GetName( j + 1 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( j + 1 ) );

    //setup the input with a bad temperature
    ent                      = 2.4640;
    temp                     = 2;
    press                    = 14.7;

    //run the calculation and check the response
    dTemp = eos->GetH_TS_USCS( temp, ent );

    if( eos->GetMessageCount() <=0 )
    {
        strcpy_s( outputline, "GetH_TS_USCS failed to capture bad temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;

    }
    else
    {
        strcpy_s( outputline, "GetH_TS_USCS captured bad temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );
        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s( outputline, "\n" );
        printf( outputline );
        fprintf(stream, outputline );
    }

    //outline the response
    strcpy_s( outputline, "Temperature, Entropy, Enthalpy\n" );
    printf( outputline );
    fprintf(stream, outputline );
    strcpy_s( outputline, "(R), (BTU/lb-R), (BTU/lb)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //setup the input with good data
    ent                      = 2.4640;
    temp                     = 559.67;
    press                    = 14.7;

    eos->SetPrecision ( 0.00001 );

    //Clear the message handler
    eos->ClearMessages();

    //run the calculation and check the response
    dTemp = eos->GetH_TS_USCS( temp, ent );

    //check that the solver converged
    if( eos->GetMessageCount() >0 )
    {

        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s( outputline, "\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;

    }

    //Check that the solver converged to the correct value
    if( !( ( dTemp < 281.0 ) && ( dTemp > 280.0 ) ) )
    {
        strcpy_s( outputline, "GetH_TS_USCS failed\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    printf( "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );
    fprintf( stream, "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );

    //setup the input with good data
    ent                      = 1.2111;
    temp                     = 299.67;
    press                    = 300.0;

    //Clear the message handler
    eos->ClearMessages();

    //run the calculation and check the response
    dTemp = eos->GetH_TS_USCS( temp, ent );

    //check that the solver converged
    if( eos->GetMessageCount() >0 )
    {

        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s( outputline, "\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;

    }

    printf( "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );
    fprintf( stream, "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );

    //restore good data that only takes a reasonable amount of time
    ent                           = 2.4640;
    temp                          = 559.67;
    press                         = 14.7;

    //Finally, a performance test
    GetLocalTime( &SysTime );
    PreviousTickCount             = CurrentTickCount;
    CurrentTickCount              = GetTickCount();
    LapsedTicks                   = ( CurrentTickCount - PreviousTickCount );
    fprintf(stream, "%i:%02i:%02i:%003i\n", 
        (int)SysTime.wHour,
        (int)SysTime.wMinute,
        (int)SysTime.wSecond,
        (int)SysTime.wMilliseconds);
    fprintf(stream, "Number of milliseconds: %i\n", LapsedTicks ); 

    for( i=0; i<1000; i++)
    {
        //run the calculation and check the response
        dTemp = eos->GetH_TS_USCS( temp, ent );
        printf( "%i\n",i);
    }

    GetLocalTime( &SysTime );
    PreviousTickCount             = CurrentTickCount;
    CurrentTickCount              = GetTickCount();
    LapsedTicks                   = ( CurrentTickCount - PreviousTickCount );
    fprintf(stream, "%i:%02i:%02i:%003i\n", 
        (int)SysTime.wHour,
        (int)SysTime.wMinute,
        (int)SysTime.wSecond,
        (int)SysTime.wMilliseconds);
    fprintf(stream, "Number of milliseconds: %i\n", LapsedTicks ); 

    //setup the input with good data
    ent                      = 2.7369;
    temp                     = 499.67;
    press                    = 1.0;

    //Clear the message handler
    eos->ClearMessages();

    //run the calculation and check the response
    dTemp = eos->GetH_TS_USCS( temp, ent );

    //check that the solver converged
    if( eos->GetMessageCount() >0 )
    {

        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s( outputline, "\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;

    }

    //Check that the solver converged to the correct value
    if( !( ( dTemp < 250.0 ) && ( dTemp > 247.0 ) ) )
    {
        strcpy_s( outputline, "GetH_TS_USCS failed\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    printf( "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );
    fprintf( stream, "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );

    //setup the input with good data
    ent                      = 1.2111;
    temp                     = 299.67;
    press                    = 300.0;

    //Clear the message handler
    eos->ClearMessages();

    //For this case, we have to have a few more iterations
    eos->SetMaxIterations( 400 );

    //run the calculation and check the response
    dTemp = eos->GetH_TS_USCS( temp, ent );

    //check that the solver converged
    if( ( eos->GetMessageCount() > 0 ) && ( dTemp == 0 ) )
    {

        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s( outputline, "\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;

    }

    //Check that the solver converged to the correct value
    if( !( ( dTemp < -29 ) && ( dTemp > -31 ) ) )
    {
        strcpy_s(outputline, sizeof(outputline), "GetH_TS_USCS failed\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    printf( "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );
    fprintf( stream, "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );

    //////////////////////////////////////////////////////////////////////
    // Validate P_VS methods
    //////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////
    //
    // The next series of tests checks the GetP_VS methods
    //
    ////////////////////////////////////////////////////////////////////////
    //
    // Begin by checking to see that illegal values are trapped
    //
    ////////////////////////////////////////////////////////////////////////

    //clear the error handler
    eos->ClearMessages();

    //Load bad volume
    dTemp = eos->GetP_VS_USCS( -133.3362, 2.2777 );

    //check for errors.  We should have at least one
    if( eos->GetMessageCount() > 0 )
    {

        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetP_VS_USCS correctly trapped a negative volume\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //include the error
        strcpy_s(outputline, sizeof(outputline), eos->GetMessageMain( 1 ) );
        printf( outputline );
        fprintf(stream, outputline );

        //eol character
        strcpy_s(outputline, sizeof(outputline), "\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //clear the error handler
        eos->ClearMessages();

    }
    else
    {
        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetP_VS_USCS failed to trap a negative volume\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        return;
    }

    //Load bad entropy
    dTemp = eos->GetP_VS_USCS( 133.3362, -2.2777 );

    //check for errors.  We should have at least one with a high priority.
    if( eos->GetMessageCount() > 0 )
    {

        //describe the result
        strcpy_s(outputline, sizeof(outputline), "GetP_VS_USCS correctly trapped a negative entropy\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //include the error
        strcpy_s(outputline, sizeof(outputline), eos->GetMessageMain( 1 ) );
        printf( outputline );
        fprintf(stream, outputline );

        //eol character
        strcpy_s(outputline, sizeof(outputline), "\n" );
        printf( outputline );
        fprintf(stream, outputline );

        //clear the error handler
        eos->ClearMessages();

    }
    else
    {
        //parameters
        strcpy_s(outputline, sizeof(outputline), "GetP_VS_USCS failed to trap a negative entropy\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        return;
    }

    ////////////////////////////////////////////////////////////////////////
    //
    // Data was take from Source 2 (Starling) for this test case.  Values 
    // should match within 1% on all samples.
    //
    ////////////////////////////////////////////////////////////////////////
    if( !check_P_VS_USCS( eos, stream, 1, 
        "Methane_P_VS_USCS.tst",
        0.01) )
    {

        eos.reset();

        //Document failure of test
        strcpy_s(outputline, sizeof(outputline), "P_VS_USCS for methane failed.\n" );
        printf(outputline);
        fprintf(stream, outputline );

        //close the output file
        fclose( stream );

        //exit
        return; 
    }

    ////////////////////////////////////////////////////////////////////////
    //
    // Test harness for P_VS for mixtures (test case #1)
    //
    ////////////////////////////////////////////////////////////////////////

    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture composition
    fluidarray[0]=0.84534;
    fluidarray[1]=0.06851;
    fluidarray[2]=0.03263;
    fluidarray[4]=0.01605;
    fluidarray[6]=0.00605;
    fluidarray[7]=0.00199;
    fluidarray[8]=0.00049;
    fluidarray[9]=0.00049;
    fluidarray[12]=0.02665;
    fluidarray[14]=0.00180;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture in P_VS should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }     

    //output the results
    fprintf( stream, "\n" );
    fprintf( stream, "Beginning mixture test case #1 for P_VS_USCS\n" );
    fprintf( stream, "Given pressure and temperature, the engine returns entropy\n" );
    fprintf( stream, "Temperature, Pressure, Entropy, Sp. Vol\n");
    fprintf( stream, "R,PSIA,BTU/lb-R,ft3/lbm\n" );
    dTemp = 564.67;
    press = 5037.1;
    dTemp01 = press;
    dTemp02 = eos->GetS_TP_USCS( dTemp, dTemp01 );
    dTemp03 = eos->GetV_TP_USCS( dTemp, dTemp01 );
    fprintf( stream, "%f,%f,%f,%f\n", dTemp, dTemp01, 
        dTemp02,
        dTemp03 );

    fprintf( stream, "Now taking the above pressure and entropy into P_VS\n");
    dTemp01 = eos->GetP_VS_USCS( dTemp03, dTemp02 );

    if( ( dTemp01 < (press * 0.99) ) || ( dTemp01 > (press * 1.01) ) )
    {
        strcpy_s(outputline, sizeof(outputline), "P_VS mixture test case #1 failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    fprintf( stream, "%f,%f,%f,%f\n", dTemp, dTemp01, 
        dTemp02,
        dTemp03 );


    ////////////////////////////////////////////////////////////////////////
    //
    // Test harness for P_VS for mixtures (test case #2)
    //
    ////////////////////////////////////////////////////////////////////////

    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture composition
    fluidarray[0]=0.84534;
    fluidarray[1]=0.06851;
    fluidarray[2]=0.03263;
    fluidarray[4]=0.01605;
    fluidarray[6]=0.00605;
    fluidarray[7]=0.00199;
    fluidarray[8]=0.00049;
    fluidarray[9]=0.00049;
    fluidarray[12]=0.02665;
    fluidarray[14]=0.00180;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture in P_VS should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }     

    //output the results
    fprintf( stream, "\n" );
    fprintf( stream, "Beginning mixture test case #2 for P_VS_USCS\n" );
    fprintf( stream, "Given pressure and temperature, the engine returns entropy\n" );
    fprintf( stream, "Temperature, Pressure, Entropy, Sp. Vol\n");
    fprintf( stream, "R,PSIA,BTU/lb-R,ft3/lbm\n" );
    dTemp = 568.973;
    press = 5281.2035;
    dTemp01 = press;
    dTemp02 = eos->GetS_TP_USCS( dTemp, dTemp01 );
    dTemp03 = eos->GetV_TP_USCS( dTemp, dTemp01 );
    fprintf( stream, "%f,%f,%f,%f\n", dTemp, dTemp01, 
        dTemp02,
        dTemp03 );

    fprintf( stream, "Now taking the above pressure and entropy into P_VS\n");
    dTemp01 = eos->GetP_VS_USCS( dTemp03, dTemp02 );

    if( ( dTemp01 < (press * 0.99) ) || ( dTemp01 > (press * 1.01) ) )
    {
        strcpy_s(outputline, sizeof(outputline), "P_VS mixture test case #2 failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }

    fprintf( stream, "%f,%f,%f,%f\n", dTemp, dTemp01, 
        dTemp02,
        dTemp03 );


    ////////////////////////////////////////////////////////////////////////
    //
    // Speed of sound testing???
    //
    ////////////////////////////////////////////////////////////////////////

    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture composition for 100% nitrogen
    fluidarray[14] = 1.000;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }     

    //output the results
    fprintf( stream, "Beginning speed of sound testing for nitrogen\n" );
    fprintf( stream, "Parameter,Units,%s\n", eos->GetName( 15 ) );
    fprintf( stream, "Tc,K,%f\n", eos->GetTempCrit_SI( 15 ) );
    fprintf( stream, "Vc,cm3/mol,%f\n", eos->GetVolCrit_SI( 15 ) );
    fprintf( stream, "eccentricity,,%f\n", eos->GetEcc( 15 ) );
    fprintf( stream, "dipole,debye,%f\n", eos->GetDipole_SI( 15 ) );
    fprintf( stream, "M,g/mol,%f\n", eos->GetMolWeight( 15 ) );
    fprintf( stream, "Mixture,,%f\n", eos->GetPercentMW( 15 ) );

    temp = eos->GetP_VS_USCS( 13.54809962, 1.4347525712136329 );
    fprintf( stream, "Pressure(V-S) (psia) = ,%f\n", temp  );

    if( ( temp < 14.695 ) || ( temp > 14.705 ) )
    {
        eos.reset();
        printf("GetP_VS_USCS failed.\n" );
        fprintf(stream, "GetP_VS_SI failed.\n" );
        return;
    }

    printf( "GetP_VS_USCS test succeeded.\n" );

    /********************************************************************
    * Test the ability to find enthalpy as a function of pressure       *
    * and entropy                                                       *
    *********************************************************************/
    //Clear the array
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i]            = 0;

    //Clear the message handler
    eos->ClearMessages();

    //Set the fluid for methane
    j                        = 0;
    fluidarray[j]            = 1.00;

    //At 100 degrees F and 14.7 PSIA S=2.4640 (Ref. 1 atm)
    //BTU/lb-R and H=280.68 BTU/lb
    //Lets see if we get the same result

    //Set the variables in the EOS object
    eos->SetMixtureData( fluidarray );
    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //Set the headers
    printf( "Calculate H_PS For Name: %s\n", eos->GetName( j + 1 ) );
    fprintf(stream, "Name:,%s\n", eos->GetName( j + 1 ) );

    //setup the input with a bad pressure
    ent                      = 2.4640;
    temp                     = 559.67;
    press                    = -1;

    //run the calculation and check the response
    dTemp = eos->GetH_PS_USCS( press, ent );

    if( eos->GetMessageCount() <=0 )
    {

        strcpy_s(outputline, sizeof(outputline), "GetH_PS_USCS failed to capture bad temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;

    }
    else
    {
        strcpy_s(outputline, sizeof(outputline), "GetH_PS_USCS captured bad temperature\n" );
        printf( outputline );
        fprintf(stream, outputline );
        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s(outputline, sizeof(outputline), "\n" );
        printf( outputline );
        fprintf(stream, outputline );
    }

    //outline the response
    strcpy_s(outputline, sizeof(outputline), "Temperature, Entropy, Enthalpy\n" );
    printf( outputline );
    fprintf(stream, outputline );
    strcpy_s(outputline, sizeof(outputline), "(R), (BTU/lb-R), (BTU/lb)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //setup the input with good data
    ent                      = 2.4640;
    temp                     = 559.67;
    press                    = 14.7;

    //Clear the message handler
    eos->ClearMessages();

    //run the calculation and check the response
    dTemp = eos->GetH_PS_USCS( press, ent );

    //check that the solver converged
    if( ( eos->GetMessageCount() > 0 ) && ( dTemp == 0 ) )
    {

        printf( eos->GetMessageMain( 1 ) );
        fprintf( stream, eos->GetMessageMain( 1 ) );
        strcpy_s(outputline, sizeof(outputline), "\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;

    }

    //Check that the solver converged to the correct value
    if( !( ( dTemp < 281.0 ) && ( dTemp > 280.0 ) ) )
    {
        strcpy_s(outputline, sizeof(outputline), "GetH_PS_USCS failed\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    printf( "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );
    fprintf( stream, "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );

    GetLocalTime( &SysTime );
    PreviousTickCount             = CurrentTickCount;
    CurrentTickCount              = GetTickCount();
    LapsedTicks                   = ( CurrentTickCount - PreviousTickCount );
    fprintf(stream, "%i:%02i:%02i:%003i\n", 
        (int)SysTime.wHour,
        (int)SysTime.wMinute,
        (int)SysTime.wSecond,
        (int)SysTime.wMilliseconds);
    fprintf(stream, "Number of milliseconds: %i\n", LapsedTicks ); 

    //header for the performance test
    fprintf(stream, "TimeStamp, Fluid Number, Number of milliseconds (14.7 psia), Number of Milliseconds (50.2 psia)\n" );

    //iterations for the performance test
    for( j=0; j<eos->GetFluidCount(); j++)
    {
        //Clear the array
        for( i=0; i<eos->GetFluidCount(); i++ )
            fluidarray[i]            = 0;

        //update index
        fluidarray[j]            = 1.00;


        //Clear the message handler
        eos->ClearMessages();

        //run the calculation and check the response
        dTemp = eos->GetH_PS_USCS( 14.7, ent );

        //check that the solver converged
        if( ( eos->GetMessageCount() > 0 ) && ( dTemp == 0 ) )
        {

            printf( eos->GetMessageMain( 1 ) );
            fprintf( stream, eos->GetMessageMain( 1 ) );
            strcpy_s(outputline, sizeof(outputline), "\n" );
            printf( outputline );
            fprintf(stream, outputline );
            eos.reset();
            fclose( stream );
            return;

        }

        for( i=0; i<3000; i++)
        {
            //run the calculation and check the response
            dTemp = eos->GetH_PS_USCS( press, ent );
        }

        GetLocalTime( &SysTime );
        PreviousTickCount             = CurrentTickCount;
        CurrentTickCount              = GetTickCount();
        LapsedTicks                   = ( CurrentTickCount - PreviousTickCount );
        fprintf(stream, "%i:%02i:%02i:%003i,", 
            (int)SysTime.wHour,
            (int)SysTime.wMinute,
            (int)SysTime.wSecond,
            (int)SysTime.wMilliseconds);
        printf("Fluid Number: %i\n", (j +1 ) );
        fprintf(stream, "%i,%i,", j,LapsedTicks ); 

        //run the calculation and check the response
        dTemp = eos->GetH_PS_USCS( 50.2, ent );

        for( i=0; i<3000; i++)
        {
            //run the calculation and check the response
            dTemp = eos->GetH_PS_USCS( press, ent );
        }

        GetLocalTime( &SysTime );
        PreviousTickCount             = CurrentTickCount;
        CurrentTickCount              = GetTickCount();
        LapsedTicks                   = ( CurrentTickCount - PreviousTickCount );
        fprintf(stream, "%i\n", LapsedTicks ); 

    }

    //outline the response
    strcpy_s(outputline, sizeof(outputline), "Temperature, Entropy, Enthalpy\n" );
    printf( outputline );
    fprintf(stream, outputline );
    strcpy_s(outputline, sizeof(outputline), "(R), (BTU/lb-R), (BTU/lb)\n" );
    printf( outputline );
    fprintf(stream, outputline );

    //setup the input with good data
    ent                      = 2.3254;
    temp                     = 219.67;
    press                    = 1.0;

    //Clear the message handler
    eos->ClearMessages();

    eos->SetMaxIterations( 200 );
    eos->SetPrecision ( 0.000001 );

    //run the calculation and check the response
    dTemp = eos->GetH_PS_USCS( press, ent );

    //Check that the solver converged to the correct value
    if( !( ( dTemp < 109.0 ) && ( dTemp > 108.0 ) ) )
    {
        strcpy_s(outputline, sizeof(outputline), "GetH_PS_USCS failed\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        fclose( stream );
        return;
    }

    printf( "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );
    fprintf( stream, "%f,%f,%f\n", 
        temp,
        ent, 
        dTemp );

    //////////////////////////////////////////////////////////////////////
    // output viscocity; checked against NIST website (443 microPoise)
    //////////////////////////////////////////////////////////////////////
    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture composition per example
    fluidarray[16] = 1.000;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }     

    //output the results
    fprintf( stream, "Beginning viscocity testing for ammonia\n" );
    fprintf( stream, "Parameter,Units,%s\n", eos->GetName( 17 ) );
    fprintf( stream, "Tc,K,%f\n", eos->GetTempCrit_SI( 17 ) );
    fprintf( stream, "Vc,cm3/mol,%f\n", eos->GetVolCrit_SI( 17 ) );
    fprintf( stream, "eccentricity,,%f\n", eos->GetEcc( 17 ) );
    fprintf( stream, "dipole,debye,%f\n", eos->GetDipole_SI( 17 ) );
    fprintf( stream, "M,g/mol,%f\n", eos->GetMolWeight( 17 ) );
    fprintf( stream, "Mixture,,%f\n", eos->GetPercentMW( 17 ) );

    temp = eos->GetViscosityGas_TP_SI( 520, 600 );
    if( ( temp > 480 ) || ( temp < 390 ) )
    {
        eos.reset();

        //Document destruction of object
        printf("Test failed at GetViscosityGas_TP_SI.\n" );
        fprintf(stream, "Test failed at GetViscosityGas_TP_SI.\n" );

        fclose( stream );

        return;
    }

    fprintf( stream, "Viscocity (microPoise) = ,%f\n", eos->GetViscosityGas_TP_SI( 520, 600 ) );

    //////////////////////////////////////////////////////////////////////
    // output viscocity; checked against NIST website (2.9746e-05 lbm/ft*s)
    //////////////////////////////////////////////////////////////////////
    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture composition per example
    fluidarray[16] = 1.000;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        return;
    }     

    //output the results
    fprintf( stream, "Beginning viscocity testing for ammonia\n" );
    fprintf( stream, "Parameter,Units,%s\n", eos->GetName( 17 ) );
    fprintf( stream, "Tc,R,%f\n", eos->GetTempCrit_USCS( 17 ) );
    fprintf( stream, "Vc,ft3/mol,%f\n", eos->GetVolCrit_USCS( 17 ) );
    fprintf( stream, "eccentricity,,%f\n", eos->GetEcc( 17 ) );
    fprintf( stream, "dipole,debye,%f\n", eos->GetDipole_SI( 17 ) );
    fprintf( stream, "M.W.,%f\n", eos->GetMolWeight( 17 ) );
    fprintf( stream, "Mixture,,%f\n", eos->GetPercentMW( 17 ) );

    temp = eos->GetViscosityGas_TP_USCS( 936, 8702 );
    if( ( temp > 0.00003 ) || ( temp < 0.00002 ) )
    {
        eos.reset();

        //Document destruction of object
        printf("Test failed at GetViscosityGas_TP_USCS.\n" );
        fprintf(stream, "Test failed at GetViscosityGas_TP_USCS.\n" );

        fclose( stream );

        return;
    }

    fprintf( stream, "Viscocity (microPoise) = ,%f\n", eos->GetViscosityGas_TP_SI( 520, 600 ) );

    //////////////////////////////////////////////////////////////////////
    // test mixtures
    //////////////////////////////////////////////////////////////////////
    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //output the results
    fprintf( stream, "Beginning mixture testing for methane-propane\n" );

    if( !check_V_PT_USCS( eos, stream, "MethanePropane_V_PT_USCS.tst", 0.02 ) )
    {
        fprintf(stream, "V_PT_USCS failed\n");
        eos.reset();
        return;
    }

    //////////////////////////////////////////////////////////////////////
    // Specific heat at constant pressure USCS
    //////////////////////////////////////////////////////////////////////
    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture for 100% propane
    fluidarray[2] = 1.000;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        return;
    }     

    //output the results
    fprintf( stream, "Beginning Cp USCS testing for propane\n" );
    fprintf( stream, "Parameter,Units,%s\n", eos->GetName( 2 ) );
    fprintf( stream, "Temperature,Rankine,1000\n");
    fprintf( stream, "Pressure,PSIA,15\n");

    dTemp = eos->GetCp_TP_USCS( 1000, 15 );
    fprintf( stream, "Cp (BTU/lbm-R) = ,%f\n", dTemp );

    //////////////////////////////////////////////////////////////////////
    // Specific heat at constant pressure SI
    //////////////////////////////////////////////////////////////////////
    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture for 100% propane
    fluidarray[2] = 1.000;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        return;
    }     

    //output the results
    fprintf( stream, "Beginning Cp SI testing for propane\n" );
    fprintf( stream, "Parameter,Units,%s\n", eos->GetName( 2 ) );
    fprintf( stream, "Temperature,Kelvin,500\n");
    fprintf( stream, "Pressure,bara,15\n");

    dTemp = eos->GetCp_TP_SI( 500, 15 );
    fprintf( stream, "Cp (kJ/kg-K) = ,%f\n", dTemp );

    //////////////////////////////////////////////////////////////////////
    // Specific heat at constant volume USCS
    //////////////////////////////////////////////////////////////////////
    //Initialize the gas composition vector
    for( i=0; i<eos->GetFluidCount(); i++ )
        fluidarray[i] = 0;

    //load mixture for 100% propane
    fluidarray[2] = 1.000;

    //Load the data into the eos object
    if( !eos->SetMixtureData( fluidarray ) )
    {
        strcpy_s(outputline, sizeof(outputline), "LoadMixture should have succeeded but failed.\n" );
        printf( outputline );
        fprintf(stream, outputline );
        eos.reset();
        return;
    }     

    //output the results
    fprintf( stream, "Beginning Cv USCS testing for propane\n" );
    fprintf( stream, "Parameter,Units,%s\n", eos->GetName( 2 ) );
    fprintf( stream, "Temperature,Rankine,1000\n");
    fprintf( stream, "Pressure,PSIA,15\n");

    dTemp = eos->GetCv_TP_USCS( 1000, 15 );
    fprintf( stream, "Cv (BTU/lbm-R) = ,%f\n", dTemp );

    /********************************************************************
    * End-of-program housekeeping functions                             *
    *********************************************************************/
    //Free all elements in the strand
    eos.reset();

    // Document destruction of object
    printf("File output to %s\n", str_filename.c_str());
    printf("Test Complete and CBWRS Destructed.\n" );
    fprintf(stream, "Test Complete and CBWRS Destructed.\n" );

    //close the output file
    fclose( stream );

    //All done
    return;
}
