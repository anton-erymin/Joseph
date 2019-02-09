////////////////////////////////////////////////////////////////////////////
// Author:		Anton Erymin										
// E-mail:		anton-erymin@mail.ru
// Version:		0.1
// Date:        3 Jaunary 2008
// Description:	Bounding box class.
////////////////////////////////////////////////////////////////////////////

#ifndef bbox_h
#define bbox_h


class BoundingBox
{
public:
	float center[3];
	float xmin, xmax, ymin, ymax, zmin, zmax;

public:
	BoundingBox();
	BoundingBox(float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax);
	~BoundingBox();
	void Draw();
};

BoundingBox::BoundingBox()
{
	xmin = xmax = ymin = ymax = zmin = zmax = 0.0f;
};

BoundingBox::BoundingBox(float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax)
{
	xmin = _xmin;
	xmax = _xmax;
	ymin = _ymin;
	ymax = _ymax;
	zmin = _zmin;
	zmax = _zmax;
};

BoundingBox::~BoundingBox()
{
};

void BoundingBox::Draw()
{
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glColor3ub(255, 255, 255);

		glVertex3f(xmin, ymin, zmax);
		glVertex3f(xmax, ymin, zmax);

		glVertex3f(xmin, ymax, zmax);
		glVertex3f(xmax, ymax, zmax);

		glVertex3f(xmin, ymin, zmax);
		glVertex3f(xmin, ymax, zmax);

		glVertex3f(xmax, ymin, zmax);
		glVertex3f(xmax, ymax, zmax);


		glVertex3f(xmin, ymin, zmin);
		glVertex3f(xmax, ymin, zmin);

		glVertex3f(xmin, ymax, zmin);
		glVertex3f(xmax, ymax, zmin);

		glVertex3f(xmin, ymin, zmin);
		glVertex3f(xmin, ymax, zmin);

		glVertex3f(xmax, ymin, zmin);
		glVertex3f(xmax, ymax, zmin);


		glVertex3f(xmax, ymin, zmax);
		glVertex3f(xmax, ymin, zmin);

		glVertex3f(xmax, ymax, zmax);
		glVertex3f(xmax, ymax, zmin);

		glVertex3f(xmin, ymin, zmax);
		glVertex3f(xmin, ymin, zmin);

		glVertex3f(xmin, ymax, zmax);
		glVertex3f(xmin, ymax, zmin);
	glEnd();
	glEnable(GL_LIGHTING);
};
#endif