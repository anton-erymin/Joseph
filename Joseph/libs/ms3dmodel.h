/////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:         5 July 2007
// Description:	Library for working with MilkShape3D format file.
//////////////////////////////////////////////////////////////////////

#ifndef ms3dmodel_h
#define ms3dmodel_h

#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include "vectors.h"

#pragma pack(1)

typedef struct 
{
	char id[10];
	unsigned int version;
} MS3DHeader;

typedef struct
{
	unsigned char flags;
	float vertex[3];
	char boneId;
	unsigned char referenceCount;
} MS3DVertex;

typedef struct
{
	unsigned short flags;
	unsigned short vertexInds[3];
	float vertexNormals[3][3];
	float s[3];
	float t[3];
	unsigned char smoothingGroup;
	unsigned char groupIndex;
} MS3DTriangle;

typedef struct
{
	unsigned char flags;
	char name[32];
	unsigned short numTriangles;
	unsigned short *triangleInds;
	char materialIndex;
} MS3DGroup;

typedef struct
{
	char name[32];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shininess;
	float transparency;
	char mode;
	char texture[128];
	char alphamap[128];
} MS3DMaterial;

typedef struct
{
	float time;
	float rotation[3];
} MS3DKeyframeRotation;

typedef struct
{
	float time;
	float position[3];
} MS3DKeyframePosition;

typedef struct
{
	unsigned char flags;
	char name[32];
	char parentName[32];
	float rotation[3];
	float position[3];
	unsigned short numKeyframesRot;
	unsigned short numKeyframesTrans;
	MS3DKeyframeRotation *keyframesRot;
	MS3DKeyframePosition *keyframesTrans;
} MS3DJoint;

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MS3DModel
{
private:
	unsigned short numVerts;
	unsigned short numTriangles;
	unsigned short numGroups;
	unsigned short numMaterials;
	unsigned short numJoints;

	float animationFPS;
	float currentTime;
	int totalFrames;

	MS3DVertex *verts;
	MS3DTriangle *triangles;
	MS3DGroup *groups;
	MS3DMaterial *materials;
	MS3DJoint *joints;
	GLuint *textures;

public:
	MS3DModel();
	~MS3DModel();
	bool LoadFromFile(char *filename);
	void Draw();
};


MS3DModel::MS3DModel()
{
	numVerts = 0;
	numTriangles = 0;
	numGroups = 0;
	numMaterials = 0;
	numJoints = 0;

	verts = NULL;
	triangles = NULL;
	groups = NULL;
	materials = NULL;
	joints = NULL;
	textures = NULL;
};

MS3DModel::~MS3DModel()
{
	delete[] verts;
	delete[] triangles;
	delete[] groups;
	delete[] materials;
	delete[] joints;
	delete[] textures;
};

bool MS3DModel::LoadFromFile(char *filename)
{
	FILE *f;
	f = fopen(filename, "rb"); 

	if (!f)
	{
		MessageBoxA(0, "Failed to open file", "MS3D Loader", MB_ICONERROR);
		return false;
	}

	MS3DHeader header;
	fread(&header, 1, sizeof(header), f);

	if (strncmp(header.id, "MS3D000000", 10) != 0)
	{
		MessageBoxA(0, "Invalid format file", "MS3D Loader", MB_ICONERROR);
		return false;
	}


	if (header.version != 4)
	{
	    MessageBoxA(0, "Version of file is not supported.\nOnly 1.4v is supported.", "MS3D Loader", MB_ICONERROR);
		return false;
	}

	fread(&numVerts, 1, 2, f);
	verts = new MS3DVertex[numVerts];
	fread(verts, sizeof(MS3DVertex), numVerts, f);
	
	fread(&numTriangles, 1, 2, f);
	triangles = new MS3DTriangle[numTriangles];
	fread(triangles, sizeof(MS3DTriangle), numTriangles, f);

	fread(&numGroups, 1, 2, f);
	groups = new MS3DGroup[numGroups];
	for (int i = 0; i < numGroups; i++)
	{
		fread(&groups[i].flags, 1, 1, f);
		fread(groups[i].name, 1, 32, f);
		fread(&groups[i].numTriangles, 1, 2, f);
		groups[i].triangleInds = new unsigned short[groups[i].numTriangles];
		fread(groups[i].triangleInds, 2, groups[i].numTriangles, f);
		fread(&groups[i].materialIndex, 1, 1, f);
	}

	fread(&numMaterials, 1, 2, f);
	materials = new MS3DMaterial[numMaterials];
	fread(materials, sizeof(MS3DMaterial), numMaterials, f);
	
	fread(&animationFPS, 4, 1, f);
	fread(&currentTime, 4, 1, f);
	fread(&totalFrames, 4, 1, f);

	fread(&numJoints, 1, 2, f);
	joints = new MS3DJoint[numJoints];
	for (int i = 0; i < numJoints; i++)
	{
		fread(&joints[i].flags, 1, 1, f);
		fread(joints[i].name, 32, 1, f);
		fread(joints[i].parentName, 32, 1, f);
		fread(joints[i].rotation, 4, 3, f);
		fread(joints[i].position, 4, 3, f);
		fread(&joints[i].numKeyframesRot, 2, 1, f);
		fread(&joints[i].numKeyframesTrans, 2, 1, f);
		joints[i].keyframesRot = new MS3DKeyframeRotation[joints[i].numKeyframesRot];
		joints[i].keyframesTrans = new MS3DKeyframePosition[joints[i].numKeyframesTrans];
		fread(joints[i].keyframesRot, sizeof(MS3DKeyframeRotation), joints[i].numKeyframesRot, f);
		fread(joints[i].keyframesTrans, sizeof(MS3DKeyframePosition), joints[i].numKeyframesTrans, f);
	}

	fclose(f);


	char texture[128], path[128];
	strcpy(path, filename);
	int p = strlen(path) - 1;
	char c = path[p];
	char slesh = '\\';
	while (c != slesh)
	{
		p--;
		c = path[p];
	}
	p++;
	path[p] = 0;



	textures = new GLuint[numMaterials];
	for (int i = 0; i < numMaterials; i++)
		if (strlen(materials[i].texture) != 0) 
		{
			strcpy(texture, path);
			strcat(texture, materials[i].texture);
			LoadTexture(texture, &textures[i]);
		}


	return true;
};


void MS3DModel::Draw()
{
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numGroups; i++)
	{
		char indexMaterial = groups[i].materialIndex;
		if (indexMaterial >= 0)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, materials[indexMaterial].ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[indexMaterial].diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, materials[indexMaterial].specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, materials[indexMaterial].emission);
			glMaterialf(GL_FRONT, GL_SHININESS, materials[indexMaterial].shininess);
			if (strlen(materials[indexMaterial].texture) != 0)
			{
				glEnd();
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textures[indexMaterial]);
				glBegin(GL_TRIANGLES);
			}
		}


		for (int j = 0; j < groups[i].numTriangles; j++)
		{
			unsigned short indexTriangle = groups[i].triangleInds[j];
			for (int k = 0; k < 3; k++)
			{
				unsigned short indexVert = triangles[indexTriangle].vertexInds[k];
				glNormal3fv(triangles[indexTriangle].vertexNormals[k]);
				glTexCoord2f(triangles[indexTriangle].s[k], 1-triangles[indexTriangle].t[k]);
				glVertex3fv(verts[indexVert].vertex);
			}
		}
	}
	glEnd();
};

#endif

