/*Run with 8 cpus*/
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MatrixSize 16384

void matTPar(float A[MatrixSize][MatrixSize], int argc, char *argv[]){
    int rank, size, i, j, bj;
    float send_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    float recv_data[2];
    float B[MatrixSize][MatrixSize];
    double time1, time2;

    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            B[i][j]=0;
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rowsPerRank = MatrixSize / size ;
    float recv_elements[MatrixSize*rowsPerRank];
    float send_elements[MatrixSize*rowsPerRank];

    if (size > MatrixSize || (MatrixSize%size)!= 0) {
	    if (rank == 0) {
	    printf("This example requires MatrixSize > size || (MatrixSizesize)!= 0 .\n");
	}
        MPI_Finalize();
        return;
    }

    bj = rank * rowsPerRank;
    time1 = MPI_Wtime();
    if(rowsPerRank == 1){
        for(i=0; i< MatrixSize; i++){
            send_elements[i]=A[i][rank];
        }
    }else{
        for(j = bj; j < bj +rowsPerRank; j++){
            for(i = 0; i < MatrixSize; i++){
                send_elements[i+(j-bj)*MatrixSize]=A[i][j];
            }
        }
    }
    MPI_Gather(&send_elements, MatrixSize * rowsPerRank, MPI_FLOAT, B, MatrixSize * rowsPerRank, MPI_FLOAT, 0, MPI_COMM_WORLD);
    time2 = MPI_Wtime();
    if(rank == 0){
        printf(" \n [Elapsed time Gather, funk (s), number of prosessors] = [%f, %d] \n", time2-time1, size);
    }

    
    if(rank == 0){/*
        printf("\n matrix: A: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",A[i][j]);
            }
        }*/
    } 

    

    MPI_Finalize();
    if(rank == 0 ){
        /*
        printf("\n matrix: C: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",B[i][j]);
            }
        }*/
        
    }


}


int main(int argc, char *argv[]) {
    int rank, size, i, j;
    float A[MatrixSize][MatrixSize];


    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            A[i][j]= (float)(rand()%9);
        }
    }

    matTPar(A, argc, argv);

    return 0;
}
