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
typedef struct NodA NodA;  //pentru arbori
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

struct NodA {
	Tranzit info;
	NodA* dreapta;
	NodA* stanga;
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
		t.nrMedCalatori = atol(aux);

		return t;
	}

}

//-------------functiile pentru grafuri------------------------
void inserareListaPrincipala(NodPrincipal** cap, Tranzit tr) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->next = NULL;
	nou->info = tr;
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
	nou->next = NULL;
	nou->info = infoT;
	NodSecundar* aux = *cap;
	if (*cap) {
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
	printf("id nod: %d\ntipul de linie: %s\nlungime: %.2f km\nnumele statiei: %s\nnumar mediu de calatori pe saptamana: %d\nnumar de statii: %d\nanii in care a fost inaugurata fiecare statie: \n",
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
		printf("\nvecini lui %d: \n", np->info.id);
		while (aux) {
			afisareTranzit(aux->info->info);
			aux = aux->next;
		}
		printf("\n\n");
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

//a)Stergerea si dezalocarea unui nod dupa id dintr - un graf; 
void stergereID(NodPrincipal** nod, int id) {
	if ((*nod)->info.id == id) {
		NodPrincipal* aux = (*nod);
		(*nod)->vecini = NULL;
		(*nod) = (*nod)->next;
		free(aux->info.denumire);
		free(aux->info.anDStatii);
		free(aux);
	}
	else {
		stergereID(&(*nod)->next, id);
	}
}

//b)Stergerea si dezalocarea grafului(toate nodurile);
dezalocareLS(NodSecundar** ns) {
	while (*ns) {
		free((*ns)->info->info.denumire);
		free((*ns)->info->info.anDStatii);
		NodSecundar* aux = *ns;
		(*ns) = (*ns)->next;
		free(aux);
	}
}

void dezalocareLP(NodPrincipal** np) {
	while (*np) {
		//daca nodul principal are vecini, apeleaza functia de mai sus
		if ((*np)->vecini) {
			dezalocareLS(&(*np)->vecini);
		}
		//acm stergem si nodul din lista principala
		free((*np)->info.denumire);
		free((*np)->info.anDStatii);
		NodSecundar* aux = *np;
		(*np) = (*np)->next;
		free(aux);
	}
}



//----------------------functiile pentru arbore binar------------
void inserareArbore(NodA** rad, Tranzit tr) {
	if ((*rad) == NULL) {
		NodA* nou = (NodA*)malloc(sizeof(NodA));
		nou->info = tr;
		nou->dreapta = nou->stanga = NULL;
		(*rad) = nou;
	}
	else {
		if (tr.id > (*rad)->info.id) {
			inserareArbore(&(*rad)->dreapta, tr);
		}
		else {
			inserareArbore(&(*rad)->stanga, tr);
		}
	}
}

void parcurgereInOrdine(NodA* rad) {
	if (rad) {
		parcurgereInOrdine(rad->stanga);
		afisareTranzit(rad->info);
		parcurgereInOrdine(rad->dreapta);
	}

}


//c)Stergerea unui nod din arbore dupa id;
//daca nodul cautat este chiar frunza, acesta se sterge normal
//daca nodul are un copil, acesta este inlocuit de copil
//daca nodul are doi copii, acesta este inlocuit de copilul cu valoarea cea mai mare (compari id urile)
//nr de copii este dat de diferenta de inaltime
//functia in caz ca nodul pe care vrem sa l stergem are doi copii/subarbori
void stergere1(NodA** ndr, NodA** nodS) {
	if ((*ndr)->stanga) {
		stergere1(&(*ndr)->stanga, &(*nodS));
	}
	else {
		(*nodS)->info = (*ndr)->info;
		NodA* aux = (*ndr);
		(*ndr) = (*ndr)->stanga;
		free(aux);
	}
}

void stergere2(NodA** nod, int id) {
	if (*nod) {
		if ((*nod)->info.id == id) {
			//nodul trebuie sters
			if ((*nod)->stanga == NULL && (*nod)->dreapta) {
				free(*nod);
				(*nod) = NULL;
			}
			else {
				if ((*nod)->dreapta == NULL) {
					NodA* aux = *nod;
					(*nod) = (*nod)->stanga;
					free(aux);
				}
				else {
					if ((*nod)->stanga == NULL) {
						NodA* aux = *nod;
						(*nod) = (*nod)->dreapta;
						free(aux);
					}
					else {
						stergere1((*nod)->stanga, (*nod));
					}
				}
			}
		}
		else {
			if ((*nod)->info.id > id)
				stergere2(&(*nod)->stanga, id);
			else
				stergere2(&(*nod)->dreapta, id);
		}
	}
	else
	{
		return "valoarea nu se afla in arbore";
	}
}



//----------------------functiile pentru lista dubla-----------------------
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
		if (strcmp(cap->prim->info.denumire, denumire) == 0) { //daca intra inseamna ca exista si primul cap/ultimul cap
			cap->prim = aux->next; //urmatorul nod de dupa nodul sters va deveni noul cap
			cap->prim->prev = NULL;
			free(aux->info.denumire); //stergi capul (unde se afla elem cautat)
			free(aux);
		}
		//daca se afla la sfarsit
		else if (strcmp(cap->ultim->info.denumire, denumire) == 0) {
			cap->ultim = aux2->prev;
			cap->ultim->next = NULL;
			free(aux2->info.denumire); //stergi capul (unde se afla elem cautat)
			free(aux2);

		}
		//daca se afla in interior (fac parcurgere de la stg la dr)
		else {
			aux = aux->next;
			while (aux->next) {
				if (strcmp(aux->info.denumire, denumire) == 0) {
					aux->prev->next = aux->next;
					aux->next->prev = aux->prev;
					aux->next = NULL;
					//aux->prev->next = aux->next;
					free(aux->info.denumire);
					free(aux->info.anDStatii);
					free(aux);
					break;
				}
				else {
					aux = aux->next;
				}

			}
		}
	}


}




void main() {
	NodPrincipal* cap = NULL;
	ListaD listad;
	listad.prim = NULL;
	listad.ultim = NULL;
	NodA* radacina = NULL;

	


	//graf
	FILE* f2 = NULL;
	f2 = fopen("tranzits2.txt", "r");
	for (int i = 0; i < 10; i++) {
		inserareListaPrincipala(&cap, creareTransit(f2));
	}
	inserareVecini(cap, 2, 7);
	inserareVecini(cap, 3, 6);
	inserareVecini(cap, 3, 5);
	inserareVecini(cap, 4, 10);
	inserareVecini(cap, 4, 11);
	afisareLista(cap);

	//a)
	/*stergereID(&cap, 2);
	printf("\n\ngraf dupa sterge nod\n\n");
	afisareLista(cap);*/


	////b)
	printf("\n\ngraf dupa dezalocare\n\n");
	dezalocareLP(&cap);

	afisareLista(cap);

	//arbore binar
	FILE* f = NULL;
	f = fopen("tranzits.txt", "r");
	/*for (int i = 0; i < 10; i++) {
		inserareArbore(&radacina, creareTransit(f));
	}

	parcurgereInOrdine(radacina);
	stergere2(&radacina, 1);
	printf("\n\narborele dupa stergere\n\n");
	parcurgereInOrdine(radacina);*/

	//lista dubla
	/*inserareListaD(&listad, creareTransit(f));
	inserareListaD(&listad, creareTransit(f));
	inserareListaD(&listad, creareTransit(f));

	afisareLD(listad);
	stergereNod(&listad, "Central line");
	printf("\n\n");
	afisareLD(listad);*/



}






