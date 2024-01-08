#include <stdio.h>
//#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MatrixSize 16384
#define BlockSize 16384

float matBlockT(float A[MatrixSize][MatrixSize]){
    //C allocating matrix that will be returned
    //clock_t t1;
    //double wt1,wt2;
    struct timespec start_time, end_time; 
    int bi,bj,i,j,rank, size;
    float ABT[MatrixSize][MatrixSize];
    for ( i = 0; i<MatrixSize; i++){
        for (j=0; j<MatrixSize; j++){
            ABT[i][j]= 0;
        }
    } 

    //int nBlocks = 9;
    //int blockSize = MatrixSize/nBlocks;


    //function work
    clock_gettime(CLOCK_REALTIME, &start_time);
    for(bi = 0; bi < MatrixSize; bi += BlockSize){
        for(bj = 0; bj < MatrixSize; bj += BlockSize){
            for(i = bi; i < bi + BlockSize; i++){
                for(j = bj; j < bj + BlockSize; j++){
                    ABT[j][i]=A[i][j];
                }
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end_time);
    double elapsed_time = ((end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec)/1.0e9);
        /*
        printf("\n matrix: A: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",A[i][j]);
            }
        }
        printf("\n matrix: ABT: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",ABT[i][j]);
            }
        }*/
        printf(" \n [time block seq (s), MatrixSize, blockSize] = [%f, %d, %d] \n", elapsed_time, MatrixSize, BlockSize);

    
    //printf( "CPU time sequential block transpose (clock) = %12.4g sec\n", (t1)/1000000.0 );

    //printf("ferdig m seq");
    return 0;
}


int main(   ) {
    int rank, size, i, j;
    float A[MatrixSize][MatrixSize];

    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            A[i][j]= (float)(rand()%9);
        }
    }

    matBlockT(A);
    //matBlockTPar(A, argc, argv);

    return 0;
}
