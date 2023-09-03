// LEEKES.cpp: implementation of the CLEEKES class.
// This implementation inlcudes everything needed to 
// calculate mixture properties for compounds in using
// the Lee-Kesler correlation
//
//////////////////////////////////////////////////////////////////////

#include "LEEKES.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLEEKES::CLEEKES()
{

    //Load the variables for the simple EOS
	sb1		= 0.1181193;
	sb2		= 0.265728;
	sb3		= 0.154790;
	sb4		= 0.030323;

	sc1		= 0.0236744;
	sc2		= 0.0186984;
	sc3		= 0.000;
	sc4		= 0.042724;

	sd1		= 0.0000155488;
	sd2		= 0.0000623689;

	sbeta	= 0.653920000;
	sgamma	= 0.060167000;

	//Load the variables for the reference EOS
	rb1		= 0.2026579;
	rb2		= 0.331511;
	rb3		= 0.027655;
	rb4		= 0.203488;

	rc1		= 0.0313385;
	rc2		= 0.0503618;
	rc3		= 0.016901;
	rc4		= 0.041577;

	rd1		= 0.000048736;
	rd2		= 0.00000740336;

	rbeta	= 1.226000000;
	rgamma	= 0.037540000;
    romega = 0.3978;

	//Initialize other class variables
	sVr		= 0;
	sTr		= 0;
	sZ		= 0;
	rVr		= 0;
	rTr		= 0;
	rZ		= 0;
	Z		= 0;

}

CLEEKES::~CLEEKES()
{

	//Clean up
	//delete debugLog;


}

/////////////////////////////////////////////////////////////
//
// Method CalcLeeKesler
//
/////////////////////////////////////////////////////////////
// This method solves for either the simple or reference
// fluid.
/////////////////////////////////////////////////////////////
bool CLEEKES::CalcLeeKesler( bool Simple, double T, double P, double *Vr )
{
     //local variables
     double VaporVr;
     double LiquidVr;
     double Tr;
     double Pr;
     double Pvpr;

     //init locals
     VaporVr   = 0;
     LiquidVr  = 0;
     Pvpr      = 0;

     //volume and temperature
     Tr = ( T / Mixture.Tc_mx );
     Pr = ( P / Mixture.Pc_mx );

     //Solve for both vapor and liquid phases
     //If the liquid phase fails to converge, return the vapor
     //value.   If neither converges, return false;
     if ( SolveSpecificVolume( Tr, Pr, Simple, &VaporVr, 10.000 ) )
     {
          if( !SolveSpecificVolume( Tr, Pr, Simple, &LiquidVr, 0.0001 ) )
          {
               *Vr = VaporVr;
               return true;
          }
     }
     else
     {
          if( !SolveSpecificVolume( Tr, Pr, Simple, &LiquidVr, 0.0001 ) )
          {
               AddMessage( messagelist, 1,
                    "SolveSpecificVolume failed to converge for both vapor and liquid",
                    "Check the temperature and pressure input parameters" );
               return false;
          }
     }
     
     //if both functions converged to the same value,
     //then return the vapor value.  If they converged
     //to different solutiions, check the vapor pressure
     //to determine which solution to return
     if( fabs( LiquidVr - VaporVr) > ( Precision * 10 ) )
     {
          if( Simple )
          {
               CalcSimpleVaporPressure( Tr, &Pvpr );
          }
          else
          {    
               CalcReferenceVaporPressure( Tr, &Pvpr );
          }
          if( ( Pr < Pvpr ) )
          {
               *Vr = VaporVr;
          }
          else
          {
               *Vr = LiquidVr;
          }
     }
     else
     {
          *Vr = VaporVr;
     }
     return true;
}

