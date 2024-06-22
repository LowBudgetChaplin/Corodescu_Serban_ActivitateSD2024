#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Proiect Proiect;
typedef struct Nod Nod;

struct Proiect {
    unsigned int id;
    char* titluProiect;
    char* contractant;
    unsigned char durata;
    float buget;
    float stadiu;
};

struct Nod {
    Nod* st;
    Nod* dr;
    Proiect info;
};

void afisareProiect(Proiect p) {
    printf("Proiectul cu id %u, titlul %s, nume contractant %s, cu o durata de %hhu zile, cu bugetul de %.2f si stadiul de %.2f%%\n",
        p.id, p.titluProiect, p.contractant, p.durata, p.buget, p.stadiu);
}

void inserareInArbore(Nod** rad, Proiect p) {
    if (*rad) {
        if ((*rad)->info.id > p.id) {
            inserareInArbore(&((*rad)->st), p);
        }
        else {
            inserareInArbore(&((*rad)->dr), p);
        }
    }
    else {
        Nod* nod = (Nod*)malloc(sizeof(Nod));
        nod->dr = NULL;
        nod->st = NULL;
        nod->info = p;
        *rad = nod;
    }
}

void afisareInOrdine(Nod* rad) {
    if (rad) {
        afisareInOrdine(rad->st);
        afisareProiect(rad->info);
        afisareInOrdine(rad->dr);
    }
}

Proiect citireProiect(FILE* f) {
    Proiect p;
    char buffer[256];

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%u", &p.id);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    p.titluProiect = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(p.titluProiect, buffer);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    p.contractant = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
    strcpy(p.contractant, buffer);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%hhu", &p.durata);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%f", &p.buget);

    if (fgets(buffer, 256, f) == NULL) exit(1);
    sscanf(buffer, "%f", &p.stadiu);

    return p;
}

Nod* citireArbore(const char* file) {
    Nod* rad = NULL;
    if (file && strlen(file) != 0) {
        FILE* f = fopen(file, "r");
        if (f != NULL) {
            int dim = 0;
            fscanf(f, "%d\n", &dim);
            for (int i = 0; i < dim; i++) {
                Proiect p = citireProiect(f);
                inserareInArbore(&rad, p);
            }
            fclose(f);
        }
    }
    return rad;
}

void dezalocareArbore(Nod* rad) {
    if (rad) {
        dezalocareArbore(rad->st);
        dezalocareArbore(rad->dr);
        free(rad->info.titluProiect);
        free(rad->info.contractant);
        free(rad);
    }
}

int detProiecteAcelasiContractant(Nod* rad, const char* contractant) {
    if (rad) {
        int counter = 0;
        if (strcmp(rad->info.contractant, contractant)==0) {
            counter = 1;
        }
        return counter + detProiecteAcelasiContractant(rad->st, contractant) + detProiecteAcelasiContractant(rad->dr, contractant);
    }
    else {
        return 0;
    }
}

Nod* cautaDupaId(Nod* nod, unsigned int id) {
    if (!nod) return NULL;
    if (nod->info.id == id) return nod;
    if (nod->info.id > id) return cautaDupaId(nod->st, id);
    return cautaDupaId(nod->dr, id);
}

void modificaStadiu(Nod* rad, unsigned int id, float procent) {
    Nod* nod = cautaDupaId(rad, id);
    if (nod) {
            nod->info.stadiu = procent;
    }
    else {
        printf("Proiectul cu id-ul cautat nu exista!\n");
    }
}

Nod* stergeNoduriFrunza(Nod* rad) {
    if (!rad) return NULL;
    if (rad) {
        if (!rad->st && !rad->dr) {
            free(rad->info.titluProiect);
            free(rad->info.contractant);
            free(rad);
            return NULL;
        }
    }
    rad->st = stergeNoduriFrunza(rad->st);
    rad->dr = stergeNoduriFrunza(rad->dr);

    return rad;
}

Nod* cautaMin(Nod* rad) {
    Nod* nod = rad;
    while (nod && nod->st != NULL) {
        nod = nod->st;
    }
    
    return nod;
}

Nod* stergeRadacina(Nod* rad) {
    if (!rad) return rad;
    if (!rad->st) {
        Nod* temp = rad->dr;
        free(rad->info.titluProiect);
        free(rad->info.contractant);
        free(rad);
        return temp;
    }
    if(!rad->dr) {
        Nod* temp = rad->st;
        free(rad->info.titluProiect);
        free(rad->info.contractant);
        free(rad);
        return temp;
    }

    Nod* temp = cautaMin(rad->dr);
    rad->info = temp->info;
    temp->dr = stergeRadacina(rad->dr);
    
    return rad;
}

void salvareProiectInVector(Nod* rad ,float prag, Proiect*** vector, int* dim) {
    if (rad) {
        salvareProiectInVector(rad->st, prag, vector, dim);
        if (rad->info.buget > prag) {
            (*dim)++;
            *vector = (Proiect**)realloc(*vector, (*dim) * sizeof(Proiect*));
            (*vector)[*dim - 1] = &rad->info;
        }
        salvareProiectInVector(rad->dr, prag, vector, dim);
    }
}

Proiect** obtineProiectBugetMare(Nod* rad, float prag, int* dim) {
    Proiect** vector = NULL;
    *dim = 0;
    salvareProiectInVector(rad, prag, &vector, dim);
    return vector;
}

int calculInaltimeArbore(Nod* rad) {
    if (!rad) return 0;
    int inaltimeStanga = calculInaltimeArbore(rad->st);
    int inaltimeDreapta = calculInaltimeArbore(rad->dr);

    return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta);
}


void main() {
    Nod* radacina = citireArbore("proiect.txt");

    const char* contractantComun = "Germania 1";
    int nrProiecteComune = detProiecteAcelasiContractant(radacina, contractantComun);
    printf("\nNr de proiecte cu numele %s este: %d\n\n", contractantComun,nrProiecteComune);
    afisareInOrdine(radacina);

    printf("\n\nAfisare dupa modificare stadiu:\n\n");
    unsigned int id = 2;
    float procentNou = 55.51;
    modificaStadiu(radacina, id, procentNou);
    
    afisareInOrdine(radacina);
    printf("\n\n\n");

    //printf("\n\nAfisare dupa stergere nod frunza:\n\n");
    //stergeNoduriFrunza(radacina);


    //printf("\n\nAfisare dupa stergere radacina:\n\n");
    //radacina = stergeRadacina(radacina);
    //afisareInOrdine(radacina);

    int dim = 3;
    float prag = 14000.00;
    Proiect** vector = obtineProiectBugetMare(radacina, prag, &dim);
    printf("Proiecte salvate in vector din arborele binar de cautare:\n");
    for (int i = 0; i < dim; i++) {
        afisareProiect(*vector[i]);
    }

    int inaltimeABC = calculInaltimeArbore(radacina);
    printf("\n\n\nInaltime arbore: %d", inaltimeABC);



    dezalocareArbore(radacina);
    radacina = NULL;
}