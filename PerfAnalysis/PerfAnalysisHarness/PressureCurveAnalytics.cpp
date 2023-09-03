#include "PressureCurveAnalytics.h"


CPressureCurveAnalytics::CPressureCurveAnalytics(void)
{
    //initialize the pressure array
    long lTemp = 0;
    while( lTemp < MAXPRESSUREARRAYSIZE )
    {
        dPressureCurveGage_USCS[lTemp] = 0.00;
        lTemp++;
    }
    lPressureCurveSampleCount = 360;
    lSuctionValveOpenIndex = 0;
    bHeadEndChamber = true;
    dClearanceVolumePercent = 0.20;
    dKAvg = 1.10;
    dBarometricPressure_USCS = 14.696;
}

CPressureCurveAnalytics::~CPressureCurveAnalytics(void)
{
}

///////////////////////////////////////////////////////////////////////
// Calculate the crank angle from the index
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bCalcCrankAngleFromIndex ( long lIndex, double *dCrankAngle )
{
    *dCrankAngle = ( 360.0 / (double)lPressureCurveSampleCount );
    *dCrankAngle = ( (double)lIndex * *dCrankAngle );
    return true;
}

///////////////////////////////////////////////////////////////////////
// Is this the head end chamber?
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bSetChamberEnd(bool bHeadEndChamber)
{
    CPressureCurveAnalytics::bHeadEndChamber = bHeadEndChamber;
    return true;
}

///////////////////////////////////////////////////////////////////////
// What chamber is this?
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetChamberEnd(bool *bHeadEndChamber)
{
    *bHeadEndChamber = CPressureCurveAnalytics::bHeadEndChamber;
    return true;
}

