#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// gcc main3.c -o main3 -lpthread

#define PENSANDO 0
#define COMENDO 1
#define FAMINTO 2

pthread_mutex_t m;
sem_t *s;
int *estado;

int macarrao;
int fim;
int n;

void* filosofo(void* args);
void esperar();
void pegar(int i);
void devolver(int i);
void testar(int i);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Wrong args\n");
        exit(1);
    }

    n = atoi(argv[1]);
    macarrao = atoi(argv[2]);
    fim = 0;

    pthread_t* filosofos = (pthread_t*) malloc(n * sizeof(pthread_t));
               s         = (sem_t*)     malloc(n * sizeof(sem_t));
               estado    = (int*)       malloc(n * sizeof(int));

	pthread_mutex_init(&m, NULL);

	for (int i = 0; i < n; i++) {
	    sem_init(&s[i], 0, 0);
	    estado[i] = PENSANDO;
    }

	for (int i = 0; i < n; i++)
		pthread_create(&filosofos[i], NULL, filosofo, (void*)&i);

	for (int i = 0; i < n; i++) {
        int id = i;
        pthread_join(filosofos[i], (void *) &id);
    }

    printf("TODOS SAÍRAM\n");

    for (int i = 0; i < n; i++)
        sem_destroy(&s[i]);

    pthread_mutex_destroy(&m);

    free(filosofos);
    free(s);
    free(estado);

    return 0;
}

void* filosofo(void* args) {
    int i = *(int*) args;

	while (!fim) {
		esperar();
		pegar(i);
        esperar();
        devolver(i);
	}

	printf("%d -> saiu\n", i);
}

void devolver(int i) {
    int esq = (i + n - 1) % n;
    int dir = (i + 1) % n;

	pthread_mutex_lock(&m);
	if (estado[i] == COMENDO) printf("%d -> largou garfo %d e %d\n", i, i, dir);
	estado[i] = PENSANDO;
	printf("%d -> pensando\n", i);
	testar(esq);
	testar(dir);
	pthread_mutex_unlock(&m);
	
}

void pegar(int i) {
    pthread_mutex_lock(&m);

    estado[i] = FAMINTO;
    printf("%d -> faminto\n", i);
    testar(i);
    pthread_mutex_unlock(&m);
    sem_wait(&s[i]);
}

void esperar() {
	srand(time(NULL));
	usleep(1 + rand() % 1000000);
	//usleep(1);
}

void testar(int i) {
    int esq = (i + n - 1) % n;
    int dir = (i + 1) % n;

    if (estado[i] == FAMINTO && estado[esq] != COMENDO && estado[dir] != COMENDO) {
		
        if (macarrao > 0) {
            estado[i] = COMENDO;
			printf("%d -> pegou garfo %d e %d\n", i, i, dir);
			printf("%d -> comendo (%d)\n", i, macarrao);
            macarrao--;
			if (macarrao == 0){
				printf("ACABOU O MACARRAO  :(\n");
				fim = 1;
			}
        }
		
        sem_post(&s[i]);
   }
}