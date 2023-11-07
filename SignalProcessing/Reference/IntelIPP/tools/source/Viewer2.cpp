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

#include "Viewer2.h"

//#include <windows.h>
#include <GL/gl.h>

#include <math.h>
#include <string.h>

float Render3DS::pIdentity4f[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

Render3DS::Render3DS(float* pViewMatrix4f)
{
	sizeXY = 2.0f;
	sizeZ = 20.0f;

	int i;
	if (pViewMatrix4f)
		for (i=0; i<16; i++)
			ModelViewMatrix[i] = pViewMatrix4f[i];
	else
		for (i=0; i<16; i++)
			ModelViewMatrix[i] = pIdentity4f[i];

	v_pointZ = 2.0f;
	v_planeZ = 1.5f;
	fill_ = 1;

	m_r0_ = 1.0f; m_g0_ = 0.5f; m_b0_ = 0.5f;
	m_r1_ = 0.5f; m_g1_ = 1.0f; m_b1_ = 0.5f;

	m_normals_ = VERTEX_NORMALS;
}

void Render3DS::Reset()
{
	Set(pIdentity4f);
}

void Render3DS::Set(float* pViewMatrix4f)
{
	int i;
	if (pViewMatrix4f)
		for (i=0; i<16; i++)
			ModelViewMatrix[i] = pViewMatrix4f[i];
	else
		for (i=0; i<16; i++)
			ModelViewMatrix[i] = pIdentity4f[i];
}

//-------------------------------------------------------------------------------------------------
void Render3DS::InitLight (void)
{
    GLfloat light_a[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//    GLfloat light_d_0[] = { 1.0f, 0.5f, 0.5f ,1.0f };
    GLfloat light_d_0[] = { m_r0_, m_g0_, m_b0_, 1.0f };
	GLfloat light_p_0[] = {50.0f, -50.0f, 50.0f, 0.0f };
//    GLfloat light_d_1[] = { 0.5f, 1.0f, 0.5f ,1.0f };
    GLfloat light_d_1[] = { m_r1_, m_g1_, m_b1_, 1.0f };
  	GLfloat light_p_1[] = {-50.0f, 50.0f, 50.0f, 0.0f };

	GLfloat mat_d[] = { 1.0f, 1.0f, 1.0f,1.0f };
  
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE , 0.0f);    
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_a);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_d_0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_p_0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_a);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_d_1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_p_1);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);

	glClearColor(0.0f,0.0f,0.0f,1.0f); 

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

//-------------------------------------------------------------------------------------------------
void Render3DS::InitStyle()
{
	unsigned long i;

	{
		GLfloat ma1[] = {0.0f, 0.0f, 2.5f, 1.0f};
		GLfloat ma2[] = {10.0f, 10.0f, 0.0f, 1.0f};
		for (i=0; i<4; i++) 
			style1.MatAmbient[i] = ma1[i]; 
		style1.GeomType = GL_TRIANGLES;
		if (fill_) style1.FillStyle  = GL_FILL;
		else style1.FillStyle  = GL_LINE;

		for (i=0; i<4; i++) 
			style2.MatAmbient[i] = ma2[i]; 
		style2.GeomType = GL_POINTS;
		style2.FillStyle  = GL_POINT;
	}
}

void Render3DS::SetFill(int fill)
{
	fill_ = fill;
}

void Render3DS::SetLight1Color(float r, float g, float b)
{
	m_r0_ = r;
	m_g0_ = g;
	m_b0_ = b;
}

void Render3DS::SetLight2Color(float r, float g, float b)
{
	m_r1_ = r;
	m_g1_ = g;
	m_b1_ = b;
}

void Render3DS::UseTriangleNormals()
{
	m_normals_ = TRIANGLE_NORMALS;
}

