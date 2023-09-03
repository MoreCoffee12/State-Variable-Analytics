/*****************************************************************************
   File Name:     RecipFormulas.cpp

   Description:   This was moved from the Recip Plot Wrapper as part of 
                  the 01 Feb 2010 changes for the Display Test Harness.
                  Use this to put Recip Calculations that are common between
                  all Recip projects:  Recip, RecipOverlay, RodPosition,
                  RecipWaterfall.  

   Author:        Wayne Johnson - BNMIN

   Contents:      CRecipFormulas class

   Change History:   

   12 March 2010 - Wayne Johnson - Bug 39347
      Modified GetGasForce to calculate in absolute pressure instead of gauge.  
      Bently Cylinder Pressure probes measure in absolute and are converted
      to gauge for all internal calculations and displayed plot scales.
      Note:  Gauge = Absolute pressure - ambient pressure 
      (At sea level:  1 Standard Atmosphere = 14.695949 psia and 0.0 psig).  

Unpublished Copyright 2010 Bently Nevada, LLC
All Rights Reserved
*****************************************************************************/

#include "stdafx.h"
#include "RecipFormulas.h"
#include <math.h>
#include "..\..\..\..\common\include\unitenumerations.h"
#include "RecipWaveformData.h"
#include "..\..\Include\PlotConstants.h"

/***************************** PUBLIC METHOD *********************************
   Function Name:    CRecipFormulas::GetGasForce

   Given pressure and a cylinder size, this method computes 
   gas force in Pounds Force or KiloNewtons.  

   Inputs     : pRecipData          -  struct with data to compute the gas compression area
                dPressure           -  pressure of given point
                lPressureUnitType   -  the unit type of the pressure measurement
                bIsPaired           -  true for a paired constant pressure chamber
                                       for the given point.  This flips the calculations
                                       to the opposite chamber type: Crank End vs. Head End
   Outputs    : None

  
   Returns    : dGasLoad            -  the gas force measurement for the given pressure
                                       and chamber configuration
  

   Notes      : One reason this is a public method is so that it can be 
                unit tested with a test harness.  This method is a refactor of code 
                modified in the wrappers for CR 21443 - 06 April 2009 - Wayne Johnson 
                and CR 22189 - 28 Jan 2009 - Wayne Johnson

*****************************************************************************/
double CRecipFormulas::GetGasForce(stRecipData* pRecipData, double dPressure, long lPressureUnitType, bool bIsPaired )// CR 22189 - 28 Jan 2009 - Wayne Johnson
{
   double dGasLoad      = 0.0;
   double dArea         = 0.0;
   double dMetricConv   = 1;


   if( !pRecipData )
   {
      return dGasLoad;
   }

   // 12 March 2010 - Wayne Johnson - Bug 39347 - Converting gauge to absolute pressure for calculation
   dPressure = dPressure + pRecipData->ambient_pressure; 

   // If the paired point is crankend than this is headend.
   if( (bIsPaired && pRecipData->bChamberCrankEnd) || (!bIsPaired && !pRecipData->bChamberCrankEnd) )
   {
      dPressure = -dPressure;
   }

   dArea = GetGasLoadArea(pRecipData, bIsPaired ); // For metric, dArea is in mm^2

   switch( lPressureUnitType )
   {
      case BENTLY_UNIT_TYPE_KILOPASCALS_GAUGE :
      {
         // ---------------------------------------------------------------------
         // Note that 1  N = Pa  * m^2
         // and       1  N = kPa * m^2 * 1000
         // and       1 kN = kPa * m^2 
         // ---------------------------------------------------------------------
         dMetricConv = MM2_TO_M2_CONVERSION_DIVISOR; 
         break;
      }
      case BENTLY_UNIT_TYPE_KGCM2_GUAGE :
      {
         // Convert kg/cm^2 to kPa before figuring Force formulas.  
         dPressure *= KGCM2_TO_PA_CONVERSION_FACTOR / PA_TO_KPA_CONVERSION_DIVISOR;  // 12 Feb 2010 - Wayne Johnson - Bug 38160 
         dMetricConv = MM2_TO_M2_CONVERSION_DIVISOR;
         break;
      }
      case BENTLY_UNIT_TYPE_BARS_GUAGE :
      {
         // Convert bar to kPa before figuring Force formulas.
         dPressure *= BAR_TO_PA_CONVERSION_FACTOR / PA_TO_KPA_CONVERSION_DIVISOR;  // 12 Feb 2010 - Wayne Johnson - Bug 38160
         dMetricConv = MM2_TO_M2_CONVERSION_DIVISOR;
         break;
      }
      default :   //case BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_GAUGE :
      {
         dMetricConv = 1; 
         break;
      }
   }

   dGasLoad = dPressure * dArea / dMetricConv; 

   return dGasLoad;

}//GetGasForce

