#include "stdafx.h"

#define DEGREESPERRAD	57.295780490442968321226

typedef struct
{
	float m[16];
} Matrix;

void MatrixIdentity(Matrix *res)
{
	for (int i = 0; i < 16; i++)
		res->m[i] = 0.0f;

	res->m[0] = 1.0f;
	res->m[5] = 1.0f;
	res->m[10] = 1.0f;
	res->m[15] = 1.0f;
}

void MatrixTranslation(Matrix *res, float xt, float yt, float zt)
{
	MatrixIdentity(res);
	res->m[3] = xt;
	res->m[7] = yt;
	res->m[11] = zt;
}

void MatrixRotation(Matrix *res, float xr, float yr, float zr)
{
	/*xr *= DEGREESPERRAD;
	yr *= DEGREESPERRAD;
	zr *= DEGREESPERRAD;

	glPushMatrix();
	glLoadIdentity();
	glRotatef(xr, 1.0f, 0.0f, 0.0f); 
	glRotatef(yr, 0.0f, 1.0f, 0.0f);
	glRotatef(zr, 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, res->m);
	glPopMatrix();*/

	float A = cos(xr);
	float B = sin(xr);
	float C = cos(yr);
	float D = sin(yr);
	float E = cos(zr);
	float F = sin(zr);

	float AD = A*D;
	float BD = B*D;

	for (int i = 0; i < 16; i++)
		res->m[i] = 0.0f;

	res->m[0] = C*E;
	res->m[1] = -C*F;
	res->m[2] = -D;
	res->m[4] = -BD*E + A*F;
	res->m[5] = BD*F + A*E;
	res->m[6] = -B*C;
	res->m[8] = AD*E + B*F;
	res->m[9] = -AD*F + B*E;
	res->m[10] = A*C;
	res->m[15] = 1.0f;

	/*res->m[0] = C*E;
	res->m[4] = -C*F;
	res->m[8] = -D;
	res->m[1] = -BD*E + A*F;
	res->m[5] = BD*F + A*E;
	res->m[9] = -B*C;
	res->m[2] = AD*E + B*F;
	res->m[10] = -AD*F + B*E;
	res->m[10] = A*C;
	res->m[15] = 1.0f;*/
}

void MatrixMult(Matrix *res, Matrix *m1, Matrix *m2)
{
	/*for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			res->m[4*j+i] = m1->m[4*(0)+i]*m2->m[4*j+0] + 
									m1->m[4*(1)+i]*m2->m[4*j+1] + 
									m1->m[4*(2)+i]*m2->m[4*j+2] + 
									m1->m[4*(3)+i]*m2->m[4*j+3];*/

	res->m[0] = m1->m[0]*m2->m[0] + m1->m[1]*m2->m[4] + m1->m[2]*m2->m[8] + m1->m[3]*m2->m[12];
	res->m[1] = m1->m[0]*m2->m[1] + m1->m[1]*m2->m[5] + m1->m[2]*m2->m[9] + m1->m[3]*m2->m[13];
	res->m[2] = m1->m[0]*m2->m[2] + m1->m[1]*m2->m[6] + m1->m[2]*m2->m[10] + m1->m[3]*m2->m[14];
	res->m[3] = m1->m[0]*m2->m[3] + m1->m[1]*m2->m[7] + m1->m[2]*m2->m[11] + m1->m[3]*m2->m[15];

	res->m[4] = m1->m[1]*m2->m[0] + m1->m[5]*m2->m[4] + m1->m[6]*m2->m[8] + m1->m[7]*m2->m[12];
	res->m[5] = m1->m[1]*m2->m[1] + m1->m[5]*m2->m[5] + m1->m[6]*m2->m[9] + m1->m[7]*m2->m[13];
	res->m[6] = m1->m[1]*m2->m[2] + m1->m[5]*m2->m[6] + m1->m[6]*m2->m[10] + m1->m[7]*m2->m[14];
	res->m[7] = m1->m[1]*m2->m[3] + m1->m[5]*m2->m[7] + m1->m[6]*m2->m[11] + m1->m[7]*m2->m[15];

	res->m[8] = m1->m[2]*m2->m[0] + m1->m[9]*m2->m[4] + m1->m[10]*m2->m[8] + m1->m[11]*m2->m[12];
	res->m[9] = m1->m[2]*m2->m[1] + m1->m[9]*m2->m[5] + m1->m[10]*m2->m[9] + m1->m[11]*m2->m[13];
	res->m[10] = m1->m[2]*m2->m[2] + m1->m[9]*m2->m[6] + m1->m[10]*m2->m[10] + m1->m[11]*m2->m[14];
	res->m[11] = m1->m[2]*m2->m[3] + m1->m[9]*m2->m[7] + m1->m[10]*m2->m[11] + m1->m[11]*m2->m[15];

	res->m[12] = m1->m[3]*m2->m[0] + m1->m[13]*m2->m[4] + m1->m[14]*m2->m[8] + m1->m[15]*m2->m[12];
	res->m[13] = m1->m[3]*m2->m[1] + m1->m[13]*m2->m[5] + m1->m[14]*m2->m[9] + m1->m[15]*m2->m[13];
	res->m[14] = m1->m[3]*m2->m[2] + m1->m[13]*m2->m[6] + m1->m[14]*m2->m[10] + m1->m[15]*m2->m[14];
	res->m[15] = m1->m[3]*m2->m[3] + m1->m[13]*m2->m[7] + m1->m[14]*m2->m[11] + m1->m[15]*m2->m[15];

	/*glPushMatrix();
	glLoadIdentity();
	glLoadMatrixf(m1->m);
	glMultMatrixf(m2->m);
	glGetFloatv(GL_MODELVIEW_MATRIX, res->m);
	glPopMatrix();*/
}

void MatrixApply(Matrix m, float *x, float *y, float *z)
{
	float xx = *x;
	float yy = *y;
	float zz = *z;

	/**x = m.m[0]*xx + m.m[4]*yy + m.m[8]*zz + m.m[12];
	*y = m.m[1]*xx + m.m[5]*yy + m.m[9]*zz + m.m[13];
	*z = m.m[2]*xx + m.m[6]*yy + m.m[10]*zz + m.m[14];*/

	*x = m.m[0]*xx + m.m[1]*yy + m.m[2]*zz + m.m[3];
	*y = m.m[4]*xx + m.m[5]*yy + m.m[6]*zz + m.m[7];
	*z = m.m[8]*xx + m.m[9]*yy + m.m[10]*zz + m.m[11];
}
void MatrixInvApply(Matrix m, float *x, float *y, float *z)
{
	float xx = *x;
	float yy = *y;
	float zz = *z;

	xx = xx - m.m[3];
	yy = yy - m.m[7];
	zz = zz - m.m[11];

	//*x = m.m[0]*xx + m.m[1]*yy + m.m[2]*zz;// + m.m[3];
	//*y = m.m[4]*xx + m.m[5]*yy + m.m[6]*zz;// + m.m[7];
	//*z = m.m[8]*xx + m.m[9]*yy + m.m[10]*zz;// + m.m[11];	

	*x = m.m[0]*xx + m.m[4]*yy + m.m[8]*zz;// + m.m[3];
	*y = m.m[1]*xx + m.m[5]*yy + m.m[9]*zz;// + m.m[7];
	*z = m.m[2]*xx + m.m[6]*yy + m.m[10]*zz;// + m.m[11];	

	/**x = *x - m.m[12];
	*y = *y - m.m[13];
	*z = *z - m.m[14];*/
}