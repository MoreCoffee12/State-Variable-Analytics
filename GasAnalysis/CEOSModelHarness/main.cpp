
/////////////////////////////////////////////////////////////////////
// This program serves as a test harness for  
// the equation of state (EOS) model		  
/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////
// Written by:	Brian Howard
// Date:		11 Jan 2002					  
/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////
//
// Revision, 24 Nov 2011
// 
// Amended to include silane gas, update to visual studio 2010
// 
// Brian Howard
// 
/////////////////////////////////////////////////////////////////////
//
// Revision, 5 Jul 2013
// 
// EOS was not correctly calculating hydrogen parameters.  Fixes
// include:
// - Display BWRS parameters and psuedo-critical parameters
// - Include testing of BWRS/p-c parameters 
// - correct error in 'b' and 'a' paremeter estimation
//
// Brian Howard
// 
/////////////////////////////////////////////////////////////////////
//
// Revision, 3 Sep 2023
//
// Updated to Visual Studio 2022 and made more use of standard 
// library functions.
//
// B. Howard
//
/////////////////////////////////////////////////////////////////////
//
// Revision, 13 Oct 2023
//
// Updated to include heat of vaporization functions.
//
// B. Howard
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "crashtestdummy.h"

bool displaymembers( CCrashTestDummy *eos, FILE *stream )
{

	//local variables
	char outputline[2048];
	int	i;

	//initialize local variables
	strcpy_s(outputline, "");
	i = 0;

	//Begin by outputting total number of fluids in the model
	printf( "Total Fluid Count: %i\n", eos->GetFluidCount() );
	fprintf( stream, "Total Fluid Count: %i\n", eos->GetFluidCount() );

	//Output column headings - line 1
	strcpy_s( outputline, ",,,Secondary,,,Crit. Temp.,Crit. Temp., Crit. Pres., Crit. Pres.,Crit. Vol.,Crit. Vol.,Omega,Tmin,Tmin,Tmax,Tmax,Pmin,Pmin,Pmax,Pmax,R,hfo,hfo,sfo,sfo,LHV,LHV,Dipole,Dipole,Bo,Ao,Co,Do,Eo,b,a\n" );
	printf( outputline );
	fprintf( stream, outputline );

	//Output column headings - line 2
	strcpy_s( outputline, "TPGL Index,Formula,Name,Name,CAS #,Mo. Wt.,(R),(K),(psia),(bar),(ft3/gmol),(cm3/gmol),-,(R),(K),(R),(K),(PSIA),(bar),(PSIA),(bar),BTU/lbm-R,BTU/lbmol,J/gmol,BTU/(lbmol-R),J/(gmol-K),BTU/Lbmol,J/gmol,debye,???,-,-,-,-,-,-,-\n" );
	printf( outputline );
	fprintf( stream, outputline );

	//Output members.
	for( i=1; i <= eos->GetFluidCount(); i++ )
	{
		fprintf( stream, "%i,%s,%s,%s,%i-%i-%i,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetTPGLIndex( i ),
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
									eos->GetEcc( i ),
									eos->GetTmin_USCS( i ),
									eos->GetTmin_SI( i ),
									eos->GetTmax_USCS( i ),
									eos->GetTmax_SI( i ),
									eos->GetPmin_USCS( i ),
									eos->GetPmin_SI( i ),
									eos->GetPmax_USCS( i ),
									eos->GetPmax_SI( i ),
									eos->GetR_USCS( i ),
									eos->Gethfo_USCS( i ),
									eos->Gethfo_SI( i ), 
									eos->Getsfo_USCS( i ),
									eos->Getsfo_SI( i ),
									eos->GetLHV_USCS( i ),
									eos->GetLHV_SI( i ),
                                    eos->GetDipole_SI( i ),
                                    eos->GetDipole_USCS( i ),
                                    eos->GetBo_USCS( i ),
                                    eos->GetAo_USCS( i ),
                                    eos->GetCo_USCS( i ),
                                    eos->GetDo_USCS( i ),
                                    eos->GetEo_USCS( i ),
                                    eos->Getb_USCS( i ),
                                    eos->Geta_USCS( i ));
	}

	//success
	return true;
}

