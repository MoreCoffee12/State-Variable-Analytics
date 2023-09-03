// CrankSlider.cpp: implementation of the CCrankSlider class.
//
//////////////////////////////////////////////////////////////////////

#include "CrankSlider.h"
#include "units.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrankSlider::CCrankSlider()
{
     
     //locals
     int                                i;

     //inits
     i                                  = 0;
     
     //Member initialization
     CrankAngleRadians                  = 0;
     CrankAngleDegrees                  = 0;
     ConnRodAngleRadians                = 0;
     ConnRodAngleDegrees                = 0;
     PistonAngleDegrees                 = 0;
     PistonAngleRadians                 = 0;

     Stroke_USCS                        = 2;
     r_USCS                             = ( Stroke_USCS / 2 );
     ConnRodLength_USCS                 = 5;
     PistonAndRodWeight_USCS            = 1.5;
     CrossheadWeight_USCS               = 1.5;
     dAmbientPressure_USCS = 9.999;
     SliderDisplacement_USCS            = 0;
     SliderVelocity_USCS                = 9.99;
     SliderAcceleration_USCS            = 9.99;
     SliderGasLoad_USCS                 = 9.99;
     SliderInertiaLoad_USCS             = 9.99;
     SliderCombinedLoad_USCS            = 9.99;
     CrossheadVerticalForce_USCS        = 9.99;
     MainBearingRadialForce_USCS        = 9.99;
     MainBearingTangentForce_USCS       = 9.99;
     MainBearingTorque_USCS             = 9.99;
     CrankRadiusToConnRodLength         = ( r_USCS / ConnRodLength_USCS );

     CCW                                = true;
     Right                              = true;
     RPM                                = 1;
     RadiansPerSecond                   = ( ( RPM / 30 ) * PI );
     RPM01                              = 1;
     RadiansPerSecond01                 = ( ( RPM01 / 30 ) * PI );
     for( i=0; i<MAXCHAMBERCOUNT; i++ )
     {
          PistonAreaCrankEnd_USCS[i]         = 0.00;
          PistonAreaHeadEnd_USCS[i]          = 0.00;
          PressureCrankEnd_USCS[i]           = 0.00;
          PressureHeadEnd_USCS[i]            = 0.00;
          dCylinderBoreHeadEndDiameter_USCS[i] = (double)i;
          dCylinderBoreCrankEndDiameter_USCS[i] = (double)i;
          dPistonRodHeadEndDiameter_USCS[i] = ( (double)i * 0.10 );   
          dPistonRodCrankEndDiameter_USCS[i] = ( (double)i * 0.10 );   
     }
     PistonAreaCrankEnd_USCS[0]         = ( 1.00 * PI * 0.25 );
     PistonAreaHeadEnd_USCS[0]          = ( 1.00 * PI * 0.25 );
     PressureCrankEnd_USCS[0]           = 12.71966;
     PressureHeadEnd_USCS[0]            = 12.71966;

   //Update the SI members as well.  Note that calling this method
   //will initialize the SI and RPS memebers
	ToSI();

}

CCrankSlider::~CCrankSlider()
{

}


