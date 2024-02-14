/*
 * Simina Dan
 * Grupa 2
 *
 * Pentru acest laborator am implementat operatiile pe multimi disjuncte si algoritmul lui kruskal.
 *
 * Pentru makeset creez o multime noua care sa contina elementul key. Aloc memorie pentru un nod nou ii dau valoarea cheii,
 * ii pun rank-ul pe 0, iar ca parinte pun nodul insusi.
 * Complexitate O(1).
 *
 * Pentru findset merg pe parinte a nodului pana cand ajung la elementul care reprezinta set-ul.
 * Complexitate O(n).
 *
 * Pentru reunion leg multimea cu mai putine elemente de multimea cu mai multe elemente. In caz ca multimile au acelasi numar de elemente
 * actualizez rank-ul.
 * Complexitate O(n).
 *
 * Pentru algoritmul lui kruskal mai intai transform fiecare nod intr-o multime si sortez muchiile arborelui in functie de cost.
 * Apoi selectez pe rand fiecare muchie verific daca cumva inchide un ciclu in arborele de acoperire, iar daca nu o inserez in arborele
 * de acoperire.
 * Complexitate O(E^2).
 */
#include <iostream>
#include "Profiler.h"

using namespace std;

#define maxValue 50000
#define maxSize 10000
#define stepSize 100
#define nrTests 5

Profiler profiler("lab9");

//Mulțimi disjuncte

struct NodeSet {
    int key;
    int rank;
    NodeSet *parent;
};

NodeSet *makeSet(int key, int n) {
    Operation opTotale = profiler.createOperation("Operatii", n);

    NodeSet *x = new NodeSet;
    opTotale.count(3);
    x->key = key;
    x->parent = x;
    x->rank = 0;

    return x;
}

NodeSet *findSet(NodeSet *x, int n) {
    Operation opTotale = profiler.createOperation("Operatii", n);

    opTotale.count();
    if (x != x->parent) {
        opTotale.count();
        x->parent = findSet(x->parent, n);
    }
    return x->parent;
}

void unify(NodeSet *x, NodeSet *y, int n) {
    Operation opTotale = profiler.createOperation("Operatii", n);

    opTotale.count();
    if (x->rank > y->rank) {
        opTotale.count();
        y->parent = x;
    } else {
        opTotale.count();
        x->parent = y;
    }

    opTotale.count();
    if (x->rank == y->rank) {
        opTotale.count();
        (y->rank)++;
    }
}

void reunion(NodeSet *x, NodeSet *y, int n) {
    unify(findSet(x, n), findSet(y, n), n);
}

//Algoritmul lui Kruskal
struct Node {
    int key;
    Node *next;
};

struct Edge {
    int u, v, w;
};

struct Graph {
    int nVertices;
    int nEdges;

    Node **lists = NULL;
    Edge *edges = NULL;
};

Node *create(int key) {
    Node *node = new Node;
    node->key = key;
    node->next = NULL;

    return node;
}

void insertFirst(Node *&first, int key) {
    Node *node = create(key);
    if (first == NULL) {
        first = node;
    } else {
        node->next = first;
        first = node;
    }
}

void displayList(Node *first) {
    for (Node *i = first; i != NULL; i = i->next) {
        cout << i->key << " ";
    }
    cout << "NULL";
}

