/*
 * Simina Dan-Marius
 * Grupa 2
 *
 * La aceasta lucrare am implementat heapsort, quicksort, bubble sort atat in varianta iterativa cat si recursiva, si randomized select
 *
 * Heapsort functioneaza in felul urmator:
 *      -interschimba elementul maxim(cel de pe prima pozitie) cu elementul de pe ultima pozitie si reface heap-ul fara a mai lua in considerare elementul de pe ultima pozitie
 *      -repeta acest lucru pana cand heap-ul ajunge sa fie compus dintr-un singur element
 *      -complexitate O(n * log n)
 *
 * Quicksort functioneaza in felul urmator:
 *      -folosesc functia de partitionare in care imi aleg ca pivot ultimul element si mut toate elenetele mai mici deccat pivotul in prima parte a sirului
 *      -dupa mut pivotul pe pozitia corecta astfel toate elementele mai mici decat pivotul vor fi in stanga lui, iar cele mai mari in dreapta
 *      -cat timp vectorul e mai mare decat un singur element repet si fac quicksort pentru sirul din stanga si din dreapta pivotului
 *      -complexitate O(n * log n) pentru cazul favorabil si mediu, si O (n^2) pentru cazul defavorabil
 *
 * Bubble Sort functioneaza in felul urmator:
 *      -plec de la premiza ca sirul e sortat si verific acest lucru parcurgand sirul
 *      -daca in timp ce parcurg vectorul dau de doua elemente care nu sunt in ordinea corecta semnalez ca am gasit elemente care nu sunt in ordinea corecta (rezulta ca sirul nu e sortat) si le interschimb
 *      -repet algoritmul cat timp sirul nu e sortat
 *      -complexitate O(n^2)
 *
 * Randomized Select functioneaza in felul urmator:
 *      -ma folosesc de randomized partition in care aleg un pivot la intamplare si mut toate elementele mai mici decat pivotul in stanga lui si toate elementele mai mari in dreapta
 *      -daca pivotul ajunge exact pe pozitia pe care o cautam inseamna ca el apare pe acea pozitie
 *      -daca nu repet procesul in partea din dreapta daca caut o pozitie mai mica decat pozitia pivotului, respectiv in stanga daca nu
 *      -complexitate O(n * log n) in cazul favorabil si mediu, iar in cazul defavorabil O(n^2)
 *
 * Comparatie Heapsort si Quicksort:
 *      -in cazul favorabil quicksort face mai putine atribuiri si comparatii decat heapsort
 *      -in cazul mediu, din nou quicksort este mai eficient si face mai putine atribuiri cat si comparatii
 *      -in cazul defavorabil heapsort face mult mai putine atribuiri si comparatii decat quicksort
 *
 * Comparatii bubble sort iterativ si recursiv:
 *      -din punct de vedere al numarului de operatii ambele variante fac acelasi numar de operatii
 *      -din punct de vedere al timpului de executie varianta recursiva are un timp de executie mai mare, dar diferenta e foarte mica
*/

#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler p("Sortari");

#define maxSize 10000
#define stepSize 100
#define nrTests 10
#define nrTimeTests 100


struct heap {
    int length;
    int heapSize;
    int A[maxSize];
};

//Heapsort

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

void heapify(heap &H, int i) {

    Operation opCompHeapsort = p.createOperation("Comparatii heapsort", H.length);
    Operation opAttrHeapsort = p.createOperation("Atribuiri heapsort", H.length);

    int l = left(i);
    int r = right(i);
    int largest;

    if (l < H.heapSize) {

        opCompHeapsort.count();
        if (H.A[l] > H.A[i]) {
            largest = l;
        } else largest = i;
    } else largest = i;

    if (r < H.heapSize) {

        opCompHeapsort.count();
        if (H.A[r] > H.A[largest]) {
            largest = r;
        }
    }

    if (largest != i) {
        opAttrHeapsort.count(3);
        swap(H.A[i], H.A[largest]);
        heapify(H, largest);
    }
}

