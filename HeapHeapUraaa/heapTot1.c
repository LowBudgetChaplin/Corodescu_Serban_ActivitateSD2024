#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Mail Mail;
typedef struct Heap Heap;

struct Mail {
	char* text;
	int prioritate;
};

struct Heap {
	Mail* vector;
	int dim;
};

Heap initHeap(int dim) {
	Heap heap;
	heap.vector = (Mail*)malloc(sizeof(Mail) * dim);
	heap.dim = dim;
	return heap;
}

void afisareMail(Mail mail) {
	printf("Mail cu textul %s si grad de prioritate %d\n", mail.text, mail.prioritate);
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.dim; i++) {
		afisareMail(heap.vector[i]);
	}
}

void filtrare(Heap heap, int pozRad) {
	int fs = pozRad * 2 + 1;
	int fd = pozRad * 2 + 2;
	int pozMax = pozRad;

	if (fs < heap.dim && heap.vector[pozMax].prioritate < heap.vector[fs].prioritate) {
		pozMax = fs;
	}

	if (fd < heap.dim && heap.vector[pozMax].prioritate < heap.vector[fd].prioritate) {
		pozMax = fd;
	}

	if (pozMax != pozRad) {

		if (heap.vector[pozRad].prioritate < heap.vector[pozMax].prioritate) {
			Mail aux = heap.vector[pozRad];
			heap.vector[pozRad] = heap.vector[pozMax];
			heap.vector[pozMax] = aux;

			if (pozMax < (heap.dim - 2) / 2) {
				filtrare(heap, pozMax);
			}

		}
	}
}

void inserareInHeap(Heap* heap, Mail m) {
	Mail* aux = (Mail*)malloc(sizeof(Mail) * (heap->dim + 1));
	aux[0] = m;
	for (int i = 0; i < heap->dim; i++) {
		aux[i] = heap->vector[i - 1];
	}
	free(heap->vector);
	heap->vector = aux;
	heap->dim++;
	filtrare((*heap), 0);
}


void dezalocare(Heap* heap) {
	for (int i = 0; i < heap->dim; i++) {
		free(heap->vector[i].text);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->dim = 0;
}

void main() {

}