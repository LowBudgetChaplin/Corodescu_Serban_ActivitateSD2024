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

void inserareNod(Nod** rad, Proiect p) {
    if (*rad) {
        if ((*rad)->info.id > p.id) {
            inserareNod(&((*rad)->st), p);
        }
        else {
            inserareNod(&((*rad)->dr), p);
        }
    }
    else {
        Nod* nou = (Nod*)malloc(sizeof(Nod));
        nou->dr = NULL;
        nou->st = NULL;
        nou->info = p;
        *rad = nou;
    }
}

Proiect citireProiect(FILE* f) {
    Proiect p;
    char buffer[256];

    if(fgets(buffer, 256, f)== NULL) return p;
    sscanf(buffer, "%u", &p.id);

    if (fgets(buffer, 256, f) == NULL) return p;
    buffer[strcspn(buffer, "\n")] = 0;
    p.titluProiect = (char*)malloc(strlen(buffer) + 1);
    strcpy(p.titluProiect, buffer);

    if (fgets(buffer, 256, f) == NULL) return p;
    buffer[strcspn(buffer, "\n")] = 0;
    p.contractant = (char*)malloc(strlen(buffer) + 1);
    strcpy(p.contractant, buffer);

    if (fgets(buffer, 256, f) == NULL) return p;
    sscanf(buffer, "%hhu", &p.durata);

    if (fgets(buffer, 256, f) == NULL) return p;
    sscanf(buffer, "%f", &p.buget);

    if (fgets(buffer, 256, f) == NULL) return p;
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
                inserareNod(&rad, p);
            }
        }
        fclose(f);
    }

    return rad;
}

void afisareArboreInOrdine(Nod* rad) {
    if (rad) {
        afisareArboreInOrdine(rad->st);
        afisareProiect(rad->info);
        afisareArboreInOrdine(rad->dr);
    }
}

void dezalocare(Nod* rad) {
    if (rad) {
        dezalocare(rad->st);
        dezalocare(rad->dr);
        free(rad->info.titluProiect);
        free(rad->info.contractant);
        free(rad);
    }
}

int detProiectContractantComun(Nod* rad, const char* contractant) {
    if (!rad) return 0;

    int count = 0;
    if (strcmp(rad->info.contractant, contractant) == 0) {
        count = 1;
    }
    return count
        + detProiectContractantComun(rad->st, contractant)
        + detProiectContractantComun(rad->dr, contractant);
}

Nod* cautaNod(Nod* rad, unsigned int id) {
    if (!rad) return NULL;
    if (rad->info.id == id) return rad;
    if (rad->info.id > id) return cautaNod(rad->st, id);
    return cautaNod(rad->dr, id);
}

void modificaStadiul(Nod* rad, unsigned int id, float procent) {
    Nod* nod = cautaNod(rad, id);
    if (nod) {
        nod->info.stadiu = procent;
    }
    else {
        printf("Nodul nu a fost gasit!");
    }
}

Nod* stergeNoduriFrunza(Nod* rad) {
    if (!rad) return NULL;
    if (!rad->st && !rad->dr) {
        free(rad->info.titluProiect);
        free(rad->info.contractant);
        free(rad);
        return NULL;
    }

    rad->st = stergeNoduriFrunza(rad->st);
    rad->dr = stergeNoduriFrunza(rad->dr);
    
    return rad;
}

void colectareProiect(Nod* rad, float prag, Proiect*** vector, int* dim) {
    if (rad) {
        colectareProiect(rad->st, prag, vector, dim);
        if (rad->info.buget > prag) {
            (*dim)++;
            *vector = (Proiect**)realloc(*vector, (*dim) * sizeof(Proiect*));
            (*vector)[*dim - 1] = &rad->info;
        }
        colectareProiect(rad->dr, prag, vector, dim);
    }
}

Proiect** obtineProiectCuBugetMare(Nod* rad, float prag, int* dim) {
    Proiect** vector = NULL;
    *dim = 0;
    colectareProiect(rad, prag, &vector, dim);
    return vector;
}

int inaltimeArbore(Nod* rad) {
    if (!rad) return 0;
    int inaltimeStanga = inaltimeArbore(rad->st);
    int inaltimeDreapta = inaltimeArbore(rad->dr);

    return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta);
}

