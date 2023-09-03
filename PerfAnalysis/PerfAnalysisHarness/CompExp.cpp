// CompExp.cpp: implementation of the CCompExp class.
//
//////////////////////////////////////////////////////////////////////

#include "CompExp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompExpCurve::CCompExpCurve()
{
	//Initialize members
    Clearance_Percent = 0.1;
    BoreDiameter_USCS = 1.5;
    RodDiameter_USCS = 0.2;
    SetStroke_USCS( 0.9 );
    SetConnRodLength_USCS( 2.25 );
    HeadEnd = true;
    GetDisplacedVolume_USCS();

    nexp = 1.019;
    ncomp = 1.40231;
    PressureAbsDischarge_USCS = 28.800;
    PressureAbsSuction_USCS = 14.696;
    PressureAbsCrankAngle_USCS = 20.001;

    LeakSuction = 0.10;
    LeakDischarge = 0.00;
    
    if( HeadEnd )
    {
        dSuctionValveCloseAngle = 180.0;
    }
    else
    {
        dSuctionValveCloseAngle = 0.0;
    }

    //Update the SI members
    ToSI();

}

CCompExpCurve::~CCompExpCurve()
{

}

//////////////////////////////////////////////////////////////////////
// The heart and soul of this object.  This method
// returns the requested pressure point           
//////////////////////////////////////////////////////////////////////
bool CCompExpCurve::bCalcPressure()
{
    	
	//local variables
    double VolumeCrankAngle;
    double dVolumeSuctionValveClose_Percent;
    double dPressurePsuedo;
    double dTemp;
    double dChamberDisplacedVolume_USCS;

    //init locals
    VolumeCrankAngle = 0.0;
    
	//Check out the data
	if(!validitycheck())
		return false;

    //update the volume
    dChamberDisplacedVolume_USCS = GetDisplacedVolume_USCS();
    
    //calculate the volume at which the suction valve closes
    dTemp = GetCrankAngleDegrees();
    if( !SetCrankAngleDegrees( dSuctionValveCloseAngle ) )
    {
        return false;
    }
    dVolumeSuctionValveClose_Percent = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
    if( !HeadEnd )
    {
        dVolumeSuctionValveClose_Percent = ( 1.00 - dVolumeSuctionValveClose_Percent );
    }
    dPressurePsuedo = ( PressureAbsSuction_USCS / pow( ( ( 1.00 + Clearance_Percent ) / ( dVolumeSuctionValveClose_Percent + Clearance_Percent ) ), ncomp ) );
    if( !SetCrankAngleDegrees( dTemp ) )
    {
        return false;
    }

	//loop through crank revolution to find the pressure
    //at the desired crank angle beginning with the Head End
    if( HeadEnd )
    {
            
        if( GetCrankAngleDegrees() < 180 )
        {
            VolumeCrankAngle = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
            PressureAbsCrankAngle_USCS = ( PressureAbsDischarge_USCS * pow( ( Clearance_Percent / ( VolumeCrankAngle + Clearance_Percent ) ), nexp ) );

            //adjust for suction valve opening
            if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS )
            {
                PressureAbsCrankAngle_USCS = PressureAbsSuction_USCS;
            }
        }
        else
        {
            VolumeCrankAngle = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
            PressureAbsCrankAngle_USCS = (dPressurePsuedo * pow( ( ( 1.00 + Clearance_Percent ) / ( VolumeCrankAngle + Clearance_Percent ) ), ncomp ) );
            if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS ) 
            {
                PressureAbsCrankAngle_USCS = PressureAbsSuction_USCS;
            }
            //open the discharge valve, if needed
            if( PressureAbsCrankAngle_USCS > PressureAbsDischarge_USCS )
            {
                PressureAbsCrankAngle_USCS = PressureAbsDischarge_USCS;
            }
        }
    }
    else
    {

        //compression cycle on the crank end
        if( GetCrankAngleDegrees() < 180 )
        {
            VolumeCrankAngle = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
            VolumeCrankAngle = ( 1.00 - VolumeCrankAngle );
            PressureAbsCrankAngle_USCS    = ( dPressurePsuedo * pow( ( ( 1.00 + Clearance_Percent ) / ( VolumeCrankAngle + Clearance_Percent ) ), ncomp ) );
            if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS ) 
            {
                PressureAbsCrankAngle_USCS = PressureAbsSuction_USCS;
            }

            //open the discharge valve
            if( PressureAbsCrankAngle_USCS > PressureAbsDischarge_USCS )
            {
                PressureAbsCrankAngle_USCS         = PressureAbsDischarge_USCS;
            }
        }
        else
        {

            VolumeCrankAngle              = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
            VolumeCrankAngle              = ( 1.00 - VolumeCrankAngle );
            PressureAbsCrankAngle_USCS    = ( PressureAbsDischarge_USCS * pow( ( Clearance_Percent / ( VolumeCrankAngle + Clearance_Percent ) ), nexp ) );

            //adjust for suction valve opening
            if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS )
            {
                PressureAbsCrankAngle_USCS = PressureAbsSuction_USCS;
            }
        }

    }
                    
	//success!
	return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * This method returns the pressure assuming leaks   *
 * (defined by the leak variables) exist in the      *
 * cylinder.                                         *
 *****************************************************/
