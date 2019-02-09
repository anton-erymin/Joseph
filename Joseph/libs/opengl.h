#ifndef opengl_h
#define opengl_h

#define WIDTH     800
#define HEIGHT    600

#include "stdafx.h"

HWND hWnd;
HDC hDC;
HGLRC hRC;

int InitOpenGL(HWND hwnd)
{
	hWnd = hwnd;
	hDC = GetDC(hWnd);

	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);
	dm.dmPelsWidth = WIDTH;
	dm.dmPelsHeight = HEIGHT;
	dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	//ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

	//ShowCursor(false);


	PIXELFORMATDESCRIPTOR pfd = {0};

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!pixelFormat) return false;
	if (!SetPixelFormat(hDC, pixelFormat, &pfd)) return false;

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.3f, 0.1f, 1000.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_MODELVIEW);

	//glViewport(0, 0, 750, 1000);

	glClearColor(0.5f, 0.5f, 0.8f, 1.0f);

	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat pos[] = {0.0f, 30.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);	
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1); 
	//glEnable(GL_COLOR_MATERIAL); 

	//glEnable(GL_DIFFUSE);
	//glEnable(GL_SPECULAR);

	//glPolygonMode(GL_BACK, GL_LINE);

	float shin = 128.0f;
	//glMaterialfv(GL_BACK, GL_SHININESS, &shin);

	glEnable(GL_LINE_SMOOTH);

	return true;
}

void ReleaseOpenGL()
{
	if (hRC)
	{
		wglMakeCurrent(hDC, 0);
		wglDeleteContext(hRC);
		hRC = 0;
	}
	if (hDC)
	{
		ReleaseDC(hWnd, hDC);
		hDC = 0;
	}
}

void LoadTexture(char *filename, GLuint *tex)
{
	AUX_RGBImageRec *image = auxDIBImageLoadA(filename);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
}


#endif

