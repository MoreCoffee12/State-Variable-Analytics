// BWRS.cpp: implementation of the CBWRS class.
// This implementation inlcudes everything needed to 
// calculate mixture properties for compounds in
// Ken Starling's book "Light Petroleum Systems"
// Coefficients for Hydrogen from Applied Hydrocarbon Thermodynamics
// Accentricy, Critical Volume, Pressure and Temperature from Ried, etc
//
//////////////////////////////////////////////////////////////////////

#include "BWRS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBWRS::CBWRS()
{
	//Initialize object variables
}

CBWRS::~CBWRS()
{

	//Clean up
	//delete debugLog;


}


/******************************************
* Function Definition: GetSDepart_MT_USCS *
*-----------------------------------------***********
* Calculate the entropy departure for mixture       *
*****************************************************/
double CBWRS::GetSDepart_MT_USCS( double M, double T )
{
    
	//local variables
	double			Term1;

	double			Term2a;
	double			Term2b;
	double			Term2c;
	double			Term2d;
	double			Term2;

	double			Term3a;
	double			Term3b;
	double			Term3;

	double			Term4;

	double			Term5a;
	double			Term5b1;
	double			Term5b2;
	double			Term5b3;
	double			Term5;

	double			SDeparture;
    
	//initialize local variables
	Term1				= 0;

	Term2a				= 0;
	Term2b				= 0;
	Term2c				= 0;
	Term2d				= 0;
	Term2				= 0;

	Term3a				= 0;
	Term3b				= 0;
	Term3				= 0;

	Term4				= 0;

	Term5a				= 0;
	Term5b1				= 0;
	Term5b2				= 0;
	Term5b3				= 0;
	Term5				= 0;

	SDeparture			= 0;
    
	//Begin by checking to see that MolarDensity is valid
	if( M <=0 )
	{
		AddMessage( messagelist, 1,
						"Molar density invalid in GetSDepart_MT_USCS",
						"" );
		return -1;
	}

	//Calculate Term1
    Term1	= -1 * r_USCS * log( M * r_USCS * T );
    
    //Calculate Term2
    Term2a	= ( Mixture.Bo_mx * r_USCS );
    Term2b	= ( 2 * Mixture.Co_mx) / pow( T, 3 );
    Term2c	= (-3 * Mixture.Do_mx) / pow( T, 4 );
    Term2d	= ( 4 * Mixture.Eo_mx) / pow( T, 5 );
    Term2	= ( -1 * (Term2a + Term2b + Term2c + Term2d) * M );
    
    Term3a	= ( Mixture.b_mx * r_USCS );
	Term3b	= ( Mixture.d_mx / pow( T, 2 ) );
	Term3	= ( -0.5 ) * ( Term3a + Term3b ) * pow( M, 2 );
    
    Term4	= (Mixture.alpha_mx * Mixture.d_mx * pow( M, 5 )) / 
				(5 * pow(T , 2));
    
    Term5a	= (2 * Mixture.c_mx) / (Mixture.gamma_mx * pow(T,3));
    Term5b1 = 1.00;
    Term5b2 = -1 * (1 + ((Mixture.gamma_mx / 2) * pow( M, 2 )));
    Term5b3 = exp(-1 * Mixture.gamma_mx * pow( M, 2 ));
    Term5	= Term5a * (Term5b1 + (Term5b2 * Term5b3));
    
    SDeparture = Term1 + Term2 + Term3 + Term4 + Term5;

    //Convert Units
    SDeparture = (1 / Mixture.molewt_mx) * (144 / 778.26) * SDeparture;

	//return value
	return SDeparture;
}
  
/****************************************
* Method Definition: GetSDepart_TP_USCS *
*---------------------------------------*************
* Calculate the entropy departure for mixture       *
*****************************************************/
double CBWRS::GetSDepart_TP_USCS( double T, double P )
{
    
	//local variables
	double MolarDensity_USCS;
    
	//initialize local variables
	MolarDensity_USCS = 0;
    
	//Calculate the Molar Density
	//This is the default case, so we use the 
	//generic Molar density solver
	MolarDensity_USCS = GetMolarDensity_TP_USCS( T, P );
	
	//Begin by checking to see that MolarDensity is valid
	if( MolarDensity_USCS <=0 )
	{
		AddMessage( messagelist, 1,
						"Molar density invalid in GetSDepart_TP_USCS",
						"" );
		return -1;
	}

	//return value
	return GetSDepart_MT_USCS( MolarDensity_USCS, T );
}
  
 /********************
 * Method Definition *
 *-------------------*********************************
 * This function iteratavly solves the equation of   *
 * state for molar density using a linear position   *
 * position method.  For most problems, it will      *
 * converge faster, but for some cases will fail to  *
 * converga at all.  For those cases we have to use  *
 * trial and error.                                  *
 ****************************************************/
