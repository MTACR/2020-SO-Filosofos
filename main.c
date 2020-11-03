#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>

// gcc main.c -o main -lpthread

typedef struct {
	pthread_mutex_t m;

	//TODO
} Input;


void* philosophize(void* args) {
	//TODO
}

int main(int argc, char *argv) {

	if (argc != 3) {
		printf("Wrong args");
		exit(1);
	}

	int n = argv[1]; //filósofos
	int m = argv[2]; //macarrão

	pthread_t* vphil = (pthread_t*)malloc(n * sizeof(pthread_t));;

	Input dta;
	pthread_mutex_init(&dta.m, NULL);

	for (int i = 0; i < n; i++)
		pthread_create(&(vphil[i]), NULL, philosophize, (void*)&dta);

	for (int i = 0; i < n; i++)
		pthread_join(vphil[i], (void*)&dta);

	return 0;
}