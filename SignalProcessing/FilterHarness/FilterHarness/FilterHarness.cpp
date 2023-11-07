// FilterHarness.cpp : Defines the entry point for the console application.
//
///////////////////////////////////////////////////////////////////////////
// Revision
// 
// Modified to include a smoothing function that mimics the smoothing
// step in Decision Support.
// 
// Brian Howard
// 18 Sep 2012
///////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <cstdio>
#include <string.h>
#include "Filter.h"


// Added this short function to compare two doubles to 
// allow for some small round-off error.  The technique
// of comparing relative absolute values is discussed in
// an articl at
// http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
// see that link for more details.
//
// Brian Howard
// 13 November 2012
bool bCompareDoubles( double dA, double dB )
{
    double dMaxRelDiff = 0.00001;

    // Calculate the difference.
    double dDiff = fabs(dA - dB);
    dA = fabs(dA);
    dB = fabs(dB);
    
    // Find the largest
    double dLargest = (dB > dA) ? dB : dA;
 
    // How's this look?
    if (dDiff <= dLargest * dMaxRelDiff)
        return true;

    // Didn't look so good.
    return false;
}

bool DisplayMembers( CFilter *filter, FILE *stream)
{
  long i,lKSize;
  bool bFlag;
  double dTemp1, dTemp2;

  bFlag = filter->bGetMaxKernelSize( &lKSize );

  fprintf(stream,"Maximum Kernel Size: %i\n", lKSize );
  
  //post the corner values
  filter->bGetHPCorner( &dTemp1 );
  fprintf(stream,"HP Filter Corner: %f\n", dTemp1);
  filter->bGetLPCorner( &dTemp1 );
  fprintf(stream,"LP Filter Corner: %f\n", dTemp1);

  //post the sampling frequency
  filter->bGetSamplingFrequency( &dTemp1 );
  fprintf(stream,"Sampling Frequency: %f\n", dTemp1);

  // Post the percent ripple
  filter->bGetPercentRipple( &dTemp1 );
  fprintf(stream,"Percent Ripple: %f\n", dTemp1);

  // Post the number of polse
  filter->bGetNumberPoles( &lKSize );
  fprintf(stream,"Number of Poles: %i\n", lKSize);

  //post the filter type
  filter->bGetHighPass( &bFlag );
  if( bFlag )
    fprintf(stream,"High Pass Filter Type\n");
  else
    fprintf(stream,"Low Pass Filter Type\n");


    //post the kernel contents
    for(i=0;i<lKSize;i++)
    {
    filter->bGet_A( &dTemp1, i );
    filter->bGet_B( &dTemp2, i );
    fprintf(stream,"A[%i] - %f    B[%i] - %f\n", i, dTemp1, i, dTemp2 );
    }

    //this space makes it easier to select the relevant data sets
    fprintf(stream," \n");

    return true;
}

#define ARRAYSIZE 32768

