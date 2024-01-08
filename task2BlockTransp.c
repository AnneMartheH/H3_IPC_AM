#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MatrixSize 16384
#define BlockSize 16

void matBlockTPar(float A[MatrixSize][MatrixSize], int argc, char *argv[]){
    int rank, size, i, j, start_position_i, start_position_j, source;
    float B[MatrixSize][MatrixSize];
    double time1, time2;
    int blocks_per_row = MatrixSize / BlockSize;
    int dest = 0;

    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            B[i][j]=0;
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    start_position_j = (rank % blocks_per_row) *BlockSize;
    start_position_i = (rank / blocks_per_row) *BlockSize;
    if (size > blocks_per_row*blocks_per_row || (MatrixSize*MatrixSize%size)!= 0) {
	    if (rank == 0) {
	    printf("This example requires MatrixSize > size || (MatrixSizesize)!= 0 .\n");
	}
        MPI_Finalize();
        return;
    }

    float sub_matrix[BlockSize][BlockSize];
    float local[BlockSize][BlockSize];
    for (i = 0; i<BlockSize; i++){
        for ( j=0; j<BlockSize; j++){
            sub_matrix[i][j]=0;
            local[i][j]= 0;
        }
    }
    int type_size_block[2]={BlockSize,BlockSize};
    int type_block_start[2]={0,0};
    MPI_Datatype block_type, resized_block;
    MPI_Type_create_subarray(2, type_size_block, type_size_block, type_block_start, MPI_ORDER_C, MPI_INT, &block_type);
    MPI_Type_commit(&block_type);
/////////////////////////////timed section_start/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//Putting values into sub matrix
    time1 = MPI_Wtime();
    if(rank != 0){
        for(i = 0; i < BlockSize; i++){
            for(j = 0; j < BlockSize; j++){
                    sub_matrix[j][i]=A[start_position_i + i][start_position_j + j];
            }
        }
///sending sub matrix
        MPI_Send(&sub_matrix, 1, block_type, dest, rank, MPI_COMM_WORLD); //tag = rank    
    }else{
        for(i = 0; i < BlockSize; i++){
            for(j = 0; j < BlockSize; j++){
                    B[j][i]=A[i][j];
            }
        }
    }           

//putting sub matrix in right place in glob matrix
    if (rank == 0){ 
        for (source = 1; source < size; source++){ //this is a bottleneck, especially if process 1 is slow
            int start_position_j_recv = (source % blocks_per_row) *BlockSize;
            int start_position_i_recv = (source / blocks_per_row) *BlockSize;
            MPI_Recv(&local, 1, block_type, source, source, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(i = 0; i < BlockSize; i++){
                for(j = 0; j < BlockSize; j++){
                        B[start_position_j_recv + j][start_position_i_recv + i]=local[j][i]; 
                }
            }
        }
    }
    time2 = MPI_Wtime();
/////////////////////////////////////timed section_end//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(rank == 0 ){/*
        printf("\n matrix: A: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",A[i][j]);
            }
        }
        printf("\n matrix: B: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%f ",B[i][j]);
            }
        }
        printf("\n matrix: sub_matrix: ");
        for (i = 0; i <BlockSize; i++){
            printf("\n");
            for(j = 0; j < BlockSize; j++){
                printf("%f ",sub_matrix[i][j]);
            }
        }
        printf("\n matrix: local: ");
        for (i = 0; i <BlockSize; i++){
            printf("\n");
            for(j = 0; j < BlockSize; j++){
                printf("%f ",local[i][j]);
            }
        }*/
        printf(" \n [Elapsed time BlockWise Transp, funk (s), number of prosessors, MatrixSize, blockSize] = [%f, %d, %d, %d] \n", time2-time1, size, MatrixSize, BlockSize);
    }

    MPI_Finalize();
}


int main(int argc, char *argv[]) {
    int rank, size, i, j;
    float A[MatrixSize][MatrixSize];

    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            A[i][j]= (float)(rand()%9);
        }
    }

    matBlockTPar(A, argc, argv);

    return 0;
}