/////////////////////////////////////////////////////////////
//
// Method CalcLeeKeslerBCD
//
/////////////////////////////////////////////////////////////
//
// This method calculates the B, C and D constants used in
// the right side of the equation
//
/////////////////////////////////////////////////////////////
bool CLEEKES::CalcLeeKeslerBCD( bool Simple, 
                                   double Tr, 
                                   double *B, 
                                   double *C, 
                                   double *D)
{
     if( Tr <= 0 )
     {
          AddMessage( messagelist, 1,
				"CalcLeeKeslerBCD trapped Tr <= 0",
				"" );
          return false;
     }
     
     if( Simple )
     {
         *B              = sb1-(sb2/Tr)-(sb3/( Tr * Tr ))-(sb4/( Tr * Tr * Tr));
	     *C              = sc1-(sc2/Tr)+(sc3/( Tr * Tr * Tr ) );
	     *D              = sd1+(sd2/Tr);
     }
     else
     {
         *B              = rb1-(rb2/Tr)-(rb3/( Tr * Tr ))-(rb4/( Tr * Tr * Tr));
	     *C              = rc1-(rc2/Tr)+(rc3/( Tr * Tr * Tr ) );
	     *D              = rd1+(rd2/Tr);
     }


     return true;
}

/////////////////////////////////////////////////////////////
//
// Method CalcLeeKeslerVaporPressure
//
/////////////////////////////////////////////////////////////
//
// This method calculates vapor pressure for the simple
// or reference fluid - Ref. The Properties of Gases & Liquids, 4th Ed
// Equation 7-2.6ff.  Also "Applied Hydrocarbon Thermodynamics",
// page 152
//
/////////////////////////////////////////////////////////////
bool CLEEKES::CalcLeeKeslerVaporPressure( double Tr, 
                                         double term1,
                                         double term2,
                                         double term3,
                                         double term4,
                                         double *Pvpr )
{
     //locals
     double temp;

     temp = ( term1 - ( term2 / Tr ) - ( term3 * log( Tr ) ) + ( term4 * pow( Tr, 6 ) ) );
     temp = exp( temp );
     
     *Pvpr = temp;

     return true;
}

/////////////////////////////////////////////////////////////
//
// Method CalcDeviation
//
/////////////////////////////////////////////////////////////
// This function solves for the deviation function
// for compressibility.  Not the same as the reference
// fluid compressiblity
/////////////////////////////////////////////////////////////
bool CLEEKES::CalcDeviation( double T, double P, double *RefVr )
{
     double      SimpleVr;

     if ( CalcLeeKesler( false, T, P, RefVr ) )
     {     
          CalcLeeKesler( true, T, P, &SimpleVr );
          *RefVr = ( ( *RefVr - SimpleVr ) / romega );
          return true;
     }

     return false;
}

/////////////////////////////////////////////////////////////
//
// Method CalcReferenceVaporPressure
//
/////////////////////////////////////////////////////////////
//
// This method calculates vapor pressure for the reference
// fluid 
//
/////////////////////////////////////////////////////////////
bool CLEEKES::CalcReferenceVaporPressure( double Tr, double *Pvpr )
{

     double sPvpr;
     CalcLeeKeslerVaporPressure( Tr, 5.92714, 6.09648, 1.28862, 0.169347, &sPvpr );
     CalcLeeKeslerVaporPressure( Tr, 15.2518, 15.6875, 13.4721, 0.4375, Pvpr );
     *Pvpr = ( sPvpr + ( *Pvpr * 0.3978 ) ) ;

     return true;
}

/////////////////////////////////////////////////////////////
//
// Method CalcSimple
//
/////////////////////////////////////////////////////////////
// This function solves for the simple reference
// fluid.
/////////////////////////////////////////////////////////////
bool CLEEKES::CalcSimple( double T, double P, double *SimpleVr )
{

     if( CalcLeeKesler( true, T, P, SimpleVr ) )
     {
          return true;
     }

     return false;
}

/////////////////////////////////////////////////////////////
//
// Method CalcSimpleVaporPressure
//
/////////////////////////////////////////////////////////////
//
// This method calculates vapor pressure for the simple
// fluid 
//
/////////////////////////////////////////////////////////////
bool CLEEKES::CalcSimpleVaporPressure( double Tr, double *Pvpr )
{

     CalcLeeKeslerVaporPressure( Tr, 5.92714, 6.09648, 1.28862, 0.169347, Pvpr );

     return true;
}

