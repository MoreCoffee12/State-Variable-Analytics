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

#include "3ds.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <windows.h>

#include <ipp.h>

/* ID COMMON CHUNKS */
#define COLOR_F				0x0010 /* Float color */
#define COLOR_24			0x0011 /* RGB byte color */
#define LIN_COLOR_24		0x0012 /* RGB byte color */
#define LIN_COLOR_F			0x0013 /* Float  color */
#define INT_PERCANTAGE		0x0030 /* Float  color */
#define FLOAT_PERCANTAGE	0x0031 /* Float  color */

/* ID MAIN CHUNKS */
#define MAIN3DS				0x4D4D

/* CHANG ID FOR MAIN LEVEL */
#define M3D_VERSION			0x0002	/* version */
#define MDATA				0x3D3D	/* Mesh section */
#define KFDATA				0xB000	/* Key Frame Defenition*/

/* CHANG ID FOR MDATA LEVEL */
#define NAMED_OBJECT		0x4000	/* Object defenition */
#define MAT_ENTRY			0xAFFF	/* Material defenition */

/* CHANG ID FOR MAT_ENTRY LEVEL */
#define MAT_NAME			0xA000	/* Name of material defenition */
#define MAT_AMBIENT			0xA010	/* Ambient color of material */
#define MAT_DIFFUSE			0xA020	/* Diffuse color of material */
#define MAT_SPECULAR		0xA030	/* Specular color of material */
#define MAT_SHININESS		0xA040	/* Shininess color of material */
#define MAT_TWO_SIDE		0xA081	/* Material map two sided */
#define MAT_FACEMAP			0xA088	/* Material mapped textures */
#define MAT_TEXMAP			0xA200	/* Material texture map defenition */

/* CHANG ID FOR MAT_TEXMAP LEVEL */
#define MAT_MAPNAME			0xA300	/* Texture file name */

/* CHANG ID FOR NAMED_OBJECT LEVEL */
#define N_TRI_OBJECT		0x4100	/* Mesh object defenition */
#define N_DIRECT_LIGHT		0x4600	/* Mesh object defenition */


/* CHANG ID FOR N_TRI_OBJECT LEVEL */
#define POINT_ARRAY			0x4110	/* A list of points */
#define FACE_ARRAY			0x4120	/* A list of faces */
#define MSH_MAT_GROUP		0x4130	/* A list of material */
#define TEX_VERTS			0x4140	/* A list of texture */
#define MESH_MATRIX			0x4160	/* transform Matrix	*/

Object3DS::Object3DS()
{
	m_numVertex = 0;
	m_shiftVertex = 0;
	m_numTriangles = 0;
	m_numMaterial = 0;
	m_numTextureFiles = 0;
	m_globalBitmap = 0;
	m_sizeGlobalBitmap = 0;
	m_numMaterial = 0;
	m_vertexData = 0;
	m_trianglesData = 0;
	m_materialData = 0;
	m_planeData = 0;

	m_side1Plane = 0;
	m_side2Plane = 0;
	m_side3Plane = 0;
//	m_pBuffer_ = 0;
//	m_bufSize_ = 0;
}

Object3DS::~Object3DS()
{
	if (m_vertexData) free(m_vertexData);
	if (m_trianglesData) free(m_trianglesData);
	if (m_materialData) free(m_materialData);
	if (m_planeData) free(m_planeData);
	if (m_side1Plane) free(m_side1Plane);
	if (m_side2Plane) free(m_side2Plane);
	if (m_side3Plane) free(m_side3Plane);
//	if (m_pBuffer_) free(m_pBuffer_);
}

unsigned char Object3DS::ReadByte(FILE *pFile)
{
	char	dataByte;

	dataByte = fgetc(pFile);

	return dataByte;
} /* ReadByte */

int Object3DS::ReadInt(FILE *pFile)
{
	unsigned char	dataByteH;
	unsigned char	dataByteL;
	int				dataInt;

	dataByteH	= ReadByte(pFile);
	dataByteL	= ReadByte(pFile);
	dataInt		= (((int)dataByteH)	| (((int)dataByteL) << 8));

	return	dataInt;
} /* ReadInt */

long Object3DS::ReadLong(FILE *pFile)
{
	int		dataIntH;
	int		dataIntL;
	long	dataLong;

	dataIntH	= ReadInt(pFile);
	dataIntL	= ReadInt(pFile);
	dataLong	= dataIntH	| (dataIntL << 16);

	return	dataLong;
} /* ReadInt */

unsigned int Object3DS::ReadSectionID(FILE *pFile)
{
	unsigned int sectioID;

	sectioID	= ReadInt(pFile);

	return sectioID;
} /* ReadSectionID */

unsigned long Object3DS::ReadSizeLevel(FILE *pFile)
{
	unsigned long sizeLevel;

	sizeLevel	= ReadLong(pFile);

	return sizeLevel;
} /* ReadSizeLevel*/

unsigned long Object3DS::ReadUnknownLevel(FILE *pFile)
{
	unsigned long sizeLevel;
	
	sizeLevel = ReadSizeLevel(pFile) - 6;
	fseek(pFile, sizeLevel, SEEK_CUR);


	return sizeLevel;
} /* ReadUnknownLevel */

unsigned long Object3DS::ReadLocalMatrix(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
//	FILE			*pFile;

//	pFile			= m_pFile;
	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	currentPosition	= ftell(pFile);
	if(currentPosition < endPosition) {
		fseek(pFile, endPosition - currentPosition, SEEK_CUR);
	} /* if */

	return sizeLevel;

} /* ReadLocalMatrix */

unsigned long Object3DS::ReadTextures(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned long	temp;
	unsigned long	numVertex;
	unsigned int	numText;
	unsigned int	index;
//	FILE			*pFile;
	
//	pFile			= m_pFile;
	numVertex		= m_numVertex;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	index			= 0;
	numText			= ReadInt(pFile);


	if(numText && numVertex >= numText) {

		index	= numVertex - numText;
	
		while(numText > 0 && currentPosition < endPosition) {
			temp	= ReadLong(pFile);
			m_vertexData[index].tu	= *((float*)&temp);
			temp	= ReadLong(pFile);
			m_vertexData[index].tv	= *((float*)&temp);

			index++; numText--;

			currentPosition	= ftell(pFile);
		} /* while */

	} /* if */

	currentPosition	= ftell(pFile);
	if(currentPosition < endPosition) {
		fseek(pFile, endPosition - currentPosition, SEEK_CUR);
	} /* if */

	return sizeLevel;
} /* ReadTextures */

unsigned long Object3DS::ReadVertexMaterial(FILE* pFile, int shiftTriangles)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned long	a;
	unsigned long	b;
	unsigned long	c;
	unsigned int	numVertex;
	unsigned int	numMat;
	MaterialData	*materialData;
	unsigned int	mat;
	char			strName[256];
	int				index;
//	FILE			*pFile;
	
//	pFile			= m_pFile;
	numMat			= m_numMaterial;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	index			= 0;
	strName[index]	= ReadByte(pFile);
	
	/* Load Object Name */
	while(strName[index] != 0 && index < 16) {
		index++;
		strName[index]	= ReadByte(pFile);
	} /* while */

	/* search material*/
	for(;numMat != 0; numMat--) {
		int rez;
		
		rez	= strcmp(strName, m_materialData[numMat - 1].nameMaterial);
		if(rez == 0) {
			materialData = &m_materialData[numMat - 1];
			break;
		} /* if */
	}/* for */

	if(numMat) {
		numVertex	= ReadInt(pFile);
		while(numVertex > 0 && currentPosition < endPosition) {
			
			mat	= ReadInt(pFile);
			mat	+= shiftTriangles;

			a	= m_trianglesData[mat].firstIndex;
			b	= m_trianglesData[mat].secondIndex;
			c	= m_trianglesData[mat].thirdIndex;
			
			m_vertexData[a].r		= materialData->diffuse.r;
			m_vertexData[a].g		= materialData->diffuse.g;
			m_vertexData[a].b		= materialData->diffuse.b;
			m_vertexData[a].flag	= numMat - 1;
			m_vertexData[b].r		= materialData->diffuse.r;
			m_vertexData[b].g		= materialData->diffuse.g;
			m_vertexData[b].b		= materialData->diffuse.b;
			m_vertexData[b].flag	= numMat - 1;
			m_vertexData[c].r		= materialData->diffuse.r;
			m_vertexData[c].g		= materialData->diffuse.g;
			m_vertexData[c].b		= materialData->diffuse.b;
			m_vertexData[c].flag	= numMat - 1;
			
			if(materialData->useTexture == 0) {
				m_vertexData[a].tu	= 0;
				m_vertexData[a].tv	= 0;
				m_vertexData[b].tu	= 0;
				m_vertexData[b].tv	= 0;
				m_vertexData[c].tu	= 0;
				m_vertexData[c].tv	= 0;
			} /* if */
			
			currentPosition	= ftell(pFile);
			numVertex--;
		} /* while */
	} /* if */

	currentPosition	= ftell(pFile);
	if(currentPosition < endPosition) {
		fseek(pFile, endPosition - currentPosition, SEEK_CUR);
	} /* if */

	return sizeLevel;
} /* ReadMaterial */

unsigned long Object3DS::ReadTriangles(FILE* pFile)
{
	unsigned int	sectionID;
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned long	shiftVertex;
	unsigned long	a;
	unsigned long	b;
	unsigned long	c;
	float			x1, y1, z1;
	float			x2, y2, z2;
	float			nx, ny, nz;
	float			norm;
	unsigned int	numTriangles;
	unsigned int	index;
	unsigned int	faceInfo;
	TrianglesData*	trianglesData;

//	FILE			*pFile;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	index			= 0;
	numTriangles	= ReadInt(pFile);

	if(numTriangles) {
		if(m_trianglesData) {
			index = m_numTriangles + numTriangles;
			m_trianglesData	= (TrianglesData*)
				realloc(m_trianglesData,sizeof(TrianglesData) * index);
			index	-= numTriangles;
		} else {
			m_trianglesData	= (TrianglesData*)
									malloc(sizeof(TrianglesData) * numTriangles);
		}/* if */

		shiftVertex		= m_shiftVertex;
		trianglesData	= m_trianglesData;
		
		while(numTriangles > 0 && currentPosition < endPosition) {
			a			= ReadInt(pFile) + shiftVertex;
			b			= ReadInt(pFile) + shiftVertex;
			c			= ReadInt(pFile) + shiftVertex;

			faceInfo	= ReadInt(pFile);
			faceInfo	&= 0x0007;

			trianglesData[index].firstIndex		= a;
			trianglesData[index].secondIndex	= b;
			trianglesData[index].thirdIndex		= c;

			
			x1	= m_vertexData[a].x -
					m_vertexData[c].x;
			y1	= m_vertexData[a].y -
					m_vertexData[c].y;
			z1	= m_vertexData[a].z -
					m_vertexData[c].z;
			x2	= m_vertexData[b].x -
					m_vertexData[a].x;
			y2	= m_vertexData[b].y -
					m_vertexData[a].y;
			z2	= m_vertexData[b].z -
					m_vertexData[a].z;

			nx	= y1 * z2 - z1 * y2;
			ny	= x2 * z1 - x1 * z2;
			nz	= x1 * y2 - y1 * x2;

			norm	= nx * nx + ny * ny + nz * nz;
			if(norm > .0000001) {
				norm = (float)(1.f/sqrt(norm));
				if(faceInfo&0x0003) {
					m_vertexData[c].nx	= nx * norm;
					m_vertexData[c].ny	= ny * norm;
					m_vertexData[c].nz	= nz * norm;
				} /* if */
				if(faceInfo&0x0006) {
					m_vertexData[b].nx	= nx * norm;
					m_vertexData[b].ny	= ny * norm;
					m_vertexData[b].nz	= nz * norm;
				} /* if */
				if(faceInfo&0x0005) {
					m_vertexData[a].nx	= nx * norm;
					m_vertexData[a].ny	= ny * norm;
					m_vertexData[a].nz	= nz * norm;
				} /* if */
			} else {
				nz	= 1;
				if(faceInfo&0x0003) {
					m_vertexData[c].nz	= nz;
				} /* if */
				if(faceInfo&0x0006) {
					m_vertexData[b].nz	= nz;
				} /* if */
				if(faceInfo&0x0005) {
					m_vertexData[a].nz	= nz;
				} /* if */

			}/* if */

			index++; numTriangles--;

			currentPosition	= ftell(pFile);

		} /* while */

		while(currentPosition < endPosition) {
			sectionID		= ReadSectionID(pFile);
			switch(sectionID) {
				case MSH_MAT_GROUP:
					ReadVertexMaterial(pFile, m_numTriangles);
					break;
				default:
					ReadUnknownLevel(pFile);
					break;
			} /* switch */
			currentPosition	= ftell(pFile);
		} /* while */

		m_numTriangles	= index;
	} /* if */

	return sizeLevel;
} /* ReadTriangles */

unsigned long Object3DS::ReadVertex(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned long	temp;
	unsigned int	numVertex;
	unsigned int	index;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	numVertex		= ReadInt(pFile);
	index			= 0;
	
	if(numVertex) {
		m_shiftVertex	= m_numVertex;

		if(m_vertexData) {
			index = m_numVertex + numVertex;
			m_vertexData	= (VertexData*)
				realloc(m_vertexData,
											sizeof(VertexData) * index);
			index	-= numVertex;
		} else {
			m_vertexData	= (VertexData*)
				malloc(sizeof(VertexData) * numVertex);
		}/* if */
		
		m_numVertex	+= numVertex;

		while(numVertex > 0 && currentPosition < endPosition) {

			memset(&m_vertexData[index], 0, sizeof(VertexData));
			temp	= ReadLong(pFile);
			m_vertexData[index].x	= *((float*)&temp);
			temp	= ReadLong(pFile);
			m_vertexData[index].y	= *((float*)&temp);
			temp	= ReadLong(pFile);
			m_vertexData[index].z	= *((float*)&temp);

			index++; numVertex--;

			currentPosition	= ftell(pFile);
		} /* while */

	} /* if */

	currentPosition	= ftell(pFile);
	if(currentPosition < endPosition) {
		fseek(pFile, endPosition - currentPosition, SEEK_CUR);
	} /* if */

	return sizeLevel;
} /* ReadVertex */

