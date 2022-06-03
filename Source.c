#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdio.h>
#include<stdlib.h> //pt atof
#include<string.h>

//Sa se defineasca propria structura(alta decat cele utilizate la seminar) care sa contina :
// 
//Pornind de la exemplele de la seminar sa se defineasca si sa se testeze urmatoarele functii :
//a)Stergerea si dezalocarea unui nod dupa id dintr - un graf; 
//b)Stergerea si dezalocarea grafului(toate nodurile);
//c)Stergerea unui nod din arbore dupa id;
//d)Stergere nod dupa un camp de tip char* dintr - o lista dubla;
//e)Sa se citeasca din fisier cel putin 8 elemente de tip struct (pe care l - ati ales) DONE


//liniile de metrou din Anglia 
typedef struct Tranzit Tranzit;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;
typedef struct Nod Nod;
typedef struct ListaD ListaD;

enum TIP {
	DT, SS
};

struct Tranzit {
	int id; //idul nodului
	enum TIP tip;
	float lungime;
	int nrStatii; //dim vectorului de mai jos
	int* anDStatii; //anii in care s a deschis fiecare statie
	char* denumire; //culoarea pe care o are pe harta
	long nrMedCalatori; //nr mediu de calatori de sapt
};

struct NodPrincipal {
	Tranzit info;
	NodPrincipal* next;
	NodPrincipal* vecini;
};

struct NodSecundar {
	NodPrincipal* info;
	NodSecundar* next;
};

struct Nod {
	Tranzit info;
	Nod* next;
	Nod* prev;
};

struct ListaD {
	Nod* ultim;
	Nod* prim;
};

Tranzit creareTransit(FILE* file)
{

	if (file != NULL) {
		Tranzit t;
		char aux[100];
		fgets(aux, 10, file);
		t.id = atoi(aux);
		fgets(aux, 10, file);
		t.tip = atoi(aux);
		fgets(aux, 10, file);
		t.lungime = atof(aux);

		fgets(aux, 10, file);
		t.nrStatii = atoi(aux);

		t.anDStatii = (int*)malloc(sizeof(t.anDStatii) * t.nrStatii);
		for (int i = 0; i < t.nrStatii; i++)
		{
			fgets(aux, 10, file);
			t.anDStatii[i] = atoi(aux);
		}

		fgets(aux, 50, file);
		char* sir = strtok(aux, "\n");
		t.denumire = (char*)malloc(strlen(sir) + 1);
		strcpy(t.denumire, sir);

		fgets(aux, 10, file);
		t.nrMedCalatori = atol(aux); //la long??

		return t;
	}

}

void inserareListaPrincipala(NodPrincipal** cap, Tranzit tr) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = tr;
	nou->next = NULL;
	nou->vecini = NULL;
	NodPrincipal* aux = *cap;
	if (*cap) {
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareListaSecundara(NodSecundar** cap, NodPrincipal* infoT) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->info = infoT;
	nou->next = NULL;
	if (*cap) {
		NodSecundar* aux = *cap;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		*cap = nou;
	}
}

const char* getStatieNume(enum TIP tip) {
	switch (tip)
	{
	case DT:
		return "Deep tube";
		break;
	case SS:
		return "Sub surface";
		break;
	default:
		break;
	}

}


void afisareTranzit(Tranzit t) {
	printf("id nod: %d\ntipul de linie: %s\nlungime: %.2f km\nnumele statiei: %s\nnumar mediu de calatori pe saptamana: %d\nnumar de statii: %d\nanii in care a fost inaugurata fiecare statie: ",
		t.id, getStatieNume(t.tip), t.lungime, t.denumire, t.nrMedCalatori, t.nrStatii);
	for (int i = 0; i < t.nrStatii; i++) {
		printf("%d ", t.anDStatii[i]);
	}
	printf("\n");
}

void afisareLista(NodPrincipal* np) {
	while (np) {
		afisareTranzit(np->info);
		NodSecundar* aux = np->vecini;
		printf("\n\nvecini:\n");
		while (aux) {
			afisareTranzit(aux->info->info);
			aux = aux->next;
		}
		printf("\n");
		np = np->next;
	}
}

