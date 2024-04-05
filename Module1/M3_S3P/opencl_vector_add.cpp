#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <chrono> // For measuring execution time

#define PRINT 1 // Define printing option

int SZ = 100000000; // Size of vectors

int *v1, *v2, *v_out; // Arrays for input and output vectors

cl_mem bufV1, bufV2, bufV_out; // OpenCL memory objects

cl_device_id device_id; // Device ID

cl_context context; // Context

cl_program program; // Program object

cl_kernel kernel; // Kernel object

cl_command_queue queue; // Command queue

cl_event event = NULL; // Event object for synchronization

int err; // Error code

cl_device_id create_device(); // Function to create OpenCL device

void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname); // Function to setup OpenCL device, context, queue, and kernel

cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename); // Function to build OpenCL program

void setup_kernel_memory(); // Function to setup memory objects for kernel

void copy_kernel_args(); // Function to copy kernel arguments

void free_memory(); // Function to release memory

void init(int *&A, int size); // Function to initialize array with random values

void print(int *A, int size); // Function to print array

int main(int argc, char **argv) {
    // Change the size of vectors if provided as command line argument
    if (argc > 1) {
        SZ = atoi(argv[1]);
    }

    // Initialize input and output vectors
    init(v1, SZ);
    init(v2, SZ);
    init(v_out, SZ);

    // Define global work size for OpenCL kernel
    size_t global[1] = {(size_t)SZ};

    // Print input vectors if PRINT is enabled
    print(v1, SZ);
    print(v2, SZ);

    // Setup OpenCL device, context, queue, and kernel
    setup_openCL_device_context_queue_kernel((char *)"./vector_ops_ocl.cl", (char *)"vector_add_ocl");

    // Setup memory objects for kernel
    setup_kernel_memory();

    // Copy kernel arguments
    copy_kernel_args();

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Enqueue kernel for execution
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global, NULL, 0, NULL, &event);
    clWaitForEvents(1, &event);

    // Read output buffer back to host memory
    clEnqueueReadBuffer(queue, bufV_out, CL_TRUE, 0, SZ * sizeof(int), &v_out[0], 0, NULL, NULL);

    // Print output vector
    print(v_out, SZ);

    // Stop measuring time
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = stop - start;

    // Print kernel execution time
    printf("Kernel Execution Time: %f ms\n", elapsed_time.count());

    // Free allocated memory
    free_memory();
}

// Function to initialize array with random values
void init(int *&A, int size) {
    A = (int *)malloc(sizeof(int) * size);

    for (long i = 0; i < size; i++) {
        A[i] = rand() % 100; 
    }
}

// Function to print array
void print(int *A, int size)
{
    // Check if printing is enabled
    if (PRINT == 0)
    {
        return;
    }

    // Print first 5 and last 5 elements if size is large
    if (PRINT == 1 && size > 15)
    {
        for (long i = 0; i < 5; i++)
        {                      
            printf("%d ", A[i]);         
        }
        printf(" ..... ");
        for (long i = size - 5; i < size; i++)
        {                        
            printf("%d ", A[i]); 
        }
    }
    else
    {
        // Print all elements
        for (long i = 0; i < size; i++)
        {                        
            printf("%d ", A[i]); 
        }
    }
    printf("\n----------------------------\n");
}

// Function to release memory
void free_memory() {
    
    clReleaseMemObject(bufV1);
    clReleaseMemObject(bufV2);
    clReleaseMemObject(bufV_out);

  
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);

    free(v1);
    free(v2);
    free(v_out); 
}

// Function to copy kernel arguments
void copy_kernel_args()
{
    clSetKernelArg(kernel, 0, sizeof(int), (void *)&SZ);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufV1);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufV2);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&bufV_out);

    if (err < 0)
    {
        perror("Couldn't create a kernel argument");
        printf("error = %d", err);
        exit(1);
    }
}

// Function to setup memory objects for kernel
void setup_kernel_memory() {
    bufV1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SZ * sizeof(int), NULL, NULL);
    bufV2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SZ * sizeof(int), NULL, NULL);
    bufV_out = clCreateBuffer(context, CL_MEM_READ_WRITE, SZ * sizeof(int), NULL, NULL);


    clEnqueueWriteBuffer(queue, bufV1, CL_TRUE, 0, SZ * sizeof(int), &v1[0], 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufV2, CL_TRUE, 0, SZ * sizeof(int), &v2[0], 0, NULL, NULL);
}

// Function to setup OpenCL device, context, queue, and kernel
void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname)
{
    // Create OpenCL device
    device_id = create_device();
    cl_int err;

    // Create OpenCL context
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err < 0)
    {
        perror("Couldn't create a context");
        exit(1);
    }

    // Build OpenCL program
    program = build_program(context, device_id, filename);

    // Create command queue
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
    if (err < 0)
    {
        perror("Couldn't create a command queue");
        exit(1);
    };

    // Create kernel
    kernel = clCreateKernel(program, kernelname, &err);
    if (err < 0)
    {
        perror("Couldn't create a kernel");
        printf("error =%d", err);
        exit(1);
    };
}

// Function to build OpenCL program
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename) {

    cl_program program;
    FILE *program_handle;
    char *program_buffer, *program_log;
    size_t program_size, log_size;

    // Read OpenCL program from file
    program_handle = fopen(filename, "r");
    if (program_handle == NULL)
    {
        perror("Couldn't find the program file");
        exit(1);
    }
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char *)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);

    // Create OpenCL program object
    program = clCreateProgramWithSource(ctx, 1,
                                        (const char **)&program_buffer, &program_size, &err);
    if (err < 0)
    {
        perror("Couldn't create the program");
        exit(1);
    }
    free(program_buffer);

    // Build OpenCL program
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err < 0)
    {
        // If build fails, print build log
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              0, NULL, &log_size);
        program_log = (char *)malloc(log_size + 1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              log_size + 1, program_log, NULL);
        printf("%s\n", program_log);
        free(program_log);
        exit(1);
    }

    return program;
}

// Function to create OpenCL device
cl_device_id create_device() {

   cl_platform_id platform;
   cl_device_id dev;
   int err;

   // Get OpenCL platform
   err = clGetPlatformIDs(1, &platform, NULL);
   if(err < 0) {
      perror("Couldn't identify a platform");
      exit(1);
   } 

   // Get OpenCL device (GPU preferred, fallback to CPU)
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      printf("GPU not found\n");
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if(err < 0) {
      perror("Couldn't access any devices");
      exit(1);   
   }

   return dev;
}
 