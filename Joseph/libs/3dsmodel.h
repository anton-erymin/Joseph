////////////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:         5 July 2007
// Description:	Library for working with 3DS (3DS Max) format file.
////////////////////////////////////////////////////////////////////////////

#ifndef max3dsmodel_h
#define max3dsmodel_h

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "vectors.h"


#define CHUNK_MAIN								0x4D4D			//Main
#define     CHUNK_OBJMESH						0x3D3D			//Mesh objects
#define         CHUNK_OBJBLOCK					0x4000			//Object
#define             CHUNK_TRIMESH				0x4100			//Triangular mesh
#define                CHUNK_VERTICES			0x4110			//Vertices
#define				   CHUNK_TRIANGLES		0x4120			//Triangles
#define			       CHUNK_MATERIALS		0x4130			//Materials
#define			       CHUNK_TEXCOORDS		0x4140			//Texture coordinates
#define			       CHUNK_TRMATRIX			0x4160			//Transformation matrix
#define			   CHUNK_CAMERA				0x4700			//Camera
#define     CHUNK_MATERIAL						0xAFFF			//Material
#define         CHUNK_MATNAME					0xA000			//Material name
#define         CHUNK_TEXTURE					0xA200			//Texture of material
#define             CHUNK_MAPFILE				0xA300			//Texture filename
#define     CHUNK_KEYFRAMER					0xB000			//Information about animation
#define         CHUNK_TRACKINFO				0xB002			//Objects behaviour
#define             CHUNK_TRACKOBJNAME		0xB010			//Object name
#define             CHUNK_TRACKPIVOT			0xB013			//Object pivot
#define             CHUNK_TRACKPOS				0xB020			//Position
#define             CHUNK_TRACKROTATE		0xB021			//Rotation
#define         CHUNK_TRACKCAMERA			0xB003			//Camera behaviour
#define             CHUNK_TRACKFOV				0xB023			//FOV
#define             CHUNK_TRACKROLL			0xB024			//Roll
#define         CHUNK_TRACKCAMTGT			0xB004			//Target




typedef struct
{
	unsigned short id;
	unsigned int length;
} Max3DSChunk;

typedef struct
{
	float vertex[3];
} Max3DSVertex;

typedef struct
{
	unsigned short vertexInds[3];
	unsigned short flags;
} Max3DSTriangle;

typedef struct
{
    float normal[3];
} Max3DSNormal;

typedef struct
{
	char name[20];
	unsigned short numVerts;
	unsigned short numTriangles;

	Max3DSVertex *verts;
	Max3DSTriangle *triangles;
	Max3DSNormal *normals;
} Max3DSGroup;


void cread(FILE *f, Max3DSChunk *chunk)
{
	fread(chunk, 1, sizeof(Max3DSChunk), f);
};


///////////////////////////////////////////////////////////////////////////////

class Max3DSModel
{
private:
	unsigned short numGroups;
	Max3DSGroup *groups;

public:
	Max3DSModel();
	~Max3DSModel();
	bool LoadFromFile(char *filename);
	void Draw();
	void CalcNormals();
};


Max3DSModel::Max3DSModel()
{
	numGroups = 0;
	groups = NULL;

};


Max3DSModel::~Max3DSModel()
{
	delete[] groups;
};