//////////////////////////////////////////////////////////////////////
// Calculates connecting rod angle using whatever
// values exist in the object.
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::CalcConnRodAngle( )
{
	//Now we need to perform a check on the variables
	if( !SanityCheck() )
		return false;

	//Calculate the value...this works for CCW and Right orientation or
    //CW and Left orientation
    ConnRodAngleRadians	= asin( ( r_USCS/ConnRodLength_USCS ) * ( sin ( CrankAngleRadians ) ) );
    
    //Correct sign for bank angle and crankshaft rotation
    if( Right == false )
    {
        ConnRodAngleRadians = ( ConnRodAngleRadians * -1 );
    }
    if( CCW == false )
    {
        ConnRodAngleRadians = ( ConnRodAngleRadians * -1 );
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
// Calculates crosshead vertical force.
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::CalcCrossheadVerticalForce()
{
    //Update the combined rod load
    if( !CalcSliderCombinedLoad() )
    {
        return false;
    }
    
    //Get the c'rod angle. orientation and rotation
    //is accounted for in this calculation
    if( !CalcConnRodAngle() )
    {
        return false;
    }

    //make the calculation 
    CrossheadVerticalForce_USCS = ( SliderCombinedLoad_USCS * tan( ConnRodAngleRadians ) );

    return true;
}

//////////////////////////////////////////////////////////////////////
// Given a starting angle, an ending angle and the 
// time, in seconds of the starting angle, this method 
// calculates the ending time, in msec.
//////////////////////////////////////////////////////////////////////
double CCrankSlider::CalcDeltaTime( double StartingAngleRadians, double EndingAngleRadians, double StartingTime)
{
 
   //locals
   double                     DeltaRadians;

   //initialize
   DeltaRadians               = 0.0;

   //Validate variables
   if( !SanityCheck() )
      return -1;

   //Calculation
   DeltaRadians               = ( StartingTime + fabs( ( EndingAngleRadians - StartingAngleRadians ) ) );


   return ( DeltaRadians / RadiansPerSecond );

}

//////////////////////////////////////////////////////////////////////
// Calculates Slider Acceleration
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::CalcSliderAcceleration()
{

     //locals
     double Term01;
     double Term02;
     double SinAngle;
     double CosAngle;
    
     //init locals
     Term01 = 0.0;
     Term02 = 0.0;
     SinAngle = cos( ( CrankAngleRadians - HALFPI ) );
     CosAngle = cos( CrankAngleRadians );
     
	//Now we need to perform a check on the variables
	if( !SanityCheck() )
		return false;

     //Exact value for acceleration
     Term01                   = ( CrankRadiusToConnRodLength  * cos( 2 * CrankAngleRadians ) ) + ( CrankRadiusToConnRodLength * CrankRadiusToConnRodLength * CrankRadiusToConnRodLength * SinAngle * SinAngle * SinAngle * SinAngle );
     Term02                   = ( pow( ( 1 - ( CrankRadiusToConnRodLength  * CrankRadiusToConnRodLength * SinAngle * SinAngle ) ) ,1.500000000000 ) );
     SliderAcceleration_USCS  = ( -1 * r_USCS * RadiansPerSecond * RadiansPerSecond * ( CosAngle + ( Term01 / Term02 ) ) );

     return true;
}

//////////////////////////////////////////////////////////////////////
// Calculates Slider Acceleration while the crankshaft experiences
// angular acceleration.
//
// Revision, 27 August 2014
// Added method
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::CalcSliderAccelerationWithAngAcc()
{

    // locals
    double cos_theta3_rad;
    double term1;
	double term2;
	double term3;
	double term4;
	double term5;
	double term6;
	double t;
	double theta2dotactual;
    double SinAngle;
    double CosAngle;
	double theta2dotdot;
    bool bTemp;
    
    // init locals
    term1 = 0.0;
	term2 = 0.0;
	term3 = 0.0;
	term4 = 0.0;
	term5 = 0.0;
	term6 = 0.0;
	t = 0.0;
    SinAngle = cos( ( CrankAngleRadians - HALFPI ) );
    CosAngle = cos( CrankAngleRadians );
     
	// Now we need to perform a check on the variables
	if( !SanityCheck() )
		return false;
	
	// The average crankshaft angular acceleration
	theta2dotdot = (RadiansPerSecond01*RadiansPerSecond01 - RadiansPerSecond*RadiansPerSecond)/(4*PI);

    // Solve for the time it takes to reach the next angular position, using
    // degrees from when the Keyphasor triggered.  That means we need to include
    // the piston angle when calculating this time.
    bTemp = _bCalcTimeWithAngAcc(theta2dotdot, RadiansPerSecond, (CrankAngleRadians+PistonAngleRadians), RPM, RPM01, &t);
    if( !bTemp )
    {
        return false;
    }
 
    // Calculate the new crankshaft angular speed
    theta2dotactual = RadiansPerSecond + t*theta2dotdot;

     // Connecting rod angle.  Here the angles are relative to TDC so there is no piston angle 
	cos_theta3_rad = sqrt(1 - (CrankRadiusToConnRodLength  * CrankRadiusToConnRodLength)*pow(SinAngle,2));

    // Exact value for acceleration while crankshaft undergoes angular acceleration.  Here
    // the angles are relative to TDC so there is no piston angle. 
	term1 = -r_USCS * CosAngle*theta2dotactual*theta2dotactual;
	term2 = -((r_USCS * r_USCS * CosAngle * CosAngle)/(ConnRodLength_USCS * cos_theta3_rad))*theta2dotactual*theta2dotactual;
	term3 = - (( pow(r_USCS,4) * CosAngle * CosAngle * pow(SinAngle,2) ) / (pow(ConnRodLength_USCS,3) * pow(cos_theta3_rad,3)))*theta2dotactual*theta2dotactual;
	term4 = +((r_USCS * r_USCS * SinAngle * SinAngle)/(ConnRodLength_USCS * cos_theta3_rad))*theta2dotactual*theta2dotactual;
	term5 = -r_USCS * SinAngle * theta2dotdot;
	term6 = - (( r_USCS * r_USCS * CosAngle * SinAngle ) / (ConnRodLength_USCS * cos_theta3_rad)) * theta2dotdot;
	SliderAcceleration_USCS = term1 + term2 + term3 + term4 + term5 + term6;


    return true;

}


//////////////////////////////////////////////////////////////////////
// Calculates Slider combined (gas + inertia ) load
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::CalcSliderCombinedLoad()
{
     if( !CalcSliderInertiaLoad() )
     {
          return false;
     }

     if( !CalcSliderGasLoad() )
     {
          return false;
     }

     SliderCombinedLoad_USCS  = ( SliderInertiaLoad_USCS + SliderGasLoad_USCS );

     return true;
}

//////////////////////////////////////////////////////////////////////
// Calculates Slider combined (gas + inertia ) load allowing for
// angular acceleration of the crankshaft
//
// Revision, 28 Jul 2014
// Added this method.
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::CalcSliderCombinedLoadWithAngAcc()
{
     if( !CalcSliderInertiaLoadWithAngAcc() )
     {
          return false;
     }

     if( !CalcSliderGasLoad() )
     {
          return false;
     }

     SliderCombinedLoad_USCS  = ( SliderInertiaLoad_USCS + SliderGasLoad_USCS );

     return true;
}

/*********************
 * Method Definition *                                 
 *-------------------*********************************
 * Calculates position (SliderDisplacement) ofthe    *
 * for a given crank radius, connecting rod length   *
 * and crank angle position					   *
 ****************************************************/
bool CCrankSlider::CalcSliderDisplacement()
{
	//Local variables
	double				Term01;
	double				Term02;
	
	//Initialize local variables
	Term01				= 0;
	Term02				= 0;

	//Now we need to perform a check on the variables
	if( !SanityCheck() )
		return false;

     //Calculate first term
     Term01				= ( r_USCS * (1 - cos( CrankAngleRadians ) ) );
    
     //Calculate second term
     Term02				= ( ( r_USCS * r_USCS ) / pow( ConnRodLength_USCS, 2 ) ) * (pow(sin(CrankAngleRadians),2));
     Term02				= ( ConnRodLength_USCS * (1 - sqrt( 1 - Term02 ) ) );
    
     //Calculate the SliderDisplacement
     SliderDisplacement_USCS	= ( Term01 + Term02 );
 

	//success
	return true;
}
//////////////////////////////////////////////////////////////////////
// Calculates the displaced volume based on bore and
// piston rod diameter                              
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::CalcDisplacedVolume( double dBoreDiameter_USCS, double dRodDiameter_USCS )
{
    dDisplacedVolume_USCS = ( CalcPistonArea( dBoreDiameter_USCS, dRodDiameter_USCS) * GetStroke_USCS() );

    return true;

}

/***********************
 * Function Definition *
 *---------------------*******************************
 * Calculates the radial force acting on the crank   *
 * shaft main bearing using the object values        *
 ****************************************************/
bool CCrankSlider::CalcMainBearingRadialForce( )
{
     //update the rod load
     if( !CalcSliderCombinedLoad() )
          return false;

	//Calculate the connecting rod angle
	if( !CalcConnRodAngle( ) )
		return false;

	//Calculate the main bearing force
	MainBearingRadialForce_USCS	= ( SliderCombinedLoad_USCS / cos ( ConnRodAngleRadians ) );
	MainBearingRadialForce_USCS	= MainBearingRadialForce_USCS * sin( ( PI / 2 ) - ( CrankAngleRadians + ConnRodAngleRadians ) );

     return true;
}

/*********************
 * Method Definition *
 *-------------------*********************************
 * Calculates the tangent force acting on the crank  *
 * shaft main bearing using the values inside the    *
 * object                                            *
 ****************************************************/
bool CCrankSlider::CalcMainBearingTangentForce( )
{

     //update the rod load
     if( !CalcSliderCombinedLoad() )
          return false;

	//Calculate the connecting rod angle
	if( !CalcConnRodAngle() )
		return false;

	//Calculate the main bearing force
	MainBearingTangentForce_USCS  = ( SliderCombinedLoad_USCS / cos (ConnRodAngleRadians));
	MainBearingTangentForce_USCS	= MainBearingTangentForce_USCS * cos( ( PI / 2 ) - ( CrankAngleRadians + ConnRodAngleRadians ) );

     return true;

}

/*********************
 * Method Definition *
 *-------------------*********************************
 * Calculates the crank torque at the crank		   *
 * shaft main bearing given the crank radius (r),	   *
 * connecting rod length (l), crank angle degrees of *
 * rotation measured from top dead center		   *
 * (CrankDegrees) and the rod load force (Force)     *
 * using the object values                           *
 ****************************************************/
bool CCrankSlider::CalcMainBearingTorque( )
{

     //update the rod load
     if( !CalcSliderCombinedLoad() )
          return false;

	//Calculate the connecting rod angle
	if( !CalcConnRodAngle( ) )
		return false;

	//Calculate the torque at the main bearing
	MainBearingTorque_USCS		= ( SliderCombinedLoad_USCS / cos ( ConnRodAngleRadians ) );
	MainBearingTorque_USCS		= ( MainBearingTorque_USCS * cos( ( PI / 2 ) - ( CrankAngleRadians + ConnRodAngleRadians ) ) );
	MainBearingTorque_USCS		= ( units.convert( units.in, units.ft ) * r_USCS ) * ( MainBearingTorque_USCS );

     return true;

}

/*********************
 * Method Definition *
 *-------------------*********************************
 * Calculates the area for a given bore and rod dia  *
 ****************************************************/
double CCrankSlider::CalcPistonArea( double bore, 
									     double rod)
{
	return ( ( ( bore * bore ) - ( rod * rod ) ) * PI * 0.25 );
}

/*********************
 * Method Definition *
 *-------------------*************************************
 * Given a starting angle, this calculates the time when *
 * the semgent of interest starts                        *
 *********************************************************/
double CCrankSlider::CalcSegStartTime( double StartingAngleRadians, double StartingTime)
{
 
   //Validate variables
   if( !SanityCheck() )
      return -1;

   return ( StartingTime + ( StartingAngleRadians / RadiansPerSecond ) );

}

/*********************
 * Method Definition *
 *-------------------**********************************
 * No arguements, but lots of assumptions.  Assuming  *
 * that time equals zero and that the piston angle is *
 * equal to zero when the time equals zero, this      *
 * method calculates the number of seconds from       *
 * when the keyphasor fired to when this slider       *
 * reaches top dead center                            *
 ******************************************************/
double CCrankSlider::CalcTDCsec()
{
 
   return ( CalcDeltaTime( 0, PistonAngleRadians, 0 ) );

}

/*********************
 * Method Definition *
 *-------------------**********************************
 * This method calculates the gas load                *
 ******************************************************/
bool CCrankSlider::CalcSliderGasLoad()

{

     //locals
     int                      i;

     //inits
     i                        = 0;

     //clear the previous gas load values
     SliderGasLoad_USCS       = 0;

     //sum the parts
     for( i=0; i<MAXCHAMBERCOUNT; i++ )
     {
          SliderGasLoad_USCS  += ( ( PistonAreaCrankEnd_USCS[i] * ( PressureCrankEnd_USCS[i]  - dAmbientPressure_USCS ) ) - 
                                   ( PistonAreaHeadEnd_USCS[i]  * ( PressureHeadEnd_USCS[i] - dAmbientPressure_USCS ) ) );

     }
     
     return true;
}

////////////////////////////////////////////////////////
// This method calculates the interia load   
//
// Revision, 29 August 2014
// Added boolean check on the CalcSliderAcceleration()
// call.
////////////////////////////////////////////////////////
bool CCrankSlider::CalcSliderInertiaLoad()

{
     //Update the acceleration of the slider
     if( !CalcSliderAcceleration() )
     {
         return false;
     }

     //Good old Newton's mass times acceleration to get inertia force
     SliderInertiaLoad_USCS   = ( ( ( PistonAndRodWeight_USCS + CrossheadWeight_USCS ) / 32.2 ) * ( -1 * SliderAcceleration_USCS / 12.0 ) );

     return true;
}

////////////////////////////////////////////////////////
// This method calculates the interia load if the 
// crankshaft experiences angular acceleration
//
// Revision, 27 Aug 2014
// Added method
////////////////////////////////////////////////////////
bool CCrankSlider::CalcSliderInertiaLoadWithAngAcc()

{
     //Update the acceleration of the slider
     if ( !CalcSliderAccelerationWithAngAcc() )
     {
         return false;
     }

     //Good old Newton's mass times acceleration to get inertia force
     SliderInertiaLoad_USCS   = ( ( ( PistonAndRodWeight_USCS + CrossheadWeight_USCS ) / 32.2 ) * ( -1 * SliderAcceleration_USCS / 12.0 ) );

     return true;
}

/*********************
 * Method Definition *
 *-------------------**********************************
 * This method calculates the velocity of the         *
 * crosshead/piston using the member values           *
 ******************************************************/
bool CCrankSlider::CalcSliderVelocity()

{
	
     //locals
     double              Term01;
     double              Term02;

     //init locals
     Term01              = 0.0;
     Term02              = 0.0;
     
     //Bail out of the arguments are unreasonable
     if( !SanityCheck() )
          return false;

     //Exact velocity
     Term01              = ( CrankRadiusToConnRodLength * sin(2 * CrankAngleRadians) );
     Term02              = ( sin( CrankAngleRadians ) * sin( CrankAngleRadians ) );
     Term02              *= ( CrankRadiusToConnRodLength * CrankRadiusToConnRodLength );
     Term02              = ( 2 * sqrt(1 - Term02) );
    
     //Load the value into the object
	SliderVelocity_USCS = ( -1 *  r_USCS * RadiansPerSecond * ( ( sin( CrankAngleRadians ) )+ ( Term01 / Term02 ) ) );

	return true;

}
//////////////////////////////////////////////////////////////////////
// This method returns the value of the ambient pressure in 
// the International System of Units (SI).
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetAmbientPressure_SI( double * dAmbientPressure )
{
    ToSI();
    *dAmbientPressure = dAmbientPressure_SI;
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the value of the ambient pressure in 
// U.S. customary units.
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetAmbientPressure_USCS( double * dAmbientPressure )
{
    *dAmbientPressure = dAmbientPressure_USCS;
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the cylinder bore diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetCylinderBoreHeadEndDiameter_SI( int ChamberNo, double *dCylinderBoreDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dCylinderBoreDiameter_SI = ( dCylinderBoreHeadEndDiameter_USCS[ ( ChamberNo - 1) ] * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the cylinder bore diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetCylinderBoreCrankEndDiameter_SI( int ChamberNo, double *dCylinderBoreDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dCylinderBoreDiameter_SI = ( dCylinderBoreCrankEndDiameter_USCS[ ( ChamberNo - 1) ] * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the cylinder bore diameter in USCS for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetCylinderBoreHeadEndDiameter_USCS( int ChamberNo, double *dCylinderBoreDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dCylinderBoreDiameter_USCS = dCylinderBoreHeadEndDiameter_USCS[ ( ChamberNo - 1) ];
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the cylinder bore diameter in USCS for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetCylinderBoreCrankEndDiameter_USCS( int ChamberNo, double *dCylinderBoreDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dCylinderBoreDiameter_USCS = dCylinderBoreCrankEndDiameter_USCS[ ( ChamberNo - 1) ];
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the piston rod diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetPistonRodHeadEndDiameter_SI( int ChamberNo, double *dPistonRodDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dPistonRodDiameter_SI = ( dPistonRodHeadEndDiameter_USCS[ ( ChamberNo - 1) ] * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the piston rod diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetPistonRodCrankEndDiameter_SI( int ChamberNo, double *dPistonRodDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dPistonRodDiameter_SI = ( dPistonRodCrankEndDiameter_USCS[ ( ChamberNo - 1) ] * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the piston rod diameter in USCS for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetPistonRodHeadEndDiameter_USCS( int ChamberNo, double *dPistonRodDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo > MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dPistonRodDiameter_USCS = dPistonRodHeadEndDiameter_USCS[ ( ChamberNo - 1) ];
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method returns the piston rod diameter in USCS for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bGetPistonRodCrankEndDiameter_USCS( int ChamberNo, double *dPistonRodDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo > MAXCHAMBERCOUNT )
    {
        return false;
    }
    
    *dPistonRodDiameter_USCS = dPistonRodCrankEndDiameter_USCS[ ( ChamberNo - 1) ];
    return true;
}

//////////////////////////////////////////////////////////////////////
// Returns the value of the connecting rod angle.
//////////////////////////////////////////////////////////////////////
double CCrankSlider::GetConnRodAngleDegrees()
{
     
     //update the object
     CalcConnRodAngle();

     //Convert
     ConnRodAngleDegrees = (ConnRodAngleRadians * units.convert( units.radians, units.degrees ) );

     return ConnRodAngleDegrees;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the piston angle            *
 ****************************************************/
double CCrankSlider::GetConnRodAngleRadians()
{
    
     //update the object
     CalcConnRodAngle();

     return ConnRodAngleRadians;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of ConnRodLength in SI         *
 ****************************************************/
double CCrankSlider::GetConnRodLength_SI()
{

     //Update the SI objects
     ToSI();
     
     return ConnRodLength_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of ConnRodLength in SI         *
 ****************************************************/
double CCrankSlider::GetConnRodLength_USCS()
{

     return ConnRodLength_USCS;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the piston angle            *
 ****************************************************/
double CCrankSlider::GetCrankAngleDegrees()
{

     CrankAngleDegrees        = (CrankAngleRadians * ( units.convert( units.radians, units.degrees ) ) );
     
     return CrankAngleDegrees;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the piston angle            *
 ****************************************************/
double CCrankSlider::GetCrankAngleRadians()
{

     return CrankAngleRadians;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the vertical force one the  *
 * crosshead in SI units                            *
 ****************************************************/
double CCrankSlider::GetCrossheadVerticalForce_SI()
{
     
     //update
     CalcCrossheadVerticalForce();
     
     //Update the SI members
     ToSI();
     

     return CrossheadVerticalForce_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the vertical force one the  *
 * crosshead in USCS units                          *
 ****************************************************/
double CCrankSlider::GetCrossheadVerticalForce_USCS()
{

     //Update the value
     CalcCrossheadVerticalForce();
    
     return CrossheadVerticalForce_USCS;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the crosshead weight in SI               *
 ****************************************************/
double CCrankSlider::GetCrossheadWeight_SI()
{

   
     //Update SI members
     ToSI();

     return CrossheadWeight_SI;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the crosshead weight in USCS             *
 ****************************************************/
double CCrankSlider::GetCrossheadWeight_USCS()
{

   
     return CrossheadWeight_USCS;

}
///////////////////////////////////////////////////////////////////
// Returns the displaced volume in SI units 
///////////////////////////////////////////////////////////////////
double CCrankSlider::GetDisplacedVolume_SI(double dBoreDiameter_SI, double dRodDiameter_SI)
{
     
    //convert to USCS
    double dBoreDiameter_USCS = ( dBoreDiameter_SI * units.convert( units.cm2, units.in2 ) );
    double dRodDiameter_USCS = ( dRodDiameter_SI * units.convert( units.cm2, units.in2 ) );

    //update the volume
     CalcDisplacedVolume(dBoreDiameter_USCS, dRodDiameter_USCS );

     //update SI members
     ToSI();

     return dDisplacedVolume_SI;
}

///////////////////////////////////////////////////////////////////
//Returns the displaced volume in USCS units
///////////////////////////////////////////////////////////////////
double CCrankSlider::GetDisplacedVolume_USCS( double dBoreDiameter_USCS, double dRodDiameter_USCS )
{

     //update the volume
     CalcDisplacedVolume( dBoreDiameter_USCS, dRodDiameter_USCS );

     return dDisplacedVolume_USCS;
}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the slider gas load in SI                *
 ****************************************************/
double CCrankSlider::GetSliderGasLoad_SI()
{

   
     //Update object
     CalcSliderGasLoad();
     
     //Update SI members
     ToSI();

     return SliderGasLoad_SI;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the slider inertia load in USCS          *
 ****************************************************/
double CCrankSlider::GetSliderGasLoad_USCS()
{

     //Update object
     CalcSliderGasLoad();
   
     return SliderGasLoad_USCS;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the slider inertia load in SI            *
 ****************************************************/
double CCrankSlider::GetSliderInertiaLoad_SI()
{

   
     //Update object
     CalcSliderInertiaLoad();
     
     //Update SI members
     ToSI();

     return SliderInertiaLoad_SI;

}

//////////////////////////////////////////////////////
// Returns the slider inertia load in SI, allowing for
// crankshaft angular acceleration
//
// Revision, 29 August 2014
// Added this method
//////////////////////////////////////////////////////
double CCrankSlider::GetSliderInertiaLoadWithAngAcc_SI()
{

   
     //Update object
     CalcSliderInertiaLoadWithAngAcc();
     
     //Update SI members
     ToSI();

     return SliderInertiaLoad_SI;

}


/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the slider inertia load in USCS          *
 ****************************************************/
double CCrankSlider::GetSliderInertiaLoad_USCS()
{

     //Update object
     CalcSliderInertiaLoad();
   
     return SliderInertiaLoad_USCS;

}

/////////////////////////////////////////////////////
// Returns the slider inertia load in USCS with the
// crankshaft experiencing angular acceleration
//
// Revision, 28 August 2014
// Added this method
/////////////////////////////////////////////////////
double CCrankSlider::GetSliderInertiaLoadWithAngAcc_USCS()
{

     //Update object
     if( !CalcSliderInertiaLoadWithAngAcc() )
     {
         return -1;
     }
   
     return SliderInertiaLoad_USCS;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the main bearing radial force in SI      *
 ****************************************************/
double CCrankSlider::GetMainBearingRadialForce_SI()
{

   
     //Update the object
     CalcMainBearingRadialForce();

     //Update SI members
     ToSI();


     return MainBearingRadialForce_SI;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the main bearing radial force in USCS    *
 ****************************************************/
double CCrankSlider::GetMainBearingRadialForce_USCS()
{

   
     //Update the object
     CalcMainBearingRadialForce();

     return MainBearingRadialForce_USCS;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the main bearing tangent force in SI     *
 ****************************************************/
double CCrankSlider::GetMainBearingTangentForce_SI()
{

   
     //Update the object
     CalcMainBearingTangentForce();

     //Update SI members
     ToSI();


     return MainBearingTangentForce_SI;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the main bearing tangent force in USCS   *
 ****************************************************/
double CCrankSlider::GetMainBearingTangentForce_USCS()
{

   
     //Update the object
     CalcMainBearingTangentForce();

     return MainBearingTangentForce_USCS;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the main bearing torque in SI            *
 ****************************************************/
double CCrankSlider::GetMainBearingTorque_SI()
{

   
     //Update the object
     CalcMainBearingTorque();

     //Update SI members
     ToSI();


     return MainBearingTorque_SI;

}

/*********************
 * Method Definition *
 *-------------------********************************
 * Returns the main bearing torque in USCS          *
 ****************************************************/
double CCrankSlider::GetMainBearingTorque_USCS()
{

   
     //Update the object
     CalcMainBearingTorque();

     return MainBearingTorque_USCS;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of piston/rod weight in SI     *
 ****************************************************/
double CCrankSlider::GetPistonAndRodWeight_SI()
{

     //Update the SI objects
     ToSI();
     
     return PistonAndRodWeight_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of piston/rod weight in USCS   *
 ****************************************************/
double CCrankSlider::GetPistonAndRodWeight_USCS()
{

   return PistonAndRodWeight_USCS;

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the piston angle            *
 ****************************************************/
double CCrankSlider::GetPistonAngleDegrees()
{

     return PistonAngleDegrees;

}

//////////////////////////////////////////////////////
// Revision - 3 Sep 2014
// Added this method
// 
// Return the piston angle in radians
//////////////////////////////////////////////////////
double CCrankSlider::GetPistonAngleRadians()
{

     return PistonAngleRadians;

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the maximum number of piston areas       *
 * (chambers) allowed in the object                 *
 ****************************************************/
int CCrankSlider::GetPistonArea_MaxCount()
{

     return MAXCHAMBERCOUNT;

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the crank end piston area   *
 * for a particular chamber in SI units             *
 ****************************************************/
double CCrankSlider::GetPistonAreaCrankEnd_SI( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          //update the SI members
          ToSI();

          return PistonAreaCrankEnd_SI[ ( ChamberNo - 1) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the crank end piston area in*
 * SI units                                         *
 ****************************************************/
double CCrankSlider::GetPistonAreaCrankEnd_SI()
{
     
     //update the SI members
     ToSI();
     
     return PistonAreaCrankEnd_SI[0];

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the crank end piston area   *
 * for a particular chamber                         *
 ****************************************************/
double CCrankSlider::GetPistonAreaCrankEnd_USCS( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          return PistonAreaCrankEnd_USCS[ ( ChamberNo - 1 ) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the crank end piston area   *
 ****************************************************/
double CCrankSlider::GetPistonAreaCrankEnd_USCS()
{

     return PistonAreaCrankEnd_USCS[0];

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the head end piston area    *
 * for a particular chamber in SI units             *
 ****************************************************/
double CCrankSlider::GetPistonAreaHeadEnd_SI( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          return PistonAreaHeadEnd_SI[ ( ChamberNo - 1) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the head end piston area in *
 * SI units                                         *
 ****************************************************/
double CCrankSlider::GetPistonAreaHeadEnd_SI()
{

     return PistonAreaCrankEnd_SI[0];

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the head end piston area    *
 * for a particular chamber                         *
 ****************************************************/
double CCrankSlider::GetPistonAreaHeadEnd_USCS( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          return PistonAreaHeadEnd_USCS[ ( ChamberNo - 1 ) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the head end piston area    *
 ****************************************************/
double CCrankSlider::GetPistonAreaHeadEnd_USCS()
{

     return PistonAreaCrankEnd_USCS[0];

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the crank end pressure      *
 * for a particular chamber in SI units             *
 ****************************************************/
double CCrankSlider::GetPressureAbsCrankEnd_SI( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          //update the SI members
          ToSI();

          return PressureCrankEnd_SI[ ( ChamberNo - 1) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the crank end presssure in  *
 * SI units                                         *
 ****************************************************/
double CCrankSlider::GetPressureAbsCrankEnd_SI()
{
     
     //update the SI members
     ToSI();
     
     return PressureCrankEnd_SI[0];

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the crank end pressure      *
 * for a particular chamber in USCS                 *
 ****************************************************/
double CCrankSlider::GetPressureAbsCrankEnd_USCS( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          return PressureCrankEnd_USCS[ ( ChamberNo - 1 ) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the default chamber, crank  *
 * end puressure                                    *
 ****************************************************/
double CCrankSlider::GetPressureAbsCrankEnd_USCS()
{

     return PressureCrankEnd_USCS[0];

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the head end pressure       *
 * for a particular chamber in SI units             *
 ****************************************************/
double CCrankSlider::GetPressureAbsHeadEnd_SI( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          return PressureHeadEnd_SI[ ( ChamberNo - 1) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the head end pressure in    *
 * SI units.                                        *
 ****************************************************/
double CCrankSlider::GetPressureAbsHeadEnd_SI()
{

     return PressureCrankEnd_SI[0];

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the pressure in the user    *
 * selected end                                     *
 ****************************************************/
double CCrankSlider::GetPressureAbsHeadEnd_USCS( int ChamberNo )
{

     if( ChamberNo <= MAXCHAMBERCOUNT )
     {
          return PressureHeadEnd_USCS[ ( ChamberNo - 1 ) ];
     }
     else
     {
          return -1;
     }

}

/********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the pressure in the default *
 * head end area                                    *
 ****************************************************/
double CCrankSlider::GetPressureAbsHeadEnd_USCS()
{

     return PressureCrankEnd_USCS[0];

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of stroke radius in SI         *
 ****************************************************/
double CCrankSlider::Getr_SI()
{

     //Update the SI objects
     ToSI();
     
     return r_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of stroke radius in USCS       *
 ****************************************************/
double CCrankSlider::Getr_USCS()
{

   return r_USCS;

}
 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of Radians Per Second          *
 ****************************************************/
double CCrankSlider::GetRadiansPerSecond()
{

   return RadiansPerSecond;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of RPM                         *
 ****************************************************/
double CCrankSlider::GetRPM()
{

   return RPM;

}

/////////////////////////////////////////////////////
// Returns the value of RPM for the second revolution
//
// Revision, 27 Aug 2014
// Added method
/////////////////////////////////////////////////////
double CCrankSlider::GetRPM01()
{

   return RPM01;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider acceleration in  *
 * SI units                                         *
 ****************************************************/
double CCrankSlider::GetSliderAcceleration_SI()
{

     //Update the value
     CalcSliderAcceleration();

     //Update the SI members
     ToSI();
     
     return SliderAcceleration_SI;

}

/////////////////////////////////////////////////////
// Returns the value of the slider acceleration in  *
// SI units when the crankshaft experiences angular
// acceleration.
//
// Revision, 28 August 2014
// Added method.
/////////////////////////////////////////////////////
double CCrankSlider::GetSliderAccelerationWithAngAcc_SI()
{

     //Update the value
     CalcSliderAccelerationWithAngAcc();

     //Update the SI members
     ToSI();
     
     return SliderAcceleration_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider acceleration in  *
 * USCS units                                       *
 ****************************************************/
double CCrankSlider::GetSliderAcceleration_USCS()
{

     //Update the value
     CalcSliderAcceleration();
     
     return SliderAcceleration_USCS;

}

////////////////////////////////////////////////////
// Returns the value of the slider acceleration in 
// USCS units allowing for crankshaft angular
// acceleration
//
// Revision, 27 August 2014
// Added method
////////////////////////////////////////////////////
double CCrankSlider::GetSliderAccelerationWithAngAcc_USCS()
{

     //Update the value
     CalcSliderAccelerationWithAngAcc();
     
     return SliderAcceleration_USCS;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider gas and inertial *
 * loads in SI                                      *
 ****************************************************/
double CCrankSlider::GetSliderCombinedLoad_SI()
{
     
     //update
     CalcSliderCombinedLoad();
     
     //Update the SI members
     ToSI();
     

     return SliderCombinedLoad_SI;

}

/////////////////////////////////////////////////////
// Returns the value of the slider gas and inertial
// loads in SI allowing for angular acceleration of
// the crankshaft.
//
// Revision, 28 August 2014
// Added this method.
/////////////////////////////////////////////////////
double CCrankSlider::GetSliderCombinedLoadWithAngAcc_SI()
{
     
     // Update internal calculations
     CalcSliderCombinedLoadWithAngAcc();
     
     // Update the SI members
     ToSI();
     
     return SliderCombinedLoad_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider gas and inertial *
 * loads in USCS                                    *
 ****************************************************/
double CCrankSlider::GetSliderCombinedLoad_USCS()
{

     //Update the value
     CalcSliderCombinedLoad();
    
     return SliderCombinedLoad_USCS;

}

//////////////////////////////////////////////////////
// Returns the value of the slider gas and inertial
// loads in USCS allowing for angular acceleration
// of the crankshaft.
//
// Revision, 28 August 2014
// Added this method
//////////////////////////////////////////////////////
double CCrankSlider::GetSliderCombinedLoadWithAngAcc_USCS()
{

     // Update the value
     CalcSliderCombinedLoadWithAngAcc();
    
     return SliderCombinedLoad_USCS;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider displacement in  *
 * SI                                               *
 ****************************************************/
double CCrankSlider::GetSliderDisplacement_SI()
{

     //Update the value
     CalcSliderDisplacement();

     //Update SI members
     ToSI();
     
     return SliderDisplacement_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider displacement in  *
 * USCS                                             *
 ****************************************************/
double CCrankSlider::GetSliderDisplacement_USCS()
{

     //Update the value
     CalcSliderDisplacement();
     
     return SliderDisplacement_USCS;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider velocity in SI   *
 ****************************************************/
double CCrankSlider::GetSliderVelocity_SI()
{

     //Update the value
     CalcSliderVelocity();

     //Update the SI members
     ToSI();
     
     return SliderVelocity_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of the slider velocity in USCS *
 ****************************************************/
double CCrankSlider::GetSliderVelocity_USCS()
{

     //Update the value
     CalcSliderVelocity();
     
     return SliderVelocity_USCS;

}

//////////////////////////////////////////////////////////////////////
// Returns the maximum value of the crank-slider
//////////////////////////////////////////////////////////////////////
double CCrankSlider::GetSliderVelocityMax_USCS()
{

     //locals
     int i;
     double temp;
     double tangle;

     //init locals
     i = 0;
     temp = 0.0;
     tangle = 0.0;

     //retrieve the existing crankangle
     tangle = GetCrankAngleDegrees();

     //loop through the crank revolution
     for( i=0; i<360; i++)
     {
          SetCrankAngleDegrees( (double)i );

          //Update the value
          CalcSliderVelocity();

          if( GetSliderVelocity_USCS() > temp)
          {
               temp = GetSliderVelocity_USCS();
          }
     }
     
     //restore the crank angle
     SetCrankAngleDegrees( tangle );

     //return
     return temp;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of stroke in SI                *
 ****************************************************/
double CCrankSlider::GetStroke_SI()
{

     //Update the SI objects
     ToSI();
     
     return Stroke_SI;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Returns the value of stroke in USCS              *
 ****************************************************/
double CCrankSlider::GetStroke_USCS()
{

     return Stroke_USCS;

}

 /********************
 * Method Definition *
 *-------------------********************************
 * Checks the data to see if it makes sense		  *
 ****************************************************/
bool CCrankSlider::SanityCheck()
{
	//locals
     int                 i;

     //init locals
     i                   = 0;
     
     //We'll begin by checking the mechanical parameters
	//The radius should always be greater than zero
	if( r_USCS <= 0 )
		return false;
	if( r_SI <= 0 )
		return false;
	if( Stroke_USCS <= 0 )
		return false;
	if( Stroke_SI <= 0 )
		return false;
	//The connecting rod length should be greater than zero
	if( ConnRodLength_USCS <= 0 )
		return false;
	if( ConnRodLength_SI <= 0 )
		return false;
	//Although there is no mechanical reason the r/l can not
	//be greater than 1, it falls well outside the expected configuration
	//of common automotive and industrial reciprocating machinery 
	//characteristics
	if( ( CrankRadiusToConnRodLength ) > 1 )
		return false;

     //lets stay away from 0 or negative RPM
     if( RPM <= 0 )
          return false;
     if( RadiansPerSecond <= 0 )
          return false;
     if( RPM01 <= 0 )
          return false;
     if( RadiansPerSecond01 <= 0 )
          return false;
     //stay away from negative piston areas
     for( i=0; i<MAXCHAMBERCOUNT; i++ )
     {
          if( PistonAreaCrankEnd_USCS[i] < 0 )
               return false;
          if( PistonAreaHeadEnd_USCS[i] < 0 )
               return false;
          if( PressureCrankEnd_USCS[i] < 0 )
               return false;
          if( PressureHeadEnd_USCS[i] < 0 )
               return false;
     }
	//success
	return true;
}

//////////////////////////////////////////////////////////////////////
// loads the ambient pressure in U.S. customary units.
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetAmbientPressure_USCS( double input_pressure )
{
     
    //validate bounds
    if( input_pressure > MAXAMBIENTPRESSURE )
    {
        return false;
    }
    if( input_pressure < MINAMBIENTPRESSURE )
    {
        return false;
    }

    //into the object
    dAmbientPressure_USCS = input_pressure;
    
    //success
    return true;
}

/**********************
 * Description         *
 *---------------------******************************
 * loads the direction into the object              *
 ****************************************************/
bool CCrankSlider::SetCCW( bool userCCW )
{
     //into the object
     CCW                           = userCCW;
     
     return true;
}

/////////////////////////////////////////////////////////////////////
// loads the connecting rod length, in SI, into the object
//
// Revision, 28 August 2014
// Added code to validate the length of the connecting rod
/////////////////////////////////////////////////////////////////////
bool CCrankSlider::SetConnRodLength_SI( double input_l )
{
    // Validate the input
    if( input_l <=0 || input_l > 100000 )
    {
        return false;
    }

    // Convert to USCS, load the value into the object, and
    // update related internals.
    ConnRodLength_USCS = input_l * ( units.convert(units.cm, units.in) );
    CrankRadiusToConnRodLength = ( r_USCS / ConnRodLength_USCS );

    // Success
    return true;
}

/////////////////////////////////////////////////////////////////////
// loads the connecting rod length, in USCS, into the object
//
// Revision, 28 August 2014
// Added code to validate the length of the connecting rod
/////////////////////////////////////////////////////////////////////
bool CCrankSlider::SetConnRodLength_USCS( double input_l )
{
    // Validate the input
    if( input_l <=0 || input_l > 100000 )
    {
        return false;
    }

     // Load the value into the object, update related internals
     ConnRodLength_USCS            = input_l;
     CrankRadiusToConnRodLength    = ( r_USCS / ConnRodLength_USCS );

     // Success
     return true;
}

/////////////////////////////////////////////////////////////////////
// Loads the crank angle, in degrees, into the object
/////////////////////////////////////////////////////////////////////
bool CCrankSlider::SetCrankAngleDegrees( double input_CrankAngle )
{
     //into the object
     CrankAngleDegrees = input_CrankAngle;
     CrankAngleRadians = ( CrankAngleDegrees * ( units.convert( units.degrees, units.radians ) ) );
     
     return true;
}

/////////////////////////////////////////////////////////////////////
// Loads the crosshead weight in U.S. Customary Units (USCS)
/////////////////////////////////////////////////////////////////////
bool CCrankSlider::SetCrossheadWeight_USCS( double input_weight )
{
     //into the object
     CrossheadWeight_USCS = input_weight;

     return true;
}

/////////////////////////////////////////////////////////////////////
// loads the piston and rod assembly weight
/////////////////////////////////////////////////////////////////////
bool CCrankSlider::SetPistonAndRodWeight_USCS( double input_weight )
{
     //into the object
     PistonAndRodWeight_USCS = input_weight;

     return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the cylinder bore diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetCylinderBoreHeadEndDiameter_SI( int ChamberNo, double dCylinderBoreDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dCylinderBoreDiameter_SI < 0 ) 
    {
        return false;
    }
    
    dCylinderBoreHeadEndDiameter_USCS[ ( ChamberNo - 1) ] = ( dCylinderBoreDiameter_SI * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the cylinder bore diameter in USCS for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetCylinderBoreHeadEndDiameter_USCS( int ChamberNo, double dCylinderBoreDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dCylinderBoreDiameter_USCS < 0 ) 
    {
        return false;
    }
    
    dCylinderBoreHeadEndDiameter_USCS[ ( ChamberNo - 1) ] = dCylinderBoreDiameter_USCS;
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the cylinder bore diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetCylinderBoreCrankEndDiameter_SI( int ChamberNo, double dCylinderBoreDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dCylinderBoreDiameter_SI < 0 ) 
    {
        return false;
    }
    
    dCylinderBoreCrankEndDiameter_USCS[ ( ChamberNo - 1) ] = ( dCylinderBoreDiameter_SI * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the cylinder bore diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetCylinderBoreCrankEndDiameter_USCS( int ChamberNo, double dCylinderBoreDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dCylinderBoreDiameter_USCS < 0 ) 
    {
        return false;
    }
    
    dCylinderBoreCrankEndDiameter_USCS[ ( ChamberNo - 1) ] = dCylinderBoreDiameter_USCS;
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the piston rod diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetPistonRodHeadEndDiameter_SI( int ChamberNo, double dPistonRodDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dPistonRodDiameter_SI < 0 ) 
    {
        return false;
    }
    
    dPistonRodHeadEndDiameter_USCS[ ( ChamberNo - 1) ] = ( dPistonRodDiameter_SI * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the piston rod diameter in USCS for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetPistonRodHeadEndDiameter_USCS( int ChamberNo, double dPistonRodDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dPistonRodDiameter_USCS < 0 ) 
    {
        return false;
    }
    
    dPistonRodHeadEndDiameter_USCS[ ( ChamberNo - 1) ] = dPistonRodDiameter_USCS;
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the piston rod diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetPistonRodCrankEndDiameter_SI( int ChamberNo, double dPistonRodDiameter_SI )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dPistonRodDiameter_SI < 0 ) 
    {
        return false;
    }
    
    dPistonRodCrankEndDiameter_USCS[ ( ChamberNo - 1) ] = ( dPistonRodDiameter_SI * units.convert( units.in, units.cm ) );
    return true;
}

//////////////////////////////////////////////////////////////////////
// This method sets the piston rod diameter in SI for the
// selected chamber
//////////////////////////////////////////////////////////////////////
bool CCrankSlider::bSetPistonRodCrankEndDiameter_USCS( int ChamberNo, double dPistonRodDiameter_USCS )
{
    if( ChamberNo < 0 || ChamberNo >= MAXCHAMBERCOUNT )
    {
        return false;
    }

    if( dPistonRodDiameter_USCS < 0 ) 
    {
        return false;
    }
    
    dPistonRodCrankEndDiameter_USCS[ ( ChamberNo - 1) ] = dPistonRodDiameter_USCS;
    return true;
}


 /**********************
 * Description         *
 *---------------------******************************
 * loads the piston angle into the object           *
 ****************************************************/
bool CCrankSlider::SetPistonAngleDegrees( double angle )
{
     //into the object
     PistonAngleDegrees            = angle;

     //Since all calculation work inside the engine is in radians,
     //update the radian equivalent at this time
	PistonAngleRadians			= ( angle * units.convert( units.degrees, units.radians ) );
     
     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the piston area into the object for the    *
 * default chamber                                  *
 ****************************************************/
bool CCrankSlider::SetPistonAreaCrankEnd_USCS( double input_area )
{
     //check data
     if( input_area < 0 )
          return false;

     //otherwise, into the object
     PistonAreaCrankEnd_USCS[0]    = input_area;

     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the piston area into the object for the    *
 * user selected chamber                            *
 ****************************************************/
bool CCrankSlider::SetPistonAreaCrankEnd_USCS( double input_area, int ChamberNo )
{
     //check data
     if( input_area < 0 )
          return false;
     if( ( ChamberNo < 1 ) || ( ChamberNo > MAXCHAMBERCOUNT ) )
          return false;

     //and into the object
     PistonAreaCrankEnd_USCS[ ( ChamberNo - 1 ) ] = input_area;

     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the piston area into the object for the    *
 * default chamber                                  *
 ****************************************************/
bool CCrankSlider::SetPistonAreaHeadEnd_USCS( double input_area )
{
     //check data
     if( input_area < 0 )
          return false;

     //otherwise, into the object
     PistonAreaHeadEnd_USCS[0]    = input_area;

     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the piston area into the object for the    *
 * user selected chamber                            *
 ****************************************************/
bool CCrankSlider::SetPistonAreaHeadEnd_USCS( double input_area, int ChamberNo )
{
     //check data
     if( input_area < 0 )
          return false;
     if( ( ChamberNo < 1 ) || ( ChamberNo > MAXCHAMBERCOUNT ) )
          return false;

     //and into the object
     PistonAreaHeadEnd_USCS[ ( ChamberNo - 1 ) ] = input_area;

     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the pressure into the object for the       *
 * default chamber                                  *
 ****************************************************/
bool CCrankSlider::SetPressureAbsCrankEnd_USCS( double input_pressure )
{
     //check data
     if( input_pressure < 0 )
          return false;

     //otherwise, into the object
     PressureCrankEnd_USCS[0]      = input_pressure;

     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the pressure into the object for the       *
 * user selected chamber                            *
 ****************************************************/
bool CCrankSlider::SetPressureAbsCrankEnd_USCS( double input_pressure, int ChamberNo )
{
     //check data
     if( input_pressure < 0 )
          return false;
     if( ( ChamberNo < 1 ) || ( ChamberNo > MAXCHAMBERCOUNT ) )
          return false;

     //and into the object
     PressureCrankEnd_USCS[ ( ChamberNo - 1 ) ] = input_pressure;

     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the pressure into the object for the       *
 * default chamber                                  *
 ****************************************************/
bool CCrankSlider::SetPressureAbsHeadEnd_USCS( double input_pressure )
{
     //check data
     if( input_pressure < 0 )
          return false;

     //otherwise, into the object
     PressureHeadEnd_USCS[0]    = input_pressure;

     return true;
}

 /**********************
 * Method Definition   *
 *---------------------******************************
 * loads the pressure into the object for the       *
 * user selected chamber                            *
 ****************************************************/
bool CCrankSlider::SetPressureAbsHeadEnd_USCS( double input_pressure, int ChamberNo )
{
     //check data, remembering we work in absolute pressure
     if( input_pressure < 0 )
          return false;

     //Check the chamber range
     if( ( ChamberNo < 0 ) || ( ChamberNo > MAXCHAMBERCOUNT ) )
          return false;

     //and into the object
     PressureHeadEnd_USCS[ ( ChamberNo - 1 ) ] = input_pressure;

     return true;
}

 /**********************
 * Description         *
 *---------------------******************************
 * loads the cylinder orientation into the object   *
 ****************************************************/
bool CCrankSlider::SetRight( bool userRight )
{
     //into the object
     Right                           = userRight;
     
     return true;
}

/////////////////////////////////////////////////////////////
// loads the crankshaft RPM into the slider object 
//
// Revision, 27 Aug 2014
// Added code to trap a negative RPM.
/////////////////////////////////////////////////////////////
bool CCrankSlider::SetRPM( double userRPM )
{
   // Locals
    double dTemp;
   
    // Cache the current value
    dTemp = RPM;
   	
   //into the object
   RPM = userRPM;
     
    // Now we need to perform a check on the variables, restore
    // the original value if the check fails.
    if( !SanityCheck() )
    {
        RPM = dTemp;
	    return false;
    }

   //Go ahead an populate the radians variable
   RadiansPerSecond              = ( ( RPM / 30.0 ) * PI );

   //Done
   return true;
}

/////////////////////////////////////////////////////////////
// Loads the second revolution crankshaft RPM into the slider 
// object 
//
// Revision, 27 August 2014
// Added method
/////////////////////////////////////////////////////////////
bool CCrankSlider::SetRPM01( double userRPM01 )
{
   
    // Locals
    double dTemp;

    // Cache the current value
    dTemp = RPM01;
   	
    // Into the object
    RPM01 = userRPM01;
     
    // Now we need to perform a check on the variables, restore
    // the original value if the check fails.
    if( !SanityCheck() )
    {
        RPM01 = dTemp;
	    return false;
    }

    // Go ahead an populate the radians per second variable
    RadiansPerSecond01 = ( ( RPM01 / 30.0 ) * PI );

    //Done
    return true;
}

/////////////////////////////////////////////////////
// loads the SI r into the object, r should be in
// centimeters.
//
// Revision, 28 August 2014
// Method implemented
/////////////////////////////////////////////////////
bool CCrankSlider::SetStroke_SI( double input_Stroke )
{

    // Validate the input
    if( input_Stroke <= 0 || input_Stroke >10000 )
    {
        return false;
    }

    // Convert from SI to USCS and update internal members
    Stroke_USCS = ( input_Stroke * units.convert(units.cm, units.in));
    r_USCS = ( Stroke_USCS / 2.0 );
    CrankRadiusToConnRodLength = ( r_USCS / ConnRodLength_USCS );

    // Success
    return true;
}

/////////////////////////////////////////////////////
// loads the USCS r into the object 
//
// Revision, 28 August 2014
// Added code to validate the input stroke.
/////////////////////////////////////////////////////
bool CCrankSlider::SetStroke_USCS( double input_Stroke )
{
    // Validate the input
    if( input_Stroke <= 0 || input_Stroke >10000 )
    {
        return false;
    }

    // update the internal object members
    Stroke_USCS = input_Stroke;
    r_USCS = ( Stroke_USCS / 2.0 );
    CrankRadiusToConnRodLength = ( r_USCS / ConnRodLength_USCS );

    // Success
    return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Calculates the SI equivalents of the USCS dims    *
 ****************************************************/
bool CCrankSlider::ToSI()
{

	  //locals
     int i;
     
     //init locals
     i = 0;
   
     //Perform the conversion
     Stroke_SI                     = ( Stroke_USCS * units.convert( units.in, units.cm ) );
     r_SI                          = ( r_USCS * units.convert( units.in, units.cm ) );
     ConnRodLength_SI              = ( ConnRodLength_USCS * units.convert( units.in, units.cm ) );
     PistonAndRodWeight_SI         = ( PistonAndRodWeight_USCS * units.convert( units.lbf, units.N ) );
     CrossheadWeight_SI            = ( CrossheadWeight_USCS * units.convert( units.lbf, units.N ) );
     dAmbientPressure_SI = ( dAmbientPressure_USCS * units.convert( units.PSIA, units.bar ) );
     SliderDisplacement_SI         = ( SliderDisplacement_USCS * units.convert (units.in, units.cm ) ) ;
     SliderVelocity_SI             = ( SliderVelocity_USCS * units.convert( units.in, units.cm ) );
     SliderAcceleration_SI         = ( SliderAcceleration_USCS * units.convert( units.in, units.cm ) );
     SliderInertiaLoad_SI          = ( SliderInertiaLoad_USCS * units.convert( units.lbf, units.N ) );
     SliderGasLoad_SI              = ( SliderGasLoad_USCS * units.convert( units.lbf, units.N ) );
     SliderCombinedLoad_SI         = ( SliderCombinedLoad_USCS * units.convert( units.lbf, units.N ) );
     CrossheadVerticalForce_SI     = ( CrossheadVerticalForce_USCS * units.convert( units.lbf, units.N ));
     MainBearingRadialForce_SI     = ( MainBearingRadialForce_USCS * units.convert( units.lbf, units.N ) );	
     MainBearingTangentForce_SI	= ( MainBearingTangentForce_USCS * units.convert( units.lbf, units.N ) );
     MainBearingTorque_SI          = ( MainBearingTorque_USCS * units.convert( units.ftlb, units.Nm ) );
     for( i=0; i<MAXCHAMBERCOUNT; i++ )
     {
         PistonAreaCrankEnd_SI[i]      = ( PistonAreaCrankEnd_USCS[i] * units.convert( units.in2, units.cm2 ) );
         PistonAreaHeadEnd_SI[i]       = ( PistonAreaHeadEnd_USCS[i] * units.convert( units.in2, units.cm2 ) );
         PressureCrankEnd_SI[i]        = ( PressureCrankEnd_USCS[i] * units.convert( units.PSIA, units.bar ) );
         PressureHeadEnd_SI[i]         = ( PressureHeadEnd_USCS[i] * units.convert( units.PSIA, units.bar ) );
     }
     dDisplacedVolume_SI       = ( dDisplacedVolume_USCS * units.convert( units.in3, units.cm3 ) );
   
	//success
	return true;
}

// Revision - 3 Sep 2014
// Added this method
//
// This method returns the elapsed time between two rotation angles when
// the crankshaft is accelerating.
bool CCrankSlider::_bCalcTimeWithAngAcc(double theta2dotdot, double theta2dot, double theta2, double RPM, double RPM01, double *t)
{

    // locals
    double term1;
	double t1;
	double t2;
    double tMin;
    double tMax;

    // Solve for the time it takes to reach the next angular position,
    // if there is more than 1 RPM of difference between the samples.
    if( abs(theta2dotdot) > 0.000001 )
    {
        // Avoid overflow errors and only process if the radicand is positive.
        term1 = (theta2dot*theta2dot)+(2*theta2dotdot*theta2);
        if( term1 >= 0 )
        {
            
            // It is a solution to quadratic equation so there are
            // two answers.  Solve for both of them here.
            t1 = (-theta2dot+sqrt(term1))/theta2dotdot;
            t2 = (-theta2dot-sqrt(term1))/theta2dotdot;

            // We need to find the root that is between the two
            // limits on speed
            tMin = std::min(60/RPM,60/RPM01)*(theta2/(2 * PI));
            tMax = std::max(60/RPM,60/RPM01)*(theta2/(2 * PI));

            if( t1 >= tMin && t1 <= tMax )
            {
                *t = t1;
            }
            else
            {
                *t = t2;
            }
        }
        else
        {
            // Estimating the delta time by taking the average
            // can sometimes result in a negative radicand.
            // In that case, we have to exit.
            return false;
        }
    }
    else
    {
        // In this case, the two RPM's are the same so
        // there is no acceleration.
        theta2dotdot = 0;
        *t = 1e20;
    }

    return true;
}