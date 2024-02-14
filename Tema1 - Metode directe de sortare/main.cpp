/*
 *  Simina Dan-Marius
 *  Grupa 2
 *
 *  La bubble sort plec de la premiza ca sirul e sortat si verific acest lucru parcurgand sirul.
 *  Daca in timp ce parcurg vectorul dau de doua elemente care nu sunt in ordinea corecta semnalez ca am gasit elemente care nu sunt in ordinea corecta (rezulta ca sirul nu e sortat) si le interschimb.
 *  Repet algoritmul cat timp sirul nu e sortat.
 *
 *  La insertion sort parcurg vectorul si inserez elementul curent pe pozitia corecta din partea stanga a sirului(fata de pozitia pe care ma aflu).
 *  Pentru a face acest lucru cat timp elementele din vector sunt mai mare decat elementul pe care vreau sa il inserez fac shift-are la dreapta.
 *
 *  La selection sort parcurg vectorul si caut in partea dreapta elementul minim.
 *  Daca gasesc un minim care nu este elementul de pe pozitia curenta atunci interschimb minimul cu elementul de pe pozitia curenta.
 *
 *  Toti cei trei algoritmi au complexitate O(n^2)
 *
 *  In cazul favorabil:
 *  Atribuiri: bubble sort cat si selection sort reusesc sa nu faca atribuiri.
 *  Comparatii: bubble sort si insertion sort fac aprope acelasi numar de comparatii, iar selection sort face un numar mai mare de comparatii decat ele.
 *  Total: bubble sort face cele mai putine operatii in cazul favorabil, apoi urmeaza insertion sort, iar selection sort face cele mai multe operatii.
 *
 *  In cazul mediu:
 *  Atribuiri: selection sort face cele mai putine atribuiri.
 *  Coparatii: insertion sort face cele mai putine, iar bubble sort si selection sort fac aproape acelasi numar de comparatii.
 *  Total: selection sort si insertion sort fac aproape acelasi numar de operatii, iar bubble sort face cele mai multe operatii.
 *
 *  In cazul defavorabil:
 *  Atribuiri: selection sort face cele mai putine atribuiri.
 *  Comparatii: toate fac aproape acelasi numar de comparatii.
 *  Total: selection sort face cele mai putine operatii, apoi urmeaza insertion sort, iar bubble sort face cele mai multe operatii.
 */

#include <iostream>
#include <time.h>
#include "Profiler.h"

using namespace std;

Profiler p("sortari");

#define maxSize 10000
#define stepSize 100
#define nrTests 5



void bubbleSort(int v[], int n) {
    Operation opCompBubbleSort = p.createOperation("Comparatii bubble sort", n);
    Operation opAttrBubbleSort = p.createOperation("Atribuiri bubble sort", n);

    bool swapped;

    do {
        swapped = false;
        for (int i = 1; i < n; i++) {
            opCompBubbleSort.count();
            if (v[i - 1] > v[i]) {
                opAttrBubbleSort.count(3);
                int aux = v[i - 1];
                v[i - 1] = v[i];
                v[i] = aux;

                swapped = true;
            }
        }

        n--;
    } while (swapped);
}

void insertionSort(int v[], int n) {
    Operation opCompInsertionSort = p.createOperation("Comparatii insertion sort", n);
    Operation opAttrInsertionSort = p.createOperation("Atribuiri insertion sort", n);

    for (int j = 1; j < n; j++) {
        opAttrInsertionSort.count();
        int key = v[j];

        int i = j - 1;
        while (i >= 0) {
            opCompInsertionSort.count();
            if (v[i] > key) {
                opAttrInsertionSort.count();
                v[i + 1] = v[i];
                i--;
            } else break;
        }

        opAttrInsertionSort.count();
        v[i + 1] = key;
    }
}

void insertionSortCuCB(int v[], int n) {
    Operation opCompInsertionSort = p.createOperation("Comparatii insertion sort", n);
    Operation opAttrInsertionSort = p.createOperation("Atribuiri insertion sort", n);

    for (int j = 1; j < n; j++) {
        opAttrInsertionSort.count();
        int key = v[j];
        int st = 0, dr = j - 1, poz = -1;

        while(st <= dr)
        {
            int m = (st + dr) / 2;

            opCompInsertionSort.count(3);
            if(v[m] <= key and (key < v[m + 1] or m == j - 1)){
                poz = m;
                break;
            }
            else{
                opCompInsertionSort.count();
                if(key < v[m])
                    dr = m - 1;
                else
                    st = m + 1;
            }
        }

        int i = j - 1;
        while (i > poz) {

            opAttrInsertionSort.count();
            v[i + 1] = v[i];
            i--;

        }

        opAttrInsertionSort.count();
        v[i + 1] = key;
    }
}

void selectionSort(int v[], int n) {
    Operation opCompSelectionSort = p.createOperation("Comparatii selection sort", n);
    Operation opAttrSelectionSort = p.createOperation("Atribuiri selection sort", n);

    for (int i = 0; i < n - 1; i++) {
        int pmin = i;

        for (int j = i + 1; j < n; j++) {
            opCompSelectionSort.count();
            if (v[j] < v[pmin]) {
                pmin = j;
            }
        }

        opCompSelectionSort.count();
        if (pmin != i) {
            opAttrSelectionSort.count(3);
            int aux = v[i];
            v[i] = v[pmin];
            v[pmin] = aux;
        }
    }
}