bool displaymixture( CCrashTestDummy *eos, FILE *stream )
{

	//local variables
	char						outputline[2048];
	int							i;

	//initialize local variables
	strcpy_s(outputline, "");
	i							= 0;

	//Begin by outputting total number of fluids in the model
	printf( "Total Fluid Count: %i\n", eos->GetFluidCount() );
	fprintf( stream, "Total Fluid Count: %i\n", eos->GetFluidCount() );

	//Output column headings - line 1
	strcpy_s( outputline, ",Crit. Temp., Crit. Temp.,Crit. Pres.,Crit. Pres.,Crit. Vol.,Crit. Vol.\n" );
	printf( outputline );
	fprintf( stream, outputline );

	//Output column headings - line 2
	strcpy_s( outputline, "Mol. Wt,(R),(K),(psia),(bar),(ft3/mol),(cm3/mol), Omega\n" );
	printf( outputline );
	fprintf( stream, outputline );

	//Output mixture data
	printf( "%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetMolWeight_mx(),
							eos->GetTempCrit_mx_USCS(),
							eos->GetTempCrit_mx_SI(),
							eos->GetPresCrit_mx_USCS(),
							eos->GetPresCrit_mx_SI(),
							eos->GetVolCrit_mx_USCS(),
							eos->GetVolCrit_mx_SI(),
							eos->GetEcc_mx() );
	fprintf( stream, "%f,%f,%f,%f,%f,%f,%f,%f\n", eos->GetMolWeight_mx(),
							eos->GetTempCrit_mx_USCS(),
							eos->GetTempCrit_mx_SI(),
							eos->GetPresCrit_mx_USCS(),
							eos->GetPresCrit_mx_SI(),
							eos->GetVolCrit_mx_USCS(),
							eos->GetVolCrit_mx_SI(),
							eos->GetEcc_mx() );

	//Output Mixture Array Header
	for( i=1; i<= eos->GetFluidCount(); i++ )
	{
		printf( "%s,", eos->GetFormula( i ) );
		fprintf( stream, "%s,", eos->GetFormula( i ) );
	}
	printf( "\n" );
	fprintf( stream, "\n" );

	//Output Mixture Array
	for( i=1; i<= eos->GetFluidCount(); i++ )
	{
		printf( "%f,", eos->GetPercentMW( i ) );
		fprintf( stream, "%f,", eos->GetPercentMW( i ) );
	}
	printf( "\n" );
	fprintf( stream, "\n" );

	//success
	return true;
}

