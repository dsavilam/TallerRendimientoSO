#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mmUtils.h"

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Uso: ./mainOpenMP tamMatriz numHilos\n");
		exit(0);
	}
	int N = atoi(argv[1]);
	int hilos = atoi(argv[2]);

	double *mA = calloc(N*N, sizeof(double));
	double *mB = calloc(N*N, sizeof(double));
	double *mC = calloc(N*N, sizeof(double));

	omp_set_num_threads(hilos);
	iniMatrix(mA, mB, N);
	impMatrix(mA, N);
	impMatrix(mB, N);

	InicioMuestra();

	#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			double sum = 0;
			for (int k = 0; k < N; k++) {
				sum += mA[i*N + k] * mB[k*N + j];
			}
			mC[i*N + j] = sum;
		}
	}

	FinMuestra();
	impMatrix(mC, N);
	free(mA); free(mB); free(mC);
	return 0;
}