unsigned long Object3DS::ReadMeshLevel(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned int	sectionID;

//	FILE			*pFile;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	while(currentPosition < endPosition) {
		sectionID		= ReadSectionID(pFile);
		switch(sectionID) {
			case POINT_ARRAY:
				ReadVertex(pFile);
				break;
			case FACE_ARRAY:
				ReadTriangles(pFile);
				break;
			case TEX_VERTS:
				ReadTextures(pFile);
				break;
			case MESH_MATRIX:
				ReadLocalMatrix(pFile);
				break;
			default:
				ReadUnknownLevel(pFile);
				break;
		} /* switch */
		currentPosition	= ftell(pFile);
	}	

	return sizeLevel;
} /* ReadMeshLevel */

unsigned long Object3DS::ReadObjectLevel(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned int	sectionID;
	unsigned char	strName[256];
	int				index;

//	FILE			*pFile;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	index			= 0;
	strName[index]	= ReadByte(pFile);
	
	/* Load Object Name */
	while(strName[index] != 0 && index < 10) {
		index++;
		strName[index]	= ReadByte(pFile);
	} /* while */

	while(currentPosition < endPosition) {
		sectionID		= ReadSectionID(pFile);
		switch(sectionID) {
			case N_TRI_OBJECT:
				ReadMeshLevel(pFile);
				break;
			case N_DIRECT_LIGHT:
				ReadUnknownLevel(pFile);
				break;
			default:
				ReadUnknownLevel(pFile);
				break;
		} /* switch */
		currentPosition	= ftell(pFile);
	} /* while */

	return sizeLevel;
} /* ReadObjectLevel */

unsigned long Object3DS::ReadTextFileName(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned long	currMaterial;
	MaterialData*	materialData;
	char			strName[256];
	int				index;
	
//	FILE			*pFile;
	
//	pFile			= m_pFile;
	currMaterial	= m_numMaterial - 1;
	materialData	= &m_materialData[currMaterial];

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;


	index			= 0;
	strName[index]	= ReadByte(pFile);
	
	/* Load Object Name */
	while(strName[index] != 0 && index < 16) {
		index++;
		strName[index]	= ReadByte(pFile);
	} /* while */
	
	sprintf(strName + strlen(strName) - 3, "bmp");
	
	printf("Load texture pFile %s\n", strName);
	index = AddTexture(strName);
	
	if(index == 0) {
		m_numTextureFiles++;
		materialData->useTexture		= 1;
	} else {
		printf("Can't open texture pFile %s\n", strName);
	}/* if */

	currentPosition	= ftell(pFile);
	if(currentPosition < endPosition) {
		fseek(pFile, endPosition - currentPosition, SEEK_CUR);
	} /* if */

	return sizeLevel;
} /* ReadMaterialName */

unsigned long Object3DS::ReadTextureMap(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
//	FILE			*pFile;
	unsigned int	sectionID;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	while(currentPosition < endPosition) {
		sectionID		= ReadSectionID(pFile);
		switch(sectionID) {
			case MAT_MAPNAME:
				ReadTextFileName(pFile);
				break;
			default:
				ReadUnknownLevel(pFile);
				break;
		} /* switch */
		currentPosition	= ftell(pFile);
	} /* while */

	return sizeLevel;

} /* ReadTextureMap */

unsigned long Object3DS::ReadMaterialColor(FILE* pFile, ColorData *colorData)
{
	unsigned long	sizeLevel;
	unsigned long	sizeSubLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
//	FILE			*pFile;
	unsigned int	sectionID;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	while(currentPosition < endPosition) {
		sectionID		= ReadSectionID(pFile);
		switch(sectionID) {
			case COLOR_24:
				sizeSubLevel	= ReadSizeLevel(pFile) - 6;
				colorData->r	= (float)ReadByte(pFile)/255.f;
				colorData->g	= (float)ReadByte(pFile)/255.f;
				colorData->b	= (float)ReadByte(pFile)/255.f;
				break;
			default:
				sizeSubLevel	= ReadSizeLevel(pFile) - 6;
				fseek(pFile, sizeSubLevel, SEEK_CUR);
				break;
		} /* switch */
		currentPosition	= ftell(pFile);
	} /* while */

	return sizeLevel;
} /* ReadMaterialColor */

unsigned long Object3DS::ReadMaterialName(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned long	currMaterial;
	MaterialData*	materialData;
	unsigned char	*strName;
	int				index;
	
//	FILE			*pFile;
	
//	pFile			= m_pFile;
	currMaterial	= m_numMaterial - 1;
	materialData	= &m_materialData[currMaterial];
	strName			= (unsigned char*)&materialData->nameMaterial;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;


	index			= 0;
	strName[index]	= ReadByte(pFile);
	
	/* Load Object Name */
	while(strName[index] != 0 && index < 16) {
		index++;
		strName[index]	= ReadByte(pFile);
	} /* while */

	currentPosition	= ftell(pFile);
	if(currentPosition < endPosition) {
		fseek(pFile, endPosition - currentPosition, SEEK_CUR);
	} /* if */

	return sizeLevel;
} /* ReadMaterialName */

unsigned long Object3DS::ReadMaterial(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned int	sectionID;
	
	unsigned long	numMaterial;
//	FILE			*pFile;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;
	
	m_numMaterial++;
	numMaterial		= m_numMaterial;

	if(m_materialData) {
		m_materialData	= (MaterialData*)
			realloc(m_materialData,
										sizeof(MaterialData) * numMaterial);
	} else {
		m_materialData	= (MaterialData*)malloc(sizeof(MaterialData));
	}/* if */

	memset(&m_materialData[numMaterial-1], 0, sizeof(MaterialData));

	while(currentPosition < endPosition) {
		sectionID		= ReadSectionID(pFile);
		switch(sectionID) {
			case MAT_NAME:
				ReadMaterialName(pFile);
				break;
			case MAT_AMBIENT:
				ReadMaterialColor(pFile,
						&m_materialData[numMaterial-1].ambient);
				break;
			case MAT_DIFFUSE:
				ReadMaterialColor(pFile,
						&m_materialData[numMaterial-1].diffuse);
				break;
			case MAT_SPECULAR:
				ReadMaterialColor(pFile,
						&m_materialData[numMaterial-1].specular);
				break;
			case MAT_SHININESS:
				ReadMaterialColor(pFile,
						&m_materialData[numMaterial-1].shininess);
				break;
			case MAT_TWO_SIDE:
				ReadUnknownLevel(pFile);
				break;
			case MAT_FACEMAP:
				ReadUnknownLevel(pFile);
				break;
			case MAT_TEXMAP:
				ReadTextureMap(pFile);
				break;
			default:
				ReadUnknownLevel(pFile);
				break;
		} /* switch */
		currentPosition	= ftell(pFile);
	} /* while */

	return sizeLevel;
} /* ReadMaterial */

unsigned long Object3DS::ReadDataLevel(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	currentPosition;
	unsigned long	endPosition;
	unsigned int	sectionID;

//	FILE			*pFile;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	currentPosition	= ftell(pFile);
	endPosition		= currentPosition + sizeLevel;

	while(currentPosition < endPosition) {
		sectionID		= ReadSectionID(pFile);
		switch(sectionID) {
			case NAMED_OBJECT:
				ReadObjectLevel(pFile);
				break;
			case MAT_ENTRY:
				ReadMaterial(pFile);
				break;
			default:
				ReadUnknownLevel(pFile);
				break;
		} /* switch */
		currentPosition	= ftell(pFile);
	} /* while */

	return sizeLevel;
} /* ReadDataLevel */

int Object3DS::ReadVersion(FILE* pFile)
{
	unsigned long	sizeLevel;
	unsigned long	version;

//	FILE			*pFile;
	
//	pFile			= m_pFile;

	sizeLevel		= ReadSizeLevel(pFile) - 6;
	version			= ReadLong(pFile);
	if(sizeLevel == 4 && version >= 2) {
		return 1;
	} /* if */
	return 0;
} /* ReadVersion */

int Object3DS::Load(FILE* pFile)
{
	unsigned long	startPosition;
	unsigned long	endPosition;
	unsigned long	currentPosition;
	unsigned long	sizeLevel;
	unsigned int	sectionID;

//	FILE			*pFile;

//	file			= m_file;

	startPosition	= ftell(pFile);
	currentPosition	= ftell(pFile);
	sectionID		= ReadSectionID(pFile);
	sizeLevel		= ReadSizeLevel(pFile);
	endPosition		= startPosition + sizeLevel;

	while(currentPosition < endPosition) {
		sectionID		= ReadSectionID(pFile);
		switch(sectionID) {
			case M3D_VERSION:
				if(!ReadVersion(pFile)) {
					return 0;
				} /* if */
				break;
			case MDATA:
				ReadDataLevel(pFile);
				break;
			default:
				ReadUnknownLevel(pFile);
				break;
		} /* switch */
		currentPosition	= ftell(pFile);

	} /* while */

	return 1;

} /* ReadFirstLevel */

