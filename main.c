#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// gcc main.c -o main -lpthread

#define PENSANDO 0
#define COMENDO 1
#define FAMINTO 2

pthread_mutex_t m;
sem_t *s;
int *estado;

int macarrao;
int fim;

void* filosofo(void* args);
void esperar();

int main(int argc, char *argv) {

	if (argc != 3) {
		printf("Wrong args");
		exit(1);
	}

	int n = argv[1];
	macarrao = argv[2];
    fim = 0;

	pthread_t* filosofos = (pthread_t*) malloc(n * sizeof(pthread_t));
	s = (sem_t*) malloc(n * sizeof(sem_t));
	estado = (int*) malloc(n * sizeof(int));

	pthread_mutex_init(&m, NULL);

	for (int i = 0; i < n; i++) {
	    sem_init(&s[i], 0, n);
	    estado[i] = PENSANDO;
    }

	for (int i = 0; i < n; i++)
		pthread_create(&filosofos[i], NULL, filosofo, (void*)&i);

	for (int i = 0; i < n; i++)
		pthread_join(filosofos[i], (void*)&i);

	return 0;
}

void* filosofo(void* args) {
    int i = *(int*) args;

	while (!fim) {
		esperar();

		sem_wait(&s[i]);
		pthread_mutex_lock(&m);

		macarrao--;

		if (macarrao == 0) {
		    fim = 1;
		    sem_post(&s[i]);
		}

		pthread_mutex_unlock(&m);
		sem_post(&s[i]);
	}
}

void esperar() {
	srand(time(NULL));
	usleep(1 + rand() % (1000));
}