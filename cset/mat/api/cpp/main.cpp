#define TEST_SILENT_IF_NO_TESTS
#include <cset/test/test.h>
#include <cset/f/f.h>
#include <cset/mat/mat.h>

void show()
{
	float *mat4 = mat4f_makeZRotation(0.45);
	char *mat = mat4f_toString(mat4);
	fprintf(stdout, "%s", mat);
	free(mat4);
	free(mat);
}

/* transpose {{{ */

void test_mat3Transpose()
{
	float *mat3f = mat3f_test();
	mat3f_transpose(mat3f);
	double *mat3d = mat3d_test();
	mat3d_transpose(mat3d);
	char *matf = mat3f_toString(mat3f);
	char *matd = mat3d_toString(mat3d);
	fput("diff/transpose/mat3f.r", matf);
	fput("diff/transpose/mat3d.r", matd);
	test_add_diff("diff/transpose/mat3.e", "diff/transpose/mat3f.r");
	test_add_diff("diff/transpose/mat3.e", "diff/transpose/mat3d.r");
	free(mat3f);
	free(mat3d);
	free(matf);
	free(matd);
}

void test_mat4Transpose()
{
	float *mat4f = mat4f_test();
	mat4f_transpose(mat4f);
	double *mat4d = mat4d_test();
	mat4d_transpose(mat4d);
	char *matf = mat4f_toString(mat4f);
	char *matd = mat4d_toString(mat4d);
	fput("diff/transpose/mat4f.r", matf);
	fput("diff/transpose/mat4d.r", matd);
	test_add_diff("diff/transpose/mat4.e", "diff/transpose/mat4f.r");
	test_add_diff("diff/transpose/mat4.e", "diff/transpose/mat4d.r");
	free(mat4f);
	free(mat4d);
	free(matf);
	free(matd);
}

/* }}} */

/* translation {{{ */
void test_mat3Translation()
{
	float *mat3f  = mat3f_makeTranslation(2.3, 3.2);
	double *mat3d = mat3d_makeTranslation(2.3, 3.2);
	char *matf = mat3f_toString(mat3f);
	char *matd = mat3d_toString(mat3d);
	fput("diff/translation/mat3f.r", matf);
	fput("diff/translation/mat3d.r", matd);
	test_add_diff("diff/translation/mat3.e", "diff/translation/mat3f.r");
	test_add_diff("diff/translation/mat3.e", "diff/translation/mat3d.r");
	free(mat3f);
	free(mat3d);
	free(matf);
	free(matd);
}

void test_mat4Translation()
{
	float *mat4f = mat4f_makeTranslation(2.3, 3.2, 5.6);
	double *mat4d = mat4d_makeTranslation(2.3, 3.2, 5.6);
	char *matf = mat4f_toString(mat4f);
	char *matd = mat4d_toString(mat4d);
	fput("diff/translation/mat4f.r", matf);
	fput("diff/translation/mat4d.r", matd);
	test_add_diff("diff/translation/mat4.e", "diff/translation/mat4f.r");
	test_add_diff("diff/translation/mat4.e", "diff/translation/mat4d.r");
	free(mat4f);
	free(mat4d);
	free(matf);
	free(matd);
}
/* }}} */

/* scale {{{ */
void test_mat3Scale()
{
	float *mat3f  = mat3f_makeScale(2.3, 3.2);
	double *mat3d = mat3d_makeScale(2.3, 3.2);
	char *matf = mat3f_toString(mat3f);
	char *matd = mat3d_toString(mat3d);
	fput("diff/scale/mat3f.r", matf);
	fput("diff/scale/mat3d.r", matd);
	test_add_diff("diff/scale/mat3.e", "diff/scale/mat3f.r");
	test_add_diff("diff/scale/mat3.e", "diff/scale/mat3d.r");
	free(mat3f);
	free(mat3d);
	free(matf);
	free(matd);
}

void test_mat4Scale()
{
	float *mat4f = mat4f_makeScale(2.3, 3.2, 5.6);
	double *mat4d = mat4d_makeScale(2.3, 3.2, 5.6);
	char *matf = mat4f_toString(mat4f);
	char *matd = mat4d_toString(mat4d);
	fput("diff/scale/mat4f.r", matf);
	fput("diff/scale/mat4d.r", matd);
	test_add_diff("diff/scale/mat4.e", "diff/scale/mat4f.r");
	test_add_diff("diff/scale/mat4.e", "diff/scale/mat4d.r");
	free(mat4f);
	free(mat4d);
	free(matf);
	free(matd);
}
/* }}} */