void buildHeap(heap &H) {

    Operation opCompHeapsort = p.createOperation("Comparatii heapsort", H.length);
    Operation opAttrHeapsort = p.createOperation("Atribuiri heapsort", H.length);

    opAttrHeapsort.count();
    H.heapSize = H.length;

    for (int i = H.heapSize / 2 - 1; i >= 0; i--) {
        heapify(H, i);
    }
}

void heapSort(heap &H) {

    Operation opCompHeapsort = p.createOperation("Comparatii heapsort", H.length);
    Operation opAttrHeapsort = p.createOperation("Atribuiri heapsort", H.length);

    buildHeap(H);

    for (int i = H.length - 1; i >= 1; i--) {
        opAttrHeapsort.count(4);
        swap(H.A[i], H.A[0]);

        opAttrHeapsort.count();
        H.heapSize--;

        heapify(H, 0);
    }
}

//Quicksort

int partition(int A[], int n, int l, int r) {

    Operation opCompQuicksort = p.createOperation("Comparatii quicksort", n);
    Operation opAttrQuicksort = p.createOperation("Atribuiri quicksort", n);


    int x = A[r];
    int i = l - 1;

    for (int j = l; j < r; j++) {
        opCompQuicksort.count();
        if (A[j] <= x) {
            i++;
            opAttrQuicksort.count(3);
            swap(A[i], A[j]);
        }
    }

    opAttrQuicksort.count(3);
    swap(A[i + 1], A[r]);
    return i + 1;
}

void quickSort(int A[], int n, int l, int r) {
    if (l >= r)
        return;

    int q = partition(A, n, l, r);
    quickSort(A, n, l, q - 1);
    quickSort(A, n, q + 1, r);

}

//Bubblesort iterativ

void bubbleSortIterativ(int v[], int n) {

    Operation opBubbleSortIterativ = p.createOperation("Operatii bubble sort iterativ", n);

    bool swapped;

    do {
        swapped = false;
        for (int i = 1; i < n; i++) {
            opBubbleSortIterativ.count();
            if (v[i - 1] > v[i]) {
                opBubbleSortIterativ.count(3);
                int aux = v[i - 1];
                v[i - 1] = v[i];
                v[i] = aux;

                swapped = true;
            }
        }
        n--;
    } while (swapped);
}

//Bubblesort recursiv

void bubbleSortRecursiv(int v[], int n, int nf) {

    Operation opBubbleSortRecursiv = p.createOperation("Operatii bubble sort recursiv", nf);

    bool swapped = false;
    for (int i = 1; i < n; i++) {
        opBubbleSortRecursiv.count();
        if (v[i - 1] > v[i]) {
            opBubbleSortRecursiv.count(3);
            int aux = v[i - 1];
            v[i - 1] = v[i];
            v[i] = aux;

            swapped = true;
        }
    }

    if (swapped)
        bubbleSortRecursiv(v, n - 1, nf);
}

//Randomized-Select

int randomizedPartition(int A[], int p, int r) {
    int j = rand() % (r - p) + p + 1;

    swap(A[r], A[j]);

    int x = A[r];
    int i = p - 1;

    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]);
    return i + 1;

}

int randomizedSelect(int A[], int p, int r, int i) {
    if(p == r)
        return A[p];

    int q = randomizedPartition(A, p, r);
    int k = q - p + 1;

    if(i == k)
        return A[q];
    else if(i < k)
        return randomizedSelect(A, p, q - 1, i);
    else
        return randomizedSelect(A, q + 1, r, i - k);
}

//Testare

void copy(int dest[], int sursa[], int n) {
    for (int i = 0; i < n; i++)
        dest[i] = sursa[i];
}

void afiseazaHeap(heap H) {
    for (int i = 0; i < H.length; i++)
        cout << H.A[i] << " ";
    cout << "\n";
}

void afiseazaVector(int A[], int n) {
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << "\n";
}