void Render3DS::UseVertexNormals()
{
	m_normals_ = VERTEX_NORMALS;
}
//-------------------------------------------------------------------------------------------------
void Render3DS::DrawNormals(Object3DS& object, Style *pStyle, float r, float g, float b)
{
	int i;
//	unsigned long i;
	glEnable(GL_NORMALIZE);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
//	glPolygonMode (GL_FRONT_AND_BACK, pStyle->FillStyle);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pStyle->MatAmbient);

	glBegin(GL_LINES);
	glColor3f(r, g, b);
	for (i=0; i< object.m_numVertex; i ++)
	{
		glVertex3fv(&(object.m_vertexData[i].x));
		glVertex3f(object.m_vertexData[i].x + object.m_vertexData[i].nx,
					object.m_vertexData[i].y + object.m_vertexData[i].ny,
					object.m_vertexData[i].z + object.m_vertexData[i].nz);
	}
	glEnd();

	glDisable(GL_NORMALIZE);
}

void Render3DS::DrawTriangleNormals(Object3DS& object,
	NormalType type,
	Style *pStyle, float r, float g, float b)
{
//	unsigned long i;
	int i;
	glEnable(GL_NORMALIZE);
//	glPolygonMode (GL_FRONT_AND_BACK, pStyle->FillStyle);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pStyle->MatAmbient);

	glBegin(GL_LINES);
	glColor3f(r, g, b);

	VertexData *pPt1, *pPt2, *pPt3, *pVer = object.m_vertexData, pt;
	PlaneData* pPlane;
	unsigned long *pIndex, *pIndex2;
	if (type == SIDE1)
	{
		pPlane = object.m_side1Plane;
		pIndex = &object.m_trianglesData->thirdIndex;
		pIndex2 = &object.m_trianglesData->secondIndex;
	}
	else if (type == SIDE2)
	{
		pPlane = object.m_side2Plane;
		pIndex = &object.m_trianglesData->firstIndex;
		pIndex2 = &object.m_trianglesData->thirdIndex;
	}
	else if (type == SIDE3)
	{
		pPlane = object.m_side3Plane;
		pIndex = &object.m_trianglesData->firstIndex;
		pIndex2 = &object.m_trianglesData->secondIndex;
	}
	else
	{
		pPlane = object.m_planeData;
		pIndex = &object.m_trianglesData->firstIndex;
	}

	for (i=0; i< object.m_numTriangles; i ++)
	{
//		pPt = &pVer[object.m_trianglesData[i].firstIndex];
		if (type==PLANE)
		{
			pPt1 = &pVer[pIndex[i*3]];
			pPt2 = &pVer[pIndex[i*3+1]];
			pPt3 = &pVer[pIndex[i*3+2]];
			pt.x = (pPt1->x + pPt2->x + pPt3->x)/3;
			pt.y = (pPt1->y + pPt2->y + pPt3->y)/3;
			pt.z = (pPt1->z + pPt2->z + pPt3->z)/3;
			glNormal3fv((float*)&object.m_planeData[i].A);
		}
		else
		{
			pPt1 = &pVer[pIndex[i*3]];
			pPt2 = &pVer[pIndex2[i*3]];
			pt.x = (pPt1->x + pPt2->x)/2.0f;
			pt.y = (pPt1->y + pPt2->y)/2.0f;
			pt.z = (pPt1->z + pPt2->z)/2.0f;
			glNormal3fv((float*)&object.m_planeData[i].A);
		}

		glVertex3fv((float*)&pt);
		glVertex3f(pt.x + pPlane[i].A/2,
			pt.y + pPlane[i].B/2,
			pt.z + pPlane[i].C/2);
	}

//		glVertex3f(pPt->x + pPlane[i].A*20,
//			pPt->y + pPlane[i].B*20,
//			pPt->z + pPlane[i].C*20);
	glEnd();

	glDisable(GL_NORMALIZE);
}

