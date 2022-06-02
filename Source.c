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
//e)Sa se citeasca din fisier cel putin 8 elemente de tip struct (pe care l - ati ales)


//liniile de metrou din Anglia 
typedef struct Tranzit Tranzit;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

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

		t.anDStatii = (int*)malloc(sizeof(t.anDStatii)*t.nrStatii);
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

void main() {
	NodPrincipal* cap = NULL;

	FILE* f = NULL;
	f = fopen("tranzits.txt", "r");
	inserareListaPrincipala(&cap, creareTransit(f));
	inserareListaPrincipala(&cap, creareTransit(f));

	afisareLista(cap);
}






