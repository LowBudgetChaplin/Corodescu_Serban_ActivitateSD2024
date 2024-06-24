#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NodLista NodLista;
typedef struct ListaDubla ListaDubla;
typedef struct Reteta Reteta;
typedef struct NodBST NodBST;

struct Reteta {
    unsigned int id;
    char* numePacient;
    char* numeMedic;
    char* statut;
    unsigned char nrMedicamente;
    float compensare;
};

struct NodLista {
    Reteta info;
    NodLista* next;
    NodLista* prev;
};

struct ListaDubla {
    NodLista* cap;
    NodLista* coada;
};

struct NodBST {
    Reteta info;
    NodBST* st;
    NodBST* dr;
};

void afisareReteta(Reteta c) {
    printf("Reteta cu id %u, al pacientului %s, medicul %s, cu statusul %s, nr medicamente %hhu si compensatia %.2f\n",
        c.id, c.numePacient, c.numeMedic, c.statut, c.nrMedicamente, c.compensare);
}

void inserareNodLista(ListaDubla* lista, Reteta r) {
    NodLista* nod = (NodLista*)malloc(sizeof(NodLista));
    if (!nod) {
        perror("Eroare la alocarea memoriei pentru nod");
        exit(1);
    }
    nod->info = r;
    nod->next = NULL;
    nod->prev = lista->coada;
    if (lista->coada != NULL) {
        lista->coada->next = nod;
    }
    else {
        lista->cap = nod;
    }
    lista->coada = nod;
}