void Render3DS::DrawRays(float* pPoints, int pointStride,
	float* pNormals, int normalStride, int len,
	Style *pStyle, float r, float g, float b)
{
//	unsigned long i;
	int i;
	glEnable(GL_NORMALIZE);
	glPolygonMode (GL_FRONT_AND_BACK, pStyle->FillStyle);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pStyle->MatAmbient);

	glBegin(GL_LINES);
	glColor3f(r, g, b);
	for (i=0; i< len; i ++)
	{
		glVertex3fv(&pPoints[i*pointStride]);
		glVertex3f(pPoints[i*pointStride] + pNormals[i*normalStride],
			pPoints[i*pointStride+1] + pNormals[i*normalStride+1],
			pPoints[i*pointStride+2] + pNormals[i*normalStride+2]);
	}
	glEnd();

	glDisable(GL_NORMALIZE);
}

void Render3DS::DrawObject(Object3DS& object,
	Style *pStyle, char* pMask)
{
//	unsigned long i;
	int i;
	glEnable(GL_NORMALIZE);
//	glPolygonMode (GL_BACK, GL_LINE);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, pStyle->MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pStyle->MatAmbient);
//	glPolygonMode (GL_FRONT_AND_BACK, pStyle->FillStyle);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pStyle->MatAmbient);

	TrianglesData* pTri;
	if (!pMask)
	{
		glPolygonMode (GL_FRONT_AND_BACK, pStyle->FillStyle);
//		glPolygonMode (GL_BACK, GL_POINT);
		glBegin(GL_TRIANGLES);
		for (i=0; i< object.m_numTriangles; i ++)
		{
			if (m_normals_ == TRIANGLE_NORMALS)
			{
				pTri = &object.m_trianglesData[i];
				glNormal3fv(&object.m_planeData[i].A);
				glVertex3fv(&(object.m_vertexData[pTri->firstIndex].x));
				glVertex3fv(&(object.m_vertexData[pTri->secondIndex].x));
				glVertex3fv(&(object.m_vertexData[pTri->thirdIndex].x));
			}
			else
			{
				pTri = &object.m_trianglesData[i];
				glNormal3fv(&(object.m_vertexData[pTri->firstIndex].nx));
				glVertex3fv(&(object.m_vertexData[pTri->firstIndex].x));
			
				glNormal3fv(&(object.m_vertexData[pTri->secondIndex].nx));
				glVertex3fv(&(object.m_vertexData[pTri->secondIndex].x));
			
				glNormal3fv(&(object.m_vertexData[pTri->thirdIndex].nx));
				glVertex3fv(&(object.m_vertexData[pTri->thirdIndex].x));
			}
		}
		glEnd();
	}
	else
		for (i=0; i< object.m_numTriangles; i ++)
		{
			pTri = &object.m_trianglesData[i];
			if (pMask[i])
//				glPolygonMode (GL_FRONT, pStyle->FillStyle);
				glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
			else glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

			glBegin(GL_TRIANGLES);
			glNormal3fv(&(object.m_vertexData[pTri->firstIndex].nx));
			glVertex3fv(&(object.m_vertexData[pTri->firstIndex].x));

			glNormal3fv(&(object.m_vertexData[pTri->secondIndex].nx));
			glVertex3fv(&(object.m_vertexData[pTri->secondIndex].x));

			glNormal3fv(&(object.m_vertexData[pTri->thirdIndex].nx));
			glVertex3fv(&(object.m_vertexData[pTri->thirdIndex].x));
			glEnd();
		}

	glDisable(GL_NORMALIZE);
}

void Render3DS::DrawLine(const float* pLine)
{
//	unsigned long i;
	glEnable(GL_NORMALIZE);
	glPolygonMode (GL_FRONT_AND_BACK, 0);
//	glPolygonMode (GL_FRONT_AND_BACK, pStyle->FillStyle);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
//	pStyle->MatAmbient);

	glBegin(GL_LINES);
	glVertex4fv(pLine);
	glVertex4fv(pLine+4);
	glEnd();

	glDisable(GL_NORMALIZE);
}

