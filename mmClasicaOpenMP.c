/*#######################################################################################
 #* Fecha: 20 sep 2025
 #* Autor: J. Corredor, PhD
 #* Tema: Programación Modular en C
 #* 	- Programa Multiplicación de Matrices algoritmo clásico
 #* 	- Paralelismo con OpenMP
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>


struct timeval inicio, fin; 

void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}

void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	printf("%9.0f \n", tiempo);
}

void impMatrix(size_t *matrix, int D){
	printf("\n");
	if(D < 9){
		for(int i=0; i<D*D; i++){
			if(i%D==0) printf("\n");
			printf("%zu ", matrix[i]);
		}
		printf("\n**-----------------------------**\n");
	}
}

void iniMatrix(size_t *m1, size_t *m2, int D){
	for(int i=0; i<D*D; i++, m1++, m2++){
		*m1 = i*2; // (size_t) rand()%10;	
		*m2 = i+2; // (size_t) rand()%10;	
	}
}

void multiMatrix(size_t *mA, size_t *mB, size_t *mC, int D){
	size_t Suma, *pA, *pB;
	#pragma omp parallel
	{
	#pragma omp for
	for(int i=0; i<D; i++){
		for(int j=0; j<D; j++){
			pA = mA+i*D;	
			pB = mB+j; //pB = mB+(j*D);	
			Suma = 0.0;
			for(int k=0; k<D; k++, pA++, pB+=D){
				Suma += *pA * *pB;
			}
			mC[i*D+j] = Suma;
		}
	}
	}
}

int main(int argc, char *argv[]){
	if(argc < 3){
		printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
		exit(0);
	}


	int N = atoi(argv[1]);
	int TH = atoi(argv[2]);
	size_t *matrixA  = (size_t *)calloc(N*N, sizeof(size_t));
	size_t *matrixB  = (size_t *)calloc(N*N, sizeof(size_t));
	size_t *matrixC  = (size_t *)calloc(N*N, sizeof(size_t));
	srand(time(NULL));

	omp_set_num_threads(TH);

	iniMatrix(matrixA, matrixB, N);

	impMatrix(matrixA, N);
	impMatrix(matrixB, N);

	InicioMuestra();
	multiMatrix(matrixA, matrixB, matrixC, N);
	FinMuestra();

	impMatrix(matrixC, N);

	/*Liberación de Memoria*/
	free(matrixA);
	free(matrixB);
	free(matrixC);
	
	return 0;
}
