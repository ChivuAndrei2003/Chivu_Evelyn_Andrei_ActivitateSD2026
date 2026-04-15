#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina
{
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

struct Nod
{
	Nod* stanga;
	Nod* dreapta;
	Masina info;
};


//creare structura pentru un nod dintr-un arbore binar de cautare

int citireMasinaDinFisier(FILE* file, Masina* m1)
{
	char buffer[100];
	char sep[3] = ",\n";
	if(fgets(buffer,100,file) == NULL)
	{
		return 0;
	}
	char* aux;
	aux = strtok(buffer,sep);
	if(aux == NULL) { return 0; }
	m1->id = atoi(aux);

	aux = strtok(NULL,sep);
	if(aux == NULL) { return 0; }
	m1->nrUsi = atoi(aux);

	aux = strtok(NULL,sep);
	if(aux == NULL) { return 0; }
	m1->pret = atof(aux);

	aux = strtok(NULL,sep);
	if(aux == NULL) { return 0; }
	m1->model = malloc(strlen(aux) + 1);
	strcpy(m1->model,aux);

	aux = strtok(NULL,sep);
	if(aux == NULL)
	{
		free(m1->model);
		return 0;
	}
	m1->numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1->numeSofer,aux);

	aux = strtok(NULL,sep);
	if(aux == NULL)
	{
		free(m1->model);
		free(m1->numeSofer);
		return 0;
	}
	m1->serie = *aux;
	return 1;
}

void afisareMasina(Masina masina)
{
	printf("Id: %d\n",masina.id);
	printf("Nr. usi : %d\n",masina.nrUsi);
	printf("Pret: %.2f\n",masina.pret);
	printf("Model: %s\n",masina.model);
	printf("Nume sofer: %s\n",masina.numeSofer);
	printf("Serie: %c\n\n",masina.serie);
}


void adaugaMasinaInArbore(Nod** root,Masina masinaNoua)
{
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if((*root) != NULL)
	{
		if(masinaNoua.id < (*root)->info.id)
		{
			//echivalent dupa model. prof: adaugaMasinaInArbore(&((*root)->stanga),masinaNoua);
			adaugaMasinaInArbore(&(*root)->stanga,masinaNoua);
		}
		else if(masinaNoua.id > (*root)->info.id)
		{
			adaugaMasinaInArbore(&(*root)->dreapta,masinaNoua);
		}

		else
		{
			if((*root)->info.model != NULL) { free((*root)->info.model); }
			if((*root)->info.numeSofer != NULL) { free((*root)->info.numeSofer); }

			//IMPORTANT:
			//In C cast-ul e implicit => nu-i nevoie de 
			  // (*root) =(char*) malloc(sizeof(Nod)); ca in C++

			(*root)->info = masinaNoua;
			(*root)->info.pret = masinaNoua.pret;
			(*root)->info.serie = masinaNoua.serie;

			(*root)->info.model = malloc(strlen(masinaNoua.model) + 1);
			strcpy((*root)->info.model,masinaNoua.model);

			(*root)->info.numeSofer = malloc(strlen(masinaNoua.numeSofer) + 1);
			strcpy((*root)->info.numeSofer,masinaNoua.numeSofer);
		}
	}

	else
	{
		(*root) = malloc(sizeof(Nod));
		(*root)->info = masinaNoua;
		(*root)->info.id = masinaNoua.id;
		(*root)->info.pret = masinaNoua.pret;
		(*root)->info.serie = masinaNoua.serie;

		(*root)->info.model = malloc(strlen(masinaNoua.model) + 1);
		strcpy((*root)->info.model,masinaNoua.model);

		(*root)->info.numeSofer = malloc(strlen(masinaNoua.numeSofer) + 1);
		strcpy((*root)->info.numeSofer,masinaNoua.numeSofer);

		(*root)->stanga = NULL;
		(*root)->dreapta = NULL;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier)
{
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier,"r");
	
	if(f == NULL)
	{
		printf("Fisierul %s nu a putut fi deschis.\n",numeFisier);
		return NULL;
	}

	Nod* root = NULL;
	Masina m;
	while(citireMasinaDinFisier(f,&m))
	{
		adaugaMasinaInArbore(&root,m);
		free(m.model);
		free(m.numeSofer);
	}
	fclose(f);

	return root;
}

