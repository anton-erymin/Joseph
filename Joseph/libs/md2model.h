/////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:         18 October 2007
// Description:	Library for working with Quake II MD2 file format.
//////////////////////////////////////////////////////////////////////

#ifndef md2model_h
#define md2model_h

#include <stdio.h>
#include <cstdio>
#include <stdlib.h>

#include "anorms.h"
#include "bbox.h"

#pragma pack(1)

typedef float MD2Vector[3];

typedef struct
{
	char ident[4];
	int version;
	int skinwidth;
	int skinheight;
	int framesize;
	int numSkins;
	int numVerts;
	int numTexCoords;
	int numTriangles;
	int numGLCommands;
	int numFrames;
	int offSkins;
	int offTexCoords;
	int offTriangles;
	int offFrames;
	int offGLCommands;
	int offEnd;
} MD2Header;

typedef struct
{
	char name[64];
} MD2Skin;

typedef struct
{
	short s;
	short t;
} MD2TexCoord;

typedef struct
{
	unsigned short vertsInds[3];
	unsigned short texCoordsInds[3];
} MD2Triangle;

typedef struct
{
	float vert[3];
	unsigned char normalIndex;
} MD2Vertex;

typedef struct
{
	MD2Vector scale;
	MD2Vector translate;
	char name[16];
	MD2Vertex *verts;
} MD2Frame;

typedef struct
{
	float s;
	float t;
	int index;
} MD2GLCommand;

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MD2Model
{
private:
	MD2Skin *skins;
	MD2TexCoord *texCoords;
	MD2Triangle *triangles;
	MD2Frame *frames;
	int *GLCommands;
	GLuint *tex;
	BoundingBox *bboxes;

public:
	MD2Header *header;

	MD2Model();
	~MD2Model();
	bool LoadFromFile(char *filename);
	void Draw(Camera *cam, int numframe, float interp);
	void Interpolate(float interp, float *v1, float *v2, float *v);
	void SetTexture(char *filename);
	void GetBoxes();
	void DrawBox(int frame, float time);
};


MD2Model::MD2Model()
{
	header = NULL;
	skins = NULL;
	texCoords = NULL;
	triangles = NULL;
	frames = NULL;
	GLCommands = NULL;
	tex = NULL;
};

MD2Model::~MD2Model()
{
	header= NULL;

	delete[] skins;
	delete[] texCoords;
	delete[] triangles;
	delete[] frames;
	delete[] GLCommands;
	delete[] tex;
};

bool MD2Model::LoadFromFile(char *filename)
{
	unsigned char v[3];

	FILE *f;
	f = fopen(filename, "rb"); 

	header = new MD2Header;

	fread(header, 1, sizeof(MD2Header), f);

	skins = new MD2Skin[header->numSkins];
	texCoords = new MD2TexCoord[header->numTexCoords];
	triangles = new MD2Triangle[header->numTriangles];
	frames = new MD2Frame[header->numFrames];
	GLCommands = new int[header->numGLCommands];
	tex = new GLuint[header->numSkins];

	//fseek(f, header->offSkins, 0);
	fread(skins, sizeof(MD2Skin), header->numSkins, f);
	//fseek(f, header->numTexCoords, 0);
	fread(texCoords, sizeof(MD2TexCoord), header->numTexCoords, f);
	//fseek(f, header->numTriangles, 0);
	fread(triangles, sizeof(MD2Triangle), header->numTriangles, f);

	//fseek(f, header->offFrames, 0);
	for (int i = 0; i < header->numFrames; i++)
	{
		frames[i].verts = new MD2Vertex[header->numVerts];
		fread(frames[i].scale, 3, 4, f);
		fread(frames[i].translate, 3, 4, f);
		fread(frames[i].name, 1, 16, f);

		for (int j = 0; j < header->numVerts; j++)
		{
			fread(v, 1, 3, f);
			frames[i].verts[j].vert[0] = (frames[i].scale[0] * v[0]) + frames[i].translate[0];
			frames[i].verts[j].vert[1] = (frames[i].scale[1] * v[1]) + frames[i].translate[1];
			frames[i].verts[j].vert[2] = (frames[i].scale[2] * v[2]) + frames[i].translate[2];
			fread(&frames[i].verts[j].normalIndex, 1, 1, f);
		}
	}

	fread(GLCommands, 4, header->numGLCommands, f);

	fclose(f);

	f = fopen("c:\\1.txt", "wt");
	char s[32];
	for (int i = 0; i < 32; i++) s[i] = 0;
	for (int i = 0; i < header->numFrames; i++)
	{
		strncpy(s, frames[i].name, 16);
		strcat(s, "\n");
		fputs(s, f);
	}


	fclose(f);

	GetBoxes();

	return true;
};