void Object3DS::Normalize()
{
//	unsigned long		k;
	int k;
	float				xMin, xMax, yMin, yMax, zMin, zMax, maxLength;

	//normalize
	xMin	= m_vertexData[0].x;
	xMax	= m_vertexData[0].x;
	yMin	= m_vertexData[0].y;
	yMax	= m_vertexData[0].y;
	zMin	= m_vertexData[0].z;
	zMax	= m_vertexData[0].z;
	for(k = 0; k < m_numVertex; k++) {
		if(xMin > m_vertexData[k].x) {
			xMin = m_vertexData[k].x;
		} 
		if(yMin > m_vertexData[k].y) {
			yMin = m_vertexData[k].y;
		} 
		if(zMin > m_vertexData[k].z) {
			zMin = m_vertexData[k].z;
		} 
		if(xMax < m_vertexData[k].x) {
			xMax = m_vertexData[k].x;
		} 
		if(yMax < m_vertexData[k].y) {
			yMax = m_vertexData[k].y;
		} 
		if(zMax < m_vertexData[k].z) {
			zMax = m_vertexData[k].z;
		} 
	} 
	maxLength	= __max( (zMax - zMin), 
		__max(xMax - xMin, yMax - yMin) )/2;
		
	for(k = 0; k < m_numVertex; k++) 
	{
		m_vertexData[k].x /= maxLength;
		m_vertexData[k].y /= maxLength;
		m_vertexData[k].z /= maxLength;
	} 
}
int Object3DS::AddTexture(char* fileName)
{
	FILE*				pFile;
	unsigned char*		fileData;
	unsigned char*		temp;
	unsigned char*		img;
	char*				fileTexture=0;
	unsigned long		index;
	unsigned long		indexX;
	unsigned long		indexY;
	unsigned long		fileSize;
	unsigned long		size;
	unsigned long		numColor;
	unsigned long		width;
	unsigned long		height;
	unsigned long		widthGlobal;
	unsigned long		heightGlobal;
	BITMAPFILEHEADER	*bmpFileHeader;
	RGBQUAD				*rgb;
	BITMAPINFOHEADER	*bmpInfoHeader;
	TextureData			*textureData;

	pFile	= fopen(fileName, "rb");
	
	if(pFile == 0) {
		return -1;
	} /* if */
	fseek(pFile, 0, SEEK_END);
	fileSize	= ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	if(	fileSize < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO) +
		sizeof(BITMAPINFOHEADER)) {
		fclose(pFile);
		return -2;
	} /* if */

	temp	= (unsigned char*)calloc(1, fileSize);
	if(temp == 0) {
		fclose(pFile);
		return -3;
	} /* if */
	
	fileData	= temp;
	fread(temp, sizeof(char), fileSize, pFile);
	fclose(pFile);

	bmpFileHeader	= (BITMAPFILEHEADER*)temp;
	temp			+= sizeof(BITMAPFILEHEADER);
	bmpInfoHeader	= (BITMAPINFOHEADER*)temp;
	temp			+= sizeof(BITMAPINFOHEADER);
	rgb				= (RGBQUAD*)temp;
	width			= bmpInfoHeader->biWidth;
	height			= bmpInfoHeader->biHeight;

	if(bmpInfoHeader->biBitCount != 24) {
		numColor	= 1 << bmpInfoHeader->biBitCount;
	} else {
		numColor	= 0;
	}/* if */

	temp			+=sizeof(RGBQUAD)*numColor;
	
	switch(bmpInfoHeader->biBitCount) {
		case 1:
			img		= temp + ((width/8 + 3)&~3)*(height - 1);
			break;
		case 4:
			img		= temp + ((width/2 + 3)&~3)*(height - 1);
			break;
		case 8:
			img		= temp + ((width + 3)&~3)*(height - 1);
			break;
		case 24:
			img		= temp + ((width*3 + 3)&~3)*(height - 1);
			break;
		default:
		free(fileData);
		return -4;
	} /* switch */

	textureData	=
		&m_materialData[m_numMaterial -1 ].textureData;

	if(m_sizeGlobalBitmap == 0) {

		m_globalTexture.x0 = 1;
		m_globalTexture.y0 = 0;

		widthGlobal		= width;
		heightGlobal	= height;

		if(	width == 0 || height == 0 ) {
			free(fileData);
		} /* if */

		widthGlobal++;
		for(index = 1; index < widthGlobal; index <<= 1) ;
		widthGlobal		= index;
		for(index = 1; index < heightGlobal; index <<= 1) ;
		heightGlobal	= index;

		size	= sizeof(BITMAPINFO) + sizeof(BITMAPFILEHEADER);
		size	+= heightGlobal * widthGlobal * sizeof(unsigned char) * 3;

		if (fileTexture) fileTexture = (char*)realloc(fileTexture,size);
		else fileTexture = (char*)malloc(size);

		if(fileTexture == 0) {
			free(fileData);
			return -4;
		} /* if */

		m_globalBitmap	= fileTexture;

		memset(fileTexture, 0, sizeof(BITMAPFILEHEADER));
		((BITMAPFILEHEADER*)fileTexture)->bfType	= 0x4D42;
		((BITMAPFILEHEADER*)fileTexture)->bfOffBits	= 
					sizeof(BITMAPINFOHEADER) +  sizeof(BITMAPFILEHEADER);
		

		fileTexture	+= sizeof(BITMAPFILEHEADER);
		memset(fileTexture, 0, sizeof(BITMAPINFOHEADER));
		((BITMAPINFOHEADER*)fileTexture)->biSize		= 
													sizeof(BITMAPINFOHEADER);
		((BITMAPINFOHEADER*)fileTexture)->biWidth		= widthGlobal;
		((BITMAPINFOHEADER*)fileTexture)->biHeight		= heightGlobal;
		((BITMAPINFOHEADER*)fileTexture)->biPlanes		= 1;
		((BITMAPINFOHEADER*)fileTexture)->biBitCount	= 24;
		((BITMAPINFOHEADER*)fileTexture)->biCompression	= BI_RGB;
		fileTexture	+= sizeof(BITMAPINFOHEADER);
		memset(fileTexture, 0xFF, widthGlobal * 3 * heightGlobal);
		fileTexture += (widthGlobal * 3 * (heightGlobal - height));

		{ /* copy Data */
			for(indexY = 0; indexY < height; indexY++) {
				unsigned char p, r, g, b;
				
				index		= 0;
				fileTexture[index*3  ] = ((unsigned char)255);
				fileTexture[index*3+1] = ((unsigned char)255);
				fileTexture[index*3+2] = ((unsigned char)255);
				index++;
				switch(bmpInfoHeader->biBitCount) {
					case 1:
						for(indexX = 0; indexX < width; indexX++) {
							p = (img[indexX/8]>>(7 - indexX%8))&0x01;
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;
						} /* for */
						img -= (bmpInfoHeader->biWidth/8 + 3) & ~3;
						break;
					case 4:
						for(indexX = 0; indexX < width/2; indexX++) {
							p = img[indexX]>>4;
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;

							p = (img[indexX]&0x0F);
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;
						} /* for */
						img -= (bmpInfoHeader->biWidth/2+3) & ~3;
						break;
					case 8:
						for(indexX = 0; indexX < width; indexX++) {
							p = img[indexX];
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;
						} /* for */
						img -= ((width+3) & ~3);
						break;
					case 24:
						for(indexX = 0; indexX < width; indexX++) {
							fileTexture[index*3  ] = img[indexX*3  ];
							fileTexture[index*3+1] = img[indexX*3+1];
							fileTexture[index*3+2] = img[indexX*3+2];
							index++;
						} /* for */
						img -= ((width*3+3) & ~3);
						break;
					default:
						free(fileData);
						return -4;
				} /* switch */
				fileTexture += (widthGlobal*3);
			} /* for */

		} /* end copy Data */
		textureData->x0	= 1;
		textureData->y0	= 0;

	} else {
		unsigned long	x0;
		unsigned long	y0;

		textureData->x0	= m_globalTexture.x0;
		textureData->y0	= m_globalTexture.y0;
		
		x0	= textureData->x0;
		y0	= textureData->y0;

		if(	x0 + bmpInfoHeader->biWidth <= m_globalTexture.width &&
			y0 + bmpInfoHeader->biHeight<= m_globalTexture.height) {
			widthGlobal		= m_globalTexture.width;
			heightGlobal	= m_globalTexture.height;
			size			= m_sizeGlobalBitmap;

		} else {
			char*	dataOld;
			if(y0 + bmpInfoHeader->biHeight <= m_globalTexture.height) {
				widthGlobal		= x0 + bmpInfoHeader->biWidth;
				heightGlobal	= m_globalTexture.height;

				for(index = 1; index < widthGlobal; index <<= 1) ;
				widthGlobal		= index;
			}  else if(	x0 + bmpInfoHeader->biWidth <= m_globalTexture.width) {
				widthGlobal		= m_globalTexture.width;
				heightGlobal	= y0 + bmpInfoHeader->biHeight;

				for(index = 1; index < heightGlobal; index <<= 1) ;
				heightGlobal	= index;
			} else {
				widthGlobal		= x0 + bmpInfoHeader->biWidth;
				heightGlobal	= y0 + bmpInfoHeader->biHeight;
 
				for(index = 1; index < widthGlobal; index <<= 1) ;
				widthGlobal		= index;
				for(index = 1; index < heightGlobal; index <<= 1) ;
				heightGlobal	= index;
			} /* if */
			
			size	= sizeof(BITMAPINFO) + sizeof(BITMAPFILEHEADER);
			size	+= heightGlobal * widthGlobal * sizeof(unsigned char) * 3;

			fileTexture	= (char*)calloc(1, size);
			temp		= (unsigned char*)fileTexture;

			if(fileTexture == 0) {
				free(fileData);
				return -4;
			} /* if */
			memcpy(fileTexture, m_globalBitmap, 
				sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER));

			((BITMAPFILEHEADER*)fileTexture)->bfType	= 0x4D42;
			((BITMAPFILEHEADER*)fileTexture)->bfOffBits	= 
						sizeof(BITMAPINFOHEADER) +  sizeof(BITMAPFILEHEADER);
			

			fileTexture	+= sizeof(BITMAPFILEHEADER);
			((BITMAPINFOHEADER*)fileTexture)->biSize		= 
														sizeof(BITMAPINFOHEADER);
			((BITMAPINFOHEADER*)fileTexture)->biWidth		= widthGlobal;
			((BITMAPINFOHEADER*)fileTexture)->biHeight		= heightGlobal;
			((BITMAPINFOHEADER*)fileTexture)->biPlanes		= 1;
			((BITMAPINFOHEADER*)fileTexture)->biBitCount	= 24;
			((BITMAPINFOHEADER*)fileTexture)->biCompression	= BI_RGB;
			fileTexture	+= sizeof(BITMAPINFOHEADER);
			memset(fileTexture, 0xFF, widthGlobal * 3 * heightGlobal);
			fileTexture	+= widthGlobal * 3 * 
				(heightGlobal - m_globalTexture.height);
			
			dataOld		=	m_globalBitmap  + sizeof(BITMAPINFOHEADER)
							+ sizeof(BITMAPFILEHEADER);

			for(indexY = 0; indexY < m_globalTexture.height; indexY++) {
				memcpy(	fileTexture + indexY * widthGlobal * 3,
						dataOld	+ indexY* m_globalTexture.width*3, 
						m_globalTexture.width*3);
			} /* for */


			free(m_globalBitmap);
			m_globalBitmap	= (char*)temp;

		} /* if */

		fileTexture		= m_globalBitmap;
		fileTexture		+= sizeof(BITMAPFILEHEADER);
		fileTexture		+= sizeof(BITMAPINFOHEADER);
		
		fileTexture		+= (widthGlobal * 3 * (heightGlobal - height));
		
		{ /* copy Data */
			for(indexY = heightGlobal - height; indexY < heightGlobal; indexY++) {
				unsigned char p, r, g, b;
				
				index		= x0;
				switch(bmpInfoHeader->biBitCount) {
					case 1:
						for(indexX = 0; indexX < width; indexX++) {
							p = (img[indexX/8]>>(7 - indexX%8))&0x01;
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;
						} /* for */
						img -= (bmpInfoHeader->biWidth/8 + 3) & ~3;
						break;
					case 4:
						for(indexX = 0; indexX < width/2; indexX++) {
							p = img[indexX]>>4;
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;

							p = (img[indexX]&0x0F);
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;
						} /* for */
						img -= (bmpInfoHeader->biWidth/2+3) & ~3;
						break;
					case 8:
						for(indexX = 0; indexX < width; indexX++) {
							p = img[indexX];
							r = rgb[p].rgbRed;
							g = rgb[p].rgbGreen;
							b = rgb[p].rgbBlue;
							fileTexture[index*3  ] = b;
							fileTexture[index*3+1] = g;
							fileTexture[index*3+2] = r;
							index++;
						} /* for */
						img -= ((width+3) & ~3);
						break;
					case 24:
						for(indexX = 0; indexX < width; indexX++) {
							fileTexture[index*3  ] = img[indexX*3  ];
							fileTexture[index*3+1] = img[indexX*3+1];
							fileTexture[index*3+2] = img[indexX*3+2];
							index++;
						} /* for */
						img -= ((width*3+3) & ~3);
						break;
					default:
						free(fileData);
						return -4;
				} /* switch */
				fileTexture += (widthGlobal*3);
			} /* for */
		} /* end copy Data */

	} /* if */

	textureData->width	= width;
	textureData->height	= height;

	((BITMAPFILEHEADER*)m_globalBitmap)->bfSize	= size;
	m_sizeGlobalBitmap	= size;
	m_globalTexture.width	= widthGlobal;
	m_globalTexture.height= heightGlobal;

	m_globalTexture.x0	+= bmpInfoHeader->biWidth;
	m_globalTexture.y0	= 0;
	free(fileData);

	return 0;
} /* AddTexture */