/* rotation {{{ */
void test_mat3Rotation()
{
	float *mat3f  = mat3f_makeRotation(0.45);
	double *mat3d = mat3d_makeRotation(0.45);
	char *matf = mat3f_toString(mat3f);
	char *matd = mat3d_toString(mat3d);
	fput("diff/rotation/mat3f.r", matf);
	fput("diff/rotation/mat3d.r", matd);
	test_add_diff("diff/rotation/mat3.e", "diff/rotation/mat3f.r");
	test_add_diff("diff/rotation/mat3.e", "diff/rotation/mat3d.r");
	free(mat3f);
	free(mat3d);
	free(matf);
	free(matd);
}

void test_mat4XRotation()
{
	float *mat4f  = mat4f_makeXRotation(0.45);
	double *mat4d = mat4d_makeXRotation(0.45);
	char *matf = mat4f_toString(mat4f);
	char *matd = mat4d_toString(mat4d);
	fput("diff/rotation/mat4fx.r", matf);
	fput("diff/rotation/mat4dx.r", matd);
	test_add_diff("diff/rotation/mat4x.e", "diff/rotation/mat4fx.r");
	test_add_diff("diff/rotation/mat4x.e", "diff/rotation/mat4dx.r");
	free(mat4f);
	free(mat4d);
	free(matf);
	free(matd);
}

void test_mat4YRotation()
{
	float *mat4f  = mat4f_makeYRotation(0.45);
	double *mat4d = mat4d_makeYRotation(0.45);
	char *matf = mat4f_toString(mat4f);
	char *matd = mat4d_toString(mat4d);
	fput("diff/rotation/mat4fy.r", matf);
	fput("diff/rotation/mat4dy.r", matd);
	test_add_diff("diff/rotation/mat4y.e", "diff/rotation/mat4fy.r");
	test_add_diff("diff/rotation/mat4y.e", "diff/rotation/mat4dy.r");
	free(mat4f);
	free(mat4d);
	free(matf);
	free(matd);
}

void test_mat4ZRotation()
{
	float *mat4f  = mat4f_makeZRotation(0.45);
	double *mat4d = mat4d_makeZRotation(0.45);
	char *matf = mat4f_toString(mat4f);
	char *matd = mat4d_toString(mat4d);
	fput("diff/rotation/mat4fz.r", matf);
	fput("diff/rotation/mat4dz.r", matd);
	test_add_diff("diff/rotation/mat4z.e", "diff/rotation/mat4fz.r");
	test_add_diff("diff/rotation/mat4z.e", "diff/rotation/mat4dz.r");
	free(mat4f);
	free(mat4d);
	free(matf);
	free(matd);
}

/* }}} */

/* indentity {{{ */
void test_mat4Indentity()
{
	float *mat4f = mat4f_indentity();
	double *mat4d = mat4d_indentity();
	char *matf = mat4f_toString(mat4f);
	char *matd = mat4d_toString(mat4d);
	fput("diff/indentity/mat4f.r", matf);
	fput("diff/indentity/mat4d.r", matd);
	test_add_diff("diff/indentity/mat4.e", "diff/indentity/mat4f.r");
	test_add_diff("diff/indentity/mat4.e", "diff/indentity/mat4d.r");
	free(mat4f);
	free(mat4d);
	free(matf);
	free(matd);
}

void test_mat3Indentity()
{
	float *mat3f  = mat3f_indentity();
	double *mat3d = mat3d_indentity();
	char *matf = mat3f_toString(mat3f);
	char *matd = mat3d_toString(mat3d);
	fput("diff/indentity/mat3f.r", matf);
	fput("diff/indentity/mat3d.r", matf);
	test_add_diff("diff/indentity/mat3.e", "diff/indentity/mat3f.r");
	test_add_diff("diff/indentity/mat3.e", "diff/indentity/mat3d.r");
	free(mat3f);
	free(mat3d);
	free(matf);
	free(matd);
}
/* }}} */

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "mat");
	test_mat4Indentity();
	test_mat3Indentity();
	test_mat4Scale();
	test_mat3Scale();
	test_mat4Translation();
	test_mat3Translation();
	test_mat4Transpose();
	test_mat3Transpose();
	test_mat4XRotation();
	test_mat4YRotation();
	test_mat4ZRotation();
	test_mat3Rotation();
	test_diff();
	/* show(); */
	return EXIT_SUCCESS;
}
