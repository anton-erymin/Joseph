/////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:        2 January 2008
// Description:	Animated geometry class.
//////////////////////////////////////////////////////////////////////

#ifndef anim_geometry_h
#define anim_geometry_h

#include "md2model.h"
#include "camera.h"
#include "vectors.h"

class AnimatedGeometry
{
private:
	MD2Model *model;
	int firstFrame, lastFrame, curFrame;
	bool loop, play;
	float time;
	float pos[3];
	float rot[3];
public:
	float forward[3], side[3], up[3];
	

public:
	void (*EndOfAnimation)();

	AnimatedGeometry(MD2Model *mod);
	~AnimatedGeometry();

	void SetPosition(float px, float py, float pz);
	void SetPosition(float *p);

	void SetRotation(float rx, float ry, float rz);
	void SetRotation(float *r);

	void SetAnimation(int ff, int lf, int cf, bool l);

	float *GetPosition();
	float *GetRotation();

	void Update();
	void Draw(Camera *cam);
	void DrawLocalAxes();

	void SetOrientation(float f1, float f2, float f3, float u1, float u2, float u3);
	void SetOrientation(float *f, float *u);

	void GetAxesFromAngles();
};

AnimatedGeometry::AnimatedGeometry(MD2Model *mod)
{
	model = mod;
	time = 0.0f;
};

AnimatedGeometry::~AnimatedGeometry()
{
	model = NULL;
};

void AnimatedGeometry::SetPosition(float px, float py, float pz)
{
	pos[0] = px;
	pos[1] = py;
	pos[2] = pz;
};

void AnimatedGeometry::SetPosition(float *p)
{
	pos[0] = p[0];
	pos[1] = p[1];
	pos[2] = p[2];
};

void AnimatedGeometry::SetRotation(float rx, float ry, float rz)
{
	rot[0] = rx;
	rot[1] = ry;
	rot[2] = rz;
};

void AnimatedGeometry::SetRotation(float *r)
{
	rot[0] = r[0];
	rot[1] = r[1];
	rot[2] = r[2];
};

void AnimatedGeometry::SetAnimation(int ff, int lf, int cf, bool l)
{
	firstFrame = ff;
	lastFrame = lf;
	curFrame = cf;
	loop = l;
	time = 0.0f;
	play = true;
};

float *AnimatedGeometry::GetPosition()
{
	return pos;
};

float *AnimatedGeometry::GetRotation()
{
	return rot;
};

void AnimatedGeometry::Update()
{	
	if (play)
	{
		time += 0.1f;
		if (time > 1.0f) 
		{
			time = 0.0f;
			curFrame++;
			if (curFrame > lastFrame) 
			{
				if (loop) curFrame = firstFrame;
				else 
				{
					play = false;
					curFrame--;
					if (EndOfAnimation) EndOfAnimation();
				}
			}
		}
	}
};

void AnimatedGeometry::Draw(Camera *cam)
{
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
	glRotatef(rot[2], 0.0f, 0.0f, 1.0f);
	cam->GetFrustum();
	model->Draw(cam, curFrame, time);
	model->DrawBox(curFrame, time);
	glPopMatrix();	
};

void AnimatedGeometry::SetOrientation(float f1, float f2, float f3, float u1, float u2, float u3)
{
	forward[0] = f1;
	forward[1] = f2;
	forward[2] = f3;

	up[0] = u1;
	up[1] = u2;
	up[2] = u3;

	Cross(forward, up, side);
};
void AnimatedGeometry::SetOrientation(float *f, float *u)
{
	forward[0] = f[0];
	forward[1] = f[1];
	forward[2] = f[2];

	up[0] = u[0];
	up[1] = u[1];
	up[2] = u[2];

	Cross(forward, up, side);
};

void AnimatedGeometry::DrawLocalAxes()
{
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3ub(0, 0, 255);
		glVertex3f(0, 0, 0); 
		glVertex3fv(forward);

		glColor3ub(255, 0, 0);
		glVertex3f(0, 0, 0); 
		glVertex3fv(side);

		glColor3ub(0, 255, 0);
		glVertex3f(0, 0, 0); 
		glVertex3fv(up);
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
};
void AnimatedGeometry::GetAxesFromAngles()
{
	float m[16];
	forward[0] = 0;
	forward[1] = 0;
	forward[2] = 1;
	side[0] = 1;
	side[1] = 0;
	side[2] = 0;

	glPushMatrix();
	glLoadIdentity();
	glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
	glRotatef(rot[2], 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();

	forward[0] = m[0]*forward[0] + m[4]*forward[1] + m[8]*forward[2] + m[12];
	forward[1] = m[1]*forward[0] + m[5]*forward[1] + m[9]*forward[2] + m[13];
	forward[2] = m[2]*forward[0] + m[6]*forward[1] + m[10]*forward[2] + m[14];

	side[0] = m[0]*side[0] + m[4]*side[1] + m[8]*side[2] + m[12];
	side[1] = m[1]*side[0] + m[5]*side[1] + m[9]*side[2] + m[13];
	side[2] = m[2]*side[0] + m[6]*side[1] + m[10]*side[2] + m[14];

	Cross(forward, side, up);
	Normalize(forward, forward);
	Normalize(side, side);
	Normalize(up, up);

	float l = 5;

	forward[0] *= l;
	forward[1] *= l;
	forward[2] *= l;

	side[0] *= l;
	side[1] *= l;
	side[2] *= l;

	up[0] *= l;
	up[1] *= l;
	up[2] *= l;

};
#endif