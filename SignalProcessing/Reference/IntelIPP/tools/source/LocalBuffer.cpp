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

#include "LocalBuffer.h"

#include <ipp.h>

LocalBuffer::LocalBuffer() : pBuffer_(0), bufSize_(0),
	index_(0)
{}

LocalBuffer::~LocalBuffer()
{
	if (pBuffer_) ippsFree(pBuffer_);
}

void LocalBuffer::SetMinAlloc(int size)
{
	if (bufSize_ < size)
	{
		if (pBuffer_) ippsFree(pBuffer_);
		pBuffer_ = (char*)ippsMalloc_8u(size);
		if (pBuffer_) bufSize_ = size;
		else bufSize_ = 0;
	}
}

float* LocalBuffer::Alloc_32f(int len)
{
	if (index_ + len*(int)sizeof(float) > bufSize_) return 0;
	float *pMem = (float*)&pBuffer_[index_];
	index_ += len*sizeof(float);
	return pMem;
}

int* LocalBuffer::Alloc_32s(int size)
{
	if (index_ + size*(int)sizeof(int) > bufSize_) return 0;
	int *pMem = (int*)&pBuffer_[index_];
	index_ += size*sizeof(int);
	return pMem;
}

unsigned char* LocalBuffer::Alloc_8u(int size)
{
	if (index_ + size*(int)sizeof(unsigned char) > bufSize_)
		return 0;
	unsigned char *pMem =
		(unsigned char*)&pBuffer_[index_];
	index_ += size*sizeof(unsigned char);
	return pMem;
}

void LocalBuffer::ReleaseAll()
{
	index_ = 0;
}