///////////////////////////////////////////////////////////////////////
// For a given quarter pressure value, find the closest index.
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Return the indicated suction valve closure point.
//////////////////////////////////////////////////////////////////////
//
// Revision
// 12 July 2015
// Changes to improve code operation: no longer need actual radius
// and stroke.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetCrankAngleAtSuctionValveClosure(double *dSuctionValveCloseAngle)
{

    double dClearanceVolume_USCS;
    double dClearanceVolumePercent;
    double dBoreDiameter_USCS;
    double dPistonRodDiameter_USCS;
    double dDisplacedVolume_USCS;
    double dAngleAtSuctionValveClosure;
    double dPistonPosition_USCS;
    long lBeforeDischargeValveOpenIndex;
    long lIndexAtDischargePressure;
    long lIndexAtSuctionPressure;
    //initialize arrays
    double x_values[MAXPRESSUREARRAYSIZE];
    double logPressure[MAXPRESSUREARRAYSIZE];
    double smoothY[MAXPRESSUREARRAYSIZE];

    //Update index
    if( !CPressureCurveAnalytics::bSetIndexValues() )
    {
        return false;
    }

    //default for crank end chamber, adjust if configured
    //for the head end chamber
    if (bHeadEndChamber)
    {
        dAngleAtSuctionValveClosure = 180.0;
        if( !bGetCylinderBoreHeadEndDiameter_USCS( 1, &dBoreDiameter_USCS ) )
        {
            return false;
        }
        if( !bGetPistonRodHeadEndDiameter_USCS( 1, &dPistonRodDiameter_USCS ) )
        {
            return false;
        }
    }
    else
    {
        double dAngleAtSuctionValveClosure = 0.0;  
        if( !bGetCylinderBoreCrankEndDiameter_USCS( 1, &dBoreDiameter_USCS ) )
        {
            return false;
        }
        if( !bGetPistonRodCrankEndDiameter_USCS( 1, &dPistonRodDiameter_USCS ) )
        {
            return false;
        }
    }

    // Retrieve the clearance volume
    dDisplacedVolume_USCS = GetDisplacedVolume_USCS( dBoreDiameter_USCS, dPistonRodDiameter_USCS );
    if( !bGetClearanceVolumePercent( &dClearanceVolumePercent ) )
    {
        return false;
    }
    dClearanceVolume_USCS = ( dClearanceVolumePercent * dDisplacedVolume_USCS );
    double dLogBDC = log10( dClearanceVolume_USCS );
    double dLogTDC = log10( dDisplacedVolume_USCS + dClearanceVolume_USCS);
    
    //initialize first order curve fit variables
    double dInstVolumePercent = 0.0;
    double dSlope = 1.0;

    //calculated distance between half slope line to curve fit data sets
    double distance;
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

    //Retrieve the pressure just before the discharge valve opens
    if( !bGetDischargeValveOpenIndex( &lBeforeDischargeValveOpenIndex ) )
    {
        return false;
    }
    if( lBeforeDischargeValveOpenIndex > 0 ) 
    {
        lBeforeDischargeValveOpenIndex--;
    }
    else
    {
        lBeforeDischargeValveOpenIndex = ( lPressureCurveSampleCount - 1 );
    }
    double dPressureBeforeDischargeOpen = dPressureCurveGage_USCS[lBeforeDischargeValveOpenIndex];

    //locate suction and discharge pressure index points
    if( !bGetDischargePressureIndex( &lIndexAtDischargePressure ) )
    {
        return false;
    }
    if( !bGetSuctionPressureIndex( &lIndexAtSuctionPressure ) )
    {
        return false;
    }

    //init to the index where compression begins
    long lHalfPressureIndex = lIndexAtSuctionPressure;

    //calculate the pressure half of the way between suction and discharge pressures
    double dPressureAtHalfPressure = dPressureCurveGage_USCS[lIndexAtSuctionPressure] 
            + 0.5*(dPressureCurveGage_USCS[lIndexAtDischargePressure] - dPressureCurveGage_USCS[lIndexAtSuctionPressure]);

    //find index that matches pressure quarter of the way between suction and discharge
    while ( dPressureCurveGage_USCS[lHalfPressureIndex] < dPressureAtHalfPressure && 
        lHalfPressureIndex < lBeforeDischargeValveOpenIndex )
    {
        ++lHalfPressureIndex;
    }
    
    //on cylinder pressure curves with very low ratios (i.e. operating unloaded), the quarter
    //pressure point may occur after the discharge valve open index.  
    nTotalSamples = ( ( lBeforeDischargeValveOpenIndex -1 ) - lHalfPressureIndex );
    if ( nTotalSamples < 1 )
    {
        if( bHeadEndChamber )
        {
            bCalcCrankAngleFromIndex( ( lPressureCurveSampleCount / 2 ) , dSuctionValveCloseAngle);
        }
        else
        {
            *dSuctionValveCloseAngle = 0.0;
        }
        return true;
    }
 
    //get pressure and volume values for upper part of curve
    for (nSample = 0; nSample <= nTotalSamples; ++nSample)
    {
      //set the Y value to the pressure
      logPressure[nSample] = log10(dPressureCurveGage_USCS[nSample + lHalfPressureIndex] + dBarometricPressure_USCS);
      
      //set the X value to the volume
      //calculate and store the log10 of absolute volume ( Va(n) = (%Vd(n) + %Cl) * Vd )
      if( !bGetChamberDisplacementAtIndex_USCS( (nSample + lHalfPressureIndex), &dPistonPosition_USCS, bHeadEndChamber ) )
      {
          return false;
      }
	  logValue = log10( dPistonPosition_USCS + dClearanceVolumePercent ); 
	  
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
    }

    //do first-pass polynomial fit for lower portion of curve
    nTotalSamples = lHalfPressureIndex - lIndexAtSuctionPressure;
    for (nSample = 0; nSample <= nTotalSamples; ++nSample)
    {
        //set the Y value to the smoothed pressure
        int maxDist = 3;
        double sum = 0;
        for ( int i = -maxDist ; i <= maxDist ; ++i )
        {
            sum += dPressureCurveGage_USCS[(nSample + lIndexAtSuctionPressure + i + lPressureCurveSampleCount) % lPressureCurveSampleCount];
        }

        smoothY[nSample] = log10(sum /(2 * maxDist + 1) + dBarometricPressure_USCS);
        logPressure[nSample] = log10(dPressureCurveGage_USCS[(nSample + lIndexAtSuctionPressure)] + dBarometricPressure_USCS );

        //set the X value to the volume
        if( !bGetChamberDisplacementAtIndex_USCS( (nSample + lIndexAtSuctionPressure), &dPistonPosition_USCS, bHeadEndChamber ) )
        {
          return false;
        }
        // Calculate and store the log10 of the percent absolute volume Va(n) = (%Vd(n) + %Cl)
        logValue = log10(dPistonPosition_USCS + dClearanceVolumePercent);

	    // CR 22358 - James McDuffie
	    if(logValue >= -2)
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
                nMaxDistanceIndex = nSample + lIndexAtSuctionPressure;
            }
        }
    }

    //start second pass
    nNewStartIndex = nMaxDistanceIndex - 10;
    nNewEndIndex = nMaxDistanceIndex + 10;

    if (nNewStartIndex < lIndexAtSuctionPressure)
    {
        nNewStartIndex = lIndexAtSuctionPressure;
    }
    if (nNewEndIndex > lHalfPressureIndex)
    {
        nNewEndIndex = lHalfPressureIndex;
    }

    nNewStartSample = nNewStartIndex - lIndexAtSuctionPressure;
    nNewEndSample = nNewEndIndex - lIndexAtSuctionPressure;

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
    if ( nMaxDistanceIndex >= lIndexAtSuctionPressure )
    {
        if( !bCalcCrankAngleFromIndex( nMaxDistanceIndex, &dAngleAtSuctionValveClosure ) )
        {
            return false;
        }
    }

    //Begin CR 3810, 12829

    // To find the point of theoretical suction valve closure, we
    // draw a line with slope KAverage passing through the point of (actual) suction valve closure.
    // The intersection of that line with the suction pressure line is the theoretical closure point.
   
    double dLogVol = 0.0;
    if(!FLOAT_EQUAL(dKAvg, 0.0)) // CR 22358 - James McDuffie
    {
        dLogVol = ( dLogPressureAtSuctionClosure - log10(dPressureCurveGage_USCS[lIndexAtSuctionPressure] + dBarometricPressure_USCS ) ) / dKAvg
                    + dLogVolumeAtSuctionClosure;
    }

    dTheoreticalSuctionValveCloseAngle = dAngleAtSuctionValveClosure;

    for (int nIndex = 0; nIndex <= lHalfPressureIndex - lIndexAtSuctionPressure; ++nIndex)
    {
        if (x_values[nIndex] <= dLogVol)
        {
            // use interpolation to generate a fractional "index"
            double dIndex = nIndex;
            if ( nIndex > 0  && !FLOAT_EQUAL((x_values[nIndex-1] - x_values[nIndex]), 0.0))
            {
                dIndex -= ( dLogVol - x_values[nIndex] ) / ( x_values[nIndex-1] - x_values[nIndex] );
            }

            if( !bCalcCrankAngleFromIndex( ( (long)dIndex + lIndexAtSuctionPressure ), &dAngleAtSuctionValveClosure ) )
            {
                return false;
            }
            break;
        }
    }
    //End CR3810, 12829
    *dSuctionValveCloseAngle = dAngleAtSuctionValveClosure;

    //success
    return true;
}

