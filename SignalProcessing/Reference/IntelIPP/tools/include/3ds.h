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

#ifndef __3DS_H__
#define __3DS_H__

#include <stdio.h>

#include "LocalBuffer.h"

typedef struct {
	float				x;
	float				y;
	float				z;
	float				nx;
	float				ny;
	float				nz;
	float				r;
	float				g;
	float				b;
	float				tu;
	float				tv;
	long				flag;
} VertexData;

typedef struct {
	unsigned long		firstIndex;
	unsigned long		secondIndex;
	unsigned long		thirdIndex;
} TrianglesData;

typedef struct {
	float A, B, C, D;
} PlaneData;

typedef struct {
	float				r;
	float				g;
	float				b;

} ColorData;

typedef struct {
	unsigned long		x0;
	unsigned long		y0;
	unsigned long		width;
	unsigned long		height;
} TextureData;

typedef struct {
	char				nameMaterial[20];
	ColorData			ambient;
	ColorData			diffuse;
	ColorData			specular;
	ColorData			shininess;
	int					useTexture;
	TextureData			textureData;
} MaterialData;

class Object3DS
{
	private: 
		static unsigned char ReadByte(FILE *file);
		static int ReadInt(FILE *file);
		static long ReadLong(FILE *file);
		static unsigned int ReadSectionID(FILE *file);
		static unsigned long ReadSizeLevel(FILE *file);
		static unsigned long ReadUnknownLevel(FILE *file);
		static unsigned long ReadLocalMatrix(FILE* pFile);
		int AddTexture(char* fileName);
		unsigned long ReadTextures(FILE* pFile);
		unsigned long ReadVertexMaterial(FILE* pFile, int shiftTriangles);
		unsigned long ReadTriangles(FILE* pFile);
		unsigned long ReadVertex(FILE* pFile);
		unsigned long ReadMeshLevel(FILE* pFile);
		unsigned long ReadObjectLevel(FILE* pFile);
		unsigned long ReadTextFileName(FILE* pFile);
		unsigned long ReadTextureMap(FILE* pFile);
		unsigned long ReadMaterialColor(FILE* pFile, ColorData *colorData);
		unsigned long ReadMaterialName(FILE* pFile);
		unsigned long ReadMaterial(FILE* pFile);
		unsigned long ReadDataLevel(FILE* pFile);
		int ReadVersion(FILE* pFile);

//		void BufAlloc_(int size);

		LocalBuffer m_buffer_;
//		char* m_pBuffer_;
//		int m_bufSize_;

	public:
		Object3DS();
		~Object3DS();
		int Load(FILE* pFile);
		void Normalize();
		void CalcCenterOfMass(float* pPoint,
			float* pMass, int algorithm= -1);

		void CalcPlanes(int algorithm = -1);
		void IntersectRay(float* pStart, float* pDir,
			unsigned char* pMask, int algorithm = -1,
			int lineSegment = 0);
		void IntersectLineSeg(float* p1,
			float* p2, unsigned char* pMask);
		void IntersectObject(Object3DS& obj,
			unsigned char* pMask,
			unsigned char* pMaskOther);

		int		m_numVertex;
		int		m_shiftVertex;
		int		m_numTriangles;
		int		m_numMaterial;
		int		m_numTextureFiles;
//		unsigned long		m_numVertex;
//		unsigned long		m_shiftVertex;
//		unsigned long		m_numTriangles;
//		unsigned long		m_numMaterial;
//		unsigned long		m_numTextureFiles;
		TextureData			m_globalTexture;
		char*				m_globalBitmap;
		unsigned long		m_sizeGlobalBitmap;
		VertexData*			m_vertexData;
		TrianglesData*		m_trianglesData;
		MaterialData*		m_materialData;

		PlaneData*			m_planeData;
		PlaneData*			m_side1Plane;
		PlaneData*			m_side2Plane;
		PlaneData*			m_side3Plane;

		void*				m_pTmp;
};

#endif /* __3DS_H__ */
