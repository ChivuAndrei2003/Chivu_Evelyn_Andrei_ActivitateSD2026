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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod
{
	Masina info;
	struct Nod* next;

};
typedef struct Nod Nod;

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable
{
	int dim;
	struct Nod** table;
};
typedef struct HashTable HashTable;


Masina citireMasinaDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer,100,file);
	char* aux;
	Masina m1;
	aux = strtok(buffer,sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL,sep));
	m1.pret = atof(strtok(NULL,sep));
	aux = strtok(NULL,sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model,aux);

	aux = strtok(NULL,sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer,aux);

	m1.serie = *strtok(NULL,sep);
	return m1;
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

void afisareListaMasini(Nod* masina)
{
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	while(masina)
	{
		afisareMasina(masina->info);
		masina = masina->next;
	}
}

void adaugaMasinaInLista(Nod* head,Masina masinaNoua)
{
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* cap = head;
	while(cap->next)
	{
		cap = cap->next;
	}

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	cap->next = nou;
}


HashTable initializareHashTable(int dimensiune)
{

	HashTable hash_table;
	hash_table.dim = dimensiune;
	hash_table.table = (Nod**)calloc(dimensiune,sizeof(Nod*));

	return hash_table;
}

int calculeazaHash(const char* nume,int dimensiune)
{
	//este calculat hash-ul in functie de dim tabelei si un atribut al masinii
	int sum = 0;
	for(int i = 0; i<strlen(nume);i++)
	{
		sum += nume[i];
	}
	return sum % dimensiune;
}

void inserareMasinaInTabela(HashTable hash,Masina galerie)
{
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int poz = calculeazaHash(galerie.model,hash.dim);

	if(hash.table[poz] == NULL)
	{
		hash.table[poz] = (Nod*)malloc(sizeof(Nod));
		hash.table[poz]->info = galerie;
		hash.table[poz]->next = NULL;
	}
	else
	{
		adaugaMasinaInLista(hash.table[poz],galerie);

	}
}

HashTable citireMasiniDinFisier(const char* numeFisier,int dimensiune)
{
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul

	HashTable hash = initializareHashTable(dimensiune);
	FILE* f = fopen(numeFisier,"r");

	//char buffer[100];
	while(!feof(f))
	{
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash,masina);
	}
	fclose(f);

	return hash;

}

void afisareTabelaDeMasini(HashTable ht)
{
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
	for(int i = 0;i < ht.dim; i++)
	{
		if(ht.table[i] != NULL)
		{
			printf("\nPe pozitia %d avem urmatoarele masin: \n",i);
			afisareListaMasini(ht.table[i]);
		}
		else
		{
			printf("\n Nu avem masini pe pozitia %d\n",i);
		}

	}
}

void dezalocareListaMasini(Nod** cap)
{

	Nod* head = *cap;
	while(head)
	{
		Nod* temp = head;
		head = head->next;
		if(temp->info.model != NULL)
		{
			free(temp->info.model);
		}
		if(temp->info.numeSofer != NULL)
		{
			free(temp->info.numeSofer);
		}
		free(temp);
	}
	*cap = NULL;
}

void dezalocareTabelaDeMasini(HashTable* ht)
{
	//sunt dezalocate toate masinile din tabela de dispersie

	for(int i = 0;i < ht->dim;i++)
	{
		dezalocareListaMasini(&(ht->table[i]));
	}
	free(ht->table);
	ht->table = NULL;
	ht->dim = 0;


}

float calculeazaMediePeLista(Nod* cap)
{
	float sum = 0;
	int numarElemente = 0;
	while(cap)
	{
		sum += cap->info.pret;

		cap = cap->next;

		numarElemente++;
	}

	float medie = numarElemente > 0 ? (sum / numarElemente) : 0;

	return medie;

}

float* calculeazaPreturiMediiPerClustere(HashTable ht,int* nrClustere)
{
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	*nrClustere = 0;
	float* preturi = malloc(sizeof(float) * ht.dim);

	for(int i = 0;i < ht.dim;i++)
	{
		if(ht.table[i] != NULL)
		{
			preturi[*nrClustere] = calculeazaMediePeLista(ht.table[i]);
			(*nrClustere)++;//nu *nrClustere++ pt ca doar muta pointerul 
		}
	}

	return preturi;
}

Masina getMasinaDinLista(Nod* cap,const char* nume_model)
{
	Masina m;
	m.id = -1;

	while(cap && strcmp(cap->info.model,nume_model) != 0)
	{
		cap = cap->next;
	}
	if(cap)
	{
		m = cap->info;
		m.model = (char*)malloc(strlen(cap->info.model) + 1);
		strcpy(m.model,cap->info.model);
		m.numeSofer = (char*)malloc(strlen(cap->info.numeSofer) + 1);
		strcpy(m.numeSofer,cap->info.numeSofer);
	}
	return m;
}

Masina getMasinaDupaNumeModel(HashTable ht,const char* nume_model_masina)
{
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 

	Masina m;
	m.id = -1;
	int poz = calculeazaHash(nume_model_masina,ht.dim);

	if(poz >= 0 && poz < ht.dim)
	{
		return getMasinaDinLista(ht.table[poz],nume_model_masina);

	}

	return m;
}

int main( )
{
	HashTable hash_table = citireMasiniDinFisier("masini.txt",7);
	afisareTabelaDeMasini(hash_table);

	int nrClustere = 3;
	float* preturi = calculeazaPreturiMediiPerClustere(hash_table,&nrClustere);

	printf("Preturile medii per clustere :\n");

	for(int i = 0;i < nrClustere;i++)
	{
		printf("Cluster %d : %.2f\n",i,preturi[i]);
	}
	printf("\n");

	Masina m = getMasinaDupaNumeModel(hash_table,"Passat");

	if(m.id != -1)
	{
		printf("Masina cautata dupa numele modelului :\n");
		afisareMasina(m);

		if(m.model != NULL)
		{
			free(m.model);
		}
		if(m.numeSofer != NULL)
		{
			free(m.numeSofer);
		}
	}
	else
	{
		printf("Modelul de masina cautat nu a fost gasit");
	}

	free(preturi);
	dezalocareTabelaDeMasini(&hash_table);


	return 0;
}