//void afisareStructuraArbore(Nod* rad, int nivel) {
//    if (rad) {
//        afisareStructuraArbore(rad->dr, nivel + 1);
//        for (int i = 0; i < nivel; i++) printf("   ");
//        printf("%u\n", rad->info.id);
//        afisareStructuraArbore(rad->st, nivel + 1);
//    }
//}

Nod* cautaNodCuDurata(Nod* rad, unsigned int id) {
    if (!rad) return NULL;
    if (rad->info.id == id) return rad;
    if (rad->info.id > id) return cautaNodCuDurata(rad->st, id);
    return cautaNodCuDurata(rad->dr, id);
}

void modificareDurata(Nod* rad, unsigned int id, unsigned char durataNoua) {
    Nod* nod = cautaNodCuDurata(rad, id);
    if (nod) {
        nod->info.durata = durataNoua;
    }
}

int nrTotalProiectePestePrag(Nod* rad, float pragBuget) {
        if (!rad) return 0;

        int count = 0;
        if (rad->info.buget > pragBuget) {
            count = 1;
        }

        return count + nrTotalProiectePestePrag(rad->st, pragBuget)+nrTotalProiectePestePrag(rad->dr, pragBuget);
}

Nod* gasesteMin(Nod* rad, float prag) {
    Nod* nod = rad;
    while (rad && nod->st) {
        nod = nod->st;
    }
    return nod;
}

Nod* stergeProiecteCuSTadiuSubPrag(Nod* rad, float prag) {
    if (!rad) return NULL;
        
    rad->st = stergeProiecteCuSTadiuSubPrag(rad->st, prag);
    rad->dr = stergeProiecteCuSTadiuSubPrag(rad->dr, prag);

    if (rad->info.stadiu < prag) {
        Nod* temp;
        if (!rad->st) {
            temp = rad->dr;
            free(rad->info.contractant);
            free(rad->info.titluProiect);
            free(rad);
            return temp;
        }
        else if (!rad->dr) {
            temp = rad->st;
            free(rad->info.contractant);
            free(rad->info.titluProiect);
            free(rad);
            return temp;
        }
        else {
            temp = gasesteMin(rad, prag);
            rad->info = temp->info;
            temp->dr = stergeProiecteCuSTadiuSubPrag(rad->dr, prag);
        }
    }

    return rad;
    
}

int nrTotalProiecte(Nod* rad) {
    if (!rad) return 0;
    return 1 + nrTotalProiecte(rad->st) + nrTotalProiecte(rad->dr);
}

void main() {
    Nod* radacina = citireArbore("proiect.txt");
    printf("Arborele initial:\n");
    afisareArboreInOrdine(radacina);

    const char* contractant = "Germania";
    int proiecteComune = detProiectContractantComun(radacina, contractant);
    printf("\n\nNr de proiecte comune: %d", proiecteComune);

    int inaltime = inaltimeArbore(radacina);
    printf("\n\nInaltime arbore: %d\n", inaltime);

    printf("\n\n\n");
    int nrProiecte = nrTotalProiecte(radacina);
    printf("\nNr total proiecte: %d", nrProiecte);

    //float procent = 100;
    //unsigned int id = 4;
    //modificaStadiul(radacina, id, procent);
    //printf("\nArborele dupa update stadiu:\n");
    //afisareArboreInOrdine(radacina);

    //stergeNoduriFrunza(radacina);
    //printf("\nArborele dupa stergere nod frunza:\n");
    //afisareArboreInOrdine(radacina);

    //printf("\n\n\n");
    //int dim = 3;
    //float prag = 12000;
    //Proiect** vector = obtineProiectCuBugetMare(radacina, prag, &dim);
    //for (int i = 0; i < dim; i++) {
    //    afisareProiect(*vector[i]);
    //}

    printf("\n\n\n");
    unsigned int durataNoua = 88;
    unsigned int idCautat = 5;
    modificareDurata(radacina, idCautat, durataNoua);
    afisareArboreInOrdine(radacina);


    printf("\n\n\n");
    float prag = 12000.01;
    int bugetCautat = nrTotalProiectePestePrag(radacina, prag);
    printf("\nNr proiete peste pragul de 1200$ este: %d", bugetCautat);


    printf("\n\n\n");
    float pragStadiu = 72.01;
    radacina = stergeProiecteCuSTadiuSubPrag(radacina, pragStadiu);
    afisareArboreInOrdine(radacina);

    
    //afisareStructuraArbore(radacina, 0);

    dezalocare(radacina);
    contractant = NULL;
    //free(vector);
}
