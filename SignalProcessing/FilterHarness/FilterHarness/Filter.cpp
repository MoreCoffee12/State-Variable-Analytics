///////////////////////////////////////////////////////////////////////////
// Revision
// 
// Modified to include a smoothing function that mimics the smoothing
// step in Decision Support.
// 
// Brian Howard
// 18 Sep 2012
///////////////////////////////////////////////////////////////////////////
// Revision 
//
// Added bi-directional filtering for zero-phase shift.
//
// Brian Howard
// 19 Sep 2012
///////////////////////////////////////////////////////////////////////////
// Revision 
//
// Added moving-average support for aperiodic waveforms.
//
// Brian Howard
// 28 Mar 2013
///////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include ".\filter.h"

CFilter::CFilter(void)
{

  dPi = 3.1415926535897932384626433832795;

  // Need to set the values for initial configuration
  _dHPCorner = (double)150;
  _dLPCorner = (double)150;
  _dSamplingFrequency = (double)3600;
  bSetHighPass (false );
  bSetLowPass( true );
  bSetNotchPass(false);
  bSetBandPass(false);

  //Enter percent ripple (0 to 29)
  bSetPercentRipple(0.5);

  //INPUT "Enter number of poles (2,4,...20)
  bSetNumberPoles(4);

  // Default to the zero-phase model
  bSetZeroPhase( true );

}

CFilter::~CFilter(void)
{
}

//////////////////////////////////////////////////////////////////
// This method serves select the appropriate method for 
// calculating filter coefficients
//////////////////////////////////////////////////////////////////
bool CFilter::_bCalcCoefficients()
{
        
    // Nothing special if this is simply high-pass or 
    // low-pass filtered
    if (_bHighPass )
    {
        return _bCalcHPLPCoefficients( true );
    }
    if (_bLowPass)
    {
        return _bCalcHPLPCoefficients(false);
    }

    // For a band-reject, or notch, filter we need to
    // call a narrow-band filter calc
    if (_bNotchPass)
      {

        return _bCalcCoefficients_NotchFilter();

      }

      // For a narrow-band pass, filter we need to
      // call a narrow-band pass filter calc
      if (_bBandPass)
      {

        return _bCalcCoefficients_NarrowBandFilter();

      }
      
      //what happened?
      return false;
}