void Object3DS::CalcPlanes(int algorithm)
{
	if (m_planeData) { delete m_planeData; m_planeData = 0;}
	if (m_side1Plane) { delete m_side1Plane; m_side1Plane = 0; }
	if (m_side2Plane) { delete m_side2Plane; m_side2Plane = 0; }
	if (m_side3Plane) { delete m_side3Plane; m_side3Plane = 0; }

if (algorithm == 0)
{
	int i;
	Ipp32f *pNormal, pTmp1[3], pTmp2[3], *pD;
	m_planeData = new PlaneData[m_numTriangles];
	for (i=0; i<m_numTriangles; i++)
	{
		pNormal = &(m_planeData[i].A);
		pD = &(m_planeData[i].D);
		pTmp1[0] =
			m_vertexData[m_trianglesData[i].secondIndex].x -
			m_vertexData[m_trianglesData[i].firstIndex].x;
		pTmp1[1] =
			m_vertexData[m_trianglesData[i].secondIndex].y -
			m_vertexData[m_trianglesData[i].firstIndex].y;
		pTmp1[2] =
			m_vertexData[m_trianglesData[i].secondIndex].z-
			m_vertexData[m_trianglesData[i].firstIndex].z;
		pTmp2[0] =
			m_vertexData[m_trianglesData[i].thirdIndex].x -
			m_vertexData[m_trianglesData[i].firstIndex].x;
		pTmp2[1] =
			m_vertexData[m_trianglesData[i].thirdIndex].y -
			m_vertexData[m_trianglesData[i].firstIndex].y;
		pTmp2[2] =
			m_vertexData[m_trianglesData[i].thirdIndex].z-
			m_vertexData[m_trianglesData[i].firstIndex].z;
		ippmCrossProduct_vv_32f_3x1(pTmp1, pTmp2, pNormal);
		ippmDotProduct_vv_32f_3x1(pNormal,
			&(m_vertexData[m_trianglesData[i].firstIndex].x),
			pD);
		*pD=-*pD;
	}
}
else if (algorithm == 10)
{
	int i;
	Ipp32f *pNormal, pTmp1[3], pTmp2[3], *pD;
	m_planeData = new PlaneData[m_numTriangles];
	for (i=0; i<m_numTriangles; i++)
	{
		pNormal = &(m_planeData[i].A);
		pD = &(m_planeData[i].D);
		pTmp1[0] =
			m_vertexData[m_trianglesData[i].secondIndex].x -
			m_vertexData[m_trianglesData[i].firstIndex].x;
		pTmp1[1] =
			m_vertexData[m_trianglesData[i].secondIndex].y -
			m_vertexData[m_trianglesData[i].firstIndex].y;
		pTmp1[2] =
			m_vertexData[m_trianglesData[i].secondIndex].z-
			m_vertexData[m_trianglesData[i].firstIndex].z;
		pTmp2[0] =
			m_vertexData[m_trianglesData[i].thirdIndex].x -
			m_vertexData[m_trianglesData[i].firstIndex].x;
		pTmp2[1] =
			m_vertexData[m_trianglesData[i].thirdIndex].y -
			m_vertexData[m_trianglesData[i].firstIndex].y;
		pTmp2[2] =
			m_vertexData[m_trianglesData[i].thirdIndex].z-
			m_vertexData[m_trianglesData[i].firstIndex].z;
		ippmCrossProduct_vv_32f_3x1(pTmp1, pTmp2, pNormal);
		ippmDotProduct_vv_32f_3x1(pNormal,
			&(m_vertexData[m_trianglesData[i].firstIndex].x),
			pD);
		*pD=-*pD;
	}
}
else if (algorithm == 1)
{
	int i;
	Ipp32f *pNormal, pTmp1[3], pTmp2[3], *pD;
	m_planeData = new PlaneData[m_numTriangles];
	const int bufSize = 1;
	Ipp32s pFirst[bufSize], pSecond[bufSize],
		pThird[bufSize];
	typedef Ipp32f* pIpp32f;
	Ipp32f **pTrianglesSOA = new pIpp32f[3];
	pTrianglesSOA[0] = (Ipp32f*)pFirst;
	pTrianglesSOA[1] = (Ipp32f*)pSecond;
	pTrianglesSOA[2] = (Ipp32f*)pThird;
	
	Ipp32f **pTrianglesAOS = new pIpp32f[3];

	//	for (i=0; i<m_numTriangles-1; i++)
	for (i=0; i<m_numTriangles; i++)
	{
		pTrianglesAOS[0] = (Ipp32f*)&m_trianglesData[i].firstIndex;
		pTrianglesAOS[1] = (Ipp32f*)&m_trianglesData[i].secondIndex;
		pTrianglesAOS[2] = (Ipp32f*)&m_trianglesData[i].thirdIndex;
//		Ipp32f *pTrianglesSOA[3] =
//		{
//			(Ipp32f*)&m_trianglesData[i].firstIndex,
//			(Ipp32f*)&m_trianglesData[i].secondIndex,
//			(Ipp32f*)&m_trianglesData[i].thirdIndex
//		};
//		ippmCopy_va_32f_PP((const Ipp32f**)pTrianglesAOS, 0, 12,
//			(Ipp32f**)pTrianglesSOA, 0, 4, 3, bufSize);
//		ippmCopy_va_32f_PP(pTrianglesAOS, 0, 12,
//			pTrianglesSOA, 0, 4, 3, bufSize);
//		ippmCopy_va_32f_PP((const Ipp32f**)pTrianglesAOS, 0, 3,
//			pTrianglesSOA, 0, 1, 3, bufSize);
		IppiSize size = {bufSize, 1};
		ippiCopy_32f_C3P3R(pTrianglesAOS[0], bufSize*3*4,
			pTrianglesSOA, bufSize*4, size);
		pNormal = &(m_planeData[i].A);
		pD = &(m_planeData[i].D);
		pTmp1[0] = m_vertexData[pSecond[0]].x -
			m_vertexData[pFirst[0]].x;
		pTmp1[1] = m_vertexData[pSecond[0]].y -
			m_vertexData[pFirst[0]].y;
		pTmp1[2] = m_vertexData[pSecond[0]].z -
			m_vertexData[pFirst[0]].z;
		pTmp2[0] = m_vertexData[pThird[0]].x -
			m_vertexData[pFirst[0]].x;
		pTmp2[1] = m_vertexData[pThird[0]].y -
			m_vertexData[pFirst[0]].y;
		pTmp2[2] = m_vertexData[pThird[0]].z -
			m_vertexData[pFirst[0]].z;
		ippmCrossProduct_vv_32f_3x1(pTmp1, pTmp2, pNormal);
		ippmDotProduct_vv_32f_3x1(pNormal,
			&(m_vertexData[pFirst[0]].x),
			pD);
		*pD=-*pD;
	}
}
else if (algorithm == 2)
{
	int i,j;
	m_planeData = new PlaneData[m_numTriangles];
//	m_side1Plane = new PlaneData[m_numTriangles];
//	m_side2Plane = new PlaneData[m_numTriangles];
//	m_side3Plane = new PlaneData[m_numTriangles];
//	const int bufSize = 1;
	const int bufSize = 256;
	Ipp32f *pNormal, pTmp1[3*bufSize], pTmp2[3*bufSize], *pD;
	int loopSize = bufSize;
	typedef Ipp32f* pIpp32f;

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize) loopSize = m_numTriangles - i;

		for (j=0; j<loopSize; j++)
		{
			int p1 = m_trianglesData[i+j].firstIndex;
			int p2 = m_trianglesData[i+j].secondIndex;
			int p3 = m_trianglesData[i+j].thirdIndex;

			pTmp1[j*3] =
				m_vertexData[p2].x - m_vertexData[p1].x;
			pTmp1[j*3+1] =
				m_vertexData[p2].y - m_vertexData[p1].y;
			pTmp1[j*3+2] =
				m_vertexData[p2].z - m_vertexData[p1].z;

			pTmp2[j*3] =
				m_vertexData[p3].x - m_vertexData[p1].x;
			pTmp2[j*3+1] =
				m_vertexData[p3].y - m_vertexData[p1].y;
			pTmp2[j*3+2] =
				m_vertexData[p3].z - m_vertexData[p1].z;
		}
		for (j=0; j<loopSize; j++)
		{
			int p1 = m_trianglesData[i+j].firstIndex;
			pNormal = &(m_planeData[i+j].A);
			pD = &(m_planeData[i+j].D);
			ippmCrossProduct_vv_32f_3x1(pTmp1+j*3, pTmp2+j*3, pNormal);
			ippmDotProduct_vv_32f_3x1(pNormal,
				&(m_vertexData[p1].x),
				pD);
			*pD=-*pD;
		}

/*		ippmCrossProduct_vava_32f_3x1(pTmp1,
									, pTmp2+j*3, pNormal);
			ippmDotProduct_vv_32f_3x1(pNormal,
				&(m_vertexData[pFirst[j]].x),
				pD);
*/
//		for (j=0; j<loopSize; j++)
//		{
//			Vab = Pa - Pb;
//			t1 = cross(Vab,Nplane);
//			t2 = dot(Vab,Nplane);
//			Vab = t2 * Vab;
//			Nab = t1 + Vab;
//			Dab = -dot(Vab,Pa);
//			if (dot(Nab,Pc) < 0) Nab = -Nab;
//			... Repeat for Vbc and Vca ...
//		}
	}
}
else if (algorithm == 20)
{
	int i,j;
	m_planeData = new PlaneData[m_numTriangles];
//	m_side1Plane = new PlaneData[m_numTriangles];
//	m_side2Plane = new PlaneData[m_numTriangles];
//	m_side3Plane = new PlaneData[m_numTriangles];
//	const int bufSize = 1;
	const int bufSize = 256;
	Ipp32f *pNormal, pTmp1[3*bufSize], pTmp2[3*bufSize], *pD;
	int loopSize = bufSize;
	Ipp32s pFirst[bufSize], pSecond[bufSize],
		pThird[bufSize];
	Ipp32f *pTrianglesSOA[3] =
	{ (Ipp32f*)pFirst, (Ipp32f*)pSecond, (Ipp32f*)pThird };
	typedef Ipp32f* pIpp32f;

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize) loopSize = m_numTriangles - i;

		IppiSize size = {loopSize, 1};
		ippiCopy_32f_C3P3R((const Ipp32f*)&m_trianglesData[i], bufSize*3*4,
			pTrianglesSOA, bufSize*4, size);
		for (j=0; j<loopSize; j++)
		{
			pTmp1[j*3] = m_vertexData[pSecond[j]].x -
				m_vertexData[pFirst[j]].x;
			pTmp1[j*3+1] = m_vertexData[pSecond[j]].y -
				m_vertexData[pFirst[j]].y;
			pTmp1[j*3+2] = m_vertexData[pSecond[j]].z -
				m_vertexData[pFirst[j]].z;
		}
		for (j=0; j<loopSize; j++)
		{
			pTmp2[j*3] = m_vertexData[pThird[j]].x -
				m_vertexData[pFirst[j]].x;
			pTmp2[j*3+1] = m_vertexData[pThird[j]].y -
				m_vertexData[pFirst[j]].y;
			pTmp2[j*3+2] = m_vertexData[pThird[j]].z -
				m_vertexData[pFirst[j]].z;
		}
		for (j=0; j<loopSize; j++)
		{
			pNormal = &(m_planeData[i+j].A);
			pD = &(m_planeData[i+j].D);
			ippmCrossProduct_vv_32f_3x1(pTmp1+j*3, pTmp2+j*3, pNormal);
			ippmDotProduct_vv_32f_3x1(pNormal,
				&(m_vertexData[pFirst[j]].x),
				pD);
			*pD=-*pD;
		}

/*		ippmCrossProduct_vava_32f_3x1(pTmp1,
									, pTmp2+j*3, pNormal);
			ippmDotProduct_vv_32f_3x1(pNormal,
				&(m_vertexData[pFirst[j]].x),
				pD);
*/
//		for (j=0; j<loopSize; j++)
//		{
//			Vab = Pa - Pb;
//			t1 = cross(Vab,Nplane);
//			t2 = dot(Vab,Nplane);
//			Vab = t2 * Vab;
//			Nab = t1 + Vab;
//			Dab = -dot(Vab,Pa);
//			if (dot(Nab,Pc) < 0) Nab = -Nab;
//			... Repeat for Vbc and Vca ...
//		}
	}
}
else if (algorithm == 3)
{
	int i,j;
	m_planeData = new PlaneData[m_numTriangles];
	const int bufSize = 256;
	Ipp32f *pNormal, pTmp1[3*bufSize], pTmp2[3*bufSize], *pD;
	int loopSize = bufSize;
	typedef Ipp32f* pIpp32f;

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize) loopSize = m_numTriangles - i;

		for (j=0; j<loopSize; j++)
		{
			int p1 = m_trianglesData[i+j].firstIndex;
			int p2 = m_trianglesData[i+j].secondIndex;
			int p3 = m_trianglesData[i+j].thirdIndex;

			pTmp1[j*3] =
				m_vertexData[p2].x - m_vertexData[p1].x;
			pTmp1[j*3+1] =
				m_vertexData[p2].y - m_vertexData[p1].y;
			pTmp1[j*3+2] =
				m_vertexData[p2].z - m_vertexData[p1].z;

			pTmp2[j*3] =
				m_vertexData[p3].x - m_vertexData[p1].x;
			pTmp2[j*3+1] =
				m_vertexData[p3].y - m_vertexData[p1].y;
			pTmp2[j*3+2] =
				m_vertexData[p3].z - m_vertexData[p1].z;
		}

		IppStatus st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 3*sizeof(Ipp32f),
			pTmp2, 3*sizeof(Ipp32f),
			(float*)&m_planeData[i], sizeof(PlaneData),
			loopSize);
		for (j=0; j<loopSize; j++)
		{
			int p1 = m_trianglesData[i+j].firstIndex;
			pNormal = &(m_planeData[i+j].A);
			pD = &(m_planeData[i+j].D);

			ippmDotProduct_vv_32f_3x1(
				pNormal,
				&(m_vertexData[p1].x),
				pD);
			*pD = -*pD;
		}
	}
}
else if (algorithm == 31)
{
	int i,j;
	m_planeData = new PlaneData[m_numTriangles];
//	m_side1Plane = new PlaneData[m_numTriangles];
//	m_side2Plane = new PlaneData[m_numTriangles];
//	m_side3Plane = new PlaneData[m_numTriangles];
//	const int bufSize = 1;
	const int bufSize = 256;
	Ipp32f *pNormal, pTmp1[3*bufSize], pTmp2[3*bufSize], *pD;
	int loopSize = bufSize;
	Ipp32s pFirst[bufSize], pSecond[bufSize],
		pThird[bufSize];
	Ipp32f *pTrianglesSOA[3] =
	{ (Ipp32f*)pFirst, (Ipp32f*)pSecond, (Ipp32f*)pThird };
	typedef Ipp32f* pIpp32f;

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize) loopSize = m_numTriangles - i;

		IppiSize size = {loopSize, 1};
		ippiCopy_32f_C3P3R((const Ipp32f*)&m_trianglesData[i], bufSize*3*4,
			pTrianglesSOA, bufSize*4, size);
		for (j=0; j<loopSize; j++)
		{
			pTmp1[j*3] = m_vertexData[pSecond[j]].x -
				m_vertexData[pFirst[j]].x;
			pTmp1[j*3+1] = m_vertexData[pSecond[j]].y -
				m_vertexData[pFirst[j]].y;
			pTmp1[j*3+2] = m_vertexData[pSecond[j]].z -
				m_vertexData[pFirst[j]].z;
		}
		for (j=0; j<loopSize; j++)
		{
			pTmp2[j*3] = m_vertexData[pThird[j]].x -
				m_vertexData[pFirst[j]].x;
			pTmp2[j*3+1] = m_vertexData[pThird[j]].y -
				m_vertexData[pFirst[j]].y;
			pTmp2[j*3+2] = m_vertexData[pThird[j]].z -
				m_vertexData[pFirst[j]].z;
		}
//		for (j=0; j<loopSize; j++)
//		{
//			pNormal = &(m_planeData[i+j].A);
//			pD = &(m_planeData[i+j].D);
//			ippmCrossProduct_vv_32f_3x1(pTmp1+j*3, pTmp2+j*3, pNormal);
//			ippmDotProduct_vv_32f_3x1(pNormal,
//				&(m_vertexData[pFirst[j]].x),
//				pD);
//			*pD=-*pD;
//		}

		IppStatus st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 3*sizeof(Ipp32f),
			pTmp2, 3*sizeof(Ipp32f),
//			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], sizeof(PlaneData),
			loopSize);
		for (j=0; j<loopSize; j++)
		{
			pNormal = &(m_planeData[i+j].A);
			pD = &(m_planeData[i+j].D);

			ippmDotProduct_vv_32f_3x1(
				pNormal,
				&(m_vertexData[pFirst[j]].x),
				pD);
			*pD = -*pD;
//			st = ippmDotProduct_vava_32f_3x1(
//				(float*)&m_planeData[i], 4*sizeof(Ipp32f),
//				pVert1, 4*sizeof(Ipp32f),
//				pD, loopSize);
		}
//		ippsMulC_32f_I(-1.0, pD, loopSize);
//		ippsMulC_32f_I(-1.0, pTmp1, loopSize);
//		ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),

//		ippiCopy_32f_C1C4R(pTmp1, loopSize*sizeof(Ipp32f),
//			(float*)&m_planeData[i].D, loopSize*4*sizeof(Ipp32f),
//			size);
		
/*		ippmCrossProduct_vava_32f_3x1(pTmp1,
									, pTmp2+j*3, pNormal);
			ippmDotProduct_vv_32f_3x1(pNormal,
				&(m_vertexData[pFirst[j]].x),
				pD);
*/
//		for (j=0; j<loopSize; j++)
//		{
//			Vab = Pa - Pb;
//			t1 = cross(Vab,Nplane);
//			t2 = dot(Vab,Nplane);
//			Vab = t2 * Vab;
//			Nab = t1 + Vab;
//			Dab = -dot(Vab,Pa);
//			if (dot(Nab,Pc) < 0) Nab = -Nab;
//			... Repeat for Vbc and Vca ...
//		}
	}
}
else if (algorithm == 30)
{
	int i,j;
	m_planeData = new PlaneData[m_numTriangles];
	const int bufSize = 1;
	Ipp32f *pNormal, pTmp1[3*bufSize], pTmp2[3*bufSize], *pD;
	int loopSize = bufSize;
	Ipp32s pFirst[bufSize], pSecond[bufSize],
		pThird[bufSize];
	Ipp32f *pTrianglesSOA[3] =
	{ (Ipp32f*)pFirst, (Ipp32f*)pSecond, (Ipp32f*)pThird };
	typedef Ipp32f* pIpp32f;

	int pTmpIndices[bufSize];
	ippsVectorRamp_32s(pTmpIndices, bufSize, 4.0, 3.0*4.0);

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize) loopSize = m_numTriangles - i;

		IppiSize size = {loopSize, 1};
		ippiCopy_32f_C3P3R(
			(const Ipp32f*)&m_trianglesData[i],
			bufSize*3*4, pTrianglesSOA, bufSize*4, size);
		ippsMulC_32s_ISfs(sizeof(VertexData),
						  pFirst, bufSize,0);
