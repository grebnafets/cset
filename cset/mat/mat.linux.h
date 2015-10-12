/*
 * This is orginized in column-major matrix ordering, same as OpenGL.
 * (It means you don't have to use transpose before you push the matrix.)
 * */

#ifndef MAT_LINUX
#define MAT_LINUX 1

#include <math.h>
#include <cset/mem/mem.h>
#include <stdarg.h>

/* helper functions {{{ */
float fdeg(float rad) {return rad * 180.0 / M_PI;}
double ddeg(double rad) {return rad * 180.0 / M_PI;}
float frad(float deg) {return deg * M_PI / 180;}
double drad(double deg) {return deg * M_PI / 180;}
void matf_swap(float *mat, size_t i1, size_t i2)
{
	float tmp;
	tmp = mat[i1];
	mat[i1] = mat[i2];
	mat[i2] = tmp;
}
void matd_swap(double *mat, size_t i1, size_t i2)
{
	double tmp;
	tmp = mat[i1];
	mat[i1] = mat[i2];
	mat[i2] = tmp;
}
/* }}} */

/* mat to string {{{ */
char mat3fmt[] = "[\n\t%.5f, %.5f, %.5f\n\t%.5f, %.5f, %.5f\n\t%.5f, %.5f, %.5f\n]\n";
char mat4fmt[] = "[\n\t%.5f, %.5f, %.5f, %.5f\n\t%.5f, %.5f, %.5f, %.5f\n\t%.5f, %.5f, %.5f, %.5f\n\t%.5f, %.5f, %.5f, %.5f\n]\n";

char *mat3f_toString(float *mat)
{
	char *buf = (char *)mem.xc(1024, sizeof(char));
	sprintf(
		buf, mat3fmt,
		mat[0], mat[1], mat[2],
		mat[3], mat[4], mat[5],
		mat[6], mat[7], mat[8]
	);
	return buf;	
}

char *mat3d_toString(double *mat)
{
	char *buf = (char *)mem.xc(1024, sizeof(char));
	sprintf(
		buf, mat3fmt,
		mat[0], mat[1], mat[2],
		mat[3], mat[4], mat[5],
		mat[6], mat[7], mat[8]
	);
	return buf;	
}

char *mat4f_toString(float *mat)
{
	char *buf = (char *)mem.xc(1024, sizeof(char));
	sprintf(
		buf, mat4fmt,
		mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]
	);
	return buf;	
}

char *mat4d_toString(double *mat)
{
	char *buf = (char *)mem.xc(1024, sizeof(char));
	sprintf(
		buf, mat4fmt,
		mat[0], mat[1], mat[2], mat[3],
		mat[4], mat[5], mat[6], mat[7],
		mat[8], mat[9], mat[10], mat[11],
		mat[12], mat[13], mat[14], mat[15]
	);
	return buf;	
}
/* }}} */

/* test matrix {{{ */
float *mat3f_test()
{
	float *mat = (float *)mem.xc(9, sizeof(float));
	mat[0] = 1;
	mat[1] = 2;
	mat[2] = 3;
	mat[3] = 4;
	mat[4] = 5;
	mat[5] = 6;
	mat[6] = 7;
	mat[7] = 8;
	mat[8] = 9;
	return mat;
}

double *mat3d_test()
{
	double *mat = (double *)mem.xc(9, sizeof(double));
	mat[0] = 1;
	mat[1] = 2;
	mat[2] = 3;
	mat[3] = 4;
	mat[4] = 5;
	mat[5] = 6;
	mat[6] = 7;
	mat[7] = 8;
	mat[8] = 9;
	return mat;
}

float *mat4f_test()
{
	float *mat = (float *)mem.xc(16, sizeof(float));
	mat[0] = 1;
	mat[1] = 2;
	mat[2] = 3;
	mat[3] = 4;
	mat[4] = 5;
	mat[5] = 6;
	mat[6] = 7;
	mat[7] = 8;
	mat[8] = 9;
	mat[9] = 10;
	mat[10] = 11;
	mat[11] = 12;
	mat[12] = 13;
	mat[13] = 14;
	mat[14] = 15;
	mat[15] = 16;
	return mat;
}

