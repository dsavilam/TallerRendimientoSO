#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "mmUtils.h"

struct parametros {
	int idH;
	int nH;
	int N;
	double *mA, *mB, *mC;
};

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Uso: ./mainPosix tamMatriz numHilos\n");
		exit(0);
	}
	int N = atoi(argv[1]);
	int hilos = atoi(argv[2]);

	double *mA = calloc(N*N, sizeof(double));
	double *mB = calloc(N*N, sizeof(double));
	double *mC = calloc(N*N, sizeof(double));

	iniMatrixPOSIX(N, mA, mB, mC);
	impMatrixPOSIX(N, mA);
	impMatrixPOSIX(N, mB);

	pthread_t p[hilos];
	InicioMuestra();

	for (int i = 0; i < hilos; i++) {
		struct parametros *args = malloc(sizeof(struct parametros));
		args->idH = i;
		args->nH = hilos;
		args->N = N;
		args->mA = mA;
		args->mB = mB;
		args->mC = mC;
		pthread_create(&p[i], NULL, multiMatrixPOSIX, args);
	}

	for (int i = 0; i < hilos; i++) {
		pthread_join(p[i], NULL);
	}

	FinMuestra();
	impMatrixPOSIX(N, mC);
	free(mA); free(mB); free(mC);
	return 0;
}
