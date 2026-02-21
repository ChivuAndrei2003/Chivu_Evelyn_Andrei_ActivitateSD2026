#include<stdio.h>
#include<malloc.h>

typedef struct Depozit Depozit;

struct Depozit {
	int id;
	float suprafata;
	int volum;
	char* denumire ;
	char clasa ;
};
struct Depozit initializare(int id,float suprafata,int volum,const char* denumire,char clasa) {
	struct Depozit s;
	s.id = id;
	s.suprafata = suprafata;
	s.volum = volum;
	s.denumire =  (char*)malloc(sizeof(char)*(strlen(denumire) +1));
	strcpy(s.denumire, denumire);
	s.clasa = clasa;	

	return s;
}
struct Depozit copiaza(Depozit d) {
	return initializare(d.id,d.suprafata,d.volum,d.denumire,d.clasa);

	
}
void afisare(struct Depozit s) {
	//afisarea tuturor atributelor.

	printf("%d. Depoztiul %s de clasa %c are o suprafata de %5.2f si un volum de %d.\n" , s.id,s.denumire,s.clasa,s.suprafata,s.volum);
}

void afisareVector(struct Depozit* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
	//deplasare + dereferentiere cu indexare cu P[2]

	for(int i=0;i<nrElemente;i++){
		afisare( *vector);
		vector++;
		//sau afisare(*(vector + i));
	}
}

struct Depozit* copiazaPrimeleNElemente(struct Depozit* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou ,pe care il vom returna, primele nrElementeCopiate
	
	
	if (nrElementeCopiate < nrElemente) {
		struct Depozit* vectorNou = malloc(sizeof(Depozit) * nrElemente);
	
		for (int i = 0; i < nrElemente; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}

		return vectorNou;
	}
	else {
		return NULL;
	}
	
}

void dezalocare(struct Depozit** vector, int* nrElemente) {
	//dezalocam elementele din vector si vectorul
}

void copiazaAnumiteElemente(struct Depozit* vector, char nrElemente, float prag, struct Depozit** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
    // tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
}

struct Depozit getPrimulElementConditionat(struct Depozit* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Depozit s;
	s.id = 1;

	return s;
}

int main() {
	struct Depozit depozit=initializare(1,34.3, 78,"Emag", 'A');
	afisare(depozit);

	int nrDepozite = 4;
	Depozit* vector =(Depozit*)malloc(sizeof(Depozit) * nrDepozite);
	vector[0] = initializare(2, 34, 45, "DPD", 'B');
	vector[1] = initializare(3, 56, 89, "Amazon", 'A');
	vector[2] = initializare(4, 465, 96, "DSC", 'A');
	//vector[3] = depozit;//shallow copy
	vector[3] = initializare(5, 34, 87, "Emag", 'A');


	afisareVector(vector, nrDepozite);
	printf("\n\n");
	int nrCopiate = 2;
	Depozit* scurt = copiazaPrimeleNElemente(vector, nrDepozite, nrCopiate);
	afisareVector(scurt, nrCopiate);
	return 0;
}