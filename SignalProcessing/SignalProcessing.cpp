// SignalProcessing.cpp : Defines the entry point for the console application.
//
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Added code to interpolate from one array size to another.
// Brian Howard
// 18 July 2012
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Added code to provide notch filtering.
// Brian Howard
// 7 August 2012
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Added code to provide waveform smoothing.
// Brian Howard
// 18 September 2012
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Added code to provide AC-coupled waveform conversion.
// Brian Howard
// 25 September 2012
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Revised filtering code to improve variable naming.
// Brian Howard
// 24 October 2012
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Revised filtering code to allow the user to select zero-phase filtering
// Brian Howard
// 13 October 2012
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Revised filtering code to include function to find the index of the
// peak value in a signal.
//
// Brian Howard
// 29 April 2013
//
//////////////////////////////////////////////////////////////////////
// Revision
//
// Revised code to include function to convert a single array of 
// doubles to a CSV string
//
// Brian Howard
// 4 April 2014
//
//////////////////////////////////////////////////////////////////////

#define dPI 3.1415926535897932384626433832795
#define MAX_ARRAYSIZE 4096
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr


#include <cmath>
#include <iostream>
#include <iomanip>
#include<sstream>
#include<string>
#include ".\FilterHarness\FilterHarness\Filter.h"
#include "..\..\Toolbox\FilterCDF\filtercdf.h"
#include "windows.h"
#include "fftw.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
// housekeeping code below this marker
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
int SignalProcessingShowVersion()
{
	return 000;
}

int SignalProcessingShowBuild()
{
	return 39;
}


