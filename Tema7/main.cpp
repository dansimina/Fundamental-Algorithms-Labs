/*
 * Simina Dan-Marius
 * Grupa 2
 *
 * Pentru build tree iau un vectorul cu elementele ordonate si capetele lui(left si right) cu care vreau sa creez arborele perfect echilibrat.
 * Apoi creez mereu nodul curent in care tin elementul din mijloc, size-ul este egal cu numarul de elemente din bucata de vector pe
 * care o prelucrez la acel moment, iar pentru copii lui apelez recursiv pentru bucata de vector de dinaintea mijlocului(left, mijloc - 1)
 * si pentru bucata de vector de dupa elementul din mijloc (mijloc + 1, right).
 * Complexitate O(n).
 * Inaltimea arborelui e aproximativ O(log n).
 *
 * Pentru os select plec de la radacina si caut elementul cu rank-ul i, pentru asta compar i cu rank-ul nodului curent, iar
 * daca rank-ul lui e egal cu i inseamna ca l-am gasit, daca i este mai mic ca rank-ul nodului curent atunci o sa caut i in continuare
 * in subarborele din partea stanga, iar daca i este mai mare decat rank-ul nodului curent o sa caut nodul cu rank-ul i - rank-ul nodului
 * curent din partea dreapta.
 * Complexitate O(log n).
 *
 * Pentru os delete fac mai intai stergerea clasica pentru un element din arborele binar de cautare, iar mai apoi merg din punctul din care
 * se sterge nodul in sus pe arbore si refac size-ul fiecarui nod. Arborele nu trebuie rebalansat deoarece stergerea nu creste inaltimea si
 * chiar daca arborele nu e balansat complexitatea la os select si os delete ramane tot O(log n).
 * Complexitate O(log n).
 *
 * OS select seamana cu randomized selection.
 */

#include <iostream>
#include "Profiler.h"

using namespace std;

#define maxSize 10000
#define stepSize 100
#define nrTests 5

Profiler profiler("arbori");

///Arbore binar de cautare echilibrat:

struct Node {
    int key;
    int size;
    Node *left, *right, *parent;
};

Node *create(int key, int size) {
    Node *p = new Node;
    p->key = key;
    p->size = size;
    p->left = NULL;
    p->right = NULL;
    p->parent = NULL;

    return p;
}

Node *buildTree(int elem[], int left, int right, Node *parent) {
    if (left > right)
        return NULL;

    int m = (left + right) / 2;

    Node *node = create(elem[m], right - left + 1);
    node->parent = parent;
    node->left = buildTree(elem, left, m - 1, node);
    node->right = buildTree(elem, m + 1, right, node);

    return node;
}

Node *osSelect(Node *root, int i, int n) {
    Operation opTotale = profiler.createOperation("Operatii select", n);

    int rank = 1;
    opTotale.count();
    if (root->left != NULL)
        rank = (root->left)->size + 1;

    opTotale.count();
    if (i == rank) {
        return root;
    } else {
        opTotale.count();
        if (i < rank)
            return osSelect(root->left, i, n);

        return osSelect(root->right, i - rank, n);
    }
}

Node *treeSuccessor(Node *node, int n) {
    Operation opTotale = profiler.createOperation("Operatii delete", n);

    opTotale.count();
    if (node->right != NULL) {
        opTotale.count();
        node = node->right;

        opTotale.count();
        while (node->left != NULL) {
            opTotale.count(2);
            node = node->left;
        }
        return node;
    }

    opTotale.count();
    Node *parent = node->parent;

    opTotale.count(2);
    while (parent != NULL and node == parent->right) {
        opTotale.count(4);
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

Node *osDelete(Node *root, Node *nodeZ, int n) {
    Operation opTotale = profiler.createOperation("Operatii delete", n);

    Node *nodeY = NULL, *nodeX = NULL;

    opTotale.count(3);
    if (nodeZ->left == NULL or nodeZ->right == NULL) {
        nodeY = nodeZ;
    } else {
        nodeY = treeSuccessor(nodeZ, n);
    }

    opTotale.count(2);
    if (nodeY->left != NULL) {
        nodeX = nodeY->left;
    } else {
        nodeX = nodeY->right;
    }

    opTotale.count();
    if (nodeX != NULL) {
        opTotale.count();
        nodeX->parent = nodeY->parent;
    }

    opTotale.count();
    if (nodeY->parent == NULL) {
        root = nodeX;
    } else {
        opTotale.count(2);
        if (nodeY == (nodeY->parent)->left) {
            (nodeY->parent)->left = nodeX;
        } else {
            (nodeY->parent)->right = nodeX;
        }
    }

    opTotale.count();
    if (nodeY != nodeZ) {
        opTotale.count();
        nodeZ->key = nodeY->key;
        //nodeZ->size = nodeY->size;
    }

    for (Node *p = nodeY; p != NULL; p = p->parent) {
        opTotale.count(3);
        p->size = (p->left == NULL ? 0 : (p->left)->size) + (p->right == NULL ? 0 : (p->right)->size) + 1;
    }

    delete nodeY;

    return root;
}

int *generareVector(int n) {
    int *a = new int[n];
    for (int i = 0; i < n; i++)
        a[i] = i + 1;

    return a;
}

void prettyPrint(Node *root, int level) {
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << root->key << " size: " << root->size << "\n";

    prettyPrint(root->left, level + 1);
    prettyPrint(root->right, level + 1);
}

void demo() {
    int n = 11;
    int *a = generareVector(n);

    Node *root = buildTree(a, 0, n - 1, NULL);
    delete[] a;

    prettyPrint(root, 0);

    Node *p;

    p = osSelect(root, 11, n);
    cout << "\nNod de sters " << p->key << "\n";
    root = osDelete(root, p, n);
    prettyPrint(root, 0);

    p = osSelect(root, 3, n);
    cout << "\nNod de sters " << p->key << "\n";
    root = osDelete(root, p, n);
    prettyPrint(root, 0);

    p = osSelect(root, 5, n);
    cout << "\nNod de sters " << p->key << "\n";
    root = osDelete(root, p, n);
    prettyPrint(root, 0);
}

void perf() {
    for (int n = stepSize; n <= maxSize; n += stepSize) {
        for (int test = 0; test < nrTests; test++) {
            int *a = generareVector(n);
            Node *root = buildTree(a, 0, n - 1, NULL);
            delete[] a;

            for (int i = 0; i < n; i++) {
                int p = rand() % (root->size) + 1;

                Node *node = osSelect(root, p, n);
                root = osDelete(root, node, n);

            }
        }
    }

    profiler.divideValues("Operatii select", nrTests);
    profiler.divideValues("Operatii delete", nrTests);
    profiler.createGroup("Operatii", "Operatii select", "Operatii delete");
    profiler.showReport();
}

int main() {

    demo();
//    perf();
    return 0;
}
