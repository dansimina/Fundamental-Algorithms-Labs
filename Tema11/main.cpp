#include <iostream>
#include <fstream>
#include "Profiler.h"

using namespace std;

#define MAX_VERTICES 200
#define MAX_EDGES 4500

Profiler profile("dfs");

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

    if(*first == NULL) {
        *first = node;
    }
    else {
        node->next = *first;
        *first = node;
    }
}

void deleteFirst(NodeLinkedList **first) {
    if(*first == NULL)
        return;

    NodeLinkedList *node = *first;
    *first = (*first)->next;
    delete node;
}

void deleteLinkedList(LinkedList &L) {
    while(L.first != NULL) {
        deleteFirst(&L.first);
    }
}

void displayLinkedList(LinkedList L) {
    if(L.first == NULL)
        cout << "NULL";

    for (NodeLinkedList* i = L.first; i != NULL ; i = i->next) {
        cout << i->key->key << " ";
    }
    cout << "\n";
}

bool dfsVisit(Graph G, Node *u, int &time, bool topoSort = false, NodeLinkedList **first = NULL, Operation *op = NULL) {
    time++;
    u->discovered = time;
    u->color = GRAY;

    bool acyclic = true;

    for (int i = 0; i < u->adjSize; ++i) {
        if (u->adj[i]->color == WHITE) {
            u->adj[i]->parent = u;
            bool result = dfsVisit(G, u->adj[i], time, topoSort, first, op);
            acyclic = acyclic == false ? false : result;
        } else if (u->adj[i]->discovered > 0 and u->adj[i]->finished == 0) {
            acyclic = false;
        }
    }

    time++;
    u->finished = time;
    u->color = BLACK;

    if(topoSort == true) {
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
    cout << parent <<"\n";
    for (int i = 0; i < n; i++) {
        if (p[i] == parent)
            prettyPrintTree(n, i, p, level + 1);
    }
}

void prettyPrintDfsTree(Graph G) {
    int *p = new int[G.nVertices]{0};

    for (int i = 0; i < G.nVertices; ++i) {
        if(G.verices[i]->parent == NULL) {
            p[i] = -1;
        }
        else {
            p[i] = G.verices[i]->parent->key;
        }
    }

    for (int i = 0; i < G.nVertices; ++i) {
        if(p[i] == -1) {
            prettyPrintTree(G.nVertices, i, p);
        }
    }

    delete[] p;
}

void demo() {
    Graph G = readFromFile("graph.txt");
    displayGraph(G);

    cout << dfs(G) << "\n";
    prettyPrintDfsTree(G);

    LinkedList L = topologicalSort(G);
    displayLinkedList(L);
}

void perf() {
    
}

int main() {
    demo();
    return 0;
}