double *mat4d_test()
{
	double *mat = (double *)mem.xc(16, sizeof(double));
	mat[0] = 1;
	mat[1] = 2;
	mat[2] = 3;
	mat[3] = 4;
	mat[4] = 5;
	mat[5] = 6;
	mat[6] = 7;
	mat[7] = 8;
	mat[8] = 9;
	mat[9] = 10;
	mat[10] = 11;
	mat[11] = 12;
	mat[12] = 13;
	mat[13] = 14;
	mat[14] = 15;
	mat[15] = 16;
	return mat;
}
/* }}} */

/* mat indentity {{{ */

float *mat3f_indentity()
{
	float *mat4 = (float *)mem.xc(9, sizeof(float));
	mat4[0] = 1;
	mat4[4] = 1;
	mat4[8] = 1;
	return mat4;
}

double *mat3d_indentity()
{
	double *mat4 = (double *)mem.xc(9, sizeof(double));
	mat4[0] = 1;
	mat4[4] = 1;
	mat4[8] = 1;
	return mat4;
}

float *mat4f_indentity()
{
	float *mat4 = (float *)mem.xc(16, sizeof(float));
	mat4[0]  = 1;
	mat4[5]  = 1;
	mat4[10] = 1;
	mat4[15] = 1;
	return mat4;
}

double *mat4d_indentity()
{
	double *mat4 = (double *)mem.xc(16, sizeof(double));
	mat4[0]  = 1;
	mat4[5]  = 1;
	mat4[10] = 1;
	mat4[15] = 1;
	return mat4;
}
/* }}} */

/* scale {{{ */
void mat3f_setScale(float *mat3, float x, float y)
{
	mat3[0] = x;
	mat3[4] = y;
	mat3[8] = 1;
}

void mat3d_setScale(double *mat3, double x, double y)
{
	mat3[0] = x;
	mat3[4] = y;
	mat3[8] = 1;
}

float *mat3f_makeScale(float x, float y)
{
	float *mat3 = mat3f_indentity();
	mat3f_setScale(mat3, x, y);
	return mat3;
}

double *mat3d_makeScale(double x, double y)
{
	double *mat3 = mat3d_indentity();
	mat3d_setScale(mat3, x, y);
	return mat3;
}

void mat4f_setScale(float *mat4, float x, float y, float z)
{
	mat4[0]  = x;
	mat4[5]  = y;
	mat4[10] = z;
	mat4[15] = 1;
}

void mat4d_setScale(double *mat4, double x, double y, double z)
{
	mat4[0]  = x;
	mat4[5]  = y;
	mat4[10] = z;
	mat4[15] = 1;
}

float *mat4f_makeScale(float x, float y, float z)
{
	float *mat4 = mat4f_indentity();
	mat4f_setScale(mat4, x, y, z);
	return mat4;
}

double *mat4d_makeScale(double x, double y, double z)
{
	double *mat4 = mat4d_indentity();
	mat4d_setScale(mat4, x, y, z);
	return mat4;
}
/* }}} */

/* translation {{{ */
void mat3f_setTranslation(float *mat3, float x, float y)
{
	mat3[6] = x;
	mat3[7] = y;
}

void mat3d_setTranslation(double *mat3, double x, double y)
{
	mat3[6] = x;
	mat3[7] = y;
}

float *mat3f_makeTranslation(float x, float y)
{
	float *mat3 = mat3f_indentity();
	mat3f_setTranslation(mat3, x, y);
	return mat3;
}

double *mat3d_makeTranslation(double x, double y)
{
	double *mat3 = mat3d_indentity();
	mat3d_setTranslation(mat3, x, y);
	return mat3;
}

void mat4f_setTranslation(float *mat4, float x, float y, float z)
{
	mat4[12] = x;
	mat4[13] = y;
	mat4[14] = z;
}

void mat4d_setTranslation(double *mat4, double x, double y, double z)
{
	mat4[12] = x;
	mat4[13] = y;
	mat4[14] = z;
}

float *mat4f_makeTranslation(float x, float y, float z)
{
	float *mat4 = mat4f_indentity();
	mat4f_setTranslation(mat4, x, y, z);
	return mat4;
}

double *mat4d_makeTranslation(double x, double y, double z)
{
	double *mat4 = mat4d_indentity();
	mat4d_setTranslation(mat4, x, y, z);
	return mat4;
}
/* }}} */

/* transpose {{{ */

