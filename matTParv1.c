/*Run with 8 cpus*/
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

#define MatrixSize 16384

int main(int argc, char *argv[]) {
    int rank, size, i, j;
    int send_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int recv_data[2];
    int send_elements[MatrixSize*MatrixSize];
    int recv_elements[MatrixSize];
    int A[MatrixSize][MatrixSize];
    int B[MatrixSize][MatrixSize];

    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            A[i][j]= (int)(rand()%9);
        }
    }

    for (i = 0; i<MatrixSize; i++){
        for ( j=0; j<MatrixSize; j++){
            B[i][j]=0;
        }
    }

    for(i=0; i< MatrixSize; i++){
        for (j = 0; j < MatrixSize; j++){
            send_elements[j+(i*MatrixSize)]=A[j][i];
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        printf("\n matrix: A: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%d ",A[i][j]);
            }
        } 
        printf("\n matrix: send elem \n: ");
        for (i = 0; i <MatrixSize*MatrixSize; i++){
                printf("%d", send_elements[i]);
            }}

    if (size != 4) {
	if (rank == 0) {
	    printf("This example requires exactly 8 processes.\n");
	}
        MPI_Finalize();
        return 1;
    }
    // int MPI_Scatter(void *sendbuf, int sendcount, MPI_Datatype senddatatype, void *recvbuf, int recvcount, MPI_Datatype recvdatatype, int source, MPI_Comm comm)
    //MPI_Scatter(send_data,2,MPI_INT,&recv_data,2,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Scatter(send_elements,MatrixSize,MPI_INT,&recv_elements,MatrixSize,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Gather(&recv_elements, MatrixSize, MPI_INT, B, MatrixSize, MPI_INT, 0, MPI_COMM_WORLD);

    // Modify received data locally
    //recv_data += 1;

    // int MPI_Gather(void *sendbuf,int sendcount,MPI_Datatype senddatatype, void *recvbuf, int recvcount, MPI_Datatype recvdatatype, int target, MPI_Comm comm)
    //MPI_Gather(&recv_data,1,MPI_INT,send_data,1,MPI_INT,0,MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\n After incrementing locally on each process: ");
        for (int i = 0; i < MatrixSize; i++) {
            //printf("%d ", recv_elements[i]);
        }
        printf("\n");
    }
    /*for(i=0; i< MatrixSize; i++){
        B[rank][i]=recv_elements[i];
    }*/

    MPI_Finalize();
    if(rank == 0 ){
        printf("\n matrix: C: ");
        for (i = 0; i <MatrixSize; i++){
            printf("\n");
            for(j = 0; j < MatrixSize; j++){
                printf("%d ",B[i][j]);
            }
        }
    }

    return 0;
}
