#include <math.h>
#include "libs\opengl.h"
#include "libs\camera.h"
#include "libs\fpscounter.h"
#include "libs\md2model.h"
#include "libs\ms3dmodel.h"
#include "libs\list.h"
#include "libs\anim_geometry.h"

#define NUM_AMISH	4
#define PI_DIV_180	0.017453292

bool keys[256];

Camera *sceneCamera;
FPSCounter *fps;
char fpsstr[32];

MD2Model *model;
MS3DModel *floormodel;
AnimatedGeometry *hero;

bool killing = false;
int killed = 0;

element *cur;

bool run = false;
bool jump = false;

void ProcessKeyboard()
{
	//GLfloat speed = 10.0f;//fps->GetFPS();
 //	GLfloat coeff = 0.5f;
	//if (keys[VK_UP]) sceneCamera->Move(coeff*speed);
	//if (keys[VK_DOWN]) sceneCamera->Move(-coeff*speed);
	//if (keys[VK_LEFT]) sceneCamera->Strafe(coeff*speed);
	//if (keys[VK_RIGHT]) sceneCamera->Strafe(-coeff*speed);
	//if (keys[VK_ESCAPE]) PostQuitMessage(0);
	if (keys[13]) killing = true;
	//if (keys[VK_INSERT])
	//{
	//	Vector v = sceneCamera->GetPosition();
	//	v.y += 5.0f;
	//	sceneCamera->SetPosition(v.x, v.y, v.z);
	//}
	//if (keys[VK_DELETE])
	//{
	//	Vector v = sceneCamera->GetPosition();
	//	v.y -= 5.0f;
	//	sceneCamera->SetPosition(v.x, v.y, v.z);
	//}

	float *pos, *rot;
	pos = hero->GetPosition();
	rot = hero->GetRotation();

	if (keys[VK_UP])
	{
		if (!run) 
		{
			run = true;
			hero->SetAnimation(40, 44, 40, true);
		}

		pos[0] -= (float)sin((rot[2]-90)*PI_DIV_180)*5;
		pos[2] -= (float)cos((rot[2]-90)*PI_DIV_180)*5;
	}
	else if (run && !keys[VK_DOWN]) 
	{
		run = false;
		hero->SetAnimation(0, 38, 0, true);
	}

	if (keys[VK_DOWN])
	{
		if (!run) 
		{
			run = true;
			hero->SetAnimation(40, 44, 40, true);
		}

		pos[0] += (float)sin((rot[2]-90)*PI_DIV_180)*5;
		pos[2] += (float)cos((rot[2]-90)*PI_DIV_180)*5;
	}
	else if (run && !keys[VK_UP]) 
	{
		run = false;
		hero->SetAnimation(0, 38, 0, true);
	}

	hero->SetPosition(pos);
	hero->SetRotation(rot);


	if (keys[VK_ESCAPE]) PostQuitMessage(0);

	if (keys[VK_SPACE])
	{
		if (!jump)
		{
			run = false;
			jump = true;
			hero->SetAnimation(66, 71, 66, false);
		}
	}
}

void ProcessMouse()
{
	POINT p; 
	GetCursorPos(&p);
	SetCursorPos(512, 384);

	int delta_x = p.x - 512;
	int delta_y = p.y - 384;
	
	if ((delta_x != 0) || (delta_y != 0))
	{	
		float *r;
		r = hero->GetRotation();
		r[2] -=(float)delta_x/100*5;
		hero->SetRotation(r);	
	}

	
}
void InitScene()
{
	sceneCamera = new Camera();
	sceneCamera->SetPitch(-20);

	fps = new FPSCounter();
	fps->Init();

	model = new MD2Model();
	model->LoadFromFile("data\\tris.md2");
	model->SetTexture("data\\5.bmp");

	floormodel = new MS3DModel();
	floormodel->LoadFromFile("data\\floor.ms3d");

	hero = new AnimatedGeometry(model);
	hero->SetAnimation(0, 38, 0, true);
	hero->SetPosition(0, 0, 0);
	hero->SetRotation(-90.0f, 0.0f, 90.0f);

	InitList();

	float h = (2*3.1415926)/NUM_AMISH;
	float g = 360/NUM_AMISH;
	for (int i = 0; i < NUM_AMISH; i++)
	{
		float p[3] = {0, 0, 0};
		float r[3] = {-90, 0, -i*g+180};

		p[0] = 40*cos(i*h);
		p[1] = 0;
		p[2] = 40*sin(i*h);

		int cf = 38*(int)rand()/RAND_MAX;
		Insert(model, p, r, 0, 38, cf);
	}
	cur = first;
}
void next()
{
	cur->killed = true;
	cur->geom->EndOfAnimation = NULL;
	killed++;
	if (killed < NUM_AMISH - 1)
	{
		int i = 0;
		while (i < 5)
		{
			cur = cur->next;
			if (!cur->killed) i++;
		}
		killing = true;
	}
	else 
	{
		while (cur->killed) cur = cur->next;
		cur->geom->SetAnimation(66, 71, 66, true);
	}
}
void UpdateDrawList()
{
	if (killing)
	{
		cur->geom->SetAnimation(179, 182, 179, false);
		cur->geom->EndOfAnimation = &next;
		killing = false;

	}

	element *p;
	p = first;
	for (int i = 0; i < NUM_AMISH; i++)
	{
		p->geom->Update();
		p->geom->Draw(sceneCamera);
		//p->geom->GetAxesFromAngles();
		//p->geom->DrawLocalAxes();
		p = p->next;
	}
}
void DrawCoordinateAxes()
{
	glDisable(GL_LIGHTING);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
		glColor3ub(255, 0, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);

		glColor3ub(0, 255, 0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);

		glColor3ub(0, 0, 255);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
	glEnable(GL_LIGHTING);
}  
void DrawScene(HWND hwnd)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ProcessKeyboard();
	ProcessMouse();

	float *rot, *pos, pcam[3];
	rot = hero->GetRotation();
	pos = hero->GetPosition();
	float cosyaw = cos((rot[2]-90)*PI_DIV_180);
	float sinyaw = sin((rot[2]-90)*PI_DIV_180);
	//float cospitch = cos(rot[0]);
	//float sinpitch = sin(rot[0]);
	//pcam[0] = pos[0] + 100*sinyaw*cospitch;
	//pcam[1] = pos[1] - 100*sinpitch;
	//pcam[2] = pos[2] + 100*cosyaw*cospitch;

	pcam[2] = pos[2] + 80*cosyaw;
	pcam[0] = pos[0] + 80*sinyaw;

	sceneCamera->SetPosition(pcam[0], 40, pcam[2]);
	sceneCamera->SetYaw(rot[2]-90);	
	sceneCamera->MouseLook();
	sceneCamera->SetCamera();
	sceneCamera->GetFrustum();

	

	glPushMatrix();
	glTranslatef(0.0f, -23.0f, 0.0f);
	floormodel->Draw();
	glPopMatrix();

	DrawCoordinateAxes();
	glEnable(GL_LIGHTING);

	UpdateDrawList();


	hero->Update();
	hero->Draw(sceneCamera);

	fps->GetStrFPS(fpsstr);
	SetWindowTextA(hwnd, fpsstr);
	SwapBuffers(hDC);
}