///////////////////////////////////////////////////////////////////////////
// This function is a c++ re-cast of the BASIC code found in chapter 33 of
// 'The Scientist's and Engineer's Guide to Digital Signal Processors'
// handbook, Figures 33-7 and Figures 33-8.
///////////////////////////////////////////////////////////////////////////
bool CFilter::_bCalcHPLPCoefficients( bool bIsHighPass )
{
  double dTA[MAXKERNELSIZE];
  double dTB[MAXKERNELSIZE];
  int i, p;
  double dFC;
  double dRP, dIP, dES, dVX, dKX;
  double dt, dW, dM, dD;
  double dX0, dX1, dX2, dY1, dY2;
  double dk;
  double dA0, dA1, dA2, dB1, dB2;
  double dSA, dSB, dGAIN;
  long lLH;

  // Reset the filter kernel
  _bResetFilterKernel();

  //enter the recursive calculation
  _A[2] = 1;
  _B[2] = 1;

  //Enter 0 for LP, 1 for HP filter
  if( bIsHighPass )
  {
    dFC = ( _dHPCorner / _dSamplingFrequency );
    lLH = 1;
  }
  else
  {
    dFC = ( _dLPCorner / _dSamplingFrequency );
    lLH = 0;
  }

  //loop
  for( p=1; p<=( _lNP / 2 ); p++)
  {
    //Calculate the pole location on the unit circle
    dRP = -cos(dPi / ( (double)_lNP * 2) + ((double)p - 1) * dPi / (double)_lNP);
    dIP = sin(dPi / ( (double)_lNP * 2) + ((double)p - 1) * dPi / (double)_lNP);
  
    //Warp from a circle to an ellipse
    if( _dPR != 0 )
    {
      dES = sqrt( pow((100 / (100 - _dPR) ),2) - 1);
      dVX = (1 / (double)_lNP) * log((1 / dES) + sqrt( (1 / ( dES * dES ) ) + 1));
      dKX = (1 / (double)_lNP) * log((1 / dES) + sqrt( (1 / ( dES * dES ) ) - 1));
      dKX = (exp(dKX) + exp(-dKX)) / 2.0;
      dRP = dRP * ((exp(dVX) - exp(-dVX)) / 2) / dKX;
      dIP = dIP * ((exp(dVX) + exp(-dVX)) / 2) / dKX;
    }

    //s-domain to z-domain conversion
    dt = ( 2.0 * tan( 1.0 / 2.0 ) );
    dW = ( 2.0 * dPi * dFC );
    dM = ( ( dRP * dRP ) + (dIP * dIP ) );
    dD = 4 - 4 * dRP * dt + dM * dt * dt;
    dX0 = ( ( dt * dt) / dD );
    dX1 = ( ( 2 * dt *dt ) / dD );
    dX2 = ( ( dt * dt ) / dD );
    dY1 = ( ( 8 - 2 * dM * dt * dt) / dD );
    dY2 = ( (-4 - 4 * dRP * dt - dM * dt * dt) / dD );

    //LP TO LP, or LP TO HP transform
    if ( lLH == 1 )
      dk = -cos(dW / 2.0 + 1.0 / 2.0) / cos(dW / 2.0 - 1.0 / 2.0);
    if ( lLH == 0 )
      dk = sin(1.0 / 2.0 - dW / 2.0) / sin(1.0 / 2.0 + dW / 2.0);
    dD = 1 + dY1 * dk - dY2 * dk * dk;

    dA0 = ( ( dX0 - dX1 * dk + dX2 * dk * dk) / dD );
    dA1 = ( (-2 * dX0 * dk + dX1 + dX1 * dk * dk - 2 * dX2 * dk) / dD );
    dA2 = ( ( dX0 * dk * dk - dX1 * dk + dX2) / dD );
    dB1 = ( ( 2 * dk + dY1 + dY1 * dk * dk - 2 * dY2 * dk) / dD );
    dB2 = ( ( -(dk * dk) - dY1 * dk + dY2) / dD );

    if ( lLH == 1 )
      dA1 = -dA1;
    if ( lLH == 1 )
      dB1 = -dB1;

    //Add coefficients to the cascade
    for (i=0; i<=22; i++)
    {
      dTA[i] = _A[i];
      dTB[i] = _B[i];
    }

    for (i=2; i<=22; i++)
    {
      _A[i] = (dA0 * dTA[i]) + (dA1 * dTA[i - 1]) + (dA2 * dTA[i - 2]);
      _B[i] = dTB[i] - (dB1 * dTB[i - 1]) - dB2 * (dTB[i - 2]);
    }

  }

  //Finish combining coefficients
  _B[2] = 0;

  for ( i = 0; i<=20; i++)
  {
    _A[i] = _A[i + 2];
    _B[i] = -(_B[i + 2]);
  }
  
  //NORMALIZE THE GAIN
  dSA = 0;
  dSB = 0;

  for (i=0; i<=20; i++)
  {
    if ( lLH == 0 )
      dSA = dSA + _A[i];
    if ( lLH == 0 )
      dSB = dSB + _B[i];
    if ( lLH == 1 )
      dSA = dSA + _A[i] * pow( (double)-1, i);
    if ( lLH == 1 )
      dSB = dSB + _B[i] * pow( (double)-1, i);
  }

  dGAIN = dSA / (1 - dSB);

  for ( i=0; i<=20; i++)
  {
    _A[i] = _A[i] / dGAIN;
  }

  return true;
}


//////////////////////////////////////////////////////////////////
// Used to calculate the coefficients for the narrow band filters
// Reference The Scientist's and Engineer's Guide to Digital 
// Signal Processors - Chapter 19 - Recursive Filters
//////////////////////////////////////////////////////////////////
bool CFilter::_bCalcSamplingFractions(double *dR, double *dK, double *dfc)
{
    // Initialze filter configuration
    double dBW = (fabs(_dLPCorner - _dHPCorner) / _dSamplingFrequency);
    *dfc = (((_dHPCorner + _dLPCorner) / 2.0) / _dSamplingFrequency);
    *dR = (1.0 - 3.0 * dBW);
    *dK = (1.0 - (2.0 * *dR * cos(2.0 * dPi * *dfc)) + (*dR * *dR));
    *dK = (*dK / (2.0 - (2.0 * cos(2.0 * dPi * *dfc))));

    return true;
}


//////////////////////////////////////////////////////////////////
// Used to calculate the coefficients for the notch (band-reject) filter
// Reference The Scientist's and Engineer's Guide to Digital 
// Signal Processors - Chapter 19 - Recursive Filters
//////////////////////////////////////////////////////////////////
bool CFilter::_bCalcCoefficients_NotchFilter()
{

      double dR = 0.0;
      double dK = 0.0;
      double dfc = 0.0;
      bool bTemp;

      // Calculate the sampling frequency fractions
      bTemp = _bCalcSamplingFractions( &dR, &dK, &dfc);
      if (bTemp == false)
      {
        return false;
      }

      bTemp = _bResetFilterKernel();
      if (bTemp == false)
      {
        return false;
      }

      _A[0] = dK;
      _A[1] = (-2 * dK * cos(2 * dPi * dfc));
      _A[2] = dK;
      _B[1] = (2 * dR * cos( 2 * dPi * dfc ) );
      _B[2] = (-dR * dR);
      
      return true;
}