/******************************************
* Function Definition: GetSDepart_TP_USCS *
*-----------------------------------------***********
* Calculate the entropy departure for mixture		*
*****************************************************/
double CLEEKES::GetSDepart_TP_USCS( double T, double P )
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
	double			MolarDensity_USCS;
    
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
	MolarDensity_USCS	= 0;
    
	//Calculate the Molar Density
	//This is the default case, so we use the 
	//generic Molar density solver
	MolarDensity_USCS	= GetMolarDensity_TP_USCS( T, P );
	
	//Begin by checking to see that MolarDensity is valid
	if( MolarDensity_USCS <=0 )
		return -1;

	//Calculate Term1
    Term1	= -1 * r_USCS * log( MolarDensity_USCS * r_USCS * T );
    
    //Calculate Term2
    Term2a	= ( Mixture.Bo_mx * r_USCS );
    Term2b	= ( 2 * Mixture.Co_mx) / pow( T, 3 );
    Term2c	= (-3 * Mixture.Do_mx) / pow( T, 4 );
    Term2d	= ( 4 * Mixture.Eo_mx) / pow( T, 5 );
    Term2	= ( -1 * (Term2a + Term2b + Term2c + Term2d) * MolarDensity_USCS );
    
    Term3a	= ( Mixture.b_mx * r_USCS );
	Term3b	= ( Mixture.d_mx / pow( T, 2 ) );
	Term3	= ( -0.5 ) * ( Term3a + Term3b ) * pow( MolarDensity_USCS, 2 );
    
    Term4	= (Mixture.alpha_mx * Mixture.d_mx * pow(MolarDensity_USCS,5)) / 
				(5 * pow(T , 2));
    
    Term5a	= (2 * Mixture.c_mx) / (Mixture.gamma_mx * pow(T,3));
    Term5b1 = 1.00;
    Term5b2 = -1 * (1 + ((Mixture.gamma_mx / 2) * pow(MolarDensity_USCS,2)));
    Term5b3 = exp(-1 * Mixture.gamma_mx * pow(MolarDensity_USCS,2));
    Term5	= Term5a * (Term5b1 + (Term5b2 * Term5b3));
    
    SDeparture = Term1 + Term2 + Term3 + Term4 + Term5;

    //Convert Units
    SDeparture = (1 / Mixture.molewt_mx) * (144 / 778.26) * SDeparture;

	//Since the eos model uses ideal gas entropy at 1 atmoshpere;
	//and this one use 1 psia, we have to convert.
	SDeparture = ( SDeparture - ( Mixture.R_USCS_mx * log ( ( 1 / 14.696 ) ) ) );

	//return value
	return SDeparture;
}
  
/////////////////////////////////////////////////////////////////////
//
//GetFalsePositionMolarDensity
//
/////////////////////////////////////////////////////////////////////
//
// Solves the equation of state for molar density using a linear position
// position method.  For most problems, it will converge faster, but 
// for some cases will fail to converga at all.  For those cases we 
// have to use trial and error.
//
/////////////////////////////////////////////////////////////////////
double CLEEKES::GetFalsePositionMolarDensity( double T, double P, double initialMD )
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
	int                           IterCount;
	FILE                          *stream;

	//Initialize local variables
	TestValue                     = 1.1;
	FirstMolarDensity             = 0;
	SecondMolarDensity            = 0;
	ThirdMolarDensity             = 0;
	FirstEstimatedPressure        = 0;
	SecondEstimatedPressure       = 0;
	ThirdEstimatedPressure        = 0;
	slope                         = 0;
	IterCount                     = 1;
	stream                        = NULL;