double CBWRS::GetFalsePositionMolarDensity( double T, double P, double initialMD )
{

	//local variables
	double                        TestValue;
	double                        FirstMolarDensity;
	double                        SecondMolarDensity;
	double                        ThirdMolarDensity;
	double                        FirstEstimatedPressure;
	double                        SecondEstimatedPressure;
	double                        ThirdEstimatedPressure;
	double                        slope;
	int						IterCount;
     double                        IterSum;
#ifdef _DEBUGFALSEPOS
     int                           i;
	FILE                          *stream;
#endif

	//Initialize local variables
	TestValue                     = 1.1;
	FirstMolarDensity             = 0;
	SecondMolarDensity            = 0;
	ThirdMolarDensity             = 0;
	FirstEstimatedPressure        = 0;
	SecondEstimatedPressure       = 0;
	ThirdEstimatedPressure        = 0;
	slope					= 0;
	IterCount                     = 1;
     IterSum                       = 0.0;
#ifdef _DEBUGFALSEPOS
     i                             = 0;
	stream					= NULL;

     //Open the log file
	if( (stream = fopen( "MDFalsePos.csv" , "a")) == NULL)
		return -1;

	//output header
     fprintf( stream, "Composition: \n" );
     for(i=0;i<GetFluidCount(); i++)
     {
          fprintf( stream, "%i, %f\n", i, GetPercentMW( ( i+1 ) ) );
     }
	fprintf( stream, "Pressure: %f\n", P );
	fprintf( stream, "Temperature: %f\n", T);
	fprintf( stream, "Interation, IterSum, FirstMolarDensity, FirstEstimatedPressure, MD2, P2, SLOPE, MD3, P3\n" );

#endif

	//Sanity check
	if(!ValidityCheck())
	{
		AddMessage( messagelist, 1,
						"GetFalsePositionMolarDensity Validity failed",
						"Check the temperature and pressure input parameters" );
		return -1;
	}

	//Try close to 0 to get to vapor phase
	FirstMolarDensity		= initialMD;
	FirstEstimatedPressure	= EOS( T, FirstMolarDensity );

	SecondMolarDensity		= ( initialMD + ( ( P * 0.1 ) / ( T * r_USCS ) ) );
	SecondEstimatedPressure  = EOS( T, SecondMolarDensity );

	do 
	{

	     //If the pressure is really small, we'll get an infinite result
	     //in the upcoming division and won't be able to converge on a 
	     //solution.
	     if( fabs( FirstMolarDensity - SecondMolarDensity ) < dblGetTrace() )
          {
		     return -1;
          }
		slope = (FirstEstimatedPressure - SecondEstimatedPressure) / (FirstMolarDensity - SecondMolarDensity);	
		
		//The curves the solver has to work on have very dramatic
		//slopes.  However, we should stop work if the slope gets too
		//large because the equation quickly goes to infinity.
		if( fabs( slope )  > ( 100000000000000.0 ) )
		{

#ifdef _DEBUGFALSEPOS
	
			//Close the log file
			fclose( stream );

#endif
			return -1;
		}
		
		//If the slope is close to zero, the resulting large molar
          //density would cause solver problems.
          if( fabs( slope ) < dblGetTrace() )
          {
               return -1;
          }
          ThirdMolarDensity = (P - SecondEstimatedPressure) / slope;
		ThirdMolarDensity = ThirdMolarDensity + SecondMolarDensity;
		
		ThirdEstimatedPressure = EOS( T, ThirdMolarDensity );

		TestValue = ThirdEstimatedPressure - P;

          //check that we haven't entered a loop where values are
          //simply swapped between the second and third densities
          if( fabs( ( SecondMolarDensity + ThirdMolarDensity ) - IterSum ) == 0.0 )
          {
               IterCount = ( MaxIterations + 1 );
          }
          else
          {
               IterSum = ( SecondMolarDensity + ThirdMolarDensity );
          }


#ifdef _DEBUGFALSEPOS
	
		//Open the log file
		fprintf( stream, "%i, %f, %f, %f, %f, %f, %f, %f, %f\n",
			IterCount,
			IterSum,
               FirstMolarDensity,
			FirstEstimatedPressure,
			SecondMolarDensity,
			SecondEstimatedPressure,
			slope,
			ThirdMolarDensity,
			ThirdEstimatedPressure);

#endif
		if ( fabs( FirstEstimatedPressure - P ) <
				fabs( SecondEstimatedPressure - P ) )
          {
			//First was better, so replace the second
			SecondMolarDensity			= ThirdMolarDensity;
			SecondEstimatedPressure		= ThirdEstimatedPressure;

		}
    
		else
		{
			//Second was better, so replace the first
			FirstMolarDensity			= ThirdMolarDensity;
			FirstEstimatedPressure		= ThirdEstimatedPressure;
		}

          IterCount++;

          if (IterCount>MaxIterations)
			TestValue=0;

	} 
	while ( fabs(TestValue) > Precision);

#ifdef _DEBUGFALSEPOS
	
	//Close the log file
	fclose( stream );

#endif

	//Check to see what threw use out of the loop
	//and what to do next
	if ( IterCount<MaxIterations )
		//We succeeded with false position
		return ThirdMolarDensity;
	else
		//Look to trial and error to solve	
		ThirdMolarDensity = GuessMolarDensity_TP_USCS( T, P );

	return ThirdMolarDensity;

}


