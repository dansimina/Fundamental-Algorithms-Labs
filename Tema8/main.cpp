/*
 * Simina Dan
 * Grupa 2
 *
 * Pentru construirea arborelui pe care urmeaza sa fac parcurgerile ma folosesc de algoritmul de reconstructie a unui arbore binar de cautare
 * la care ii trimit un vector de elemente generate aleator.
 * Complexitate O(n^2).
 *
 * Pentru parcurgerea recursiva cat timp radacina nu e NULL apelez recursiv pentru fiul stang si fiul drept, iar afisarea o fac intre cele
 * doua apeluri astfel parcurgerea fiind in inordine.
 * Complexitate O(n).
 *
 * Pentru parcurgerea iterativa cat timp radacina nu e NULL marchez fiecare nod in ce stadiu e (poate sa fie in 1, 2 sau 3):
 *      -daca nodul e in stadiul 1 inseamna ca am ajuns in el pentru prima data, il marchez ca trece in stadiul 2 si incerc sa merg pe
 *      ramura stanga
 *      -daca nodul e in stadiul 2 inseamna ca m-am intors de pe ramura stanga, il marchez ca trece in stadiul 3 si incerc sa merg pe
 *      ramura dreapta, totodata afisez cand nodul e in stadiul 2 astfel parcurgerea este in inordine
 *      -daca nodul e in stadiul 3 inseamna ca am revenit si din ramura dreapta si am terminat cu el si ma duc inapoi in tatal sau.
 * Complexitate O(n).
 *
 * Ambele parcurgeri fac acelasi numar de operatii.
 *
 * Pentru quicksort ma folosesc de o functie de partitionare care imparte toate elemetele din sir in functie de un pivot, toate elementele
 * mai mici decat pivotul in stanga sa si toate elementele mai mari in dreapta sa, iar apoi apelez recursiv functia pentru partea din stanga
 * si partea din dreapta a pivotului.
 * Complexitate O(n^2).
 *
 * Pentru quicksort hibritizat fac acelasi lucru ca la quicksort doar ca pentru siruri de dimensiuni mici aplic direct insertionsort.
 * Complexitate O(n^2).
 *
 * Din punct de vedere al numarului de operatii ambele variante de quicksort fac un numar asemanator.
 * Din punct de vedere al timpului de executie varianta hibrida este mult mai eficienta.
 *
 * Un optim al pragului folosit in hibritizare este undeva intre 20 - 30, deoarece pentru valori din acest interval algoritmul face
 * un numar redus de operatii si are si un timp de executie mic
 */

#include <iostream>
#include "Profiler.h"

using namespace std;

#define maxSize 10000
#define  stepSize 100
#define nrTestsTime 1000
#define nrTestsOp1 10
#define nrTestsOp2 100
#define maxThreshold 100

Profiler profiler("tema8");

///Parcurgere recursiva si iterativa a unui arbore binar
struct Node {
    int key;
    int stage;
    Node *left;
    Node *right;
    Node *parent;
};

