#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>


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

void multiMatrix(double *mA, double *mB, double *mC, int D, int filaI, int filaF) {
	double Suma, *pA, *pB;
    for (int i = filaI; i < filaF; i++) {
        for (int j = 0; j < D; j++) {
			Suma = 0;
			pA = mA+i*D;
			pB = mB+j;
            for (int k = 0; k < D; k++, pA++, pB+=D) {
				Suma += *pA * *pB;	
            }
			mC[i*D+j] = Suma;
        }
    }
}

void impMatrix(double *matrix, int D) {
	if (D < 9) {
    	printf("\nImpresión	...\n");
    	for (int i = 0; i < D*D; i++, matrix++) {
			if(i%D==0) printf("\n");
            	printf(" %f ", *matrix);
        	}
        printf("\n ");
    }
}

// Function to initialize matrix with random values
void iniMatrix(double *mA, double *mB, int D){
	for (int i = 0; i < D*D; i++, mA++, mB++){
            *mA = rand() % 10; 
            *mB = rand() % 10; 
        }
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("\n \t\tUse: $./nom_ejecutable Size Hilos \n");
		exit(0);
	}
	int N      = (int) atoi(argv[1]);
	int num_P  = (int) atoi(argv[2]);
    double *matA = (double *) calloc(N*N, sizeof(double));
    double *matB = (double *) calloc(N*N, sizeof(double));
    double *matC = (double *) calloc(N*N, sizeof(double));

    srand(time(0)); // Seed for random number generation
    
    iniMatrix(matA, matB, N);
    impMatrix(matA, N);
    impMatrix(matB, N);
    
    int rows_per_process = N/ num_P;

	InicioMuestra();
    for (int i = 0; i < num_P; i++) {
        pid_t pid = fork();
        
        if (pid == 0) { // Child process
            int start_row = i * rows_per_process;
            int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;
            
			multiMatrix(matA, matB, matC, N, start_row, end_row); 
            
			if(N<9){
            	// Print the portion calculated by this child
           		printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), start_row, end_row-1);
            	for (int r = start_row; r < end_row; r++) {
                	for (int c = 0; c < N; c++) {
                    	printf(" %f ", matC[N*r+c]);
                	}
                	printf("\n");
            	}
			}
            exit(0); // Child exits after completing its task
        } else if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
    }
    
    // Parent waits for all children to complete
    for (int i = 0; i < num_P; i++) {
        wait(NULL);
    }
  	
	FinMuestra(); 
 
	free(matA);
	free(matB);
	free(matC);

    return 0;
}
