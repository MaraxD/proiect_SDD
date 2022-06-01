#define _CRT_SECURE_NO_WARNINGS
#include<malloc.h>
#include<stdio.h>
#include<string.h>

//Sa se defineasca propria structura(alta decat cele utilizate la seminar) care sa contina :
// 
//Pornind de la exemplele de la seminar sa se defineasca si sa se testeze urmatoarele functii :
//a)Stergerea si dezalocarea unui nod dupa id dintr - un graf;
//b)Stergerea si dezalocarea grafului(toate nodurile);
//c)Stergerea unui nod din arbore dupa id;
//d)Stergere nod dupa un camp de tip char* dintr - o lista dubla;
//e)Sa se citeasca din fisier cel putin 8 elemente de tip struct (pe care l - ati ales)


typedef struct Tranzit Tranzit;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

enum ORASE {
	LONDON, LIVERPOOL, TYNE_WEAR, GLASGLOW
};

struct Tranzit {
	int id; //idul nodului
	float lungime;
	int linii;
	char* denumireT;
	enum ORASE oras; //unde se afla sisteme alea 
	long voltage;
	int dim; //dim vectorului de mai jos
	int* nrTrenuri; //nr de trenuri care circula prin fiecare statie
	//ex in londra sunt n linii, iar pe fiecare linie merg m trenuri
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

Tranzit creareTransit(
	float lungime,
	int linii,
	int id,
	const char* denumireT,
	enum ORASE oras,
	long voltage,
	int dim,
	int* nrTrenuri)
{
	Tranzit t;

	t.lungime = lungime;
	t.linii = linii;
	t.id = id;
	t.denumireT = (char*)malloc(strlen(denumireT) + 1);
	strcpy(t.denumireT, denumireT);
	t.oras = oras;
	t.voltage = voltage;

	t.dim = dim;

	t.nrTrenuri = (int*)malloc(sizeof(nrTrenuri) * dim);
	for (int i = 0; i < dim; i++) {
		t.nrTrenuri[i] = nrTrenuri[i];
	}


	return t;
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

const char* getOrasNume(enum ORASE oras) {
	switch (oras)
	{
	case LONDON:
		return "London";
		break;
	case LIVERPOOL:
		return "Liverpool";
		break;
	case TYNE_WEAR:
		return "Tyne and Wear";
		break;
	case GLASGLOW:
		return "Glasglow";
		break;
	default:
		break;
	}
}


void afisareTranzit(Tranzit t) {
	printf("id nod: %d\nlungime: %.2f\nnumar de linii: %d\nnume tranzit: %s\noras: %s\nvoltage: %ld\nnr de trenuri pe fiecare linie: ",
		t.id, t.lungime, t.linii, t.denumireT, getOrasNume(t.oras), t.voltage);
	for (int i = 0; i < t.dim; i++) {
		printf("%d ", t.nrTrenuri[i]);
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
	int* nrTrenuri[5] = { 7,8,7,7,7 };

	inserareListaPrincipala(&cap, creareTransit(402, 11, 1, "London Underground", 0, 630, 5, nrTrenuri));
	inserareListaPrincipala(&cap, creareTransit(34, 7, 2, "Dockland Light Railway", 0, 750, 5, nrTrenuri));
	inserareListaPrincipala(&cap, creareTransit(121, 2, 3, "Merseyrail", 1, 750, 5, nrTrenuri));
	inserareVecini(cap, 1, 2);
	inserareVecini(cap, 1, 3);

	afisareLista(cap);
}






