#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <malloc.h>

typedef struct Moneda Moneda;
typedef struct Nod Nod;
//typedef struct ListaDubla ListaDubla;

struct Moneda {
    char* tara;
    int valoare;
    float greutate;
    int an;
};

struct Nod {
    Moneda info;
    Nod* next;
};

Moneda initializareMoneda(const char* tara, int valoare, float greutate, int an)
{
    Moneda moneda;
    moneda.valoare = valoare;
    moneda.greutate = greutate;
    moneda.an = an;
    moneda.tara = (char*)malloc(sizeof(char) * (strlen(tara) + 1));
    strcpy(moneda.tara, tara);
    return moneda;
}

void afisareMoneda(Moneda moneda) {
    printf("%s, %d, %f, %d\n", moneda.tara, moneda.valoare, moneda.greutate, moneda.an);
}

Nod* inserareInceput(Nod* cap, Moneda moneda) {
    Nod* nou = malloc(sizeof(Nod));
    nou->info = moneda;
    nou->next = cap;
    return nou;
}

void afisareLista(Nod* lista) {
    while (lista) {
        afisareMoneda(lista->info);
        lista = lista->next;
    }
}

void stergereLista(Nod** lista) {
    while (*lista != NULL) {
        Nod* temp;
        temp = *lista;
        *lista = (*lista)->next;
        free(temp->info.tara);
        free(temp);
    }
}


void main()
{
    Moneda moneda1 = initializareMoneda("RO", 1, 2.5, 2020);
    Moneda moneda2 = initializareMoneda("ENG", 2, 2, 2021);

    Nod* lista = NULL;
    lista = inserareInceput(lista, moneda1);
    lista = inserareInceput(lista, moneda2);
    afisareLista(lista);
    stergereLista(&lista);
   
}