//This subroutine grinds through the Starling
//modified BWR EOS
double CBWRS::EOS( double T, double MD )
{
	
	//local variables
	double Term2, Term2c, Term2d, Term2e;
	double Term3, Term3a, Term3c;
	double Term4, Term4a;
	double Term5, Term5a, Term5b, Term5c;
     double MDSq;
     double TSq;
     
	//initialize local variables
	Term2 = 0;
	Term2c = 0;
	Term2d = 0;
	Term2e = 0;

	Term3 = 0;
	Term3a = 0;
	Term3c = 0;

	Term4 = 0;
	Term4a = 0;

	Term5 = 0;
	Term5a = 0;
	Term5b = 0;
	Term5c = 0;

     MDSq = (MD * MD);
     TSq = (T * T);

	//Calculate the second term in the EOS
	Term2c    = ( ( Mixture.Co_mx / TSq ) * -1 );
	Term2d    = ( Mixture.Do_mx / ( TSq * T ) );
	Term2e    = ( ( Mixture.Eo_mx / ( TSq * TSq ) ) * -1 );
	Term2     = ( ( ( Mixture.Bo_mx * r_USCS * T ) - Mixture.Ao_mx + Term2c + Term2d + Term2e ) * MDSq );
    
	//Calculate the third term in the EOS
	Term3a    = ( Mixture.b_mx * r_USCS * T );
	Term3c    = ( ( Mixture.d_mx / T ) * -1 );
	Term3     = ( Term3a - Mixture.a_mx + Term3c ) * ( MDSq * MD );
    
	//Calculate the fourth term in the EOS
	Term4a    = ( Mixture.a_mx + ( Mixture.d_mx / T ) );
	Term4     = ( Mixture.alpha_mx * Term4a * pow( MD, 6 ) );
    
	//Calculate the fifth term in the EOS
	Term5a    = ( 1 + ( Mixture.gamma_mx * MDSq  ) );
	Term5b    = exp(-1 * Mixture.gamma_mx * MDSq );
	Term5c    = ( ( Mixture.c_mx *  MDSq * MD ) / TSq );
	Term5     = ( Term5a * Term5b * Term5c );

	//Calculate the estimated pressure
	return ( ( MD * r_USCS * T ) + Term2 + Term3 + Term4 + Term5 );
}

/*********************
 * Method Definition *
 *-------------------*********************************
 * This function calculations the fugacity           *
 * Ref. Starlings Book, Page 2                       *
 ****************************************************/