void demo() {
    int a[] = {1, 5, 2, 1, 2, 7, 2, 4, 3};
    int n = sizeof(a) / sizeof(a[0]);

    heap H1 = {0};
    H1.length = n;
    copy(H1.A, a, H1.length);

    heapSort(H1);
    cout << "Heapsort: ";
    afiseazaHeap(H1);

    int A[n];
    copy(A, a, n);

    quickSort(A, n, 0, n - 1);
    cout << "Quicksort: ";
    afiseazaVector(A, n);

    int v1[n], v2[n];

    copy(v1, a, n);
    bubbleSortIterativ(v1, n);
    cout << "Bubble sort iterativ: ";
    afiseazaVector(v1, n);

    copy(v2, a, n);
    bubbleSortRecursiv(v2, n, n);
    cout << "Bubble sort recursiv: ";
    afiseazaVector(v2, n);

    int Ap[n];
    copy(Ap, a, n);
    cout << "Randomized select (i = 5): " << randomizedSelect(Ap, 0, n - 1, 5);

}

void pregatire(int v[], int st, int dr) {
    if (st < dr) {
        int mijloc = (st + dr) / 2;

        pregatire(v, st, mijloc - 1);
        pregatire(v, mijloc + 1, dr);
        swap(v[dr], v[mijloc]);
    }
}

void perfQuicksortSiHeapsort() {
    int n;
    int a[maxSize];

//    caz favorabil
    p.reset("Favorabil");

    for (n = stepSize; n <= maxSize; n += stepSize) {
        FillRandomArray(a, n, 10, 50000, TRUE, DESCENDING);

        heap H1 = {0};
        H1.length = n;
        copy(H1.A, a, n);
        heapSort(H1);

        int A[maxSize];
        copy(A, a, n);
        reverse(A, A + n);
        pregatire(A, 0, n - 1);
        quickSort(A, n, 0, n - 1);
    }

    p.addSeries("Total heapsort", "Comparatii heapsort", "Atribuiri heapsort");
    p.addSeries("Total quicksort", "Comparatii quicksort", "Atribuiri quicksort");

    p.createGroup("Favorabil comparatii", "Comparatii heapsort", "Comparatii quicksort");
    p.createGroup("Favorabil atribuiri", "Atribuiri heapsort", "Atribuiri quicksort");
    p.createGroup("Favorabil total", "Total heapsort", "Total quicksort");

//    caz mediu
    p.reset("Mediu");

    for (n = stepSize; n <= maxSize; n += stepSize) {
        for (int test = 0; test < nrTests; test++) {
            FillRandomArray(a, n, 10, 50000, TRUE, UNSORTED);

            heap H1 = {0};
            H1.length = n;
            copy(H1.A, a, n);
            heapSort(H1);

            int A[maxSize];
            copy(A, a, n);
            quickSort(A, n, 0, n - 1);
        }
    }

    p.divideValues("Comparatii heapsort", nrTests);
    p.divideValues("Comparatii quicksort", nrTests);
    p.divideValues("Atribuiri heapsort", nrTests);
    p.divideValues("Atribuiri quicksort", nrTests);

    p.addSeries("Total heapsort", "Comparatii heapsort", "Atribuiri heapsort");
    p.addSeries("Total quicksort", "Comparatii quicksort", "Atribuiri quicksort");

    p.createGroup("Mediu comparatii", "Comparatii heapsort", "Comparatii quicksort");
    p.createGroup("Mediu atribuiri", "Atribuiri heapsort", "Atribuiri quicksort");
    p.createGroup("Mediu total", "Total heapsort", "Total quicksort");

//    caz defavorabil
    p.reset("Defavorabil");

    for (n = stepSize; n <= maxSize; n += stepSize) {
        FillRandomArray(a, n, 10, 50000, TRUE, ASCENDING);

        heap H1 = {0};
        H1.length = n;
        copy(H1.A, a, n);
        heapSort(H1);

        int A[maxSize];
        copy(A, a, n);
        quickSort(A, n, 0, n - 1);
    }

    p.addSeries("Total heapsort", "Comparatii heapsort", "Atribuiri heapsort");
    p.addSeries("Total quicksort", "Comparatii quicksort", "Atribuiri quicksort");

    p.createGroup("Defavorabil comparatii", "Comparatii heapsort", "Comparatii quicksort");
    p.createGroup("Defavorabil comparatii heapsort", "Comparatii heapsort");
    p.createGroup("Defavorabil atribuiri", "Atribuiri heapsort", "Atribuiri quicksort");
    p.createGroup("Defavorabil atribuiri heapsort", "Atribuiri heapsort");
    p.createGroup("Defavorabil total", "Total heapsort", "Total quicksort");
    p.createGroup("Defavorabil total heapsort", "Total heapsort");

    p.showReport();
}

