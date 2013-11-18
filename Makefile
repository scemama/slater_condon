CC=gcc
CFLAGS=
FC=ifort 
FFLAGS=-O2 -ipo
SSE2=-xSSE2
AVX=-xAVX
PIN=taskset -c 0

.PHONY: all

all: test_cu_sse2 test_cu_avx test_h2o_sse2 test_h2o_avx

test_cu_sse2: rdtsc.o $(wildcard *.f90)
	@cp main/test_cu.f90 main.f90
	$(FC) $(FFLAGS) $(SSE2) *.f90 rdtsc.o -o $@
	@rm main.f90

test_cu_avx: rdtsc.o $(wildcard *.f90) test_cu_sse2
	@cp main/test_cu.f90 main.f90
	$(FC) $(FFLAGS) $(AVX) *.f90 rdtsc.o -o $@
	@rm main.f90

test_h2o_sse2: rdtsc.o $(wildcard *.f90) test_cu_avx
	@cp main/test.f90 main.f90
	$(FC) $(FFLAGS) $(SSE2) *.f90 rdtsc.o -o $@
	@rm main.f90

test_h2o_avx: rdtsc.o $(wildcard *.f90) test_h2o_sse2
	@cp main/test.f90 main.f90
	$(FC) $(FFLAGS) $(AVX) *.f90 rdtsc.o -o $@
	@rm main.f90

rdtsc.o: rdtsc.c
	$(CC) $(CFLAGS) -c $^

tests: all
	@echo ==============
	@echo Cu, SSE2
	@echo ==============
	$(PIN) ./test_cu_sse2
	@echo ==============
	@echo Cu, AVX
	@echo ==============
	$(PIN) ./test_cu_avx
	@echo ==============
	@echo H2O, SSE2
	@echo ==============
	$(PIN) ./test_h2o_sse2
	@echo ==============
	@echo H2O, AVX
	@echo ==============
	$(PIN) ./test_h2o_avx

clean:
	rm -f *.o test_*
