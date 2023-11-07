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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <ipp.h>

#include "wave.h"

//
// WaveFile class definition
//

WaveFile::~WaveFile()
{
	if (pFilename_) ippsFree(pFilename_);
	if (pHead_) ippsFree(pHead_);
}

// Open a wave file and parse the header
int WaveFile::Open(const char* pFilename)
{
	char pHead[12];
	FILE* pFile = fopen(pFilename, "rb");
	if (!pFile) return -1;
	pFilename_ = (char*)ippsMalloc_8s(strlen(pFilename));
	strcpy(pFilename_, pFilename);

	fread(pHead, 1, 12, pFile);

#define TOINT(x) (*(int*)&(x))
#define TOSHORT(x) (*(short*)&(x))
	
	// Extract key information, such as type, format, sample rate,
	//   length, etc.
	// return if not WAVE data in a RIFF file

	// pHead[0] == "RIFF"
	if (TOINT(pHead[0]) != 0x46464952)
		{ fclose(pFile); return -1; }
	// pHead[8] == "WAVE"
	if (TOINT(pHead[8]) != 0x45564157)
		{ fclose(pFile); return -1; }

	int headMax = TOINT(pHead[4]);
	pHead_ = (char*)ippsMalloc_8s(headMax);
	ippsCopy_8u((Ipp8u*)pHead, (Ipp8u*)pHead_, 12);
	fread(pHead_+12, 1, headMax-12, pFile);

	// pHead_[8] == "fmt "
	if (TOINT(pHead_[12]) != 0x20746d66)
		{ fclose(pFile); return -1; }

	NumChannels = TOSHORT(pHead_[22]);
	SampleRate = TOINT(pHead_[24]);
	BitsPerSample = TOSHORT(pHead_[34]);
	BytesPerSample = (BitsPerSample+7)/8;
	if (BytesPerSample > 4)
		{ fclose(pFile); return -1; }
	ShiftBits = BytesPerSample*8 - BitsPerSample;

	hSize_ = TOINT(pHead_[16]) + 20;

	while (hSize_ < headMax-8)
	{
		if (TOINT(pHead_[hSize_]) != 0x61746164)
		{
			hSize_ += TOINT(pHead_[hSize_+4]) + 8;
		}
		else
		{
			DataLength = TOINT(pHead_[hSize_+4]);
			BytesPerElem = NumChannels*BytesPerSample;
			NumSamples = DataLength / BytesPerSample;
			hSize_ += 8;
			fclose(pFile);
			isValid_ = 1;
			return 0;
		}
	}
	fclose(pFile);
	return -1;
#undef TOINT
#undef TOSHORT
}

// Write new samples, using the same header information
// Requires a pre-constructed header from a valid wave file
int WaveFile::WriteAs(const short* pSamples, const char* pFilename)
{
	if (!isValid_) return -1;
	int bufSize = 1024;
	char* pBuf = (char*)ippsMalloc_8s(bufSize);
	int rawElem = bufSize / BytesPerElem;
	FILE * pWFile = fopen(pFilename, "wb");
	if (!pWFile) { ippsFree(pBuf); return -1;}
	strcpy(pFilename_, pFilename);

	fwrite(pHead_, 1, hSize_, pWFile);

	int i, j=0;
	for (i=0; i<NumSamples; i++)
	{
		if (j>bufSize-BytesPerSample)
		{
			fwrite(pBuf, 1, j, pWFile);
			j=0;
		}
		if (BytesPerSample == 1)
		{
			*(unsigned char*)(pBuf+j) =
				(unsigned char) (pSamples[i] << ShiftBits);
			j++;
		}	
		else
		{
			*(short*)(pBuf+j) = pSamples[i] << ShiftBits;
			j++;
		}
	}
	if (j>0) fwrite(pBuf, 1, j, pWFile);

	ippsFree(pBuf);
	fclose(pWFile);
	return 0;
}

// Read the data into an array of short elements
// Converts from byte if necessary
int WaveFile::ReadAsShort(short* pSamples, int numElem) const
{
	if (!isValid_) return -1;
	int i,j, sampnum=0;
	int rawsize = numElem * BytesPerElem;
	int bufSize = 65536;
	int rawelem = bufSize / BytesPerElem;
	char* pBuf = (char*)ippsMalloc_8s(bufSize);

	FILE* pFile = fopen(pFilename_, "rb");
	int sizeread = fread( pBuf, 1, hSize_, pFile);
	for (i=0; (i<rawsize) && sizeread; i+=sizeread*BytesPerElem)
	{
		sizeread = fread( pBuf, BytesPerElem, rawelem, pFile);
		if (BytesPerSample == 1)
			for (j=0; (j<sizeread*BytesPerElem)&&(sampnum<numElem); j+=BytesPerSample)
			{
				pSamples[sampnum] = (unsigned char)pBuf[j];
				pSamples[sampnum++] >>= ShiftBits;
			}
		else
			for (j=0; (j<sizeread*BytesPerElem)&&(sampnum<numElem); j+=BytesPerSample)
			{
				pSamples[sampnum] = *(short*)&pBuf[j];
				pSamples[sampnum++] >>= ShiftBits;
			}
	}
//	free(pBuf);
	fclose(pFile);
	ippsFree(pBuf);
	return sampnum;
}

int WaveFile::ReadAsInt(int* samples, int nSamp) const
{
	// Placeholder

	return -1;
}