///////////////////////////////////////////////////////////////////////
// Return the indicated suction valve closure point using proof of
// concept ideas.  Not a production method.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetCrankAngleAtSuctionValveClosure_Test(double *dSuctionValveCloseAngle)
{

    double dClearanceVolume_USCS;
    double dClearanceVolumePercent;
    double dBoreDiameter_USCS;
    double dPistonRodDiameter_USCS;
    double dDisplacedVolume_USCS;
    double dAngleAtSuctionValveClosure;
    double dCompressionRatio;
    double dPistonPosition_USCS;
    double dX1;
    double dY1;
    long lBeforeDischargeValveOpenIndex;
    long lIndexAtDischargePressure;
    long lIndexAtSuctionPressure;
    
    //initialize arrays
    double x_values[MAXPRESSUREARRAYSIZE];
    double logPressure[MAXPRESSUREARRAYSIZE];
    double smoothY[MAXPRESSUREARRAYSIZE];

    //Update index
    if( !CPressureCurveAnalytics::bSetIndexValues() )
    {
        return false;
    }

    //default for crank end chamber, adjust if configured
    //for the head end chamber
    if (bHeadEndChamber)
    {
        dAngleAtSuctionValveClosure = 180.0;
        if( !bGetCylinderBoreHeadEndDiameter_USCS( 1, &dBoreDiameter_USCS ) )
        {
            return false;
        }
        if( !bGetPistonRodHeadEndDiameter_USCS( 1, &dPistonRodDiameter_USCS ) )
        {
            return false;
        }
    }
    else
    {
        double dAngleAtSuctionValveClosure = 0.0;  
        if( !bGetCylinderBoreCrankEndDiameter_USCS( 1, &dBoreDiameter_USCS ) )
        {
            return false;
        }
        if( !bGetPistonRodCrankEndDiameter_USCS( 1, &dPistonRodDiameter_USCS ) )
        {
            return false;
        }
    }

    //retrieve the clearance volume
    dDisplacedVolume_USCS = GetDisplacedVolume_USCS( dBoreDiameter_USCS, dPistonRodDiameter_USCS );
    if( !bGetClearanceVolumePercent( &dClearanceVolumePercent ) )
    {
        return false;
    }
    dClearanceVolume_USCS = ( dClearanceVolumePercent * dDisplacedVolume_USCS );
    double dLogBDC = log10( dClearanceVolume_USCS );
    double dLogTDC = log10( dDisplacedVolume_USCS + dClearanceVolume_USCS);
    
    //initialize first order curve fit variables
    double dInstVolumePercent = 0.0;
    double dSlope = 1.0;

    //calculated distance between half slope line to curve fit data sets
    double distance;
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

    //Retrieve the pressure just before the discharge valve opens
    if( !bGetDischargeValveOpenIndex( &lBeforeDischargeValveOpenIndex ) )
    {
        return false;
    }
    if( lBeforeDischargeValveOpenIndex > 0 ) 
    {
        lBeforeDischargeValveOpenIndex--;
    }
    else
    {
        lBeforeDischargeValveOpenIndex = ( lPressureCurveSampleCount - 1 );
    }
    double dPressureBeforeDischargeOpen = dPressureCurveGage_USCS[lBeforeDischargeValveOpenIndex];

    //locate suction and discharge pressure index points
    if( !bGetDischargePressureIndex( &lIndexAtDischargePressure ) )
    {
        return false;
    }
    if( !bGetSuctionPressureIndex( &lIndexAtSuctionPressure ) )
    {
        return false;
    }

    //init to the index where compression begins
    long lQuarterPressureIndex = lIndexAtSuctionPressure;

    //calculate the pressure at quarter of the way between suction and discharge pressures
    double dPressureAtQuarterPressure = dPressureCurveGage_USCS[lIndexAtSuctionPressure] 
            + 0.50*(dPressureCurveGage_USCS[lIndexAtDischargePressure] - dPressureCurveGage_USCS[lIndexAtSuctionPressure]);

    //find index that matches pressure quarter of the way between suction and discharge
    while ( dPressureCurveGage_USCS[lQuarterPressureIndex] < dPressureAtQuarterPressure && 
        lQuarterPressureIndex < lBeforeDischargeValveOpenIndex )
    {
        ++lQuarterPressureIndex;
    }
    
    //on cylinder pressure curves with very low ratios (i.e. operating unloaded), the quarter
    //pressure point may occur after the discharge valve open index.  
    nTotalSamples = ( ( lBeforeDischargeValveOpenIndex -1 ) - lQuarterPressureIndex );
    if ( nTotalSamples < 1 )
    {
        if( bHeadEndChamber )
        {
            bCalcCrankAngleFromIndex( ( lPressureCurveSampleCount / 2 ) , dSuctionValveCloseAngle);
        }
        else
        {
            *dSuctionValveCloseAngle = 0.0;
        }
        return true;
    }
 
    //get stroke radius
    double dStrokeRadius = Getr_USCS();
    if( dStrokeRadius <= 0 )
    {
        return false;
    }

    //get pressure and volume values for upper part of curve
    for (nSample = 0; nSample <= nTotalSamples; ++nSample)
    {
      //set the Y value to the pressure
      logPressure[nSample] = log10(dPressureCurveGage_USCS[nSample + lQuarterPressureIndex] + dBarometricPressure_USCS);
      
      //set the X value to the volume
      //calculate and store the log10 of absolute volume ( Va(n) = (%Vd(n) + %Cl) * Vd )
      if( !bGetChamberDisplacementAtIndex_USCS( (nSample + lQuarterPressureIndex), &dPistonPosition_USCS, bHeadEndChamber ) )
      {
          return false;
      }
	  logValue = log10( ( ( dPistonPosition_USCS / ( dStrokeRadius * 2 ) ) + dClearanceVolumePercent ) * dDisplacedVolume_USCS ); 
	  
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
        //dHalfSlope = dSlope/2;
        //dC_value = logPressure[nTotalSamples] - (dHalfSlope * x_values[nTotalSamples]);
 
        //test code set #1 below this marker ->
        dHalfSlope = -1.0;
        dC_value = ( - dHalfSlope * log10( ( dClearanceVolume_USCS + dDisplacedVolume_USCS )* 1.1 ) );
        dC_value = ( dC_value + ( log10( dPressureCurveGage_USCS[lIndexAtSuctionPressure] + dBarometricPressure_USCS) ) );
        //<- end test code set #1

        //test code set #2 below this marker ->
        if( !bGetCompressionRatio( &dCompressionRatio ) )
        {
            return false;
        }
        dHalfSlope = dSlope * ( ( 0.125 * dCompressionRatio ) + 0.375 );
        dC_value = logPressure[nTotalSamples] - (dHalfSlope * x_values[nTotalSamples]);
        //dC_value = ( - dHalfSlope * log10( ( dClearanceVolume_USCS + dDisplacedVolume_USCS )* 1.1 ) );
        //dC_value = ( dC_value + ( log10( dPressureCurveGage_USCS[lIndexAtSuctionPressure] + dBarometricPressure_USCS) ) );
        //<- end test code set #2

    }

    //do first-pass polynomial fit for lower portion of curve
    nTotalSamples = lQuarterPressureIndex - lIndexAtSuctionPressure;
    for (nSample = 0; nSample <= nTotalSamples; ++nSample)
    {
        //set the Y value to the smoothed pressure
        int maxDist = 3;
        double sum = 0;
        for ( int i = -maxDist ; i <= maxDist ; ++i )
        {
            sum += dPressureCurveGage_USCS[(nSample + lIndexAtSuctionPressure + i + lPressureCurveSampleCount) % lPressureCurveSampleCount];
        }

        smoothY[nSample] = log10(sum /(2 * maxDist + 1) + dBarometricPressure_USCS);
        logPressure[nSample] = log10(dPressureCurveGage_USCS[(nSample + lIndexAtSuctionPressure)] + dBarometricPressure_USCS );

        //set the X value to the volume
        if( !bGetChamberDisplacementAtIndex_USCS( (nSample + lIndexAtSuctionPressure), &dPistonPosition_USCS, bHeadEndChamber ) )
        {
          return false;
        }
        //calculate and store the log10 of absolute volume ( Va(n) = (%Vd(n) + %Cl) * Vd )
        logValue = log10( ( ( dPistonPosition_USCS / ( dStrokeRadius * 2 ) ) + dClearanceVolumePercent ) * dDisplacedVolume_USCS ); 

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
            dX1 = x_values[nSample];
            dY1 = polynomial(x_values[nSample]);
            distance =  ((dHalfSlope * x_values[nSample]) + 
                     (-1 * polynomial(x_values[nSample])) +
                     dC_value) /
                     sqrt(dHalfSlope * dHalfSlope + 1);

            if (nSample == 0 ||
               distance > dMaxDistance)
            {
                dMaxDistance = distance;
                nMaxDistanceIndex = nSample + lIndexAtSuctionPressure;
            }
        }
    }

    //start second pass
    nNewStartIndex = nMaxDistanceIndex - 10;
    nNewEndIndex = nMaxDistanceIndex + 10;

    if (nNewStartIndex < lIndexAtSuctionPressure)
    {
        nNewStartIndex = lIndexAtSuctionPressure;
    }
    if (nNewEndIndex > lQuarterPressureIndex)
    {
        nNewEndIndex = lQuarterPressureIndex;
    }

    nNewStartSample = nNewStartIndex - lIndexAtSuctionPressure;
    nNewEndSample = nNewEndIndex - lIndexAtSuctionPressure;

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
    if ( nMaxDistanceIndex >= lIndexAtSuctionPressure )
    {
        if( !bCalcCrankAngleFromIndex( nMaxDistanceIndex, &dAngleAtSuctionValveClosure ) )
        {
            return false;
        }
    }

    //Begin CR 3810, 12829

    // To find the point of theoretical suction valve closure, we
    // draw a line with slope KAverage passing through the point of (actual) suction valve closure.
    // The intersection of that line with the suction pressure line is the theoretical closure point.
   
    double dLogVol = 0.0;
    if(!FLOAT_EQUAL(dKAvg, 0.0)) // CR 22358 - James McDuffie
    {
        dLogVol = ( dLogPressureAtSuctionClosure - log10(dPressureCurveGage_USCS[lIndexAtSuctionPressure] + dBarometricPressure_USCS ) ) / dKAvg
                    + dLogVolumeAtSuctionClosure;
    }

    dTheoreticalSuctionValveCloseAngle = dAngleAtSuctionValveClosure;

    for (int nIndex = 0; nIndex <= lQuarterPressureIndex - lIndexAtSuctionPressure; ++nIndex)
    {
        if (x_values[nIndex] <= dLogVol)
        {
            // use interpolation to generate a fractional "index"
            double dIndex = nIndex;
            if ( nIndex > 0  && !FLOAT_EQUAL((x_values[nIndex-1] - x_values[nIndex]), 0.0))
            {
                dIndex -= ( dLogVol - x_values[nIndex] ) / ( x_values[nIndex-1] - x_values[nIndex] );
            }

            if( !bCalcCrankAngleFromIndex( ( (long)dIndex + lIndexAtSuctionPressure ), &dAngleAtSuctionValveClosure ) )
            {
                return false;
            }
            break;
        }
    }
    //End CR3810, 12829
    *dSuctionValveCloseAngle = dAngleAtSuctionValveClosure;

    //success
    return true;
}