/***************************** PUBLIC METHOD *********************************
   Function Name:    CRecipFormulas::GetGasLoadArea

   Given cylinder size and machine configuration, this method computes 
   the compression area of the cylinder.   

   Inputs     : pRecipData          -  struct with data to compute the gas compression area
                bIsPaired           -  true for a paired constant pressure chamber
                                       for the given point.  This flips the calculations
                                       to the opposite chamber type: Crank End vs. Head End
   Outputs    : None
 
   Returns    : dGasLoadArea        -  the cylinder area minus any piston rod diameter
                                       in the given chamber.  
  
   Notes      : The only reason this is a public method is so that it can be 
                unit tested with a test harness.

*****************************************************************************/
double CRecipFormulas::GetGasLoadArea(stRecipData* pRecipData, bool bIsPaired)
{
   double dGasLoadArea = 1;
   
   // Machine Context is determined by bIsPaired - When the point is instrumented on a 
   // cylinder having constant pressure on the paired chamber, this method will be 
   // invoked twice with bIsPaired == true to do the calculations for the paired constant pressure chamber. 
   if( (bIsPaired && pRecipData->bChamberCrankEnd) || (!bIsPaired && !pRecipData->bChamberCrankEnd) )
   // HEAD END CHAMBER
   {
      if( !pRecipData->bTandemCylinderThrow || (pRecipData->bTandemCylinderThrow && !pRecipData->bCylinderCrankEnd) )
      {
         dGasLoadArea = PI * (  (pRecipData->cylinder_bore_diameter  * 
                                 pRecipData->cylinder_bore_diameter) - 
                                (pRecipData->tail_diameter * 
                                 pRecipData->tail_diameter)) / 4;
      }
      else if( pRecipData->bTandemCylinderThrow && pRecipData->bCylinderCrankEnd )
      {
         dGasLoadArea = PI * (  (pRecipData->cylinder_bore_diameter  * 
                                 pRecipData->cylinder_bore_diameter) -
                                (pRecipData->piston_diameter2 * 
                                 pRecipData->piston_diameter2) ) / 4;
      }
   }
   // CRANK END CHAMBER
   else
   {
      dGasLoadArea = PI * (  (pRecipData->cylinder_bore_diameter  * 
                              pRecipData->cylinder_bore_diameter) -
                             (pRecipData->piston_diameter * 
                              pRecipData->piston_diameter) ) / 4;

   } 

   return dGasLoadArea;

}//GetGasLoadArea

/***************************** PUBLIC METHOD *********************************
   Function Name:    CRecipFormulas::GetGravityForInertialForces

   Given a unit type, this method returns the proper Inertial Force Constant.   

   Inputs     : lPressureUnitType      -  long used to decide to return metric gravity or not
                bGetConversion         -  When true this method is used to return a kilo Newton
                                       conversion instead of a gravity constant.
 
   Returns    : dReturn                -  Either a gravity conversion or a kilo Newton conversion constant.  
  
   Notes      : One reason this is a public method is so that it can be 
                unit tested with a test harness.

*****************************************************************************/
double CRecipFormulas::GetGravityForInertialForces( long lPressureUnitType, bool bGetConversion )
{
   double dReturn = GRAVITY_INCHES_SEC_SQUARED;

   if( BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_GAUGE != lPressureUnitType )
   {
      if( !bGetConversion )
      {
         dReturn = GRAVITY_METRIC_MM_SEC_SQUARED;
      }
      else
      {
         dReturn = KILO_NEWTON_DIVISOR;
      }
   }

   return dReturn;

}//GetGravityForInertialForces



