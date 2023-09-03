/*******************************************************************************
   File Name:   RecipPPLCalc.cpp

   Description: Contains the implementation to the CRecipPPLCalc class.

   Author:      Kevin Jensen - BNMIN

   Contents:
      Class CRecipPPLCalc
      GetRecipPPLs					          
      GetGasLoad	
      GetVolumetricEfficiencies	
      GetPistonPositionAtAngle	
      GetCompressionRatio	
      GetIndicatedHorsePower	
      GetTheoreticalDischargeTemp	
      GetGasFlows	
      GetClearanceVolume	
      GetSuctionDischargePressure	
      GetEngValue
      SetAngleOffsetIndex
      SetSuctionAndDischargeIndexes
      SetValveOpenIndex
      GetAtoDCount
      GetCrankAngleAtSuctionValveClosure

   Change History:
      cr# 8726
      cr9094
      cr 9160
      cr 9227
      1 May 2002 KJ fixed divide by zero errors cr 845
      May 9 2002 - Kevin Jensen - fix negative horsepower on crank end
      Jan 27 2002 Kevin Jensen - fix cr 1878
      9 April 2003 Kevin Jensen -
      5 November 2003 - Mike Peden - BNMIN - CF 314
         Fixed kW conversion factor, loop counter for indicated horse power,
         and conversion from kW to watts.
      30 Sep 2005 - Alan Sommercorn
         Stepless Unloader Enhancement
         1.  Added two parameters to GetRecipPPLs so that we can get the
         percentage of clearance volume entered by the user in Config if
         they choose not to use the calculated clearance volume.
         2.  Added the SetAngleOffsetIndex, SetSuctionAndDischargeIndexes, 
         and SetValveOpenIndex to set class member variables once at the 
         beginning and eliminate redundant code.
         3.  Added several member variables to eliminate the calculation of
         values in multiple functions and to reduce the number of values being
         passed to methods within the same class.
         4.  Added GetAtoDCount method to take an eng value and calculate the
         A to D value.
         5.  Added GetCrankAngleAtSuctionValveClosure method to calculate the
         suction valve closure angle using the pressure waveform data and 
         Rodney Stansbury's Line Method Algorithm.  This method is used by the
         GetVolumetricEfficiencies method to help determine the correct piston
         position for the volumetric efficiency equation.  The capacity, flow
         balance, and power / median capacity ratio calculations all use the
         resulting volumetric efficiency value in their calculations.  Power
         Loss calculations are not being modified.  Clearance volume uses a
         value that is the same as the old volumetric efficiency but represents
         something else.

      01 Dec 2005 - Tim Valdepena - CR 3718
         Added code to constructor to log an event in the NT event log if
         MATLAB fails to initialize.

      13 Dec 2005 - Alan Sommercorn - CR 3810
         Adding the calculation of the theoretical suction 
         valve closure to the GetCrankAngleAtSuctionValveClosure method
         so that the adiabatic pressure curves can be calculated correctly.

      26 Jan 2006 - Tim Valdepena  - CR 4774 
         In SetValveOpenIndex, changed the <= and >= in the while loops 
         to < and >, respectively

      07 Feb 2006 - Tim Valdepena - CR 7220
         Added destructor and put call to UninitializeMatlabPloyfit() here

      27 Feb 2006 - Tim Valdepena
         CR7606 - Added g_lReferences to track how many objects are 
         using the Matlab runtime.  This is to fix a problem where 
         closing a plot group would uninitialize the Matlab Runtime, 
         causing subsequently opened plot groups to not work properly.

      09 Jun 2006 - Ivan Johnson BNMIN - CR 9476
         Remove Matlab and all associated code.

      15 Nov 2006 - Ivan Johnson BNMIN - CR 12550
         Adjust the suction loss calculations to include the area above the 
         suction pressure line, up to the point the unloader releases, as well 
         as the area below the suction line.  Make an adjustment for the useful work
         done compressing the gas prior to the point where the unloader releases 
         (suction valve closes).

      15 Nov 2006 - Ivan Johnson BNMIN - CR 12551
         Adjust calculation of suction volumetric efficiency to use theoretical 
         rather than actual point of suction valve closure.  This accounts for
         the fact that, with a stepless unloader, the gas is already slightly 
         compressed when the suction valve closes.

      15 Nov 2006 - Ivan Johnson BNMIN - CR 12829
         Shift the theoretical curve for the suction cycle to more closely align
         with the actual data.

      27 Dec 2006 - Daanish Rumani - CR 11194
         Dynamically assigning the sizes of arrays used to store intermediate results
         instead of having statically assigned arrays. Changed the return type of
         SetValveOpenIndex(). It returns false on failure so that the calling function
         can be notified of an error condition.

      18 Nov 07 - Tim Valdepena - CR 14045 modified SetValveOpenIndex to fix a problem
         with adiabatic overlays

      28 Jan 2009 - Wayne Johnson - CR 22189
      Made bool parameter into a long, to allow for kg/cm^2 metric units 
      in addition to the existing psi English units and Pascal SI units.
      Changed formula conversion numbers for the new units.  

      25 March 2009 - James McDuffie - CR 22358
      Made changes to safeguard against divide by zero errors, and ensured that
      if an Isentropic Compression Process is not associated and Software Variables
      are enabled, that the FitPolynomial method does not crash DAQ.

      06 April 2009 - Wayne Johnson - CR 21443
      Changed formula conversion numbers to allow for pressure measurement units in Bars.  

   Unpublished Copyright 1998-2009 Bently Nevada, LLC
   All Rights Reserved
*******************************************************************************/

#include "stdafx.h"
#include "..\interfaces\RecipPPLs.h"
#include "RecipPPLCalc.h"
//#include "..\..\Display\Common\classes\curve.h"  //for CStaticSamplePairList typedef
#include <math.h>
#include "..\classes\floatcompare.h"
#include "..\Math\polynomial.h"  // CR 9476
#include "..\include\unitenumerations.h"  // CR 22189 - 28 Jan 2009 - Wayne Johnson

const double PI = 3.14159265358979;//32384626433832795;
//const double PRESSURE_CONVERSION = 6.895;
const double TEMP_CONST_F = 459.69;
const double TEMP_CONST_C = 273.16;
const double PSIA_CONST = 14.7;
const double CAPACITY_CONV = .00000083333333333333;
/////////////////////////////////////////////////////////////////////////////
// CRecipPPLCalc