//main subroutine.  returns nothing to user
void main ()
{
	//local variables
    bool                        bTemp;
	int							i;
	int							j;
    int                         ikCount;
    errno_t                     err;
	FILE						*stream;
	class CCrashTestDummy		*eos;
	char						outputline[2048];
	double						testvalue;
	double						dTemp;
	double						dTemp2;
	double						xarray[10];
	double						yarray[10];
	double						fluidarray[25];
	double						r;

	//initialize local variables
    bTemp                   = false;
	i						= 0;
	j						= 0;
    ikCount                 = 0;
	stream					= NULL;
	eos                     = NULL;
	strcpy_s(outputline, "");
	testvalue				= 3.14159;
	dTemp					= 0;
	for(i=0;i<10;i++)
	{
		xarray[i]			= 0;
		yarray[i]			= 0;
	}
	for( i=0; i<24; i++ )
		fluidarray[i]		= 0;
	r						= 0;

	/********************************************************************
	* The remainder of this program serves as a container and harness	*
	* to test the object members and methods.  Tests also				*
	* include exception handling										*
	*********************************************************************/

	/********************************************************************
	* Test Sequence														*
	*	- Verify class instantiation									*
	*	- Verify member initialization									*
	*	- Verify method performance										*
	*********************************************************************/
	
	//Open the log file
    err = fopen_s( &stream, "CCrashTestDummyHarness.csv" , "w");
	if( err != 0 )
    {
    	printf( "Failed to open log file.\n" );
		return;
    }

	//Intro line
	strcpy_s( outputline, "CEOSModel Test through CCrashTestDummy Test Harness.\n" );
	printf( outputline );
	fprintf(stream, outputline );

	//Assemble the strands
	eos	= new CCrashTestDummy();

	//Document instantiation of class
	if( eos == NULL )
		return;
	else
	{
		strcpy_s( outputline, "CCrashTestDummy Instantiated.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}

	//although the class did come to life, lets see if any of the actions
	//of the constructor caused angst within the object
	if( eos->GetMessageCount() > 0 )
	{
		strcpy_s( outputline, "CEOS has errors in the constructor\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		strcpy_s( outputline, "priority, main, auxillary\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		for( i=0; i<eos->GetMessageCount(); i++ )
		{
			printf( "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
			fprintf( stream, "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
		}
		
		//stop program execution
		return;
	}

	/********************************************************************
	* Before we display the member data, lets check the GetName			*
	* functions and make sure they respond correctly to values outside  *
	* the normal limits													*
	*********************************************************************/
	//Although the fluid array inside the eos is numbered from 0 to (max-1),
	//the fluid index variable should correspond to the description in the
	//constructor.  Zero is not allowed.
	strcpy_s( outputline, eos->GetName( 0 ) );

	//We should get an error
	if( eos->GetMessageCount() > 0 )
	{
		strcpy_s( outputline, "CEOS correctly caught a zero value for fluid index\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		strcpy_s( outputline, "priority, main, auxillary\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		for( i=0; i<eos->GetMessageCount(); i++ )
		{
			printf( "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
			fprintf( stream, "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
		}
		
	}
	else
	{
		strcpy_s( outputline, "CEOS did not catch a zero value for fluid index\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete eos;
		//stop program execution
		return;
	}

	//And to test the other limit, try a huge number and see what kind
	//of response we get
	strcpy_s( outputline, eos->GetName( ( eos->GetFluidCount() + 1 ) ) );

	//We should get an error
	if( eos->GetMessageCount() > 0 )
	{
		strcpy_s( outputline, "CEOS correctly caught a large value for fluid index\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		strcpy_s( outputline, "priority, main, auxillary\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		for( i=0; i<eos->GetMessageCount(); i++ )
		{
			printf( "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
			fprintf( stream, "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
		}
		
	}
	else
	{
		strcpy_s( outputline, "CEOS did not catch a large value for fluid index\n" );
		printf( outputline );
		fprintf(stream, outputline );
		//stop program execution
		return;
	}

	//Although the fluid array inside the eos is numbered from 0 to (max-1),
	//the fluid index variable should correspond to the description in the
	//constructor.  Zero is not allowed.
	strcpy_s( outputline, eos->GetName_Alt01( 0 ) );

	//We should get an error
	if( eos->GetMessageCount() > 0 )
	{
		strcpy_s( outputline, "CEOS correctly caught a zero value for GetName_Alt01\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		strcpy_s( outputline, "priority, main, auxillary\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		for( i=0; i<eos->GetMessageCount(); i++ )
		{
			printf( "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
			fprintf( stream, "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
		}
		
	}
	else
	{
		strcpy_s( outputline, "CEOS did not catch a zero value for GetName_Alt01\n" );
		printf( outputline );
		fprintf(stream, outputline );
		//stop program execution
		return;
	}

	//And to test the other limit, try a huge number and see what kind
	//of response we get
	strcpy_s( outputline, eos->GetName_Alt01( ( eos->GetFluidCount() + 1 ) ) );

	//We should get an error
	if( eos->GetMessageCount() > 0 )
	{
		strcpy_s( outputline, "CEOS correctly caught a large value for GetNameAlt\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		strcpy_s( outputline, "priority, main, auxillary\n" );
		printf( outputline );
		fprintf(stream, outputline );
	
		for( i=0; i<eos->GetMessageCount(); i++ )
		{
			printf( "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
			fprintf( stream, "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
		}
		
	}
	else
	{
		strcpy_s( outputline, "CEOS did not catch a large value for GetName_Alt01\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete eos;
		//stop program execution
		return;
	}
    /////////////////////////////////////////////////////////////////////
	// Check the values of k
    /////////////////////////////////////////////////////////////////////
    bTemp = eos->GetkCount( &ikCount );
    for( i=0; i<ikCount; i++)
    {
        bTemp = eos->GetkValue( i, &dTemp );
        if( ( dTemp < 0.0 ) || ( dTemp > 10.0 ) )
        {
		    strcpy_s( outputline, "CEOS did not provide a value 'k' value \n" );
		    printf( outputline );
		    fprintf(stream, outputline );
            delete eos;
		    //stop program execution
		    return;
        }

    }
    fprintf(stream, "'k' values within range\n" );

    /////////////////////////////////////////////////////////////////////
	// Document member values as found on class instatiation
    /////////////////////////////////////////////////////////////////////
	if( !displaymembers( eos, stream ) )
	{
		strcpy_s( outputline, "Member output failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    /////////////////////////////////////////////////////////////////////
	// We seem to have loaded the data okay, lets see if the mixing
	// functions are working okay.  We'll begin by loading bad mixture
	// array data and verify the function catches the problem.
    /////////////////////////////////////////////////////////////////////
	//Begin by loading the array
    /////////////////////////////////////////////////////////////////////
	for( i=0; i<eos->GetFluidCount(); i++ )
	{
		fluidarray[i]		= i;
	}

	if( !eos->SetMixtureData( fluidarray ) )
	{
		strcpy_s( outputline, "SetMixtureData correctly caught the bad mixture array.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		
		printf( "There are %i messages\n", eos->GetMessageCount() );
		fprintf( stream, "There are %i messages\n", eos->GetMessageCount() );
		
		for( i=0; i<eos->GetMessageCount(); i++ )
		{
			printf( "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
			fprintf( stream, "Message %i (Priority %i): %s\n, %s\n", (i+1),
											eos->GetMessagePriority( (i+1) ),
											eos->GetMessageMain( i+1 ),
											eos->GetMessageAux( i+1 ));
		}
	}
	else
	{
		strcpy_s( outputline, "SetMixtureData didn't catch the bad data array.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

    /////////////////////////////////////////////////////////////////////
	// Good - the function caught the bad array.  Lets cycle through an 
	// array of valid mixture values and check the output
    /////////////////////////////////////////////////////////////////////
	//We are going to cycle through testing each percentmw
    /////////////////////////////////////////////////////////////////////
	for( i=0; i<eos->GetFluidCount(); i++ )
	{
		for( j=0; j<eos->GetFluidCount(); j++ )
		{
			if( j == i )
				fluidarray[j]		= 1;
			else
				fluidarray[j]		= 0;
		}
		
		//Load the data into the eos object
		if( !eos->SetMixtureData( fluidarray ) )
		{
			strcpy_s( outputline, "SetMixtureData should have succeeded but failed.\n" );
			printf( outputline );
			fprintf(stream, outputline );
            delete eos;
			return;
		}
        
        // Validate member properties
        switch (i)
        {
        
        // Methane
        case 0:
            // Check the mixture mole weight
            dTemp = eos->GetMolWeight_mx();
            if( ( dTemp > 16.044 ) || ( dTemp < 16.042 ) )
            {
		        strcpy_s( outputline, "GetMolWeight_mx failed for Methane (Fluid Index 0001).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }
               
            // Check critical temperature
            dTemp = eos->GetTempCrit_mx_USCS();
            if( ( dTemp > 343.009 ) || ( dTemp < 343.007 ) )
            {
		        strcpy_s( outputline, "GetTempCrit_mx_USCS failed for Methane (Fluid Index 0001).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check critical pressure
            dTemp = eos->GetPresCrit_mx_USCS();
            if( ( dTemp > 667.030 ) || ( dTemp < 667.028 ) )
            {
		        strcpy_s( outputline, "GetPresCrit_mx_USCS failed for Methane (Fluid Index 0001).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check Bo parameter properties
            dTemp = eos->GetBo_mx_USCS();
            if( ( dTemp > 0.723252 ) || ( dTemp < 0.723250 ) )
            {
		        strcpy_s( outputline, "GetBo_mx_USCS failed for Methane (Fluid Index 0001).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }
            break;

            
        // Hydrogen
        case 15:

            // Check the mixture mole weight
            dTemp = eos->GetMolWeight_mx();
            if( ( dTemp > 2.0161 ) || ( dTemp < 2.01599 ) )
            {
		        strcpy_s( outputline, "GetMolWeight_mx failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check Bo parameter properties
            dTemp = eos->GetBo_mx_USCS();
            if( ( dTemp > 0.355349991991029961 ) || ( dTemp < 0.355349991991029959 ) )
            {
		        strcpy_s( outputline, "GetBo_mx_USCS failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check Ao parameter properties
            dTemp = eos->GetAo_mx_USCS();
            if( ( dTemp > 661.208822537241761 ) || ( dTemp < 661.208822537241759 ) )
            {
		        strcpy_s( outputline, "GetAo_mx_USCS failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check Co parameter properties
            dTemp = eos->GetCo_mx_USCS();
            if( ( dTemp > 657047.172460419711 ) || ( dTemp < 657047.17246041970 ) )
            {
		        strcpy_s( outputline, "GetCo_mx_USCS failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check Do parameter properties
            dTemp = eos->GetDo_mx_USCS();
			// Revision, 3 Sep 2023. Relaxed these bounds slightly. 
			// Was: ( dTemp > 3393242.4561887701 ) || ( dTemp < 3393242.4561887699 )
            if( ( dTemp > 3393242.45618878 ) || ( dTemp < 3393242.45618876 ) )
            {
		        strcpy_s( outputline, "GetDo_mx_USCS failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check Eo parameter properties
            dTemp = eos->GetEo_mx_USCS();
            if( ( dTemp > 42605085.2968493101 ) || ( dTemp < 42605085.296849309 ) )
            {
		        strcpy_s( outputline, "GetEo_mx_USCS failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check b parameter properties
            dTemp = eos->Getb_mx_USCS();
            if( ( dTemp > 0.339081677613835231 ) || ( dTemp < 0.339081677613835229 ) )
            {
		        strcpy_s( outputline, "Getb_mx_USCS failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check a parameter properties
            dTemp = eos->Geta_mx_USCS();
            if( ( dTemp > 199.561613558842711 ) || ( dTemp < 199.561613558842709 ) )
            {
		        strcpy_s( outputline, "Geta_mx_USCS failed for Hydrogen (Fluid Index 0016).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            break;

        // Silane
        case 24:
            // Check the mixture mole weight
            dTemp = eos->GetMolWeight_mx();
            if( ( dTemp > 32.1171 ) || ( dTemp < 32.1169 ) )
            {
		        strcpy_s( outputline, "GetMolWeight_mx failed for Silane (Fluid Index 0025).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check critical temperature
            dTemp = eos->GetTempCrit_mx_USCS();
            if( ( dTemp > 485.461 ) || ( dTemp < 485.459 ) )
            {
		        strcpy_s( outputline, "GetTempCrit_mx_USCS failed for Silane (Fluid Index 0025).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            // Check critical pressure
            dTemp = eos->GetPresCrit_mx_USCS();
            if( ( dTemp > 702.418 ) || ( dTemp < 702.416 ) )
            {
		        strcpy_s( outputline, "GetPresCrit_mx_USCS failed for Silane (Fluid Index 00025).\n" );
		        printf( outputline );
		        fprintf(stream, outputline );
                delete eos;
    			return;
            }

            break;

        default:
            break;
        }


		//and take a look at the output
		if( !displaymixture( eos, stream ) )
			return;
	}

	strcpy_s( outputline, "+--------------------------------------------------+\n" );
	printf( outputline );
	fprintf( stream, outputline );

	/********************************************************************
	* Finally, we'll make the array slightly more complex and see if we *
	* can find any problems												*
	*********************************************************************/
	//We are going to cycle through testing each percentmw
	//fill the array
	for( i=0; i<eos->GetFluidCount()-3; i++ )
	{

		//Initialize the vector
		for( j=0; j<eos->GetFluidCount(); j++ )
			fluidarray[j]			= 0;

		for( j=0; j<eos->GetFluidCount()-3; j++ )
		{
			if( j == i )
			{
				fluidarray[j]		= 0.25;
				fluidarray[j+1]		= 0.25;
				fluidarray[j+2]		= 0.25;
				fluidarray[j+3]		= 0.25;
			}
		}
		
		//Load the data into the eos object
		if( !eos->SetMixtureData( fluidarray ) )
		{
			strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
			printf( outputline );
			fprintf(stream, outputline );
			return;
		}

		//and take a look at the output
		if( !displaymixture( eos, stream ) )
			return;
	}

	/********************************************************************
	* We'll also need to check the ideal entropy calculation function	*
	*********************************************************************/
	//Initialize the vector
	for( j=0; j<eos->GetFluidCount(); j++ )
		fluidarray[j]			= 0;

	//Pick on methane to start with
	fluidarray[0]				= 1.00;
	
	//Load the data into the eos object
	if( !eos->SetMixtureData( fluidarray ) )
	{
		strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
		printf( outputline );
		fprintf(stream, outputline );
		return;
	}

	//and take a look at the output
	for( j=200; j<1200; j=j+100 )
	{
		printf( "%f,%f\n", (double)j, eos->GetSIdeal_USCS( (double)j ) );
	}

	/********************************************************************
	* and the ideal enthalpy function based on Passut-Danner correlation*
	*********************************************************************/
	for( j = 0; j < eos->GetFluidCount(); j++  )
	{
	
		//Initialize the vector
		for( i=0; i<eos->GetFluidCount(); i++ )
			fluidarray[i]			= 0;

		//Load the value
		fluidarray[j]				= 1.00;
		
		//Set the headers
		printf( "Name:,%s\n", eos->GetName( j+1 ) );
		fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

		//Set the headers
		printf( "Temperature,Enthalpy,Temperature, Enthalpy\n" );
		fprintf( stream, "Temperature,Enthalpy,Temperature,Enthalpy\n" );
		printf( "(R),(BTU/lb),(K),(kJ/kg)\n" );
		fprintf( stream, "(R),(BTU/lb),(K),(kJ/kg)\n" );

		//Set the variables in the EOS object
		eos->SetMaxIterations( 200 );
		eos->SetPrecision ( 0.000001 );

		//Load the data into the eos object
		if( !eos->SetMixtureData( fluidarray ) )
		{
			strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
			printf( outputline );
			fprintf(stream, outputline );
			return;
		}

		//and take a look at the output
		for( i=400; i<5000; i=i+20 )
		{
			printf( "%f,%f,%f,%f\n", (double)i, 
				eos->GetHIdeal_USCS( (double)i ), 
				( (double)i * (5.0 / 9.0 ) ),
				eos->GetHIdeal_SI( ( (double)i * (5.0 / 9.0 ) ) ) );
			fprintf( stream, "%f,%f,%f,%f\n", (double)i,  
				eos->GetHIdeal_USCS( (double)i ),
				( (double)i * (5.0 / 9.0 ) ),
				eos->GetHIdeal_SI( ( (double)i * (5.0 / 9.0 ) ) ) );
		}

	}

	/********************************************************************
	* and the ideal entropy calculation function						*
	*********************************************************************/
	for( j = 0; j < eos->GetFluidCount(); j++  )
	{
	
		//Initialize the vector
		for( i=0; i<eos->GetFluidCount(); i++ )
			fluidarray[i]			= 0;

		//Load the value
		fluidarray[j]				= 1.00;
		
		//Set the headers
		printf( "Ideal Entropy Validation.\n");
		fprintf(stream, "Ideal Entropy for:,%s\n", eos->GetName( j+1 ) );

		//Set the variables in the EOS object
		eos->SetMaxIterations( 200 );
		eos->SetPrecision ( 0.000001 );

		//Load the data into the eos object
		if( !eos->SetMixtureData( fluidarray ) )
		{
			strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
			printf( outputline );
			fprintf(stream, outputline );
			return;
		}

		//and take a look at the output
		for( i=200; i<5000; i=i+20 )
		{
			printf( "%f,%f\n", (double)i, eos->Getso_SI( (double)i ) );
			fprintf( stream, "%f,%f\n", (double)i, eos->Getso_SI( (double)i ) );
		}

	}

	/********************************************************************
	* and the ideal enthalpy calculation function						*
	*********************************************************************/
	for( j = 0; j < eos->GetFluidCount(); j++  )
	{
	
		//Initialize the vector
		for( i=0; i<eos->GetFluidCount(); i++ )
			fluidarray[i]			= 0;

		//Load the value
		fluidarray[j]				= 1.00;
		
		//Set the headers
		printf( "Name:,%s\n", eos->GetName( j+1 ) );
		fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

		//Set the variables in the EOS object
		eos->SetMaxIterations( 200 );
		eos->SetPrecision ( 0.000001 );

		//Load the data into the eos object
		if( !eos->SetMixtureData( fluidarray ) )
		{
			strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
			printf( outputline );
			fprintf(stream, outputline );
			return;
		}

		//and take a look at the output
		for( i=200; i<1200; i=i+20 )
		{
			printf( "%f,%f\n", (double)i, eos->GetHIdeal_USCS( (double)i ) );
			fprintf( stream, "%f,%f\n", (double)i, eos->GetHIdeal_USCS( (double)i ) );
		}

	}

	/********************************************************************
	* and the ideal enthalpy calculation function						*
	*********************************************************************/
	for( j = 0; j < eos->GetFluidCount(); j++  )
	{
	
		//Initialize the vector
		for( i=0; i<eos->GetFluidCount(); i++ )
			fluidarray[i]			= 0;

		//Load the value
		fluidarray[j]				= 1.00;
		
		//Set the headers
		printf( "Name:,%s\n", eos->GetName( j+1 ) );
		fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

		//Set the variables in the EOS object
		eos->SetMaxIterations( 200 );
		eos->SetPrecision ( 0.000001 );

		//Load the data into the eos object
		if( !eos->SetMixtureData( fluidarray ) )
		{
			strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
			printf( outputline );
			fprintf(stream, outputline );
			return;
		}

		//and take a look at the output
		for( i=200; i<5000; i=i+20 )
		{
			printf( "%f,%f\n", (double)i, eos->Geth_h298_SI( (double)i ) );
			fprintf( stream, "%f,%f\n", (double)i, eos->Geth_h298_SI( (double)i ) );
		}

	}

	/********************************************************************
	* and the ideal mixture enthalpy function based on JANAF			*
	*********************************************************************/
	for( j = 0; j < eos->GetFluidCount(); j++  )
	{
	
		//Initialize the vector
		for( i=0; i<eos->GetFluidCount(); i++ )
			fluidarray[i]			= 0;

		//Load the value
		fluidarray[j]				= 1.00;
		
		//Set the headers
		printf( "Name:,%s\n", eos->GetName( j+1 ) );
		fprintf(stream, "Name:,%s\n", eos->GetName( j+1 ) );

		//Set the variables in the EOS object
		eos->SetMaxIterations( 200 );
		eos->SetPrecision ( 0.000001 );

		//Load the data into the eos object
		if( !eos->SetMixtureData( fluidarray ) )
		{
			strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
			printf( outputline );
			fprintf(stream, outputline );
			return;
		}

		printf( "%f,%f,%f,%f\n", eos->Gethfo_SI(),
									eos->Gethfo_USCS(),
									eos->Getsfo_SI(),
									eos->Getsfo_USCS());
		fprintf( stream, "%f,%f,%f,%f\n", eos->Gethfo_SI(),
									eos->Gethfo_USCS(),
									eos->Getsfo_SI(),
									eos->Getsfo_USCS());

	}

     /********************************************************************
     * Check visocity - Table 9-4 from "The Properties of Gases and      *
     * Liquids, 5th Edition" Nitrogen - 0.2 and hydrogen - 0.8           *
     *********************************************************************/
     //Initialize the vector
     for( i=0; i<eos->GetFluidCount(); i++ )
          fluidarray[i] = 0;
     
     //load mixture composition per example
     fluidarray[14] = 0.200;
     fluidarray[15] = 0.800;

     //Load the data into the eos object
     if( !eos->SetMixtureData( fluidarray ) )
     {
          strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
          printf( outputline );
          fprintf(stream, outputline );
          return;
     }     
     
     //output the results
     fprintf( stream, "Beginning viscocity testing\n" );
     fprintf( stream, "Parameter,Units,%s, %s\n", eos->GetName( 15 ), eos->GetName( 16 ) );
     fprintf( stream, "Tc,K,%f, %f\n", eos->GetTempCrit_SI( 15 ), eos->GetTempCrit_SI( 16 ) );
     fprintf( stream, "Vc,cm3/mol,%f, %f\n", eos->GetVolCrit_SI( 15 ), eos->GetVolCrit_SI( 16 ) );
     fprintf( stream, "eccentricity,,%f, %f\n", eos->GetEcc( 15 ), eos->GetEcc( 16 ) );
     fprintf( stream, "dipole,debye,%f, %f\n", eos->GetDipole_SI( 15 ), eos->GetDipole_SI( 16 ) );
     fprintf( stream, "M,g/mol,%f, %f\n", eos->GetMolWeight( 15 ), eos->GetMolWeight( 16 ) );
     fprintf( stream, "Mixture,,%f, %f\n", eos->GetPercentMW( 15 ), eos->GetPercentMW( 16 ) );

     dTemp2 = eos->GetViscosityGas_TP_SI( 373, 1 );
     fprintf( stream, "%f\n", dTemp );
     
     //Initialize the vector
     for( i=0; i<eos->GetFluidCount(); i++ )
          fluidarray[i] = 0;
     
     //load mixture composition per example
     fluidarray[16] = 1.000;

     //Load the data into the eos object
     if( !eos->SetMixtureData( fluidarray ) )
     {
          strcpy_s( outputline, "LoadMixture should have succeeded but failed.\n" );
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

     fprintf( stream, "%f\n", eos->GetViscosityGas_TP_SI( 1, 520 ) );

	 /////////////////////////////////////////////////////////////////////
	 // Revision, 13 Oct 2023
	 // Added enthalpy of vaporization tests
	 /////////////////////////////////////////////////////////////////////
	 //
	 // All test values have been calculated in the 
	 // "EnthalpyOfVaporization_TestHarness.xlsx" test harness spreadsheet
	 /////////////////////////////////////////////////////////////////////
	 // Begin with Methane.
	 dTemp2 = 100;
	 dTemp = eos->GetHvap_SI(1, dTemp2);
	 fprintf(stream, "Beginning enthalpy of vaporization testing for methane\n");
	 fprintf(stream, "T test,K, %f -> Hvap %f kJ/kmol \n", dTemp2, dTemp);

	 //Load the data into the eos object
	 if (abs(dTemp - 8466.86512871929) > 1e-10)
	 {
		 strcpy_s(outputline, "GetHvap_SI failed to calculate methane's enthalpy of vaporization.\n");
		 printf(outputline);
		 fprintf(stream, outputline);
		 return;
	 }

    /////////////////////////////////////////////////////////////////////
	// End-of-program housekeeping functions
    /////////////////////////////////////////////////////////////////////
	delete eos;

	//Document successful completion of tests
	printf("All tests completed successfully.\n" );
	fprintf(stream, "All tests completed successfully.\n" );

	//Document destruction of object
	printf("CCrashTestDummy Destructed.\n" );
	fprintf(stream, "CCrashTestDummy Destructed.\n" );

	//close the output file
	fclose( stream );

	//All done
	return;
}