void Render3DS::DrawTriangle(const float* pTriangle)
{
//	unsigned long i;
	glEnable(GL_NORMALIZE);
//	glPolygonMode (GL_FRONT_AND_BACK, pStyle->FillStyle);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, pStyle->MatAmbient);

	glBegin(GL_TRIANGLES);
//	glVertex3fv(pTriangle);
//	glVertex3fv(pTriangle+4);
//	glVertex3fv(pTriangle+8);
	glVertex4fv(pTriangle);
	glVertex4fv(pTriangle+4);
	glVertex4fv(pTriangle+8);
	glEnd();

	glDisable(GL_NORMALIZE);
}

//-------------------------------------------------------------------------------------------------
void Render3DS::MakePlane()
{
	GLfloat ma_plane[] = {10.0f, 10.0f, 10.0f, 1.0f};
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ma_plane);
	glBegin(GL_QUADS);
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f(-1.5f, -1.5f, v_planeZ);//plane's vertices 
	glVertex3f(1.5f, -1.5f, v_planeZ);//plane's vertices 
	glVertex3f(1.5f, 1.5f, v_planeZ);//plane's vertices 
	glVertex3f(-1.5f, 1.5f, v_planeZ);//plane's vertices 
	glEnd();
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0);
	glBegin(GL_POINTS); //point of view
	glVertex3f(0.0f, 0.0f, v_pointZ);//plane's vertices 
	glEnd();
	glDisable(GL_POINT_SMOOTH);
}

//---------------------------------------------------------
void Render3DS::GetBounds(const RECT *pWindowRect,
	float *left, float* right, float* top, float* bottom)
{
	int w = pWindowRect->right-pWindowRect->left;
	int h = pWindowRect->bottom-pWindowRect->top;
	if (w <= h)
	{
		*left = -sizeXY;
		*right = sizeXY;
		*bottom = -sizeXY*(GLfloat)h/(GLfloat)w;
		*top = sizeXY*(GLfloat)h/(GLfloat)w;
	}
    else
	{
		*left = -sizeXY*(GLfloat)w/(GLfloat)h;
		*right = sizeXY*(GLfloat)w/(GLfloat)h;
		*bottom = -sizeXY;
		*top = sizeXY;
	}
}

void Render3DS::Reshape(RECT *pRect)
{
	int w = pRect->right-pRect->left;
	int h = pRect->bottom-pRect->top;
    glViewport(pRect->left, pRect->top, pRect->right, pRect->bottom);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	float left, right, top, bottom;
	GetBounds(pRect, &left, &right, &top, &bottom);
	glOrtho(left, right, bottom, top,
			-sizeZ, sizeZ);
//	if (w <= h)
//    glOrtho (-sizeXY, sizeXY,
//		-sizeXY*(GLfloat)h/(GLfloat)w, 
//        sizeXY*(GLfloat)h/(GLfloat)w, -sizeZ, sizeZ);
//    else 
//    glOrtho (-sizeXY*(GLfloat)w/(GLfloat)h, 
//        sizeXY*(GLfloat)w/(GLfloat)h,
//		-sizeXY, sizeXY, -sizeZ, sizeZ);
    glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ModelViewMatrix);
}

