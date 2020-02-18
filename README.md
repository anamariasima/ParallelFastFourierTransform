# ParallelFastFourierTransform

/* SIMA ANA-MARIA - 333CB */

Programul citeste dintr-un fisier un numar N si un vector de N elemente complexe.  Returneaza un vector de N numere complexe rezultate in urma aplicarii transformarii Fourier pe vectorul de input. Se scrie intr-un fisier N pe prima linie, urmat de cele N elemente complexe pe linii separate. In cadrul homeworkFT.c se aplica transformarea Fourier discreta, iar in homeworkFFT.c aplicam Fast Fourier.

homeworkFT.c:

Declarare de variabile globale:
- N: numar elemente vector;
- P: numar thread-uri;
- in, out: vectori de input, respectiv output.


Functia void* threadFunction(void*) este cea apelata de thread-uri. In cadrul ei se aplica tranformarea Fourier pentru a determina fiecare element out[k] in functie de in[t] dupa formula transformarii. Paralelizarea s-a facut cu ajutorul indecsilor de start si end.

Functia void printFile(FILE*) scrie in fisier N pe prima linie, urmat de elementele vectorului.

In main():

	Aici are loc citirea din fisier, alocarea vectorilor de in si out, precum si crearea thread-urilor in functie de P, numarul de thread-uri dat in linia de comanda.

	Se apeleaza functia de printare.

	Urmeaza dezalocarea de memorie si inchiderea fisierelor.


homeworkFFT.c:

Declarare de variabile globale:
- N: numar elemente vector;
- P: numar thread-uri;
- in, out: vectori de input, respectiv output.
- step: preluat din implementarea Rosetta(lungimea fiecarui nivel al arborelui de recursivitate).

Structura Tstr e folosita pentru a apela threadFunction() cu argumente.

In cadrul acestui program am analizat cazul pentru 1, 2, respectiv 4 thread-uri. 

- 1 thread: 
	- void threadFunction3(void*): apeleaza _fft().
	- in void fft(cplx*, int) se creeaza un thread care apeleaza threadFunction3().

- 2 thread-uri:
	- void* threadFunction1(void*): apeleaza primul apel recursiv de _fft(pe un subarbore);
	- void* threadFunction1(void*): apeleaza al doilea apel recursiv de _fft(pe celalalt subarbore).

	- in void fft(cplx*, int) se creeaza 2 thread-uri: unul pentru threadFunction1() si celalalt pentru threadFunction2(). Astfel, un thread se va ocupa de primul apel recursiv de _fft(), iar celalalt de al doilea(vezi implementarea Rosetta).

- pentru 4 thread-uri:
	- void threadFunction(void*): realizeaza un apel de _fft cu step-ul corespunzator(4).
	Acum cele doua threadFunction de la punctul anterior genereaza crearea a cate un thread fiecare datorita conditiei din _fft ca numarul de thread-uri sa fie egal cu 4 si step-ul mai mic decat 4.

In void _fft(cplx*, cplx*, int, int) are loc transformarea Fourier propriu-zisa. 

In functia void fft(cplx*, int) are loc crearea thread-urilor cu threadFunction1(), respectiv threadFunction2() daca P = 2 sau P = 4. Altfel, se creeaza un thread cu threadFunction3() pentru P = 1.

Functia void printFile(FILE*) scrie in fisier N pe prima linie, urmat de elementele vectorului rezultat.

In main():
	
	Aici are loc citirea din fisier, alocarea vectorilor de in si out.

	Se apeleaza fft cu vectorul de input si numarul de elemente ale vectorului.

	Urmeaza printarea, dezalocarea de memorie si inchiderea fisierelor.