void MD2Model::Draw(Camera *cam, int numframe, float interp)
{
	if (!cam->InFrustum(&bboxes[numframe])) return;

	GLfloat s, t;
	MD2Vector v, v1, v2;
	float n[3], n1[3], n2[3];
	MD2Frame *frame1, *frame2;
	MD2Vertex *vert1, *vert2;
	MD2GLCommand *packet;
	int i;
	int *ptr;

	if ((numframe < 0) || (numframe > header->numFrames-1)) return;

	float nomat[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT, nomat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, nomat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, nomat);
	glMaterialfv(GL_FRONT, GL_EMISSION, nomat);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);

	frame1 = &frames[numframe];
	frame2 = &frames[numframe+1];

	//glBegin(GL_TRIANGLES);
	//for (int i = 0; i < header->numTriangles; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		
	//		vert = &frame->verts[triangles[i].vertsInds[j]];

	//		s = texCoords[triangles[i].texCoordsInds[j]].s/header->skinwidth;
	//		t = texCoords[triangles[i].texCoordsInds[j]].t/header->skinheight;

	//		//glTexCoord2f(s, t);
	//		glNormal3fv(norms[vert->normalIndex]);

	//		v[0] = (frame->scale[0]*(float)vert->vert[0]) + frame->translate[0];
	//		v[1] = (frame->scale[1]*(float)vert->vert[1]) + frame->translate[1];
	//		v[2] = (frame->scale[2]*(float)vert->vert[2]) + frame->translate[2];

	//		glVertex3fv(v);
	//	}
	//}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *tex);

	ptr = GLCommands;
	i = *(ptr);

	while (i != 0)
    {
		if (i < 0)
		{
		glBegin (GL_TRIANGLE_FAN);
		i = -i;
		}
		else glBegin (GL_TRIANGLE_STRIP);

		ptr++;
		for (int j = 0; j < i; j++)
		{
			packet = (MD2GLCommand *)ptr;

			vert1 = &frame1->verts[packet->index];
			vert2 = &frame2->verts[packet->index];

			n1[0] = norms[vert1->normalIndex][0];
			n1[1] = norms[vert1->normalIndex][1];
			n1[2] = norms[vert1->normalIndex][2];

			n2[0] = norms[vert2->normalIndex][0];
			n2[1] = norms[vert2->normalIndex][1];
			n2[2] = norms[vert2->normalIndex][2];

			v1[0] = vert1->vert[0];
			v1[1] = vert1->vert[1];
			v1[2] = vert1->vert[2];

			v2[0] = vert2->vert[0];
			v2[1] = vert2->vert[1];
			v2[2] = vert2->vert[2];

			Interpolate(interp, v1, v2, v);
			Interpolate(interp, n1, n2, n);

			glTexCoord2f(packet->s, 1.0f - packet->t);
			glNormal3fv(n);
			glVertex3fv(v);

			ptr += 3;
		}
		i = *(ptr);
		glEnd();
	}
};

void MD2Model::Interpolate(float interp, float *v1, float *v2, float *v)
{
	v[0] = v1[0] + interp * (v2[0] - v1[0]);
	v[1] = v1[1] + interp * (v2[1] - v1[1]);
	v[2] = v1[2] + interp * (v2[2] - v1[2]);
};

void MD2Model::SetTexture(char *filename)
{
	LoadTexture(filename, tex);
};
void MD2Model::GetBoxes()
{
	float xmin, xmax, ymin, ymax, zmin, zmax, x, y, z;
	MD2Frame fr;

	bboxes = new BoundingBox[header->numFrames];

	for (int i = 0; i < header->numFrames; i++)
	{
		fr = frames[i];
		xmin = xmax = fr.verts[0].vert[0];
		ymin = ymax = fr.verts[0].vert[1];
		zmin = zmax = fr.verts[0].vert[2];

		for (int j = 1; j < header->numVerts; j++)
		{
			x = fr.verts[j].vert[0];
			y = fr.verts[j].vert[1];
			z = fr.verts[j].vert[2];

			if (x < xmin) xmin = x;
			else if (x > xmax) xmax = x;

			if (y < ymin) ymin = y;
			else if (y > ymax) ymax = y;

			if (z < zmin) zmin = z;
			else if (z > zmax) zmax = z;
		}

		bboxes[i] = BoundingBox(xmin, xmax, ymin, ymax, zmin, zmax);
	}

};	
void MD2Model::DrawBox(int frame, float time)
{
	/*BoundingBox bbox;
	float f11[3], f12[3], f21[3], f22[3], f31[3], f32[3];

	bbox = bboxes[frame];
	f11[0] = bbox.xmin;
	f11[1] = bbox.xmax;
	f11[2] = bbox.ymin;
	f12[0] = bbox.ymax;
	f12[1] = bbox.zmin;
	f12[2] = bbox.zmax;

	bbox = bboxes[frame+1];
	f21[0] = bbox.xmin;
	f21[1] = bbox.xmax;
	f21[2] = bbox.ymin;
	f22[0] = bbox.ymax;
	f22[1] = bbox.zmin;
	f22[2] = bbox.zmax;

	Interpolate(time, f11, f21, f31);
	Interpolate(time, f21, f22, f32);
	
	BoundingBox *box = new BoundingBox(f31[0], f31[1], f31[2], f32[0], f32[1], f32[2]);
	box->Draw();*/
	bboxes[frame].Draw();
};
#endif