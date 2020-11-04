#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// gcc main.c -o main -lpthread

typedef struct {
	pthread_mutex_t m;
	sem_t pgarfos;

	int macarrao;
	int fim;
} Input;


void* philosophize(void* args) {
	Input* dta = (Input*)args;

	while (!(dta->fim)) {
		sleep(dta->n);

		sem_wait(&(dta->pgarfos));
		sem_wait(&(dta->pgarfos));
		pthread_mutex_lock(&(dta->m));

		dta->macarrao--;

		if (dta->macarrao == 0) {
		    dta->fim = 1;
		    sem_post(&(dta->pgarfos));
		    sem_post(&(dta->pgarfos));
		}

		pthread_mutex_unlock(&(dta->m));
		sem_post(&(dta->pgarfos));
		sem_post(&(dta->pgarfos));
	}
}

int main(int argc, char *argv) {

	if (argc != 3) {
		printf("Wrong args");
		exit(1);
	}

	//int n = argv[1];
	//int n = 2;

	pthread_t* vphil = (pthread_t*)malloc(n * sizeof(pthread_t));;

	Input dta;
	//dta.macarrao = argv[2];
	//dta.macarrao = 10;
    dta.fim = 0;
	sem_init(&dta.pgarfos, 0, n);
	pthread_mutex_init(&dta.m, NULL);

	for (int i = 0; i < n; i++)
		pthread_create(&(vphil[i]), NULL, philosophize, (void*)&dta);

	for (int i = 0; i < n; i++)
		pthread_join(vphil[i], (void*)&dta);

	return 0;
}