double CBWRS::EOSFugacity( double T, double MD )
{
	//Local Variables
	double Term1;
	double Term2, Term2a, Term2b, Term2c, Term2d, Term2e;
	double Term3, Term3a, Term3b, Term3c;
	double Term4, Term4a, Term4b;
	double Term5, Term5a, Term5b, Term5c;
     double MDSq;

	//Initialize local variables
	Term1		= 0;

	Term2a		= 0;
	Term2b		= 0;
	Term2c		= 0;
	Term2d		= 0;
	Term2e		= 0;
	Term2		= 0;
	
	Term3a		= 0;
	Term3b		= 0;
	Term3c		= 0;
	Term3		= 0;

	Term4a		= 0;
	Term4b		= 0;
	Term4		= 0;

	Term5a         = 0;
	Term5b         = 0;
	Term5c         = 0;
	Term5          = 0;

     MDSq           = (MD * MD);

     //Calculate the first term
	Term1		= ( r_USCS * T * log( MD * r_USCS * T ) );

	//Calculate the second term
	Term2a		= ( Mixture.Bo_mx * r_USCS * T );
	Term2b		= ( Mixture.Ao_mx * -1 );
	Term2c		= ( ( Mixture.Co_mx / ( T * T ) ) * -1 );
	Term2d		= ( Mixture.Do_mx / ( T * T * T ) );
	Term2e		= ( ( Mixture.Eo_mx / pow( T, 4 ) ) * -1 );
	Term2		= ( Term2a + Term2b + Term2c + Term2d + Term2e );
	Term2		= ( 2 * Term2 * MD );

	//And the third term
	Term3a		= ( Mixture.b_mx * r_USCS * T );
	Term3b		= ( -1 * Mixture.a_mx );
	Term3c		= ( -1 * ( Mixture.d_mx / T ) );
	Term3		= ( 1.5 * ( Term3a + Term3b + Term3c ) * MDSq );

	//And the fourth term
	Term4a		= Mixture.a_mx;
	Term4b		= ( Mixture.d_mx / T );
	Term4		= ( 6.0 / 5.0 ) * Mixture.alpha_mx * (Term4a + Term4b) * pow( MD, 5 );

	//And the fifth
	Term5a		= ( -0.5 * Mixture.gamma_mx * MDSq );
	Term5b		= (-1.0 * pow(Mixture.gamma_mx, 2) * pow( MD, 4 ) );
	Term5c		= (-1.0 * Mixture.gamma_mx * MDSq );
	Term5		= exp( Term5c );
	Term5		= ( ( 1 + Term5a + Term5b ) * Term5 );
	Term5		= ( 1 - Term5 );
	Term5		= ( Mixture.c_mx / ( Mixture.gamma_mx * T * T ) ) * Term5;

	//Return the fugacity
	return exp( ( Term1 + Term2 + Term3 + Term4 + Term5 ) / ( r_USCS * T ) );

}

/*****************************
 * Function Definition:Geta *
 *---------------------------*************************
 * This function returns the a value in the BWRS EOS *
 ****************************************************/
double CBWRS::Geta( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"Geta failed the ValidityCheckFluid call",
						"" );
		return -1;
	}

	return FluidList[( fluidindex - 1 )].a;

}

/********************************
 * Function Definition:GetAlpha *
 *------------------------------**********************
 * This function returns the Alpha value in the      *
 * BWRS EOS											 *
 ****************************************************/
double CBWRS::GetAlpha( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"GetAlpha failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].alpha;

}

/*****************************
 * Function Definition:GetAo *
 *---------------------------*************************
 * This function returns the Ao value in the BWRS EOS*
 ****************************************************/
double CBWRS::GetAo( int fluidindex )
{
	//check that the index is valid
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"GetAo failed the ValidityCheckFluid call",
						"" );
		return -1;
	}

	return FluidList[( fluidindex - 1 )].Ao;

}

/*****************************
 * Function Definition:Getb *
 *---------------------------*************************
 * This function returns the b value in the BWRS EOS *
 ****************************************************/
double CBWRS::Getb( int fluidindex )
{
	//check that the index is valid
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"Getb failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].b;

}

/*****************************
 * Function Definition:GetBo *
 *---------------------------*************************
 * This function returns the Bo value in the BWRS EOS*
 ****************************************************/
double CBWRS::GetBo( int fluidindex )
{
	//check that the index is valid
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"GetBo failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].Bo;

}

/****************************
 * Function Definition:Getc *
 *--------------------------**************************
 * This function returns the c value in the BWRS EOS *
 ****************************************************/
