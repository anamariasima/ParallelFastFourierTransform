#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>
 

int N, P;//N - numar elemente vector, P - numar thread-uri
typedef double complex cplx;
cplx *in, *out;
int step = 1;
void _fft(cplx buf[], cplx out[], int n, int step);


typedef struct str { //structura folosita pentru a apela threadFunction cu argumente
	cplx *buf;
	cplx *out;
	int n;
	int step;  

}Tstr;

void* threadFunction(void *args) {

	Tstr *aux = args;
	cplx* buf = aux -> buf;
	cplx* out = aux -> out; 
	int step_prim = aux->step;

	_fft(buf, out, N, step_prim);
	
	return NULL;
}

void _fft(cplx buf[], cplx out[], int n, int step)
{

	pthread_t tid;
	if (step < n) {

		if(step < P && P == 4) { // pt P = 4 se deschid  2 thread-uri in plus fata de 
								//cele pe care ruleaza threadFunction1() si threadFunction2()
			Tstr* aux = malloc(sizeof(Tstr));
			aux->buf = out;
			aux->out = buf;
			aux->n = n;
			aux->step = step * 2;

			pthread_create(&tid, NULL, threadFunction, aux); 
		
			_fft(out + step, buf + step, n, step * 2);

			pthread_join(tid, NULL);	
		} 
		else {
			_fft(out, buf, n, step * 2);
			
			_fft(out + step, buf + step, n, step * 2);
		}
 		
		for (int i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * M_PI * i / n) * out[i + step]; 
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
			
		}
	}
}
  

void* threadFunction1(void *args) {
	
	_fft(out, in, N, 2);
	
	return NULL;
}

void* threadFunction2(void *args) {
	
	_fft(out + 1, in + 1, N, 2);
	
	return NULL;
}

void* threadFunction3(void *args) {
	
	_fft(in, out, N, 1);
	
	return NULL;
}


void fft(cplx buf[], int n)
{
	
	int i;
	pthread_t tid[2];
	int thread_id[2];
	
	for (i = 0; i < n; i++) out[i] = buf[i];
 
	if(P != 1) {//pt P = 2 sau P = 4 se deschid 2 thread-uri
		
		for(i = 0; i < 2; i++) {
			thread_id[i] = i;
		}

	
		pthread_create(&(tid[0]), NULL, threadFunction1, &(thread_id[0]));
		pthread_create(&(tid[1]), NULL, threadFunction2, &(thread_id[1]));


		for(i = 0; i < 2; i++) {   
			pthread_join(tid[i], NULL);
		}

		for (int i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * M_PI * i / n) * out[i + step]; 
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t; 
			
		}
	}
	else { // cazul pt P = 1 - un thread pe care ruleaza _fft();
    	thread_id[0] = 0;

    	pthread_create(&(tid[0]), NULL, threadFunction3, &(thread_id[0]));

    	pthread_join(tid[0], NULL);
	
	}
}



void printFile(FILE* fout) {
	fprintf(fout, "%d", N);

	for(int i = 0; i < N; i++) {
		fprintf(fout, "\n%lf %lf", creal(in[i]), cimag(in[i]));
	}
}

int main(int argc, char * argv[]) {

	if(argc != 4)
		return -1;
	
	FILE *fin, *fout;
	int i;
    
    P = atoi(argv[3]); 
   	
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

   	//citire N
	int read = fscanf(fin, "%d", &N);
	if(read <= 0) {
		exit(1);
	}
	
    in =  malloc(N * sizeof(cplx));//vector de input
    out = malloc(N * sizeof(cplx));//vector de output
   	
   	//citire vector
   	for(i = 0; i < N; i++) {
		read = fscanf(fin, "%lf", &in[i]);
		if(read <= 0) {
			exit(1);
		}
		
	}
	
	
   	fft(in, N);
   	
   	printFile(fout);

   	free(in);
   	free(out);
   	
   	fclose(fin);
   	fclose(fout);


	return 0;
}
