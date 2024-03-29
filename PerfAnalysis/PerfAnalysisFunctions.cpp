#include "Math.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include "PerfAnalysisHarness\compexp.h"
#include "PerfAnalysisHarness\pressurecurveanalytics.h"
#include "crankslider.h"

/// <summary>
/// Retrieves the the C++ dll version number.
/// </summary>
/// <file>PerfAnalysisFunctions.cpp.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <returns>An integer representing the build number.</returns>
int	PerfShowVersion()
{
	return 1;
}

/// <summary>
/// Retrieves the the C++ dll build number.
/// </summary>
/// <file>PerfAnalysisFunctions.cpp.cpp</file>
/// <author>Brian Howard</author>
/// <date>2001</date>
/// <returns>An integer representing the build number.</returns>
int PerfShowBuild()
{
	return 0;
}

double IMMSCFD2LBHR(double MMSCFD,
					double MolarDensity,
					double MolecWeight)
{
	double SCFD;
	double Density;

	SCFD = MMSCFD * 1000000;
	Density = MolarDensity*MolecWeight;

	return (SCFD/24.00000)*Density;
}

double CIMMSCFD(double CylDisp,
				double VE,
				double PSIA,
				double PSIAStd,
				double TempAbs,
				double TempAbsStd,
				double Z,
				double ZStd)

{
	double Term01;
	double Term02;

	Term01 = CylDisp * VE * 1440 * 0.000001;
	Term02 = (PSIA/PSIAStd)*(TempAbsStd/TempAbs)*(ZStd/Z);

	return Term01 * Term02;
}



