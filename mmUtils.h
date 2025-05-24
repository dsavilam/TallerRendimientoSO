#ifndef MMUTILS_H
#define MMUTILS_H

#include <sys/time.h>

void InicioMuestra();
void FinMuestra();

// OpenMP y Fork
void iniMatrix(double *mA, double *mB, int D);
void impMatrix(double *matrix, int D);
void multiMatrixFork(double *mA, double *mB, double *mC, int D, int filaI, int filaF);

// POSIX
void iniMatrixPOSIX(int D, double *mA, double *mB, double *mC);
void impMatrixPOSIX(int D, double *matrix);
void *multiMatrixPOSIX(void *args);

#endif