//		ippsAddC_32s_ISfs(sizeof(VertexData),
//						  pFirst, bufSize,0);
		ippsMulC_32s_ISfs(sizeof(VertexData),
						  pSecond, bufSize,0);
//		ippsAddC_32s_ISfs(sizeof(VertexData),
//						  pSecond, bufSize,0);
		ippsMulC_32s_ISfs(sizeof(VertexData),
						  pThird, bufSize,0);
//		ippsAddC_32s_ISfs(sizeof(VertexData),
//						  pThird, bufSize,0);
		IppStatus st = ippmSub_vava_32f_3x1_L(
			(const Ipp32f**)pSecond, (Ipp32s)(m_vertexData-1),
			(const Ipp32f**)pFirst, (Ipp32s)(m_vertexData-1),
			(Ipp32f**)pTmpIndices, (Ipp32s)(pTmp1-1),
			loopSize);
		st = ippmSub_vava_32f_3x1_L(
			(const Ipp32f**)pThird, (Ipp32s)(m_vertexData-1),
			(const Ipp32f**)pFirst, (Ipp32s)(m_vertexData-1),
			(Ipp32f**)pTmpIndices, (Ipp32s)(pTmp2-1),
			loopSize);
		for (j=0; j<loopSize; j++)
		{
			pNormal = &(m_planeData[i+j].A);
			pD = &(m_planeData[i+j].D);
//			ippmCrossProduct_vv_32f_3x1(pTmp1+j*3, pTmp2+j*3, pNormal);
			pFirst[j]-=sizeof(VertexData);
			pFirst[j]/=sizeof(VertexData);
//			ippmDotProduct_vv_32f_3x1(pNormal,
//				&(m_vertexData[pFirst[j]].x),
//				pD);
			*pD=-*pD;
		}
	}
}
else if (algorithm == 41)
{
	int i;
//	IppStatus st;
	m_planeData = new PlaneData[m_numTriangles];
//	m_side1Plane = new PlaneData[m_numTriangles];
//	m_side2Plane = new PlaneData[m_numTriangles];
//	m_side3Plane = new PlaneData[m_numTriangles];
//	const int bufSize = 1;
//	const int bufSize = 3072;
	const int bufSize = 256;
	int loopSize = bufSize;
/*	BufAlloc_((4+4+4+4+4+1)*sizeof(Ipp32f)*bufSize+
			  3*sizeof(Ipp32f*)*bufSize);
	Ipp32f* pTmp1 = (Ipp32f*)m_pBuffer_;
	Ipp32f* pTmp2 = &((Ipp32f*)m_pBuffer_)[4*bufSize];
	Ipp32f* pVert1 = &((Ipp32f*)m_pBuffer_)[8*bufSize];
	Ipp32f* pVert2 = &((Ipp32f*)m_pBuffer_)[12*bufSize];
	Ipp32f* pVert3 = &((Ipp32f*)m_pBuffer_)[16*bufSize];
	Ipp32f* pD = &((Ipp32f*)m_pBuffer_)[20*bufSize];
	Ipp32f* pDummy = &((Ipp32f*)m_pBuffer_)[21*bufSize];
	Ipp32f** pPtrs1 = (Ipp32f**)pDummy;
	Ipp32f** pPtrs2 = &((Ipp32f**)pDummy)[bufSize];
	Ipp32f** pPtrs3 = &((Ipp32f**)pDummy)[2*bufSize];
*/
	m_buffer_.SetMinAlloc(
		(4+4+4+4+4+1)*sizeof(Ipp32f)*bufSize+
		3*sizeof(Ipp32f*)*bufSize);
	Ipp32f* pTmp1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pTmp2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert3 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pD = m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs1 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs2 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs3 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);

//	Ipp32f pTmp1[4*bufSize], pTmp2[4*bufSize];
//	Ipp32f *pPtrs1[bufSize], *pPtrs2[bufSize],
//		*pPtrs3[bufSize];
//	Ipp32f pVert1[bufSize*4], pVert2[bufSize*4],
//		pVert3[bufSize*4];
//	Ipp32f pD[bufSize];

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize)
			loopSize = m_numTriangles - i;

		IppiSize size = {loopSize, 1};
		if (sizeof(Ipp32f*) == 4)
		{
			Ipp32s* pIndices[3] = { (Ipp32s*)pPtrs1,
				(Ipp32s*)pPtrs2, (Ipp32s*)pPtrs3 };
			ippiCopy_32f_C3P3R(
				(const Ipp32f*)&m_trianglesData[i],
				bufSize*3*4, (float*const*)pIndices,
				bufSize*4, size);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[0], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[0], bufSize,0);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[1], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[1], bufSize,0);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[2], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[2], bufSize,0);
		}
		else if (sizeof(Ipp32f*) == 8)
		{
			//
		}

		ippsSet_32f(1.0, pVert1, loopSize*4);
		ippsSet_32f(1.0, pVert2, loopSize*4);
		ippsSet_32f(1.0, pVert3, loopSize*4);

		ippmCopy_va_32f_LS(
			(const float**)pPtrs1, 0, sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f), sizeof(Ipp32f), 
			4, loopSize);
//			3, loopSize);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs2, 0, sizeof(Ipp32f),
			pVert2, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			4, loopSize);
//			3, loopSize);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs3, 0, sizeof(Ipp32f),
			pVert3, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			4, loopSize);
//			3, loopSize);

		IppStatus st = ippmSub_vava_32f_3x1(
			pVert2, 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pTmp1, 4*sizeof(Ipp32f), loopSize);
		st = ippmSub_vava_32f_3x1(
			pVert3, 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pTmp2, 4*sizeof(Ipp32f), loopSize);

		st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 4*sizeof(Ipp32f),
			pTmp2, 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			loopSize);
		st = ippmDotProduct_vava_32f_3x1(
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pD, loopSize);
		ippsMulC_32f_I(-1.0, pD, loopSize);
		ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),
			(float*)&m_planeData[i].D, loopSize*4*sizeof(Ipp32f),
			size);

	}
	m_buffer_.ReleaseAll();
}
else if (algorithm == 4)
{
	int i;
	m_planeData = new PlaneData[m_numTriangles];
	const int bufSize = 256;
	int loopSize = bufSize;

	m_buffer_.SetMinAlloc(
		(4+4+4+4+4+1)*sizeof(Ipp32f)*bufSize+
		3*sizeof(Ipp32f*)*bufSize);
	Ipp32f* pTmp1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pTmp2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert3 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pD = m_buffer_.Alloc_32f(bufSize);
/*	Ipp32f** pPtrs1 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs2 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs3 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);
*/
	Ipp32f **pPtrs1, **pPtrs2, **pPtrs3;
	if (!m_pTmp)
	{
		m_pTmp = ippsMalloc_32s(m_numTriangles*3);
		pPtrs1 = (Ipp32f**) m_pTmp;
		pPtrs2 = &((Ipp32f**)m_pTmp)[m_numTriangles];
		pPtrs3 = &((Ipp32f**)m_pTmp)[m_numTriangles*2];
		for (i=0; i<m_numTriangles; i++)
		{
			pPtrs1[i] =
				&m_vertexData[m_trianglesData[i].firstIndex].x;
			pPtrs2[i] =
				&m_vertexData[m_trianglesData[i].secondIndex].x;
			pPtrs3[i] =
				&m_vertexData[m_trianglesData[i].thirdIndex].x;
		}
	}
	else
	{
		pPtrs1 = (Ipp32f**) m_pTmp;
		pPtrs2 = &((Ipp32f**)m_pTmp)[m_numTriangles];
		pPtrs3 = &((Ipp32f**)m_pTmp)[m_numTriangles*2];
	}

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize)
			loopSize = m_numTriangles - i;

		IppiSize size = {loopSize, 1};

		ippmCopy_va_32f_LS(
			(const float**)pPtrs1, 0, sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f), sizeof(Ipp32f), 
			3, loopSize);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs2, 0, sizeof(Ipp32f),
			pVert2, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			3, loopSize);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs3, 0, sizeof(Ipp32f),
			pVert3, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			3, loopSize);

		IppStatus st = ippmSub_vava_32f_3x1(
			pVert2, 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pTmp1, 4*sizeof(Ipp32f), loopSize);
		st = ippmSub_vava_32f_3x1(
			pVert3, 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pTmp2, 4*sizeof(Ipp32f), loopSize);

		st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 4*sizeof(Ipp32f),
			pTmp2, 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			loopSize);
		st = ippmDotProduct_vava_32f_3x1(
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pD, loopSize);
		ippsMulC_32f_I(-1.0, pD, loopSize);
		ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),
			(float*)&m_planeData[i].D, loopSize*4*sizeof(Ipp32f),
			size);

	}
	m_buffer_.ReleaseAll();
}
else if (algorithm == 40)
{
	int i,j;
//	IppStatus st;
	m_planeData = new PlaneData[m_numTriangles];
//	m_side1Plane = new PlaneData[m_numTriangles];
//	m_side2Plane = new PlaneData[m_numTriangles];
//	m_side3Plane = new PlaneData[m_numTriangles];
//	const int bufSize = 1;
//	const int bufSize = 3072;
	const int bufSize = 256;
	int loopSize = bufSize;
/*	BufAlloc_((4+4+4+4+4+1)*sizeof(Ipp32f)*bufSize+
			  3*sizeof(Ipp32f*)*bufSize);
	Ipp32f* pTmp1 = (Ipp32f*)m_pBuffer_;
	Ipp32f* pTmp2 = &((Ipp32f*)m_pBuffer_)[4*bufSize];
	Ipp32f* pVert1 = &((Ipp32f*)m_pBuffer_)[8*bufSize];
	Ipp32f* pVert2 = &((Ipp32f*)m_pBuffer_)[12*bufSize];
	Ipp32f* pVert3 = &((Ipp32f*)m_pBuffer_)[16*bufSize];
	Ipp32f* pD = &((Ipp32f*)m_pBuffer_)[20*bufSize];
	Ipp32f* pDummy = &((Ipp32f*)m_pBuffer_)[21*bufSize];
	Ipp32f** pPtrs1 = (Ipp32f**)pDummy;
	Ipp32f** pPtrs2 = &((Ipp32f**)pDummy)[bufSize];
	Ipp32f** pPtrs3 = &((Ipp32f**)pDummy)[2*bufSize];
*/
	m_buffer_.SetMinAlloc(
		(4+4+4+4+4+1)*sizeof(Ipp32f)*bufSize+
		3*sizeof(Ipp32f*)*bufSize);
	Ipp32f* pTmp1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pTmp2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert3 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pD = m_buffer_.Alloc_32f(bufSize);
	Ipp32f* pFirst = m_buffer_.Alloc_32f(bufSize);
	Ipp32f* pSecond = m_buffer_.Alloc_32f(bufSize);
	Ipp32f* pThird = m_buffer_.Alloc_32f(bufSize);

	Ipp32f *pTrianglesSOA[3] =
	{ (Ipp32f*)pFirst, (Ipp32f*)pSecond, (Ipp32f*)pThird };

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize)
			loopSize = m_numTriangles - i;

		IppiSize size = {loopSize, 1};
//		ippiCopy_32f_C3P3R(
//			(const Ipp32f*)&m_trianglesData[i],
//			loopSize*sizeof(TrianglesData)*sizeof(Ipp32f),
//			pTrianglesSOA, loopSize*sizeof(Ipp32f), size);
		for (j=0; j<loopSize; j++)
		{
			int p1 = m_trianglesData[i+j].firstIndex;
			int p2 = m_trianglesData[i+j].secondIndex;
			int p3 = m_trianglesData[i+j].thirdIndex;
//			int p1 = pFirst[j];
//			int p2 = pSecond[j];
//			int p3 = pThird[j];
			pVert1[j*4] = m_vertexData[p1].x;
			pVert1[j*4+1] = m_vertexData[p1].y;
			pVert1[j*4+2] = m_vertexData[p1].z;
//			pVert1[j*4+3] = 1.0f;
			pVert2[j*4] = m_vertexData[p2].x;
			pVert2[j*4+1] = m_vertexData[p2].y;
			pVert2[j*4+2] = m_vertexData[p2].z;
//			pVert2[j*4+3] = 1.0f;
			pVert3[j*4] = m_vertexData[p3].x;
			pVert3[j*4+1] = m_vertexData[p3].y;
			pVert3[j*4+2] = m_vertexData[p3].z;
//			pVert3[j*4+3] = 1.0f;
		}
/*		if (sizeof(Ipp32f*) == 4)
		{
			Ipp32s* pIndices[3] = { (Ipp32s*)pPtrs1,
				(Ipp32s*)pPtrs2, (Ipp32s*)pPtrs3 };
			ippiCopy_32f_C3P3R(
				(const Ipp32f*)&m_trianglesData[i],
				bufSize*3*4, (float*const*)pIndices,
				bufSize*4, size);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[0], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[0], bufSize,0);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[1], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[1], bufSize,0);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[2], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[2], bufSize,0);
		}
		else if (sizeof(Ipp32f*) == 8)
		{
			//
		}
		ippsSet_32f(1.0, pVert1, loopSize*4);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs1, 0, sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f), sizeof(Ipp32f), 
			3, loopSize);
		ippsSet_32f(1.0, pVert2, loopSize*4);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs2, 0, sizeof(Ipp32f),
			pVert2, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			3, loopSize);
		ippsSet_32f(1.0, pVert3, loopSize*4);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs3, 0, sizeof(Ipp32f),
			pVert3, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			3, loopSize);
*/
		IppStatus st;
		ippsSub_32f(pVert2, pVert1, pTmp1, loopSize);
