/*
 * Simina Dan-Marius
 * Grupa 2
 *
 * Pentru acest laborator am implementat dfs, sortare topologica si tarjan.
 *
 * Pentru dfs mai intai initializez toate nodurile din graf cu culoare alba, parintele NULL si timpii de finalizare, iar timpul global il
 * pun pe 0 apoi aplic dfs visit pentru toate nodurile albe din graf. In dfs visit mai intai procesez nodul curent si ii setez timpul de
 * descoperire, culoarea o pun pe gri, iar apoi apelez recursiv dfs visit pentru toti vecinii lui. La final cand termin cu nodul curent
 * ii setez timpul de finalizare si ii pun culoarea pe negru. Totodata in dfs verific daca am muchie de la nod gri la nod gri adica daca
 * am graful e aciclic si returnez true daca nu are ciclu si false daca are.
 * Complexitate O(V + E).
 *
 * Pentru sortare topologica apelez dfs iar in partea de vizitari cand setez timpul de finalizare pentru nod il adaug si la inceputul unei
 * liste simplu inlantuite, iar daca dupa terminarea dfs-ului obtin ca graful e aciclic returnez lista, iar daca graful are cicluri
 * returnez o lista goala pentru ca daca graful are cicluri nu are sortare topologica.
 * Complexitate O(V + E).
 *
 * Pentru tarjan folosesc un dfs modificat. Doar ca aici in loc de timpi folosesc index care reprezinta cate noduri au fost vizitate
 * inainte de nodul curent, lowLink care reprezinta indexul radacinii componentei tare conexe si o stiva in care tin nodurile vizitate.
 * Totodata initializez indexul fiecarui nod cu -1 si nu mai folosesc culori pentru ca indexul = -1 indica ca nodul e nevizitat.
 * Pe langa partea de vizitare a nodurilor si actualizarea campurilor nodului curent, la revenirea din apelurile recursive mai verific
 * daca nodul curent e radacina unei noi componente tare conexe, adica daca lowLink e egal cu index, iar daca da inseamna ca avem o
 * noua componenta conexa(cresc nr de componente) si scot din stiva toate nodurile vizitate pana la nodul curent si la marchez ca fac parte
 * din aceeasi componenta tare conexa.
 * Complexitate O(V + E).
 *
 * La performance putem observa din grafic cum variaza numarul de operatii mai intai cand numarul de noduri e constant si variem muchiile,
 * iar mai apoi cand numarul de muchii e constant si variem nodurile.
 */
#include <iostream>
#include <fstream>
#include "Profiler.h"

using namespace std;

#define MAX_VERTICES 200
#define MAX_EDGES 4500
#define STEP_VERTICES 10
#define STEP_EDGES 100

Profiler profiler("dfs");

enum Color {
    WHITE, GRAY, BLACK
};

struct Node {
    int key;

    int adjSize;
    Node **adj;

    Color color;
    Node *parent;
    int discovered;
    int finished;

    int index;
    int lowLink;
    bool onStack;
    int comp;
};

struct Graph {
    int nVertices;
    Node **verices;
};

struct Edge {
    int u;
    int v;
};

struct NodeLinkedList {
    Node *key;
    NodeLinkedList *next;
};

struct LinkedList {
    NodeLinkedList *first = NULL;
};

void insertFirst(NodeLinkedList **first, Node *key) {
    NodeLinkedList *node = new NodeLinkedList;
    node->next = NULL;
    node->key = key;

    if (*first == NULL) {
        *first = node;
    } else {
        node->next = *first;
        *first = node;
    }
}

Node *deleteFirst(NodeLinkedList **first) {
    if (*first == NULL)
        return NULL;

    NodeLinkedList *node = *first;
    *first = (*first)->next;

    Node *key = node->key;
    delete node;
    return key;
}

void deleteLinkedList(LinkedList &L) {
    while (L.first != NULL) {
        deleteFirst(&L.first);
    }
}

void displayLinkedList(LinkedList L) {
    if (L.first == NULL)
        cout << "NULL";

    for (NodeLinkedList *i = L.first; i != NULL; i = i->next) {
        cout << i->key->key << " ";
    }
    cout << "\n";
}

