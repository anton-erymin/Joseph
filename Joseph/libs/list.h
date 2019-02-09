#include "stdafx.h"
#include "md2model.h"
#include "anim_geometry.h"

struct element
{
	AnimatedGeometry *geom;
	bool killed;
	element *next;
};

element *first, *last;

void InitList()
{
	first = NULL;
	last = NULL;
}

void Insert(MD2Model *m, float *p, float *r, int ff, int lf, int cf)
{
	element *e = new element;
	if (!first)
	{	
		first = e;
		last = e;
		first->next = first;
	}
	else
	{
		last->next = e;
		e->next = first;
		last = e;
	}

		e->geom = new AnimatedGeometry(m);
		e->geom->SetPosition(p[0], p[1], p[2]);
		e->geom->SetRotation(r[0], r[1], r[2]);
		e->geom->SetAnimation(ff, lf, cf, true);
		e->killed = false;
}


void Delete(element *de, element *re)
{
	element *ptr = de->next;
	delete de;
	re->next = ptr;
}