//		IppStatus st = ippmSub_vava_32f_3x1(
//			pVert2, 4*sizeof(Ipp32f),
//			pVert1, 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f), loopSize);
		ippsSub_32f(pVert3, pVert1, pTmp1, loopSize);
//		st = ippmSub_vava_32f_3x1(
//			pVert3, 4*sizeof(Ipp32f),
//			pVert1, 4*sizeof(Ipp32f),
//			pTmp2, 4*sizeof(Ipp32f), loopSize);

		st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 4*sizeof(Ipp32f),
			pTmp2, 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			loopSize);
		st = ippmDotProduct_vava_32f_3x1(
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pD, loopSize);
		ippsMulC_32f_I(-1.0, pD, loopSize);
		ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),
			(float*)&m_planeData[i].D, loopSize*4*sizeof(Ipp32f),
			size);

	}
	m_buffer_.ReleaseAll();
}
else
{
	int i,j;
//	IppStatus st;
	m_planeData = new PlaneData[m_numTriangles];
	m_side1Plane = new PlaneData[m_numTriangles];
	m_side2Plane = new PlaneData[m_numTriangles];
	m_side3Plane = new PlaneData[m_numTriangles];
//	const int bufSize = 1;
	const int bufSize = 3072;
	int loopSize = bufSize;
/*	BufAlloc_((4+4+4+4+4+1)*sizeof(Ipp32f)*bufSize+
			  3*sizeof(Ipp32f*)*bufSize);
	Ipp32f* pTmp1 = (Ipp32f*)m_pBuffer_;
	Ipp32f* pTmp2 = &((Ipp32f*)m_pBuffer_)[4*bufSize];
	Ipp32f* pVert1 = &((Ipp32f*)m_pBuffer_)[8*bufSize];
	Ipp32f* pVert2 = &((Ipp32f*)m_pBuffer_)[12*bufSize];
	Ipp32f* pVert3 = &((Ipp32f*)m_pBuffer_)[16*bufSize];
	Ipp32f* pD = &((Ipp32f*)m_pBuffer_)[20*bufSize];
	Ipp32f* pDummy = &((Ipp32f*)m_pBuffer_)[21*bufSize];
	Ipp32f** pPtrs1 = (Ipp32f**)pDummy;
	Ipp32f** pPtrs2 = &((Ipp32f**)pDummy)[bufSize];
	Ipp32f** pPtrs3 = &((Ipp32f**)pDummy)[2*bufSize];
*/
	m_buffer_.SetMinAlloc(
		(4+4+4+4+4+1)*sizeof(Ipp32f)*bufSize+
		3*sizeof(Ipp32f*)*bufSize);
	Ipp32f* pTmp1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pTmp2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert1 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert2 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pVert3 = m_buffer_.Alloc_32f(4*bufSize);
	Ipp32f* pD = m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs1 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs2 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);
	Ipp32f** pPtrs3 =
		(Ipp32f**)m_buffer_.Alloc_32f(bufSize);

//	Ipp32f pTmp1[4*bufSize], pTmp2[4*bufSize];
//	Ipp32f *pPtrs1[bufSize], *pPtrs2[bufSize],
//		*pPtrs3[bufSize];
//	Ipp32f pVert1[bufSize*4], pVert2[bufSize*4],
//		pVert3[bufSize*4];
//	Ipp32f pD[bufSize];

	for (i=0; i<m_numTriangles; i+=bufSize)
	{
		if (i>m_numTriangles-bufSize)
			loopSize = m_numTriangles - i;

		IppiSize size = {loopSize, 1};
		if (sizeof(Ipp32f*) == 4)
		{
			Ipp32s* pIndices[3] = { (Ipp32s*)pPtrs1,
				(Ipp32s*)pPtrs2, (Ipp32s*)pPtrs3 };
			ippiCopy_32f_C3P3R(
				(const Ipp32f*)&m_trianglesData[i],
				bufSize*3*4, (float*const*)pIndices,
				bufSize*4, size);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[0], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[0], bufSize,0);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[1], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[1], bufSize,0);
			ippsMulC_32s_ISfs(sizeof(VertexData),
				pIndices[2], bufSize,0);
			ippsAddC_32s_ISfs((Ipp32s)m_vertexData,
				pIndices[2], bufSize,0);
		}
		else if (sizeof(Ipp32f*) == 8)
		{
			//
		}
		ippsSet_32f(1.0, pVert1, loopSize*4);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs1, 0, sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f), sizeof(Ipp32f), 
			3, loopSize);
		ippsSet_32f(1.0, pVert2, loopSize*4);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs2, 0, sizeof(Ipp32f),
			pVert2, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			3, loopSize);
		ippsSet_32f(1.0, pVert3, loopSize*4);
		ippmCopy_va_32f_LS(
			(const float**)pPtrs3, 0, sizeof(Ipp32f),
			pVert3, 4*sizeof(Ipp32f), sizeof(Ipp32f),
			3, loopSize);

		IppStatus st = ippmSub_vava_32f_3x1(
			pVert2, 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pTmp1, 4*sizeof(Ipp32f), loopSize);
		st = ippmSub_vava_32f_3x1(
			pVert3, 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pTmp2, 4*sizeof(Ipp32f), loopSize);

		st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 4*sizeof(Ipp32f),
			pTmp2, 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			loopSize);
		st = ippmDotProduct_vava_32f_3x1(
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			pVert1, 4*sizeof(Ipp32f),
			pD, loopSize);
		ippsMulC_32f_I(-1.0, pD, loopSize);
		ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),
			(float*)&m_planeData[i].D, loopSize*4*sizeof(Ipp32f),
			size);

//-------------------------------
//		Vab = Va - Vb
//			tmp2 = cross(Vab,Nplane) = cross(pTmp1, (float*)&m_planeData[i])
		st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
//			(float*)pTmp2, 4*sizeof(Ipp32f),
			(float*)&m_side3Plane[i], 4*sizeof(Ipp32f),
			loopSize);
//			pD = dot(Vab,Nplane);
//			pD = dot(pTmp1, (float*)&m_planeData[i])
//		st = ippmDotProduct_vava_32f_3x1(
//			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f),
//			pD, loopSize);

//			Nab = t2 * Vab;
//			m_side3Plane = pTmp1*pD
		// No function ippmMul_vav_32f_3x1, so...
/*		for (j=0; j<loopSize; j++)
		{
			m_side3Plane[i+j].A = pTmp1[j*4]*pD[j];
			m_side3Plane[i+j].B = pTmp1[j*4+1]*pD[j];
			m_side3Plane[i+j].C = pTmp1[j*4+2]*pD[j];
			m_side3Plane[i+j].D = 0.0;
		}
*/		
//			Nab += pTmp3;
//			m_side3Plane += pTmp
//		ippsAdd_32f_I(pTmp2, (Ipp32f*)&m_side3Plane[i], loopSize*4);
//		ippsCopy_32f_I(pTmp2, (Ipp32f*)&m_side3Plane[i], loopSize*4);
		
//			Dab = -dot(Vab,Pa);
//			pD = dot(pTmp1, pVert1);
		st = ippmDotProduct_vava_32f_3x1(
			(float*)pVert1, 4*sizeof(Ipp32f),
			(float*)&m_side3Plane[i], 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f),
			pD, loopSize);
		ippsMulC_32f_I(-1.0f, pD, loopSize);
		st = ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),
			(float*)&m_side3Plane[i].D, loopSize*4*sizeof(Ipp32f),
			size);

//			if (dot(Nab,Pc) < 0) Nab = -Nab;
//			pD = dot(m_side3Plane, pVert3);
		st = ippmDotProduct_vava_32f_4x1(
			(float*)pVert3, 4*sizeof(Ipp32f),
			(float*)&m_side3Plane[i], 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f),
			pD, loopSize);
		for (j=0; j<loopSize; j++)
		{
			if (pD[j]<0.0f)
			{
				m_side3Plane[i+j].A = -m_side3Plane[i+j].A;
				m_side3Plane[i+j].B = -m_side3Plane[i+j].B; 
				m_side3Plane[i+j].C = -m_side3Plane[i+j].C; 
				m_side3Plane[i+j].D = -m_side3Plane[i+j].D; 
			}
		}
			
//--------------------

//			Vab = Pa - Pb;
/*		st = ippmSub_vava_32f_3x1(
			pVert1, 4*sizeof(Ipp32f),
			pVert3, 4*sizeof(Ipp32f),
			pTmp1, 4*sizeof(Ipp32f), loopSize);
*/
//			tmp2 = cross(Vab,Nplane) = cross(pTmp1, (float*)&m_planeData[i])
		st = ippmCrossProduct_vava_32f_3x1(
			pTmp2, 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			(float*)&m_side2Plane[i], 4*sizeof(Ipp32f),
			loopSize);
//			pD = dot(Vab,Nplane);
//			pD = dot(pTmp1, (float*)&m_planeData[i])
/*		st = ippmDotProduct_vava_32f_3x1(
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			pTmp1, 4*sizeof(Ipp32f),
			pD, loopSize);
*/
//			Nab = t2 * Vab;
//			m_side2Plane = pTmp1*pD
		// No function ippmMul_vav_32f_3x1, so...
/*		for (j=0; j<loopSize; j++)
		{
			m_side2Plane[i+j].A = pTmp1[j*4]*pD[j];
			m_side2Plane[i+j].B = pTmp1[j*4+1]*pD[j];
			m_side2Plane[i+j].C = pTmp1[j*4+2]*pD[j];
			m_side2Plane[i+j].D = 0;
		}
*/		
//			Nab += pTmp3;
//			m_side2Plane += pTmp
//		ippsAdd_32f_I(pTmp2, (Ipp32f*)&m_side2Plane[i], loopSize*4);
		
//			Dab = -dot(Vab,Pa);
		// should this be Dab = -dot(Nab,Pa)?
//			pD = dot(pTmp1, pVert1);
		st = ippmDotProduct_vava_32f_3x1(
			(float*)pVert3, 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f),
			(float*)&m_side2Plane[i], 4*sizeof(Ipp32f),
			pD, loopSize);
		ippsMulC_32f_I(-1.0f, pD, loopSize);
		st = ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),
			(float*)&m_side2Plane[i].D, loopSize*4*sizeof(Ipp32f),
			size);

//			if (dot(Nab,Pc) < 0) Nab = -Nab;
//			pD = dot(m_side2Plane, pVert3);
		st = ippmDotProduct_vava_32f_4x1(
			(float*)pVert2, 4*sizeof(Ipp32f),
			(float*)&m_side2Plane[i], 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f),
			pD, loopSize);
		for (j=0; j<loopSize; j++)
		{
			if (pD[j]<0.0f)
			{
				m_side2Plane[i+j].A = -m_side2Plane[i+j].A;
				m_side2Plane[i+j].B = -m_side2Plane[i+j].B; 
				m_side2Plane[i+j].C = -m_side2Plane[i+j].C; 
				m_side2Plane[i+j].D = -m_side2Plane[i+j].D; 
			}
		}
//-----------------------------
		//			Vab = Pa - Pb;
		st = ippmSub_vava_32f_3x1(
			pVert3, 4*sizeof(Ipp32f),
			pVert2, 4*sizeof(Ipp32f),
			pTmp1, 4*sizeof(Ipp32f), loopSize);
//			tmp2 = cross(Vab,Nplane) = cross(pTmp1, (float*)&m_planeData[i])
		st = ippmCrossProduct_vava_32f_3x1(
			pTmp1, 4*sizeof(Ipp32f),
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			(Ipp32f*)&m_side1Plane[i], 4*sizeof(Ipp32f),
			loopSize);
//			pD = dot(Vab,Nplane);
//			pD = dot(pTmp1, (float*)&m_planeData[i])
/*		st = ippmDotProduct_vava_32f_3x1(
			(float*)&m_planeData[i], 4*sizeof(Ipp32f),
			pTmp1, 4*sizeof(Ipp32f),
			pD, loopSize);
*/
//			Nab = t2 * Vab;
//			m_side1Plane = pTmp1*pD
		// No function ippmMul_vav_32f_3x1, so...
/*		for (j=0; j<loopSize; j++)
		{
			m_side1Plane[i+j].A = pTmp1[j*4]*pD[j];
			m_side1Plane[i+j].B = pTmp1[j*4+1]*pD[j];
			m_side1Plane[i+j].C = pTmp1[j*4+2]*pD[j];
			m_side1Plane[i+j].D = 0;
		}
*/		
//			Nab += pTmp3;
//			m_side1Plane += pTmp
//		ippsAdd_32f_I(pTmp2, (Ipp32f*)&m_side1Plane[i], loopSize*4);
		
//			Dab = -dot(Vab,Pa);
//			pD = dot(pTmp1, pVert1);
		st = ippmDotProduct_vava_32f_3x1(
			(float*)pVert2, 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f),
			(float*)&m_side1Plane[i], 4*sizeof(Ipp32f),
			pD, loopSize);
		ippsMulC_32f_I(-1.0f, pD, loopSize);
		st = ippiCopy_32f_C1C4R(pD, loopSize*sizeof(Ipp32f),
			(float*)&m_side1Plane[i].D, loopSize*4*sizeof(Ipp32f),
			size);

//			if (dot(Nab,Pc) < 0) Nab = -Nab;
//			pD = dot(m_side1Plane, pVert3);
		st = ippmDotProduct_vava_32f_4x1(
			(float*)pVert1, 4*sizeof(Ipp32f),
//			pTmp1, 4*sizeof(Ipp32f),
			(float*)&m_side1Plane[i], 4*sizeof(Ipp32f),
			pD, loopSize);
		for (j=0; j<loopSize; j++)
		{
			if (pD[j]<0.0f)
			{
				m_side1Plane[i+j].A = -m_side1Plane[i+j].A;
				m_side1Plane[i+j].B = -m_side1Plane[i+j].B; 
				m_side1Plane[i+j].C = -m_side1Plane[i+j].C; 
				m_side1Plane[i+j].D = -m_side1Plane[i+j].D; 
			}
		}
	}
	m_buffer_.ReleaseAll();
}
}

