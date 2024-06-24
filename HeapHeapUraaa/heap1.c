#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct Quiz Quiz;
typedef struct Heap Heap;

struct Quiz {
	char* materie;
	int grileGresite;
};

struct Heap {
	Quiz* vector;
	int dimensiune;
	int dimensiuneTotala;
};

Quiz initQuiz(char* materie, int grileGresite){
	Quiz q;
	q.materie = (char*)malloc(strlen(materie) + 1);
	strcpy(q.materie, materie);
	q.grileGresite = grileGresite;
	return q;
}

Heap initHeap(int nr) {
	Heap heap;
	heap.vector =(Quiz*)malloc(nr * sizeof(Quiz));
	heap.dimensiuneTotala = nr;
	heap.dimensiune = 0;
	return heap;
}

Heap addQuizzHeap(Quiz quiz, Heap heap) {
	if (heap.dimensiune < heap.dimensiuneTotala) {
		heap.vector[heap.dimensiune] = quiz;
		heap.dimensiune++;
		//filtrare ceva
	}
	return heap;
}

void afisareQuiz(Quiz q) {
	printf("Materia: %s\n", q.materie);
	printf("Grile gresite %d\n", q.grileGresite);
}

void afisareHeap(Heap h) {
	for (int i = 0; i < h.dimensiuneTotala; i++) {
		afisareQuiz(h.vector[i]);
	}
}

void afisareTotalaHeap(Heap h) {
	for (int i = 0; i > h.dimensiuneTotala; i++) {
		afisareQuiz(h.vector[i]);
	}
}

void dezalocareTotalHeap(Heap* heap) {
	for (int i = 0; i < heap->dimensiuneTotala; i++) {
		free(heap->vector[i].materie);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->dimensiune = 0;
	heap->dimensiuneTotala = 0;
}

void filtrareHeap(Heap heap, int pozRadacina) {
	int nodSt = pozRadacina * 2 + 1;
	int nodDr = pozRadacina * 2 + 2;
	int pozMin = pozRadacina;
	//verificare nodul din stanga si dc e mai mic decat radacina
	if (nodSt < heap.dimensiune &&
		heap.vector[nodSt].grileGresite < heap.vector[pozMin].grileGresite) {
		pozMin = nodSt;
	}
	//verif nod dreapta si dc e < minimul dintre stanga si radacina
	if (nodDr < heap.dimensiune && 
		heap.vector[nodDr].grileGresite < heap.vector[pozMin].grileGresite) {
		pozMin = nodDr;
	}

	if (pozMin != pozRadacina) {
		Quiz aux;
		aux = heap.vector[pozRadacina];
		heap.vector[pozRadacina] = heap.vector[pozMin];
		heap.vector[pozMin] = aux;
		//verif daca nodul cu poz minim are copii 
		if (pozMin * 2 + 1 < heap.dimensiune - 1) {
			filtrareHeap(heap, pozMin);
		}
	}

}

//extrage minimul din heap
Quiz extagereMinim(Heap* heap) {
	if (heap->dimensiune > 0) {
		Quiz aux;
		//switch intre primul element si ultimul din vector
		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->dimensiune - 1];
		heap->vector[heap->dimensiune - 1] = aux;

		//hide ultim element
		heap->dimensiune--;

		for (int i = (heap->dimensiune) / 2 - 1; i >= 0; i--) {
			filtrareHeap((*heap), i);
		}
		return aux;
	}
	else {
		return initQuiz("N-avem bro", -1);
	}
}


void main() {
	Heap heap;
	heap = initHeap(6);
	heap.vector[0] = initQuiz("Analiza", 4);
	heap.vector[1] = initQuiz("PEAG", 7);
	heap.vector[2] = initQuiz("PSM", 3);
	heap.vector[3] = initQuiz("SO", 9);
	heap.vector[4] = initQuiz("POO", 2);
	heap.vector[5] = initQuiz("PAW", 6);

	heap.dimensiune = 6;
	afisareHeap(heap);

	for (int i = (heap.dimensiune) / 2 - 1; i >= 0; i--) {
		filtrareHeap(heap, i);
	}
	printf("\n");
	afisareHeap(heap);
	
	printf("\n");
	afisareQuiz(extagereMinim(&heap));
	afisareQuiz(extagereMinim(&heap));
	afisareQuiz(extagereMinim(&heap));
	afisareQuiz(extagereMinim(&heap));
	afisareQuiz(extagereMinim(&heap));
	afisareQuiz(extagereMinim(&heap));

	printf("\n");
	afisareTotalaHeap(heap);

	dezalocareTotalHeap(&heap);
	afisareTotalaHeap(heap);
}