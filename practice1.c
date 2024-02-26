#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

struct Farmacie {
	char* nume;
	int nrAngajati;
	float* salarii;
	int anInfiintare;
};

struct Farmacie initializare(const char* nume, int nrAngajati, float* salarii, int anInfiintare) {
	struct Farmacie f;

	f.nume = malloc((strlen(nume) + 1) * sizeof(char));
	strcpy(f.nume, nume);

	f.anInfiintare = anInfiintare;
	f.nrAngajati = nrAngajati;
	f.salarii = malloc(sizeof(float) * nrAngajati);
	for (int i = 0; i < nrAngajati; i++) {
		f.salarii[i] = salarii[i];
	}

	return f;
}


void afisareFarmacie(struct Farmacie f) {
	printf("Denumire: %s, infiintata in %d", f.nume, f.anInfiintare);
	printf(", nr anagajti: %d, cu salariile: ", f.nrAngajati);
	for (int i = 0; i < f.nrAngajati; i++) {
		printf("%5.2f,", f.salarii);
	}
	printf("\n");
}

void main() {
	float* vector = malloc(sizeof(float) * 2);
	vector[0] = 105.2;
	vector[1] = 100;

	struct Farmacie f = initializare("Tei", 5, vector, 2002);
	afisareFarmacie(f);
	free(vector);
	free(f.nume);
	free(f.salarii);
}