/*
void Object3DS::BufAlloc_(int size)
{
	if (size<=m_bufSize_) return;

	if (m_pBuffer_) m_pBuffer_ = (char*)realloc(m_pBuffer_, size);
	else m_pBuffer_ = (char*)malloc(size);

	if (m_pBuffer_) m_bufSize_ = size;
	else size = 0;
}
*/

void Object3DS::IntersectObject(Object3DS& obj,
	unsigned char* pMask, unsigned char* pMaskOther)
{
	int i=0;
	int p1, p2, p3;
	int len = m_numTriangles,
		otherLen = obj.m_numTriangles;
	
	Ipp8u* pMaskTmp = ippsMalloc_8u(IPP_MAX(len, otherLen));
	Ipp8u max;
	i=0;
//	for (i=0; i<10; i++)
	for (i=0; i<len; i++)
	{
		IppiSize size = {otherLen, 1};
		p1 = m_trianglesData[i].firstIndex;
		p2 = m_trianglesData[i].secondIndex;
		p3 = m_trianglesData[i].thirdIndex;

		obj.IntersectLineSeg(
			(float*)&m_vertexData[p1],
			(float*)&m_vertexData[p2], pMaskTmp);
		ippiMax_8u_C1R(pMaskTmp, otherLen, size, &max);
		pMask[i] |= max;
		ippsOr_8u_I(pMaskTmp, pMaskOther, otherLen);

		obj.IntersectLineSeg(
			(float*)&m_vertexData[p2],
			(float*)&m_vertexData[p3], pMaskTmp);
		ippiMax_8u_C1R(pMaskTmp, otherLen, size, &max);
		pMask[i] |= max;
		ippsOr_8u_I(pMaskTmp, pMaskOther, otherLen);

		obj.IntersectLineSeg(
			(float*)&m_vertexData[p3],
			(float*)&m_vertexData[p1], pMaskTmp);
		ippiMax_8u_C1R(pMaskTmp, otherLen, size, &max);
		pMask[i] |= max;
		ippsOr_8u_I(pMaskTmp, pMaskOther, otherLen);
	}

	i=0;
	ippsZero_8u(pMaskTmp,IPP_MAX(len, otherLen));
	for (i=0; i<otherLen; i++)
	{
		IppiSize size = {len, 1};
		p1 = obj.m_trianglesData[i].firstIndex;
		p2 = obj.m_trianglesData[i].secondIndex;
		p3 = obj.m_trianglesData[i].thirdIndex;

		IntersectLineSeg(
			(float*)&obj.m_vertexData[p1],
			(float*)&obj.m_vertexData[p2], pMaskTmp);
		ippiMax_8u_C1R(pMaskTmp, len, size, &max);
		pMaskOther[i] |= max;
		ippsOr_8u_I(pMaskTmp, pMask, len);

		IntersectLineSeg(
			(float*)&obj.m_vertexData[p2],
			(float*)&obj.m_vertexData[p3], pMaskTmp);
		ippiMax_8u_C1R(pMaskTmp, len, size, &max);
		pMaskOther[i] |= max;
		ippsOr_8u_I(pMaskTmp, pMask, len);

//		IntersectLineSeg(
//			(float*)&obj.m_vertexData[p3],
//			(float*)&obj.m_vertexData[p1], pMaskTmp);
//		ippiMax_8u_C1R(pMaskTmp, len, size, &max);
//		pMaskOther[i] |= max;
//		ippsOr_8u_I(pMaskTmp, pMask, len);
	}
	ippsFree(pMaskTmp);
}

void Object3DS::IntersectLineSeg(float* p1,
	float* p2, unsigned char* pMask)
{
	float pStart[4],pDir[4];
	pStart[0] = p1[0];
	pStart[1] = p1[1];
	pStart[2] = p1[2];
	pStart[3] = 1.0;
	pDir[0] = p2[0]-p1[0];
	pDir[1] = p2[1]-p1[1];
	pDir[2] = p2[2]-p1[2];
	pDir[3] = 0.0;

	IntersectRay(pStart, pDir, pMask, 1, 1);
}

void Object3DS::IntersectRay(float* pStart, float* pDir,
	unsigned char* pMask, int algorithm, int lineSeg)
//	int cumulative)
{
	if (algorithm == 0)
	{
		int i;
		float dotProdNum, dotProdDenom, t;
		float pPoint[4] = {0.0f, 0.0f, 0.0f, 1.0f};
		for (i=0; i<m_numTriangles; i++)
		{
			ippmDotProduct_vv_32f_4x1(
				(const float*)&m_planeData[i], pStart,
				&dotProdNum);
			ippmDotProduct_vv_32f_4x1(
				(const float*)&m_planeData[i], pDir,
				&dotProdDenom);
			t = dotProdNum / dotProdDenom;
	//		ippsDiv_32f(&dotProdNum, &dotProdDenom, &t, 1);

			pPoint[0] = pStart[0]-t*pDir[0];
			pPoint[1] = pStart[1]-t*pDir[1];
			pPoint[2] = pStart[2]-t*pDir[2];

			IppStatus st = ippmDotProduct_vv_32f_4x1(
				(float*)&m_side1Plane[i],// 4*sizeof(Ipp32f),
				pPoint,// 4*sizeof(Ipp32f),
				&t);
			pMask[i] = (t>0.0f);
			st = ippmDotProduct_vv_32f_4x1(
				(float*)&m_side2Plane[i],// 4*sizeof(Ipp32f),
				pPoint,// 4*sizeof(Ipp32f),
				&t);
			pMask[i] &= (t>0.0f);
			st = ippmDotProduct_vv_32f_4x1(
				(float*)&m_side3Plane[i],// 4*sizeof(Ipp32f),
				pPoint,//4*sizeof(Ipp32f),
				&t);
			pMask[i] &= (t>0.0f);
		}
	}
	else
	{
		IppStatus st;
		const int bufSize = 256;
//		const int bufSize = 3072;
		int loopSize = bufSize;
//		BufAlloc_(sizeof(Ipp32f)*bufSize*(4+1+1+1));
		m_buffer_.SetMinAlloc( bufSize +
			sizeof(Ipp32f)*bufSize*(4+1+1+1));
		int i,j;
/*		Ipp32f* pTmp1 = (float*)m_pBuffer_;
		Ipp32f* pTmp2 =
			&((float*)m_pBuffer_)[bufSize];
		Ipp32f* pT =
			&((float*)m_pBuffer_)[bufSize*2];
		Ipp32f* pPoint =
			&((float*)m_pBuffer_)[bufSize*3];
		Ipp32f* pDummy =
			&((float*)m_pBuffer_)[bufSize*(3+4)];
		Ipp8u* pMaskTmp = (Ipp8u*) pDummy;
*/
		Ipp32f* pTmp1 =
			m_buffer_.Alloc_32f(bufSize);
		Ipp32f* pTmp2 =
			m_buffer_.Alloc_32f(bufSize);
		Ipp32f* pT =
			m_buffer_.Alloc_32f(bufSize);
		Ipp32f* pPoint =
			m_buffer_.Alloc_32f(bufSize*4);
		Ipp8u* pMaskTmp =
			m_buffer_.Alloc_8u(bufSize);

//		float dotProdNum, dotProdDenom, t;
//		float pPoint[4] = {0.0f, 0.0f, 0.0f, 1.0f};
		for (i=0; i<m_numTriangles; i+=bufSize)
		{
			if (i>m_numTriangles-bufSize)
				loopSize = m_numTriangles - i;
			ippmDotProduct_vav_32f_4x1(
				(const float*)&m_planeData[i],
				4*sizeof(Ipp32f), pStart,
				pTmp1, loopSize);
			ippmDotProduct_vav_32f_4x1(
				(const float*)&m_planeData[i],
				4*sizeof(Ipp32f), pDir,
				pTmp2, loopSize);
			ippsDiv_32f(pTmp2, pTmp1, pT, loopSize);
//			t = dotProdNum / dotProdDenom;
	//		ippsDiv_32f(&dotProdNum, &dotProdDenom, &t, 1);

			for (j=0; j<loopSize; j++)
			{
				pPoint[j*4] = pStart[0]-pT[j]*pDir[0];
				pPoint[j*4+1] = pStart[1]-pT[j]*pDir[1];
				pPoint[j*4+2] = pStart[2]-pT[j]*pDir[2];
				pPoint[j*4+3] = 1.0;
			}

			IppiSize imSize = {loopSize, 1};

			st = ippiCompareC_32f_C1R(
				pTmp2, loopSize*sizeof(Ipp32f), 0.0, 
				&pMask[i], loopSize*sizeof(Ipp32f),
				imSize, ippCmpEq);
			ippsNot_8u_I(&pMask[i], loopSize);
			if (lineSeg)
			{
				st = ippiCompareC_32f_C1R(
					pT, loopSize*sizeof(Ipp32f), 0.0f, 
//					&pMask[i], loopSize*sizeof(Ipp8u),
					pMaskTmp, loopSize*sizeof(Ipp8u),
					imSize, ippCmpLessEq);
				ippsAnd_8u_I(pMaskTmp, &pMask[i], loopSize);
				
				st = ippiCompareC_32f_C1R(
					pT, loopSize*sizeof(Ipp32f), -1.0f, 
					pMaskTmp, loopSize*sizeof(Ipp8u),
					imSize, ippCmpGreaterEq);
				
				ippsAnd_8u_I(pMaskTmp, &pMask[i], loopSize);
			}

			st = ippmDotProduct_vava_32f_4x1(
				(float*)&m_side1Plane[i], 4*sizeof(Ipp32f),
				pPoint, 4*sizeof(Ipp32f),
				pT, loopSize);
			if (1)
//			if (lineSeg)
			{
				st = ippiCompareC_32f_C1R(
					pT, loopSize*sizeof(Ipp32f), 0.0, 
					pMaskTmp, loopSize*sizeof(Ipp32f),
					imSize, ippCmpGreater);
				ippsAnd_8u_I(pMaskTmp, &pMask[i], loopSize);
			}
			else
			{
				st = ippiCompareC_32f_C1R(
					pT, loopSize*sizeof(Ipp32f), 0.0, 
					&pMask[i], loopSize*sizeof(Ipp32f),
					imSize, ippCmpGreater);
			}

			st = ippmDotProduct_vava_32f_4x1(
				(float*)&m_side2Plane[i], 4*sizeof(Ipp32f),
				pPoint, 4*sizeof(Ipp32f),
				pT, loopSize);
			st = ippiCompareC_32f_C1R(
				pT, loopSize*sizeof(Ipp32f), 0.0, 
				pMaskTmp, loopSize*sizeof(Ipp32f),
				imSize, ippCmpGreater);
			ippsAnd_8u_I(pMaskTmp, &pMask[i], loopSize);

			st = ippmDotProduct_vava_32f_4x1(
				(float*)&m_side3Plane[i], 4*sizeof(Ipp32f),
				pPoint, 4*sizeof(Ipp32f),
				pT, loopSize);
			st = ippiCompareC_32f_C1R(
				pT, loopSize*sizeof(Ipp32f), 0.0, 
				pMaskTmp, loopSize*sizeof(Ipp8u),
				imSize, ippCmpGreater);
			ippsAnd_8u_I(pMaskTmp, &pMask[i], loopSize);

		}
		m_buffer_.ReleaseAll();
	}
}

