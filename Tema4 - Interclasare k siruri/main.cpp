/*
 * Simina Dan-Marius
 * Grupa 2
 *
 * La aceasta lucrare am implementat operatiile de creare nod, insert first, delete first, structura de heap,
 * functia de generare a k liste de dimensiuni diferite si functia de interclasare a k liste.
 *
 * Am implementat min heap-ul, iar in el stochez primul(cel mai mic) element din fiecare lista, astfel in radacina heap-ului se afla mereu
 * valoarea minima din dintre toate valorile din cele k liste.
 *
 * In functia de generare liste pun pentru fiecare lista n / k elemente iar in ultima lista pun ce a ramas.
 *
 * Pentru interclasarea a k liste ma folosesc de min heap si adaug in lista care rezulta elementul din radacina heap-ului care e mereu elementul
 * minim, iar dupa in radacina pun elementul urmator din lista daca exista si refac heap-ul, iar daca nu mai exista elemente in lista
 * aduc in varf elementul de pe ultima pozitie, scad dimensiunea heap-ului si il refac (heapify O(log n)).
 * Repet algoritmul pana cand dimensiunea heap-ului ajunge la 0.
 * Complexitate O(n * log k)
 *
 * Pentru cazul in care am k1 = 5, k2 = 10 si k3 = 100, iar n-ul varianza intre 100 si 10000 cu cat sunt mai putine liste cu atat se
 * fac mai putine operatii. Pentru k1 se fac cele mai putine operatii apoi pentru k2, iar cele mai multe se fac pt k3, dar diferentele
 * nu sunt foarte mari, mai ales intre k1 si k2
 *
 * Pentru cazul in care n e constant 10000, iar k variaza intre 10 si 500 se fac mai putine operatii cu cat k e mai mic, dar intre
 * valori mari diferenta incepe sa nu mai fie atat de mare.
 *
 */
#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler p("Interclasare_k_liste");

#define maxSize 10000
#define stepSize 100
#define nrTests 5

struct node {
    int key;
    node *next;
};

struct heap {
    int length;
    int heapSize;
    node *A[maxSize];
};

//Liste

node *create(int key) {
    node *p = new node;
    p->key = key;
    p->next = NULL;

    return p;
}

void insertFirst(node *&first, int key) {
    node *p = create(key);
    if (first == NULL) {
        first = p;
    } else {
        p->next = first;
        first = p;
    }
}

void deleteFirst(node *&first) {
    if (first != NULL) {
        node *p = first;
        first = first->next;
        delete p;
    }
}

void afisareLista(node *first) {
    cout << "Lista: ";
    for (node *p = first; p != NULL; p = p->next) {
        cout << p->key << " ";
    }
    cout << "\n";
}

void afisareListe(node *liste[], int k) {
    for (int i = 0; i < k; i++) {
        cout << "K" << i + 1 << ": ";
        afisareLista(liste[i]);
    }
}

void stergeLista(node *&first) {
    while (first != NULL)
        deleteFirst(first);
}

node *creareLista(int v[], int n) {
    node *first = NULL;
    for (int i = 0; i < n; i++) {
        insertFirst(first, v[i]);
    }
    return first;
}

void generareListe(node *liste[], int k, int n) {
    int lungimeLista = 0, sumaLungimi = 0;
    int v[maxSize] = {0};
    for (int i = 0; i < k; i++) {
        if (i == k - 1) {
            lungimeLista = n - sumaLungimi;
        } else {
            lungimeLista = n / k;
            sumaLungimi += lungimeLista;
        }
        FillRandomArray(v, lungimeLista, 10, 50000, FALSE, DESCENDING);
        liste[i] = creareLista(v, lungimeLista);
    }
}

