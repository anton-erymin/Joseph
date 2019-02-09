/////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:         2 July 2007
// Description:	Library for working with virtual camera in OpenGL.
//////////////////////////////////////////////////////////////////////

#ifndef camera_h
#define camera_h

#include <math.h>
#include "bbox.h"
#include "vectors.h"

typedef float Frustum[6][4];


class Camera
{
private:
	float pos[3];
	float eye[3], up[3], center[3], forward[3], side[3];
	float pitch, yaw, roll;
	float sensitivity;
	Frustum frustum;

public:
	Camera();
	void SetPitch(float p);
	void SetYaw(float y);
	void SetRoll(float r);
	void SetPosition(float x, float y, float z);
	void SetSensitivity(float sens);
	void SetLook(float eyex, float eyey, float eyez, float cx, float cy, float cz, float upx, float upy, float upz);
	float GetPitch();
	float GetYaw();
	float GetRoll();
	float *GetPosition();
	float GetSensitivity();
	void GetFrustum();
	void SetCamera();
	void Move(float t);
	void Strafe(float t);
	void MouseLook();
	bool InFrustum(BoundingBox *b);
	void RotateY(float a);
};

Camera::Camera()
{
	pos[0] = 0.0f;
	pos[1] = 0.0f;
	pos[2] = 0.0f;
	pitch = yaw = roll = 0.0f;
	sensitivity = 5.0f;
	SetCursorPos(512, 384);
};


void Camera::SetPitch(float p)
{
	pitch = p;
};

void Camera::SetYaw(float y)
{
	yaw = y;
};

void Camera::SetRoll(float r)
{
	roll = r;
};

void Camera::SetPosition(float x, float y, float z)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
};

void Camera::SetSensitivity(float sens)
{
	sensitivity = sens;
};

float Camera::GetPitch()
{
	return pitch;
};

float Camera::GetYaw()
{
	return yaw;
};

float Camera::GetRoll()
{
	return roll;
};

float *Camera::GetPosition()
{
	return pos;
};

float Camera::GetSensitivity()
{
	return sensitivity;
};

void Camera::SetCamera()
{
	glLoadIdentity();
	glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(-roll, 0.0f, 0.0f, 1.0f);
	glTranslatef(-pos[0], -pos[1], -pos[2]);	

	/*glLoadIdentity();
	center[0] = eye[0] + forward[0];
	center[1] = eye[1] + forward[1];
	center[2] = eye[2] + forward[2];
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);*/
};

void Camera::Move(float t)
{
	pos[0] -= (float)sin(yaw*PI_DIV_180)*t;
	pos[2] -= (float)cos(yaw*PI_DIV_180)*t;
};

void Camera::Strafe(float t)
{
	pos[0] += (float)sin((yaw-90.0f)*PI_DIV_180)*t;
	pos[2] += (float)cos((yaw-90.0f)*PI_DIV_180)*t;
};
void Camera::MouseLook()
{
	POINT p; 
	GetCursorPos(&p);
	SetCursorPos(512, 384);

	GLint delta_x = p.x - 512;
	GLint delta_y = p.y - 384;

	if ((delta_x != 0) || (delta_y != 0))
	{
		yaw -= (float)delta_x/100*sensitivity;
		pitch -= (float)delta_y/100*sensitivity;

		if (yaw > 360) yaw = 0.0f;
		if (yaw < 0) yaw = 360.0f;
		if (pitch > 90) pitch = 90.0f;
		if (pitch < -90) pitch = -90;
	}
};

