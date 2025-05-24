#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>           
#include "mmUtils.h"

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("\n \t\tUse: $./mainFork Size Hilos \n");
		exit(0);
	}
	
	int N = atoi(argv[1]);
	int num_P = atoi(argv[2]);
	double *matA = (double *) calloc(N*N, sizeof(double));
	double *matB = (double *) calloc(N*N, sizeof(double));
	double *matC = (double *) calloc(N*N, sizeof(double));

	srand(time(NULL));
	iniMatrix(matA, matB, N);
	impMatrix(matA, N);
	impMatrix(matB, N);

	int rows_per_process = N / num_P;
	InicioMuestra();
	for (int i = 0; i < num_P; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			int start = i * rows_per_process;
			int end = (i == num_P - 1) ? N : start + rows_per_process;
			multiMatrixFork(matA, matB, matC, N, start, end); // ✅ Corregido nombre de función
			if (N < 9) {
				printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), start, end - 1);
				for (int r = start; r < end; r++) {
					for (int c = 0; c < N; c++) {
						printf(" %f ", matC[N*r + c]);
					}
					printf("\n");
				}
			}
			exit(0);
		} else if (pid < 0) {
			perror("fork failed");
			exit(1);
		}
	}
	for (int i = 0; i < num_P; i++) wait(NULL);
	FinMuestra();

	free(matA);
	free(matB);
	free(matC);
	return 0;
}