///////////////////////////////////////////////////////////////////////
// Retrieve the ratio of compression
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetCompressionRatio( double *dCompressionRatio )
{
    
    double dSuctionPressureAbs;
    double dDischargePressureAbs;
    long lTemp = ( lPressureCurveSampleCount / 2 );

    //establish index values
    if( !bSetIndexValues() )
    {
        return false;
    }

    //retrieve the absolute pressures
    dSuctionPressureAbs = ( dPressureCurveGage_USCS[ lSuctionPressureIndex ] + dBarometricPressure_USCS );
    dDischargePressureAbs = ( dPressureCurveGage_USCS[ lDischargePressureIndex ] + dBarometricPressure_USCS );

    //calculate the compression ratio
    if( dSuctionPressureAbs == 0 )
    {
        *dCompressionRatio = 0.00;
    }
    else
    {
        *dCompressionRatio = ( dDischargePressureAbs / dSuctionPressureAbs );
    }

    //success
    return true;
}

///////////////////////////////////////////////////////////////////////
// Return the indicated percent clearance.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetClearanceVolumePercent( double *dClearancePercent )
{
    //update members
    if( !bGetClearanceVolumePercent( dKAvg, dClearancePercent) )
    {
        return false;
    }

    //success
    return true;
}


///////////////////////////////////////////////////////////////////////
// Retrieve the clearance volume.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetClearanceVolumePercent(double dKAvg, 
                                double *dClearanceVolume )
{
    return bGetClearanceVolumePercent(dKAvg, 
                                dClearanceVolume,
                                true);
}