void Camera::GetFrustum()
{
	float view[16], proj[16], clip[16];
	float t;

	glPushMatrix();
	glGetFloatv(GL_MODELVIEW_MATRIX, view);
	glGetFloatv(GL_PROJECTION_MATRIX, proj);
	glLoadMatrixf(proj);
	glMultMatrixf(view);
	glGetFloatv(GL_MODELVIEW_MATRIX, clip);
	glPopMatrix();
	
	frustum[0][0] = clip[ 3] - clip[ 0];
	frustum[0][1] = clip[ 7] - clip[ 4];
	frustum[0][2] = clip[11] - clip[ 8];
	frustum[0][3] = clip[15] - clip[12];

	t = (float) sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
	frustum[0][0] /= t;
	frustum[0][1] /= t;
	frustum[0][2] /= t;
	frustum[0][3] /= t;

	frustum[1][0] = clip[ 3] + clip[ 0];
	frustum[1][1] = clip[ 7] + clip[ 4];
	frustum[1][2] = clip[11] + clip[ 8];
	frustum[1][3] = clip[15] + clip[12];

	t = (float) sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
	frustum[1][0] /= t;
	frustum[1][1] /= t;
	frustum[1][2] /= t;
	frustum[1][3] /= t;

	frustum[2][0] = clip[ 3] + clip[ 1];
	frustum[2][1] = clip[ 7] + clip[ 5];
	frustum[2][2] = clip[11] + clip[ 9];
	frustum[2][3] = clip[15] + clip[13];

	t = (float) sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
	frustum[2][0] /= t;
	frustum[2][1] /= t;
	frustum[2][2] /= t;
	frustum[2][3] /= t;

	frustum[3][0] = clip[ 3] - clip[ 1];
	frustum[3][1] = clip[ 7] - clip[ 5];
	frustum[3][2] = clip[11] - clip[ 9];
	frustum[3][3] = clip[15] - clip[13];

	t = (float) sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
	frustum[3][0] /= t;
	frustum[3][1] /= t;
	frustum[3][2] /= t;
	frustum[3][3] /= t;

	frustum[4][0] = clip[ 3] - clip[ 2];
	frustum[4][1] = clip[ 7] - clip[ 6];
	frustum[4][2] = clip[11] - clip[10];
	frustum[4][3] = clip[15] - clip[14];

	t = (float) sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
	frustum[4][0] /= t;
	frustum[4][1] /= t;
	frustum[4][2] /= t;
	frustum[4][3] /= t;

	frustum[5][0] = clip[ 3] + clip[ 2];
	frustum[5][1] = clip[ 7] + clip[ 6];
	frustum[5][2] = clip[11] + clip[10];
	frustum[5][3] = clip[15] + clip[14];

	t = (float) sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
	frustum[5][0] /= t;
	frustum[5][1] /= t;
	frustum[5][2] /= t;
	frustum[5][3] /= t;
};
bool Camera::InFrustum(BoundingBox *b)
{
	for (int p = 0; p < 6; p++)
	{
		if(frustum[p][0]*b->xmin + frustum[p][1]*b->ymin + frustum[p][2]*b->zmin + frustum[p][3] > 0) continue;
		if(frustum[p][0]*b->xmax + frustum[p][1]*b->ymin + frustum[p][2]*b->zmin + frustum[p][3] > 0) continue;
		if(frustum[p][0]*b->xmin + frustum[p][1]*b->ymax + frustum[p][2]*b->zmin + frustum[p][3] > 0) continue;
		if(frustum[p][0]*b->xmax + frustum[p][1]*b->ymax + frustum[p][2]*b->zmin + frustum[p][3] > 0) continue;
		if(frustum[p][0]*b->xmin + frustum[p][1]*b->ymin + frustum[p][2]*b->zmax + frustum[p][3] > 0) continue;
		if(frustum[p][0]*b->xmax + frustum[p][1]*b->ymin + frustum[p][2]*b->zmax + frustum[p][3] > 0) continue;
		if(frustum[p][0]*b->xmin + frustum[p][1]*b->ymax + frustum[p][2]*b->zmax + frustum[p][3] > 0) continue;
		if(frustum[p][0]*b->xmax + frustum[p][1]*b->ymax + frustum[p][2]*b->zmax + frustum[p][3] > 0) continue;
		return false;
	}
	return true;
};
void Camera::SetLook(float eyex, float eyey, float eyez, float cx, float cy, float cz, float upx, float upy, float upz)
{
	eye[0] = eyex;
	eye[1] = eyey;
	eye[2] = eyez;

	center[0] = cx;
	center[1] = cy;
	center[2] = cz;

	up[0] = upx;
	up[1] = upy;
	up[2] = upz;

	forward[0] = center[0] - eye[0];
	forward[1] = center[1] - eye[1];
	forward[2] = center[2] - eye[2];

	Cross(forward, up, side);
};

void Camera::RotateY(float a) 
{ 
  forward[0] = forward[0]*cos(a*PI_DIV_180) - side[0]*sin(a*PI_DIV_180); 
  forward[1] = forward[1]*cos(a*PI_DIV_180) - side[1]*sin(a*PI_DIV_180); 
  forward[2] = forward[2]*cos(a*PI_DIV_180) - side[2]*sin(a*PI_DIV_180); 
  Normalize(forward, forward);
  Cross(forward, up, side); 
};
#endif
