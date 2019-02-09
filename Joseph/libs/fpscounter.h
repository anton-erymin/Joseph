/////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:         2 July 2007
// Description:	Library for counting FPS (frames per second).
//////////////////////////////////////////////////////////////////////

#ifndef fpscounter_h
#define fpscounter_h

#include <cstdio>

class FPSCounter
{
private:
	GLuint numFrames;
	DWORD lastTime, time;
	GLfloat currentFPS;

public:
	FPSCounter();
	void Init();
	GLfloat GetFPS();
	void GetStrFPS(char *buf);
};

FPSCounter::FPSCounter()
{
	numFrames = 0;
	currentFPS = 0.0f;
};

void FPSCounter::Init()
{
	lastTime = GetTickCount();
};

GLfloat FPSCounter::GetFPS()
{
	numFrames++;
	time = GetTickCount();
	DWORD elapsed = time - lastTime;
	if (elapsed > 1000)
	{
		currentFPS = numFrames*1000.0f/elapsed;
		numFrames = 0;
		lastTime = time;
	}
	return currentFPS;
};

void FPSCounter::GetStrFPS(char* buf)
{
	GetFPS();
	sprintf(buf, "FPS: %d frames/sec", (int)currentFPS);
};

#endif

