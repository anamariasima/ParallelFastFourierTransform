#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <complex.h>

int N, P;//N - numar elemente vector, P - numar thread-uri
double s1, s2;
typedef double complex cplx;//structura de numere complexe
cplx *in, *out;


void* threadFunction(void *args) {

	int thread_id = *(int*)args;
	int ceil = 1 + ((N - 1)/P);
	int start = thread_id * ceil;
	int end = fmin((thread_id+1) * ceil, N);
	
	for(int k = start; k < end; k++) {
		
		s1 = 0;
		s2 = 0;
		for(int t = 0; t < N; t++) {
		    s1 += creal(cexp(2 * M_PI * t * k / N * (-I)) * in[t]);//partea reala a output-ului
		    s2 += cimag(cexp(2 * M_PI * t * k / N * (-I)) * in[t]);//partea imag a output-ului
			

		}
		out[k] = s1 + s2 * I;
		

	}
	return NULL;
}

void printFile(FILE* fout) {
	fprintf(fout, "%d", N);

	for(int i = 0; i < N; i++) {
		fprintf(fout, "\n%lf %lf", creal(out[i]), cimag(out[i]));
	}
}


int main(int argc, char * argv[]) {

	if(argc != 4)
		return 1;
	
	FILE *fin, *fout;
	int i;
    
    P = atoi(argv[3]);
   	
   	pthread_t tid[P];
	int thread_id[P];

	
	fin = fopen(argv[1], "r");
	
	if(fin == NULL)
   	{
      printf("Error!");   
      exit(1);             
   	}
	fout = fopen(argv[2], "w");
   	
   	if(fout == NULL)
   	{
      printf("Error!");   
      exit(1);             
   	}

	
	int read = fscanf(fin, "%d", &N);
	if(read <= 0) {
		exit(1);
	}
    

	in = malloc(N * sizeof(cplx));//vector de input
    out = malloc(N * sizeof(cplx));//vector de output
   
   	for(i = 0; i < N; i++) {
		read = fscanf(fin, "%lf", &in[i]);
		if(read <= 0) {
			exit(1);
		}
	}
	
   	for(i = 0; i < P; i++)
		thread_id[i] = i;
	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}
	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}
	
	printFile(fout);

   	free(in);
	free(out);

   	fclose(fin);
   	fclose(fout);


	return 0;
}