#ifdef _DEBUGFALSEPOS
	
	//Open the log file
	if( (stream = fopen( "MDFalsePos.csv" , "w")) == NULL)
		return -1;

	//output header
	fprintf( stream, "Pressure: %f", P );
	fprintf( stream, "Temperature: %f", T);
	fprintf( stream, "Interation, MD1, P1, MD2, P2, SLOPE, MD3, P3\n" );

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

		slope					= (FirstEstimatedPressure - SecondEstimatedPressure) / (FirstMolarDensity - SecondMolarDensity);	
		
		//The curves the solver has to work on have very dramatic
		//slopes.  However, we should stop work if the slope gets too
		//large.
		if( slope > ( +1 * pow( 10.0, 14.0 ) ) )
		{

#ifdef _DEBUGFALSEPOS
	
			//Close the log file
			fclose( stream );

#endif
			return -1;
		}
		
		ThirdMolarDensity		= (P - SecondEstimatedPressure) / slope;
		ThirdMolarDensity		= ThirdMolarDensity + SecondMolarDensity;
		
		ThirdEstimatedPressure	= EOS( T, ThirdMolarDensity );

		TestValue				= ThirdEstimatedPressure - P;

#ifdef _DEBUGFALSEPOS
	
		//Open the log file
		fprintf( stream, "%i, %f, %f, %f, %f, %f, %f, %f\n",
			IterCount,
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

		IterCount = IterCount + 1;

		if (IterCount>MaxIterations)
			TestValue=0;

	} 
	while (fabs(TestValue) > Precision);

#ifdef _DEBUGFALSEPOS
	
	//Close the log file
	fclose( stream );

#endif

	//Check to see what threw use out of the loop
	//and what to do next
	if (IterCount<MaxIterations)
		//We succeeded with false position
		return ThirdMolarDensity;
	else
		//Look to trial and error to solve	
		ThirdMolarDensity = GuessMolarDensity_TP_USCS( T, P );

	return ThirdMolarDensity;

}


//This subroutine calculates the pressure
//given the temperature and molar density
//using the Lee-Kesler EOS
double CLEEKES::EOS( double T, double MD )
{
    double Vr0;
	double Vrr;
	double P1;
	double P2;
	double P3;
	double V;
	double V1;
	double V2;
	double V3;
	double Del1, Del2, Del3;
    double TestValue;
    double slope;
	bool bCheck;
    int IterCount;

    //check that temperature and molar density values are valid
    if ( T <= 0 )
    {
		AddMessage( messagelist, 1,
						"Temperature less than or equal to 0",
						"" );
		return -1;
	}

    if ( MD <= 0 )
    {
		AddMessage( messagelist, 1,
						"Molar density less than or equal to 0",
						"" );
		return -1;
	}

    //invert molar density to obtain molar specific volume
    V = ( 1 / MD );

	//begin by estimating pressure assuming an ideal gas
	P1 = ( r_USCS * T * MD);
	bCheck = CalcLeeKesler(true, T, P1, &Vr0);
	bCheck = CalcLeeKesler(false, T, P1, &Vrr);
    V1 = Vr0 + ( ( Mixture.omega_mx * ( Vrr - Vr0 ) ) / romega ) ;
    V1 = ( ( V1 * r_USCS * Mixture.Tc_mx ) / ( Mixture.Pc_mx ) );
    Del1 = ( V1 - V );

	P2 = ( P1 * 0.9 );
	bCheck = CalcLeeKesler(true, T, P2, &Vr0);
	bCheck = CalcLeeKesler(false, T, P2, &Vrr);
    V2 = Vr0 + ( ( Mixture.omega_mx * ( Vrr - Vr0 ) ) / romega ) ;
    V2 = ( ( V2 * r_USCS * Mixture.Tc_mx ) / ( Mixture.Pc_mx ) );
    Del2 = ( V2 - V );

	//begin iteration
	IterCount = 0;
    do
	{
		slope = (P1 - P2) / (V1 - V2);	
		
		P3 = ( slope * ( V - V1 ) ) + P1;

        //validate that the P3 is still positive
        if( P3 <= 0 )
        {
            if( P1 < P2 )
                P3 = ( 0.9 * P1 );
            else
                P3 = ( 0.9 * P2 );
        }
	    bCheck = CalcLeeKesler(true, T, P3, &Vr0);
	    bCheck = CalcLeeKesler(false, T, P3, &Vrr);
        V3 = Vr0 + ( ( Mixture.omega_mx * ( Vrr - Vr0 ) ) / romega ) ;
        V3 = ( ( V3 * r_USCS * Mixture.Tc_mx ) / ( Mixture.Pc_mx ) );
        Del3 = ( V3 - V );


        if (fabs(Del1) < fabs(Del2))
        {
		    //First was better, so replace the second
			P2 = P3;
			V2 = V3;
			Del2 = Del3;
		}
		else
		{
			//Second was better, so replace the first
			P1 = P3;
			V1 = V3;
			Del1 = Del3;
		}
		IterCount++;

		TestValue = Del3;

		if (IterCount>MaxIterations)
			TestValue=0;

	}
	while (fabs(TestValue) > Precision);

    return P3;	

}