void afisareArboreInOrdine(Nod* root)
{
	if(root != NULL)
	{
		afisareArboreInOrdine(root->stanga);
		afisareMasina(root->info);
		afisareArboreInOrdine(root->dreapta);
	}
};

void afisareArborePreOrdine(Nod* root)
{
	if(root != NULL)
	{
		afisareMasina(root->info);
		afisareArborePreOrdine(root->stanga);
		afisareArborePreOrdine(root->dreapta);
	}
};

void afisareArborePostOrdine(Nod* root)
{
	if(root != NULL)
	{
		
		afisareArborePostOrdine(root->stanga);
		afisareArborePostOrdine(root->dreapta);
		afisareMasina(root->info);

	}
};

void afisareMasiniDinArbore(Nod* root)
{
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	printf("\nParcurgere si afisare arbore in ordine :\n ");
	afisareArboreInOrdine(root);

	printf("\nParcurgere si afisare arbore in preordine :\n ");
	afisareArborePreOrdine(root);

	printf("\nParcurgere si afisare arbore in postordine :\n ");
	afisareArborePostOrdine(root);

}

void dezalocareArboreDeMasini(Nod** root)
{
	//sunt dezalocate toate masinile si arborele de elemente
	if((*root) != NULL)
	{
		dezalocareArboreDeMasini(&(*root)->stanga);
		dezalocareArboreDeMasini(&(*root)->dreapta);
		free((*root)->info.model);
		free((*root)->info.numeSofer);
		free(*root);
		*root = NULL;
	}
}

Masina getMasinaByID(Nod* root,int id)
{
	Masina m;
	m.id = -1;
	if(root == NULL) return m;

	if(id > root->info.id)
	{
		return getMasinaByID(root->dreapta,id);
	}
	else if(id < root->info.id)
	{
		return getMasinaByID(root->stanga,id);
	}
	else
	{
		m = root->info;
		m.numeSofer = malloc(strlen(root->info.numeSofer) + 1);
		strcpy(m.numeSofer,root->info.numeSofer);

		m.model = malloc(strlen(root->info.model) + 1);
		strcpy(m.model,root->info.model);

		return m;
	}


	return m;
}

int determinaNumarNoduri(Nod* root)
{
	//calculeaza numarul total de noduri din arborele binar de cautare
	if(root == NULL) return 0;

	int s = determinaNumarNoduri(root->stanga);
	int d = determinaNumarNoduri(root->dreapta);

	return 1 + s + d;
}

int calculeazaInaltimeArbore(Nod* root)
{
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if(root == NULL) return -1;

	int inaltimeStanga = calculeazaInaltimeArbore(root->stanga);
	int inaltimeDreapta = calculeazaInaltimeArbore(root->dreapta);

	return (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
}

float calculeazaPretTotal(Nod* root)
{
	//calculeaza pretul tuturor masinilor din arbore.

	if(root == NULL) return 0;

	return root->info.pret
		+ calculeazaPretTotal(root->stanga)
		+ calculeazaPretTotal(root->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* root,const char* numeSofer)
{
	//calculeaza pretul tuturor  masinilor unui sofer.
	if(root == NULL)return 0;

	float pret = 0;
	if(strcmp(root->info.numeSofer,numeSofer) == 0)
	{
		pret += root->info.pret;
	}
	pret += calculeazaPretulMasinilorUnuiSofer(root->stanga,numeSofer);
	pret += calculeazaPretulMasinilorUnuiSofer(root->dreapta,numeSofer);

	return pret;
}

int main( )
{
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	afisareMasiniDinArbore(arbore);

	Masina m = getMasinaByID(arbore,3);
	printf("\n\n***************");
	afisareMasina(m);

	int i = determinaNumarNoduri(arbore);
	printf("\n Numar noduri arbore: %d",i);

	int inaltimeArbore = calculeazaInaltimeArbore(arbore);
	printf("\n Inaltime arbore: %d",inaltimeArbore);

	float pretTotal = calculeazaPretTotal(arbore);
	printf("\n Pretul tuturor masinilor din arbore: %.2F",pretTotal);

	float pretMasini_Gheorghe = calculeazaPretulMasinilorUnuiSofer(arbore,"Gheorghe");
	printf("\n Pretul tuturor masinilor lui Gheorghe: %.2F",pretMasini_Gheorghe);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}