//-------------------------------------------------------------------------------------------------
void Render3DS::Rotate(int x, int y, int x_prev, int y_prev, RECT *pRect)
{
	int w = pRect->right-pRect->left;
	int h = pRect->bottom-pRect->top;
	float	sumS, sumE, len, norm, angle;
	float	start[3], end[3], out[3];
	unsigned short i=0;
	
	norm = (float)__min(h,w);
	start[0] = (2*x_prev-w)/(norm);
	start[1] = (h-2*y_prev)/(norm);
	start[2] = 0.0f;
	sumS =  (float)sqrt(start[0]*start[0]+start[1]*start[1]+start[2]*start[2]);
	if (sumS < 0.8f){
		start[2] =(float) sqrt(0.8f - sumS);	
		sumS = (float)sqrt(start[0]*start[0]+start[1]*start[1]+start[2]*start[2]);
	}
	end[0] = (2*x-w)/(norm);
	end[1] = (h-2*y)/(norm);
	end[2] = 0.0f;
	sumE = (float)sqrt(end[0]*end[0]+end[1]*end[1]+end[2]*end[2]);
	if (sumE < 0.8f){
		end[2] = (float)sqrt(0.8f - sumE);
		sumE = (float)sqrt(end[0]*end[0]+end[1]*end[1]+end[2]*end[2]);
	}
	angle = (start[0]*end[0]+start[1]*end[1]+start[2]*end[2])/(sumS*sumE);
	if (angle<=-1.0f)
		angle = -0.99999f;
	if (angle>=1.0f)
		angle = 0.99999f;
	angle = (float)acos(angle)*180.0f/3.141592653589f;
		
	out[0] =start[1]*end[2]-start[2]*end[1];
	out[1] =-start[0]*end[2]+start[2]*end[0];
	out[2] =start[0]*end[1]-start[1]*end[0];
	len = (float)sqrt(out[0]*out[0]+out[1]*out[1]+out[2]*out[2]);
	if (len != 0.0) 
		for (i=0; i<3; i++)
			out[i] /= len;				
			
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glRotatef(angle, out[0], out[1], out[2]);
	glMultMatrixf(ModelViewMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewMatrix);
}

//-------------------------------------------------------------------------------------------------
void Render3DS::Translate(int x, int y, int x_prev, int y_prev, RECT *pRect)
{
	int w = pRect->right-pRect->left;
	int h = pRect->bottom-pRect->top;
	float	norm;
	float	out[2];
	
	norm = (float)__min(h,w);
	out[0] = (2*x - 2*x_prev)/(norm);
	out[1] = (-2*y + 2*y_prev)/(norm);

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glTranslatef(out[0], out[1], 0.0f);
	glMultMatrixf(ModelViewMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewMatrix);
}

//-------------------------------------------------------------------------------------------------
void Render3DS::Scale(int delta, RECT *pRect)
{
	int w = pRect->right-pRect->left;
	int h = pRect->bottom-pRect->top;
	float out = (float) 10.0f*(delta)/h;
	if (1+out> 0.0f) {
		sizeXY += sizeXY*out; 
		sizeZ += sizeZ*out;
	}
	Reshape(pRect);
}

//-------------------------------------------------------------------------------------------------
void Render3DS::DisplayInit()
{
	glPushMatrix ();
	glLoadIdentity();
	InitLight();   
	InitStyle();   
	glPopMatrix ();
}

void Render3DS::DisplayNormals(Object3DS& object, float r, float g, float b)
{
	glPushMatrix ();
	DrawNormals(object, &style1, r, g, b);
	glPopMatrix ();
}

void Render3DS::DisplayTriangleNormals(Object3DS& object,
	NormalType type, float r, float g, float b)
{
	glPushMatrix ();
	DrawTriangleNormals(object, type, &style1, r, g, b);
	glPopMatrix ();
}
void Render3DS::Display(Object3DS& object, char* pMask)
{
	glPushMatrix ();
	DrawObject(object, &style1, pMask);
	glPopMatrix ();
}

void Render3DS::DisplayDone()
{
	glFlush();
}

// Calculating ray-triangle intersection
// pre-calculate: triangle plane parameters
// pre-calculate: inside/outside tests (weights) for all three sides

Mask3DS::Mask3DS() : m_len_(0), m_pMask_(0) {}
Mask3DS::~Mask3DS() { if (m_pMask_) delete [] m_pMask_; }

void Mask3DS::CopySettings(Object3DS& object)
{
	m_len_ = object.m_numTriangles;
	m_pMask_ = new char[m_len_];
}

//end of "Viewer.c"

