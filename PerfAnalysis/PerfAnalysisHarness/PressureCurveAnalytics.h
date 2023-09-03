//////////////////////////////////////////////////////////////////////
// Interface for the pressure curve analytics section
// 
// This object provides the basic analytics for analysis of the 
// cylinder pressure curve.
//
// 13 July 2010
// Brian Howard
//////////////////////////////////////////////////////////////////////


#pragma once

#include "windows.h"
#include <assert.h>
#include <iterator>
#include <numeric>
#include <complex>
#include "units.h"
#include "crankslider.h"
#include "polynomial.h"
#include "nr.h"
using namespace std;

#define MAXPRESSUREARRAYSIZE 32768
#define MAX_EPSILON 0.00000001

template<typename value_type>
value_type mean(const value_type* first, const value_type* last) 
{
   double sum = std::accumulate( first, last, value_type() );
   int count = std::distance(first, last);
   assert( count != 0 );
   return sum / count;
}

class CPressureCurveAnalytics : public CCrankSlider
{
public:
    //////////////////////////////////////////////////////////////////////
    // members

    //////////////////////////////////////////////////////////////////////
    // methods
    CPressureCurveAnalytics(void);
    ~CPressureCurveAnalytics(void);

    bool bLoadPressureCurveGage_USCS( double *dDataInArray_USCS,
        double dLocalBarometricPressure_USCS,
        long lArrayCount );
    bool bLoadPressureCurveGage_SI( double *dDataInArray, 
        double dLocalBarometricPressure_USCS,
        long lArrayCount ) {return false;}

    bool bGetPressureCurveGage_USCS( double *dDataOutArray, 
        long lArrayMax );

    bool bGetPressureCurveGage_SI( double *dDataOutArray, 
        long lArrayMax ) {return false;}

    bool bSetPressureCurveSampleCount( long lArrayCount );
    bool bGetPressureCurveSampleCount( long *lSampleCount );

    bool bSetChamberEnd( bool bHeadEndChamber );
    bool bGetChamberEnd( bool *bHendEndChamber );

    bool bGetVolumetricEfficiencies( double *dVolumetricEfficiency );
    bool bGetVolumetricEfficiencies( double *dVolumetricEfficiencySuction,
                                        double *dVolumetricEfficiencyDischarge);
    bool bGetVolumetricEfficienciesAsArray( double *dVolumetricEfficiencySuctionArray,
                                        double *dVolumetricEfficiencyDischargeArray);

    bool bGetCrankAngleAtSuctionValveClosure( double *dSuctionValveCloseAngle );
    bool bGetCrankAngleAtSuctionValveClosure_Test( double *dSuctionValveCloseAngle );

    bool bGetClearanceVolumePercent( double *dClearancePercent );
    bool bGetClearanceVolumePercent(double dKAve, 
                                double *dClearanceVolume );
    bool bGetClearanceVolumePercent(double dKAve, 
                                double *dClearanceVolume,
                                bool bUseSuctionConditions);
    bool bGetClearanceVolumePercentAsArray(double dKAve, 
                                double *dClearanceVolume,
                                bool bUseSuctionConditions);

    bool bGetCompressionRatio( double *dCompressionRatio );
    bool bSetBarometricPressure_USCS( double *dInputBaroPressure );
    bool bSetBarometricPressure_SI( double *dInputBaroPressure ) {return false; }
    bool bGetBarometricPressure_USCS( double *dOutputBaroPressure );
    bool bGetBarometricPressure_SI( double *dOutputBaroPressure ) {return false; }
    bool bGetkAvg( double *dkAvgReturn );
    bool bSetkAvg( double dkAvgInput );
    bool bGetDischargeValveOpenIndex( long *lIndex );
    bool bGetDischargePressureIndex( long *lIndex );
    bool bGetPistonPositionAtIndex_USCS( long lIndex, double *dPistonPosition );
    bool bGetSuctionPressureIndex( long *lIndex );
    bool bGetAtoDCount( double *dPressureGage_USCS );
    
    bool bGetDisplacedVolumeArray( double *dDisplacedVolumeArray, long lMaxDisplacedVolumeCount );
    
    void CPressureCurveAnalytics::FitPolynomial(const double x[], const double y[], size_t ndata, int degree, ShiftedPolynomial* pResult);

    bool bCalcCrankAngleFromIndex ( long lIndex, double *dCrankAngle );

private:

    //////////////////////////////////////////////////////////////////////
    // members
    bool bHeadEndChamber;
    double dClearanceVolumePercent;
    double dBarometricPressure_USCS;
    long lPressureCurveSampleCount;
    long lDischargePressureIndex;
    long lSuctionPressureIndex;
    long lSuctionValveOpenIndex;
    long lDischargeValveOpenIndex;
    double dKAvg;
    double dPressureCurveGage_USCS[MAXPRESSUREARRAYSIZE];
    double dPressureCurveBackupGage_USCS[MAXPRESSUREARRAYSIZE];
    double dTheoreticalSuctionValveCloseAngle;

    //////////////////////////////////////////////////////////////////////
    // methods
    bool bSetIndexValues();
    bool bGetChamberDisplacementAtIndex_USCS( long lIndex, double *dPistonPosition, bool bHeadEnd );
    bool FLOAT_EQUAL( double dA, double dB);

};