double CBWRS::Getc( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"Getc failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].c;

}

/*****************************
 * Function Definition:GetCo *
 *---------------------------*************************
 * This function returns the Co value in the BWRS EOS*
 ****************************************************/
double CBWRS::GetCo( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"GetCo failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].Co;

}

/****************************
 * Function Definition:Getd *
 *--------------------------**************************
 * This function returns the d value in the BWRS EOS *
 ****************************************************/
double CBWRS::Getd( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"Getd failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].d;

}

/*****************************
 * Function Definition:GetDo *
 *---------------------------*************************
 * This function returns the Do value in the BWRS EOS*
 ****************************************************/
double CBWRS::GetDo( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"GetDo failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].Do;

}

/*****************************
 * Function Definition:GetEo *
 *---------------------------*************************
 * This function returns the Eo value in the BWRS EOS*
 ****************************************************/
double CBWRS::GetEo( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"GetEo failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].Eo;

}

/********************************
 * Function Definition:GetGamma *
 *------------------------------**********************
 * This function returns the Gamma value in the      *
 * BWRS EOS											 *
 ****************************************************/
double CBWRS::GetGamma( int fluidindex )
{
	//check that the index is valid
	if( !ValidityCheckFluid( fluidindex ))
	{
		AddMessage( messagelist, 50,
						"GetGamma failed the ValidityCheckFluid call",
						"" );
		return -1;
	}


	return FluidList[( fluidindex - 1 )].gamma;

}

/******************************************
* Function Definition: GetHDepart_MT_USCS *
*-----------------------------------------***********
* Calculate the entropy departure for mixture given *
* the molar density and temperature					*
*****************************************************/
double CBWRS::GetHDepart_MT_USCS( double M, double T )
{
    
	//local variables
	double				Term1a;
	double				Term1b;
	double				Term1c;
	double				Term1d;
	double				Term1e;
	double				Term1;

	double				Term2a;
	double				Term2b;
	double				Term2c;
	double				Term2;

	double				Term3a;
	double				Term3b;
	double				Term3;

	double				Term4a;
	double				Term4b;
	double				Term4c;
	double				Term4d;
	double				Term4;

	double				HDeparture;
    
	//initialize local variables
	Term1a				= 0;
	Term1b				= 0;
	Term1c				= 0;
	Term1d				= 0;
	Term1e				= 0;
	Term1				= 0;

	Term2a				= 0;
	Term2b				= 0;
	Term2c				= 0;
	Term2				= 0;

	Term3a				= 0;
	Term3b				= 0;
	Term3				= 0;

	Term4a				= 0;
	Term4b				= 0;
	Term4c				= 0;
	Term4d				= 0;
	Term4				= 0;

	HDeparture			= 0;
    
	//Begin by checking to see that MolarDensity is valid
	if( M <=0 )
	{
		AddMessage( messagelist, 1,
						"Molar density invalid in GetHDepart_MT_USCS",
						"" );
		return -1;
	}

	//Calculate Term1
    Term1a		= ( Mixture.Bo_mx * r_USCS * T );
	Term1b		= ( -2 * Mixture.Ao_mx );
	Term1c		= ( ( -4 * Mixture.Co_mx ) / ( pow( T, 2 ) ) );
	Term1d		= ( ( 5 * Mixture.Do_mx ) / ( pow( T, 3 ) ) );
	Term1e		= ( ( -6 * Mixture.Eo_mx ) / ( pow( T, 4 ) ) );
    Term1		= ( ( Term1a + Term1b + Term1c + Term1d + Term1e ) * M );
    
    //Calculate Term2
    Term2a		= ( 2 * Mixture.b_mx * r_USCS * T );
    Term2b		= ( -3 * Mixture.a_mx );
    Term2c		= ( ( -4 * Mixture.d_mx) / T );
    Term2		= ( 0.5 * ( Term2a + Term2b + Term2c ) * pow( M, 2 ) );
    
    //Calculate Term3
    Term3a		= ( 6 * Mixture.a_mx );
	Term3b		= ( ( 7 * Mixture.d_mx ) / T );
	Term3		= ( ( 1.0 / 5.0 ) * Mixture.alpha_mx * ( Term3a + Term3b ) * pow( M, 5 ) );
    
	//Calculate Term4
	Term4a		= ( Mixture.c_mx / ( Mixture.gamma_mx * pow( T, 2 ) ) ); 
	Term4b		= ( 0.50 * ( Mixture.gamma_mx * pow( M, 2 ) ) );
	Term4c		= ( -1 * ( pow( Mixture.gamma_mx, 2 ) ) * ( pow( M, 4 ) ) );
	Term4d		= ( -1 * Mixture.gamma_mx * pow( M, 2 ) );
	Term4		= ( 3 + Term4b + Term4c );
	Term4		= ( Term4 * exp( Term4d ) );
	Term4		= ( Term4a * ( 3 - Term4 ) );

	//Sum the terms
    HDeparture = ( Term1 + Term2 + Term3 + Term4 );

	//convert from psia-ft3/lbmole to BTU/lb
	HDeparture	= ( HDeparture * (1 / Mixture.molewt_mx) );
	HDeparture	= ( HDeparture * units.convert( units.ft2, units.in2 ) );
	HDeparture	= ( HDeparture * units.convert( units.lbf_ft, units.BTU ) );

	//return value
	return HDeparture;
}
  
