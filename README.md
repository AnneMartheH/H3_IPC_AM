# Homework 3 IPC

To run the files in this cluster you have to upload the file an the related pbs file to the unitn cluster. The path in the pbs files have to be changed to were your files lie on the cluster. Then you have to deliver a job submisiion to the cluster, by typing this instructions:

-qsub filename.pbs

After the file has run you can find the results in the .o file. The name of the .o file is given in line 5 in the pbs file. 

The c files with the realted pbs files:

- the c file: matTParV2.c is realted to test_mpi.pbs
- matParV1.c and matParV3.c can be related to test_mpi.pbs if you exhange the matTParV2.c in line 34.
- the c file: task2BlockTransp.c is related to ws_64.pbs
- the c file: task2BlockSeq.c is related to test_mpi.pbs

In order to run one of the above files with a different MatrixSize, you can change the variable MatrixSize once in the top section of each .c file. And then run the related pbs file again. In the file task2BlockTransp the blockSize has to be equal to the number of threads that the file is running with. 

In file matParV2.c is the parallelised transposed code without blocks.
In file matParV1.c and matParV3.c it is tested different implementations of a non blockwise transpose with MPI. 
In file task2BlockTransp.c is the paralellised code with blocks.
In file task2BlockSeq.c is the segeuntial block tranposed. 

The computer i have worked on has this processor: 11th Gen Intel(R) Core(TM) i7-1165G7 @ 2.80GHz 2.80 GHz x64 based proccesor

architecture connected to the test_mpi file.
![image](https://github.com/AnneMartheH/H3_IPC_AM/assets/99717256/3e1af45c-834e-4006-8a1e-6655ac102690)

architecture connected to the ws_64.pbs file.
![image](https://github.com/AnneMartheH/H3_IPC_AM/assets/99717256/f553ff0c-2c00-416e-b9b2-cb1a14a0f5bf)