// Play audio directly from file

int WaveFile::Play()
{
	return (int)PlaySound( pFilename_, NULL, SND_FILENAME);
}

int WaveFile::Play(const char* pFilename)
{
	return (int)PlaySound( pFilename, NULL, SND_FILENAME);
}

//
// WaveMem class definition
//

// WaveMem holds playable sound data, but has no header information,
// no dependence on a particular file, and and no ability to write to
// a wave file

short* WaveMem::GetShortBuffer()
{
	if (bytesPerSample_ == 2) return (short*)pBuffer_;
	else return 0;
}

const short* WaveMem::GetShortBuffer() const
{
	if (bytesPerSample_ == 2) return (short*)pBuffer_;
	else return 0;
}

int WaveMem::ReadFromFile(const WaveFile* pWaveFile)
{
	InitAlloc(pWaveFile->GetNumSamples(), pWaveFile->GetSampleBits(),
			  pWaveFile->GetSampleRate(), pWaveFile->GetNumChannels());

	len_ = pWaveFile->ReadAsShort((short*)pBuffer_, len_) /
		pWaveFile->GetNumChannels();
	return len_;
}

int WaveMem::CopySettings(const WaveMem* pWaveMem)
{
	bitsPerSample_ = pWaveMem->bitsPerSample_;
	bytesPerSample_ = (pWaveMem->bitsPerSample_+7)/8;
	samplesPerSec_ = pWaveMem->samplesPerSec_;
	nChannels_ = pWaveMem->nChannels_;

	playReady_ = 0;
	memReady_ = 0;

	return 0;
}

int WaveMem::Init(int bitsPerSample, int samplesPerSec, int nChannels)
{
	bitsPerSample_ = bitsPerSample;
	bytesPerSample_ = (bitsPerSample+7)/8;
	samplesPerSec_ = samplesPerSec;
	nChannels_ = nChannels;

	playReady_ = 0;
	memReady_ = 0;

	return 0;
}

int WaveMem::Free()
{
	if (pBuffer_) ippsFree(pBuffer_);
	pBuffer_ = 0;
	memReady_ = 0;
	len_ = 0;
	return 0;
}

int WaveMem::Alloc(int len)
{
	if (memReady_) return 0;
	if (pBuffer_) ippsFree(pBuffer_);

	bufSize_ = len*bytesPerSample_*nChannels_;
	if ((pBuffer_ = ippsMalloc_8u(bufSize_)) == 0) return -1;

	len_ = len;
	memReady_ = 1;

	return 0;
}

void CALLBACK playCallback(HWAVEOUT hWaveOut, UINT message,
		DWORD data, DWORD param1, DWORD param2)
{
	if (message == WOM_DONE)
	{
		waveOutReset(hWaveOut);
		waveOutClose(hWaveOut);
	}
	else if (message == WOM_OPEN)
	{
	}
}


int WaveMem::Play(int wait)
{
	HWAVEOUT hWaveOut_;
	MMRESULT      mmrs;
	WAVEFORMATEX  wfx;

	// Set up PCMWAVEFORMAT for 44.1 kHz 16-bit stereo.
	wfx.wFormatTag      = WAVE_FORMAT_PCM;
	wfx.nSamplesPerSec  = samplesPerSec_;
	wfx.nChannels       = nChannels_;
	wfx.nAvgBytesPerSec = samplesPerSec_*nChannels_*bitsPerSample_;
	wfx.nBlockAlign     = 4;
	wfx.wBitsPerSample  = bitsPerSample_;
	wfx.cbSize          = 0;

	// See if format is supported by any device in the system.
	mmrs = waveOutOpen(
					   0,                // ptr can be NULL for query
					   WAVE_MAPPER,         // The device identifier 
					   &wfx,                // Defines the requested format 
					   0,                   // No callback 
					   0,                   // No instance data 
					   WAVE_FORMAT_QUERY);  // Query only, do not open
	if(mmrs != 0) return -1;

	WAVEHDR head;
	ippsSet_8u(0, (unsigned char*)&head, sizeof(WAVEHDR));
	head.dwBufferLength = bufSize_;
	head.dwLoops = 1;
	head.lpData = (char*) pBuffer_;

	mmrs = waveOutOpen(
	   &hWaveOut_,
	   WAVE_MAPPER,
	   &wfx,
		0,
		0,
	   CALLBACK_FUNCTION);

	if (mmrs != MMSYSERR_NOERROR) return -1;

	// prepare wave-out header
	if(waveOutPrepareHeader(hWaveOut_,&head,sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
		return -1;

	if(waveOutWrite(hWaveOut_,&head,sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
		return -1;

	while (!(head.dwFlags & WHDR_DONE))
		Sleep(20);

	waveOutReset(hWaveOut_);
	waveOutClose(hWaveOut_);

	return 0;
}

int WaveMem::Set(short* pSamples)
{
	if (!memReady_) return -1;
	if (bytesPerSample_ == 2)
		ippsCopy_16s(pSamples, (Ipp16s*)pBuffer_, len_ * nChannels_);
	else if (bytesPerSample_ == 1)
	{
		IppiSize size = {len_ * nChannels_, 1};
		ippiConvert_16s8u_C1R(pSamples, size.width*2, pBuffer_, size.width, size);		
	}
	return 0;
}

int WaveMem::Set(char* pSamples)
{
	return -1;
}
