CC=g++
#CC=arm-linux-gnueabihf-g++

all: vector_add_c vector_add_opencl

vector_add_opencl: common.o vector_add_opencl.o
	$(CC) -o vector_add_opencl common.o vector_add_opencl.o -lOpenCL

vector_add_opencl.o: vector_add_opencl.cpp 
	$(CC) -c vector_add_opencl.cpp -lOpenCL
	
common.o: common.cpp 
	$(CC) -c common.cpp -lOpenCL

vector_add_c: vector_add_c.cpp 
	$(CC) -o vector_add_c vector_add_c.cpp

clean: 
	rm -r *.o vector_add_opencl vector_add_c