//////////////////////////////////////////////////////////////////
// Used to calculate the coefficients for the band-pass filter
// Reference The Scientist's and Engineer's Guide to Digital 
// Signal Processors - Chapter 19 - Recursive Filters
//////////////////////////////////////////////////////////////////
bool CFilter::_bCalcCoefficients_NarrowBandFilter()
{

    double dR = 0.0;
    double dK = 0.0;
    double dfc = 0.0;
    bool bTemp;

    // Calculate the sampling frequency fractions
    bTemp = _bCalcSamplingFractions(&dR, &dK, &dfc);
    if (bTemp == false)
    {
    return false;
    }
    bTemp = _bResetFilterKernel();
    if (bTemp == false)
    {
    return false;
    }

    _A[0] = (1 - dK);
    _A[1] = (2 * ( dK - dR ) * cos(2 * dPi * dfc));
    _A[2] = ( ( dR * dR ) - dK);
    _B[1] = (2 * dR * cos(2 * dPi * dfc));
    _B[2] = (-dR * dR);

    return true;
}


//////////////////////////////////////////////////////////////////
// Initialize the kernel arrays to zero.
//////////////////////////////////////////////////////////////////
bool CFilter::_bResetFilterKernel()
{

    long i;

    for (i = 0; i < MAXKERNELSIZE; i++)
    {
    _A[i] = 0;
    _B[i] = 0;
    }

    return true;
}


bool CFilter::bFilterWaveform( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount)
{
  
    //local variables
    int i;
	int j;
    double dTemp;
    int iKernelSize;
    bool bIsZeroPhase;

    //initialize local variables
    iKernelSize = 20;
    bGetZeroPhase( &bIsZeroPhase);

    //allocate memory for the arrays   
    double* dFilteredArray = new double[ MAXFILTERARRAYSIZE ];
    if( dFilteredArray == NULL  )
    {
        return false;
    }
    double* dPaddedArray = new double[ MAXFILTERARRAYSIZE ];
    if( dPaddedArray == NULL )
    {
        return false;
    }

    //validate array size
    if( (lArrayCount * 2) > MAXFILTERARRAYSIZE )
    {
        delete[] dFilteredArray;
        delete[] dPaddedArray;
        return false;
    }

    //validate the sample count
    if( lArrayCount < iKernelSize )
    {
        delete[] dFilteredArray;
        delete[] dPaddedArray;
        return false;
    }

    //initialize the return array
    for(i=0;i<(2*lArrayCount);i++)
    {

        dFilteredArray[i] = dDataInArray[i % lArrayCount];
        dPaddedArray[i] = dDataInArray[i % lArrayCount];

    }

    //implement the filter kernel, forwards
    for(i=iKernelSize;i<(2*lArrayCount);i++)
    {
        dTemp=0;

        for(j=0;j<iKernelSize;j++)
        {
            dTemp = (dTemp + (dPaddedArray[i-j] * _A[j]));
            dTemp = (dTemp + (dFilteredArray[i-j] * _B[j]));
        }

        dFilteredArray[i] = dTemp;
    }

    // Shift and copy the forward filterd data into 
    // output array.  
    for(i=0;i<lArrayCount;i++)
    {
        dDataOutArray[i] = dFilteredArray[i + lArrayCount];
    }

    // If configured, perform zero-phase filtering
    if( bIsZeroPhase )
    {
        // Pad arrays with forward-filtered data
        // for the next step.
        for(i=0;i<(2*lArrayCount);i++)
        {

            dFilteredArray[i] = dDataOutArray[i % lArrayCount];
            dPaddedArray[i] = dDataOutArray[i % lArrayCount];

        }

	    // Implement the filter kernel, backwards
        for(i=(2*lArrayCount)-iKernelSize-1;i>=0;i--)
        {
            dTemp=0;

            for(j=0;j<iKernelSize;j++)
            {
                dTemp = (dTemp + (dPaddedArray[i+j] * _A[j]));
                dTemp = (dTemp + (dFilteredArray[i+j] * _B[j]));
            }

            dFilteredArray[i] = dTemp;
        }

        // copy the data into the return array
        for(i=0;i<lArrayCount;i++)
        {
            dDataOutArray[i] = dFilteredArray[i];
        }
    }

    //release memory
    delete[] dFilteredArray;
    delete[] dPaddedArray;

    //success
    return true;
}