///////////////////////////////////////////////////////////////////////
// Retrieve the clearance volume.  Overloaded to include a flag that
// allows either the suction or discharge conditions to be used
// to calculate the volume.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetClearanceVolumePercent(double dKAvg, 
                                double *dClearanceVolume,
                                bool bUseSuctionConditions)
{
    
    double dRC;
    double dVolumetricEfficiencySuction;
    double dVolumetricEfficiencyDischarge;
    double dConnectingRodLength = GetConnRodLength_USCS();
    double dStrokeRadius = Getr_USCS();

    if( !bSetkAvg( dKAvg ) )
    {
        return false;
    }
    
    if( !bSetIndexValues() )
    {
        return false;
    }

    if( !bGetCompressionRatio( &dRC ) )
    {
        return false;
    }

    if( !bGetVolumetricEfficiencies( &dVolumetricEfficiencySuction,
                                        &dVolumetricEfficiencyDischarge) )
    {
        return false;
    }

    // Calculate the clearance volume based on the suction conditions.  
    // Reference the white paper "Paper-Body-VolBal_061031.doc" for details.
    if (  bUseSuctionConditions == true )
    {

        // Avoid dividing by zero
        if( fabs(dRC) < 1.01 )
        {
            dClearanceVolumePercent = (double)0.0;
        }
        else
        {
            dClearanceVolumePercent = ( ( 1.0 - dVolumetricEfficiencySuction ) / ( pow( dRC, ( 1.00 / dKAvg ) ) - 1.00 ) );
        }

    }
    else
    {
        
        // Avoid dividing by zero
        if( fabs(dRC) < 1.01 )
        {
            dClearanceVolumePercent = (double)0.0;
        }
        else
        {
            dClearanceVolumePercent = ( ( pow( dRC, ( 1 / dKAvg ) ) * dVolumetricEfficiencyDischarge ) - 1.00 );
            dClearanceVolumePercent = ( dClearanceVolumePercent / ( 1.00 -  pow( dRC, ( 1 / dKAvg ) ) ) );
        }

    }

    // Return the value.
    *dClearanceVolume = dClearanceVolumePercent;
    
    // Log as success.
    return true;
}

