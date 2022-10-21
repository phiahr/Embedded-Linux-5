#include <iostream>
#include <sys/time.h>	/* time*/
#include <sys/ioctl.h>	/* ioctl */

/* cout */
using namespace std;

int main(void) {
    // Create two input vectors
    int i, j;
struct timeval start, end; 
    double tdiff;
    //const int VECTOR_SIZE=1024*1024;
    const int VECTOR_SIZE=1024*1024; // 5 times larger array size
    int *inputA = new int[VECTOR_SIZE];
    int *inputB = new int[VECTOR_SIZE];
    int *outputC = new int[VECTOR_SIZE];

    

    for(i = 0; i < VECTOR_SIZE; i++) {inputA[i] = i;inputB[i] = VECTOR_SIZE -i;}
    gettimeofday(&start, NULL);
    // Computes vector_add
    for(i = 0; i < VECTOR_SIZE; i++) outputC[i] = inputA[i] + inputB[i];
    // Display last 100 results to the screen
gettimeofday(&end, NULL);
    for (int i = 0; i < 100 /*VECTOR_SIZE*/; i++) cout << inputA[VECTOR_SIZE-100+i] << " + " <<     inputB[VECTOR_SIZE-100+i] << " = " << outputC[i] << "\n";
    
    delete[] inputA;
    delete[] inputB; 
    delete[] outputC; 

    

    tdiff = (double)(1000*(end.tv_sec-start.tv_sec))+((end.tv_usec-start.tv_usec)/1000.0); 
    cout << "Execution time: " << tdiff << "ms" << endl; 
    return 0;
}