Node *create(int key) {
    Node *node = new Node;
    node->key = key;
    node->stage = 1;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

int split(int elem[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        if (elem[left] < elem[i])
            return i - 1;
    }
    return right;
}

Node *buildTree(int elem[], int left, int right, Node *parent) {
    if (left > right)
        return NULL;

    Node *node = create(elem[left]);
    node->parent = parent;

    int pos = split(elem, left, right);
    node->left = buildTree(elem, left + 1, pos, node);
    node->right = buildTree(elem, pos + 1, right, node);

    return node;
}

void recursiveInorder(Node *root, bool perf, int n) {
    Operation opTotale = profiler.createOperation("Inordine recursiv", n);

    if (root == NULL)
        return;

    recursiveInorder(root->left, perf, n);
    if (perf == 0)
        cout << root->key << " ";
    else
        opTotale.count();
    recursiveInorder(root->right, perf, n);
}

void iterativeInorder(Node *root, bool perf, int n) {
    Operation opTotale = profiler.createOperation("Inordine iterativ", n);

    while (root != NULL) {
        if (root->stage == 1) {
            root->stage = 2;
            if (root->left != NULL) {
                root = root->left;
            }
        }

        if (root->stage == 2) {
            root->stage = 3;
            if (perf == 0)
                cout << root->key << " ";
            else
                opTotale.count();

            if (root->right != NULL) {
                root = root->right;
            }
        }

        if (root->stage == 3) {
            root = root->parent;
        }
    }
}

void prettyPrint(Node *root, int level) {
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << root->key << "\n";

    prettyPrint(root->left, level + 1);
    prettyPrint(root->right, level + 1);
}

///Hibridizare quicksort.

void insertionSort(int a[], int left, int right, int n) {
    Operation opTotale = profiler.createOperation("Operatii QuickSort hibrid", n);

    for (int j = left; j <= right; j++) {
        int key = a[j];

        int i = j - 1;
        while (i >= left) {
            opTotale.count();
            if (a[i] > key) {
                opTotale.count();
                a[i + 1] = a[i];
                i--;
            } else break;
        }

        opTotale.count();
        a[i + 1] = key;
    }
}

int partitionQuickSortHybrid(int a[], int left, int right, int n) {
    Operation opTotale = profiler.createOperation("Operatii QuickSort hibrid", n);
//    int pos = rand() % (right - left + 1) + left;
//    swap(a[pos], a[right]);
    int x = a[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        opTotale.count();
        if (a[j] <= x) {
            i++;
            opTotale.count(3);
            swap(a[i], a[j]);
        }
    }

    opTotale.count(3);
    swap(a[i + 1], a[right]);
    return i + 1;
}

void quickSortHybrid(int a[], int left, int right, int threshold, int n) {
    Operation opTotale = profiler.createOperation("Operatii QuickSort hibrid", n);

    if (left > right)
        return;

    opTotale.count();
    if (right - left + 1 > threshold) {
        int pivot = partitionQuickSortHybrid(a, left, right, n);
        quickSortHybrid(a, left, pivot - 1, threshold, n);
        quickSortHybrid(a, pivot + 1, right, threshold, n);
    } else {
        insertionSort(a, left, right, n);
    }
}

int partitionQuickSort(int a[], int left, int right, int n) {
    Operation opTotale = profiler.createOperation("Operatii QuickSort", n);
//    int pos = rand() % (right - left + 1) + left;
//    swap(a[pos], a[right]);
    int x = a[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        opTotale.count();
        if (a[j] <= x) {
            i++;
            opTotale.count(3);
            swap(a[i], a[j]);
        }
    }

    opTotale.count(3);
    swap(a[i + 1], a[right]);
    return i + 1;
}

void quickSort(int a[], int left, int right, int n) {
    Operation opTotale = profiler.createOperation("Operatii QuickSort", n);

    if (left > right)
        return;

    int pivot = partitionQuickSort(a, left, right, n);
    quickSort(a, left, pivot - 1, n);
    quickSort(a, pivot + 1, right, n);
}

///demo si perf
void display(int a[], int n) {
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << "\n";
}

void demo() {
    int a[] = {10, 5, 3, 9, 7, 8, 20, 15, 12, 17};
    int n = sizeof(a) / sizeof(a[0]);
    int aTest[n];

    cout << "Parcurgere recursiva si iterativa a unui arbore binar:\n";
    Node *root = buildTree(a, 0, n - 1, NULL);

    prettyPrint(root, 0);
    recursiveInorder(root, false, n);
    cout << "\n";
    iterativeInorder(root, false, n);
    cout << "\n";

    copy(a, a + n, aTest);
    cout << "\nQuicksort fara hibritizare \n";
    quickSort(aTest, 0, n - 1, 0);
    display(aTest, n);

    copy(a, a + n, aTest);
    cout << "\nHibridizare quicksort utilizand insertion sort iterativ: \n";
    quickSortHybrid(aTest, 0, n - 1, 5, 0);
    display(aTest, n);
}

void perfInorder() {
//    Testare parcurgeri arbori
    profiler.reset("parcurgeri");

    for (int n = stepSize; n <= maxSize; n += stepSize) {
        for (int test = 1; test <= nrTestsOp1; test++) {
            int a[maxSize] = {0};
            FillRandomArray(a, n, 10, 50000, true, UNSORTED);
            Node *root = buildTree(a, 0, n - 1, NULL);
            recursiveInorder(root, true, n);
            iterativeInorder(root, true, n);
        }
    }

    profiler.divideValues("Inordine recursiv", nrTestsOp1);
    profiler.divideValues("Inordine iterativ", nrTestsOp1);

    profiler.createGroup("Parcurgeri", "Inordine recursiv", "Inordine iterativ");

    profiler.showReport();
}

void perfQuicksort() {
//    Quicksort hibritizat vs quicksort normal
    profiler.reset("Quicksort hibritizat vs quicksort normal");

    for (int n = stepSize; n <= maxSize; n += stepSize) {

        int a[maxSize] = {0};
        int a1[maxSize] = {0};
        int a2[maxSize] = {0};

        for (int test = 1; test <= nrTestsOp2; test++) {
            FillRandomArray(a, n, 10, 50000, true, UNSORTED);

            copy(a, a + n, a1);
            quickSort(a1, 0, n - 1, n);

            copy(a, a + n, a2);
            quickSortHybrid(a2, 0, n - 1, 30, n);
        }

        FillRandomArray(a, n, 10, 50000, true, UNSORTED);


        profiler.startTimer("Timp QuickSort", n);
        for (int test = 1; test <= nrTestsTime; test++) {
            copy(a, a + n, a1);
            quickSort(a1, 0, n - 1, n);
        }
        profiler.stopTimer("Timp QuickSort", n);

        profiler.startTimer("Timp QuickSort hibrid", n);
        for (int test = 1; test <= nrTestsTime; test++) {
            copy(a, a + n, a2);
            quickSortHybrid(a2, 0, n - 1, 30, n);
        }
        profiler.stopTimer("Timp QuickSort hibrid", n);

    }

    profiler.divideValues("Operatii QuickSort hibrid", nrTestsOp2);
    profiler.divideValues("Operatii QuickSort", nrTestsOp2);
    profiler.createGroup("Operatii", "Operatii QuickSort hibrid", "Operatii QuickSort");

    profiler.divideValues("Timp QuickSort hibrid", nrTestsTime);
    profiler.divideValues("Timp QuickSort", nrTestsTime);
    profiler.createGroup("Timp", "Timp QuickSort hibrid", "Timp QuickSort");

    profiler.showReport();
}

void perfThreshold() {
//    Testare quicksort hibritizat.
    profiler.reset("Testare quicksort hibritizat.");

    int n = maxSize;
    for (int i = 0; i <= maxThreshold; i++) {

        int a[maxSize] = {0};
        int aTest[maxSize] = {0};

        for (int test = 1; test <= nrTestsOp2; test++) {
            FillRandomArray(a, n, 10, 50000, true, UNSORTED);
            quickSortHybrid(a, 0, n - 1, i, i);
        }

        FillRandomArray(a, n, 10, 50000, true, UNSORTED);

        profiler.startTimer("Timp QuickSort hibrid variez prag", i);
        for (int test = 0; test < nrTestsTime; test++) {
            copy(a, a + n, aTest);
            quickSortHybrid(aTest, 0, n - 1, i, i);
        }
        profiler.stopTimer("Timp QuickSort hibrid variez prag", i);
    }

    profiler.divideValues("Timp QuickSort hibrid variez prag", nrTestsTime);
    profiler.divideValues("Operatii QuickSort hibrid", nrTestsOp2);

    profiler.showReport();
}

int main() {

    demo();
//    perfInorder();
//    perfQuicksort();
//    perfThreshold();
    return 0;
}