//////////////////////////////////////////////////////////////////////
// Retrieve the clearance volume
//////////////////////////////////////////////////////////////////////
bool bXGetClearanceVolumePercent( double *dIndicatedPressureCurveArrayGage_USCS,
                                            long lPressureArrayCount,
                                            bool bSteplessUnloader,
                                            bool bUseAlternativeValveClosureAlg,
                                            double dBaroPressure_USCS,
                                            double connrodlength,
                                            double stroke,
                                            double dCylinderBoreCrankEndDiameter_USCS,
                                            double dCylinderBoreHeadEndDiameter_USCS,
                                            double dPistonRodCrankEndDiameter_USCS,
                                            double dPistonRodHeadEndDiameter_USCS,
				                            double nexp,
				                            double ncomp,
				                            bool headend,                                            
                                            double *dClearanceVolumeSuction,
                                            double *dClearanceVolumeDischarge)
{
   
    // We need some help from the pressure curve analytics
    // object to derived parameters that will be consumed
    // by the compression/expansion object
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    if( !pcurveanalytics->bSetChamberEnd( headend ) )
    {
         return false;
    }
    if( !pcurveanalytics->SetStroke_USCS ( stroke ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( connrodlength ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetAmbientPressure_USCS( dBaroPressure_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dIndicatedPressureCurveArrayGage_USCS,
                                dBaroPressure_USCS,
                                lPressureArrayCount ) )
    {
        return false;
    }

    // Calculate the clearance volume based on suction conditions
    if( !pcurveanalytics->bGetClearanceVolumePercent( ( ( ncomp + nexp ) / 2.0 ), 
                                dClearanceVolumeSuction,
                                true) )
    {
        return false;
    }
    
    // Calculate the clearance volume based on discharge conditions
    if( !pcurveanalytics->bGetClearanceVolumePercent( ( ( ncomp + nexp ) / 2.0 ), 
                                dClearanceVolumeDischarge,
                                false) )
    {
        return false;
    }
    
    //release memory
    delete pcurveanalytics;

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Retrieve the clearance volume as arrays
//////////////////////////////////////////////////////////////////////
bool bXGetClearanceVolumePercentAsArrays( double *dIndicatedPressureCurveArrayGage_USCS,
                                            long lPressureArrayCount,
                                            bool bSteplessUnloader,
                                            bool bUseAlternativeValveClosureAlg,
                                            double dBaroPressure_USCS,
                                            double connrodlength,
                                            double stroke,
                                            double dCylinderBoreCrankEndDiameter_USCS,
                                            double dCylinderBoreHeadEndDiameter_USCS,
                                            double dPistonRodCrankEndDiameter_USCS,
                                            double dPistonRodHeadEndDiameter_USCS,
				                            double nexp,
				                            double ncomp,
				                            bool headend,                                            
                                            double *dClearanceVolumeSuction,
                                            double *dClearanceVolumeDischarge)
{
   
    // We need some help from the pressure curve analytics
    // object to derived parameters that will be consumed
    // by the compression/expansion object
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    if( !pcurveanalytics->bSetChamberEnd( headend ) )
    {
         return false;
    }
    if( !pcurveanalytics->SetStroke_USCS ( stroke ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( connrodlength ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetAmbientPressure_USCS( dBaroPressure_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dIndicatedPressureCurveArrayGage_USCS,
                                dBaroPressure_USCS,
                                lPressureArrayCount ) )
    {
        return false;
    }

    // Calculate the clearance volume based on suction conditions
    if( !pcurveanalytics->bGetClearanceVolumePercentAsArray( ( ( ncomp + nexp ) / 2.0 ), 
                                dClearanceVolumeSuction,
                                true) )
    {
        return false;
    }
    
    // Calculate the clearance volume based on discharge conditions
    if( !pcurveanalytics->bGetClearanceVolumePercentAsArray( ( ( ncomp + nexp ) / 2.0 ), 
                                dClearanceVolumeDischarge,
                                false) )
    {
        return false;
    }
    
    //release memory
    delete pcurveanalytics;

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Retrieve the volumetric efficiencies for a waveform
//////////////////////////////////////////////////////////////////////
bool bXGetVolumetricEfficiencies( double *dIndicatedPressureCurveArrayGage_USCS,
                                            long lPressureArrayCount,
                                            bool bSteplessUnloader,
                                            bool bUseAlternativeValveClosureAlg,
                                            double dBaroPressure_USCS,
                                            double connrodlength,
                                            double stroke,
                                            double dCylinderBoreCrankEndDiameter_USCS,
                                            double dCylinderBoreHeadEndDiameter_USCS,
                                            double dPistonRodCrankEndDiameter_USCS,
                                            double dPistonRodHeadEndDiameter_USCS,
				                            double nexp,
				                            double ncomp,
				                            bool headend,
                                            double *dVolumetricEfficiencySuction,
                                            double *dVolumetricEfficiencyDischarge)
{

    // We need some help from the pressure curve analytics
    // object to derived parameters that will be consumed
    // by the compression/expansion object
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    if( !pcurveanalytics->bSetChamberEnd( headend ) )
    {
         return false;
    }
    if( !pcurveanalytics->SetStroke_USCS ( stroke ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( connrodlength ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetAmbientPressure_USCS( dBaroPressure_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dIndicatedPressureCurveArrayGage_USCS,
                                dBaroPressure_USCS,
                                lPressureArrayCount ) )
    {
        return false;
    }
    if( !pcurveanalytics->bGetVolumetricEfficiencies( dVolumetricEfficiencySuction, dVolumetricEfficiencyDischarge) )
    {
        return false;
    }

    // Release memory
    delete pcurveanalytics;

    // Success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Retrieve the volumetric efficiencies array for a waveform
//////////////////////////////////////////////////////////////////////
bool bXGetVolumetricEfficienciesAsArray( double *dIndicatedPressureCurveArrayGage_USCS,
                                            long lPressureArrayCount,
                                            bool bSteplessUnloader,
                                            bool bUseAlternativeValveClosureAlg,
                                            double dBaroPressure_USCS,
                                            double connrodlength,
                                            double stroke,
                                            double dCylinderBoreCrankEndDiameter_USCS,
                                            double dCylinderBoreHeadEndDiameter_USCS,
                                            double dPistonRodCrankEndDiameter_USCS,
                                            double dPistonRodHeadEndDiameter_USCS,
				                            double nexp,
				                            double ncomp,
				                            bool headend,
                                            double *dVolumetricEfficiencySuctionArray,
                                            double *dVolumetricEfficiencyDischargeArray)
{

    // We need some help from the pressure curve analytics
    // object to derived parameters that will be consumed
    // by the compression/expansion object
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    if( !pcurveanalytics->bSetChamberEnd( headend ) )
    {
         return false;
    }
    if( !pcurveanalytics->SetStroke_USCS ( stroke ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( connrodlength ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetAmbientPressure_USCS( dBaroPressure_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dIndicatedPressureCurveArrayGage_USCS,
                                dBaroPressure_USCS,
                                lPressureArrayCount ) )
    {
        return false;
    }
    if( !pcurveanalytics->bGetVolumetricEfficienciesAsArray( dVolumetricEfficiencySuctionArray, dVolumetricEfficiencyDischargeArray) )
    {
        return false;
    }

    // Release memory
    delete pcurveanalytics;

    // Success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Retrieve the adiabatic curve for stepless unloaders. 
// 
// Revision
// In cpp the bool is a single byte, but the VBA Boolean is two bytes.
// The VBA code should cast the return value for this function as 
// a byte, then re-cast the byte to a VBA Boolean.
//////////////////////////////////////////////////////////////////////
bool bGetAdiabaticPressureCurveGage_USCS( double *dIndicatedPressureCurveArrayGage_USCS,
                                            double *dPressureCurveOutputArrayGage_USCS,
                                            long lPressureArrayCount,
                                            bool bSteplessUnloader,
                                            bool bUseAlternativeValveClosureAlg,
                                            double dBaroPressure_USCS,
                                            double connrodlength,
                                            double stroke,
                                            double dCylinderBoreCrankEndDiameter_USCS,
                                            double dCylinderBoreHeadEndDiameter_USCS,
                                            double dPistonRodCrankEndDiameter_USCS,
                                            double dPistonRodHeadEndDiameter_USCS,
				                            double nexp,
				                            double ncomp,
				                            bool headend )
{
    //Initialize object
	class CCompExpCurve *compexp;
    double dClearanceVolume;
    double dSuctionValveClosureAngle;
    long lPressureSuctionIndex;
    long lPressureDischargeIndex;

	//Define local variables
	compexp = new CCompExpCurve();
 
    //Transfer data to the compression/expansion object
    if( !compexp->bSetHeadEnd( headend, true ) )
    {
         return false;
    }
    if( !compexp->SetStroke_USCS ( stroke ) )
    {
        return false;
    }
    if( !compexp->SetConnRodLength_USCS( connrodlength ) )
    {
        return false;
    }
    if( !compexp->SetBoreDiameter_USCS( dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !compexp->SetRodDiameter_USCS( dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !compexp->SetnComp( ncomp ) )
    {
        return false;
    }
    if( !compexp->SetnExp( nexp ) )
    {
        return false;
    }
    if( !compexp->bSetAmbientPressure_USCS( dBaroPressure_USCS ) )
    {
        return false;
    }
    
    //we need some help from the pressure curve analytics
    //object to derived parameters that will be consumed
    //by the compression/expansion object
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    if( !pcurveanalytics->bSetChamberEnd( headend ) )
    {
         return false;
    }
    if( !pcurveanalytics->SetStroke_USCS ( stroke ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( connrodlength ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetAmbientPressure_USCS( dBaroPressure_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dIndicatedPressureCurveArrayGage_USCS,
                                dBaroPressure_USCS,
                                lPressureArrayCount ) )
    {
        return false;
    }
    if( !pcurveanalytics->bGetClearanceVolumePercent( ( ( ncomp + nexp ) / 2.0 ), 
                                &dClearanceVolume ) )
    {
        return false;
    }
    
    if( bUseAlternativeValveClosureAlg )
    {
        if( !pcurveanalytics->bGetCrankAngleAtSuctionValveClosure_Test( &dSuctionValveClosureAngle ) )
        {
            return false;
        }
    }
    else
    {
        if( !pcurveanalytics->bGetCrankAngleAtSuctionValveClosure( &dSuctionValveClosureAngle ) )
        {
            return false;
        }
    }

    if( !pcurveanalytics->bGetSuctionPressureIndex( &lPressureSuctionIndex ) )
    {
        return false;
    }
    if( !pcurveanalytics->bGetDischargePressureIndex( &lPressureDischargeIndex ) )
    {
        return false;
    }

    //release memory
    delete pcurveanalytics;


    if( !compexp->SetClearance_Percent( dClearanceVolume ) )
    {
        return false;
    }
    if( !compexp->SetPressureAbsSuction_USCS( dIndicatedPressureCurveArrayGage_USCS[lPressureSuctionIndex] + dBaroPressure_USCS ) )
    {
        return false;
    }
    if( !compexp->SetPressureAbsDischarge_USCS( dIndicatedPressureCurveArrayGage_USCS[lPressureDischargeIndex] + dBaroPressure_USCS ) )
    {
        return false;
    }

	//Make the calculation and return the value
    if( !compexp->bGetTheoreticalCompressionCurveGage_USCS( bSteplessUnloader,
                                                dSuctionValveClosureAngle,
                                                dPressureCurveOutputArrayGage_USCS,
                                                lPressureArrayCount ) )
    {
        return false;
    }

    //Free memory
	delete compexp;

    //success
    return true;
}


/// <summary>
/// Computes the absolute crank angle pressure based on compressor kinematics and gas properties.
/// </summary>
/// <file>PerfAnalysisFunctions.cpp.cpp</file>
/// <param name="connrodlength">The connecting rod length in USCS units, inches.</param>
/// <param name="stroke">The compressor stroke in USCS units, inches.</param>
/// <param name="bore">The bore diameter in USCS units, inches.</param>
/// <param name="rod">The rod diameter in USCS units, inches.</param>
/// <param name="crankangle">The crank angle in degrees.</param>
/// <param name="clearance">The clearance in percentage.</param>
/// <param name="nexp">The polytropic expansion exponent (unitless).</param>
/// <param name="ncomp">The polytropic compression exponent (unitless).</param>
/// <param name="psuct">The absolute suction pressure in USCS units, PSIA.</param>
/// <param name="pdish">The absolute discharge pressure in USCS units, PSIA.</param>
/// <param name="headend">A boolean value set to true for the head end or false for the crank end.</param>
/// <returns>
/// The calculated absolute pressure at the given crank angle in USCS units. Returns -1.00 if any step in the calculation fails.
/// </returns>
/// <remarks>
/// <para>This function utilizes the CCompExpCurve class for computation.</para>
/// <para>The function returns -1.00 for invalid inputs or calculation failures.</para>
/// </remarks>
/// <example>
/// <code>
/// double result = CCOMPEXP(5.0, 2.0, 4.0, 1.0, 90.0, 0.1, 1.3, 1.4, 14.7, 100.0, true);
/// </code>
/// </example>
/// <exception cref="std::bad_alloc">Throws if memory allocation for CCompExpCurve fails.</exception>
/// <todo>
/// Next-time-open items:
/// 1. Currently using magic number return, not best practice. Replace with enum list.
/// </todo>
double CCOMPEXP(double* connrodlength,
                    double* stroke,
                    double* bore,
                    double* rod,
                    double* crankangle,
                    double* clearance,
                    double* nexp,
                    double* ncomp,
                    double* psuct,
                    double* pdish,
                    bool* headend )
{
    // Define local variables
    // Revision, 16 Sep 2023, used heap memory to avoid stack overflow
    // and more standard library functions. 
    // Was class CCompExpCurve *compexp;
    std::unique_ptr<CCompExpCurve> compexp = std::make_unique<CCompExpCurve>();
    double d_press = 0.00;

    // Check for null pionters
    if( connrodlength == nullptr )
		return -1.00;
    if( stroke == nullptr )
        return -1.00;
    if( bore == nullptr )
        return -1.00;
    if( rod == nullptr )
        return -1.00;
    if( crankangle == nullptr )
        return -1.00;
    if( clearance == nullptr )
        return -1.00;
    if( nexp == nullptr )
        return -1.00;
    if( ncomp == nullptr )
        return -1.00;
    if( psuct == nullptr )
        return -1.00;
    if( pdish == nullptr )
        return -1.00;
    if( headend == nullptr )
        return -1.00;

     // Transfer data into the compression/expansion object. 
     // The CCompExpCurve class handles exceptions for this function
     if( !compexp->bSetHeadEnd( *headend, true ) )
          return -1.00;
     if( !compexp->SetStroke_USCS ( *stroke ) )
          return -1.00;
     if( !compexp->SetConnRodLength_USCS( *connrodlength ) )
          return -1.00;
     if( !compexp->SetBoreDiameter_USCS( *bore ) )
          return -1.00;
     if( !compexp->SetRodDiameter_USCS( *rod ) )
          return -1.00;
     if( !compexp->SetCrankAngleDegrees( *crankangle ) )
          return -1.00;
     if( !compexp->SetClearance_Percent( *clearance ) )
          return -1.00;
     if( !compexp->SetnComp( *ncomp ) )
          return -1.00;
     if( !compexp->SetnExp( *nexp ) )
          return -1.00;
     if( !compexp->SetPressureAbsSuction_USCS( *psuct ) )
          return -1.00;
     if( !compexp->SetPressureAbsDischarge_USCS( *pdish ) )
          return -1.00;

     // Make the calculation
     if( !compexp->bCalcPressure() )
          return -1.00;

     // Retrieve the data
     d_press = compexp->GetPressureAbsCrankAngle_USCS();

	// Return the value
	return d_press;

}

//return swept volume in SI units
double SweptVolume_SI(double stroke,
                        double bore,
                        double rod )
{

	//local variables
     double                   temp;

     //Initialize local variables
     temp                     = -1.00;

     //calculate the swept volume
     temp = ( stroke * ( ( ( bore * bore ) - ( rod * rod ) ) * PI * 0.25 ) );

     return temp;

}

//return swept volume in SI units
double SweptVolume_USCS(double stroke,
                        double bore,
                        double rod )
{

     //local variables
     double                   temp;

     //Initialize local variables
     temp                     = -1.00;

     //calculate the swept volume
     temp = ( stroke * ( ( ( bore * bore ) - ( rod * rod ) ) * PI * 0.25 ) );

     return temp;

}

//return the suction valve closure angle.  used for stepless unloaders
bool bSuctionValveClosureAngle_USCS( double *dPressureArrayGage_USCS,
                                        long lPressureArrayCount,
                                        bool bHeadEndChamber,
                                        double dBarametricPressure_USCS,
                                        double dkAvg,
                                        double dConnectingRodLength_USCS,
                                        double dStroke_USCS,
                                        double dCylinderBoreCrankEndDiameter_USCS,
                                        double dCylinderBoreHeadEndDiameter_USCS,
                                        double dPistonRodCrankEndDiameter_USCS,
                                        double dPistonRodHeadEndDiameter_USCS,
                                        double *dSuctionValveClosureAngle)
{
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    //load the gage pressure curve and related information into the object.
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dPressureArrayGage_USCS, dBarametricPressure_USCS, lPressureArrayCount))
    {
        return false;
    }
    if( !pcurveanalytics->bSetkAvg( dkAvg ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetChamberEnd( bHeadEndChamber ) )
    {
        return false;
    }

    //set mechancial parameters
    if( !pcurveanalytics->SetStroke_USCS( dStroke_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( dConnectingRodLength_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }

    //Retrieve the suction valve closure point
    if( !pcurveanalytics->bGetCrankAngleAtSuctionValveClosure( dSuctionValveClosureAngle ) )
    {
        return false;
    }

    //release memory
    delete pcurveanalytics;

    return true;
}

//return the suction valve closure angle.  used for stepless unloaders
bool bSuctionValveClosureAngle_Test_USCS( double *dPressureArrayGage_USCS,
                                        long lPressureArrayCount,
                                        bool bHeadEndChamber,
                                        double dBarametricPressure_USCS,
                                        double dkAvg,
                                        double dConnectingRodLength_USCS,
                                        double dStroke_USCS,
                                        double dCylinderBoreCrankEndDiameter_USCS,
                                        double dCylinderBoreHeadEndDiameter_USCS,
                                        double dPistonRodCrankEndDiameter_USCS,
                                        double dPistonRodHeadEndDiameter_USCS,
                                        double *dSuctionValveClosureAngle)
{
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    //load the gage pressure curve and related information into the object.
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dPressureArrayGage_USCS, dBarametricPressure_USCS, lPressureArrayCount))
    {
        return false;
    }
    if( !pcurveanalytics->bSetkAvg( dkAvg ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetChamberEnd( bHeadEndChamber ) )
    {
        return false;
    }

    //set mechancial parameters
    if( !pcurveanalytics->SetStroke_USCS( dStroke_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( dConnectingRodLength_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }

    //Retrieve the suction valve closure point
    if( !pcurveanalytics->bGetCrankAngleAtSuctionValveClosure_Test( dSuctionValveClosureAngle ) )
    {
        return false;
    }

    //release memory
    delete pcurveanalytics;

    return true;
}

// Revision
// 
// A generic function was needed to plot any waveform
// versus percent displaced volume.
// B. Howard
// 31 October 2012
bool bGetDisplacedVolumeArray_Percent( double *dDisplacedVolumeArray_Percent,
                                        long lSamplesPerRev,
                                        double dConnectingRodLength_USCS,
                                        double dStroke_USCS)
{

   
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();
    bool bHeadEndChamber;
    long lIndex;
    double dPistonPosition_USCS;

    // Load the gage pressure curve and related information into the object.
    if( !pcurveanalytics->bSetPressureCurveSampleCount( lSamplesPerRev ) )
    {
        delete pcurveanalytics;
        return false;
    }

    // Set mechancial parameters.
    if( !pcurveanalytics->SetStroke_USCS( dStroke_USCS ) )
    {
        delete pcurveanalytics;
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( dConnectingRodLength_USCS ) )
    {
        delete pcurveanalytics;
        return false;
    }

    // Calculate percent displacemet (which is the same
    // as percent displaced volume) values
    bHeadEndChamber = true;
    dPistonPosition_USCS = 0;
    for (lIndex = 0; lIndex < lSamplesPerRev ; ++lIndex)
    {

        //set the X value to the volume
        if( !pcurveanalytics->bGetPistonPositionAtIndex_USCS( lIndex, &dPistonPosition_USCS ) )
        {
        delete pcurveanalytics;
            return false;
        }
	    dDisplacedVolumeArray_Percent[lIndex] = ( dPistonPosition_USCS / dStroke_USCS ); 
	  
   }

    //release memory
    delete pcurveanalytics;

    // Success!
    return true;
}


//return the displaced volume array in absolute terms
bool bGetDisplacedVolumeArray_USCS( double *dPressureArrayGage_USCS,
                                        double *dDisplacedVolumeArray_USCS,
                                        long lArrayCount,
                                        bool bHeadEndChamber,
                                        double dBarametricPressure_USCS,
                                        double dkAvg,
                                        double dConnectingRodLength_USCS,
                                        double dStroke_USCS,
                                        double dCylinderBoreCrankEndDiameter_USCS,
                                        double dCylinderBoreHeadEndDiameter_USCS,
                                        double dPistonRodCrankEndDiameter_USCS,
                                        double dPistonRodHeadEndDiameter_USCS )
{
    class CPressureCurveAnalytics *pcurveanalytics;
    pcurveanalytics	= new CPressureCurveAnalytics();

    //load the gage pressure curve and related information into the object.
    if( !pcurveanalytics->bLoadPressureCurveGage_USCS( dPressureArrayGage_USCS, dBarametricPressure_USCS, lArrayCount))
    {
        return false;
    }
    if( !pcurveanalytics->bSetkAvg( dkAvg ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetChamberEnd( bHeadEndChamber ) )
    {
        return false;
    }

    //set mechancial parameters
    if( !pcurveanalytics->SetStroke_USCS( dStroke_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->SetConnRodLength_USCS( dConnectingRodLength_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreCrankEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetCylinderBoreHeadEndDiameter_USCS( 1, dCylinderBoreCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodCrankEndDiameter_USCS( 1, dPistonRodCrankEndDiameter_USCS ) )
    {
        return false;
    }
    if( !pcurveanalytics->bSetPistonRodHeadEndDiameter_USCS( 1, dPistonRodHeadEndDiameter_USCS ) )
    {
        return false;
    }

    //retrieve the displaced volume curve
    if( !pcurveanalytics->bGetDisplacedVolumeArray( dDisplacedVolumeArray_USCS, lArrayCount ) )
    {
        return false;
    }

    //release memory
    delete pcurveanalytics;

    return true;
}