void perfBubbleSort() {
    int n;
    int a[maxSize];
    int v1[maxSize], v2[maxSize];

    p.reset("Bubble sort");

    for (n = stepSize; n <= maxSize; n += stepSize) {
        for (int test = 0; test < nrTests; test++) {
            FillRandomArray(a, n, 10, 50000, 0, UNSORTED);
            copy(v1, a, n);
            bubbleSortIterativ(v1, n);

            copy(v2, a, n);
            bubbleSortRecursiv(v2, n, n);
        }
    }

    for (n = stepSize; n <= maxSize; n += stepSize) {

        FillRandomArray(a, n, 10, 50000, 0, UNSORTED);

        p.startTimer("Bubble sort iterativ timp", n);
        for (int test = 0; test < nrTimeTests; test++) {
            copy(v1, a, n);
            bubbleSortIterativ(v1, n);
        }
        p.stopTimer("Bubble sort iterativ timp", n);

        p.startTimer("Bubble sort recursiv timp", n);
        for (int test = 0; test < nrTimeTests; test++) {
            copy(v2, a, n);
            bubbleSortRecursiv(v2, n, n);
        }
        p.stopTimer("Bubble sort recursiv timp", n);

    }

    p.divideValues("Operatii bubble sort iterativ", nrTests);
    p.divideValues("Operatii bubble sort recursiv", nrTests);

    p.divideValues("Bubble sort iterativ timp", nrTimeTests);
    p.divideValues("Bubble sort recursiv timp", nrTimeTests);

    p.createGroup("Total operatii", "Operatii bubble sort iterativ", "Operatii bubble sort recursiv");

    p.createGroup("Timp", "Bubble sort iterativ timp", "Bubble sort recursiv timp");

    p.showReport();

}

int comp(int s1[], int s2[], int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i])
            return 1;
    }

    return 0;
}

void verifSortari() {
    int n;
    int a[maxSize];
    int vQuicksort[maxSize], vBubbleSortIterativ[maxSize], vBubbleSortRecursiv[maxSize], vRandomizedQuicksort[maxSize];
    heap H;

    for (n = stepSize; n <= maxSize; n += stepSize) {
        FillRandomArray(a, n);

        H.length = n;
        copy(H.A, a, n);
        copy(vQuicksort, a, n);
        copy(vBubbleSortIterativ, a, n);
        copy(vBubbleSortRecursiv, a, n);
        copy(vRandomizedQuicksort, a, n);

        sort(a, a + n);

        heapSort(H);
        if (comp(H.A, a, n) != 0) {
            cout << "Greseala heapsort";
            exit(0);
        }

        quickSort(vQuicksort, n, 0, n - 1);
        if (comp(vQuicksort, a, n) != 0) {
            cout << "Greseala quicksort";
            exit(0);
        }

        bubbleSortIterativ(vBubbleSortIterativ, n);
        if (comp(vBubbleSortIterativ, a, n) != 0) {
            cout << "Greseala bubble sort iterativ";
            exit(0);
        }

        bubbleSortRecursiv(vBubbleSortRecursiv, n, n);
        if (comp(vBubbleSortRecursiv, a, n) != 0) {
            cout << "Greseala bubble sort recursiv";
            exit(0);
        }
    }
    cout << "ok";
}

int main() {
    demo();
//    perfQuicksortSiHeapsort();
//    perfBubbleSort();
//    verifSortari();

    return 0;
}
