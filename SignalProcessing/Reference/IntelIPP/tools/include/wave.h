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

//#include <stdio.h>

class WaveFile
{
private:
	int SampleRate;
	int BitsPerSample;
	int BytesPerSample;
	int ShiftBits;
	int NumChannels;
	int NumSamples;
	int DataLength;
	int BytesPerElem;

	int isValid_;
	char* pHead_;
	char* pFilename_;
	int hSize_;

public:
	WaveFile() : isValid_(0), pHead_(0), pFilename_(0) { }
	~WaveFile();
	// Opens and reads header
	// returns 0 if OK
	int Open(const char* pFilename);

	int GetSampleBits() const { return BitsPerSample; }
	int GetSampleBytes() const { return BytesPerSample; }
	int GetNumChannels() const { return NumChannels; }
	int GetNumSamples() const { return NumSamples; }
	int GetElemBytes() const { return BytesPerElem; }
	int GetSampleRate() const { return SampleRate; }

	int WriteAs(const short* pSamples,
				const char* pFilename);

	int ReadAsInt(int *pSamples, int numElem) const;
	int ReadAsShort(short* pSamples, int numElem) const;

	int Play();
	static int Play(const char* filename);
};

class WaveMem
{
	private:
		int bitsPerSample_;
		int bytesPerSample_;
		int samplesPerSec_;
		int nChannels_;

		int len_;

		unsigned char* pBuffer_;
		int bufSize_;

		int playReady_;
		int memReady_;

	public:
		WaveMem() : playReady_(0), memReady_(0),
			pBuffer_(0) {}
		~WaveMem() { Free(); }

		int Init(int bitsPerSample, int samplesPerSec,
				 int nChannels);
		int Alloc(int len);
		int Free();
		int InitAlloc(int len, int bitsPerSample,
					  int samplesPerSec, int nChannels)
		{
			if (!Init(bitsPerSample, samplesPerSec,
					  nChannels))
				return Alloc(len);
			else return -1;
		}

		int CopySettings(const WaveMem* pWaveMem);

		// length in elements
		int Set(short* pSamples);
		int Set(char* pSamples);

		void SetBitsPerSample(int bitsPerSample)
		{
			bitsPerSample_ = bitsPerSample;
			bytesPerSample_ = (bitsPerSample+7)/8;
			playReady_ = 0; memReady_ = 0;
		}
		void SetSamplesPerSec(int samplesPerSec)
		{
			samplesPerSec_ = samplesPerSec;
			playReady_ = 0; memReady_ = 0;
		}
		void SetNumChannels(int nChannels)
		{
			nChannels_ = nChannels;
			playReady_ = 0; memReady_ = 0;
		}

		int GetSamplesPerSec() const
			{ return samplesPerSec_; }
		int GetNumChannels() const
			{ return nChannels_; }
		int GetBitsPerSample() const
			{ return bitsPerSample_; }

		int GetLen() const { return len_;}
		const short* GetShortBuffer() const;
		short* GetShortBuffer();
		int ReadFromFile(const WaveFile* pWaveFile);
		int WriteToFile(WaveFile* pWaveFile);

		int Play(int wait = 1);
};