double CLEEKES::EOSFugacity( double T, double MD )
{

     return -1;

}

/*****************************
 * Function Definition:Geta *
 *---------------------------*************************
 * This function returns the a value in the LEEKES EOS *
 ****************************************************/
double CLEEKES::Geta( int fluidindex )
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
 * LEEKES EOS											 *
 ****************************************************/
double CLEEKES::GetAlpha( int fluidindex )
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
 * This function returns the Ao value in the LEEKES EOS*
 ****************************************************/
double CLEEKES::GetAo( int fluidindex )
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
 * This function returns the b value in the LEEKES EOS *
 ****************************************************/
double CLEEKES::Getb( int fluidindex )
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
 * This function returns the Bo value in the LEEKES EOS*
 ****************************************************/
double CLEEKES::GetBo( int fluidindex )
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
 * This function returns the c value in the LEEKES EOS *
 ****************************************************/
double CLEEKES::Getc( int fluidindex )
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
 * This function returns the Co value in the LEEKES EOS*
 ****************************************************/
double CLEEKES::GetCo( int fluidindex )
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
 * This function returns the d value in the LEEKES EOS *
 ****************************************************/
double CLEEKES::Getd( int fluidindex )
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
 * This function returns the Do value in the LEEKES EOS*
 ****************************************************/
double CLEEKES::GetDo( int fluidindex )
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
 * This function returns the Eo value in the LEEKES EOS*
 ****************************************************/
double CLEEKES::GetEo( int fluidindex )
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
 * LEEKES EOS											 *
 ****************************************************/
double CLEEKES::GetGamma( int fluidindex )
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
* Function Definition: GetHDepart_TP_USCS *
*-----------------------------------------***********
* Calculate the entropy departure for mixture		*
*****************************************************/
double CLEEKES::GetHDepart_TP_USCS( double T, double P )
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
	double				MolarDensity_USCS;
    
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

	//Calculate Term1
    Term1a		= ( Mixture.Bo_mx * r_USCS * T );
	Term1b		= ( -2 * Mixture.Ao_mx );
	Term1c		= ( ( -4 * Mixture.Co_mx ) / ( pow( T, 2 ) ) );
	Term1d		= ( ( 5 * Mixture.Do_mx ) / ( pow( T, 3 ) ) );
	Term1e		= ( ( -6 * Mixture.Eo_mx ) / ( pow( T, 4 ) ) );
    Term1		= ( ( Term1a + Term1b + Term1c + Term1d + Term1e ) * MolarDensity_USCS );
    
    //Calculate Term2
    Term2a		= ( 2 * Mixture.b_mx * r_USCS * T );
    Term2b		= ( -3 * Mixture.a_mx );
    Term2c		= ( ( -4 * Mixture.d_mx) / T );
    Term2		= ( 0.5 * ( Term2a + Term2b + Term2c ) * pow(MolarDensity_USCS, 2 ) );
    
    //Calculate Term3
    Term3a		= ( 6 * Mixture.a_mx );
	Term3b		= ( ( 7 * Mixture.d_mx ) / T );
	Term3		= ( 0.2 * ( Term3a + Term3b ) * pow( MolarDensity_USCS, 5 ) );
    
	//Calculate Term4
	Term4a		= ( Mixture.c_mx / ( Mixture.gamma_mx * pow( T, 2 ) ) ); 
	Term4b		= ( 0.50 * ( Mixture.gamma_mx * pow( MolarDensity_USCS, 2 ) ) );
	Term4c		= ( -1 * ( pow( Mixture.gamma_mx, 2 ) ) * ( pow( MolarDensity_USCS, 4 ) ) );
	Term4d		= ( -1 * Mixture.gamma_mx * pow( MolarDensity_USCS, 2 ) );
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
  