/**********************************************************
   Name:  CRecipPPLCalc::GetRecipPPLs

   Change History:
   DD-MMM-YYYY    CR Number   Name                 Description
   -----------------------------------------------------------------------------
   22-Nov-2006    CR 11194    Daanish Rumani       Returning E_FAIL if
                                                   SetValveOpenIndex() returns
                                                   failure.

**********************************************************/
STDMETHODIMP CRecipPPLCalc::GetRecipPPLs(/*[in]*/ float fSampleRate,
                                       /*[in]*/ float fXdcr_scale_factor,
                                       /*[in]*/ float fMv_per_bit,
                                       /*[in]*/ float fAmpl_offset,
                                       /*[in]*/ float fTSF_offset,
                                       /*[in]*/ long lPistonAngle,
                                       /*[in]*/ ULONG ulSpeed,
                                       /*[in]*/ short sKPHIndex,
                                       /*[in]*/ short nSamples,
                                       /*[in]*/ short *pSamples,
                                       /*[in}*/ long lPressureUnitType, // CR 22189 - 28 Jan 2009 - Wayne Johnson
                                       /*[in]*/ BOOL bTempUnitF, 
                                       /*[in]*/ stRecipData recipData, 
                                       /*[in]*/ stRecipTempPressureData tempPressureData, 
                                       /*[in]*/ stRecipParameterInfo recipParamInfo,
                                       /*[in]*/ stRecipParamData recipParamData,
                                       /*[out]*/stRecipPPLs* pRecipPPLs)
{
	HRESULT hResult = E_FAIL;
   double dGasLoad = 0;
   double dRC = 0;
   
   m_fXdcr_scale_factor = fXdcr_scale_factor;
   m_fMv_per_bit = fMv_per_bit;
   m_fAmpl_offset = fAmpl_offset;
   m_fTSF_offset = fTSF_offset;
   m_pSamples = pSamples;
   m_nSamples = nSamples;
   m_recipParamInfo = recipParamInfo;
   m_recipParamData = recipParamData;

   m_dAngleInc = 1;
   if(!FLOAT_EQUAL(fSampleRate, 0.0)) // CR 22358 - James McDuffie
   {
      m_dAngleInc = 360.00 / fSampleRate;
   }
   else
   {
      return hResult;
   }

   //set commonly used indexes for various suction and 
   //discharge valve events
   SetAngleOffsetIndex((int)lPistonAngle, sKPHIndex);
   SetSuctionAndDischargeIndexes(recipData.bChamberCrankEnd);
   // <<< CR 11194 Daanish Rumani 22 Nov 2006
   //SetValveOpenIndex();
   if(!SetValveOpenIndex())
   {
      return E_FAIL;
   }
   // >>> CR 11194 Daanish Rumani 22 Nov 2006

   //Begin Kevin Jensen cr 1878
   ZeroMemory(pRecipPPLs, sizeof(stRecipPPLs));
   //END Kevin Jensen cr 1878
   dGasLoad = GetGasLoad(recipData);
   pRecipPPLs->gasLoad = dGasLoad;
   m_dDisplacedVolume = dGasLoad * recipData.stroke_length;

   GetVolumetricEfficiencies(ulSpeed,
                           lPressureUnitType,   // CR 22189 - 28 Jan 2009 - Wayne Johnson
                           recipData,
                           tempPressureData,
                           *pRecipPPLs);

   GetIndicatedHorsePower(ulSpeed,
                           lPressureUnitType,   // CR 22189 - 28 Jan 2009 - Wayne Johnson
                           recipData,
                           *pRecipPPLs);
   
   dRC = GetCompressionRatio(recipData,tempPressureData);
   GetTheoreticalDischargeTemp(dRC,bTempUnitF, tempPressureData,*pRecipPPLs);
   
   GetGasFlows(ulSpeed,
               lPressureUnitType,   // CR 22189 - 28 Jan 2009 - Wayne Johnson
               bTempUnitF, 
               recipData,
               tempPressureData,
               *pRecipPPLs);

   //adjust values to be displayed properly
   pRecipPPLs->dischargeVolumetricEfficiency = pRecipPPLs->dischargeVolumetricEfficiency * 100;
   pRecipPPLs->suctionVolumetricEfficiency = pRecipPPLs->suctionVolumetricEfficiency * 100;
   //Begin 9 April 2003 KJ change to be clearance percent
   pRecipPPLs->theoreticalClearanceVolume = pRecipPPLs->theoreticalClearanceVolume * 100;
   //End 9 April 2003 KJ change to be clearance percent
   if(pRecipPPLs->powerLossDischargeCycle < 0)
   {
      pRecipPPLs->powerLossDischargeCycle = -pRecipPPLs->powerLossDischargeCycle;
   }
   if(pRecipPPLs->powerLossSuctionCycle < 0)
   {
     pRecipPPLs->powerLossSuctionCycle = -pRecipPPLs->powerLossSuctionCycle;
   }

	return S_OK;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetSuctionDischargePressure

   Change History:
   DD-MMM-YYYY    CR Number   Name                 Description
   -----------------------------------------------------------------------------
   22-Nov-2006    CR 11194    Daanish Rumani       Returning E_FAIL if
                                                   SetValveOpenIndex() returns
                                                   failure.

**********************************************************/
STDMETHODIMP CRecipPPLCalc::GetSuctionDischargePressure(/*[in]*/ float fSampleRate,
							/*[in]*/ float fXdcr_scale_factor,
							/*[in]*/ float fMv_per_bit,
							/*[in]*/ float fAmpl_offset,
							/*[in]*/ float fTSF_offset,
							/*[in]*/ long lPistonAngle,
							/*[in]*/ short sKPHIndex,
							/*[in]*/ short *pSamples,
							/*[in]*/ BOOL bChamberCrankEnd, 
							/*[out]*/double* dSuctionPressure,
							/*[out]*/double* dDischargePressure)
{
   int iSuctionPressure = 0;
   int iDischargePressure= 0;

   m_pSamples = pSamples;
   m_fXdcr_scale_factor = fXdcr_scale_factor;
   m_fMv_per_bit = fMv_per_bit;
   m_fAmpl_offset = fAmpl_offset;
   m_fTSF_offset = fTSF_offset;

   m_dAngleInc = 1;
   if(!FLOAT_EQUAL(fSampleRate,0.0)) // CR 22358 - James McDuffie
   {
      m_dAngleInc = 360.00 / fSampleRate;
   }
   else
   {
      return E_FAIL;
   }

   if(!FLOAT_EQUAL(m_dAngleInc,0.0)) // CR 22358 - James McDuffie
   {
	  m_nSamples = (int)(fSampleRate / m_dAngleInc);
   }
   else
   {
	   return E_FAIL;
   }
 
   //set commonly used indexes for various suction and 
   //discharge valve events
   SetAngleOffsetIndex((int)lPistonAngle, sKPHIndex);
   SetSuctionAndDischargeIndexes(bChamberCrankEnd);
   // <<< CR 11194 Daanish Rumani 22 Nov 2006
   //SetValveOpenIndex();
   if(!SetValveOpenIndex())
   {
      return E_FAIL;
   }
   // >>> CR 11194 Daanish Rumani 22 Nov 2006

	*dSuctionPressure = GetEngValue(m_nSuctionPressureIndex);

	*dDischargePressure = GetEngValue(m_nDischargePressureIndex);
	
  return S_OK;
}

/**********************************************************
   Name:  CRecipPPLCalc::SetSuctionAndDischargeIndexes

  Change History:

**********************************************************/
void CRecipPPLCalc::SetSuctionAndDischargeIndexes(BOOL bChamberCrankEnd)
{
	m_nSuctionPressureIndex = m_nAngleOffsetIndex;
	m_nDischargePressureIndex = m_nAngleOffsetIndex;

	if(!FLOAT_EQUAL(m_dAngleInc, 0.0)) // CR 22358 - James McDuffie
	{
		if(!bChamberCrankEnd)//headend
		{
			m_nSuctionPressureIndex = m_nAngleOffsetIndex + (int)(180.0 / m_dAngleInc);
		}
		else
		{
			m_nDischargePressureIndex = m_nAngleOffsetIndex + (int)(180.0 / m_dAngleInc);
		}
	}
}

/**********************************************************
   Name:  CRecipPPLCalc::SetValveOpenIndex

  Change History:
  26 Jan 2006 Tim Valdepena CR 4774 changed the <= and >= in the while loops to < and >, respectively
  
  22 Nov 2006 CR 11194 Daanish Rumani
      Modified. Changed the return value to BOOL. Returns false when a suitable index
      is not found so that the calling function can be notified of an error.

  18 Nov 07 - Tim Valdepena - CR 14045  - added '+ m_nAngleOffsetIndex' to the 2 while loops
     and the if statement at the bottom to fix a problem with adiabatic overlays

**********************************************************/
// <<< CR 11194 Daanish Rumani 22 Nov 2006
//void CRecipPPLCalc::SetValveOpenIndex()
BOOL CRecipPPLCalc::SetValveOpenIndex()
// >>> CR 11194 Daanish Rumani 22 Nov 2006
{
   m_nAfterSuctionOpenIndex = m_nDischargePressureIndex; //init to known position before suction valve opens
   m_nAfterDischargeOpenIndex = m_nSuctionPressureIndex;  //init to known position before discharge valve opens

   //find the first sample index where the pressure drops below the suction pressure
	while(m_pSamples[m_nAfterSuctionOpenIndex] > m_pSamples[m_nSuctionPressureIndex] && //CR4774 - TLV
         m_nAfterSuctionOpenIndex < m_nSamples + m_nAngleOffsetIndex) // m_nAfterSuctionOpenIndex < m_nSamples-1) // CR 11194  //CR 14045
   { 
	   m_nAfterSuctionOpenIndex++;
	}

   //find the first sample index where the pressure goes above the discharge pressure
   while(m_pSamples[m_nAfterDischargeOpenIndex] < m_pSamples[m_nDischargePressureIndex] && //CR4774 - TLV
         m_nAfterDischargeOpenIndex < m_nSamples + m_nAngleOffsetIndex) // m_nAfterDischargeOpenIndex < m_nSamples-1) // CR 11194 //CR 14045
   {
      m_nAfterDischargeOpenIndex++;
   }
   
   // <<< CR 11194 Daanish Rumani 22 Nov 2006
   // If the required index/sample was not found.
   if(m_nAfterSuctionOpenIndex >= (m_nSamples + m_nAngleOffsetIndex) ||
      m_nAfterDischargeOpenIndex >= (m_nSamples + m_nAngleOffsetIndex)) // CR 14045
   {
      return FALSE;
   }
   else
   {
      return TRUE;
   }
   // >>> CR 11194 Daanish Rumani 22 Nov 2006
}

/**********************************************************
   Name:  CRecipPPLCalc::SetAngleOffsetIndex

   Description:  The angle offset is the amount of angle
                 that the crank has to turn between the kph
                 event and zero degrees (Top Dead Center).
                 The index for the angle offset simply gives
                 us the sample number to look at in the sample
                 array.

  Change History:

**********************************************************/
void CRecipPPLCalc::SetAngleOffsetIndex(int nPistonAngle,
                                       short nKphIndex)
{
   //init to zero since this is what it will be most of the time
   // this also avoids uninitialized problems
   m_nAngleOffsetIndex = 0;

   //make the piston angle positive
	if(nPistonAngle < 0)
	{
		nPistonAngle += 360;
	}

   //convert the angle to an index
   if (!FLOAT_EQUAL(m_dAngleInc, 0.0)) // CR 22358 - James McDuffie
   {
	   m_nAngleOffsetIndex = (int)((double)nPistonAngle / m_dAngleInc);
   }

   //add the kph index to the offset
   m_nAngleOffsetIndex += nKphIndex;

   return;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetEngValue

  Change History:

**********************************************************/
double CRecipPPLCalc::GetEngValue(int index)
{
  double dVoltage = 0;
  double dEngUnits = 0;
  
  dVoltage = m_pSamples[index] * m_fMv_per_bit;
  dVoltage = dVoltage + m_fAmpl_offset;

  if(!FLOAT_EQUAL(m_fXdcr_scale_factor, 0.0)) // CR 22358 - James McDuffie
  {
    dEngUnits = dVoltage / m_fXdcr_scale_factor;
  }
 
  dEngUnits = dEngUnits + m_fTSF_offset;

  return dEngUnits;
}



/**********************************************************
   Name:  CRecipPPLCalc::GetGasLoad

  Change History:

**********************************************************/
double CRecipPPLCalc::GetGasLoad(stRecipData recipData)
{
  double dGasLoad = 1;
  if(!recipData.bChamberCrankEnd)//HEADEND
	{
      if(!recipData.bTandemCylinderThrow)
      {
		   dGasLoad = PI * ((recipData.cylinder_bore_diameter * 
											 recipData.cylinder_bore_diameter) - 
											 (recipData.tail_diameter * 
											 recipData.tail_diameter)) /4;
      }
      else
      {
         if(recipData.bCylinderCrankEnd)
         {
            dGasLoad = PI * ((recipData.cylinder_bore_diameter * 
										recipData.cylinder_bore_diameter) -
										(recipData.piston_diameter2 * 
										recipData.piston_diameter2)) /4;
         }
         else
         {
            dGasLoad = PI * ((recipData.cylinder_bore_diameter * 
											 recipData.cylinder_bore_diameter) - 
											 (recipData.tail_diameter * 
											 recipData.tail_diameter)) /4;
         }
      }
	}
	else
	{
      if(!recipData.bTandemCylinderThrow)
      {
		   dGasLoad = PI * ((recipData.cylinder_bore_diameter * 
										recipData.cylinder_bore_diameter) -
										(recipData.piston_diameter * 
										recipData.piston_diameter)) /4;
      }
      else
      {
         if(recipData.bCylinderCrankEnd)
         {
            dGasLoad = PI * ((recipData.cylinder_bore_diameter * 
										recipData.cylinder_bore_diameter) -
										(recipData.piston_diameter * 
										recipData.piston_diameter)) /4;
         }
         else
         {
            dGasLoad = PI * ((recipData.cylinder_bore_diameter * 
										recipData.cylinder_bore_diameter) -
										(recipData.piston_diameter * 
										recipData.piston_diameter)) /4;
         }
      }
	} 
   return dGasLoad;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetVolumetricEfficiencies

   Change History:
      2 Sep 2005  Alan Sommercorn
      Added a call to GetCrankAngleAtSuctionValveClosure and
      changed the suction volumetric efficiency equation to
      use the piston position at crank angle suction closure
      instead of at zero or 180 degrees.

**********************************************************/
void CRecipPPLCalc::GetVolumetricEfficiencies(ULONG ulSpeed,
                                                long lPressureUnitType,   // CR 22189 - 28 Jan 2009 - Wayne Johnson
                                                stRecipData recipData, 
                                                stRecipTempPressureData tempPressureData, 
                                                stRecipPPLs &recipPPLs)
{
   double dAngleD0 = 0.0;
   double dAngleS0 = 0.0;
   double dPressureS1= 0.0;
   double dPressureS2 = 0.0;
   double dPressureD1 = 0.0;
   double dPressureD2 = 0.0; 
   double dPowerSuctionLoss = 0.0;
   double dPowerDischargeLoss = 0.0;
   double dConversionFactor = 0.0;
   double dPistonPosAtSuctionOpen = 0.0;
   double dPistonPosAtDischargeOpen = 0.0;
   double dPistonPosAtDischargeClose = 0.0;
   double dPistonPosAtDischargeClose180 = 0.0;

   double dRC = GetCompressionRatio(recipData,tempPressureData);
   double dL_Plus_r = 0.0;
   double dL_Minus_r = 0.0;
   double dExpansionVolume = 0.0;
   double dClearanceVolume = 0.0;

   //get pressure measurement after suction valve opened
	dPressureS1 = GetEngValue(m_nAfterSuctionOpenIndex);
   //get pressure measurement before suction valve opened
	dPressureS2 = GetEngValue(m_nAfterSuctionOpenIndex - 1);
   //get pressure measurement after discharge valve opened
   dPressureD1 = GetEngValue(m_nAfterDischargeOpenIndex);
   //get pressure measurement before discharge valve opened
   dPressureD2 = GetEngValue(m_nAfterDischargeOpenIndex - 1);

   //get angle before suction valve opened
	dAngleS0 = (m_nAfterSuctionOpenIndex - m_nAngleOffsetIndex - 1) * m_dAngleInc;
   //get angle before discharge valve opened
   dAngleD0 = (m_nAfterDischargeOpenIndex - m_nAngleOffsetIndex - 1) * m_dAngleInc;

   //Begin 1 May 2002 KJ fixed divide by zero errors cr 845
   if(!FLOAT_EQUAL(dPressureS2, dPressureS1)) // CR 22358 - James McDuffie
   {
      //add the fraction of the Angle Increment to the angle before opening to get exact angle of valve opening
      dAngleS0 += (dPressureS2 - tempPressureData.pressureSuction)/(dPressureS2 - dPressureS1) * m_dAngleInc;
   }
   if(!FLOAT_EQUAL(dPressureD1, dPressureD2)) // CR 22358 - James McDuffie
   {
      dAngleD0 += (tempPressureData.pressureDischarge - dPressureD2)/(dPressureD1 - dPressureD2) * m_dAngleInc;
   }
   //End 1 May 2002 KJ fixed divide by zero errors cr 845

   dPistonPosAtSuctionOpen = GetPistonPositionAtAngle(recipData,dAngleS0);
   dPistonPosAtDischargeOpen = GetPistonPositionAtAngle(recipData,dAngleD0);
   //this calculation assumes that discharge valve was closed at zero degrees or suction opened at 180
   dL_Plus_r = recipData.connecting_rod_length + recipData.stroke_length/2;
   //this calculation assumes that discharge valve was closed at 180 degrees or suction opened at zero
   dL_Minus_r = recipData.connecting_rod_length - recipData.stroke_length/2;


   if(!FLOAT_EQUAL(recipData.stroke_length, 0.0)) // CR 22358 - James McDuffie
   {
	   if(!recipData.bChamberCrankEnd)//Headend
	   {
		   dExpansionVolume = (dPistonPosAtSuctionOpen - dL_Minus_r) / recipData.stroke_length;
	   }
	   else
	   {
		   dExpansionVolume = (dL_Plus_r - dPistonPosAtSuctionOpen) / recipData.stroke_length;
	   }
   }

   double dKAve = recipData.kAverage;

   GetClearanceVolume(dRC, dKAve, dExpansionVolume, recipPPLs); 


   //calculate the crank angle at suction valve closure
   double dAngleAtSuctionClosure = GetCrankAngleAtSuctionValveClosure(tempPressureData.pressureSuction,  // CR 9476
                                                                      tempPressureData.pressureDischarge,
                                                                      recipData,
                                                                      recipPPLs);

   recipPPLs.suctionValveCloseAngle = dAngleAtSuctionClosure;

   double dPistonPosAtTheoreticalSuctionClose = GetPistonPositionAtAngle(recipData, recipPPLs.theoreticalSuctionValveCloseAngle);  // CR 12551

   //calculate volumetric efficiencies for suction and discharge (EVS and EVD)
   if(!FLOAT_EQUAL(recipData.stroke_length, 0.0)) // CR 22358 - James McDuffie
   {
	   if(!recipData.bChamberCrankEnd)//Headend
	   {
		   recipPPLs.suctionVolumetricEfficiency = 
			   (float)((dPistonPosAtSuctionOpen - dPistonPosAtTheoreticalSuctionClose) / recipData.stroke_length);  // CR 12551

		   recipPPLs.dischargeVolumetricEfficiency = 
			   (float)((dL_Plus_r - dPistonPosAtDischargeOpen) / recipData.stroke_length);
	   }
	   else
	   {
		   recipPPLs.suctionVolumetricEfficiency = 
			   (float)((dPistonPosAtTheoreticalSuctionClose - dPistonPosAtSuctionOpen) / recipData.stroke_length);  // CR 12551

		   recipPPLs.dischargeVolumetricEfficiency = 
			   (float)((dPistonPosAtDischargeOpen - dL_Minus_r) / recipData.stroke_length);
	   }
   }
   

   //gas power
   if( lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_ABSOLUTE || lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_GAUGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
   {
      dConversionFactor = 2.5252525 / 1000000;
   }
   else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_KGCM2_ABSOLUTE  || lPressureUnitType == BENTLY_UNIT_TYPE_KGCM2_GUAGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
   {
      dConversionFactor = 1.634441666 / 1000000000;
   }
   else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_BARS_ABSOLUTE  || lPressureUnitType == BENTLY_UNIT_TYPE_BARS_GUAGE )    // CR 21443 - 06 April 2009 - Wayne Johnson
   {
      dConversionFactor = 1.666666666 / 1000000000;
   }
   else // Pascals (SI unit)
   {
      //Start CF 314 -- Incorrect Recip Calculations in SI units -- Mike Peden - 5 Nov 03
      dConversionFactor = 1.66665692/100000000000; //16.66597/1000000;//1.14901150078 / 10000000000;
      //End CF 314
   }

   // <<<begin CR 12550
   double dGasLoad = recipPPLs.gasLoad;
   double dAngleS1 = (m_nAfterSuctionOpenIndex - m_nAngleOffsetIndex) * m_dAngleInc;
   double dAngleD1 = (m_nAfterDischargeOpenIndex - m_nAngleOffsetIndex) * m_dAngleInc;

   double dAngleAtDischargeClosure;  // angle at which the discharge valve closes
   double dDirection;
   if(recipData.bChamberCrankEnd)
   {
      dAngleAtDischargeClosure = 180;
      dDirection = 1;
   }
   else
   {
      dAngleAtDischargeClosure = 360;
      dDirection = -1;
   }

   if ( dAngleAtSuctionClosure < dAngleS1 )
   {
      dAngleAtSuctionClosure += 360;
   }

   dPowerSuctionLoss = dDirection * dGasLoad * ulSpeed * dConversionFactor *
      (tempPressureData.pressureSuction - (tempPressureData.pressureSuction + GetEngValue(m_nAfterSuctionOpenIndex))/2) * 
      (GetPistonPositionAtAngle(recipData,dAngleS1) - GetPistonPositionAtAngle(recipData,dAngleS0));
   dPowerDischargeLoss = dDirection * dGasLoad * ulSpeed * dConversionFactor *
      (tempPressureData.pressureDischarge - (tempPressureData.pressureDischarge + GetEngValue(m_nAfterDischargeOpenIndex))/2) * 
      (GetPistonPositionAtAngle(recipData,dAngleD1) - GetPistonPositionAtAngle(recipData,dAngleD0));

   int iSampleRate = 0;

   if(!FLOAT_EQUAL(m_dAngleInc,0.0)) // CR 22358 - James McDuffie
   {
	   iSampleRate = (int)(360/m_dAngleInc + 0.5);
   }

   for(double dAngleCount = dAngleS1; dAngleCount < dAngleAtSuctionClosure; dAngleCount += m_dAngleInc)
   {
	   int iAngleIndex = 0;

	   if(!FLOAT_EQUAL(m_dAngleInc,0.0)) // CR 22358 - James McDuffie
	   {   
		   iAngleIndex = (int)(dAngleCount/m_dAngleInc + 0.5) % iSampleRate + m_nAngleOffsetIndex;
	   }

	   dPowerSuctionLoss += dDirection * dGasLoad * ulSpeed * dConversionFactor *
		   (tempPressureData.pressureSuction - (GetEngValue(iAngleIndex) + GetEngValue(iAngleIndex + 1))/2) * 
		   //Start CF 314 -- Incorrect Recip Calculations in SI units -- Mike Peden - 5 Nov 03
		   (GetPistonPositionAtAngle(recipData,dAngleCount + m_dAngleInc) - GetPistonPositionAtAngle(recipData, dAngleCount));
	   //End CF 314
   }

   // volume at theoretical suction closure
   double Vtsc = GetInstantaneousVolumeAtAngle(recipData, recipPPLs, recipPPLs.theoreticalSuctionValveCloseAngle);

   // volume at suction closure
   double Vsc  = GetInstantaneousVolumeAtAngle(recipData, recipPPLs, recipPPLs.suctionValveCloseAngle);

   double dSuctionPressureAbsolute = tempPressureData.pressureSuction + recipData.ambient_pressure;
   double dOneMinusK = 1;

   if(!FLOAT_EQUAL(recipData.kAverage, 1.0)) // Prevents Divide by zero later on
   {
	   dOneMinusK = 1 - recipData.kAverage;
   }

   // this value is the C in the equation for the adiabatic expansion of an ideal gas, P * V^K = C
   double dAdiabaticExpansionConstant = dSuctionPressureAbsolute * pow(Vtsc, recipData.kAverage);

   // The area under the theoretical pressure waveform between the points of theoretical
   // and actual suction valve closure represents useful work done to compress the gas 
   // prior to suction valve closure.  Since this work is not a true loss, we subtract
   // it from the suction power loss.
   double dUsefulWork = ( dAdiabaticExpansionConstant / dOneMinusK )  *  ( pow(Vtsc, dOneMinusK) - pow(Vsc, dOneMinusK) )
      - dSuctionPressureAbsolute * (Vtsc - Vsc);

   dPowerSuctionLoss -= ulSpeed * dConversionFactor * dUsefulWork;

   for(double dAngleCount = dAngleD1; dAngleCount < dAngleAtDischargeClosure; dAngleCount += m_dAngleInc)
   {
	   int iAngleIndex = 0;

	   if(!FLOAT_EQUAL(m_dAngleInc, 0.0)) // CR 22358 - James McDuffie
	   {
		   iAngleIndex = (int)(dAngleCount/m_dAngleInc + 0.5) % iSampleRate + m_nAngleOffsetIndex;
	   }

	   dPowerDischargeLoss += dDirection * dGasLoad * ulSpeed * dConversionFactor *
		   (tempPressureData.pressureDischarge - (GetEngValue(iAngleIndex) + GetEngValue(iAngleIndex+1))/2) * 
		   //Start CF 314 -- Incorrect Recip Calculations in SI units -- Mike Peden - 5 Nov 03
		   (GetPistonPositionAtAngle(recipData,dAngleCount + m_dAngleInc) - GetPistonPositionAtAngle(recipData,dAngleCount));
	   //End CF 314

   }
   // >>>end CR 12550

   recipPPLs.powerLossDischargeCycle = (float)dPowerDischargeLoss;
   recipPPLs.powerLossSuctionCycle = (float)dPowerSuctionLoss;
   return;
}


/**********************************************************
   Name:  CRecipPPLCalc::GetPistonPositionAtAngle

  Change History:

**********************************************************/
double CRecipPPLCalc::GetPistonPositionAtAngle(stRecipData recipData, double dAngle)
{
   double dPistonPosition = 0;
   if(FLOAT_EQUAL(recipData.connecting_rod_length, 0.0)) // CR 22358 - James McDuffie
   {
     dPistonPosition = -1;
   }
   else
   {
	    dPistonPosition = recipData.stroke_length/2 * cos(dAngle * PI / 180.00) 
							+ recipData.connecting_rod_length 
							* cos(asin(recipData.stroke_length/2*sin(dAngle * PI /180.00)/recipData.connecting_rod_length));
   }
	 return dPistonPosition;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetInstantaneousVolumeAtAngle

   Change History:
   15 Nov 2006 - Ivan Johnson BNMIN
      Created for CR 12550.

**********************************************************/
double CRecipPPLCalc::GetInstantaneousVolumeAtAngle(const stRecipData& recipData,
                                                    const stRecipPPLs& recipPPLs,
                                                    double dAngle)
{
   double dL_Minus_r = recipData.connecting_rod_length - recipData.stroke_length/2;
   double dL_Plus_r = recipData.connecting_rod_length + recipData.stroke_length/2;

   double dClearancePercent = recipPPLs.theoreticalClearanceVolume;
   if(m_recipParamInfo.bUserCylinderClearance)
   {
      dClearancePercent = m_recipParamInfo.fCylinderPercentClearance/100;
   }

   double dInstVolumePercent = 0.0;
   
   //calculate instantaneous volume in percent of displaced volume %Vd(n)
   if(!FLOAT_EQUAL(recipData.stroke_length, 0.0)) // CR 22358 - James McDuffie
   {
	   if (recipData.bChamberCrankEnd)
	   {
		   // %Vd(n) = (x(n) - (L - r))/stroke
		   dInstVolumePercent = (GetPistonPositionAtAngle(recipData, dAngle) - dL_Minus_r) / recipData.stroke_length;
	   }
	   else
	   {
		   // %Vd(n) = ((L + r) - x(n))/stroke
		   dInstVolumePercent = (dL_Plus_r - GetPistonPositionAtAngle(recipData, dAngle)) / recipData.stroke_length;
	   }
   }

   //calculate the absolute volume ( Va(n) = (%Vd(n) + %Cl) * Vd )
   return (dInstVolumePercent + dClearancePercent) * m_dDisplacedVolume;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetCompressionRatio

  Change History:

**********************************************************/
double CRecipPPLCalc::GetCompressionRatio(stRecipData recipData, stRecipTempPressureData tempPressureData)
{
   double dCompressionRatio = 0;

   if(!FLOAT_EQUAL((tempPressureData.pressureSuction + recipData.ambient_pressure), 0.0)) // CR 22358 - James McDuffie
   {
	   if(true) //unit type
	   {
		   dCompressionRatio = (tempPressureData.pressureDischarge + recipData.ambient_pressure) /
			   (tempPressureData.pressureSuction + recipData.ambient_pressure);
	   }
	   /*else
	   {
	   dCompressionRatio = (tempPressureData.pressureDischarge + recipData.ambient_pressure / PRESSURE_CONVERSION) /
	   (tempPressureData.pressureSuction + recipData.ambient_pressure/ PRESSURE_CONVERSION);
	   }*/
   }

   return dCompressionRatio;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetIndicatedHorsePower

  Change History:

**********************************************************/
void CRecipPPLCalc::GetIndicatedHorsePower(ULONG ulSpeed,
                                         long lPressureUnitType,  // CR 22189 - 28 Jan 2009 - Wayne Johnson
                                         stRecipData recipData, 
                                         stRecipPPLs &recipPPLs)
{
   double dIHP= 0;
   double dSummation = 0;

   //unit type conversions
   if( lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_ABSOLUTE || lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_GAUGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
   {
      dIHP = recipPPLs.gasLoad * ulSpeed * 2.525252/1000000;
   }
   else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_KGCM2_ABSOLUTE  || lPressureUnitType == BENTLY_UNIT_TYPE_KGCM2_GUAGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
   {
      dIHP = recipPPLs.gasLoad * ulSpeed * 1.634441666 / 1000000000;
   }
   else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_BARS_ABSOLUTE   || lPressureUnitType == BENTLY_UNIT_TYPE_BARS_GUAGE )   // CR 21443 - 06 April 2009 - Wayne Johnson
   {
      dIHP = recipPPLs.gasLoad * ulSpeed * 1.666666666 / 1000000000;
   }
   else // Pascals (SI unit)
   {
      //Start CF 314 -- Incorrect Recip Calculations in SI units -- Mike Peden - 5 Nov 03
      dIHP = recipPPLs.gasLoad * ulSpeed * 1.66665692/100000000000;/*16.66597/1000000;*/ //1.14901150078/10000000000;
      //End CF 314
   }

   if(!FLOAT_EQUAL(m_dAngleInc, 0.0)) // CR 22358 - James McDuffie
   {
	   for(double dCount = 0; dCount < 180; dCount = dCount + m_dAngleInc)
	   {
		   int index = (int)(dCount / m_dAngleInc) + m_nAngleOffsetIndex;
		   int index2;
		   int index3 = (int)((360.0 - dCount)/m_dAngleInc) + m_nAngleOffsetIndex;
		   int index4;
		   int indexInc = (int)(1.0 / m_dAngleInc);
		   index2 = index + indexInc;
		   index4 = index3 - indexInc;

		   dSummation = dSummation + 
			   //Start CF 314 -- Incorrect Recip Calculations in SI units -- Mike Peden - 5 Nov 03
			   ((GetPistonPositionAtAngle(recipData,dCount) - GetPistonPositionAtAngle(recipData,dCount + m_dAngleInc)) * 
			   //End CF 314
			   ((GetEngValue(index3) + GetEngValue(index4))/2 - 
			   (GetEngValue(index) + GetEngValue(index2))/2));
	   }
   }
   recipPPLs.indicatedHorsePower = (float)(dIHP * dSummation);
   //BEGIN May 9 - Kevin Jensen - fix negative horsepower on crank end
   if(recipData.bChamberCrankEnd)
   {
     recipPPLs.indicatedHorsePower *= -1;
   }
   //BEGIN May 9 - Kevin Jensen - fix negative horsepower on crank end

   return;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetTheoreticalDischargeTemp

  Change History:

**********************************************************/
void CRecipPPLCalc::GetTheoreticalDischargeTemp(double dRC,
                                                BOOL bTempUnitF,  
                                                stRecipTempPressureData tempPressureData, 
                                                stRecipPPLs &recipPPLs)
{
  if(tempPressureData.kSuction && !FLOAT_EQUAL(tempPressureData.kSuction, 0.0)) // CR 2235 - James McDuffie
  {
    double dTempConv = 0;
    if(bTempUnitF)
    {
      dTempConv = TEMP_CONST_F;
    }
    else
    {
      dTempConv = TEMP_CONST_C;
    }
    recipPPLs.theoreticalDischargeTemp = (float)((tempPressureData.tempSuction + dTempConv) *
         pow(dRC,(tempPressureData.kSuction-1)/tempPressureData.kSuction) - dTempConv);
  }
  else
  {
    recipPPLs.theoreticalDischargeTemp = 0;
  }
   return;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetGasFlows

  Change History:

**********************************************************/
void CRecipPPLCalc::GetGasFlows(ULONG ulSpeed,
                                 long lPressureUnitType,  // CR 22189 - 28 Jan 2009 - Wayne Johnson
                                 BOOL bTempUnitF, 
                                 stRecipData recipData, 
                                 stRecipTempPressureData tempPressureData, 
                                 stRecipPPLs &recipPPLs)
{
   double dAbs_Ps = 0;
   double dAbs_Pd = 0;
   //double dTempTheoretical= 0;
   double dRC= 0;
   double dTheoreticalDischargeCap;
   double dTempConv;
   double dStdTemp = 0;
   double dDisplacedVolume = m_dDisplacedVolume;

   if(bTempUnitF)
   {
      dTempConv = TEMP_CONST_F;
      dStdTemp = 60;
   }
   else
   {
      dTempConv = TEMP_CONST_C;
      dStdTemp = 15.5555;
   }
   //unit type divide PB by 6.895
   dAbs_Ps = tempPressureData.pressureSuction + recipData.ambient_pressure;
   dAbs_Pd = tempPressureData.pressureDischarge + recipData.ambient_pressure;
   
   if(!FLOAT_EQUAL(dAbs_Ps, 0.0)) // CR 22358 - James McDuffie
   {
		dRC = dAbs_Pd/dAbs_Ps;
   }

   // pressure measurement unit type conversions
   if( lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_ABSOLUTE || lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_GAUGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
   {
      // no conversion necessary for English units
   }
   else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_KGCM2_ABSOLUTE  || lPressureUnitType == BENTLY_UNIT_TYPE_KGCM2_GUAGE || // CR 22189 - 28 Jan 2009 - Wayne Johnson
            lPressureUnitType ==  BENTLY_UNIT_TYPE_BARS_ABSOLUTE   || lPressureUnitType == BENTLY_UNIT_TYPE_BARS_GUAGE  )  // CR 21443 - 06 April 2009 - Wayne Johnson
   {
      // Converting mm^3 to m^3
      dDisplacedVolume /= 1000000000;
   }

   else // Pascals (SI unit)
   {
      // Converting mm^3 to m^3
      dDisplacedVolume /= 1000000000;
   }

   //dTempTheoretical = (tempPressureData.tempSuction + dTempConvNoSTD)*(pow(dRC,(tempPressureData.kSuction - 1)/tempPressureData.kSuction)) - dTempConvNoSTD;
   //BEGIN 1 May 2002 KJ fixed divide by zero errors cr 845
   if(tempPressureData.zSuction && (!FLOAT_EQUAL(tempPressureData.zSuction, 0.0)) && (!FLOAT_EQUAL(tempPressureData.tempSuction, 0.0))) // CR 22358 - James McDuffie
   {
      recipPPLs.capacitySuctionValve = dAbs_Ps/PSIA_CONST * (dTempConv + dStdTemp) / (dTempConv + tempPressureData.tempSuction) *
         recipPPLs.suctionVolumetricEfficiency * tempPressureData.zStandard/tempPressureData.zSuction *
         dDisplacedVolume * ulSpeed * CAPACITY_CONV;
   }

   if(tempPressureData.zDischarge && (!FLOAT_EQUAL(tempPressureData.zDischarge, 0.0)) && (!FLOAT_EQUAL(tempPressureData.tempDischarge, 0.0)))// CR 22358 - James McDuffie
   {
      recipPPLs.capacityDischargeValve = dAbs_Pd/PSIA_CONST * (dTempConv + dStdTemp) / (dTempConv + tempPressureData.tempDischarge) *
         recipPPLs.dischargeVolumetricEfficiency * tempPressureData.zStandard/tempPressureData.zDischarge *
         dDisplacedVolume * ulSpeed * CAPACITY_CONV;
      dTheoreticalDischargeCap = dAbs_Pd/PSIA_CONST * (dTempConv + dStdTemp) / (dTempConv + recipPPLs.theoreticalDischargeTemp) *
         recipPPLs.dischargeVolumetricEfficiency * tempPressureData.zStandard/tempPressureData.zDischarge *
         dDisplacedVolume * ulSpeed * CAPACITY_CONV;
   }
   //END 1 May 2002 KJ fixed divide by zero errors cr 845

   if( lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_ABSOLUTE || lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_GAUGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
   {
      // English units need no further conversions - don't need USCS specific backed out
   }
   else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_KGCM2_ABSOLUTE  || lPressureUnitType == BENTLY_UNIT_TYPE_KGCM2_GUAGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
   {
      recipPPLs.capacitySuctionValve = recipPPLs.capacitySuctionValve * PSIA_CONST / CAPACITY_CONV * 60 / 1.033227453;
      recipPPLs.capacityDischargeValve = recipPPLs.capacityDischargeValve * PSIA_CONST / CAPACITY_CONV * 60 / 1.033227453;
      dTheoreticalDischargeCap = dTheoreticalDischargeCap * PSIA_CONST / CAPACITY_CONV * 60 / 1.033227453;
   }
   else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_BARS_ABSOLUTE   || lPressureUnitType == BENTLY_UNIT_TYPE_BARS_GUAGE  ) // CR 21443 - 06 April 2009 - Wayne Johnson
   {
      recipPPLs.capacitySuctionValve = recipPPLs.capacitySuctionValve * PSIA_CONST / CAPACITY_CONV * 60 / 1.01325;   //Standard Atmosphere to Bars = 1.01325
      recipPPLs.capacityDischargeValve = recipPPLs.capacityDischargeValve * PSIA_CONST / CAPACITY_CONV * 60 / 1.01325;
      dTheoreticalDischargeCap = dTheoreticalDischargeCap * PSIA_CONST / CAPACITY_CONV * 60 / 1.01325;
   }
   else // Pascals SI Units 
   {
      recipPPLs.capacitySuctionValve = recipPPLs.capacitySuctionValve * PSIA_CONST / CAPACITY_CONV * 60 / 101.3565;
      recipPPLs.capacityDischargeValve = recipPPLs.capacityDischargeValve * PSIA_CONST / CAPACITY_CONV * 60 / 101.3565;
      dTheoreticalDischargeCap = dTheoreticalDischargeCap * PSIA_CONST / CAPACITY_CONV * 60 / 101.3565;
   }

   recipPPLs.theoreticalCapacity = (dTheoreticalDischargeCap + recipPPLs.capacitySuctionValve)/2;
   recipPPLs.medianCapacity = (recipPPLs.capacitySuctionValve + recipPPLs.capacityDischargeValve) / 2;

   //BEGIN 1 May 2002 KJ fixed divide by zero errors cr 845
   if(recipPPLs.capacityDischargeValve && (!FLOAT_EQUAL(recipPPLs.capacityDischargeValve, 0.0)))// CR 22358 - James McDuffie
   {
      recipPPLs.cylinderFlowBalance = recipPPLs.capacitySuctionValve / recipPPLs.capacityDischargeValve;
   }

   if(dTheoreticalDischargeCap && (!FLOAT_EQUAL(dTheoreticalDischargeCap, 0.0)))// CR 22358 - James McDuffie
   {
      recipPPLs.theoreticalFlowBalance = recipPPLs.capacitySuctionValve/ dTheoreticalDischargeCap;
   }

   if(recipPPLs.medianCapacity && (!FLOAT_EQUAL(recipPPLs.medianCapacity, 0.0)))// CR 22358 - James McDuffie
   {
      recipPPLs.powerToMedianCapacityRatio = recipPPLs.indicatedHorsePower / recipPPLs.medianCapacity;

      if( lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_ABSOLUTE || lPressureUnitType == BENTLY_UNIT_TYPE_POUNDS_PER_SQUARE_INCH_GAUGE ) // CR 22189 - 28 Jan 2009 - Wayne Johnson
      {
         // English units need no further conversions - don't need USCS specific backed out for Watts conversion from kWatts
      }
      else if( lPressureUnitType ==  BENTLY_UNIT_TYPE_KGCM2_ABSOLUTE  || lPressureUnitType == BENTLY_UNIT_TYPE_KGCM2_GUAGE || // CR 22189 - 28 Jan 2009 - Wayne Johnson
               lPressureUnitType ==  BENTLY_UNIT_TYPE_BARS_ABSOLUTE   || lPressureUnitType == BENTLY_UNIT_TYPE_BARS_GUAGE  )  // CR 21443 - 06 April 2009 - Wayne Johnson
      {
         recipPPLs.powerToMedianCapacityRatio = recipPPLs.powerToMedianCapacityRatio * 1000;
      }
      else 
      {
         //Start CF 314 -- Incorrect Recip Calculations in SI units -- Mike Peden - 5 Nov 03
         recipPPLs.powerToMedianCapacityRatio = recipPPLs.powerToMedianCapacityRatio * 1000;
         //End CF 314
      }
   }
   //END1 May 2002 KJ fixed divide by zero errors cr 845

   return;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetClearanceVolume

  Change History:

**********************************************************/
void CRecipPPLCalc::GetClearanceVolume(double dRC,
                                       double dKAve, 
                                       double dExpansionVolume,
                                       stRecipPPLs &recipPPLs)
{
   double dClearancePercent = 0;
   //BEGIN 1 May 2002 KJ fixed divide by zero errors cr 845
   //Begin 9 April 2003 KJ change to be clearance percent
   if(FLOAT_LESS_THAN(dRC,1.0) || 
      FLOAT_EQUAL(dRC, 1.0)    ||
      dKAve == 0.0)  //make sure we will not divide by zero
   {
      dClearancePercent = 0.01;  //1% to match what RecipPlotGroupWrapper was doing for theoretical curve calc
   }
   else
   {
      dClearancePercent = (1 - dExpansionVolume) / (pow(dRC,1/dKAve) - 1);
   }
   //End 9 April 2003 KJ change to be clearance percent
   //END 1 May 2002 KJ fixed divide by zero errors cr 845
   //Begin 9 April 2003 KJ change to be clearance percent
   recipPPLs.theoreticalClearanceVolume = dClearancePercent;
   //End 9 April 2003 KJ change to be clearance percent
   return;
}

/**********************************************************
   Name:  CRecipPPLCalc::GetAtoDCount

  Change History:

**********************************************************/
double CRecipPPLCalc::GetAtoDCount(double dEngVal)
{
  double dVoltage;
  double dAtoDCount;

  dEngVal = dEngVal - m_fTSF_offset;

  dVoltage = dEngVal * m_fXdcr_scale_factor;

  dVoltage -= m_fAmpl_offset;
  
  if(!FLOAT_EQUAL(m_fMv_per_bit, 0.0))// CR 22358 - James McDuffie
  {
	dAtoDCount = dVoltage/m_fMv_per_bit;
  }

  return dAtoDCount;
}


/**********************************************************
   Name:  CRecipPPLCalc::GetCrankAngleAtSuctionValveClosure

   Description:  This method uses the Line Method Algorithm
                 developed by Rodney Stansbury to calculate
                 the crank angle at suction valve closure.

  Change History:
      Alan Sommercorn - 13 Dec 2005 - CR 3810
      Adding the calculation of the theoretical suction 
      valve closure so that the adiabatic pressure curves
      can be calculated correctly.

   09 Jun 2006 - Ivan Johnson
      CR9476 - Replace Matlab calculations with ones based
      on Numerical Recipes book.
	  

   22 Nov 2006 - CR 11194 - Daanish Rumani
      Changed the sizes of x_values[], y_values[], logPressure[], and smoothY[].
      Assigning the sizes for these dynamically to the maximum number of samples
      possible i.e. m_nSamples. Also dynamically allocating x_2ndpass[], y_2ndpass,
      and smoothY_2ndpass[].

**********************************************************/
double CRecipPPLCalc::GetCrankAngleAtSuctionValveClosure(double dSuctionPressure,
                                                            double dDischargePressure,
                                                            stRecipData recipData,
                                                            stRecipPPLs &recipPPLs)
{
   double dAngleAtSuctionValveClosure = 0.0;  //default for crank end

   if (!recipData.bChamberCrankEnd)
   {
      dAngleAtSuctionValveClosure = 180.0;
   }

   double dClearancePercent = recipPPLs.theoreticalClearanceVolume;
   if(m_recipParamInfo.bUserCylinderClearance)
   {
      dClearancePercent = m_recipParamInfo.fCylinderPercentClearance/100;
   }

   double dClearanceVolume = m_dDisplacedVolume * dClearancePercent;
   double dLogBDC = log10(dClearanceVolume);
   double dLogTDC = log10(m_dDisplacedVolume + dClearanceVolume);

   double dInstVolumePercent = 0.0;
   double dSlope = 1.0;
   double dY_Intercept = 0.0;

   double dAngle = 0.0;
   // <<< CR 11194 Daanish Rumani 27th Dec 2006
   //double x_values[720] = {0};  //represent log10 Volume
   //double y_values[720] = {0};  //calculated from curve fit coefficients and x_values
   //double logPressure[720] = {0};  //log10 of eng values from A to D sample values
   //double smoothY[720] = {0};  // 7-point moving average of y_values  // CR 9476
   double* x_values = NULL;
   double* y_values = NULL;
   double* logPressure = NULL;
   double* smoothY = NULL;
   x_values = new double[m_nSamples];
   y_values = new double[m_nSamples];
   logPressure = new double[m_nSamples];
   smoothY = new double[m_nSamples];

   ZeroMemory(x_values, sizeof(double) * m_nSamples);
   ZeroMemory(y_values, sizeof(double) * m_nSamples);
   ZeroMemory(logPressure, sizeof(double) * m_nSamples);
   ZeroMemory(smoothY, sizeof(double) * m_nSamples);
   // >>> CR 11194 Daanish Rumani 27th Dec 2006

   double distance;   //calculated distance between half slope line to curve fit data sets
   double dHalfSlope = 1.0;
   double dC_value = 0.0;
   double dMaxDistance = 0.0;
   double dLogPressureAtSuctionClosure = 0.0; // CR 12829
   double dLogVolumeAtSuctionClosure = 0.0; // CR 12829
   int nMaxDistanceIndex = 0;

   int nSample = 0;
   int nTotalSamples = 0;
   int nCoefficient = 0;
   int nCoeffIndex = 0;
   int nNewStartIndex = 0;
   int nNewEndIndex = 0;
   int nNewStartSample = 0;
   int nNewEndSample = 0;
   double logValue = 0.0;

   double dCylinderArea = PI * recipData.cylinder_bore_diameter * recipData.cylinder_bore_diameter * 0.25;
   double dPressureBeforeDischargeOpen = GetEngValue(m_nAfterDischargeOpenIndex - 1);
   int nQuarterPressureIndex = m_nSuctionPressureIndex; //init to the index where compression begins

   //calculate the pressure at quarter of the way between suction and discharge pressures
   double dPressureAtQuarterPressure = dSuctionPressure + 0.25*(dDischargePressure - dSuctionPressure);

   //find index that matches pressure quarter of the way between suction and discharge
   double dAtoDCountQuarterPressure = GetAtoDCount(dPressureAtQuarterPressure);
   
   while (m_pSamples[nQuarterPressureIndex] < dAtoDCountQuarterPressure && 
          nQuarterPressureIndex < m_nAfterDischargeOpenIndex-1)
   {
      ++nQuarterPressureIndex;
   }

   nTotalSamples = m_nAfterDischargeOpenIndex-1 - nQuarterPressureIndex;

   // begin CR 9476
   if ( nTotalSamples < 1 )
   {
      return 0.;
   }

   //get pressure and volume values for upper part of curve
   for (nSample = 0; nSample <= nTotalSamples; ++nSample)
   {
      //set the Y value to the pressure
      logPressure[nSample] = log10(GetEngValue(nSample + nQuarterPressureIndex) + recipData.ambient_pressure);
      
      //set the X value to the volume
      // get the angle - subtract the angle offset so that we base our angle off of zero
      dAngle = (nSample + nQuarterPressureIndex - m_nAngleOffsetIndex) * m_dAngleInc;

      //calculate and store the log10 of absolute volume ( Va(n) = (%Vd(n) + %Cl) * Vd )
	  logValue = log10(GetInstantaneousVolumeAtAngle(recipData, recipPPLs, dAngle)); // CR 12550
	  
	  // CR 22358 - James McDuffie
	  if(logValue >= 0)
	  {
		x_values[nSample] = logValue;
	  }
	  else
	  {
		x_values[nSample] = 0;
	  }
   }

   //do a linear curve fit for the upper portion of the curve
   {
      int ndata = nTotalSamples+1;
      int degree = 1;  // degree of polynomial to fit
      ShiftedPolynomial polynomial;

      FitPolynomial(x_values, logPressure, ndata, degree, &polynomial);

      dSlope = polynomial(1) - polynomial(0);
      dHalfSlope = dSlope/2;
      dC_value = logPressure[nTotalSamples] - (dHalfSlope * x_values[nTotalSamples]);
      dY_Intercept = polynomial(0);
   }

   //do first-pass polynomial fit for lower portion of curve
   nTotalSamples = nQuarterPressureIndex - m_nSuctionPressureIndex;
   for (nSample = 0; nSample <= nTotalSamples; ++nSample)
   {
      //set the Y value to the smoothed pressure
      int maxDist = 3;
      double sum = 0;
      for ( int i = -maxDist ; i <= maxDist ; ++i )
      {
         sum += GetEngValue((nSample + m_nSuctionPressureIndex + i + m_nSamples) % m_nSamples);
      }

      smoothY[nSample] = log10(sum /(2 * maxDist + 1) + recipData.ambient_pressure);
      logPressure[nSample] = log10(GetEngValue(nSample + m_nSuctionPressureIndex) + recipData.ambient_pressure);

      //set the X value to the volume
      // get the angle
      dAngle = (nSample + m_nSuctionPressureIndex - m_nAngleOffsetIndex) * m_dAngleInc;

      //calculate and store the log10 of absolute volume ( Va(n) = (%Vd(n) + %Cl) * Vd )
      logValue = log10(GetInstantaneousVolumeAtAngle(recipData, recipPPLs, dAngle)); // CR 12550

	  // CR 22358 - James McDuffie
	  if(logValue >= 0)
	  {
		x_values[nSample] = logValue;
	  }
	  else
	  {
		x_values[nSample] = 0;
	  }
   }

   {
      int ndata = nTotalSamples+1;  // true number of samples
      int degree = 6;  // degree of polynomial to fit
      ShiftedPolynomial polynomial;

      FitPolynomial(x_values, smoothY, ndata, degree, &polynomial);

      //calculate the distance between each data point on the half slope line and
      //the corresponding data point on the curve fit - keep track of the max distance
      //and its index
      for (nSample = 0; nSample <= nTotalSamples; ++nSample)
      {
         distance =  ((dHalfSlope * x_values[nSample]) + 
                     (-1 * polynomial(x_values[nSample])) +
                     dC_value) /
                     sqrt(dHalfSlope * dHalfSlope + 1);

         if (nSample == 0 ||
               distance > dMaxDistance)
         {
            dMaxDistance = distance;
            nMaxDistanceIndex = nSample + m_nSuctionPressureIndex;
         }
      }
   }

   //start second pass
   nNewStartIndex = nMaxDistanceIndex - 10;
   nNewEndIndex = nMaxDistanceIndex + 10;

   if (nNewStartIndex < m_nSuctionPressureIndex)
   {
      nNewStartIndex = m_nSuctionPressureIndex;
   }
   if (nNewEndIndex > nQuarterPressureIndex)
   {
      nNewEndIndex = nQuarterPressureIndex;
   }

   nNewStartSample = nNewStartIndex - m_nSuctionPressureIndex;
   nNewEndSample = nNewEndIndex - m_nSuctionPressureIndex;

   {
      // <<< CR 11194 Daanish Rumani 27th Dec 2006
      //double x_2ndpass[21];
      //double y_2ndpass[21];
      //double smoothY_2ndpass[21];
      double* x_2ndpass = NULL;
      double* y_2ndpass = NULL;
      double* smoothY_2ndpass = NULL;
      
      if((nNewEndSample - nNewStartSample) > 0)
      {
         x_2ndpass = new double[nNewEndSample - nNewStartSample + 1];
         y_2ndpass = new double[nNewEndSample - nNewStartSample + 1];
         smoothY_2ndpass = new double[nNewEndSample - nNewStartSample + 1];
         ZeroMemory(x_2ndpass, sizeof(double) * (nNewEndSample - nNewStartSample + 1));
         ZeroMemory(y_2ndpass, sizeof(double) * (nNewEndSample - nNewStartSample + 1));
         ZeroMemory(smoothY_2ndpass, sizeof(double) * (nNewEndSample - nNewStartSample + 1));
      }
      else
      {
         x_2ndpass = new double[1];
         y_2ndpass = new double[1];
         smoothY_2ndpass = new double[1];
         x_2ndpass[0] = 0.0;
         y_2ndpass[0] = 0.0;
         smoothY_2ndpass[0] = 0.0;
      }
      // >>> CR 11194 Daanish Rumani 27th Dec 2006

      for (nSample = nNewStartSample; nSample <= nNewEndSample; ++nSample)
      {
         x_2ndpass[nSample - nNewStartSample] = x_values[nSample];
         y_2ndpass[nSample - nNewStartSample] = logPressure[nSample];
         smoothY_2ndpass[nSample - nNewStartSample] = smoothY[nSample];
      }

      int degree = 6;  // degree of polynomial to fit
      ShiftedPolynomial polynomial;

      FitPolynomial(x_2ndpass, smoothY_2ndpass, nNewEndSample - nNewStartSample + 1, degree, &polynomial);

      for (nSample = 0; nSample <= nNewEndSample - nNewStartSample; ++nSample)
      {
         distance =  ((dHalfSlope * x_2ndpass[nSample]) + 
                              (-1 * polynomial(x_2ndpass[nSample])) +
                              dC_value)/
                              sqrt(dHalfSlope * dHalfSlope + 1);

         if (nSample == 0 ||
               distance > dMaxDistance)
         {
            dMaxDistance = distance;
            nMaxDistanceIndex = nSample + nNewStartIndex;
            dLogPressureAtSuctionClosure = y_2ndpass[nSample]; // CR 12829
            dLogVolumeAtSuctionClosure = x_2ndpass[nSample]; // CR 12829
         }
      }
      
      // <<< CR 11194 Daanish Rumani 27 Dec 2006
      delete[] x_2ndpass;
      delete[] y_2ndpass;
      delete[] smoothY_2ndpass;
      // >>> CR 11194 Daanish Rumani 27 Dec 2006
   }
   // end CR 9476

   if (nMaxDistanceIndex >= m_nSuctionPressureIndex)
   {
      dAngleAtSuctionValveClosure = (nMaxDistanceIndex - m_nAngleOffsetIndex) * m_dAngleInc;
   }

   //Begin CR 3810, 12829

   // To find the point of theoretical suction valve closure, we
   // draw a line with slope KAverage passing through the point of (actual) suction valve closure.
   // The intersection of that line with the suction pressure line is the theoretical closure point.
   
   double dLogVol = 0.0;
   if(!FLOAT_EQUAL(recipData.kAverage, 0.0)) // CR 22358 - James McDuffie
   {
      dLogVol = ( dLogPressureAtSuctionClosure - log10(dSuctionPressure + recipData.ambient_pressure) ) / recipData.kAverage
      + dLogVolumeAtSuctionClosure;
   }

   recipPPLs.theoreticalSuctionValveCloseAngle = dAngleAtSuctionValveClosure;

   for (int nIndex = 0; nIndex <= nQuarterPressureIndex - m_nSuctionPressureIndex; ++nIndex)
   {
      if (x_values[nIndex] <= dLogVol)
      {
         // use interpolation to generate a fractional "index"
         double dIndex = nIndex;
         if ( nIndex > 0  && !FLOAT_EQUAL((x_values[nIndex-1] - x_values[nIndex]), 0.0))
         {
            dIndex -= ( dLogVol - x_values[nIndex] ) / ( x_values[nIndex-1] - x_values[nIndex] );
         }

         recipPPLs.theoreticalSuctionValveCloseAngle = (dIndex + m_nSuctionPressureIndex - m_nAngleOffsetIndex) * m_dAngleInc;
         break;
      }
   }
   //End CR3810, 12829

   // <<< CR 11194 Daanish Rumani 27 Dec 2006
   delete[] x_values;
   delete[] y_values;
   delete[] logPressure;
   delete[] smoothY;
   // >>> CR 11194 Daanish Rumani 27 Dec 2006
   return dAngleAtSuctionValveClosure;
}


/**********************************************************
   Name:  CRecipPPLCalc::CRecipPPLCalc

  Change History:
   01 Dec 2005 - Tim Valdepena
      Added code for CR# 3718 to log an event in the NT event log if
      MATLAB(R) fails to initialize.

   27 Feb 2006 - Tim Valdepena
      CR7606 - Added g_lReferences to track how many objects are 
      using the Matlab runtime.  This is to fix a problem where 
      closing a plot group would uninitialize the Matlab Runtime, 
      causing subsequently opened plot groups to not work properly.

   09 Jun 2006 - Ivan Johnson
      CR9476 - Rolled back previous changes as part of removing 
      Matlab from System 1.
**********************************************************/
CRecipPPLCalc::CRecipPPLCalc()
{
}

/**********************************************************
   Name:  CRecipPPLCalc::~CRecipPPLCalc

  Change History:
   07 Feb 2006 - Tim Valdepena
      Created for CR 7220

   09 Jun 2006 - Ivan Johnson
      CR9476 - Rolled back previous changes as part of removing 
      Matlab from System 1.
**********************************************************/
CRecipPPLCalc::~CRecipPPLCalc()
{
}