bool Max3DSModel::LoadFromFile(char *filename)
{
	int p;
	FILE *f;
	f = fopen(filename, "rb");

	if (!f)
	{
		MessageBoxA(0, "Failed to open file", "3DS Loader", MB_ICONERROR);
		return false;
	}

	Max3DSChunk chunk;	
	
	while (!eof(fileno(f)))
	{
		cread(f, &chunk);

		switch (chunk.id)
		{
			case CHUNK_MAIN:
				break;

			case CHUNK_OBJMESH:
				break;

			case CHUNK_OBJBLOCK:
				numGroups++;		
			    fseek(f, chunk.length - 6, SEEK_CUR);	
				break;

			default:
				fseek(f, chunk.length - 6, SEEK_CUR);
		}
	}


	fseek(f, 0, SEEK_SET);

//numGroups = 1;
	groups = new Max3DSGroup[numGroups];
	unsigned short curGroup = 0;

	while (!eof(fileno(f)))
	{
		cread(f, &chunk);

		switch (chunk.id)
		{
			case CHUNK_MAIN:
				break;

			case CHUNK_OBJMESH:
				break;

			case CHUNK_OBJBLOCK:
				char c;
				p = 0;
				do
				{
					fread(&c, 1, 1, f);
					groups[curGroup].name[p] = c;
					p++;
				} while (c != 0 && p < 20);
				break;

			case CHUNK_TRIMESH:
				break;

			case CHUNK_VERTICES:
				fread(&groups[curGroup].numVerts, 1, 2, f);
				groups[curGroup].verts = new Max3DSVertex[groups[curGroup].numVerts];
				fread(groups[curGroup].verts, sizeof(Max3DSVertex), groups[curGroup].numVerts, f);
				break;

			case CHUNK_TRIANGLES:
				fread(&groups[curGroup].numTriangles, 1, 2, f);
				groups[curGroup].triangles = new Max3DSTriangle[groups[curGroup].numTriangles];
				fread(groups[curGroup].triangles, sizeof(Max3DSTriangle), groups[curGroup].numTriangles, f);
				curGroup++;
				break;

			default:
				fseek(f, chunk.length - 6, SEEK_CUR);
		}
	}

	fclose(f);
	
	CalcNormals();

	return true;
};


void Max3DSModel::CalcNormals()
{
	Vector *faceNormals[numGroups];
    
	for (int i = 0; i < numGroups; i++)
	{
		faceNormals[i] = new Vector[groups[i].numTriangles];
		
		for (int j = 0; j < groups[i].numTriangles; j++)
		{
			Max3DSTriangle tr = groups[i].triangles[j];
			Vector a = Vector(groups[i].verts[tr.vertexInds[0]].vertex);
			Vector b = Vector(groups[i].verts[tr.vertexInds[1]].vertex);
			Vector c = Vector(groups[i].verts[tr.vertexInds[2]].vertex);

			Vector ab = c - a;
			Vector cb = c - b;
			Vector n = CrossProduct(ab, cb);
			n = Normalize(n);
			faceNormals[i][j] = n;
		}
	}

	for (int i = 0; i < numGroups; i++)
	{
		groups[i].normals = new Max3DSNormal[groups[i].numVerts];
		
		for (int j = 0; j < groups[i].numVerts; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				groups[i].normals[j].normal[k] = 0.0f;
			}
		}
	}
	
	for (int i = 0; i < numGroups; i++)
 	{
 		for (int j = 0; j < groups[i].numTriangles; j++)
 		{
 			for (int k = 0; k < 3; k++)
 			{
				unsigned short index = groups[i].triangles[j].vertexInds[k];
				
 				groups[i].normals[index].normal[0] += faceNormals[i][j].x;
 				groups[i].normals[index].normal[1] += faceNormals[i][j].y;
 				groups[i].normals[index].normal[2] += faceNormals[i][j].z;
			}
 		}
  	}	

  	for (int i = 0; i < numGroups; i++)
  	{
  		for (int j = 0; j < groups[i].numVerts; j++)
  		{
  			Vector v = Vector(groups[i].normals[j].normal[0], 
					          groups[i].normals[j].normal[1], 
          					  groups[i].normals[j].normal[2]);
		    v = Normalize(v);
		    groups[i].normals[j].normal[0] = v.x;
		    groups[i].normals[j].normal[1] = v.y;
		    groups[i].normals[j].normal[2] = v.z;
  		}
  		
    	delete[] faceNormals[i];
    }
  		

};


void Max3DSModel::Draw()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numGroups; i++)
		for (int j = 0; j < groups[i].numTriangles; j++)
		{
		
			for (int k = 0; k < 3; k++)
			{
				unsigned short index = groups[i].triangles[j].vertexInds[k];
				glNormal3fv(groups[i].normals[index].normal);
				glVertex3fv(groups[i].verts[index].vertex);
			}
		}
	glEnd();
};


#endif