bool dfsVisit(Graph G, Node *u, int &time, bool topoSort = false, NodeLinkedList **first = NULL, Operation *op = NULL) {

    if (op != NULL) op->count(3);
    time++;
    u->discovered = time;
    u->color = GRAY;

    bool acyclic = true;

    for (int i = 0; i < u->adjSize; ++i) {

        if (op != NULL) op->count();
        if (u->adj[i]->color == WHITE) {

            if (op != NULL) op->count();
            u->adj[i]->parent = u;

            bool result = dfsVisit(G, u->adj[i], time, topoSort, first, op);
            acyclic = acyclic == false ? false : result;

        } else if (u->adj[i]->color == GRAY and u != u->adj[i]) {
            acyclic = false;
        }
    }

    if (op != NULL) op->count(3);
    time++;
    u->finished = time;
    u->color = BLACK;

    if (topoSort == true) {
        insertFirst(first, u);
    }

    return acyclic;
}

bool dfs(Graph G, bool topoSort = false, NodeLinkedList **first = NULL, Operation *op = NULL) {
    for (int i = 0; i < G.nVertices; ++i) {
        G.verices[i]->color = WHITE;
        G.verices[i]->parent = NULL;
        G.verices[i]->discovered = 0;
        G.verices[i]->finished = 0;
    }

    int time = 0;
    bool acyclic = true;

    for (int i = 0; i < G.nVertices; ++i) {
        if (G.verices[i]->color == WHITE) {
            bool result = dfsVisit(G, G.verices[i], time, topoSort, first, op);
            acyclic = acyclic == false ? false : result;
        }
    }

    return acyclic;
}

LinkedList topologicalSort(Graph G) {
    LinkedList L = {NULL};
    bool acyclic = dfs(G, true, &L.first);
    if (acyclic == false)
        deleteLinkedList(L);
    return L;
}

void strongConnected(Graph G, Node *u, int &index, LinkedList &L, int &nComponents) {
    u->index = index;
    u->lowLink = index;
    index++;
    insertFirst(&L.first, u);
    u->onStack = true;

    for (int i = 0; i < u->adjSize; ++i) {
        if (u->adj[i]->index == -1) {
            strongConnected(G, u->adj[i], index, L, nComponents);
            u->lowLink = min(u->lowLink, u->adj[i]->lowLink);
        } else if (u->adj[i]->onStack == true) {
            u->lowLink = min(u->lowLink, u->adj[i]->index);
        }
    }

    Node *v;
    if (u->lowLink == u->index) {
        nComponents++;
        do {
            v = deleteFirst(&L.first);
            v->onStack = false;
            v->comp = nComponents;
        } while (v != u);
    }
}

int tarjan(Graph G) {
    int index = 0;
    LinkedList L = {NULL};
    int nComponents = 0;

    for (int i = 0; i < G.nVertices; ++i) {
        G.verices[i]->index = -1;
        G.verices[i]->lowLink = -1;
        G.verices[i]->onStack = false;
        G.verices[i]->comp = 0;
    }

    for (int i = 0; i < G.nVertices; ++i) {
        if (G.verices[i]->index == -1) {
            strongConnected(G, G.verices[i], index, L, nComponents);
        }
    }

    return nComponents;
}

Node *create(int key) {
    Node *node = new Node;

    node->key = key;
    node->adjSize = 0;
    node->adj = NULL;
    node->parent = NULL;
    node->color = WHITE;
    node->discovered = 0;
    node->finished = 0;

    return node;
}

Graph readFromFile(string file) {
    ifstream fin(file);
    Graph G = {0};

    fin >> G.nVertices;
    G.verices = new Node *[G.nVertices];
    for (int i = 0; i < G.nVertices; ++i) {
        G.verices[i] = create(i);
    }

    int nEdges = 0;
    Edge *edges = new Edge[MAX_EDGES];
    while (fin >> edges[nEdges].u >> edges[nEdges].v) {
        G.verices[edges[nEdges].u]->adjSize++;
        nEdges++;
    }

    for (int i = 0; i < G.nVertices; ++i) {
        G.verices[i]->adj = new Node *[G.verices[i]->adjSize];
        G.verices[i]->adjSize = 0;
    }

    for (int i = 0; i < nEdges; ++i) {
        G.verices[edges[i].u]->adj[G.verices[edges[i].u]->adjSize++] = G.verices[edges[i].v];
    }

    delete[] edges;
    fin.close();
    return G;
}

void displayGraph(Graph G) {
    for (int i = 0; i < G.nVertices; ++i) {
        cout << G.verices[i]->key << ": ";
        for (int j = 0; j < G.verices[i]->adjSize; ++j) {
            cout << G.verices[i]->adj[j]->key << " ";
        }
        cout << "\n";
    }
}