///////////////////////////////////////////////////////////////////////
// Retrieve the clearance volume.  Overloaded to include a flag that
// allows either the suction or discharge conditions to be used
// to calculate the volume.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetClearanceVolumePercentAsArray(double dKAvg, 
                                double *dClearanceVolume,
                                bool bUseSuctionConditions)
{   
    // Initialize local members
    double _dClearanceVolume = 0.0;
    long lSampleOffset;
    long lIndex;

    // Outer loop for the different piston angles
    for (lSampleOffset=0; lSampleOffset<lPressureCurveSampleCount; lSampleOffset++)
    {
        // Adjust the working pressure array for the piston angle
        for( lIndex=0; lIndex<lPressureCurveSampleCount; lIndex++)
        {
            dPressureCurveGage_USCS[ lIndex ] = dPressureCurveBackupGage_USCS[ ( lIndex + lSampleOffset ) % lPressureCurveSampleCount ];
        }
        
        // Debug
        if ( lSampleOffset == 91 )
        {
            _dClearanceVolume = 1.0;
        }

        // Calculation the volumetric efficiencies
        if( !bGetClearanceVolumePercent(dKAvg,
                                        &_dClearanceVolume,
                                        bUseSuctionConditions))
        {
            return false;
        }

        // Copy values into array.
        dClearanceVolume[ lSampleOffset ] = _dClearanceVolume;

    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////
// Return the indicated volumetric efficiency.  For a single 
// instance it is assumed that it is the suction VE that is to
// be returned.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetVolumetricEfficiencies(double *dVolumetricEfficiency)
{
    
    // Initialize local members
    double dVolumetricEfficiencyDischarge;

    //success
    return bGetVolumetricEfficiencies( dVolumetricEfficiency, &dVolumetricEfficiencyDischarge );
}


///////////////////////////////////////////////////////////////////////
// Return the indicated volumetric efficiencies.  This overloaded
// method returns both the suction and discharge VE's.
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetVolumetricEfficiencies(double *dVolumetricEfficiencySuction,
                                                                double *dVolumetricEfficiencyDischarge)
{
    
    // Initialize local members
    double dPistonPositionAtSuctionOpen = 0.0;
    double dPistonPositionAtDischargeOpen = 0.0;;
    double dStrokeRadius = Getr_USCS();

    // The member determines critical index such as
    // valve opening and closing events
    if( !bSetIndexValues() )
    {
        return false;
    }

    // Find the displacement value for the suction valve opening
    if( !bGetChamberDisplacementAtIndex_USCS( lSuctionValveOpenIndex, &dPistonPositionAtSuctionOpen, bHeadEndChamber ) )
    {
        return false;
    }
    
    // Calculate the suction volumetric efficiency.  Reference the white paper.
    *dVolumetricEfficiencySuction = ( 1.0 - ( dPistonPositionAtSuctionOpen / (  dStrokeRadius * 2 ) ) );

    // Find the displacement value for the discharge valve opening
    if( !bGetChamberDisplacementAtIndex_USCS( lDischargeValveOpenIndex, &dPistonPositionAtDischargeOpen, bHeadEndChamber ) )
    {
        return false;
    }
    
    // Calculate the discharge volumetric efficiency.  Reference the white paper.
    *dVolumetricEfficiencyDischarge = ( 0.0 + ( dPistonPositionAtDischargeOpen / (  dStrokeRadius * 2 )  ) );

    // Success
    return true;
}

///////////////////////////////////////////////////////////////////////
// Returns two arrays with the indicated volumetric efficiencies at each
// piston angle.  
///////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetVolumetricEfficienciesAsArray(double *dVolumetricEfficiencySuctionArray,
                                                                double *dVolumetricEfficiencyDischargeArray)
{
    
    // Initialize local members
    double dVESuction = 0.0;
    double dVEDischarge = 0.0;
    long lSampleOffset;
    long lIndex;
    
    // Outer loop for the different piston angles
    for (lSampleOffset=0; lSampleOffset<lPressureCurveSampleCount; lSampleOffset++)
    {
        // Adjust the working pressure array for the piston angle
        for( lIndex=0; lIndex<lPressureCurveSampleCount; lIndex++)
        {
            dPressureCurveGage_USCS[ lIndex ] = dPressureCurveBackupGage_USCS[ ( lIndex + lSampleOffset ) % lPressureCurveSampleCount ];
        }
        
        // Calculation the volumetric efficiencies
        if( !bGetVolumetricEfficiencies(&dVESuction,
                                        &dVEDischarge))
        {
            return false;
        }

        // Copy values into arrays
        dVolumetricEfficiencySuctionArray[ lSampleOffset ] = dVESuction;
        dVolumetricEfficiencyDischargeArray[ lSampleOffset ] = dVEDischarge;

    }

    // Success
    return true;
}


///////////////////////////////////////////////////////////////////////
// Return the indicated cylinder pressure curve
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetPressureCurveGage_USCS( double *dDataOutArray, 
                                                    long lArrayMax )
{
    long lTemp;
    lTemp =0;
    
    //validate lenth of array
    if( lArrayMax < MAXPRESSUREARRAYSIZE )
    {
        return false;
    }

    //copy the data into the object
    while( lTemp < lArrayMax )
    {
        dPressureCurveGage_USCS[ lTemp ] = dDataOutArray[ lTemp ];
        lTemp++;
    }

    //success
    return true;
}

///////////////////////////////////////////////////////////////////////
// Return the number of samples in the indicated cylinder pressure
// waveform
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetPressureCurveSampleCount( long *lSampleCount )
{
    *lSampleCount = lPressureCurveSampleCount; 
    return true;
}
///////////////////////////////////////////////////////////////////////
// Load the indicated cylinder pressure curve
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bLoadPressureCurveGage_USCS(double *dDataInArray_USCS,
        double dLocalBarometricPressure_USCS,
        long lArrayCount)
{
    long lTemp;
    lTemp =0;
    
    //validate barometric pressure
    if( dLocalBarometricPressure_USCS <= 0 )
    {
        return false;
    }
    if( dLocalBarometricPressure_USCS > 20 )
    {
        return false;
    }
    //copy the data into the object
    dBarometricPressure_USCS = dLocalBarometricPressure_USCS;

    //update the object sample count variables
    if ( !CPressureCurveAnalytics::bSetPressureCurveSampleCount( lArrayCount) )
    {
        return false;
    }

    // Copy the data into the object and validate
    while( lTemp < lArrayCount )
    {
        dPressureCurveGage_USCS[ lTemp ] = dDataInArray_USCS[ lTemp ];
        dPressureCurveBackupGage_USCS[ lTemp ] = dDataInArray_USCS[ lTemp ];
        
        // No negative absolute pressures
        if( dPressureCurveGage_USCS[ lTemp ] + dLocalBarometricPressure_USCS < 0 )
        {        
            return false;
        }
        lTemp++;
    }

    //success
    return true;
}

///////////////////////////////////////////////////////////////////////
// Return the barometric pressure
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetBarometricPressure_USCS( double *dOutputBaroPressure )
{
    *dOutputBaroPressure = dBarometricPressure_USCS;
    return true;
}

///////////////////////////////////////////////////////////////////////
// Locate and calculate key index values
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bSetIndexValues()
{
    
    //establish which end we are working on and calculate
    //the suction pressure and discharge pressure index values
    if( bHeadEndChamber )
    {
        lSuctionPressureIndex = ( lPressureCurveSampleCount / 2 );
        lDischargePressureIndex = 0;
    }
    else
    {
        lSuctionPressureIndex = 0;
        lDischargePressureIndex = ( lPressureCurveSampleCount / 2 );
    }

    //find the first point the indicated pressure drops below suction pressure
    //initialize to known point in the curve where pressure should exceed suction pressure
    lSuctionValveOpenIndex = lDischargePressureIndex;
    while( dPressureCurveGage_USCS[lSuctionValveOpenIndex] > dPressureCurveGage_USCS[lSuctionPressureIndex] )
    {
        lSuctionValveOpenIndex++;
        //wrap if needed
        if( lSuctionValveOpenIndex > lPressureCurveSampleCount )
        {
            lSuctionValveOpenIndex = ( lSuctionValveOpenIndex - lPressureCurveSampleCount );
        }
    }

    //find the first point pressure exceeds discharge pressure
    //initialize at known point where the pressure will be below discharge pressure
    lDischargeValveOpenIndex = lSuctionPressureIndex;
    while( dPressureCurveGage_USCS[lDischargeValveOpenIndex] < dPressureCurveGage_USCS[lDischargePressureIndex] )
    {
        lDischargeValveOpenIndex++;
        //wrap if needed
        if( lDischargeValveOpenIndex >= lPressureCurveSampleCount )
        {
            lDischargeValveOpenIndex = ( lDischargeValveOpenIndex - lPressureCurveSampleCount );
        }
    }

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Calculate the piston position at an index
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetPistonPositionAtIndex_USCS( long lIndex, double *dPistonPosition_USCS )
{
    
    double dCrankAngle;

    if( !bCalcCrankAngleFromIndex( lIndex, &dCrankAngle ) )
    {
        return false;
    }
    
    if( !SetCrankAngleDegrees( dCrankAngle ) )
    {
        return false;
    }
    
    *dPistonPosition_USCS = GetSliderDisplacement_USCS();

    //success
    return true;
}

///////////////////////////////////////////////////////////////////////
// Calculate the chamber displacement value at an index
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetChamberDisplacementAtIndex_USCS( long lIndex, 
                                                                double *dPistonPosition_USCS, 
                                                                bool bHeadEnd )
{
    
    
    //get stroke radius
    double dStrokeRadius = Getr_USCS();
    if( dStrokeRadius <= 0 )
    {
        return false;
    }

    if( !bGetPistonPositionAtIndex_USCS( lIndex, dPistonPosition_USCS ) )
    {
      return false;
    }
    if( !bHeadEndChamber )
    {
        *dPistonPosition_USCS = ( ( dStrokeRadius * 2 ) - *dPistonPosition_USCS );
    }
    return true;
}

//////////////////////////////////////////////////////////////////////
// Get the discharge valve open index point
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetDischargeValveOpenIndex( long * lIndex )
{
    *lIndex = lDischargeValveOpenIndex;
    return true;
}

//////////////////////////////////////////////////////////////////////
// Get the displaced volume array
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetDisplacedVolumeArray( double *dDisplacedVolumeArray, 
                                                            long lMaxDisplacedVolumeCount )
{

    double dClearanceVolume_USCS;
    double dDisplacedVolume_USCS;
    double dClearanceVolumePercent;
    double dAngleAtSuctionValveClosure;
    double dBoreDiameter_USCS;
    double dPistonRodDiameter_USCS;
    double dPistonPosition_USCS;
    double dStrokeRadius;
    long lIndex;

    //validate array sizes
    if( lMaxDisplacedVolumeCount < lPressureCurveSampleCount )
    {
        return false;
    }

    //get stroke radius
    dStrokeRadius = Getr_USCS();
    if( dStrokeRadius <= 0 )
    {
        return false;
    }

    //default for crank end chamber, adjust if configured
    //for the head end chamber
    if (bHeadEndChamber)
    {
        dAngleAtSuctionValveClosure = 180.0;
        if( !bGetCylinderBoreHeadEndDiameter_USCS( 1, &dBoreDiameter_USCS ) )
        {
            return false;
        }
        if( !bGetPistonRodHeadEndDiameter_USCS( 1, &dPistonRodDiameter_USCS ) )
        {
            return false;
        }
    }
    else
    {
        double dAngleAtSuctionValveClosure = 0.0;  
        if( !bGetCylinderBoreCrankEndDiameter_USCS( 1, &dBoreDiameter_USCS ) )
        {
            return false;
        }
        if( !bGetPistonRodCrankEndDiameter_USCS( 1, &dPistonRodDiameter_USCS ) )
        {
            return false;
        }
    }

    //retrieve the clearance volume
    dDisplacedVolume_USCS = GetDisplacedVolume_USCS( dBoreDiameter_USCS, dPistonRodDiameter_USCS );
    if( !bGetClearanceVolumePercent( &dClearanceVolumePercent ) )
    {
        return false;
    }
    dClearanceVolume_USCS = ( dClearanceVolumePercent * dDisplacedVolume_USCS );

    //calculate volume values
    for (lIndex = 0; lIndex <= lPressureCurveSampleCount; ++lIndex)
    {

        //set the X value to the volume
        //calculate and store the log10 of absolute volume ( Va(n) = (%Vd(n) + %Cl) * Vd )
        if( !bGetChamberDisplacementAtIndex_USCS( lIndex, &dPistonPosition_USCS, bHeadEndChamber ) )
        {
            return false;
        }
	    dDisplacedVolumeArray[lIndex] = ( ( ( dPistonPosition_USCS / ( dStrokeRadius * 2 ) ) + dClearanceVolumePercent ) * dDisplacedVolume_USCS ); 
	  
   }

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Get the discharge pressure index point
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetDischargePressureIndex( long * lIndex )
{
    *lIndex = lDischargePressureIndex;
    return true;
}

//////////////////////////////////////////////////////////////////////
// Get the suction pressure index point
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetSuctionPressureIndex( long * lIndex )
{
    *lIndex = lSuctionPressureIndex;
    return true;
}

//////////////////////////////////////////////////////////////////////
// set the object sample count member value
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bSetPressureCurveSampleCount( long lArrayCount )
{
    //validate lenth of array
    if( lArrayCount > MAXPRESSUREARRAYSIZE || lArrayCount <=0 )
    {
        return false;
    }

    //copy the data into the object
    lPressureCurveSampleCount = lArrayCount;
    
    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Get average isentropic exponents
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bGetkAvg(double *dkAvgReturn)
{
    *dkAvgReturn = CPressureCurveAnalytics::dKAvg;
    return true;
}

//////////////////////////////////////////////////////////////////////
// Set average isentropic exponents
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::bSetkAvg(double dkAvgInput)
{
    
    if( ( dkAvgInput < 1.00 ) || ( dkAvgInput > 7.00 ) )
    {
        return false;
    }

    CPressureCurveAnalytics::dKAvg = dkAvgInput;
    
    //success
    return true;
}

/*************************** PUBLIC FUNCTION *******************************
  Function Name:  FitPolynomial

  Purpose:        Fit a polynomial to the passed data set.

  Author:         Ivan Johnson BNMIN

  Inputs:         x        the input array of x-values
                  y        the input array of y-values
                  ndata    the number of elements in x and y
                  degree   the degree of the polynomial to be fitted
                  pResult  a pointer to the resulting fitted polynomial

  Outputs:        The result polynomial is stored in the location to which 
                  'pResult' points.

  Notes:          This function finds the polynomial of the specified degree
                  that best fits the data in a least-squares sense.  It uses
                  singular values decomposition to compute the result, as
                  described in Numerical Recipes (www.nr.com), chapter 15.

  Change History:
**************************************************************************/
void CPressureCurveAnalytics::FitPolynomial(const double x[], const double y[], size_t ndata, int degree, ShiftedPolynomial* pResult)
{
   const int ma = degree + 1;  // number of coefficients to estimate
   const Vec_DP sig(1, ndata);  // individual standard deviations are unknown, so just use sigma==1
   Vec_DP a(ma);
   Mat_DP u(ndata, ma);
   Mat_DP v(ma, ma);
   Vec_DP w(ma);
   double chisq = 0;

   // Perform a change of variables to center the data around the 
   // y-axis.  This will help to reduce floating-point roundoff error.

   double x_mean = mean(x, x + ndata);

   Vec_DP x_shifted(ndata);  // the array of x-values, with each element adjusted by x_mean so that the data are centered around the y-axis
   Vec_DP y_nrvec(ndata);  // the array of y-values, in the form of a Numerical Recipes vector
   for ( size_t i = 0 ; i != ndata ; ++i )
   {
      x_shifted[i] = x[i] - x_mean;
      y_nrvec[i] = y[i];
   }

   NR::svdfit(x_shifted, y_nrvec, sig, a, u, v, w, chisq, NR::fpoly);

   *pResult = ShiftedPolynomial(Polynomial(&a[0], a.size()), x_mean);
}

//////////////////////////////////////////////////////////////////////
// Compare two floats
//////////////////////////////////////////////////////////////////////
bool CPressureCurveAnalytics::FLOAT_EQUAL( double dA, double dB)
{

    double dRelativeError;

    //Are they close on absolute size?
    if (fabs(dA - dB) < MAX_EPSILON)
    {
        return true;
    }

    //what about relative?
    if (fabs(dB) > fabs(dA))
    {
        dRelativeError = fabs((dA - dB) / dB);
    }
    else
    {
        dRelativeError = fabs((dA - dB) / dA);
    }
    if ( dRelativeError <= MAX_EPSILON )
    {
        return true;
    }
    return false;
}