NodPrincipal* cautareNod(NodPrincipal* nod, int id) {
	while (nod && nod->info.id != id) {
		nod = nod->next;
	}
	return nod;
}

void inserareVecini(NodPrincipal* nod, int id1, int id2) {
	NodPrincipal* nod1 = cautareNod(nod, id1);
	NodPrincipal* nod2 = cautareNod(nod, id2);
	if (nod1 != NULL && nod2 != NULL) {
		inserareListaSecundara(&nod1->vecini, nod2);
		inserareListaSecundara(&nod2->vecini, nod1);
	}
}


Tranzit creareDefault() {
	Tranzit t;
	t.id = NULL;
	t.tip = -1;
	t.lungime = 00.0;
	t.nrMedCalatori = -1;
	t.nrStatii = -1;
	t.denumire = NULL;
	t.anDStatii = -1;

	return t;
}

//a)Stergerea si dezalocarea unui nod dupa id dintr - un graf; 
void stergereID(NodPrincipal** nod, int id) {
	NodPrincipal* aux = (*nod);
	NodPrincipal* n = aux;
	if (aux->info.id == id) {
		aux = aux->next;

		free(n->info.denumire);
		free(n);
		n = NULL;
	}
	else {
		stergereID((*nod)->next, id);
	}
	(*nod) = aux;



}


//functiile pentru lista dubla
//inserare la inceput
void inserareListaD(ListaD* cap, Tranzit tr) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = cap->prim;
	nou->prev = NULL;
	nou->info = tr;
	if (cap->prim) {
		cap->prim->prev = nou;
		cap->prim = nou; //capul listei noi devin fix ce am adaugat
	}
	else {
		cap->prim = cap->ultim = nou;
	}
}

//fac de la stanga la dr
void afisareLD(ListaD ld) {
	while (ld.prim) {
		afisareTranzit(ld.prim->info);
		ld.prim = ld.prim->next;
	}
}

//d)Stergere nod dupa un camp de tip char* dintr - o lista dubla;
void stergereNod(ListaD* cap, char* denumire) {
	Nod* aux = cap->prim;
	Nod* aux2 = cap->ultim;
	if (cap->prim || cap->ultim) {
		if (strcmp(cap->prim->info.denumire, denumire)==0) { //daca intra inseamna ca exista si primul cap/ultimul cap
			cap->prim = aux->next; //urmatorul nod de dupa nodul sters va deveni noul cap
			free(aux->info.denumire); //stergi capul (unde se afla elem cautat)
			free(aux);
		}
		//daca se afla la sfarsit
		else if (strcmp(cap->ultim->info.denumire, denumire)==0) {
			cap->ultim = aux2->prev;
			free(aux2->info.denumire); //stergi capul (unde se afla elem cautat)
			free(aux2);
		}
		//daca se afla in interior (fac parcurgere de la stg la dr)
		else {
			aux = aux->next;
			/*while (aux->next) {*/
				if (strcmp(aux->info.denumire, denumire)==0) {
					aux->prev->next = aux->next;
					free(aux->info.denumire);
					free(aux);
				}/*
				else {
					aux = aux->next;
				}*/

			/*}*/
		}
	}


}




void main() {
	NodPrincipal* cap = NULL;
	ListaD listad;
	listad.prim = NULL;
	listad.ultim = NULL;

	FILE* f = NULL;
	f = fopen("tranzits.txt", "r");

	//arbore
	/*inserareListaPrincipala(&cap, creareTransit(f));
	inserareListaPrincipala(&cap, creareTransit(f));
	inserareListaPrincipala(&cap, creareTransit(f));
	inserareVecini(cap, 1, 2);
	inserareVecini(cap, 1, 3);
	afisareLista(cap);
	stergereID(&cap, 2);
	printf("\n\ngraf dupa sterge nod\n\n");
	afisareLista(cap);*/

	//lista dubla
	inserareListaD(&listad, creareTransit(f));
	inserareListaD(&listad, creareTransit(f));
	inserareListaD(&listad, creareTransit(f));

	afisareLD(listad);
	stergereNod(&listad, "Bakerloo line");
	printf("\n\n");
	afisareLD(listad);



}