bool CCompExpCurve::CalcPressureLeak()
{

    //local variables
    double VolumeCrankAngle;
    double Velocity;
    double dChamberDisplacedVolume_USCS;

    //init locals
    VolumeCrankAngle = 0.0;
    Velocity = 0.0;

	//Check out the data
	if(!validitycheck())
    {
        return false;
    }

     //update the volume
    dChamberDisplacedVolume_USCS = GetDisplacedVolume_USCS(); 

    Velocity = ( GetSliderVelocityMax_USCS() - ( fabs( GetSliderVelocity_USCS() ) * ( 0.5 * LeakSuction)) ) / ( GetSliderVelocityMax_USCS() );
    Velocity = ( 2.00 - ( 0.5 * LeakSuction ) - Velocity );

     //loop through crank revolution to find the pressure
     //at the desired crank angle beginning with the Head End
     if( HeadEnd )
     {
          
          if( GetCrankAngleDegrees() < 180 )
          {
               VolumeCrankAngle              = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
               PressureAbsCrankAngle_USCS    = ( PressureAbsDischarge_USCS * pow( ( Clearance_Percent / ( VolumeCrankAngle + Clearance_Percent ) ), ( nexp / ( 1 - LeakSuction ) ) ) );
               PressureAbsCrankAngle_USCS    = ( PressureAbsCrankAngle_USCS * Velocity );
          
               //adjust for suction valve opening
               if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS )
               {
                    PressureAbsCrankAngle_USCS    = PressureAbsSuction_USCS;
               }
          }
          else
          {
               VolumeCrankAngle              = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
               PressureAbsCrankAngle_USCS    = ( PressureAbsSuction_USCS * pow( ( ( 1.00 + Clearance_Percent ) / ( VolumeCrankAngle + Clearance_Percent ) ), ( ncomp * ( 1 - LeakSuction ) ) ) );
               PressureAbsCrankAngle_USCS    = ( PressureAbsCrankAngle_USCS * Velocity );

               //adjust for suction valve opening
               if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS )
               {
                    PressureAbsCrankAngle_USCS    = PressureAbsSuction_USCS;
               }
          
               //open the discharge valve
               if( PressureAbsCrankAngle_USCS > PressureAbsDischarge_USCS )
               {
                    PressureAbsCrankAngle_USCS    = PressureAbsDischarge_USCS;
                    PressureAbsCrankAngle_USCS    = ( PressureAbsCrankAngle_USCS * Velocity );
               }
          }
     }
     else
     {

          //compression cycle on the crank end
          if( GetCrankAngleDegrees() < 180 )
          {
               VolumeCrankAngle              = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
               VolumeCrankAngle              = ( 1.00 - VolumeCrankAngle );
               PressureAbsCrankAngle_USCS    = ( PressureAbsSuction_USCS * pow( ( ( 1.00 + Clearance_Percent ) / ( VolumeCrankAngle + Clearance_Percent ) ), ncomp ) );
               PressureAbsCrankAngle_USCS    = ( PressureAbsCrankAngle_USCS * Velocity );

               //adjust for suction valve opening
               if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS )
               {
                    PressureAbsCrankAngle_USCS    = PressureAbsSuction_USCS;
               }

               //open the discharge valve
               if( PressureAbsCrankAngle_USCS > PressureAbsDischarge_USCS )
               {
                    PressureAbsCrankAngle_USCS         = PressureAbsDischarge_USCS;
                    PressureAbsCrankAngle_USCS    = ( PressureAbsCrankAngle_USCS * Velocity );
               }
          }
          else
          {

               VolumeCrankAngle              = ( ( CalcPistonArea( BoreDiameter_USCS, RodDiameter_USCS) * GetSliderDisplacement_USCS() ) / ( dChamberDisplacedVolume_USCS ) );
               VolumeCrankAngle              = ( 1.00 - VolumeCrankAngle );
               PressureAbsCrankAngle_USCS    = ( PressureAbsDischarge_USCS * pow( ( Clearance_Percent / ( VolumeCrankAngle + Clearance_Percent ) ), nexp ) );
               PressureAbsCrankAngle_USCS    = ( PressureAbsCrankAngle_USCS * Velocity );
               
               //adjust for suction valve opening
               if( PressureAbsCrankAngle_USCS < PressureAbsSuction_USCS )
               {
                    PressureAbsCrankAngle_USCS         = PressureAbsSuction_USCS;
               }
          }

     }
                    
	//success!
	return true;

}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the bore diameter in SI                   *
 ****************************************************/
