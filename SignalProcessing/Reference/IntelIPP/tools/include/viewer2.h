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

#ifndef __VIEWER_H__
#define __VIEWER_H__

#include "3ds.h"

#if defined _WINDOWS
#include <windows.h>
#else 
typedef struct tagRECT
{
	long left;
    long top;
    long right;
    long bottom;
} RECT;
#endif

class Render3DS
{
	public:

		typedef enum {PLANE, SIDE1, SIDE2, SIDE3} NormalType;

	private:
	typedef struct{
		//color and style of object's visualization,
		//(are defined automatically according to the problem)
//		GLenum			GeomType;
//		GLenum			FillStyle;
		int			GeomType;
		int			FillStyle;
		float			MatAmbient[4];
	} Style;

//	GLfloat sizeXY, sizeZ;
	float sizeXY, sizeZ;
	int x_prev,y_prev;
	float m_r0_, m_r1_, m_g0_, m_g1_, m_b0_, m_b1_;
	static float pIdentity4f[16];

	Style style1, style2;

	float v_pointZ;
	float v_planeZ;
	int fill_;
	typedef enum { VERTEX_NORMALS, TRIANGLE_NORMALS };
	int m_normals_;

	void Normalize(Object3DS& object);
	void InitLight (void);
	void InitStyle();
	void DrawObject(Object3DS& object, Style *pStyle,
				   char* pMask = 0);
	void DrawRays(float* pPoints, int pointStride,
		float* pNormals, int normalStride, int len,
		Style *pStyle, float r, float g, float b);
	void DrawNormals(Object3DS& object, Style *pStyle,
		float r, float g, float b);
	void DrawTriangleNormals(Object3DS& object,
		NormalType type,
		Style *pStyle, float r, float g, float b);
	
	void MakePlane();

	public:

	void SetFill(int fill = 1);
	void SetLight1Color(float r, float g, float b);
	void SetLight2Color(float r, float g, float b);
	float ModelViewMatrix[16];

	Render3DS(float* pViewMatrix4f = pIdentity4f);

	void GetBounds(const RECT *pWindowRect,
				   float *left, float* right,
				   float* top, float* bottom);
	float GetZMin() { return -sizeZ;}
	float GetZMax() { return sizeZ;}

	void Reset();
	void Set(float* pViewMatrix4f = pIdentity4f);
	void UseTriangleNormals();
	void UseVertexNormals();

	void Reshape(RECT *pRect);
	void Rotate(int x, int y, int x_prev, int y_prev, RECT *pRect);
	void Translate(int x, int y, int x_prev, int y_prev, RECT *pRect);
	void Scale(int delta, RECT *pRect);
	void DisplayInit();
	void Display(Object3DS& object, char* pMask=0);
	void DisplayNormals(Object3DS& object,
		float r=1.0, float g=1.0, float b=1.0);
	void DisplayTriangleNormals(Object3DS& object,
		NormalType type,
		float r=1.0, float g=1.0, float b=1.0);
	void DisplayDone();

	void DrawTriangle(const float* pTriangle);
	void DrawLine(const float* pLine);
	
};

class Mask3DS
{
	private:
		int m_len_;
		char* m_pMask_;

	public:
		~Mask3DS();
		Mask3DS();
		void CopySettings(Object3DS&);

	
};

#endif //__VIEWER_H__


