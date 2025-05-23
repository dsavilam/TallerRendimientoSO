GCC = gcc
CFLAGS = -lm
FOPENMP = -fopenmp -O3
POSIX = -lpthread

modulo = inter.c

PROGRAMAS = mmClasicaOpenMP mmClasicaFork mmClasicaPosix

ALL: $(PROGRAMAS)


mmClasicaPosix:
	$(GCC) $(CLFAGS) $@.c -o $@ $(POSIX)

mmClasicaFork:
	$(GCC) $(CLFAGS) $@.c -o $@ 

mmClasicaOpenMP:
	$(GCC) $(CLFAGS) $@.c -o $@ $(FOPENMP)

clean:
	$(RM) $(PROGRAMAS) 