void Object3DS::CalcCenterOfMass(float* pPoint,
	float* pMass, int algorithm)
{
	int i;
	IppStatus st;

	Ipp32f pCenterBody[4]={0.0f, 0.0f, 0.0f, 1.0f};
	Ipp32f massBody = 0.0f;
	int bufSize = 256;
	int loopSize = bufSize;
	if (algorithm == 0)
	{
		for (i = 0; i<m_numTriangles; i++)
		{
			// find the center of mass of the current triangle
			//P0, P1 and P2 are vertices of the
			// current triangle
			Ipp32f *pP0	= (Ipp32f*)&m_vertexData[
						  m_trianglesData[i].firstIndex];
			Ipp32f *pP1	= (Ipp32f*)&m_vertexData[
						  m_trianglesData[i].secondIndex];
			Ipp32f *pP2	= (Ipp32f*)&m_vertexData[
						  m_trianglesData[i].thirdIndex];
			//S10, S20 are vectors with the starts at P0 and the ends at P1 and P2 respectively
			//tmp is temporary variable to contain the cross product of S10 and S20, 
			//while triangle's area is computing
			Ipp32f S10[3], S20[3], tmp[3], CenterTriangle[3], MassTriangle=0;

			//compute S10 and S20 coordinates: S10 = P1 - P0, S20 = P2 - P0
			//*** Change these so that they write intopS10 and pS20 (hint: &pS10[i] and &pS20[i])
//			st = ippmSub_vv_32f_3x1(pP1, pP0, S10);
			S10[0] = pP1[0] - pP0[0];
			S10[1] = pP1[1] - pP0[1];
			S10[2] = pP1[2] - pP0[2];
			S20[0] = pP2[0] - pP0[0];
			S20[1] = pP2[1] - pP0[1];
			S20[2] = pP2[2] - pP0[2];
//			st = ippmSub_vv_32f_3x1(pP2, pP0, S20);

			//*** End the loop here:
			// }

			//*** Convert these statements to their array-of-vectors equivalents, using pS10, pS20, pTmp, and pMassTriangle
			//find the mass of the triangle: if density  = 1 than mass = area = |S10 x S20|/2
			st = ippmCrossProduct_vv_32f_3x1(S10, S20, tmp);
			st = ippmL2Norm_v_32f_3x1(tmp, &MassTriangle);

			MassTriangle /= 2.0f;

			//*** Convert these statements to their array-of-vectors equivalents
			//find the center of triangle's mass: CenterTriangle = P0 + (S10 + S20)/3
			st = ippmAdd_vv_32f_3x1(S10, S20,
					CenterTriangle);
			st = ippmMul_vc_32f_3x1(CenterTriangle,
					1.0f/3.0f, CenterTriangle);

			//*** Put this statement in a loop, and modify it to work with pCenterTriangle
			// for (unsigned long  i = 0; i <data1.stride*data1.numTriangles; i+=data1.stride){
//			st = ippmAdd_vv_32f_3x1(pP0, CenterTriangle,
//					CenterTriangle);
			CenterTriangle[0] += pP0[0];
			CenterTriangle[1] += pP0[1];
			CenterTriangle[2] += pP0[2];

			// }

			// *** Because pMassTriangle is an array of constants, not vectors, use:
			// ippsDivC_32f_I(2.0f, pMassTriangle, count);

			// *** Put these statements in a loop, and modify them to work with the pCenterTriangle array of vectors
			// and pMassTriangle 
			// for (i = 0; i <data1.stride*data1.numTriangles; i+=data1.stride)
			// {
			pCenterBody[0] +=
				CenterTriangle[0] * MassTriangle;
			pCenterBody[1] +=
				CenterTriangle[1] * MassTriangle;
			pCenterBody[2] +=
				CenterTriangle[2] * MassTriangle;
//			st = ippmMul_vc_32f_3x1(
//				CenterTriangle, MassTriangle, CenterTriangle);
//			st = ippmAdd_vv_32f_3x1(
//				CenterTriangle, pCenterBody, pCenterBody);
			massBody += MassTriangle;
			// }
		}
	}
	else if (algorithm == 1)
	{
		int size = m_numTriangles * 4;
		m_buffer_.SetMinAlloc(size*5*sizeof(Ipp32f));
		Ipp32f *pS10 = m_buffer_.Alloc_32f(size);
		ippsMalloc_32f(size);
		Ipp32f *pS20 = m_buffer_.Alloc_32f(size);
		Ipp32f *pCenterTriangle =
			m_buffer_.Alloc_32f(size);
		Ipp32f *pTmp = m_buffer_.Alloc_32f(size);
		Ipp32f *pMassTriangle =
			m_buffer_.Alloc_32f(size);

	//	Ipp32f S10[3], S20[3], Tmp[3], vCenterTriangle[3], cMassTriangle=0;
		int i;

		for (i = 0; i<m_numTriangles; i++)
		{
			//find the center of mass of the current triangle
			//P0, P1 and P2 are vertices of the current triangle
			Ipp32f *pP0	= (Ipp32f*)&m_vertexData[
						  m_trianglesData[i].firstIndex];
			Ipp32f *pP1	= (Ipp32f*)&m_vertexData[
						  m_trianglesData[i].secondIndex];
			Ipp32f *pP2	= (Ipp32f*)&m_vertexData[
						  m_trianglesData[i].thirdIndex];
			//S10, S20 are vectors with the starts at P0 and the ends at P1 and P2 respectively
			//tmp is temporary variable to contain the cross product of S10 and S20, 
			//while triangle's area is computing

			//compute S10 and S20 coordinates: S10 = P1 - P0, S20 = P2 - P0
			//*** Change these so that they write into pS10 and pS20 (hint: &pS10[i] and &pS20[i])
			pS10[i*4] = pP1[0] - pP0[0];
			pS10[i*4+1] = pP1[1] - pP0[1];
			pS10[i*4+2] = pP1[2] - pP0[2];
			pS20[i*4] = pP2[0] - pP0[0];
			pS20[i*4+1] = pP2[1] - pP0[1];
			pS20[i*4+2] = pP2[2] - pP0[2];
		}

		int count = m_numTriangles;
		int stride = sizeof(Ipp32f)*4;
		//find the center of triangle's mass:
		// CenterTriangle = P0 + (S10 + S20)/3
		st = ippmAdd_vava_32f_3x1(
			pS10, stride, pS20, stride,
			pCenterTriangle, stride, count);
		st = ippmMul_vac_32f_3x1(
			pCenterTriangle, stride, 1.0f/3.0f,
			pCenterTriangle, stride, count);

		for (i = 0; i<m_numTriangles; i++)
		{
			Ipp32f *pP0	= (Ipp32f*)&m_vertexData[
						  m_trianglesData[i].firstIndex];
			pCenterTriangle[i*4] += pP0[0];
			pCenterTriangle[i*4+1] += pP0[1];
			pCenterTriangle[i*4+2] += pP0[2];
		}

		//find the mass of the triangle
		// density  = 1 so mass = area = |S10 x S20|/2
		st = ippmCrossProduct_vava_32f_3x1(
			pS10, stride, pS20, stride,
			pTmp, stride, count);
		st = ippmL2Norm_va_32f_3x1(
			pTmp, stride, pMassTriangle, count);
		ippsDivC_32f_I(2.0f, pMassTriangle, count);

		//add the result to the center of mass:
		//pCenterBody += MassTriangle * CenterTriangles   
		for (i = 0; i < m_numTriangles; i++)
		{
			pCenterBody[0] +=
				pCenterTriangle[i*4] * pMassTriangle[i];
			pCenterBody[1] +=
				pCenterTriangle[i*4+1] * pMassTriangle[i];
			pCenterBody[2] +=
				pCenterTriangle[i*4+2] * pMassTriangle[i];

			//add triangle's mass to the body's mass
			massBody += pMassTriangle[i];
		}
		m_buffer_.ReleaseAll();
	}
else if (algorithm == 2)
{
	int size = bufSize*4;
	m_buffer_.SetMinAlloc(size*5*sizeof(Ipp32f));
	Ipp32f *pS10 = m_buffer_.Alloc_32f(size);
	ippsMalloc_32f(size);
	Ipp32f *pS20 = m_buffer_.Alloc_32f(size);
	Ipp32f *pCenterTriangle =
		m_buffer_.Alloc_32f(size);
	Ipp32f *pTmp = m_buffer_.Alloc_32f(size);
	Ipp32f *pMassTriangle =
		m_buffer_.Alloc_32f(size);

	int i,j;

	for (i = 0; i<m_numTriangles; i+=bufSize)
	{
		if (i+bufSize > m_numTriangles)
			loopSize = m_numTriangles-i;
		for (j=0; j<loopSize; j++)
		{
			//P0, P1 and P2 are vertices of the current
			// triangle
			Ipp32f *pP0	= (Ipp32f*)&m_vertexData[
				m_trianglesData[i+j].firstIndex];
			Ipp32f *pP1	= (Ipp32f*)&m_vertexData[
				m_trianglesData[i+j].secondIndex];
			Ipp32f *pP2	= (Ipp32f*)&m_vertexData[
				m_trianglesData[i+j].thirdIndex];

			//compute S10 and S20 coordinates:
			// S10 = P1 - P0, S20 = P2 - P0
			pS10[j*4] = pP1[0] - pP0[0];
			pS10[j*4+1] = pP1[1] - pP0[1];
			pS10[j*4+2] = pP1[2] - pP0[2];
			pS20[j*4] = pP2[0] - pP0[0];
			pS20[j*4+1] = pP2[1] - pP0[1];
			pS20[j*4+2] = pP2[2] - pP0[2];
		}

		int count = loopSize;
		int stride = sizeof(Ipp32f)*4;
		//find the center of triangle's mass:
		// CenterTriangle = P0 + (S10 + S20)/3
		st = ippmAdd_vava_32f_3x1(
			pS10, stride, pS20, stride,
			pCenterTriangle, stride, count);
		st = ippmMul_vac_32f_3x1(
			pCenterTriangle, stride, 1.0f/3.0f,
			pCenterTriangle, stride, count);

		for (j=0; j<loopSize; j++)
		{
			Ipp32f *pP0	= (Ipp32f*)&m_vertexData[
				  m_trianglesData[i+j].firstIndex];
			pCenterTriangle[j*4] += pP0[0];
			pCenterTriangle[j*4+1] += pP0[1];
			pCenterTriangle[j*4+2] += pP0[2];
		}

		//find the mass of the triangle
		// density  = 1 so mass = area = |S10 x S20|/2
		st = ippmCrossProduct_vava_32f_3x1(
			pS10, stride, pS20, stride,
			pTmp, stride, count);
		st = ippmL2Norm_va_32f_3x1(
			pTmp, stride, pMassTriangle, count);
		ippsDivC_32f_I(2.0f, pMassTriangle, count);

		//add the result to the center of mass:
		//pCenterBody += MassTriangle * CenterTriangles   
		for (j=0; j<loopSize; j++)
		{
			pCenterBody[0] +=
				pCenterTriangle[j*4] * pMassTriangle[j];
			pCenterBody[1] +=
				pCenterTriangle[j*4+1] * pMassTriangle[j];
			pCenterBody[2] +=
				pCenterTriangle[j*4+2] * pMassTriangle[j];

			//add triangle's mass to the body's mass
			massBody += pMassTriangle[j];
		}
	}
		m_buffer_.ReleaseAll();
}
	else
	{
//		int size = m_numTriangles * 4;
		int size = bufSize * 4;
		Ipp32f massTmp;
		m_buffer_.SetMinAlloc(size*5*sizeof(Ipp32f));
		Ipp32f *pS10 = m_buffer_.Alloc_32f(size);
		ippsMalloc_32f(size);
		Ipp32f *pS20 = m_buffer_.Alloc_32f(size);
//		Ipp32f *pP0 = m_buffer_.Alloc_32f(size);
		Ipp32f *pCenterTriangle =
			m_buffer_.Alloc_32f(size);
		Ipp32f *pTmp = m_buffer_.Alloc_32f(size);
		Ipp32f *pMassTriangle =
			m_buffer_.Alloc_32f(size);

	//	Ipp32f S10[3], S20[3], Tmp[3], vCenterTriangle[3], cMassTriangle=0;
		int i,j;

		for (i = 0; i<m_numTriangles; i+=bufSize)
		{
			if (i+bufSize > m_numTriangles)
				loopSize = m_numTriangles-i;
		for (j=0; j<loopSize; j++)
		{
			//find the center of mass of the current triangle
			//P0, P1 and P2 are vertices of the current triangle
			Ipp32f *pP0t = (Ipp32f*)&m_vertexData[
				m_trianglesData[i+j].firstIndex];
			Ipp32f *pP1t = (Ipp32f*)&m_vertexData[
				m_trianglesData[i+j].secondIndex];
			Ipp32f *pP2t = (Ipp32f*)&m_vertexData[
				m_trianglesData[i+j].thirdIndex];
			//S10, S20 are vectors with the starts at P0 and the ends at P1 and P2 respectively
			//tmp is temporary variable to contain the cross product of S10 and S20, 
			//while triangle's area is computing

			//compute S10 and S20 coordinates: S10 = P1 - P0, S20 = P2 - P0
			//*** Change these so that they write into pS10 and pS20 (hint: &pS10[i] and &pS20[i])
			pS10[j*4] = pP1t[0] - pP0t[0];
			pS10[j*4+1] = pP1t[1] - pP0t[1];
			pS10[j*4+2] = pP1t[2] - pP0t[2];
			pS20[j*4] = pP2t[0] - pP0t[0];
			pS20[j*4+1] = pP2t[1] - pP0t[1];
			pS20[j*4+2] = pP2t[2] - pP0t[2];
//			pP0[j*4] = pP0t[0];
//			pP0[j*4+1] = pP0t[1];
//			pP0[j*4+2] = pP0t[2];
		}

		int count = loopSize;
		int stride = sizeof(Ipp32f)*4;
		//find the center of triangle's mass:
		// CenterTriangle = P0 + (S10 + S20)/3
		st = ippmAdd_vava_32f_3x1(
			pS10, stride, pS20, stride,
			pCenterTriangle, stride, count);
		st = ippmMul_vac_32f_3x1(
			pCenterTriangle, stride, 1.0f/3.0f,
			pCenterTriangle, stride, count);
//		st = ippmAdd_vava_32f_3x1(
//			pP0, stride, pCenterTriangle, stride,
//			pCenterTriangle, stride, count);
		for (j=0; j<loopSize; j++)
		{
			Ipp32f *pP0	= (Ipp32f*)&m_vertexData[
				  m_trianglesData[i+j].firstIndex];
			pCenterTriangle[j*4] += pP0[0];
			pCenterTriangle[j*4+1] += pP0[1];
			pCenterTriangle[j*4+2] += pP0[2];
		}

		//find the mass of the triangle
		// density  = 1 so mass = area = |S10 x S20|/2
		st = ippmCrossProduct_vava_32f_3x1(
			pS10, stride, pS20, stride,
			pTmp, stride, count);
		st = ippmL2Norm_va_32f_3x1(
			pTmp, stride, pMassTriangle, count);
		ippsDivC_32f_I(2.0f, pMassTriangle, count);

		//add the result to the center of mass:
		//pCenterBody += MassTriangle * CenterTriangles   
		for (j=0; j<loopSize; j++)
		{
			pCenterBody[0] +=
				pCenterTriangle[j*4] * pMassTriangle[j];
			pCenterBody[1] +=
				pCenterTriangle[j*4+1] * pMassTriangle[j];
			pCenterBody[2] +=
				pCenterTriangle[j*4+2] * pMassTriangle[j];

			//add triangle's mass to the body's mass
//			massBody += pMassTriangle[j];
		}
		ippsSum_32f(pMassTriangle, count, &massTmp,
				   ippAlgHintNone);
		massBody += massTmp;
		
		}
		m_buffer_.ReleaseAll();
	}
	//normalize the body's center mass coordinates:
	//pCenterBody *= 1/massBody; 
	st = ippmMul_vc_32f_3x1(pCenterBody, 1.0f/massBody,
//			pCenterBody);
			pPoint);

//	pPoint[0] = pCenterBody[0];
//	pPoint[1] = pCenterBody[1];
//	pPoint[2] = pCenterBody[2];
	*pMass = massBody;
}