/******************************************
* Function Definition: GetHDepart_TP_USCS *
*-----------------------------------------***********
* Calculate the entropy departure for mixture		*
*****************************************************/
double CBWRS::GetHDepart_TP_USCS( double T, double P )
{
    
	//local variables
	double				HDeparture;
	double				MolarDensity_USCS;
    
	//initialize local variables
	HDeparture			= 0;
	MolarDensity_USCS	= 0;
    
	//Calculate the Molar Density
	//This is the default case, so we use the 
	//generic Molar density solver
	MolarDensity_USCS	= GetMolarDensity_TP_USCS( T, P );

	//Begin by checking to see that MolarDensity is valid
	if( MolarDensity_USCS <=0 )
	{
		AddMessage( messagelist, 1,
						"Molar density failed to converge in GetHDepart_TP_USCS",
						"" );
		return -1;
	}

	//Calculate the enthaply
	HDeparture			= GetHDepart_MT_USCS( MolarDensity_USCS, T );

	//return value
	return HDeparture;
}
  
/***********************
 * Function Definition *
 *---------------------******************************
 * This function calculations the fugacity          *
 * for the mixture.                                 *
 * Ref. Starlings Book, Page 2                      *
 ****************************************************/
double CBWRS::GetFugacity_TP_USCS( double T, double P )
{

	//local variables
	double			MolarDensity;

	//initialize local variables
	MolarDensity	= 0;

	//Calculate the Molar Density.  Inside the GetMolar
	//Density function is the algortihm for determining
	//which side of the vapor dome we fall on.
	MolarDensity	= GetMolarDensity_TP_USCS( T, P );
	
	//Begin by checking to see that MolarDensity is valid
	//We could have failed to converge all together.
	if(MolarDensity <=0)
	{
		AddMessage( messagelist, 1,
						"Molar Density solver failed to converge",
						"" );
		return -1;
	}

	return EOSFugacity( T, MolarDensity );
}

////////////////////////////////////////////////////////////////////////////////////////
//
// Method Definition : GetMolarDensity_TP_SI
//
// This function returns the molar density in SI
//
////////////////////////////////////////////////////////////////////////////////////////
double CBWRS::GetMolarDensity_TP_SI( double T, double P )
{
     //locals
     double    temp;

     //init locals
     temp      = 0.0;

     //call the solver
     temp = GetMolarDensity_TP_USCS( ( T * units.convert( units.K, units.R ) ), 
                                        ( P * units.convert( units.bar, units.PSIA ) ) );
     
     //check for failed convergence
     if( temp < 0 )
          return -1;
     
     //convert from USCS to SI
     temp = ( temp * units.convert( units.cm3, units.ft3 ) );
     temp = ( temp * units.convert( units.lb, units.g ) );

     return temp;
}


 /*********************************************
 * Method Definition: GetMolarDensity_TP_USCS *
 *--------------------------------------------********
 * This function calculates the molar density        *
 * for the mixture.	                                 *
 * This routine has to be able to detect when the    *
 * conditions fall under the vapor dome.  For a      *
 * single fluid, we don't know where the value is    *
 * without knowing the quality and we have to throw  *
 * back a message                                    *
 ****************************************************/
