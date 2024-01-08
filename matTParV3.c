/*Run with 8 cpus*/
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MatrixSize 256

void matTPar(float A[MatrixSize][MatrixSize], int argc, char *argv[]){
    int rank, size, i, j, bj;
    float recv_data[MatrixSize];
    float B[MatrixSize][MatrixSize];
    double time1, time2;
    MPI_Datatype row_type, col_type;

    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            B[i][j]=0;
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Type_contiguous (MatrixSize, MPI_FLOAT, &row_type);
    //MPI_Type_vector(MatrixSize, 1, MatrixSize, MPI_INT, &col_type);
    MPI_Type_commit(&row_type);
    //MPI_Type_commit(&col_type);
    
    int rowsPerRank = MatrixSize / size ;
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

    MPI_Allgather(&send_elements, rowsPerRank, row_type, B, rowsPerRank, row_type, MPI_COMM_WORLD);
    time2 = MPI_Wtime();
    
    //could have freed the MPI_Type
    
    if(rank == 0 ){
        /*
        printf("\n matrix: A: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",A[i][j]);
            }
        } */
        /*
        printf("\n matrix: recvElem \n: ");
        for (i = 0; i <MatrixSize*rowsPerRank; i++){
                printf("%d", recv_elements[i]);
            }
        *//*
        printf("\n matrix: C: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",B[i][j]);
            }
        }*/
        
        printf(" \n [Elapsed time AllG (s), number of prosessors] = [%f, %d] \n", time2-time1, size);
        //printf("rows per rank = %d", rowsPerRank);
        
    
    }

MPI_Finalize();
}

int main(int argc, char *argv[]) {
    int i,j;
    float A[MatrixSize][MatrixSize];
    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            A[i][j]= (float)(rand()%9);
        }
    }

    matTPar(A, argc, argv);

    return 0;
}
