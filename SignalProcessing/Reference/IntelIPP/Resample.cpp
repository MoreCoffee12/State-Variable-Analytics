///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004 Intel Corporation 
// All rights reserved. 
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met: 
//
// * Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer. 
// * Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution. 
// * Neither name of Intel Corporation nor the names of its contributors 
// may be used to endorse or promote products derived from this software 
// without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ipp.h"
#include "tools.h"
#include "wave.h"

int main(int argc, char* argv[])
{
	int len, filterLen, delayLen, i;
	Ipp32f* pTaps;
	Ipp16s* pDelayLine;

	WaveFile wf;
	if (argc<1)
	{
		printf("Usage: Resample.exe <wavefile>\n");
		return -1;
	}
	if (wf.Open(argv[1]))
	{
		printf("Can't open file %s\n", argv[1]);
		return -1;
	}

	WaveMem wm, wmd;
	wm.ReadFromFile(&wf);
	//	wm.Play(0);

	len = wm.GetLen();

	wmd.CopySettings(&wm);
	wmd.SetSamplesPerSec(8000);
	int upSample = wmd.GetSamplesPerSec()/100,
		downSample = wm.GetSamplesPerSec()/100;
	wmd.Alloc(len*upSample/downSample);
	
	int offset = 8500, viewLen = 200;
	spView_16s(wm.GetShortBuffer()+offset, viewLen,
			   "Source", 0);

	// Linear interpolation filter

	filterLen = upSample*2-1;
	pTaps = ippsMalloc_32f(filterLen);
	ippsVectorRamp_32f(pTaps, upSample,
		1.0/upSample, 1.0/upSample);
	ippsVectorRamp_32f(pTaps+upSample-1, upSample,
		1.0, -1.0/upSample);
	spView_32f(pTaps, filterLen, "Filter", 0);

	// Set up delay line
	delayLen = (filterLen+upSample-1)/upSample;
	pDelayLine = ippsMalloc_16s(delayLen);
	ippsZero_16s(pDelayLine, delayLen);

	ippsFIRMR32f_Direct_16s_Sfs(
		wm.GetShortBuffer(), wmd.GetShortBuffer(),
		len/downSample,	pTaps, filterLen,
		upSample, 1, downSample, 0, pDelayLine, 0);

	spView_16s(wmd.GetShortBuffer()+
			   offset*upSample/downSample,
			   viewLen*upSample/downSample,
			   "Destination", 1);

	// Print out some samples
	/*
	for (i=10; i<30; i++)
	{
		offset = 9000;
		printf("S %d: %d\n", i+offset,
			   (int)wm.GetShortBuffer()[i+offset]);
		printf("D %d: %d\n", (i+offset)*upSample/downSample,
			   (int)wmd.GetShortBuffer()[(i+offset)*upSample/downSample]);
		printf("D %d: %d\n", (i+offset)*upSample/downSample+1,
			   (int)wmd.GetShortBuffer()[(i+offset)*upSample/downSample+1]);
	}
	*/

	// Should be the same:
	//	wmd.Play(0);

	ippsFree(pTaps);
	ippsFree(pDelayLine);

	return 0;
}