/***************************** PUBLIC METHOD *********************************
   Function Name:    CRecipFormulas::GetInertialForce

   This method returns the Inertial Force for the given machinery.   
   Note that a struct could have been passed in, but for formula readability & clarity,
   the arguements essentially rename struct variables into formula components.  

   Inputs     : (all arguments)
 
   Returns    : dIntertialLoad   -  The inertial force for the given machinery.  
  
   Notes      : One reason this is a public method is so that it can be 
                unit tested with a test harness.


   For Reference:  Original Code from RecipPlotWrapper.cpp
   -----------------------------------------------------------------------------------------------------

   dInertialLoad = cos(dSampleAngle * PI /180.00 + asin(pWaveformCurve->m_pRecipData->stroke_length/2 * sin(dSampleAngle* PI /180.00) / pWaveformCurve->m_pRecipData->connecting_rod_length));
   dInertialLoad = dInertialLoad /
      cos(asin(pWaveformCurve->m_pRecipData->stroke_length/2 * sin(dSampleAngle* PI /180.00) / pWaveformCurve->m_pRecipData->connecting_rod_length));

   dInertialLoad = dInertialLoad + 
      ((pWaveformCurve->m_pRecipData->stroke_length/2 * pow(cos(dSampleAngle* PI /180.00),2))/
      (pWaveformCurve->m_pRecipData->connecting_rod_length* pow(cos(asin(pWaveformCurve->m_pRecipData->stroke_length/2 * sin(dSampleAngle* PI /180.00) / pWaveformCurve->m_pRecipData->connecting_rod_length)),3)));

   dInertialLoad = dInertialLoad * (pWaveformCurve->m_pRecipData->stroke_length/2*((pWaveform->m_pRevolutionSpeeds[0] * 2 * PI)/60) * ((pWaveform->m_pRevolutionSpeeds[0] * 2 * PI)/60));

   // dInertialLoad = dInertialLoad - (dGravity * sin(pWaveformCurve->m_pRecipData->v_angle * PI / 180)); //BEGIN CR 17150 - 05 Nov 2007 - James McDuffie
   // If b_angle_orientation is ZERO, then b Angle is not being used, so use VAngle with old databases
   if(pWaveformCurve->m_pRecipData->b_angle_orientation == 0)
   {
      dInertialLoad = dInertialLoad - (dGravity * cos((pWaveformCurve->m_pRecipData->v_angle / 2) * PI / 180));
   }
   else
   {
      // Use Bank Angle to determine gravity effects
      dInertialLoad = dInertialLoad - (dGravity * cos((pWaveformCurve->m_pRecipData->b_angle) * PI / 180));
   }
   //END   CR 17150 - 05 Nov 2007 - James McDuffie

   dInertialLoad = dInertialLoad * ((pWaveformCurve->m_pRecipData->piston_weight + pWaveformCurve->m_pRecipData->crosshead_weight) / dGravityConversion);

   -----------------------------------------------------------------------------------------------------

*****************************************************************************/

double CRecipFormulas::GetInertialForce(   double SampleAngle,
                                             double StrokeLength, 
                                             double ConnectingRodLength, 
                                             double RevolutionSpeed,
                                             double PistonWeight, 
                                             double CrossheadWeight, 
                                             long   BankAngleOrientation, 
                                             long   V_Angle, 
                                             double BankAngle,
                                             double dGravity, 
                                             double dGravityConversion )
{
   double dInertialLoad = 0.0;

   dInertialLoad = cos(SampleAngle * PI /180.00 + asin( StrokeLength/2 * sin(SampleAngle* PI /180.00) / ConnectingRodLength ));
   dInertialLoad = dInertialLoad / cos(asin( StrokeLength / 2 * sin(SampleAngle* PI / 180.00) / ConnectingRodLength ));

   dInertialLoad = dInertialLoad + 
      ((StrokeLength / 2 * pow( cos(SampleAngle* PI /180.00),2 )) /
      (ConnectingRodLength * pow( cos(asin(StrokeLength / 2 * sin(SampleAngle* PI / 180.00) / ConnectingRodLength)),3 )));

   dInertialLoad = dInertialLoad * ( StrokeLength / 2 * ((RevolutionSpeed * 2 * PI)/60) * ((RevolutionSpeed * 2 * PI)/60) );
   
   // If b_angle_orientation is ZERO, then b Angle is not being used, so use VAngle with old databases
   if( BankAngleOrientation == 0 )
   {
      dInertialLoad = dInertialLoad - (dGravity * cos((V_Angle / 2) * PI / 180));
   }
   else
   {
      // Use Bank Angle to determine gravity effects
      dInertialLoad = dInertialLoad - (dGravity * cos((BankAngle) * PI / 180));
   }

   dInertialLoad = dInertialLoad * ( (PistonWeight + CrossheadWeight) / dGravityConversion );

   return dInertialLoad;

}//GetInertialForce

