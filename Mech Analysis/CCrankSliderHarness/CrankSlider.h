//////////////////////////////////////////////////////////////////////
// This is the interface for the crank-slider   
// class	                                      
//////////////////////////////////////////////////////////////////////
//                                               
//////////////////////////////////////////////////////////////////////
// Written by:	Brian Howard                     
// Date:		18 Dec 2001                      
//////////////////////////////////////////////////////////////////////
// Unit Assumptions:                            
//    - All angles are in radians               
//    - All angular velocity in radians/second  
//    - All time is in seconds                  
//    - All lengths are in USCS inches          
//    - All pressure in USCS absolute PSIA      
//    - All forces are in USCS lb force         
//    - All torques are in USCS ft-lb           
//
// SI base unit definitions:
//    - All lengths are in centimeters
//
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// Revision Notes
//
//////////////////////////////////////////////////////////////////////
// Updated to include GetSliderVelocityMax_USCS, displaced volume
// members and methods and piston/bore information.
//
// 14 July 2010
// Brian Howard
//////////////////////////////////////////////////////////////////////
// Add the methods GetSliderInertiaLoadWithAngAcc_USCS(); and
// GetSliderInertiaLoadWithAngAcc_SI(); to allow calculation of 
// inertia load under crankshaft angular acceleration
//
// 27 Aug 2014
// Brian Howard
//////////////////////////////////////////////////////////////////////


#include "units.h"

#if !defined(AFX_CRANKSLIDER_H__684BD538_731E_4D94_ACE8_C165132179A6__INCLUDED_)
#define AFX_CRANKSLIDER_H__684BD538_731E_4D94_ACE8_C165132179A6__INCLUDED_

#define MAXCHAMBERCOUNT 10
#define PI 3.1415926535897932384626433832795
#define HALFPI 1.5707963267948966192313216916398
#define MAXAMBIENTPRESSURE 20.000
#define MINAMBIENTPRESSURE 0.000

class CCrankSlider  
{
public:

	/*Members
	******************************************************************/


	/*Methods
	******************************************************************/
                                        CCrankSlider();
     virtual                            ~CCrankSlider();

     //Object access methods
     bool                               SetPistonAngleDegrees( double angle );
     bool                               SetCCW( bool userCCW );
     bool                               SetRight( bool userRight );
     bool                               SetRPM( double userRPM );
     bool                               SetRPM01( double userRPM01 );
     bool                               SetCrankAngleDegrees( double input_CrankAngle );

     bool                               SetStroke_USCS (double input_Stroke );
     bool                               SetConnRodLength_USCS (double input_l );
     bool                               SetPistonAndRodWeight_USCS (double input_weight );
     bool                               SetCrossheadWeight_USCS( double input_weight );
     bool                               bSetAmbientPressure_USCS( double input_pressure );
     bool                               SetPistonAreaCrankEnd_USCS( double input_area );
     bool                               SetPistonAreaCrankEnd_USCS( double input_area, int ChamberNo );
     bool                               SetPistonAreaHeadEnd_USCS( double input_area );
     bool                               SetPistonAreaHeadEnd_USCS( double input_area, int ChamberNo );
     bool                               SetPressureAbsCrankEnd_USCS( double input_pressure );
     bool                               SetPressureAbsCrankEnd_USCS( double input_pressure, int ChamberNo );
     bool                               SetPressureAbsHeadEnd_USCS( double input_pressure );
     bool                               SetPressureAbsHeadEnd_USCS( double input_pressure, int ChamberNo );

