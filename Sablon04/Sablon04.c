#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraAvion
{
	int id;
	int nrLocuri;
	float pret;
	char* model;
	char* numePilot;
	unsigned char serie;
};
typedef struct StructuraAvion Avion;

typedef struct Nod Nod;

struct Nod
{
	Avion info;
	Nod* urm;
};

//creare structura pentru un nod dintr-o lista simplu inlantuita
Avion citireAvionDinFisier (FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets (buffer,100,file);
	char* aux;
	Avion m1;
	aux = strtok (buffer,sep);
	m1.id = atoi (aux);
	m1.nrLocuri = atoi (strtok (NULL,sep));
	m1.pret = atof (strtok (NULL,sep));
	aux = strtok (NULL,sep);
	m1.model = malloc (strlen (aux) + 1);
	strcpy(m1.model,aux);

	aux = strtok (NULL,sep);
	m1.numePilot = malloc (strlen (aux) + 1);
	strcpy (m1.numePilot,aux);

	m1.serie = *strtok (NULL,sep);
	return m1;
}

void afisarAvion(Avion avion)
{
	printf ("Id: %d\n",avion.id);
	printf ("Nr. locuri : %d\n",avion.nrLocuri);
	printf ("Pret: %.2f\n",avion.pret);
	printf ("Model: %s\n",avion.model);
	printf ("Nume spilot : %s\n",avion.numePilot);
	printf ("Serie: %c\n\n",avion.serie);
}

void afisareListaAvioane (Nod* cap)
{
	//afiseaza toate elemente de tip Avion din lista simplu inlantuita
	//prin apelarea functiei afisar Avion()

	while(cap)
	{
		afisarAvion(cap->info);
		cap = cap->urm;
	}
}

void adaugAvionInLista (Nod** cap,Avion avionNou)
{
	//adauga la final in lista primita un nou Avion pe care-l primim ca parametru

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = avionNou;
	nou->urm = NULL;

	if((*cap) == NULL)
	{
		*cap = nou;
	}
	else
	{
		Nod* p = *cap;
		while(p->urm)
		{
			p = p->urm;
		}
		p->urm = nou;
	}
}

void adaugaLaInceputInLista (Nod** cap,Avion avionNou)
{
	//adauga la inceputul listei o noua Avion pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = avionNou;
	nou->urm = *cap;
	*cap = nou;

}

void* citireListaAvionDinFisier (const char* numeFisier)
{
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citir AvionDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier,"r");
	Nod* lista = NULL;

	while(!feof(f))
	{
		adaugaLaInceputInLista(&lista,citireAvionDinFisier(f));

	}
	fclose(f);
	return lista;
}

void dezalocareListaAvioane (Nod** cap)
{
	//sunt dezalocate toate masinile si lista de elemente

	while((*cap) != NULL)
	{
		Nod* aux = (*cap);
		*cap = (*cap)->urm;
		free(aux->info.numePilot);
		free(aux->info.model);
		free(aux);
	}
}

float calculeazaPretMediu (Nod* lista)
{
	//calculeaza pretul mediu al masinilor din lista.
	int count = 0;
	float sum = 0;
	while(lista)
	{
		sum += lista->info.pret;
		count++;
		lista = lista->urm;
	}

	return (count > 0) ? sum / count : 0;
}

void stergeAvionDinSeria (Nod** cap,char serieCautata)
{
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca Avion se afla si pe prima pozitie, si pe ultima pozitie

	Nod* p = *cap;
	Nod* anterior = NULL;
	while(p != NULL)
	{
		if(p->info.serie == serieCautata)
		{
			Nod* deSters = p;

			if(anterior == NULL)
			{
				*cap = p->urm;
			}
			else
			{
				anterior->urm = p->urm;
			}
			p = p->urm;

			free(deSters->info.model);
			free(deSters->info.numePilot);
			free(deSters);
		}

		else
		{
			anterior = p;
			p = p->urm;
		}
	}
}

float calculeazaPretulAvionuluiUnuiPilot (Nod* lista,const char* numePilot)
{
	//calculeaza pretul tuturor masinilor unui sofer.
	float sum = 0;
	while(lista)
	{
		if(strcmp(numePilot,lista->info.numePilot) == 0)
		{
			sum += lista->info.pret;
		}
		lista = lista->urm;
	}
	return sum;
}

int main ()
{
	Nod* cap = NULL;
	cap = citireAvionDinFisier("avion.txt");

	afisareListaAvioane(cap);

	printf("%f \n\n",calculeazaPretMediu(cap));
	printf("%f \n\n",calculeazaPretulAvionuluiUnuiPilot(cap,"Ionescu"));

	dezalocareListaAvioane(&cap);
	return 0;
}