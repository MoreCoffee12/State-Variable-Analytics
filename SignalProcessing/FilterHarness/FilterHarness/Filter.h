///////////////////////////////////////////////////////////////////////////
// Revision
// 
// Modified to include a smoothing function that mimics the smoothing
// step in Decision Support.
// 
// Brian Howard
// 18 Sep 2012
///////////////////////////////////////////////////////////////////////////

#pragma once
#define MAXKERNELSIZE 32
#define MAXFILTERARRAYSIZE 1310700
#include <cmath>

class CFilter
{
public:

  //////////////////////////////////////////////////////////////////
  // Members
  //////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////
  // Methods
  //////////////////////////////////////////////////////////////////
  CFilter(void);
  ~CFilter(void);

  //object interface methods
  bool bFilterWaveform( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount );
  bool bSmoothWaveform( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount,
						  long lHalfKernelLength,
                          long lNumberOfPasses);
  bool bSmoothWaveformAperiodic( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount,
						  long lHalfKernelLength);
  bool bSmoothWaveform( double *dDataInArray, 
                          double *dDataOutArray,
                          long lArrayCount,
						  long lHalfKernelLength);

  bool bGet_A( double *dAOut, long lAIndex );
  bool bGet_B( double *dBOut, long lBIndex );
  bool bGetHighPass( bool *bHP );
  bool bGetLowPass( bool *bLP );
  bool bGetNotchPass( bool *bNP );
  bool bGetBandPass( bool *bBP );
  bool bGetPercentRipple( double *dPR );
  bool bGetNumberPoles( long *lNP );
  bool bGetHPCorner( double *dHPCorner );
  bool bGetLPCorner( double *dLPCorner );
  bool bGetMaxKernelSize( long *lKSize );
  bool bGetSamplingFrequency( double *dSFreq );
  bool bGetZeroPhase( bool *bIsZeroPhase );
  
  bool bSetHighPass( bool bHP );
  bool bSetLowPass( bool bLP );
  bool bSetNotchPass( bool bNP );
  bool bSetBandPass( bool bBP );
  bool bSetPercentRipple( double dPR );
  bool bSetNumberPoles( long lNP );
  bool bSetHPCorner( double dHPCorner );
  bool bSetLPCorner( double dLPCorner );
  bool bSetSamplingFrequency( double dSFreq );
  bool bSetZeroPhase( bool bIsZeroPhase );

private:

  //////////////////////////////////////////////////////////////////
  // Members
  //////////////////////////////////////////////////////////////////
  double _A[MAXKERNELSIZE];
  double _B[MAXKERNELSIZE];
  double _dHPCorner;
  double _dLPCorner;
  double dPi;
  double _dSamplingFrequency;
  bool _bHighPass;
  bool _bLowPass;
  bool _bNotchPass;
  bool _bBandPass;
  bool _bIsZeroPhase;
  double _dPR;
  long _lNP;

  //////////////////////////////////////////////////////////////////
  // Methods
  //////////////////////////////////////////////////////////////////
  bool _bResetFilterKernel();  
  bool _bCalcCoefficients();
  bool _bCalcHPLPCoefficients( bool bIsHighPass );
  bool _bCalcCoefficients_NotchFilter();
  bool _bCalcCoefficients_NarrowBandFilter();
  bool _bCalcSamplingFractions(double *dR, double *dK, double *dfc);

};
