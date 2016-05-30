#include <stdio.h>
#include <stdlib.h>
//#define CSET_THREADS_PRE printf("%s\n", __func__);
//#define CSET_THREADS_POST printf("%s\n", __func__);
#include <cset/test/test.h>
#include <cset/threads/threads.h>

int main(int argc, char **argv)
{
	cl_device_id device_id = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int clerr;

	clerr = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	printf("%d %s\n", platform_id, cset_threads_opencl_err_to_string(clerr));
	clerr = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
	printf("%s\n", cset_threads_opencl_err_to_string(clerr));
	return EXIT_SUCCESS;
}
