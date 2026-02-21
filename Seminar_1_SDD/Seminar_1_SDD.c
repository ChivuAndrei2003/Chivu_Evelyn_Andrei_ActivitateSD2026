#include<stdio.h>
#include<malloc.h>

struct Depozit {
	int id;
	float suprafata;
	int volum;
	char* denumire ;
	char clasa ;
};
struct Depozit initializare(/*parametri necesari*/) {
	struct Depozit s;
	s.id = 1;
	//initializare structura 
		return s;
}

void afisare(struct Depozit s) {
	//afisarea tuturor atributelor.
}

void afisareVector(struct Depozit* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
}

struct Depozit* copiazaPrimeleNElemente(struct Depozit* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Depozit *vectorNou=NULL;

	return vectorNou;
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

	return 0;
}