///////////////////////////////////////////////////////////
// Method Definition     
///////////////////////////////////////////////////////////
//
// Calculates the fugacity for a mixture using Lee-Kesler
// Ref. Applied Hydrocarbon Thermodynamics
//
///////////////////////////////////////////////////////////
double CLEEKES::GetFugacity_TP_USCS( double T, double P )
{

	//Note:  The sFug and rFug
	//variable acutally store ln(f/p), but this made
	//a lousy variable name
	double	sFug;
	double	rFug;
	
	//As noted above, this variable actually stores the value
	//of ln(f/p)
	double	Fug;

	double    B;
     double    C;
     double    D;
     double	E;

	double	term1;
	double	term2;
	double	term3;
	double	term4;
	double	term5;
	double	term6;
	double	term7;

	//Initialize the local variables
	sFug		= 0;
	rFug		= 0;
	Fug			= 0;

	term1		= 0;
	term2		= 0;
	term3		= 0;
	term4		= 0;
	term5		= 0;
	term6		= 0;
	term7		= 0;

     B              = 0;
     C              = 0;
     D              = 0;
	E			= 0;

	//Calcuate the Tr's
	sTr			= ( T / Mixture.Tc_mx );
	rTr			= ( T / Mixture.Tc_mx );

	//Check the data prior to division
	if( Mixture.Vc_mx<=0 )
		return -1;
	if( Mixture.Tc_mx<=0 )
		return -1;

     //Calculate the simple variables
     CalcLeeKeslerBCD( true, Tr, &B, &C, &D );
	E			= exp(-1 * (sgamma / pow(sVr,2)));
	E			= (sbeta + 1 + (sgamma / pow(sVr,2))) * E;
	E			= sbeta + 1 - E;
	E			= (sc4 / (2 * pow(sTr,3) * sgamma)) * E;
	//Calculate the fugacity for the simple fluid
	term1		= sZ;
	term2		= -1;
	term3		= -1 * log(sZ);
	term4		= B / sVr;
	term5		= C / (2 * pow(sVr, 2));
	term6		= D / (5 * pow(sVr, 5));
	term7		= E;
	sFug		     = (term1 + term2 + term3 + term4 + term5 + term6 + term7);
	
     //Calculate the reference variables
     CalcLeeKeslerBCD( false, Tr, &B, &C, &D );
	E			= exp(-1 * (rgamma / pow(rVr,2)));
	E			= (rbeta + 1 + (rgamma / pow(rVr,2))) * E;
	E			= rbeta + 1 - E;
	E			= (rc4 / (2 * pow(rTr,3) * rgamma)) * E;
	//Calculate the fugacity for the reference fluid
	term1		= rZ;
	term2		= -1;
	term3		= -1 * log(rZ);
	term4		= B / rVr;
	term5		= C / (2 * pow(rVr, 2));
	term6		= D / (5 * pow(rVr, 5));
	term7		= E;
	rFug		= (term1 + term2 + term3 + term4 + term5 + term6 + term7);

	//Calculate the Fug term (as noted above, not acually fugacity...)
	Fug			= sFug + (Mixture.omega_mx/0.3978) * (rFug - sFug);
	
	//Actually, this returned value is the fugacity
	//return (exp(Fug) * Pressure);
	return (exp(Fug));
}

 /***********************************************
 * Method Definition: GetMolarDensity_TP_USCS   *
 *----------------------------------------------******
 * This function calculates the molar density        *
 * for the mixture.                                  *
 * This routine has to be able to detect when the    *
 * conditions fall under the vapor dome.  For a      *
 * single fluid, we don't know where the value is    *
 * without knowing the quality and we have to throw  *
 * back a message                                    *
 ****************************************************/
