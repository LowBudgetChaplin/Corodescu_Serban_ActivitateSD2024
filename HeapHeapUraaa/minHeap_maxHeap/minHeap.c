#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct Platforma Platforma;
typedef struct Heap Heap;

struct Platforma {
    char* nume;
    char* owner;
    int dataCreare;
    int nrUtilizatori;
    int pretLicenta;
};

struct Heap {
    int dim;
    int capacitate;
    Platforma* vector;
};

void afisarePlatforma(Platforma p) {
    printf("Platforma: %s, prop: %s, data: %d, util: %d, pretLicenta: %d\n",
        p.nume, p.owner, p.dataCreare, p.nrUtilizatori, p.pretLicenta);
}

Heap initHeap(int capacitate) {
    Heap heap;
    heap.dim = 0;
    heap.capacitate = capacitate;
    heap.vector = (Platforma*)malloc(sizeof(Platforma) * capacitate);
    return heap;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.dim; i++) {
        afisarePlatforma(heap.vector[i]);
    }
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->dim; i++) {
        free(heap->vector[i].nume);
        free(heap->vector[i].owner);
    }
    free(heap->vector);
    heap->vector = NULL;
    heap->dim = 0;
    heap->capacitate = 0;
}

void filtrare(Heap heap, int pozRad) {
    int fs = pozRad * 2 + 1;
    int fd = pozRad * 2 + 2;
    int pozMin = pozRad;

    if (fs < heap.dim && heap.vector[pozMin].pretLicenta > heap.vector[fs].pretLicenta) {
        pozMin = fs;
    }
    if (fd < heap.dim && heap.vector[pozMin].pretLicenta > heap.vector[fd].pretLicenta) {
        pozMin = fd;
    }

    if (pozMin != pozRad) {
        Platforma aux = heap.vector[pozRad];
        heap.vector[pozRad] = heap.vector[pozMin];
        heap.vector[pozMin] = aux;
        filtrare(heap, pozMin);
    }
}

Platforma extrageMin(Heap* heap) {
    if (heap->dim > 0) {
        Platforma aux = heap->vector[0];
        heap->vector[0] = heap->vector[heap->dim - 1];
        heap->dim--;
        filtrare(*heap, 0);
        return aux;
    }
    Platforma p = { NULL, NULL, 0, 0, 0 };
    return p;
}

void inserareInHeap(Heap* heap, Platforma p) {
    if (heap->dim == heap->capacitate) {
        heap->capacitate *= 2;
        heap->vector = (Platforma*)realloc(heap->vector, sizeof(Platforma) * heap->capacitate);
        if (heap->vector == NULL) {
            printf("Error reallocating memory\n");
            exit(1);
        }
    }
    heap->vector[heap->dim] = p;
    heap->dim++;
    for (int i = (heap->dim - 1) / 2; i >= 0; i--) {
        filtrare(*heap, i);
    }
}

Platforma citirePlatforma(FILE* f) {
    Platforma p;
    char buffer[256];

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading nume\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    p.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    if (p.nume == NULL) {
        printf("Error allocating memory for nume\n");
        exit(1);
    }
    strcpy(p.nume, buffer);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading owner\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    p.owner = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    if (p.owner == NULL) {
        printf("Error allocating memory for owner\n");
        exit(1);
    }
    strcpy(p.owner, buffer);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading dataCreare\n");
        exit(1);
    }
    sscanf(buffer, "%d", &p.dataCreare);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading nrUtilizatori\n");
        exit(1);
    }
    sscanf(buffer, "%d", &p.nrUtilizatori);

    if (fgets(buffer, 256, f) == NULL) {
        printf("Error reading pretLicenta\n");
        exit(1);
    }
    sscanf(buffer, "%d", &p.pretLicenta);

    return p;
}

Heap citireHeap(const char* file) {
    Heap heap;
    heap.dim = 0;
    heap.capacitate = 1;
    heap.vector = NULL;

    if (file && strlen(file) != 0) {
        FILE* f = fopen(file, "r");
        if (f != NULL) {
            int dim = 0;
            fscanf(f, "%d\n", &dim);
            heap = initHeap(dim);
            for (int i = 0; i < dim; i++) {
                Platforma p = citirePlatforma(f);
                inserareInHeap(&heap, p);
            }
            fclose(f);
        }
        else {
            printf("Error opening file\n");
            exit(1);
        }
    }
    return heap;
}

void modificaPretLicenta(Heap* heap, char* nume, int noulPret) {
    for (int i = 0; i < heap->dim; i++) {
        if (strcmp(heap->vector[i].nume, nume) == 0) {
            heap->vector[i].pretLicenta = noulPret;
            for (int j = (heap->dim - 1) / 2; j >= 0; j--) {
                filtrare(*heap, j);
            }
            return;
        }
    }
    printf("Platforma cu numele %s nu a fost gasita\n", nume);
}

void sortareHeap(Heap* heap) {
    int originalDim = heap->dim;
    for (int i = heap->dim - 1; i > 0; i--) {
        Platforma aux = heap->vector[0];
        heap->vector[0] = heap->vector[i];
        heap->vector[i] = aux;
        heap->dim--;
        filtrare(*heap, 0);
    }
    heap->dim = originalDim;
}

int main() {
    Heap heap = citireHeap("platforme.txt");

    printf("Platformele din MinHeap:\n");
    afisareHeap(heap);
    printf("\n\n");

    Platforma platformaNoua = { "Platforma Noua", "Proprietar Nou", 2023, 1000, 700 };
    inserareInHeap(&heap, platformaNoua);
    printf("MinHeap dupa inserarea unei noi platforme:\n");
    afisareHeap(heap);
    printf("\n\n");

    Platforma minPlatforma = extrageMin(&heap);
    printf("Platforma cu pretul de licenta cel mai mic extrasa:\n");
    afisarePlatforma(minPlatforma);
    printf("MinHeap dupa extragerea platformei cu pretul de licenta minim:\n");
    afisareHeap(heap);
    printf("\n\n");

    modificaPretLicenta(&heap, "Platforma 2", 800);
    printf("MinHeap dupa modificarea pretului de licenta pentru Platforma 2:\n");
    afisareHeap(heap);
    printf("\n\n");

    sortareHeap(&heap);
    printf("Platformele sortate in ordinea crescatoare a pretului de licenta:\n");
    afisareHeap(heap);
    printf("\n\n");

    dezalocareHeap(&heap);

    return 0;
}