double CBWRS::GetMolarDensity_TP_USCS( double T, double P )
{

	//initialize molar density
	double MDf;
	double MDv;
	double diff;

	//Initialize local variables
	MDv = 0.01;
	MDf = 10.0;
	diff = 0;

	//Begin by finding the molar density coming from the liquid
	//side of the dome and from the vapor side of the dome.
	//If the values are the same, we are done.  If they are
	//different, the we need the quality as well to return
	//a solution.  Don't worry about the liquid molar density
     //if this is a mixture.
	MDv = GetFalsePositionMolarDensity( T, P, MDv );
	if( uniary )
     {
          MDf = GetFalsePositionMolarDensity( T, P, MDf );
     }
     else
     {    
          //ignore the liquid portion and check that the vapor
          //did converge.  If it didn't, try a larger guess
          MDf = -1;
          if( MDv < dblGetTrace() )
          {
               MDv = 10.0;
             	MDv = GetFalsePositionMolarDensity( T, P, MDv );
          }


     }
	//If neither solution converged, there is not point in
	//going any further
	if( ( MDv <= 0 ) && ( MDf <= 0 ) )
	{
		AddMessage( messagelist, 1,
					"Molar Density failed to converge",
					"" );
		return -1;
	}

	//Parse out the responses from the server.  If only one
	//solution converged, then return the solution that did
	//converge
	if( ( MDv > 0 ) && ( MDf <= 0 ) )
		return MDv;
	if( ( MDf > 0 ) && ( MDv <= 0 ) )
		return MDf;

	//Calculate the difference
	diff	= fabs( MDf - MDv );

	//Well, both solutions must have converged and we'd feel 
     //great if both approaches converged to approximately the same value.  
	//In that case they don't we need to see which value to return.  
	//If the values converged to unique values, we have to
	//decide where we are in relation to the vapor dome.
	//If: 
	//   - this is not a mixture
	//   - the pressure is less than the critical pressure 
	//   - the temperature less than the critical temperature 
	//   - the pressure is greater than the vapor pressure at temperature T 
	//We are to the left of the vapor dome and can return the liquid
	//values.  Otherwise, return the vapor values.
	if( diff > ( Precision * 10 ) )
	{
		if( ( T < Mixture.Tc_mx ) && ( P < Mixture.Pc_mx ) && ( uniary ) )
		{
			if( ( P > GetVaporPressure_T_USCS ( T ) ) )
				return MDf;
			else
				return MDv;
		}
	}

	//we converged to a solution
	return MDv;

}

 /***********************
 * Function Definition *
 *---------------------*******************************
 * This function solves the EOS using trial and      *
 * error                                             *
 ****************************************************/
 double CBWRS::GuessMolarDensity_TP_USCS( double T, double P )
{

	//local variables
	double                   TestValue;
	double                   MolarDensity;
	double                   EstimatedPressure;
	double                   StepValue;
	int                      IterCount;

	//initialize local variables
	TestValue                = 1.1;
	MolarDensity             = 0;
	EstimatedPressure        = 0;
	StepValue                = 0;	
	IterCount                = 1;

	//Define Step value
	StepValue	               = ( 0.1 * ( P / ( T * r_USCS ) ) );

	//Try 0 to get to vapor phase
	MolarDensity             = 0;


	do {

		//Calculate the pressure
		EstimatedPressure = EOS( T, MolarDensity );

		//Sanity check on the Estimated Pressure
		if(EstimatedPressure < 0)
			return -1;

		//Figure out how much difference
		TestValue = EstimatedPressure - P;
    
		if (TestValue < 0)
        
			MolarDensity = MolarDensity + StepValue;
    
		else
		{
			//We went to far, so go back and reduce the step
			MolarDensity   = ( MolarDensity - StepValue );
			StepValue      = ( StepValue / 2.00000 );
			MolarDensity   = ( MolarDensity + StepValue );
		}
               

		IterCount++;
		if ( IterCount>MaxIterations )
          {
			TestValue      = 0;
          }

	} 

	while ( fabs(TestValue) > Precision );

	//Success?
	if( IterCount > MaxIterations )
	{
		AddMessage( messagelist, 200,
						"GuessMolarDensity Failed to converge",
						"Check the temperature and pressure input parameters" );
		return -1;
	}

	return MolarDensity;

}

