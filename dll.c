#include <stdio.h>
#include <malloc.h>
typedef struct Santier Santier;
typedef struct Nod Nod;
typedef struct Nodldi Nodldi;
typedef struct ListaDubla ListaDubla;

struct Santier {
    char* numeProiect;
    int nrMuncitori;
    float suprafata;
};

struct Nod {
    Santier santier;
    Nod* next;
};

struct Nodldi {
    Nodldi* prev;
    Nodldi* next;
    Santier info;
};

struct ListaDubla {
    Nodldi* prim;
    Nodldi* ultim;
};

void inserareInceput(ListaDubla* lista, Santier s) {
    Nodldi* aux = malloc(sizeof(Nodldi));
    aux->info = s;
    aux->next = lista->prim;
    aux->next = NULL;
    if (lista->prim != NULL) {
        lista->prim->prev = aux;
        lista->prim = aux;
    }
    else {
        
        lista->prim->next = aux;
    }
    lista->prim = aux;

}

//afisre LDI de la primul al ultimul
void afisareLDI(ListaDubla lista) {
    for (Nodldi* nod = lista.prim; nod != NULL; nod = nod->next) {
        afisareSantier(nod->info);
    }
    printf("\n");
}

float densitateMuncitori(Santier santier) {
    if (santier.suprafata > 0) {
        return santier.nrMuncitori / santier.suprafata;
    }
    else return 0;
}

void stergeSantierDupaNume(ListaDubla* list, const char* numeSantier) {
    Nodldi* nod=list->prim;

    while (nod != NULL) {
        if (strcmp(nod->info.numeProiect, numeSantier) == 0) {
            if (nod->prev == NULL) { //daca este primul nod
                if (nod->next == NULL) {
                    list->prim = NULL;
                    list->ultim = NULL;
                }
                else {
                    nod->next->prev = NULL;
                    list->prim = nod->next;
                }
            }
            else {
                if (nod->next == NULL) {
                    nod->prev->next = NULL;
                    list->ultim = nod->prev;
                }
                else {
                    nod->prev->next = nod->next;
                    nod->next->prev = nod->prev;
                }
            }
            free(nod->info.numeProiect);
            free(nod);
            //break;

            nod = NULL;
        }
        else {
            nod = nod->next;
        }
    }
    nod = nod->next;
}

Santier initializareSantier(const char* numeProiect, int nrMuncitori, float suprafata) {
    Santier santier;
    santier.numeProiect = (char*)malloc(strlen(numeProiect) + 1);
    strcpy(santier.numeProiect, numeProiect);
    santier.nrMuncitori = nrMuncitori;
    santier.suprafata = suprafata;
    return santier;
}



void afisareSantier(Santier santier)
{
    printf("Santierul %s are %d muncitori si %.2f m^2 suprafata", santier.numeProiect, santier.nrMuncitori, santier.suprafata);
    printf("\n");
}

void stergereCompelt(ListaDubla* lista) {
    if (lista != NULL) {
     
            Nodldi* aux = lista->prim;
            while (aux != NULL) {
                aux = aux->next;
                free(aux->info.numeProiect);
                Nodldi* temp = aux->next;
                free(aux);
                aux = temp; //SAU temp = aux->prev
            }
            lista->prim = NULL;
            lista->ultim = NULL;
        }
}

int calculNrMunctiroi(ListaDubla lista, floa)


int main() {
    Santier s1 = initializareSantier("Santier 1", 10, 301);
    Santier s2 = initializareSantier("Santier 2", 11, 302);
    Santier s3 = initializareSantier("Santier 3", 12, 303);
    ListaDubla lista;
    Nodldi* prim = NULL;
    Nodldi* ultim = NULL;
    inserareInceput(&lista, s1);
    inserareInceput(&lista, s2);
    inserareInceput(&lista, s3);
    afisareLDI(lista);
    stergesantierdupanume(&lista, "santier 2");
    afisareldi(lista);
    stergesantierdupanume(&lista, "santier 1");
    afisareldi(lista);
    stergesantierdupanume(&lista, "santier 3");
    afisareldi(lista);
}