double CCompExpCurve::GetBoreDiameter_SI()
{
     
     //Update the SI members
     ToSI();

     //return the value
     return BoreDiameter_SI;

}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the bore diameter in USCS                 *
 ****************************************************/
double CCompExpCurve::GetBoreDiameter_USCS()
{
     return BoreDiameter_USCS;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the clearance in percent                  *
 ****************************************************/
double CCompExpCurve::GetClearance_Percent()
{
     return Clearance_Percent;
}

//////////////////////////////////////////////////////////////////////
// Returns the clearance in SI units
//////////////////////////////////////////////////////////////////////
double CCompExpCurve::GetClearance_SI()
{
     //update SI members
     ToSI();

     double dDisplacedVolume_SI = GetDisplacedVolume_SI();

     //convert the percent clearance to engineering units
     dClearance_SI = ( dDisplacedVolume_SI * Clearance_Percent );

     return dClearance_SI;
}

//////////////////////////////////////////////////////////////////////
// Returns the clearance in USCS units 
//////////////////////////////////////////////////////////////////////
double CCompExpCurve::GetClearance_USCS()
{

     double dDisplacedVolume_USCS = GetDisplacedVolume_USCS();

     //convert the percent clearance to engineering units
     dClearance_USCS      = ( dDisplacedVolume_USCS * Clearance_Percent );

     return dClearance_USCS;
}

//////////////////////////////////////////////////////////////////////
// Returns the displaced volume in SI units 
//////////////////////////////////////////////////////////////////////
double CCompExpCurve::GetDisplacedVolume_SI()
{
    return CCrankSlider::GetDisplacedVolume_SI( BoreDiameter_SI, RodDiameter_SI );
}

//////////////////////////////////////////////////////////////////////
// Returns the displaced volume in USCS units 
//////////////////////////////////////////////////////////////////////
double CCompExpCurve::GetDisplacedVolume_USCS()
{
    return CCrankSlider::GetDisplacedVolume_USCS( BoreDiameter_USCS, RodDiameter_USCS );
}

  /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the suction leak coefficient              *
 ****************************************************/
double CCompExpCurve::GetLeakSuction()
{

     return LeakSuction;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the discharge leak coefficient            *
 ****************************************************/
double CCompExpCurve::GetLeakDischarge()
{

     return LeakDischarge;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the compression curve coefficient         *
 ****************************************************/
double CCompExpCurve::GetnComp()
{

     return ncomp;

}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the expansion curve coefficient           *
 ****************************************************/
double CCompExpCurve::GetnExp()
{

     return nexp;

}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the discharge pressure in SI at crankangle*
 ****************************************************/
double CCompExpCurve::GetPressureAbsCrankAngle_SI()
{
     
     //update SI members
     ToSI();

     return PressureAbsCrankAngle_SI;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the discharge pressure in USCS units at   *
 * crank angle                                       *
 ****************************************************/
double CCompExpCurve::GetPressureAbsCrankAngle_USCS()
{

     return PressureAbsCrankAngle_USCS;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the discharge pressure in SI              *
 ****************************************************/
double CCompExpCurve::GetPressureAbsDischarge_SI()
{
     
     //update SI members
     ToSI();

     return PressureAbsDischarge_SI;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the discharge pressure in USCS units      *
 ****************************************************/
double CCompExpCurve::GetPressureAbsDischarge_USCS()
{

     return PressureAbsDischarge_USCS;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the suction pressure in SI                *
 ****************************************************/
double CCompExpCurve::GetPressureAbsSuction_SI()
{
     
     //update SI members
     ToSI();

     return PressureAbsSuction_SI;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the suction pressure in USCS units        *
 ****************************************************/
double CCompExpCurve::GetPressureAbsSuction_USCS()
{

     return PressureAbsSuction_USCS;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the rod diameter in SI                    *
 ****************************************************/
double CCompExpCurve::GetRodDiameter_SI()
{
     
     //Update the SI members
     ToSI();

     //return the value
     return RodDiameter_SI;

}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Returns the Rod diameter in USCS                  *
 ****************************************************/
double CCompExpCurve::GetRodDiameter_USCS()
{
     return RodDiameter_USCS;
}

/////////////////////////////////////////////////////
// Method Definition
/////////////////////////////////////////////////////
// load the bore diameter
/////////////////////////////////////////////////////
bool CCompExpCurve::SetBoreDiameter_USCS( double inputbore )
{

     if( inputbore > 0 )
     {
          BoreDiameter_USCS = inputbore;
     }
     else
     {
          return false;
     }

     return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * load the end clerance in decimal percent          *
 ****************************************************/
bool CCompExpCurve::SetClearance_Percent( double inputclearance )
{
     if( inputclearance >= 0 )
          Clearance_Percent             = inputclearance;
     else
          return false;

     return true;
}

//////////////////////////////////////////////////////////////////////
// Boolean for chamber orientation of headend or crankend
//////////////////////////////////////////////////////////////////////
bool CCompExpCurve::bSetHeadEnd( bool bIsHeadEnd, bool bUseDefaultSuctionValveCloseAngle )
{
    HeadEnd = bIsHeadEnd;
        
    //default value for the suction valve close angle
    //should be updated, depending on which chamber
    //is active.  But we allow the user to choose so
    //the method doesn't overwrite a value the user has
    //configured in the object
    if( bUseDefaultSuctionValveCloseAngle )
    {
         if( HeadEnd )
        {
            dSuctionValveCloseAngle = 180.0;
        }
        else
        {
            dSuctionValveCloseAngle = 0.0;
        }
    }

    return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * sets the severity of suction valve leak           *
 ****************************************************/
bool CCompExpCurve::SetLeakSuction( double inputleak )
{
     if( ( inputleak >= 0 ) && ( inputleak <= 1.0 ) )
     {
          LeakSuction    = inputleak;
     }
     else
     {
          return false;
     }
     
     return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * sets the severity of discharge valve leak         *
 ****************************************************/
bool CCompExpCurve::SetLeakDischarge( double inputleak )
{
     if( ( inputleak >= 0 ) && ( inputleak <= 1.0 ) )
     {
          LeakDischarge  = inputleak;
     }
     else
     {
          return false;
     }
     
     return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Load the compressibility factor                   *
 ****************************************************/
bool CCompExpCurve::SetnComp( double inputn )
{

     if( inputn > 0 )
     {
          ncomp          = inputn;
     }
     else
     {
          return false;
     }

     return true;

}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Load the compressibility factor for expansion     *
 ****************************************************/
bool CCompExpCurve::SetnExp( double inputn )
{

     if( inputn > 0 )
     {
          nexp           = inputn;
     }
     else
     {
          return false;
     }

     return true;

}

 /********************
 * Method Definition *
 *-------------------*********************************
 * load the discharge pressure                       *
 ****************************************************/
bool CCompExpCurve::SetPressureAbsDischarge_USCS( double inputpressure )
{

     if( inputpressure > 0 )
          PressureAbsDischarge_USCS     = inputpressure;
     else
          return false;

     return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * load the suction pressure                         *
 ****************************************************/
bool CCompExpCurve::SetPressureAbsSuction_USCS( double inputpressure )
{

     if( inputpressure > 0 )
          PressureAbsSuction_USCS       = inputpressure;
     else
          return false;

     return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * load the rod  diameter                            *
 ****************************************************/
bool CCompExpCurve::SetRodDiameter_USCS( double inputrod )
{

     if( inputrod >= 0 )
          RodDiameter_USCS              = inputrod;
     else
          return false;

     return true;
}

 /********************
 * Method Definition *
 *-------------------*********************************
 * Calculates the SI equivalents of the USCS dims    *
 ****************************************************/
bool CCompExpCurve::ToSI()
{
    double dTemp01 = GetDisplacedVolume_USCS(); 
    dClearance_USCS          = ( dTemp01 * Clearance_Percent );
    double dTemp02 = GetDisplacedVolume_SI(); 
    dClearance_SI            = ( dTemp02 * Clearance_Percent );
    BoreDiameter_SI          = ( BoreDiameter_USCS * units.convert( units.in, units.cm ) );
    RodDiameter_SI           = ( RodDiameter_USCS * units.convert( units.in, units.cm ) );
    PressureAbsSuction_SI    = ( PressureAbsSuction_USCS * units.convert( units.PSIA, units.bar ) );
    PressureAbsDischarge_SI  = ( PressureAbsDischarge_USCS * units.convert( units.PSIA, units.bar ) );
    PressureAbsCrankAngle_SI = ( PressureAbsCrankAngle_USCS * units.convert( units.PSIA, units.bar ) );
     
    //done
    return true;
}

//This function takes a look at the data
//to see if any of it is crap.  If it is,
//it dumps out of the function and returns
//false
bool CCompExpCurve::validitycheck()
{
	//check to see that clearance is valid
	if( Clearance_Percent < 0 )
		return false;

	//We are dealing with absolute pressures, so
	//neither pressure should be less than 0
	if( PressureAbsDischarge_USCS < 0 )
		return false;
	if( PressureAbsSuction_USCS < 0 )
		return false;

	//The exponent should always be greater than zero
	if( nexp <= 0 )
		return false;
	if( ncomp <= 0 )
		return false;

   //last, but not least, make sure the 
   //mechanical side is okay
   if( !CalcSliderDisplacement() )
      return false;

	//No problems!
	return true;
}

//////////////////////////////////////////////////////////////////////
// return a theoretical curve using the configured values for
// members in this object
//////////////////////////////////////////////////////////////////////
bool CCompExpCurve::bGetTheoreticalCompressionCurveGage_USCS( bool bSteplessUnloader,
                                            double dSuctionValveClosureAngle,
                                            double *dPressureCurveArray,
                                            long lPressureArrayCount )
{
    double dLocalCrankAngle;
    double dArrayCountToAngle = ( (double)360 / (double)lPressureArrayCount);
    double dAmbientPressure;
    long lArrayIndex;

    //load the ambient pressure
    if( !bGetAmbientPressure_USCS( &dAmbientPressure ) )
    {
        return false;
    }

    //load the valve closure angle
    if( !bSetSuctionValveCloseAngle( dSuctionValveClosureAngle ) )
    {
        return false;
    }

    for( lArrayIndex = 0; lArrayIndex< lPressureArrayCount; lArrayIndex++)
    {
        dLocalCrankAngle = ( (double)lArrayIndex * dArrayCountToAngle  );
        SetCrankAngleDegrees( dLocalCrankAngle );
        if( !bCalcPressure() )
        {
            return false;
        }
        
        //save the value to the array
        dPressureCurveArray[lArrayIndex] = ( PressureAbsCrankAngle_USCS - dAmbientPressure );    
    }

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// object set method for the suction valve close angle
//////////////////////////////////////////////////////////////////////
bool CCompExpCurve::bSetSuctionValveCloseAngle( double dSuctionValveCloseAngle )
{
    if( dSuctionValveCloseAngle < 0 )
    {
        return false;
    }

    CCompExpCurve::dSuctionValveCloseAngle = fmod( dSuctionValveCloseAngle, 360.0 );

    return true;
}

//////////////////////////////////////////////////////////////////////
// object get method for the suction valve close angle
//////////////////////////////////////////////////////////////////////
bool CCompExpCurve::bGetSuctionValveCloseAngle( double *dSuctionValveCloseAngle )
{

    *dSuctionValveCloseAngle = CCompExpCurve::dSuctionValveCloseAngle;

    return true;
}