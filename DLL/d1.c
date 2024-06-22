#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
typedef struct Comanda Comanda;
typedef struct NodABC NodABC;
typedef struct ABC ABC;

struct Comanda {
    unsigned int id;
    char* statusComanda;
    char* numeClient;
    unsigned char nrProduse;
    float sumaPlata;
};

struct Nod {
    Comanda info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* cap;
    Nod* coada;
};

struct NodABC {
    NodABC* st;
    NodABC* dr;
    Comanda info;
};

struct ABC {
    NodABC* root;
};

void afisareComanda(Comanda c) {
    printf("Proiectul cu id %u, statusul comenzii %s, clientul %s, cu nr de produse %hhu si suma de plata de %.2f RON\n",
        c.id, c.statusComanda, c.numeClient, c.nrProduse, c.sumaPlata);
}

void inserare(ListaDubla* lista, Comanda c) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    if (!nod) {
        perror("Failed to allocate memory for node");
        exit(1);
    }
    nod->info = c;
    nod->next = NULL;
    nod->prev = lista->coada;
    if (lista->coada) {
        lista->coada->next = nod;
    }
    else {
        lista->cap = nod;
    }
    lista->coada = nod;
}

void afisareListaDubla(ListaDubla* lista) {
    Nod* temp = lista->cap;
    while (temp) {
        afisareComanda(temp->info);
        temp = temp->next;
    }
}

Comanda citireComanda(FILE* f) {
    Comanda c;
    char buffer[256];

    if (fgets(buffer, sizeof(buffer), f) == NULL) exit(1);
    sscanf(buffer, "%u", &c.id);

    if (fgets(buffer, sizeof(buffer), f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    c.statusComanda = (char*)malloc(strlen(buffer) + 1);
    if (!c.statusComanda) {
        perror("Failed to allocate memory for statusComanda");
        exit(1);
    }
    strcpy(c.statusComanda, buffer);

    if (fgets(buffer, sizeof(buffer), f) == NULL) exit(1);
    buffer[strcspn(buffer, "\n")] = 0;
    c.numeClient = (char*)malloc(strlen(buffer) + 1);
    if (!c.numeClient) {
        perror("Failed to allocate memory for numeClient");
        exit(1);
    }
    strcpy(c.numeClient, buffer);

    if (fgets(buffer, sizeof(buffer), f) == NULL) exit(1);
    sscanf(buffer, "%hhu", &c.nrProduse);

    if (fgets(buffer, sizeof(buffer), f) == NULL) exit(1);
    sscanf(buffer, "%f", &c.sumaPlata);

    return c;
}

ListaDubla* citireLista(const char* file) {
    ListaDubla* lista = (ListaDubla*)malloc(sizeof(ListaDubla));
    if (!lista) {
        perror("Failed to allocate memory for ListaDubla");
        exit(1);
    }
    lista->cap = NULL;
    lista->coada = NULL;
    if (file && strlen(file) != 0) {
        FILE* f = fopen(file, "r");
        if (f) {
            int dim = 0;
            fscanf(f, "%d\n", &dim);
            for (int i = 0; i < dim; i++) {
                Comanda c = citireComanda(f);
                inserare(lista, c);
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

void stergeListaDubla(ListaDubla* lista) {
    Nod* nod = lista->cap;
    while (nod) {
        Nod* temp = nod;
        nod = nod->next;
        free(temp->info.statusComanda);
        free(temp->info.numeClient);
        free(temp);
    }
    lista->cap = NULL;
    lista->coada = NULL;
}

float valoareMedie(ListaDubla* lista, const char* stadiu) {
    Nod* nod = lista->cap;
    float valTotala = 0.0;
    int counter = 0;
    while (nod) {
        if (strcmp(nod->info.statusComanda, stadiu) == 0) {
            valTotala += nod->info.sumaPlata;
            counter++;
        }
        nod = nod->next;
    }
    return counter ? valTotala / counter : 0;
}

int existaIdDuplicat(ListaDubla* lista) {
    Nod* nod = lista->cap;
    while (nod) {
        Nod* checker = nod->next;
        while (checker) {
            if (nod->info.id == checker->info.id) {
                return 1;
            }
            checker = checker->next;
        }
        nod = nod->next;
    }
    return 0;
}

int existaClientiUnici(ListaDubla* lista) {
    Nod* nod = lista->cap;
    int counter = 0;
    while (nod) {
        int gasit = 0;
        Nod* checker = lista->cap;
        while (checker != nod) {
            if (strcmp(nod->info.numeClient, checker->info.numeClient) == 0) {
                gasit = 1;
                break;
            }
            checker = checker->next;
        }
        if (!gasit) {
            counter++;
        }
        nod = nod->next;
    }
    return counter;
}

NodABC* inserareInABC(NodABC* rad, Comanda c) {
    if (rad) {
        if (rad->info.id > c.id) {
            rad->st = inserareInABC(rad->st, c);
        }
        else {
            rad->dr = inserareInABC(rad->dr, c);
        }
    }
    else {
        NodABC* nou = (NodABC*)malloc(sizeof(NodABC));
        if (!nou) {
            perror("Failed to allocate memory for NodABC");
            exit(1);
        }
        nou->dr = NULL;
        nou->st = NULL;
        nou->info = c;
        rad = nou;
    }
    return rad;
}

void traversareABC(NodABC* rad) {
    if (rad) {
        traversareABC(rad->st);
        afisareComanda(rad->info);
        traversareABC(rad->dr);
    }
}

ABC* salvareInABC(ListaDubla* lista, float prag) {
    ABC* abc = (ABC*)malloc(sizeof(ABC));
    if (!abc) {
        perror("Failed to allocate memory for ABC");
        exit(1);
    }
    abc->root = NULL;
    Nod* nod = lista->cap;
    while (nod) {
        if (nod->info.sumaPlata > prag) {
            abc->root = inserareInABC(abc->root, nod->info);
        }
        nod = nod->next;
    }
    return abc;
}

void stergeABC(NodABC* rad) {
    if (rad) {
        stergeABC(rad->st);
        stergeABC(rad->dr);
        free(rad->info.numeClient);
        free(rad->info.statusComanda);
        free(rad);
    }
}

int main() {
    ListaDubla* listaDubla = citireLista("comenzi.txt");
    if (!listaDubla) {
        perror("Failed to create ListaDubla");
        return 1;
    }

    afisareListaDubla(listaDubla);
    const char* stadiuCautat = "Completed";
    float valMedie = valoareMedie(listaDubla, stadiuCautat);
    printf("\nValoarea medie a comenzilor livrate este: %.2f\n", valMedie);

    int duplicat = existaIdDuplicat(listaDubla);
    printf("Exista ID-uri duplicate: %s\n", duplicat ? "DA" : "NU");

    int nrClientiUnici = existaClientiUnici(listaDubla);
    printf("Nr de clienti unici este: %d\n", nrClientiUnici);

    float prag = 100.5;
    //ABC* radacina = salvareInABC(listaDubla, prag);
    //if (!radacina) {
    //    perror("Failed to create ABC");
    //    stergeListaDubla(listaDubla);
    //    free(listaDubla);
    //    return 1;
    //}

    //printf("Comenzile salvate in ABC peste pragul %.2f sunt:\n", prag);
    //traversareABC(radacina->root);

    stergeListaDubla(listaDubla);
    free(listaDubla);

    //stergeABC(radacina->root);
    //free(radacina);

    return 0;
}