void four1(double data[], unsigned long nn, int isign)
{
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	double tempr,tempi;

	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) {
		if (j > i) {
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m=nn;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax=2;
	while (n > mmax) {
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}
#undef SWAP

void realft(double data[], unsigned long n, int isign)
{
	void four1(double data[], unsigned long nn, int isign);
	unsigned long i,i1,i2,i3,i4,np3;
	double c1=0.5,c2,h1r,h1i,h2r,h2i;
	double wr,wi,wpr,wpi,wtemp,theta;

	theta=3.141592653589793/(double) (n>>1);
	if (isign == 1) {
		c2 = -0.5;
		four1(data,n>>1,1);
	} else {
		c2=0.5;
		theta = -theta;
	}
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	wr=1.0+wpr;
	wi=wpi;
	np3=n+3;
	for (i=2;i<=(n>>2);i++) {
		i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
		h1r=c1*(data[i1]+data[i3]);
		h1i=c1*(data[i2]-data[i4]);
		h2r = -c2*(data[i2]+data[i4]);
		h2i=c2*(data[i1]-data[i3]);
		data[i1]=h1r+wr*h2r-wi*h2i;
		data[i2]=h1i+wr*h2i+wi*h2r;
		data[i3]=h1r-wr*h2r+wi*h2i;
		data[i4] = -h1i+wr*h2i+wi*h2r;
		wr=(wtemp=wr)*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
	}
	if (isign == 1) {
		data[1] = (h1r=data[1])+data[2];
		data[2] = h1r-data[2];
	} else {
		data[1]=c1*((h1r=data[1])+data[2]);
		data[2]=c1*(h1r-data[2]);
		four1(data,n>>1,-1);
	}
}


//////////////////////////////////////////////////////////////////////
// Return features of the FFT calculation
//////////////////////////////////////////////////////////////////////
int GetFFTMaxArraySize()
{
  return MAX_ARRAYSIZE;
}


//////////////////////////////////////////////////////////////////////
// Performs the FFT calculation for real numbers
//////////////////////////////////////////////////////////////////////
bool bCalcFFTReal(double *dDataInArray_Re,
              double *dDataInArray_Im,
              double *dDataOutArray_Amplitude,
              double *dDataOutArray_Phase,
              long lArrayElementCount,
              bool bSizeData,
              long lWindowType)
{

	//These are the input and output arrays for
	// the fftw library
	fftw_complex in_w[MAX_ARRAYSIZE];
	fftw_complex out[MAX_ARRAYSIZE];
	fftw_plan p;

    //othe locals
    long i, mm, m;
    double w, facp,facp2, facm, pitwice;
    double dTemp;
    double dTimebaseAmplitude;
    double dTimebaseMaximum;
    double dTimebaseMinimum;
    double dFFtAmpl;

    //initialize
    mm = lArrayElementCount;
    m = ( lArrayElementCount / 2 );
    facm = (double)m;
    facp = 1/facm;
    facp2 = ( facp * facp );
    pitwice = ( dPI * 2 );
    dTimebaseAmplitude = 0;
    dFFtAmpl = 0;
    for( i=0; i<MAX_ARRAYSIZE; i++)
    {
        in_w[i].re = 0.00;
        in_w[i].im = 0.00;
    }

    //window the data
    for (i=0; i<=m; i++)
    {
        //Apply the applicable window
        switch( lWindowType )
        {
        case 0:
            w=1;
            break;
        case 1:
            w = ( 1.0-fabs( (i-facm)*(facp) ) );
            break;
        case 2:
            w = ( 1.0- ( (i-facm)*(i-facm)*(facp2) ) );
            break;
        case 3:
            w = ( 0.5 * ( 1 - cos( ( pitwice * i / m ) ) ) );
            break;
        case 4:
            w = ( 0.53836 - 0.46164 * cos( ( pitwice * i / m ) ) );
            break;
        case 5:
            w = ( 1 - ( 1.93 * cos( ( pitwice * i / m ) ) ) );
            w = ( w + 1.29 * cos( ( pitwice * 2 * i / m ) ) );
            w = ( w - 0.388 * cos( ( pitwice * 3 * i / m ) ) );
            w = ( w + 0.032 * cos( ( pitwice * 4 * i / m ) ) );
            break;
        default:
            w=1;
            break;
        }

        //multiply and copy over the windowed waveform
        in_w[i].re = (dDataInArray_Re[i]*w);
        in_w[i].im *= (dDataInArray_Im[i]*w);
        in_w[mm-i].re = (dDataInArray_Re[mm-i]*w);
        in_w[mm-i].im *= (dDataInArray_Im[mm-i]*w);

    }

    //Capture timebase amplitude
	dTimebaseMaximum = dDataInArray_Re[0];
    dTimebaseMinimum = dTimebaseMaximum;
    for (i=0; i<lArrayElementCount; i++)
	{
        dTemp = dDataInArray_Re[i];
        if( dTemp > dTimebaseMaximum )
        {
			dTimebaseMaximum = dTemp;
        }
        if( dTemp < dTimebaseMinimum )
        {
            dTimebaseMinimum = dTemp;
        }
    }
    dTimebaseAmplitude = fabs( dTimebaseMaximum - dTimebaseMinimum );

    //Generate the plan for the fft
	p = fftw_create_plan((int)lArrayElementCount, FFTW_FORWARD, FFTW_ESTIMATE);

	//initialize the fftw output array to zero
	for (i=0; i<lArrayElementCount; i++)
	{
		out[i].re	= 0;
		out[i].im	= 0;
	}

	//Calculate the fft
	fftw_one(p, in_w, out);

	//finished with the fft plan
	fftw_destroy_plan(p);

    //return the values
	for (i=0; i<lArrayElementCount; i++)
	{
		dDataOutArray_Amplitude[i] = sqrt(pow(out[i].re,2) + pow(out[i].im,2));
        dFFtAmpl = dFFtAmpl + dDataOutArray_Amplitude[i];

        //Calculate the phase in radians and convert to degrees
        dDataOutArray_Phase[i] = atan2(out[i].im, out[i].re);
		if(out[i].im < 0)
        {
			dDataOutArray_Phase[i] = dDataOutArray_Phase[i] + dPI;
        }
        dDataOutArray_Phase[i] = ( dDataOutArray_Phase[i] * ( 180 / dPI ) );
	}

    //adjust the scale, if required
    if( bSizeData )
    {
		for (i=0; i<lArrayElementCount; i++)
		{
			dDataOutArray_Amplitude[i] = ( dDataOutArray_Amplitude[i] * ( ( dTimebaseAmplitude * 2.0 ) / dFFtAmpl) );
		}
    }


    return true;
}

//////////////////////////////////////////////////////////////////////
// Performs the FFT calculation
//////////////////////////////////////////////////////////////////////
int CalcFFT(double *dDataInArray,
              double *dDataOutArray,
              long lArrayElementCount,
              bool bSizeData,
              long lWindowType)
{
   	//locals
    int i, mm, m;
    double w, facp,facp2, facm, pitwice, dTemp;
    double dBig, dFFTAmpl, dSize[MAX_ARRAYSIZE];

    //initialize
    mm = lArrayElementCount;
    m = ( lArrayElementCount / 2 );
    facm = (double)m;
    facp = 1/facm;
    facp2 = ( facp * facp );
    pitwice = ( dPI * 2 );
    dBig = -1e10;
    for( i=0; i<lArrayElementCount; i++)
    {
      dDataOutArray[i] = dDataInArray[i];
      if( fabs(dDataOutArray[i]) > dBig )
        dBig = fabs(dDataOutArray[i]);
    }

    //window the data
    for (i=0; i<=m; i++)
    {
        //Apply the applicable window
        switch( lWindowType )
        {
        case 0:
            w=1;
            break;
        case 1:
            w = ( 1.0-fabs( (i-facm)*(facp) ) );
            break;
        case 2:
            w = ( 1.0- ( (i-facm)*(i-facm)*(facp2) ) );
            break;
        case 3:
            w = ( 0.5 * ( 1 - cos( ( pitwice * i / m ) ) ) );
            break;
        case 4:
            w = ( 0.53836 - 0.46164 * cos( ( pitwice * i / m ) ) );
            break;
        case 5:
            w = ( 1 - ( 1.93 * cos( ( pitwice * i / m ) ) ) );
            w = ( w + 1.29 * cos( ( pitwice * 2 * i / m ) ) );
            w = ( w - 0.388 * cos( ( pitwice * 3 * i / m ) ) );
            w = ( w + 0.032 * cos( ( pitwice * 4 * i / m ) ) );
            break;
        default:
            w=1;
            break;
        }

        //multiply and copy over the windowed waveform
        dDataOutArray[i] = (dDataOutArray[i]*w);
        dDataOutArray[mm-i] = (dDataOutArray[mm-i]*w);
    }

    //call the FFT function
    dTemp = dDataOutArray[0];
    dTemp = dDataOutArray[1];
    dTemp = dDataOutArray[2];

    realft( dDataOutArray, (unsigned long)lArrayElementCount, 1 );

    //if desired, size and scale the data
    if (bSizeData)
    {
          dFFTAmpl = 0;
          for (i=2;i<mm;i++)
          {
            dSize[i]=sqrt((dDataOutArray[2*i-1])*(dDataOutArray[2*i-1])+(dDataOutArray[2*i])*(dDataOutArray[2*i]));
            dFFTAmpl = dFFTAmpl + dSize[i];
          }
          dSize[1]=fabs(dDataOutArray[1]);
          dFFTAmpl = dFFTAmpl + fabs(dDataOutArray[1]);
          dSize[mm+1]=fabs(dDataOutArray[2]);
          dFFTAmpl = dFFTAmpl + fabs(dDataOutArray[2]);

          for(i=0;i<lArrayElementCount;i++)
          {
            if( i <=mm )
              dDataOutArray[i] = ( dSize[i] * ( dBig / dFFTAmpl));
            else
              dDataOutArray[i] = 0;
          }
    }


    return 0;
}

//////////////////////////////////////////////////////////////////////
// Performs the waveform filtering
//////////////////////////////////////////////////////////////////////
int FilterWaveform(double *dDataInArray,
              double *dDataOutArray,
              long lArrayElementCount,
              bool bHighPass,
              bool bLowPass,
              bool bNotchPass,
              bool bBandPass,
              bool bIsZeroPhase,
              double dHPCorner,
              double dLPCorner,
              double dSamplingFrequency,
              double dPercentRipple,
              long lPoleCount)
{

  //initiate class, other locals
  class CFilter *filter;
  filter = new CFilter();
  int i;
  bool bTemp;

  //configure the filter object
  filter->bSetHighPass( bHighPass );
  filter->bSetLowPass( bLowPass );
  filter->bSetNotchPass( bNotchPass );
  filter->bSetBandPass( bBandPass );
  filter->bSetHPCorner( dHPCorner );
  filter->bSetLPCorner( dLPCorner );
  filter->bSetZeroPhase( bIsZeroPhase );
  filter->bSetSamplingFrequency( dSamplingFrequency );
  filter->bSetPercentRipple( dPercentRipple );
  filter->bSetNumberPoles( lPoleCount );

  // Perform the calculation
  bTemp = filter->bFilterWaveform( dDataInArray, dDataOutArray, lArrayElementCount );

  // Loop several times for the narrow band filters
  if ( bBandPass || bNotchPass )
  {
    for (i = 0; i < 10; i++)
    {
        filter->bFilterWaveform( dDataOutArray, dDataOutArray, lArrayElementCount );
    }
  }

  //delete the class object
  delete filter;

  return 0;
}

//////////////////////////////////////////////////////////////////////
// Performs the waveform smoothing, single pass
//////////////////////////////////////////////////////////////////////
bool bXSmoothWaveform(double *dDataInArray,
              double *dDataOutArray,
              long lArrayElementCount,
              long lHalfKernelLength)
{

  // Initiate class, other locals.
  class CFilter *filter;
  filter = new CFilter();
  bool bTemp;

  // Perform the calculation.
  bTemp = filter->bSmoothWaveform( dDataInArray, dDataOutArray, lArrayElementCount, lHalfKernelLength );

  // Delete the class object.
  delete filter;

  // Success.
  return bTemp;
}

//////////////////////////////////////////////////////////////////////
// Performs the waveform smoothing, single pass aperiodc
//////////////////////////////////////////////////////////////////////
bool bXSmoothWaveformAperiodic(double *dDataInArray,
              double *dDataOutArray,
              long lArrayElementCount,
              long lHalfKernelLength)
{

  // Initiate class, other locals.
  class CFilter *filter;
  filter = new CFilter();
  bool bTemp;

  // Perform the calculation.
  bTemp = filter->bSmoothWaveformAperiodic( dDataInArray, dDataOutArray, lArrayElementCount, lHalfKernelLength );

  // Delete the class object.
  delete filter;

  // Success.
  return bTemp;
}

//////////////////////////////////////////////////////////////////////
// Performs the waveform smoothing, multiple pass
//////////////////////////////////////////////////////////////////////
bool bXSmoothWaveformMultiplePass(double *dDataInArray,
              double *dDataOutArray,
              long lArrayElementCount,
              long lHalfKernelLength,
              long lNumberOfPasses)
{

  // Initiate class, other locals.
  class CFilter *filter;
  filter = new CFilter();
  bool bTemp;

  // Perform the calculation.
  bTemp = filter->bSmoothWaveform( dDataInArray, dDataOutArray, lArrayElementCount, lHalfKernelLength, lNumberOfPasses );

  // Delete the class object.
  delete filter;

  // Success.
  return bTemp;
}

//////////////////////////////////////////////////////////////////////
// Interpolate a waveform
//////////////////////////////////////////////////////////////////////
bool bWaveformInterpolate (double *dDataInArray,
                                double *dWaveformTimeLength,
                                long lDataInArrayCount,
                                double *dDataOutArray,
                                long lDataOutArrayCount)
{
    return false;
}

//////////////////////////////////////////////////////////////////////
// Decimate a waveform.
//////////////////////////////////////////////////////////////////////
bool bWaveformDecimate ( double *dDataInArray,
                                double *dWaveformTimeLength,
                                long lDataInArrayCount,
                                double *dDataOutArray,
                                long lDataOutArrayCount)
{
    bool bHighPass;
    bool bLowPass;
    bool bNotchPass;
    bool bBandPass;
    bool bIsZeroPhase;
    int iSuccessFlag;
    long lDataIndex;
    long lDataOutIndex;
    double dHPCorner;
    double dLPCorner;
    double dDataStep;
    double dSamplingFrequencyIn;
    double dSamplingFrequencyOut;
    double *dFilteredWaveform = NULL;

    // Validate input arguments.
    if( lDataInArrayCount <= 0 )
    {
        return false;
    }
    if( lDataOutArrayCount <= 0 )
    {
        return false;
    }
    if( *dWaveformTimeLength <= 0 )
    {
        return false;
    }

    // Initialize local variables
    dDataStep = ( (double)lDataInArrayCount / (double)lDataOutArrayCount);
    lDataOutIndex = 0;
    bIsZeroPhase = true;
    dSamplingFrequencyIn = ( (double)lDataInArrayCount / *dWaveformTimeLength );
    dSamplingFrequencyOut = ( (double)lDataOutArrayCount / *dWaveformTimeLength );

    // Validate argumements
    if ( dDataStep <= 0.0 )
    {
        return false;
    }

    // Initialize intermediate array to hold the filtered
    // waveform
    dFilteredWaveform = new double[lDataInArrayCount];

    // Low-pass filter the input waveform to avoid aliasing as it
    // is decimated.
    bHighPass = false;
    bLowPass = true;
    bNotchPass = false;
    bBandPass = false;
    dHPCorner = ( dSamplingFrequencyOut / 2.05 );
    dLPCorner = dHPCorner;
    iSuccessFlag = FilterWaveform(dDataInArray,
                                    dFilteredWaveform,
                                    lDataInArrayCount,
                                    bHighPass,
                                    bLowPass,
                                    bNotchPass,
                                    bBandPass,
                                    bIsZeroPhase,
                                    dHPCorner,
                                    dLPCorner,
                                    dSamplingFrequencyIn,
                                    0.5,
                                    4);


    // Did the call to the filter succeed?
    if( iSuccessFlag < 0 )
    {

        // Clear out variables and memory
        delete [] dFilteredWaveform;

        // Set flag
        return false;
    }

    // Loop through the input data array and pick the points for
    // the output array.
    for( lDataIndex = 0; lDataIndex < lDataInArrayCount; lDataIndex++ )
    {
        if( ( (double)lDataOutIndex * dDataStep ) < lDataIndex )
        {
            dDataOutArray[lDataOutIndex] = dFilteredWaveform[lDataIndex];
            lDataOutIndex++;
        }
    }

    // Clear out variables and memory
    delete [] dFilteredWaveform;

    // Success
    return true;
}

//////////////////////////////////////////////////////////////////////
// External call to resample a waveform
//////////////////////////////////////////////////////////////////////
bool bXWaveformResample( double *dDataInArray,
                                double *dWaveformTimeLength,
                                long lDataInArrayCount,
                                double *dDataOutArray,
                                long lDataOutArrayCount)
{

    // Validate data
    if( lDataInArrayCount <= 0 )
    {
        return false;
    }
    if( lDataOutArrayCount <= 0 )
    {
        return false;
    }

    // Is this decimation or interpolation?
    if( lDataInArrayCount < lDataOutArrayCount )
    {
        return bWaveformInterpolate( dDataInArray, dWaveformTimeLength, lDataInArrayCount,
                                        dDataOutArray, lDataOutArrayCount);
    }
    else
    {
        return bWaveformDecimate( dDataInArray, dWaveformTimeLength, lDataInArrayCount,
                                        dDataOutArray, lDataOutArrayCount);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
// Performs the RMS characterization of a waveform
//////////////////////////////////////////////////////////////////////
bool CalcWaveformRMS(double *dDataInArray,
              long lArrayElementCount,
			  double *dRMS )
{
   	//locals
    int i;
	double dTemp;

	//initialize
	i=0;
	dTemp = 0;

	//validate data structure and count
	if( lArrayElementCount <=0 || lArrayElementCount > MAX_ARRAYSIZE )
		return false;

	//loop through and perform the calculation, essentially this is a
	//standard deviation
	dTemp = 0;
	for( i=0; i<lArrayElementCount; i++ )
	{
		dTemp = ( dTemp + ( dDataInArray[i] * dDataInArray[i] ) );
	}

	dTemp = ( ( (double)1 / (double)lArrayElementCount ) * dTemp );
	dTemp = sqrt( dTemp );

	//so far, so good, update the external memory called out
	//by the pointer
	*dRMS = dTemp;

	//success
	return true;
}

//////////////////////////////////////////////////////////////////////
// Calculates the average within a band
//////////////////////////////////////////////////////////////////////
bool bXGetBandAverage(double *dWaveform,
              long lWaveformSamples,
              long lBandAnchor,
              long lBandSampleCount,
              long lBandOffsetSampleCount,
              double *dAverage )
{

    // Locals
    int i;
    long lIndex;
    double dTotal;

    // Initialize local variables
    dTotal = 0.0;

    // Check the incoming data
    if ( lBandSampleCount <= 0 )
    {
        return false;
    }

    // Loop through the band
     for( i=0; i<lBandSampleCount; i++ )
     {

        // Calculate the index
        lIndex = (lBandAnchor - lBandOffsetSampleCount + i);

        // Adjust for end of array condition
        if (lIndex >= lWaveformSamples)
            lIndex = (lIndex - lWaveformSamples);

        // Adjust for beginning of array condition
        if (lIndex < 0)
            lIndex = (lIndex + lWaveformSamples);

        // Add to the total
        dTotal = dTotal + dWaveform[lIndex];

     }

    // So far, so good, update the external memory called out
	// by the pointer
	*dAverage = (dTotal / (double)lBandSampleCount);

    return true;
}


//////////////////////////////////////////////////////////////////////
// Calculates the average for a waveform
//////////////////////////////////////////////////////////////////////
bool bXGetWaveformAverage(double *dWaveform,
              long lWaveformSamples,
              double *dAverage )
{
    return bXGetBandAverage( dWaveform, lWaveformSamples, 0, lWaveformSamples, 0, dAverage);
}

//////////////////////////////////////////////////////////////////////
// Returns an AC-Coupled waveform
//////////////////////////////////////////////////////////////////////
bool bXGetWaveformAsACCoupled(double *dWaveform,
              long lWaveformSamples)
{
    bool bTemp;
    long lIndex;
    double dAverage;

    dAverage = 0.0;

    // Calculate the average
    bTemp = bXGetBandAverage( dWaveform, lWaveformSamples, 0, lWaveformSamples, 0, &dAverage);
    if( bTemp == false )
    {
        return false;
    }

    // Adjust the waveform
    for( lIndex=0; lIndex<lWaveformSamples; lIndex++)
    {
        dWaveform[lIndex] = (dWaveform[lIndex] - dAverage );
    }

    // Success
    return true;

}

//////////////////////////////////////////////////////////////////////
// Return the index of the largest event.  Index is relative to the
// first sample of the signal, /*not*/ the first sample of the band.
//////////////////////////////////////////////////////////////////////
bool bGetBandPeakIndex(double *dWaveform,
              long lWaveformSamples,
              long lBandAnchor,
              long lBandSampleCount,
              long lBandOffsetSampleCount,
              long *lPeakIndex )
{
    // Locals
    long lLoop;
    long lIndex;

    // Guess that the first sample in the band is the largest.
    *lPeakIndex = ( lBandAnchor - lBandOffsetSampleCount );

    // Loop through the data and see if any of the samples are
    // larger than the first.
    for( lLoop=1; lLoop<lBandSampleCount; lLoop++ )
    {

        //calculate the index
        lIndex = ( (lBandAnchor - lBandOffsetSampleCount + lLoop)  % lWaveformSamples );

        // Find the maximum
        if ((dWaveform[lLoop]) > (dWaveform[*lPeakIndex]))
            *lPeakIndex = lIndex;
     }

    // Success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Return the index of the smallest event.  Index is relative to the
// first sample of the signal, /*not*/ the first sample of the band.
//////////////////////////////////////////////////////////////////////
bool bGetBandValleyIndex(double *dWaveform,
              long lWaveformSamples,
              long lBandAnchor,
              long lBandSampleCount,
              long lBandOffsetSampleCount,
              long *lValleyIndex )
{
    // Locals
    long lLoop;
    long lIndex;

    // Guess that the first sample in the band is the largest.
    *lValleyIndex = ( lBandAnchor - lBandOffsetSampleCount );

    // Loop through the data and see if any of the samples are
    // larger than the first.
    for( lLoop=1; lLoop<lBandSampleCount; lLoop++ )
    {

        //calculate the index
        lIndex = ( (lBandAnchor - lBandOffsetSampleCount + lLoop)  % lWaveformSamples );

        // Find the minimum
        if ((dWaveform[lLoop]) < (dWaveform[*lValleyIndex]))
            *lValleyIndex = lIndex;
     }

    // Success
    return true;
}


//////////////////////////////////////////////////////////////////////
// Performs the pk-pk characterization of a band
//////////////////////////////////////////////////////////////////////
bool bGetBandPkPk(double *dWaveform,
              long lWaveformSamples,
              long lBandAnchor,
              long lBandSampleCount,
              long lBandOffsetSampleCount,
              double *dBandPkPk )
{
   	//locals
     int i;
     long lIndex;
     double dMax;
     double dMin;

     //Initialize local variables
     dMax = -1000000000000.0;
     dMin = 10000000000000.0;

     for( i=0; i<lBandSampleCount; i++ )
     {

        //calculate the index
        lIndex = (lBandAnchor - lBandOffsetSampleCount + i);

        //Adjust for end of array condition
        if (lIndex >= lWaveformSamples)
            lIndex = (lIndex - lWaveformSamples);

        //Adjust for beginning of array condition
        if (lIndex < 0)
            lIndex = (lIndex + lWaveformSamples);

        //find maximum and minimum
        if (dWaveform[lIndex] > dMax)
            dMax = dWaveform[lIndex];
        if (dWaveform[lIndex] < dMin)
            dMin = dWaveform[lIndex];
     }

    // So far, so good, update the external memory called out
	// by the pointer.
	*dBandPkPk = (dMax - dMin);

	//success
	return true;
}

//////////////////////////////////////////////////////////////////////
// Performs the root mean square (RMS) characterization of a band
//////////////////////////////////////////////////////////////////////
bool bGetBandRMS(double *dWaveform,
              long lWaveformSamples,
              long lBandAnchor,
              long lBandSampleCount,
              long lBandOffsetSampleCount,
              double *dBandRMS )
{
   	//locals
     int i;
     long lIndex;
     double dSum;

     //Initialize local variables
     dSum = 0.0;

     //validate band sample count
     if (lBandSampleCount <= 0 )
     {
         return false;
     }

     for( i=0; i<lBandSampleCount; i++ )
     {

        //calculate the index
        lIndex = (lBandAnchor - lBandOffsetSampleCount + i);

        //Adjust for end of array condition
        if (lIndex >= lWaveformSamples)
            lIndex = (lIndex - lWaveformSamples);

        //Adjust for beginning of array condition
        if (lIndex < 0)
            lIndex = (lIndex + lWaveformSamples);

        //square, add to sum
        dSum = dSum + ( dWaveform[lIndex] * dWaveform[lIndex] );

     }

     //so far, so good, update the external memory called out
	//by the pointer
	*dBandRMS = sqrt( ( 1.0 / (double)lBandSampleCount ) * dSum );

	//success
	return true;
}

//////////////////////////////////////////////////////////////////////
// Performs a linear regression on a set of data and returns the
// slope and intercept
//////////////////////////////////////////////////////////////////////
bool bGetSlopeIntercept(double *dIndepArray,
						double *dDepArray,
						long lArraySize,
						double *dSlope,
						double *dIntercept)
{

	//locals
	double dSumX;
	double dSumY;
	double dSumXX;
	double dSumXY;
	int i;

	//initialize
	dSumX = 0;
	dSumY = 0;
	dSumXX = 0;
	dSumXY = 0;

	//perform the regression.  Reference "Statistics, A Tool For
	//Social Research" pages 388 to 392
	for( i=0; i< lArraySize; i++ )
	{
		dSumX = ( dSumX + dIndepArray[i] );
		dSumY = ( dSumY + dDepArray[i] );
		dSumXX = ( dSumXX + ( dIndepArray[i] * dIndepArray[i] ) );
		dSumXY = ( dSumXY + ( dIndepArray[i] * dDepArray[i] ) );
	}

	*dSlope = ( ( (double)lArraySize * dSumXY) - ( dSumX * dSumY) );
	*dSlope = *dSlope / ( ( (double)lArraySize * dSumXX ) - (dSumX * dSumX) );

	*dIntercept = ( ( dSumY / lArraySize)  - ( *dSlope * ( dSumX / lArraySize ) ) );

	//success
	return true;
}

//////////////////////////////////////////////////////////////////////
// Converts a column of CSV values in a string to an arry of double
//////////////////////////////////////////////////////////////////////
bool bXGetColAsDoubleArray(char *pszString,
                           double *dCDFArray,
                           long lMaxCDFArrayCount,
                           long lCDFColumn,
                           long *lRowsFound )
{

     CFilterCDF *filter;
	bool bTemp;

     filter = new CFilterCDF();

     bTemp = filter->bSetStringData( pszString );
     if( !bTemp )
     {
          delete filter;
          return false;
     }

     bTemp = filter->bGetColAsDoubleArray( dCDFArray, lMaxCDFArrayCount, lCDFColumn, lRowsFound );

     delete filter;

     return bTemp;
}

//////////////////////////////////////////////////////////////////////
// Converts a row of CSV values in a string to an arry of double
//////////////////////////////////////////////////////////////////////
bool bXGetRowAsDoubleArray(char *pszString,
                           double *dCDFArray,
                           long lMaxCDFArrayCount,
                           long lCDFRow,
                           long *lColsFound )
{

     CFilterCDF *filter;
	bool bTemp;

     filter = new CFilterCDF();

     bTemp = filter->bSetStringData( pszString );
     if( !bTemp )
     {
          delete filter;
          return false;
     }

     bTemp = filter->bGetRowAsDoubleArray( dCDFArray, lMaxCDFArrayCount, lCDFRow, lColsFound );

     delete filter;

     return bTemp;
}

//////////////////////////////////////////////////////////////////////
// Converts a single record from a CSV string
//
// Revision - 22 Nov 2011
// B. Howard
// Added check to ensure buffer is not over run
//
//////////////////////////////////////////////////////////////////////
bool bXGetRecordAsString(char *pszString,
                           long lCDFRow,
                           long lCDFColumn,
                           LPSTR pszRecordString,
                           long lRecordStringSize )
{

     CFilterCDF *filter;
	 bool bTemp;
     char szTempRecordString[2048];

     filter = new CFilterCDF();

     bTemp = filter->bSetStringData( pszString );
     if( !bTemp )
     {
          delete filter;
          return false;
     }

     bTemp = filter->bGetRecordAsString( lCDFColumn, lCDFRow, szTempRecordString );

     delete filter;

     if( bTemp == false )
     {
         return false;
     }

     // Do not over-run buffer
     if( strlen( szTempRecordString ) >= (unsigned long)lRecordStringSize )
     {
         return false;
     }

     strcpy_s( pszRecordString, lRecordStringSize, szTempRecordString );

     return bTemp;
}

//////////////////////////////////////////////////////////////////////
//
// Revision.  4 April 2014, Brian Howard
//
//////////////////////////////////////////////////////////////////////
// Given an array of double values, this function returns a
// comma-separated value (CSV) string.  Useful for importing data 
// into applications via the clipboard.
//////////////////////////////////////////////////////////////////////
bool bXGetStringFromArrayToRow(LPSTR pszString,
                           long *lStringSize,
                           double *dInputArray01,
                           long lArrayElementCount)
{


    //locals
    long lIndex;
    std::ostringstream o;

    // Set precision
    o << std::setprecision(12);

    //validate string length
    if( *lStringSize <= 0 )
    {
        return false;
    }

    //construct the string
    for( lIndex=0;lIndex<(lArrayElementCount-1); lIndex++ )
    {
        //begin conversion to string, append comma, new line
        if( !( o << dInputArray01[lIndex] ) )
        {
            return false;
        }
        o << +",";
    }
    o << dInputArray01[lIndex];

	// Truncate the string so it only contains data of interest
	//o.seekp(0,ios::end);
	*lStringSize = (long)o.tellp()+1;

	//copy the string over
    strcpy_s( pszString, *lStringSize, o.str().c_str());

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Given an array of doubles, a starting time and an ending time
// this function returns a comma-separated value (CSV) two column
// string.  Useful for importing data into applications via the
// clipboard
//////////////////////////////////////////////////////////////////////
bool bXGetStringFromArrayAndTime(LPSTR pszString,
                           long lStringSize,
                           double *dInputArray,
                           long lArrayElementCount,
                           double dStartingTime,
                           double dEndingTime )
{


    //locals
    double dDelta;
    long lIndex;
    std::ostringstream o;

    // Set precision
    o << std::setprecision(12);

    //validate string length
    if( lStringSize <= 0 )
    {
        return false;
    }

    //initialize variables
    dDelta = ( ( dEndingTime - dStartingTime ) / (double)lArrayElementCount );

    //construct the string
    for( lIndex=0;lIndex<lArrayElementCount; lIndex++ )
    {
        //begin conversion to string, append comma, new line
        if( !( o << ( dDelta * (double)lIndex )) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray[lIndex]) )
        {
            return false;
        }
        o << +"\n";

    }

    //copy the string over
    strcpy_s( pszString, lStringSize, o.str().c_str());

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Given two arrays of doubles, this function returns a
// comma-separated value (CSV) two column
// string.  Useful for importing data into applications via the
// clipboard
//////////////////////////////////////////////////////////////////////
bool bXGetStringFromArrays02(LPSTR pszString,
                           long lStringSize,
                           double *dInputArray01,
                           long lArrayElementCount,
                           double *dInputArray02 )
{


    //locals
    long lIndex;
    std::ostringstream o;

    // Set precision
    o << std::setprecision(12);

    //validate string length
    if( lStringSize <= 0 )
    {
        return false;
    }

    //construct the string
    for( lIndex=0;lIndex<lArrayElementCount; lIndex++ )
    {
        //begin conversion to string, append comma, new line
        if( !( o << dInputArray01[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray02[lIndex] ) )
        {
            return false;
        }
        o << +"\n";

    }

    //copy the string over
    strcpy_s( pszString, lStringSize, o.str().c_str());

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Given three arrays of doubles, this function returns a
// comma-separated value (CSV) two column
// string.  Useful for importing data into applications via the
// clipboard
//////////////////////////////////////////////////////////////////////
bool bXGetStringFromArrays03(LPSTR pszString,
                           long lStringSize,
                           double *dInputArray01,
                           long lArrayElementCount,
                           double *dInputArray02,
                           double *dInputArray03)
{


    //locals
    long lIndex;
    std::ostringstream o;

    // Set precision
    o << std::setprecision(12);

    //validate string length
    if( lStringSize <= 0 )
    {
        return false;
    }

    //construct the string
    for( lIndex=0;lIndex<lArrayElementCount; lIndex++ )
    {
        //begin conversion to string, append comma, new line
        if( !( o << dInputArray01[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray02[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray03[lIndex] ) )
        {
            return false;
        }
        o << +"\n";

    }

    //copy the string over
    strcpy_s( pszString, lStringSize, o.str().c_str());

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Given four arrays of doubles, this function returns a
// comma-separated value (CSV) two column
// string.  Useful for importing data into applications via the
// clipboard
//////////////////////////////////////////////////////////////////////
bool bXGetStringFromArrays04(LPSTR pszString,
                           long lStringSize,
                           double *dInputArray01,
                           long lArrayElementCount,
                           double *dInputArray02,
                           double *dInputArray03,
                           double *dInputArray04)
{


    //locals
    long lIndex;
    std::ostringstream o;

    // Set precision
    o << std::setprecision(12);

    //validate string length
    if( lStringSize <= 0 )
    {
        return false;
    }

    //construct the string
    for( lIndex=0;lIndex<lArrayElementCount; lIndex++ )
    {
        //begin conversion to string, append comma, new line
        if( !( o << dInputArray01[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray02[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray03[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray04[lIndex] ) )
        {
            return false;
        }
        o << +"\n";

    }

    //copy the string over
    strcpy_s( pszString, lStringSize, o.str().c_str());

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Given five arrays of doubles, this function returns a
// comma-separated value (CSV) column string.  
// Useful for importing data into applications via the
// clipboard
//////////////////////////////////////////////////////////////////////
bool bXGetStringFromArrays05(LPSTR pszString,
                           long lStringSize,
                           double *dInputArray01,
                           long lArrayElementCount,
                           double *dInputArray02,
                           double *dInputArray03,
                           double *dInputArray04, 
                           double *dInputArray05)
{


    //locals
    long lIndex;
    std::ostringstream o;

    // Set precision
    o << std::setprecision(12);

    //validate string length
    if( lStringSize <= 0 )
    {
        return false;
    }

    //construct the string
    for( lIndex=0;lIndex<lArrayElementCount; lIndex++ )
    {
        //begin conversion to string, append comma, new line
        if( !( o << dInputArray01[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray02[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray03[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray04[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray05[lIndex] ) )
        {
            return false;
        }
        o << +"\n";

    }

    //copy the string over
    strcpy_s( pszString, lStringSize, o.str().c_str());

    //success
    return true;
}

//////////////////////////////////////////////////////////////////////
// Given six arrays of doubles, this function returns a
// comma-separated value (CSV) column string.  
// Useful for importing data into applications via the
// clipboard
//////////////////////////////////////////////////////////////////////
bool bXGetStringFromArrays06(LPSTR pszString,
                           long lStringSize,
                           double *dInputArray01,
                           long lArrayElementCount,
                           double *dInputArray02,
                           double *dInputArray03,
                           double *dInputArray04, 
                           double *dInputArray05, 
                           double *dInputArray06)
{


    //locals
    long lIndex;
    std::ostringstream o;

    // Set precision
    o << std::setprecision(12);

    //validate string length
    if( lStringSize <= 0 )
    {
        return false;
    }

    //construct the string
    for( lIndex=0;lIndex<lArrayElementCount; lIndex++ )
    {
        //begin conversion to string, append comma, new line
        if( !( o << dInputArray01[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray02[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray03[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray04[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray05[lIndex] ) )
        {
            return false;
        }
        o << +",";
        if( !( o << dInputArray06[lIndex] ) )
        {
            return false;
        }
        o << +"\n";

    }

    //copy the string over
    strcpy_s( pszString, lStringSize, o.str().c_str());

    //success
    return true;
}