     bool                               SetStroke_SI (double input_Stroke ) ;
     bool                               SetConnRodLength_SI (double input_l );
     bool                               SetPistonAndRodWeight_SI (double input_weight ) { return false; }
     bool                               SetCrossheadWeight_SI( double input_weight ) {return false; }
     bool                               SetAmbientPressure_SI( double input_pressure ) { return false; }
     bool                               SetPistonAreaCrankEnd_SI( double input_area ) { return false; }
     bool                               SetPistonAreaCrankEnd_SI( double input_area, int ChamberNo ) {return false; }
     bool                               SetPistonAreaHeadEnd_SI( double input_area ) {return false;}
     bool                               SetPistonAreaHeadEnd_SI( double input_area, int ChamberNo ) {return false;}
     bool                               SetPressureAbsCrankEnd_SI( double input_pressure ) {return false;}
     bool                               SetPressureAbsCrankEnd_SI( double input_pressure, int ChamberNo ) {return false;}
     bool                               SetPressureAbsHeadEnd_SI( double input_pressure ){return false;}
     bool                               SetPressureAbsHeadEnd_SI( double input_pressure, int ChamberNo ){return false;}

     int                                GetPistonArea_MaxCount();
     double                             GetPistonAngleDegrees();
     double                             GetPistonAngleRadians();
     double                             GetRPM();
     double                             GetRPM01();
     double                             GetRadiansPerSecond();
     double                             GetCrankAngleDegrees();
     double                             GetCrankAngleRadians();
     double                             GetConnRodAngleDegrees();
     double                             GetConnRodAngleRadians();

     double                             Getr_USCS();
     double                             GetStroke_USCS();
     double                             GetConnRodLength_USCS();
     double                             GetPistonAndRodWeight_USCS();
     double                             GetCrossheadWeight_USCS();
     bool bGetAmbientPressure_USCS( double * dAmbientPressure );
     double                             GetPistonAreaCrankEnd_USCS();
     double                             GetPistonAreaCrankEnd_USCS( int ChamberNo );
     double                             GetPistonAreaHeadEnd_USCS();
     double                             GetPistonAreaHeadEnd_USCS( int ChamberNo );
     double                             GetPressureAbsCrankEnd_USCS();
     double                             GetPressureAbsCrankEnd_USCS( int ChamberNo );
     double                             GetPressureAbsHeadEnd_USCS();
     double                             GetPressureAbsHeadEnd_USCS( int ChamberNo );
     double                             GetSliderDisplacement_USCS();
     double GetSliderVelocity_USCS();
     double GetSliderVelocityMax_USCS();
     double                             GetSliderAcceleration_USCS();
     double                             GetSliderAccelerationWithAngAcc_USCS();
     double                             GetSliderInertiaLoad_USCS();
     double                             GetSliderInertiaLoadWithAngAcc_USCS();
     double                             GetSliderGasLoad_USCS();
     double                             GetSliderCombinedLoad_USCS();
     double                             GetSliderCombinedLoadWithAngAcc_USCS();
     double                             GetCrossheadVerticalForce_USCS();
     double                             GetMainBearingRadialForce_USCS();
     double                             GetMainBearingTangentForce_USCS();
     double                             GetMainBearingTorque_USCS();

     double                             Getr_SI();
     double                             GetStroke_SI();
     double                             GetConnRodLength_SI();
     double                             GetPistonAndRodWeight_SI();
     double                             GetCrossheadWeight_SI();
     bool bGetAmbientPressure_SI( double * dAmbientPressure);
     double                             GetPistonAreaCrankEnd_SI();
     double                             GetPistonAreaCrankEnd_SI( int ChamberNo );
     double                             GetPistonAreaHeadEnd_SI();
     double                             GetPistonAreaHeadEnd_SI( int ChamberNo );
     double                             GetPressureAbsCrankEnd_SI();
     double                             GetPressureAbsCrankEnd_SI( int ChamberNo );
     double                             GetPressureAbsHeadEnd_SI();
     double                             GetPressureAbsHeadEnd_SI( int ChamberNo );