void verif(int v1[], int v2[], int v3[], int n){
    for(int i = 0; i < n; i++)
        if(v1[i] != v2[i] or v1[i] != v3[i]){
            cout << "Greseala!";
            exit(0);
        }
}

void copy(int destinatie[], int sursa[], int n) {
    for (int i = 0; i < n; i++)
        destinatie[i] = sursa[i];
}

void afisare(int v[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

void demo() {
    int v[] = {8, 7, 4, 5, 2, 1, 9, 3};
    int n = sizeof(v) / sizeof(v[0]);

    int v1[n], v2[n], v3[n], v4[n];
    copy(v1, v, n);
    copy(v2, v, n);
    copy(v3, v, n);
    copy(v4, v, n);

    bubbleSort(v1, n);
    printf("Sirul sortat cu bubble sort:\n");
    afisare(v1, n);

    insertionSort(v2, n);
    printf("Sirtul sortat cu insertion sort:\n");
    afisare(v2, n);

    selectionSort(v3, n);
    printf("Sirtul sortat cu selection sort:\n");
    afisare(v3, n);

    insertionSortCuCB(v4, n);
    printf("Sirtul sortat cu insertion sort cu cautare binara:\n");
    afisare(v4, n);
}

void perf() {
    int v[maxSize], v1[maxSize], v2[maxSize], v3[maxSize];
    int n;

//    caz favorabil
    p.reset("Favorabil");

    for (n = stepSize; n <= maxSize; n += stepSize) {
        FillRandomArray(v, n, 10, 50000, false, ASCENDING);

        copy(v1, v, n);
        copy(v2, v, n);
        copy(v3, v, n);

        bubbleSort(v1, n);
        insertionSort(v2, n);
        selectionSort(v3, n);

        verif(v1, v2, v3, n);

    }

    p.addSeries("Total bubble sort", "Comparatii bubble sort", "Atribuiri bubble sort");
    p.addSeries("Total insertion sort", "Comparatii insertion sort", "Atribuiri insertion sort");
    p.addSeries("Total selection sort", "Comparatii selection sort", "Atribuiri selection sort");

    p.createGroup("Favorabil comparatii", "Comparatii bubble sort", "Comparatii insertion sort");
    p.createGroup("Favorabil comparatii selection sort", "Comparatii selection sort");

    p.createGroup("Favorabil atribuiri", "Atribuiri insertion sort", "Atribuiri selection sort", "Atribuiri bubble sort");

    p.createGroup("Favorabil total", "Total bubble sort", "Total insertion sort");
    p.createGroup("Favorabil total selection sort", "Total selection sort");



//    caz mediu
    p.reset("Mediu");

    for (n = stepSize; n <= maxSize; n += stepSize) {
        for (int test = 0; test < nrTests; test++) {
            FillRandomArray(v, n);
            copy(v1, v, n);
            copy(v2, v, n);
            copy(v3, v, n);

            bubbleSort(v1, n);
            insertionSort(v2, n);
            selectionSort(v3, n);

            verif(v1, v2, v3, n);

        }
    }

    p.divideValues("Comparatii bubble sort", nrTests);
    p.divideValues("Atribuiri bubble sort", nrTests);
    p.addSeries("Total bubble sort", "Comparatii bubble sort", "Atribuiri bubble sort");

    p.divideValues("Comparatii insertion sort", nrTests);
    p.divideValues("Atribuiri insertion sort", nrTests);
    p.addSeries("Total insertion sort", "Comparatii insertion sort", "Atribuiri insertion sort");

    p.divideValues("Comparatii selection sort", nrTests);
    p.divideValues("Atribuiri selection sort", nrTests);
    p.addSeries("Total selection sort", "Comparatii selection sort", "Atribuiri selection sort");

    p.createGroup("Mediu comparatii", "Comparatii bubble sort", "Comparatii insertion sort", "Comparatii selection sort");

    p.createGroup("Mediu atribuiri", "Atribuiri bubble sort", "Atribuiri insertion sort");
    p.createGroup("Mediu atribuiri selection sort", "Atribuiri selection sort");

    p.createGroup("Mediu total", "Total bubble sort", "Total insertion sort", "Total selection sort");

//    caz defavorabil
    p.reset("Defavorabil");

    for (n = stepSize; n <= maxSize; n += stepSize) {
        FillRandomArray(v, n, 10, 50000, false, DESCENDING);

        copy(v1, v, n);
        copy(v2, v, n);
        copy(v3, v, n);

        reverse(v3, v3 + n / 2);

        bubbleSort(v1, n);
        insertionSort(v2, n);
        selectionSort(v3, n);

        verif(v1, v2, v3, n);

    }

    p.addSeries("Total bubble sort", "Comparatii bubble sort", "Atribuiri bubble sort");
    p.addSeries("Total insertion sort", "Comparatii insertion sort", "Atribuiri insertion sort");
    p.addSeries("Total selection sort", "Comparatii selection sort", "Atribuiri selection sort");

    p.createGroup("Defavorabil comparatii", "Comparatii bubble sort", "Comparatii insertion sort", "Comparatii selection sort");

    p.createGroup("Defavorabil atribuiri", "Atribuiri bubble sort", "Atribuiri insertion sort");
    p.createGroup("Defavorabil atribuiri selection sort", "Atribuiri selection sort");

    p.createGroup("Defavorabil total", "Total bubble sort", "Total insertion sort", "Total selection sort");

    p.showReport();
}

int main() {

    clock_t begin = clock();
    demo();
//    perf();
    clock_t finish = clock();

    cout << (double) (finish - begin) / CLOCKS_PER_SEC;
    return 0;
}