//////////////////////////////////////////////////////////////////
// This method serves smooths a waveform to mimic the smoothing
// step in Decision Support Studio
// 
//////////////////////////////////////////////////////////////////
// Revision
//
// Modified to implement the filter via recursion to improve
// speed of execution and to enable multiple-passes.
// Reference chapter 14 of 'The Scientist's and Engineer's 
// Guide to Digital Signal Processors' for details.
//
// Brian Howard
// 28 Sep 2012
//
//////////////////////////////////////////////////////////////////
bool CFilter::bSmoothWaveform( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount, 
						  long lHalfKernelLength, 
                          long lNumberOfPasses)
{
    
	long lIndex;
	long lIndexKernel;
    long lPassIndex;
	double dSum;

    // Validate array size.
    if( lArrayCount > MAXFILTERARRAYSIZE )
    {
        return false;
    }

	// Validate kernel size
	if( lHalfKernelLength <= 0 )
	{
		return false;
	}

    // Allocate memory for an intermediate array and copy the
    // input array into it.
    double* dDataIntermediate = new double[ lArrayCount ];
    for(lIndex=0;lIndex<lArrayCount;lIndex++)
    {
        dDataIntermediate[lIndex] = dDataInArray[lIndex];
    }

	// Outer loop for number of passes
    for( lPassIndex = 1; lPassIndex<=lNumberOfPasses; lPassIndex++)
    {

        // Reset the accumulator.
	    lIndex=0;
        dSum = dDataIntermediate[ lIndex ];
		
	    // Accumulate the values.
	    for( lIndexKernel=1; lIndexKernel<=lHalfKernelLength; lIndexKernel++ )
	    {
		    dSum = dSum + dDataIntermediate[ (lIndex + lArrayCount + lIndexKernel) % lArrayCount ];	
		    dSum = dSum + dDataIntermediate[ (lIndex + lArrayCount - lIndexKernel) % lArrayCount ];	
	    }

        // Smooth and create the output array
        for(lIndex=0;lIndex<lArrayCount;lIndex++)
        {
		

		    // Store the smoothed values.
            dDataOutArray[lIndex] = (dSum / (double)( ( (double)lHalfKernelLength * 2 ) + 1 ));

            // Update the cummulative total to move the to next
	        // value via recursion.
            dSum = dSum - dDataIntermediate[ (lIndex + lArrayCount - lHalfKernelLength) % lArrayCount ];	
		    dSum = dSum + dDataIntermediate[ (lIndex + 1 + lArrayCount + lHalfKernelLength) % lArrayCount ];	
        }

        // Copy the output array back to the intermediate for the
        // next pass.
        for(lIndex=0;lIndex<lArrayCount;lIndex++)
        {
            dDataIntermediate[lIndex] = dDataOutArray[lIndex];
        }

    }

    //release memory
    delete[] dDataIntermediate;

    // Success.
	return true;
}

//////////////////////////////////////////////////////////////////
// This method serves smooths a waveform to mimic the smoothing
// as implemented in Savitzky-Golay for aperiodic data
// 
// Brian Howard
// 28 Mar 2013
//
//////////////////////////////////////////////////////////////////
bool CFilter::bSmoothWaveformAperiodic( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount, 
						  long lHalfKernelLength)
{
    
	long lIndex;
	long lIndexKernel;
	double dSum;

    // Validate array size.
    if( lArrayCount > MAXFILTERARRAYSIZE )
    {
        return false;
    }

	// Validate kernel size
	if( lHalfKernelLength <= 0 )
	{
		return false;
	}

    // Reset the accumulator.
	lIndex=lHalfKernelLength;
    dSum = dDataInArray[ lIndex ];
		
	// Accumulate the values.
	for( lIndexKernel=1; lIndexKernel<=lHalfKernelLength; lIndexKernel++ )
	{
		dSum = dSum + dDataInArray[ (lIndex + lArrayCount + lIndexKernel) % lArrayCount ];	
		dSum = dSum + dDataInArray[ (lIndex + lArrayCount - lIndexKernel) % lArrayCount ];	
	}

    // Smooth and create the output array
    for(lIndex=lHalfKernelLength;lIndex<(lArrayCount-lHalfKernelLength);lIndex++)
    {
		

		// Store the smoothed values.
        dDataOutArray[lIndex-lHalfKernelLength] = (dSum / (double)( ( lHalfKernelLength * 2 ) + 1 ));

        // Update the cummulative total to move the to next
	    // value via recursion.
        dSum = dSum - dDataInArray[ (lIndex + lArrayCount - lHalfKernelLength) % lArrayCount ];	
		dSum = dSum + dDataInArray[ (lIndex + 1 + lArrayCount + lHalfKernelLength) % lArrayCount ];	
    }

    // Success.
	return true;
}

