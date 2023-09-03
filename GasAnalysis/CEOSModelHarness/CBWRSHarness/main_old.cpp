
/*--------------------------------------------*
/* This program serves as a test harness for  *
/* the equation of state (EOS) model		  *
/*--------------------------------------------*/
//
/*--------------------------------------------*
/* Written by:	Brian Howard				  *
/* Date:		11 Jan 2002					  *
/**********************************************/

#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "bwrs.h"
#include "filtercdf.h"

bool checkVaporPressure_SI( CBWRS *eos, FILE *stream, 
                           int FluidIndex, 
                           char *SourceFile,
                           double uncert)
{

     //local variables
     int i;
     int j;
     int Cols;
     double xarray[1024];
	double fluidarray[24];
     double T;
     double VapPress;
     double VapPressPub;
     double Vf;
     double VfPub;
     double Vg;
     double VgPub;
     double hf;
     double hfPub;
     class CFilterCDF filterCDF;
     
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
		xarray[i]                     = 0;
	}

     //Clear the array
	for( i=0; i<eos->GetFluidCount(); i++ )
		fluidarray[i]		= 0;

	//Set the fluid for methane
	fluidarray[ ( FluidIndex - 1 )]			= 1.00;

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
     Cols = filterCDF.GetCols( SourceFile );
     j = filterCDF.GetRows( SourceFile, (int)Cols );
     filterCDF.LoadData( SourceFile, xarray, (i*j) );

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


bool checkerror( CBWRS *eos, FILE *stream, char *location )
{

	//local variables
	int							i;
	char						outputline[2048];

	//initialize local variables
	i						= 0;
	strcpy(outputline, "");

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

bool displaymembers( CBWRS *eos, FILE *stream )
{

	//local variables
	char						outputline[2048];
	int							i;

	//initialize local variables
	strcpy(outputline, "");
	i							= 0;

	//Begin by outputting total number of fluids in the model
	printf( "Total Fluid Count: %i\n", eos->GetFluidCount() );
	fprintf( stream, "Total Fluid Count: %i\n", eos->GetFluidCount() );

	//Output column headings - line 1
	strcpy( outputline, ",,,Secondary,,,Crit. Temp.,Crit. Temp., Crit. Pres., Crit. Pres.,Crit. Vol.,Crit. Vol.,Omega\n" );
	printf( outputline );
	fprintf( stream, outputline );

	//Output column headings - line 2
	strcpy( outputline, "TPGL Index,Formula,Name,Name,CAS #,Mo. Wt.,(R),(K),(psia),(bar),(ft3/mol),(cm3/mol),-\n" );
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

bool displayBWRSParams( CBWRS *eos, FILE *stream )
{

	//local variables
	char						outputline[2048];
	int							i;

	//initialize local variables
	strcpy(outputline, "");
	i							= 0;

	//Output column headings - line 1
	strcpy( outputline, "Fluid Formula, Name, Bo,Ao,Co,Do,Eo,b,a,d,alpha,c,gamma\n" );
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

//main subroutine.  returns nothing to user
void main ()
{
	//local variables
	int                                i;
	int                                j;
	FILE                               *stream;
	class CBWRS                        *eos;
	char                               outputline[2048];
	double                             testvalue;
	double                             dtemp;
	double                             dtemp02;
     double                             temp;
     double                             press;
     double                             ent;
     double                             enth;
	double						xarray[1024];
	double						yarray[1024];
	double                             r;
	double                             fluidarray[24];
     double                             uncert;
     SYSTEMTIME                         SysTime;
	int                                CurrentTickCount;
     int                                PreviousTickCount;
     int                                LapsedTicks;

	//initialize local variables
	i                                  = 0;
	j                                  = 0;
	stream                             = NULL;
	eos                                = NULL;
	strcpy(outputline, "");
	testvalue				          = 3.14159;
	dtemp					     = 0;
	dtemp02                            = 0;
     temp                               = 0;
     press                              = 0;
     ent                                = 0;
     enth                               = 0;
	for(i=0;i<1024;i++)
	{
		xarray[i]                     = 0;
		yarray[i]                     = 0;
	}
	r                                  = 0;
     uncert                             = 0.05;
	CurrentTickCount                   = GetTickCount();
     PreviousTickCount                  = GetTickCount();
     LapsedTicks                        = ( CurrentTickCount - PreviousTickCount);

	/********************************************************************
	* The remainder of this program serves as a container and harness   *
	* to test the object members and methods.  Tests also               *
	* include exception handling                                        *
	*********************************************************************/

	/********************************************************************
	* Test Sequence                                                     *
	*	- Verify class instantiation                                   *
	*	- Verify member initialization                                 *
	*	- Verify method performance                                    *
	*********************************************************************/
	
	//Open the log file
	if( (stream = fopen( "CBWRSHarness.csv" , "w")) == NULL)
		return;

	//Intro line
	strcpy( outputline, "CEOSModel Test through CBWRS Test Harness.\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Assemble the strands
	eos	= new CBWRS();

	//Document instantiation of class
	if( eos == NULL )
		return;
	else
	{
		strcpy( outputline, "CBWRS Instantiated.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

	/********************************************************************
	* Documemt member values as found on class instatiation				*
	*********************************************************************/
	if( !displaymembers( eos, stream ) )
	{
		strcpy( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	strcpy( outputline, "\n" );
	printf( outputline );
	fprintf(stream, outputline );
	
     //////////////////////////////////////////////////////////////////////
	// Validate P_VS methods
     //////////////////////////////////////////////////////////////////////
     //Initialize the gas composition vector
     for( i=0; i<eos->GetFluidCount(); i++ )
          fluidarray[i] = 0;
     
     //load mixture composition for 100% nitrogen
     fluidarray[14] = 1.000;

     //Load the data into the eos object
     if( !eos->SetMixtureData( fluidarray ) )
     {
          strcpy( outputline, "LoadMixture should have succeeded but failed.\n" );
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
          delete eos;
          printf("GetP_VS_USCS failed.\n" );
          fprintf(stream, "GetP_VS_SI failed.\n" );
          return;
     }

     printf( "GetP_VS_USCS test succeeded.\n" );
 

     /********************************************************************
	* End-of-program housekeeping functions                             *
	*********************************************************************/
	//Free all elements in the strand
	delete eos;

	//Document destruction of object
	printf("Test Complete and CBWRS Destructed.\n" );
	fprintf(stream, "Test Complete and CBWRS Destructed.\n" );

	//close the output file
	fclose( stream );

	//All done
	return;
}