int _tmain(int argc, _TCHAR* argv[])
{
    //local variables
    FILE *stream;
    char outputline[2048];
    class CFilter *filter;
    int i;
    double* dData = new double[ARRAYSIZE];
    double* dFilteredData = new double[ARRAYSIZE];
    long lDataIndex;
    double dPi;
    double dCoefficient;
    double dSamplingFrequency;
    bool bTestOK;
    errno_t err;

    //initialize local variables
    lDataIndex = (long)ARRAYSIZE;
    dPi = 3.1415926535897932384626433832795;

    // Open the log file.
    err = fopen_s( &stream, "FilterHarness.csv" , "w");
    if( err != 0 )
    {
	    return 0;
    }

	// Instantiate the class.
	try
    {
        filter = new CFilter();
    }
    catch (...)
    {
		strcpy_s( outputline, "CFilter Instantiation failed\n" );
		printf( outputline );
		fprintf(stream, outputline );
    }

    // Document instantiation of class.
	if( filter == NULL )
		return 0;
	else
	{
		strcpy_s( outputline, "CFilter Instantiated.\n" );
		printf( outputline );
		fprintf(stream, outputline );
	}
    if( !DisplayMembers( filter, stream) )
    {
        fprintf(stream, "DisplayMembers failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Construct the test waveform.
    for(i=0;i<lDataIndex;i++)
    {
        dData[i] = sin( 40 * i * dPi / (double)lDataIndex );
        dData[i] = dData[i] + sin( 400 * i * dPi / (double)lDataIndex );
    }

    // Added this section to check the recursion coefficient values
    // for zero corner values.  Reference Olga's e-mail of 30 Nov 2012
    // B. Howard
    // 7 December 2012
    //
    // A zero-valued sampling frequency should not be allowed
    dSamplingFrequency = ( (double)lDataIndex * 2.8 );
    if( filter->bSetSamplingFrequency( (dSamplingFrequency * 0.00) ) )
    {
		strcpy_s( outputline, "The filter object failed to trap a zero value sampling frequency.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Zero-valued corners should not be allowed
    if( filter->bSetLPCorner( dSamplingFrequency * 0.00 ) )
    {
		strcpy_s( outputline, "The filter object failed to trap a zero value low-pass corner.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }
    if( filter->bSetHPCorner( dSamplingFrequency * 0.00 ) )
    {
		strcpy_s( outputline, "The filter object failed to trap a zero value high-pass corner.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Added this section to check the recursion coefficient values.
    // B. Howard
    // 13 November 2012
    //
    // Configure the filter for low-pass with an fc of
    // 0.01, where fc equals the corner frequency 
    // divided by the sampling frequency.
    dSamplingFrequency = ( (double)lDataIndex * 2.8 );
    if( !filter->bSetSamplingFrequency( dSamplingFrequency ) )
    {
		strcpy_s( outputline, "bSetSamplingFrequency failed for a valid sampling frequency.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    if( !filter->bSetLPCorner( dSamplingFrequency * 0.01 ) )
    {
        fprintf(stream, "bSetLPCorner failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Check the results
    if( !filter->bGet_A( &dCoefficient, 0 ) )
    {
        fprintf(stream, "bGet_A failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    if( !bCompareDoubles( 4.149425e-07, dCoefficient ) )
    {
		strcpy_s( outputline, "The recursion coefficients did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Check the results
    if( !filter->bGet_B( &dCoefficient, 2 ) )
    {
        fprintf(stream, "bGet_A failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    if( !bCompareDoubles( -5.688233, dCoefficient ) )
    {
		strcpy_s( outputline, "The recursion coefficients did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Configure the filter for a low-pass corner of
    // 150 Hertz without zero-phase
    filter->bSetLPCorner( 150.00 );
    filter->bSetZeroPhase( false );
    dSamplingFrequency = ( (double)lDataIndex * 2.8 );
    if( !filter->bSetSamplingFrequency( dSamplingFrequency ) )
    {
        fprintf(stream, "bSetSamplingFrequency failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the default low-pass filter
    if( !filter->bFilterWaveform( dData, dFilteredData, lDataIndex ))
    {
        fprintf(stream, "bFilterWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
	fprintf(stream, "Outputting raw and LP filtered waveforms.\n" );
	fprintf(stream, "Index, Unfiltered, Filtered.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%0.10f,%0.10f\n", i, dData[i], dFilteredData[i] );

        // Test cases.
        switch(i)
        {
        case 10:
            {
                if( !bCompareDoubles( -0.88679757845, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 480:
            {
                if( !bCompareDoubles( 0.659096110358, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }

    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The LP filtered waveform results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

   // Configure the filter for a low-pass corner of
    // 150 Hertz with zero-phase
    filter->bSetLPCorner( 150.00 );
    filter->bSetZeroPhase( true );
    dSamplingFrequency = ( (double)lDataIndex * 2.8 );
    if( !filter->bSetSamplingFrequency( dSamplingFrequency ) )
    {
        fprintf(stream, "bSetSamplingFrequency failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the default low-pass filter
    if( !filter->bFilterWaveform( dData, dFilteredData, lDataIndex ))
    {
        fprintf(stream, "bFilterWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
	fprintf(stream, "Outputting raw and LP filtered waveforms.\n" );
	fprintf(stream, "Index, Unfiltered, Filtered.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%0.10f,%0.10f\n", i, dData[i], dFilteredData[i] );

        // Test cases.
        switch(i)
        {
        case 10:
            {
                if( !bCompareDoubles( 0.0386569082, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 480:
            {
                if( !bCompareDoubles( 0.9716822249, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }

    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The LP filtered waveform results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }
    
    // Modify the filter object characteristics
    if( !filter->bSetHighPass( true ))
    {
        fprintf(stream, "bSetHighPass failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the filter.
    if( !filter->bFilterWaveform( dData, dFilteredData, lDataIndex ))
    {
        fprintf(stream, "bFilterWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
	fprintf(stream, "Outputting raw and HP filtered waveforms.\n" );
	fprintf(stream, "Index, Unfiltered, Filtered.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%0.10f,%0.10f\n", i, dData[i], dFilteredData[i] );
 
        // Test cases.
        switch(i)
        {
        case 10:
            {
                if( !bCompareDoubles( 0.3778445155, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 480:
            {
                if( !bCompareDoubles( -0.4316698913, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The HP filtered waveform results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Revision.  17 Jan 2013, B. Howard
    // Added this section to check higher-order poles

    // Modify the filter object characteristics for a 6-pole filter
    if( !filter->bSetNumberPoles( 6 ))
    {
        fprintf(stream, "bSetNumberPoles failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Set the corner frequency ratio at 0.02 = 1835 Hz
    if( !filter->bSetHPCorner( 1835) )
    {
        fprintf(stream, "bSetHP Corner failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the filter.
    if( !filter->bFilterWaveform( dData, dFilteredData, lDataIndex ))
    {
        fprintf(stream, "bFilterWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
	fprintf(stream, "Outputting raw and HP filtered waveforms with 6 poles and Fc=0.02 (ref. Table 20-3).\n" );
	fprintf(stream, "Index, Unfiltered, Filtered.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%0.10f,%0.10f\n", i, dData[i], dFilteredData[i] );
 
        // Test cases.
        switch(i)
        {
        case 11:
            {
                if( !bCompareDoubles( 0.0000000077195993806255907, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 2602:
            {
                if( !bCompareDoubles( -0.000000012759790747373141, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The HP filtered waveform (with 6 poles, Fc = 0.02) results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Revision.  23 Mar 2015, B. Howard
    // Added this section to allow 4-pole high pass coefficients to be
    // calculated and compared to those presented in Table 20-2
    // Modify the filter object characteristics for a 6-pole filter
    if (!filter->bSetNumberPoles(4))
    {
        fprintf(stream, "bSetNumberPoles failed.\n");
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Set the corner frequency ratio at 0.20 = 18350 Hz
    if (!filter->bSetHPCorner(18350))
    {
        fprintf(stream, "bSetHP Corner failed.\n");
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the filter.
    if (!filter->bFilterWaveform(dData, dFilteredData, lDataIndex))
    {
        fprintf(stream, "bFilterWaveform failed.\n");
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
    fprintf(stream, "Outputting filter coefficients with 4 poles and Fc=0.20 (ref. Table 20-2).\n");

    // Document results
    if (!DisplayMembers(filter, stream))
    {
        fprintf(stream, "DisplayMembers failed.\n");
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }



    // Success, so now move the Fc to 0.25 and increase the pole count 
    // to 12.  According to Table 20-3 this should still be a stable
    // filter.  Note that this does a pretty good job of squashing the 
    // output.  To validate results a unit impulse will be introduced at 
    // the second element.
    dData[1] = 1.0;

    // Modify the filter object characteristics for a 14-pole filter
    if( !filter->bSetNumberPoles( 14 ))
    {
        fprintf(stream, "bSetNumberPoles failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Set the corner frequency ratio at 0.25 = 22937.6 Hz
    if( !filter->bSetHPCorner( 22937.6 ) )
    {
        fprintf(stream, "bSetHP Corner frequency to 22937.6 Hz failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the filter.
    if( !filter->bFilterWaveform( dData, dFilteredData, lDataIndex ))
    {
        fprintf(stream, "bFilterWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
	fprintf(stream, "Outputting raw and HP filtered waveforms with 14 poles and Fc=0.25 (ref. Table 20-3).\n" );
	fprintf(stream, "Index, Unfiltered, Filtered.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%0.10f,%0.10f\n", i, dData[i], dFilteredData[i] );
 
        // Test cases.
        switch(i)
        {
        case 1:
            {
                if( !bCompareDoubles( 0.4819548386, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 11:
            {
                if( !bCompareDoubles( -0.0006342501, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }    
    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The HP filtered waveform (with 12 poles, Fc = 0.25) results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    //---------------------------------------------------------------------------------------------------------------------------
    // Modify the filter object characteristics for a 2-pole filter
    if( !filter->bSetNumberPoles( 2 ))
    {
        fprintf(stream, "bSetNumberPoles failed for 2 poles.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the filter.
    if( !filter->bFilterWaveform( dData, dFilteredData, lDataIndex ))
    {
        fprintf(stream, "bFilterWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
	fprintf(stream, "Outputting raw and HP filtered waveforms with 2 poles and Fc=0.25 (ref. Table 20-3).\n" );
	fprintf(stream, "Index, Unfiltered, Filtered.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%0.10f,%0.10f\n", i, dData[i], dFilteredData[i] );
 
        // Test cases.
        switch(i)
        {
        case 1:
            {
                if( !bCompareDoubles( 0.487248604, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 11:
            {
                if( !bCompareDoubles( 0.0000880871, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }    
    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The HP filtered waveform (with 2 poles, Fc = 0.25) results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    //---------------------------------------------------------------------------------------------------------------------------
    // Set the corner frequency ratio at 0.001 = 91.75 Hz
    if( !filter->bSetHPCorner( 91.75 ) )
    {
        fprintf(stream, "bSetHP Corner frequency to 91.75 Hz failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Pass the data through the filter.
    if( !filter->bFilterWaveform( dData, dFilteredData, lDataIndex ))
    {
        fprintf(stream, "bFilterWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Output the data and spot check the results.
    bTestOK = true;
	fprintf(stream, "Outputting raw and HP filtered waveforms with 2 poles and Fc=0.001 (ref. Table 20-3).\n" );
	fprintf(stream, "Index, Unfiltered, Filtered.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%0.10f,%0.10f\n", i, dData[i], dFilteredData[i] );
 
        // Test cases.
        switch(i)
        {
        case 1:
            {
                if( !bCompareDoubles( 0.9949017061, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 11:
            {
                if( !bCompareDoubles( 0.4142500852, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }    
    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The HP filtered waveform (with 2 poles, Fc = 0.001) results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    //---------------------------------------------------------------------------------------------------------------------------
    // Remove the unit impulse
    dData[1] = 0.042175062943258922;

    // Pass the data through the smoother.
    if( !filter->bSmoothWaveform( dData, dFilteredData, lDataIndex, 20 ))
    {
        fprintf(stream, "bSmoothWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

	fprintf(stream, "Outputting raw and smoothed waveforms.\n" );
	fprintf(stream, "Index, Unfiltered, Smoothed.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%f,%f\n", i, dData[i], dFilteredData[i] );
    }
    fprintf(stream,",,,\n");

    // Pass the data through the smoother, 5 passes
    if( !filter->bSmoothWaveform( dData, dFilteredData, lDataIndex, 20, 5 ))
    {
        fprintf(stream, "bSmoothWaveform failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

	fprintf(stream, "Outputting raw and 5-pass smoothed waveforms.\n" );
	fprintf(stream, "Index, Unfiltered, Smoothed.\n" );
    for(i=0;i<lDataIndex;i++)
    {
        fprintf(stream, "%i,%f,%f\n", i, dData[i], dFilteredData[i] );
    }


    // Begin unit testing for the aperiodic moving average filter
    dData[0] = 1;
    dData[1] = 2;
    dData[2] = 2;
    dData[3] = 3;
    dData[4] = 4;
    dData[5] = 5;
    if( !filter->bSmoothWaveformAperiodic( dData, dFilteredData, 6, 2))
    {
        fprintf(stream, "bSmoothWaveformAperiodic failed.\n" );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

 	fprintf(stream, "Outputting raw and aperiodic smoothed waveform.\n" );
	fprintf(stream, "Index, Unfiltered, Smoothed.\n" );
    for(i=0;i<6;i++)
    {
        fprintf(stream, "%i,%f,%f\n", i, dData[i], dFilteredData[i] );
    }
    for(i=0;i<2;i++)
    {
 
        // Test cases.
        switch(i)
        {
        case 0:
            {
                if( !bCompareDoubles( 2.4, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        case 1:
            {
                if( !bCompareDoubles( 3.2, dFilteredData[i] ) )
                {
                    bTestOK = false;
                }
                break;
            }
        default:
            {
                break;
            }
        }    
    }
    fprintf(stream,",,,\n");

    // Did the spot-checks succeed?
    if( !bTestOK )
    {
		strcpy_s( outputline, "The HP filtered waveform (with 2 poles, Fc = 0.001) results did not match expected values.\n" );
		printf( outputline );
		fprintf(stream, outputline );
        delete[] dData;
        delete[] dFilteredData;
        delete filter;
        return 0;
    }

    // Delete the class object.
    delete filter;

    // Document destruction of object.
 	strcpy_s( outputline, "Test Complete Successfully and CFilter Destructed.\n" );
	printf( outputline );
    fprintf(stream, outputline );

	// Close the output file.
	fclose( stream );

    // Delete the arrays.
    delete[] dData;
    delete[] dFilteredData;

    // Done.
    return 0;
}