Reteta citireReteta(FILE* f) {
    Reteta r;
    char buffer[256];

    if (fgets(buffer, 256, f) == NULL) {
        perror("Eroare la citirea id-ului");
        exit(1);
    }
    sscanf(buffer, "%u", &r.id);

    if (fgets(buffer, 256, f) == NULL) {
        perror("Eroare la citirea numePacient");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    r.numePacient = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    if (!r.numePacient) {
        perror("Eroare la alocarea memoriei pentru numePacient");
        exit(1);
    }
    strcpy(r.numePacient, buffer);

    if (fgets(buffer, 256, f) == NULL) {
        perror("Eroare la citirea numeMedic");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    r.numeMedic = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    if (!r.numeMedic) {
        perror("Eroare la alocarea memoriei pentru numeMedic");
        exit(1);
    }
    strcpy(r.numeMedic, buffer);

    if (fgets(buffer, 256, f) == NULL) {
        perror("Eroare la citirea statut");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    r.statut = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    if (!r.statut) {
        perror("Eroare la alocarea memoriei pentru statut");
        exit(1);
    }
    strcpy(r.statut, buffer);

    if (fgets(buffer, 256, f) == NULL) {
        perror("Eroare la citirea nrMedicamente");
        exit(1);
    }
    sscanf(buffer, "%hhu", &r.nrMedicamente);

    if (fgets(buffer, 256, f) == NULL) {
        perror("Eroare la citirea compensare");
        exit(1);
    }
    sscanf(buffer, "%f", &r.compensare);

    return r;
}

ListaDubla* citireLista(const char* file) {
    ListaDubla* lista = (ListaDubla*)malloc(sizeof(ListaDubla));
    if (!lista) {
        perror("Eroare la alocarea memoriei pentru lista");
        exit(1);
    }
    lista->cap = NULL;
    lista->coada = NULL;
    if (file != NULL && strlen(file) != 0) {
        FILE* f = fopen(file, "r");
        if (f != NULL) {
            int dim = 0;
            fscanf(f, "%d\n", &dim);
            for (int i = 0; i < dim; i++) {
                Reteta r = citireReteta(f);
                inserareNodLista(lista, r);
            }
            fclose(f);
        }
        else {
            perror("Error opening file");
            free(lista);
            exit(1);
        }
    }
    return lista;
}

void afisareLista(ListaDubla* lista) {
    NodLista* nod = lista->cap;
    while (nod) {
        afisareReteta(nod->info);
        nod = nod->next;
    }
}

void eliberareLista(ListaDubla* lista) {
    NodLista* current = lista->cap;
    while (current) {
        NodLista* temp = current;
        current = current->next;
        free(temp->info.numePacient);
        free(temp->info.numeMedic);
        free(temp->info.statut);
        free(temp);
    }
    lista->cap = NULL;
    lista->coada = NULL;
}

int nrMedicamentePestePrag(ListaDubla* lista, int prag) {
    NodLista* nod = lista->cap;
    int nrMedicamente = 0;
    while (nod) {
        if (nod->info.nrMedicamente > prag) {
            nrMedicamente++;
        }
        nod = nod->next;
    }
    return nrMedicamente;
}

void actualizareProcent(ListaDubla* lista, float procent, const char* pacient) {
    NodLista* nod = lista->cap;
    while (nod) {
        if (strcmp(nod->info.numePacient, pacient) == 0) {
            nod->info.compensare = procent;
        }
        nod = nod->next;
    }
}

void stergeNoduri(ListaDubla* lista, unsigned char prag) {
    NodLista* current = lista->cap;
    while (current) {
        NodLista* next = current->next;
        if (current->info.nrMedicamente < prag) {
            if (current->prev) {
                current->prev->next = current->next;
            }
            else {
                lista->cap = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            else {
                lista->coada = current->prev;
            }
            free(current->info.numePacient);
            free(current->info.numeMedic);
            free(current->info.statut);
            free(current);
        }
        current = next;
    }
}

void inserareInArbore(NodBST** rad, Reteta r) {
    if (*rad) {
        if ((*rad)->info.id > r.id) {
            inserareInArbore(&((*rad)->st), r);
        }
        else {
            inserareInArbore(&((*rad)->dr), r);
        }
    }
    else {
        NodBST* nod = (NodBST*)malloc(sizeof(NodBST));
        if (!nod) {
            exit(1);
        }
        nod->dr = NULL;
        nod->st = NULL;

        nod->info.id = r.id;
        nod->info.numePacient = (char*)malloc(strlen(r.numePacient) + 1);
        strcpy(nod->info.numePacient, r.numePacient);
        nod->info.numeMedic = (char*)malloc(strlen(r.numeMedic) + 1);
        strcpy(nod->info.numeMedic, r.numeMedic);
        nod->info.statut = (char*)malloc(strlen(r.statut) + 1);
        strcpy(nod->info.statut, r.statut);

        nod->info.nrMedicamente = r.nrMedicamente;
        nod->info.compensare = r.compensare;
        *rad = nod;
    }
}

void afisareInOrdine(NodBST* rad) {
    if (rad) {
        afisareInOrdine(rad->st);
        afisareReteta(rad->info);
        afisareInOrdine(rad->dr);
    }
}

void eliberareArbore(NodBST* rad) {
    if (rad) {
        eliberareArbore(rad->st);
        eliberareArbore(rad->dr);
        free(rad->info.numePacient);
        free(rad->info.numeMedic);
        free(rad->info.statut);
        free(rad);
    }
}

NodBST* salvareInArboreDinListaDubla(ListaDubla* lista) {
    NodBST* rad = NULL;
    NodLista* nod = lista->cap;
    while (nod) {
        inserareInArbore(&rad, nod->info);
        nod = nod->next;
    }
    return rad;
}

int main() {
    ListaDubla* lista = citireLista("retete.txt");
    if (!lista) {
        fprintf(stderr, "Eroare la citirea listei de retete\n");
        return 1;
    }

    afisareLista(lista);
    int prag = 1;
    int nrMedicamente = nrMedicamentePestePrag(lista, prag);
    printf("\n\nNr de medicamente peste pragul de %d este: %d\n", prag, nrMedicamente);

    const char* nume = "Serban Corodescu";
    float procent = 60.02;
    actualizareProcent(lista, procent, nume);
    printf("\n\nLista cu actualizare compensatie pacient:\n");
    afisareLista(lista);

    unsigned char pragMinim = 2;
    stergeNoduri(lista, pragMinim);
    printf("\n\nLista cu pacienti peste %hhu medicamente:\n", pragMinim);
    afisareLista(lista);

    NodBST* radacina = salvareInArboreDinListaDubla(lista);
    printf("\n\nDatele salvate din lista dubla in arborele binar de cautare sunt:\n");
    afisareInOrdine(radacina);

    eliberareLista(lista);
    eliberareArbore(radacina);
    free(lista);

    return 0;
}