void prettyPrintTree(int n, int parent, int *p, int level = 0) {
    for (int i = 0; i < level; i++) {
        printf("        ");
    }
    cout << parent << "\n";
    for (int i = 0; i < n; i++) {
        if (p[i] == parent)
            prettyPrintTree(n, i, p, level + 1);
    }
}

void prettyPrintDfsTree(Graph G) {
    int *p = new int[G.nVertices]{0};

    for (int i = 0; i < G.nVertices; ++i) {
        if (G.verices[i]->parent == NULL) {
            p[i] = -1;
        } else {
            p[i] = G.verices[i]->parent->key;
        }
    }

    for (int i = 0; i < G.nVertices; ++i) {
        if (p[i] == -1) {
            prettyPrintTree(G.nVertices, i, p);
        }
    }

    delete[] p;
}

Graph generate(int nVertices, int nEdges) {
    bool **adMatrix = new bool *[nVertices];
    for (int i = 0; i < nVertices; i++) {
        adMatrix[i] = new bool[nVertices]{0};
    }

    Graph G = {0};

    G.nVertices = nVertices;
    G.verices = new Node *[G.nVertices];
    for (int i = 0; i < G.nVertices; ++i) {
        G.verices[i] = create(i);
    }

    int cntEdges = 0;
    while (cntEdges < nEdges) {
        int u = rand() % G.nVertices;
        int v = rand() % G.nVertices;

        if (u != v and adMatrix[u][v] == 0) {
            adMatrix[u][v] = 1;
            G.verices[u]->adjSize++;
            cntEdges++;
        }
    }

    for (int i = 0; i < nVertices; ++i) {
        if (G.verices[i]->adjSize > 0) {
            G.verices[i]->adj = new Node *[G.verices[i]->adjSize]{NULL};
            G.verices[i]->adjSize = 0;

            for (int j = 0; j < nVertices; ++j) {
                if (adMatrix[i][j] == 1) {
                    G.verices[i]->adj[G.verices[i]->adjSize++] = G.verices[j];
                }
            }
        }
    }

    for (int i = 0; i < nVertices; i++) {
        delete adMatrix[i];
    }
    delete[] adMatrix;

    return G;
}

void deleteGraph(Graph &G) {
    for (int i = 0; i < G.nVertices; ++i) {
        delete[] G.verices[i]->adj;
    }

    for (int i = 0; i < G.nVertices; ++i) {
        delete G.verices[i];
    }

    delete[] G.verices;
}

void displayComponents(Graph G, int nComponents) {
    cout << "\nSunt " << nComponents << " componente\n";
    for (int i = 1; i <= nComponents; ++i) {
        cout << "Componenta " << i << ":\n";
        for (int j = 0; j < G.nVertices; ++j) {
            if(i == G.verices[j]->comp) {
                cout << G.verices[j]->key << " ";
            }
        }
        cout << "\n";
    }
}

void demo() {
    Graph G1 = readFromFile("graph.txt");
    cout << "\nPrimul graf:\n";
    displayGraph(G1);

    cout << "Graful e aciclic: " << dfs(G1) << "\n";
    prettyPrintDfsTree(G1);

    LinkedList L = topologicalSort(G1);
    cout << "\nSortarea topoligica:\n";
    displayLinkedList(L);
    deleteLinkedList(L);

    Graph G2 = readFromFile("graph_test_comp_tari_conexe.txt");
    cout << "\nAl doilea graf:\n";
    displayGraph(G2);

    int nComponents = tarjan(G2);
    displayComponents(G2, nComponents);
}

void perf() {
    for (int i = 1000; i <= MAX_EDGES; i += STEP_EDGES) {
        Operation op = profiler.createOperation("dfs-edges", i);

        Graph G = {0};
        G.nVertices = 100;
        G = generate(G.nVertices, i);
        dfs(G, false, NULL, &op);
        deleteGraph(G);
    }

    for (int i = 100; i < MAX_VERTICES; i += STEP_VERTICES) {
        Operation op = profiler.createOperation("dfs-vertices", i);

        Graph G = {0};
        G.nVertices = i;
        G = generate(G.nVertices, MAX_EDGES);
        dfs(G, false, NULL, &op);
        deleteGraph(G);
    }

    profiler.showReport();
}

int main() {
    demo();
//    perf();
    return 0;
}