//Heap

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void heapify(heap &H, int i, const int n, const string sK) {

    string aux = "Operatii " + sK;
    const char *s = aux.c_str();
    Operation nrOperatii = p.createOperation(s, n);

    int l = left(i);
    int r = right(i);
    int largest;

    if (l < H.heapSize) {
        nrOperatii.count();
        if (H.A[l]->key < H.A[i]->key) {
            largest = l;
        } else largest = i;
    } else largest = i;

    if (r < H.heapSize) {
        nrOperatii.count();
        if (H.A[r]->key < H.A[largest]->key) {
            largest = r;
        }
    }

    if (largest != i) {
        nrOperatii.count(3);
        swap(H.A[i], H.A[largest]);
        heapify(H, largest, n, sK);
    }
}

void buildHeap(heap &H, const int n, const string sK) {

    string aux = "Operatii " + sK;
    const char *s = aux.c_str();
    Operation nrOperatii = p.createOperation(s, n);

    nrOperatii.count();
    H.heapSize = H.length;

    for (int i = H.heapSize / 2 - 1; i >= 0; i--) {
        heapify(H, i, n, sK);
    }
}

void afisareHeap(heap H) {
    cout << "Heap: ";
    for (int i = 0; i < H.length; i++) {
        cout << H.A[i]->key << " ";
    }
    cout << "\n";
}

node *mergeSortedLists(node *liste[], int k, const int n, const string sK) {

    string aux = "Operatii " + sK;
    const char *s = aux.c_str();
    Operation nrOperatii = p.createOperation(s, n);

    node *first = NULL, *last = NULL;
    heap H = {0};

    nrOperatii.count(2);
    H.length = k;
    H.heapSize = k;

    for (int i = 0; i < k; i++) {
        nrOperatii.count();
        H.A[i] = liste[i];
    }

    buildHeap(H, n, sK);

    nrOperatii.count();
    while (H.heapSize > 0) {
        nrOperatii.count();

        nrOperatii.count();
        if (last != NULL) {
            nrOperatii.count();
            last->next = H.A[0];
        } else {
            nrOperatii.count();
            first = H.A[0];
        }

        nrOperatii.count(2);
        last = H.A[0];
        H.A[0] = H.A[0]->next;

        nrOperatii.count();
        if (H.A[0] == NULL) {
            nrOperatii.count(2);
            H.A[0] = H.A[H.heapSize - 1];
            H.heapSize--;
        }

        nrOperatii.count();
        if (H.heapSize > 1) {
            heapify(H, 0, n, sK);
        }
    }

    nrOperatii.count();
    last->next = NULL;

    return first;
}

void demo() {
    int k = 4;
    int n = 20;
    node *liste[maxSize];

    for (int i = 0; i < maxSize; i++)
        liste[i] = NULL;

    generareListe(liste, k, n);
    afisareListe(liste, k);

    node *first = mergeSortedLists(liste, k, 0, "");
    afisareLista(first);

    stergeLista(first);
    afisareLista(first);
}

void perf() {
    int n = 0, ki[3] = {5, 10, 100};
    node *liste[maxSize];
    node *first = NULL;

    for (int i = 0; i < maxSize; i++)
        liste[i] = NULL;

    for (int i = 0; i < 3; i++) {
        string sK = "k" + to_string(i + 1);
        for (n = stepSize; n <= maxSize; n += stepSize) {
            for (int teste = 0; teste <= nrTests; teste++) {
                generareListe(liste, ki[i], n);
                first = mergeSortedLists(liste, ki[i], n, sK);
                stergeLista(first);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        const char *sK = ("Operatii k" + to_string(i + 1)).c_str();
        p.divideValues(sK, nrTests);
    }

    p.createGroup("Operatii totale pentru k1 egal 5, k2 egal 10, k3 egal 100", "Operatii k1", "Operatii k2",
                  "Operatii k3");

    n = maxSize;
    string sK = "n egal 10000";
    for (int k = 10; k <= 500; k += 10) {
        for (int teste = 0; teste <= nrTests; teste++) {
            generareListe(liste, k, n);
            first = mergeSortedLists(liste, k, k, sK);
            stergeLista(first);
        }
    }

    p.divideValues("Operatii n egal 10000", nrTests);

    p.showReport();
}

int main() {
    demo();
//    perf();
    return 0;
}
