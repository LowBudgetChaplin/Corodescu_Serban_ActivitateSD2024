#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct Masina Masina;
typedef struct Nod Nod;

struct Masina {
	char* marca;
	int anFabricatie;
	float pret;
};

struct Nod {
	Masina info;
	Nod* next;
};


Masina initMasina(const char* marca, int anFabricatie, float pret) {
	Masina masina;

	masina.anFabricatie = anFabricatie;
	masina.pret = pret;
	masina.marca = (char*)malloc((strlen(marca) + 1) * sizeof(char));
	strcpy(masina.marca, marca);

	return masina;
}

void afisareMasina(Masina masina) {
	printf("Marca %s, an fabricatie %i, pret %5.2f RON\n",
		masina.marca, masina.anFabricatie, masina.pret);
}

Nod* inserareInceput(Nod* cap, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = cap;
	return nou;
}

void inserareSfarsit(Nod** cap, Masina masina) {
	if(*cap != NULL){
		Nod* temp = *cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}

		Nod* nodNou = (Nod*)malloc(sizeof(Nod));
		nodNou->info = masina;
		nodNou->next = NULL;
		temp->next = nodNou;
	}
	else {
		*cap = malloc(sizeof(Nod));
		(*cap)->info = masina;
		(*cap)->next = NULL;
	}
}

void afisareLista(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

//1.Implementati o functie care sterge un nod de pe o pozitie data ca parametru. 
//Daca lista are mai putine noduri decat index-ul dat, nu se realizeaza stergerea.

void stergeNod(Nod **cap, int poz) {
	if (*cap == NULL) {
		return 1;
	}

	Nod* temp = *cap;
	Nod* prev = NULL;

	if (poz == 0) {
		*cap = temp->next;
		free(temp->info.marca);
		free(temp);
		return 2;
	}

	if (temp == NULL) {
		return 3;
	}

	int count=0;
	while (*cap != NULL && count < poz) {
		count++;
		prev = temp;
		temp = temp->next;

	}

	prev->next = temp->next;
	free(temp->info.marca);
	free(temp);

}

//2. Implementati o functie care sa insereze elementele in cadrul listei simplu 
//inlantuite astfel incat acestea sa fie sortate crescator dupa 
//un camp la alegerea voastra.

void inserareDupaAn(Nod** cap, Masina masina) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masina;
	nodNou->next = NULL;

	if (*cap == NULL || (*cap)->info.anFabricatie >= masina.anFabricatie) {
		nodNou->next = *cap;
		*cap = nodNou;
	}
	else
	{
		Nod* temp = *cap;
		while (temp->next != NULL && temp->next->info.anFabricatie < masina.anFabricatie) {
			temp = temp->next;
		}
		nodNou->next = temp->next;
		temp->next = nodNou;
	}
	
}

//3. Implementati o functie care salveaza intr-un vector toate obiectele care 
//indeplinesc o conditie stabilita de voi. 
//Realizati deep copy, astfel incat elementele din 
//vector sa fie diferentiate de cele din lista. Aveti grija 
//la alocarea de spatiu, deoarece trebuie sa stabiliti pentru 
//cate elemente veti aloca spatiu.





//4. Implementati o functie care primeste lista si doua pozitii. 
//In cadrul functiei trebuie sa interschimbati elementele de pe cele 
//doua pozitii din lista primita.

void interschimbare(Nod* cap, int poz1, int poz2) {
	if (poz1 == poz2 || poz1 < 0 || poz2 < 0){
		return 1;
	}

	Nod* nod1 = cap;
	Nod* prev1 = NULL;
	for (int i = 0; nod1 != NULL && i < poz1; i++) {
		prev1 = nod1;
		nod1 = nod1->next;
	}

	Nod* nod2 = cap;
	Nod* prev2 = NULL;
	for (int i = 0; nod2 != NULL && i < poz2; i++) {
		prev2 = nod2;
		nod2 = nod2->next;
	}

	if (prev1 != NULL) {
		prev1->next = nod2;
	}
	else {
		cap = nod2;
	}

	if (prev2 != NULL) {
		prev2->next = nod1;
	}
	else {
		cap = nod1;
	}

	Nod* temp = nod2->next;
	nod2->next = nod1->next;
	nod1->next = temp;
}



void dezalocareLista(Nod** cap) {
	Nod* temp;
	while (*cap != NULL) {
		temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.marca);
		free(temp);
	}
}

void main() {
	Masina masina;
	Masina masina2;
	Masina masina3;
	Masina masina4;
	Masina masina5;

	masina = initMasina("Audi", 2021, 12500.5);
	masina2 = initMasina("BMW", 2019, 15500);
	masina3 = initMasina("Dacia Golan", 2018, 12000);
	masina4 = initMasina("Renault", 2018, 10000);
	masina5 = initMasina("Subaru", 2003, 9000);
	afisareMasina(masina);
	afisareMasina(masina2);

	Nod* cap = NULL;
	printf("Lista dupa inserare la inceput:\n");
	cap = inserareInceput(cap, masina5);
	cap = inserareInceput(cap, masina4);
	cap = inserareInceput(cap, masina3);
	cap = inserareInceput(cap, masina2);
	cap = inserareInceput(cap, masina);

	afisareLista(cap);

	printf("\n\nLista dupa inserare la final:\n");
	Nod* cap2 = NULL;
	inserareSfarsit(&cap2, masina4);
	inserareSfarsit(&cap2, masina3);
	afisareLista(cap2);


	printf("\n\nLista dupa stergere nod ales:\n");
	stergeNod(&cap, 1);
	afisareLista(cap);

	printf("\n\nLista dupa inserarea sortata:\n");
	inserareDupaAn(&cap, masina4);
	inserareDupaAn(&cap, masina5);
	afisareLista(cap);

	printf("\n\nLista cu noduri interschimbate:\n");
	interschimbare(cap, 1, 2);
	afisareLista(cap);

	dezalocareLista(&cap);
	dezalocareLista(&cap2);


	/*
		while (cap) {
		Nod* nod = cap->next;
		free(cap->info.marca);		
		free(cap);
		cap = nod;
	}

	while (cap2) {
		Nod* nod = cap2;
		cap2 = cap2->next;
		free(nod->info.marca);
		free(nod);
	}
	
	*/

}