double CLEEKES::GetMolarDensity_TP_USCS( double T, double P )
{

	//initialize molar density
	double         MDf;
	double         MDv;
	double         diff;

	//Initialize local variables
	MDv            = 0.01;
	MDf            = 10.0;
	diff           = 0;

	//Begin by finding the molar density coming from the liquid
	//side of the dome and from the vapor side of the dome.
	//If the values are the same, we are done.  If they are
	//different, the we need the quality as well to return
	//a solution.
	MDv			= GetFalsePositionMolarDensity( T, P, MDv );
	MDf			= GetFalsePositionMolarDensity( T, P, MDf );

	//parse out the response check that both solutions
	//converged
	if( MDv <= 0 )
	{
		if ( MDf <= 0 )
		{
			AddMessage( messagelist, 1,
							"Molar Density failed to converge",
							"" );
			return -1;
		}
		else
			return MDf;
	}

	if( MDf <= 0 )
	{
		if ( MDv <= 0 )
		{
			AddMessage( messagelist, 1,
							"Molar Density failed to converge",
							"" );
			return -1;
		}
		else
			return MDv;
	}

	//Calculate the difference
	diff		= fabs( MDf - MDv );

	//Here, we'd feel great if both approaches converged
	//to approximately the same value.  The ultimate decision
	//on whether to use the value or not is left to the calling function.
	//A message is added to the list to inform the calling function of
	//the concern
	if( diff > ( Precision * 10 ) )
	{
		AddMessage( messagelist, 50,
						"Both liquid and vapor returned a solution.  Gas molar density is returned by default",
						"" );
	}

	//we converged to a solution
	return MDv;

}

 /***********************
 * Function Definition *
 *---------------------*******************************
 * This function solves the EOS using trial and 	 *
 * error											 *
 ****************************************************/
 double CLEEKES::GuessMolarDensity_TP_USCS( double T, double P )
{

	//local variables
	double				TestValue;
	double				MolarDensity;
	double				EstimatedPressure;
	double				StepValue;
	int					IterCount;

	//initialize local variables
	TestValue			= 1.1;
	MolarDensity		= 0;
	EstimatedPressure	= 0;
	StepValue			= 0;	
	IterCount			= 1;

	//Define Step value
	StepValue	= 0.1 * ( P / ( T * r_USCS ) );

	//Try 0 to get to vapor phase
	MolarDensity = 0;


	do {

		//Calculate the pressure
		EstimatedPressure = EOS( T, P );

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
			MolarDensity = MolarDensity - StepValue;
			StepValue = StepValue / 2.00000;
			MolarDensity = MolarDensity + StepValue;
		}
               

		IterCount = IterCount + 1;
		if (IterCount>MaxIterations)
			TestValue=0;

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

/////////////////////////////////////////////////////////
//
// Method RightSideSimple
//
/////////////////////////////////////////////////////////
//
// Calculates the right side of the Lee-Kessler relation
// Ref. Properties of Gases & Liquides, Chapter 3
//
/////////////////////////////////////////////////////////
double CLEEKES::RightSide( double *Vr, 
                          double B,
                          double C,
                          double D,
                          double beta,
                          double gamma,
                          double c4,
                          double *Tr )
{
	//local variables
	//These are temporary storage variables
	//to help divide and conquerer the math
	double	Term1;
	double	Term2;
	double	Term3;
	double	Term4, Term4a, Term4b, Term4c;

	if( *Vr != 0.0 )
     {
          Term1	= ( B / *Vr );

	     Term2	= ( C / (*Vr * *Vr) );

	     Term3	= ( D / pow( *Vr, 5) );

	     Term4a	= ( c4 / ( (*Tr) * (*Tr) * (*Tr) * (*Vr) * (*Vr) ) );
	     Term4b	= ( beta + ( gamma/( (*Vr) * (*Vr) ) ) );
	     Term4c	= ( exp( (-1*gamma)/( (*Vr) * (*Vr) ) ) );
	     Term4	= ( Term4a * Term4b * Term4c );
	
	     return ( 1 + Term1 + Term2 + Term3 + Term4 );
     }
     else
     {
          return -1.00;
     }
}

/////////////////////////////////////////////////////////
//
// Method Definition
//
/////////////////////////////////////////////////////////
//
// Solves for specific volume.  Reference Equation 3-7.1
// in Reid, etc. fourth edition, "The Properties of
// Gases and Liquids"
//
/////////////////////////////////////////////////////////
bool CLEEKES::SolveSpecificVolume( double Tr, double Pr, bool Simple, double *V, double Vinit )
{
	//locals     
    double	TestValue;
	double	FirstsVr;
	double	SecondsVr;
	double	ThirdsVr;
	double	FirstEstimatedZ;
	double	SecondEstimatedZ;
	double	ThirdEstimatedZ;
	double	FirstDelta;
	double	SecondDelta;
	double	ThirdDelta;
	double	StepValue;
	double	slope;
	int		IterCount;
	double	leftside;
     double    B;
     double    C;
     double    D;
     double    c4;
     double    beta;
     double    gamma;

	//init locals
     IterCount	= 1;
	TestValue	= 1.1;

	//Sanity check
	if(!ValidityCheck())
		return false;

     //Calculate the right side variables
     CalcLeeKeslerBCD( Simple, Tr, &B, &C, &D );
     if( Simple )
     {
          beta  = sbeta;
          gamma = sgamma;
          c4    = sc4;
     }
     else
     {
          beta  = rbeta;
          gamma = rgamma;
          c4    = rc4;
     }

	//Define Step value
	StepValue = 0.001;

	//Try 0 to get to vapor phase
	FirstsVr = Vinit;
	FirstEstimatedZ = RightSide( &FirstsVr, B, C, D, beta, gamma, c4, &Tr );
	leftside = (Pr * FirstsVr / Tr);
	FirstDelta = FirstEstimatedZ - leftside;

	SecondsVr = FirstsVr + StepValue;
	SecondEstimatedZ = RightSide( &SecondsVr, B, C, D, beta, gamma, c4, &Tr );
	leftside = (Pr * SecondsVr / Tr);
	SecondDelta = SecondEstimatedZ - leftside;

	do 
	{

		slope			= (FirstDelta - SecondDelta) / (FirstsVr - SecondsVr);	
		
		ThirdsVr            = ( ( -1 * FirstDelta) / slope) + FirstsVr;
		
		ThirdEstimatedZ     = RightSide( &ThirdsVr, B, C, D, beta, gamma, c4, &Tr );
		leftside		     = (Pr * ThirdsVr / Tr);
		ThirdDelta		= ThirdEstimatedZ - leftside;

		if (fabs(FirstDelta) <
				fabs(SecondDelta))
       {
			//First was better, so replace the second
			SecondsVr			= ThirdsVr;
			SecondEstimatedZ	= ThirdEstimatedZ;
			SecondDelta			= ThirdDelta;
		}
    
		else
		{
			//Second was better, so replace the first
			FirstsVr		= ThirdsVr;
			FirstEstimatedZ = ThirdEstimatedZ;
			FirstDelta		= ThirdDelta;
		}

		IterCount		= IterCount + 1;

		TestValue		= ThirdDelta;

		if (IterCount>MaxIterations)
			TestValue=0;

	} 
	while (fabs(TestValue) > Precision);

	//Check to see what threw use out of the loop
	//and what to do next
	if (IterCount<MaxIterations)
     {
		//We succeeded with false position
          *V = ThirdsVr;
		return true;
     }
	else
		//Look to trial and error to solve	
		//ThirdsVr = GuesssVr();
		return false;

	return false;

}