// Default case of a single pass
bool CFilter::bSmoothWaveform( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount, 
						  long lHalfKernelLength)
{
    
	return bSmoothWaveform( dDataInArray, 
                          dDataOutArray,
                          lArrayCount, 
						  lHalfKernelLength, 
                          1);
}

bool CFilter::bGet_A( double *dAOut, long lAIndex )
{
  *dAOut = _A[lAIndex];
  return true;
}

bool CFilter::bGet_B( double *dBOut, long lBIndex )
{
  *dBOut = _B[lBIndex];
  return true;
}

bool CFilter::bGetHighPass( bool *bHP )
{
  *bHP = _bHighPass;
  return true;
}

bool CFilter::bGetLowPass( bool *bLP )
{
  *bLP = _bLowPass;
  return true;
}

bool CFilter::bGetNotchPass( bool *bNP )
{
  *bNP = _bNotchPass;
  return true;
}

bool CFilter::bGetBandPass( bool *bBP )
{
  *bBP = _bBandPass;
  return true;
}

bool CFilter::bGetHPCorner( double *dHPCorner )
{
  *dHPCorner = CFilter::_dHPCorner;
  return true;
}

bool CFilter::bGetLPCorner( double *dLPCorner )
{
  *dLPCorner = CFilter::_dLPCorner;
  return true;
}

bool CFilter::bGetMaxKernelSize( long *lKSize )
{
  *lKSize = MAXKERNELSIZE;
  return true;
}

bool CFilter::bGetNumberPoles( long *lNP )
{
  *lNP = _lNP;
  return true;
} 

bool CFilter::bGetPercentRipple( double *dPR )
{
  *dPR = CFilter::_dPR;
  return true;
}

bool CFilter::bGetSamplingFrequency( double *dSFreq )
{
  *dSFreq = _dSamplingFrequency;
  return true;
}

bool CFilter::bGetZeroPhase( bool *bIsZeroPhase )
{
  *bIsZeroPhase = _bIsZeroPhase;
  return true;
}

bool CFilter::bSetHighPass( bool bHP )
{
  _bHighPass = bHP;
  _bNotchPass = false;
  _bBandPass = false;
  if ( !_bCalcCoefficients() )
    return false;
  return true;
}

bool CFilter::bSetLowPass( bool bLP )
{
  _bLowPass = bLP;
  _bNotchPass = false;
  _bBandPass = false;
  return _bCalcCoefficients();
}

bool CFilter::bSetNotchPass( bool bNP )
{
  _bNotchPass = bNP;
  if ( !_bCalcCoefficients() )
    return false;
  return true;
}

bool CFilter::bSetBandPass( bool bBP )
{
  _bBandPass = bBP;
  if ( !_bCalcCoefficients() )
    return false;
  return true;
}

bool CFilter::bSetHPCorner( double dHPCorner )
{

    // Is the HP Corner valid?
    if( dHPCorner <= 0 )
    {
        return false;
    }

    CFilter::_dHPCorner = dHPCorner;
    if ( !_bCalcCoefficients() )
    {
        return false;
    }
    return true;
}

bool CFilter::bSetLPCorner( double dLPCorner )
{

    // Is the HP Corner valid?
    if( dLPCorner <= 0 )
    {
        return false;
    }

    // load the value into the object
    CFilter::_dLPCorner = dLPCorner;
    if ( !_bCalcCoefficients() )
    {
        return false;
    }
    return true;
}

bool CFilter::bSetNumberPoles( long lNP )
{
  CFilter::_lNP = lNP;
  if ( !_bCalcCoefficients() )
    return false;
  return true;
}

bool CFilter::bSetPercentRipple( double dPR )
{
  CFilter::_dPR = dPR;
  if ( !_bCalcCoefficients() )
    return false;
  return true;
}

bool CFilter::bSetSamplingFrequency( double dSFreq )
{
  
    // Is the sampling frequency valid?
    if( dSFreq <= 0 )
    {
        return false;
    }
  
    // Copy it into the object
    _dSamplingFrequency = dSFreq;
    if ( !_bCalcCoefficients() )
    {
        return false;
    }
    return true;
}

bool CFilter::bSetZeroPhase( bool bIsZeroPhase )
{
  _bIsZeroPhase = bIsZeroPhase;
  if ( !_bCalcCoefficients() )
    return false;
  return true;
}