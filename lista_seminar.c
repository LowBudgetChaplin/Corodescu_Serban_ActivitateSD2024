#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>


typedef struct Santier Santier;
typedef struct Nod Nod;

struct Santier {
	char* numeProiect;
	int nrMuncitori;
	float suprafata;
};

struct Nod {
	Nod* next;
	Santier info;
};

Santier initSantier(const char* numeProiect, int nrMuncitori, float suprafata) {
	Santier santier;
	santier.numeProiect = (char*)malloc(strlen((numeProiect)+1) * sizeof(char));
	strcpy(santier.numeProiect, numeProiect);
	santier.nrMuncitori = nrMuncitori;
	santier.suprafata = suprafata;

	return santier;
}

Nod* inserareInceput(Nod* cap, Santier santier) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = santier;
	nou->next = cap;
	return nou;
}

void afisareSantier(Santier santier) {
	printf("\nSantierul %s cu nuamrul de muncitori: %d si suprafata proiectului de %.2f mp\n\n", santier.numeProiect, santier.nrMuncitori, santier.suprafata);

}

void afisareLista(Nod* cap) {
	while (cap != NULL) {
		//procesare
		afisareSantier(cap->info);
		cap = cap->next;

	}

}

int afisareSumaMuncitori(Nod* cap) {
	int suma = 0;
	while (cap != NULL) {
		suma = suma + cap->info.nrMuncitori;
		cap = cap->next;
	}
	return suma;
}

void stergereLista(Nod** cap) {
	
	while ((*cap) != NULL) {
		free((*cap)->info.numeProiect);
		Nod* aux;
		aux	= (*cap)->next;
		free(*cap);
		(*cap) = aux;
	}
	(*cap) = NULL;
}

//numele santierului cu densitatea cea mai mare de muncitori pe mp2
float calculDensitate(Santier santier) {
	if (santier.suprafata > 0)
	{
		return santier.nrMuncitori / santier.suprafata;
	}
	else return 0;
}

char* numeSantierDensitateMaxima(Nod* cap) {
	float densitateaMaxima = 0;
	char* aux;

	while (cap != NULL) {
		if (calculDensitate(cap->info) > densitateaMaxima) {
			densitateaMaxima = calculDensitate(cap->info);
			aux = cap->info.numeProiect;
		}
		cap = cap->next;
	}
	if(aux != NULL)
	{
		char* numeProiectDensitate = (char*)malloc(strlen(aux) + 1);
		strcpy(numeProiectDensitate, aux);

		return numeProiectDensitate;
	}
	else {
		return NULL;
	}


}

void  main() {

	Nod* cap = NULL;
	Santier s1 = initSantier("Santoieru Cucu", 100, 2500.5);
	Santier s2 = initSantier("Santoieru Hash", 120, 2600.5);
	Santier s3 = initSantier("Santoieru 3", 130, 2700.5);

	cap = inserareInceput(cap, s1);
	cap = inserareInceput(cap, s2);
	cap = inserareInceput(cap, s3);
	cap = inserareInceput(cap, initSantier("Santier 4", 140, 37500));

	afisareLista(cap);

	int suma = afisareSumaMuncitori(cap);
	printf("Numarul de munctiori este: %d", suma);

	printf("\n%s\n", numeSantierDensitateMaxima(cap));

	stergereLista(&cap);
	afisareLista(cap);
}