void mat3f_transpose(float *mat)
{
	matf_swap(mat, 1, 3);
	matf_swap(mat, 2, 6);
	matf_swap(mat, 5, 7);
}

void mat3d_transpose(double *mat)
{
	matd_swap(mat, 1, 3);
	matd_swap(mat, 2, 6);
	matd_swap(mat, 5, 7);
}

void mat4f_transpose(float *mat)
{
	matf_swap(mat, 1, 4);
	matf_swap(mat, 2, 8);
	matf_swap(mat, 3, 12);
	matf_swap(mat, 6, 9);
	matf_swap(mat, 7, 13);
	matf_swap(mat, 11, 14);
}

void mat4d_transpose(double *mat)
{
	matd_swap(mat, 1, 4);
	matd_swap(mat, 2, 8);
	matd_swap(mat, 3, 12);
	matd_swap(mat, 6, 9);
	matd_swap(mat, 7, 13);
	matd_swap(mat, 11, 14);
}

/* }}} */

/* rotatation {{{ */

void mat3f_setRotation(float *mat, float angle)
{
	float c = cos(frad(angle));
	float s = sin(frad(angle));
	mat[0] = c;
	mat[1] = -s;
	mat[3] = s;
	mat[4] = c;
}

float *mat3f_makeRotation(float angle)
{
	float *mat = mat3f_indentity();
	mat3f_setRotation(mat, angle);
	return mat;
}

void mat3d_setRotation(double *mat, double angle)
{
	double c = cos(drad(angle));
	double s = sin(drad(angle));
	mat[0] = c;
	mat[1] = -s;
	mat[3] = s;
	mat[4] = c;
}

double *mat3d_makeRotation(double angle)
{
	double *mat = mat3d_indentity();
	mat3d_setRotation(mat, angle);
	return mat;
}

void mat4f_setXRotation(float *mat, float angle)
{
	float c = cos(frad(angle));
	float s = sin(frad(angle));
	mat[5]  = c;
	mat[6]  = s;
	mat[9]  = -s;
	mat[10] = c;
}

float *mat4f_makeXRotation(float angle)
{
	float *mat = mat4f_indentity();
	mat4f_setXRotation(mat, angle);
	return mat;
}

void mat4d_setXRotation(double *mat, double angle)
{
	double c = cos(drad(angle));
	double s = sin(drad(angle));
	mat[5]  = c;
	mat[6]  = s;
	mat[9]  = -s;
	mat[10] = c;
}

double *mat4d_makeXRotation(double angle)
{
	double *mat = mat4d_indentity();
	mat4d_setXRotation(mat, angle);
	return mat;
}

void mat4f_setYRotation(float *mat, float angle)
{
	float c = cos(frad(angle));
	float s = sin(frad(angle));
	mat[0]  = c;
	mat[2]  = -s;
	mat[8]  = s;
	mat[10] = c;
}

float *mat4f_makeYRotation(float angle)
{
	float *mat = mat4f_indentity();
	mat4f_setYRotation(mat, angle);
	return mat;
}

void mat4d_setYRotation(double *mat, double angle)
{
	double c = cos(drad(angle));
	double s = sin(drad(angle));
	mat[0]  = c;
	mat[2]  = -s;
	mat[8]  = s;
	mat[10] = c;
}

double *mat4d_makeYRotation(double angle)
{
	double *mat = mat4d_indentity();
	mat4d_setYRotation(mat, angle);
	return mat;
}

void mat4f_setZRotation(float *mat, float angle)
{
	float c = cos(frad(angle));
	float s = sin(frad(angle));
	mat[0] = c;
	mat[1] = s;
	mat[4] = -s;
	mat[5] = c;
}

float *mat4f_makeZRotation(float angle)
{
	float *mat = mat4f_indentity();
	mat4f_setZRotation(mat, angle);
	return mat;
}

void mat4d_setZRotation(double *mat, double angle)
{
	double c = cos(drad(angle));
	double s = sin(drad(angle));
	mat[0] = c;
	mat[1] = s;
	mat[4] = -s;
	mat[5] = c;
}

double *mat4d_makeZRotation(double angle)
{
	double *mat = mat4d_indentity();
	mat4d_setZRotation(mat, angle);
	return mat;
}
/* }}} */

#endif /* MAT_LINUX */
