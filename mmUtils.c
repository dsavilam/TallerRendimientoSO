#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mmUtils.h"

struct timeval inicio, fin;

void InicioMuestra(){
	gettimeofday(&inicio, NULL);
}

void FinMuestra(){
	gettimeofday(&fin, NULL);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec * 1000000 + fin.tv_usec);
	printf("%9.0f \n", tiempo);
}

// ---------- FORK Y OPENMP ----------
void iniMatrix(double *mA, double *mB, int D){
	for (int i = 0; i < D*D; i++, mA++, mB++){
        *mA = rand() % 10;
        *mB = rand() % 10;
    }
}

void impMatrix(double *matrix, int D){
	if (D < 9) {
    	for (int i = 0; i < D*D; i++, matrix++) {
			if(i%D == 0) printf("\n");
            printf(" %.2f ", *matrix);
        }
        printf("\n ");
    }
}

void multiMatrixFork(double *mA, double *mB, double *mC, int D, int filaI, int filaF){
	double Suma, *pA, *pB;
	for (int i = filaI; i < filaF; i++) {
		for (int j = 0; j < D; j++) {
			Suma = 0;
			pA = mA + i*D;
			pB = mB + j;
			for (int k = 0; k < D; k++, pA++, pB += D) {
				Suma += *pA * *pB;	
			}
			mC[i*D + j] = Suma;
		}
	}
}

// ---------- POSIX ----------
void iniMatrixPOSIX(int D, double *mA, double *mB, double *mC){
	for (int i = 0; i < D*D; i++) {
        mA[i] = rand() % 10;
        mB[i] = rand() % 10;
        mC[i] = 0;
    }
}

void impMatrixPOSIX(int D, double *matrix){
	if (D < 12) {
    	for (int i = 0; i < D*D; i++) {
			if(i%D == 0) printf("\n");
            printf(" %.3f ", matrix[i]);
        }
        printf("\n>-------------------->\n");
    }
}

void *multiMatrixPOSIX(void *variables){
	struct parametros {
		int idH;
		int nH;
		int N;
		double *mA, *mB, *mC;
	} *data = variables;

	int ini = (data->N / data->nH) * data->idH;
	int fin = (data->N / data->nH) * (data->idH + 1);

	for (int i = ini; i < fin; i++) {
		for (int j = 0; j < data->N; j++) {
			double *pA = data->mA + i * data->N;
			double *pB = data->mB + j;
			double suma = 0;
			for (int k = 0; k < data->N; k++, pA++, pB += data->N) {
				suma += *pA * *pB;
			}
			data->mC[i * data->N + j] = suma;
		}
	}
	pthread_exit(NULL);
}