     bool bGetCylinderBoreHeadEndDiameter_USCS( int ChamberNo, double *dCylinderBoreDiameter_USCS );
     bool bGetCylinderBoreHeadEndDiameter_SI( int ChamberNo, double *dCylinderBoreDiameter_USCS );
     bool bGetPistonRodHeadEndDiameter_USCS( int ChamberNo, double *dPistonRodDiameter_USCS );
     bool bGetPistonRodHeadEndDiameter_SI( int ChamberNo, double *dPistonRodDiameter_USCS );
     bool bGetCylinderBoreCrankEndDiameter_USCS( int ChamberNo, double *dCylinderBoreDiameter_USCS );
     bool bGetCylinderBoreCrankEndDiameter_SI( int ChamberNo, double *dCylinderBoreDiameter_USCS );
     bool bGetPistonRodCrankEndDiameter_USCS( int ChamberNo, double *dPistonRodDiameter_USCS );
     bool bGetPistonRodCrankEndDiameter_SI( int ChamberNo, double *dPistonRodDiameter_USCS );

     bool bSetCylinderBoreHeadEndDiameter_USCS( int ChamberNo, double dCylinderBoreDiameter_USCS );
     bool bSetCylinderBoreHeadEndDiameter_SI( int ChamberNo, double dCylinderBoreDiameter_USCS );
     bool bSetPistonRodHeadEndDiameter_USCS( int ChamberNo, double dPistonRodDiameter_USCS );
     bool bSetPistonRodHeadEndDiameter_SI( int ChamberNo, double dPistonRodDiameter_USCS );
     bool bSetCylinderBoreCrankEndDiameter_USCS( int ChamberNo, double dCylinderBoreDiameter_USCS );
     bool bSetCylinderBoreCrankEndDiameter_SI( int ChamberNo, double dCylinderBoreDiameter_USCS );
     bool bSetPistonRodCrankEndDiameter_USCS( int ChamberNo, double dPistonRodDiameter_USCS );
     bool bSetPistonRodCrankEndDiameter_SI( int ChamberNo, double dPistonRodDiameter_USCS );

     double GetDisplacedVolume_USCS(double dBoreDiameter_USCS, double dRodDiameter_USCS);
     double GetDisplacedVolume_SI(double dBoreDiameter_SI, double dRodDiameter_SI);

     double                             GetSliderDisplacement_SI();
     double                             GetSliderVelocity_SI();
     double                             GetSliderAcceleration_SI();
     double                             GetSliderAccelerationWithAngAcc_SI();
     double                             GetSliderInertiaLoad_SI();
     double                             GetSliderInertiaLoadWithAngAcc_SI();
     double                             GetSliderGasLoad_SI();
     double                             GetSliderCombinedLoad_SI();
     double                             GetSliderCombinedLoadWithAngAcc_SI();
     double                             GetCrossheadVerticalForce_SI();
     double                             GetMainBearingRadialForce_SI();
     double                             GetMainBearingTangentForce_SI();
     double                             GetMainBearingTorque_SI();

     //workhorse methods
     bool                               CalcSliderDisplacement();
     bool                               CalcSliderVelocity();
     bool                               CalcSliderAcceleration();
     bool                               CalcSliderAccelerationWithAngAcc();
     bool                               CalcSliderInertiaLoad();
     bool                               CalcSliderInertiaLoadWithAngAcc();
     bool                               CalcSliderGasLoad();
     bool                               CalcSliderCombinedLoad();
     bool                               CalcSliderCombinedLoadWithAngAcc();
     bool                               CalcCrossheadVerticalForce();
     bool                               CalcMainBearingRadialForce();
     bool                               CalcMainBearingTangentForce();
     bool                               CalcMainBearingTorque();
     bool                               CalcConnRodAngle();
     double                             CalcTDCsec();
     double                             CalcDeltaTime( double StartingAngleRadians, double EndingAngleRadians, double StartingTime );
     double                             CalcSegStartTime( double StartingAngleRadians, double StartingTime );
     double                             CalcPistonArea( double bore, double rod );

private:

     /*Members
     ******************************************************************/
     //housekeeping and common variables
     class                              CUnits units;
     bool                               CCW;
     bool                               Right;

     //Internally, all calculation will be performed in Radians
     double                             PistonAngleDegrees;
     double                             PistonAngleRadians;
     double                             RPM;
     double                             RadiansPerSecond;
     double                             RPM01;
     double                             RadiansPerSecond01;
     double                             CrankAngleRadians;
     double                             CrankAngleDegrees;
     double                             ConnRodAngleRadians;
     double                             ConnRodAngleDegrees;
     double                             CrankRadiusToConnRodLength;

     //Internally, all calulations will be done in USCS
     //and converted to SI at the end.  All lengths USCS
     //lengths are in inches
     double                             r_USCS;
     double                             Stroke_USCS;
     double                             ConnRodLength_USCS;
     double                             PistonAndRodWeight_USCS;
     double                             CrossheadWeight_USCS;
     
     double dDisplacedVolume_USCS;
     double dDisplacedVolume_SI;
     
     double dAmbientPressure_USCS;

     double dCylinderBoreHeadEndDiameter_USCS[MAXCHAMBERCOUNT];
     double dCylinderBoreCrankEndDiameter_USCS[MAXCHAMBERCOUNT];
     double dPistonRodHeadEndDiameter_USCS[MAXCHAMBERCOUNT];
     double dPistonRodCrankEndDiameter_USCS[MAXCHAMBERCOUNT];
     
     double                             PressureCrankEnd_USCS[MAXCHAMBERCOUNT];
     double                             PressureHeadEnd_USCS[MAXCHAMBERCOUNT];
     double                             PistonAreaCrankEnd_USCS[MAXCHAMBERCOUNT];
     double                             PistonAreaHeadEnd_USCS[MAXCHAMBERCOUNT];
     double                             SliderDisplacement_USCS;
     double                             SliderVelocity_USCS;
     double                             SliderAcceleration_USCS;
     double                             SliderGasLoad_USCS;
     double                             SliderInertiaLoad_USCS;
     double                             SliderCombinedLoad_USCS;
     double                             CrossheadVerticalForce_USCS;
     double                             MainBearingRadialForce_USCS;
     double                             MainBearingTangentForce_USCS;
     double                             MainBearingTorque_USCS;

     //All SI lengths are in cm
     double                             r_SI;
     double                             Stroke_SI;
     double                             ConnRodLength_SI;
     double                             PistonAndRodWeight_SI;
     double                             CrossheadWeight_SI;
     double dAmbientPressure_SI;
     double                             PressureCrankEnd_SI[MAXCHAMBERCOUNT];
     double                             PressureHeadEnd_SI[MAXCHAMBERCOUNT];
     double                             PistonAreaCrankEnd_SI[MAXCHAMBERCOUNT];
     double                             PistonAreaHeadEnd_SI[MAXCHAMBERCOUNT];
     double                             SliderDisplacement_SI;
     double                             SliderVelocity_SI;
     double                             SliderAcceleration_SI;
     double                             SliderGasLoad_SI;
     double                             SliderInertiaLoad_SI;
     double                             SliderCombinedLoad_SI;
     double                             CrossheadVerticalForce_SI;
     double                             MainBearingRadialForce_SI;
     double                             MainBearingTangentForce_SI;
     double                             MainBearingTorque_SI;

    //Methods
    //////////////////////////////////////////////////////////////////////
    //housekeeping functions
    bool ToSI();
    bool SanityCheck();
    bool CalcDisplacedVolume( double dBoreDiameter_USCS, double dRodDiameter_USCS );
    bool _bCalcTimeWithAngAcc(double theta2dotdot, double theta2dot, double theta2, double RPM, double RPM01, double *t);

};

#endif // !defined(AFX_CRANKSLIDER_H__684BD538_731E_4D94_ACE8_C165132179A6__INCLUDED_)