Graph generate(int n) {
    int **adMatrix = new int *[n];
    for (int i = 0; i < n; i++) {
        adMatrix[i] = new int[n]{0};
    }
    int nComponents = n;

    NodeSet *a[maxSize] = {NULL};
    for (int i = 0; i < n; i++) {
        a[i] = makeSet(i, n);
    }

    Graph G = {NULL};
    G.nVertices = n;
    G.nEdges = 4 * n < n * (n - 1) / 2 ? 4 * n : n * (n - 1) / 2;
    G.lists = new Node *[n]{NULL};
    G.edges = new Edge[G.nEdges]{NULL};

    int cntEdges = 0;
    while (cntEdges < G.nEdges - nComponents + 1) {
        int u = rand() % n;
        int v = rand() % n;
        int w = rand() % maxValue;

        if (u != v and adMatrix[u][v] == 0 and adMatrix[v][u] == 0) {
            adMatrix[u][v] = 1;
            adMatrix[v][u] = 1;
            insertFirst(G.lists[u], v);
            insertFirst(G.lists[v], u);
            G.edges[cntEdges] = Edge{u, v, w};
            cntEdges++;

            if (findSet(a[u], n) != findSet(a[v], n)) {
                reunion(a[u], a[v], n);
                nComponents--;
            }
        }
    }

    for (int i = 1; i < n && nComponents > 1; i++) {
        if (findSet(a[0], n) != findSet(a[i], n)) {
            int w = rand() % maxValue;
            adMatrix[0][i] = 1;
            adMatrix[0][i] = 1;
            insertFirst(G.lists[0], i);
            insertFirst(G.lists[i], 0);
            G.edges[cntEdges] = Edge{0, i, w};
            nComponents--;
            reunion(a[0], a[i], n);
        }
    }

    for (int i = 0; i < n; i++) {
        delete adMatrix[i];
    }
    delete[] adMatrix;

    return G;
}

void displayGraph(Graph G) {
    cout << "\n\nNumar noduri: " << G.nVertices;
    cout << "\nNumar muchii: " << G.nEdges;

    cout << "\n\n Liste de adiacenta:";
    for (int i = 0; i < G.nVertices; i++) {
        cout << "\n" << i << ": ";
        displayList(G.lists[i]);
    }

    cout << "\n\n Costuri muchii: \n";
    for (int i = 0; i < G.nEdges; i++) {
        cout << G.edges[i].u << " " << G.edges[i].v << " " << G.edges[i].w << "\n";
    }

    cout << "\n";
}

int partition(Edge a[], int left, int right) {
    int pos = rand() % (right - left + 1) + left;
    swap(a[pos], a[right]);
    Edge x = a[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (a[j].w <= x.w) {
            i++;
            swap(a[i], a[j]);
        }
    }

    swap(a[i + 1], a[right]);
    return i + 1;
}

void quickSort(Edge a[], int left, int right) {
    if (left > right)
        return;

    int pivot = partition(a, left, right);
    quickSort(a, left, pivot - 1);
    quickSort(a, pivot + 1, right);
}

Graph kruskal(Graph G, int n) {
    Graph A = {0};
    A.nVertices = G.nVertices;
    A.nEdges = G.nVertices - 1;

    A.lists = new Node *[A.nVertices]{NULL};
    A.edges = new Edge[A.nEdges]{NULL};

    NodeSet *a[maxSize] = {NULL};
    for (int i = 0; i < G.nVertices; i++) {
        a[i] = makeSet(i, n);
    }

    quickSort(G.edges, 0, G.nEdges - 1);

    A.nEdges = 0;
    for (int i = 0; i < G.nEdges; i++) {
        if (findSet(a[G.edges[i].u], n) != findSet(a[G.edges[i].v], n)) {
            A.edges[A.nEdges++] = G.edges[i];
            insertFirst(A.lists[G.edges[i].u], G.edges[i].v);
            insertFirst(A.lists[G.edges[i].v], G.edges[i].u);

            reunion(a[G.edges[i].u], a[G.edges[i].v], n);
        }
    }

    return A;
}

void demo() {
    const int n = 10;
    NodeSet *a[n] = {NULL};
    for (int i = 0; i < n; i++) {
        a[i] = makeSet(i, n);
    }

    for (int i = 0; i < n; i++) {
        cout << findSet(a[i], n)->key << " ";
    }
    cout << "\n";

    reunion(a[2], a[4], n);
    reunion(a[1], a[3], n);
    reunion(a[2], a[6], n);
    reunion(a[4], a[8], n);
    reunion(a[1], a[5], n);

    for (int i = 0; i < n; i++) {
        cout << findSet(a[i], n)->key << " ";
    }

    Graph G = generate(n);
    displayGraph(G);

    Graph A = kruskal(G, n);
    displayGraph(A);

}

void perf() {
    for (int n = stepSize; n <= maxSize; n += stepSize) {
        for (int test = 1; test <= nrTests; test++) {
            Graph G = generate(n);
            Graph A = kruskal(G, n);
        }
    }

    profiler.divideValues("Operatii", nrTests);

    profiler.showReport();
}

int main() {
//    demo();
    perf();
    return 0;
}
