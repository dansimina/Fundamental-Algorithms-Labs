/*
 * Simina Dan-Marius
 * Grupa 2
 *
 * Pentru determinarea vecinilor unei celule ma uit pe pozitiile de jos, stanga, sus, dreapta si verific daca sunt libere si daca
 * se afla inauntrul matricii, iar daca da le adaug in lista.
 * Complexitate O(1).
 *
 * Pentru BFS mai intai initializez fiecare nod cu culoarea alb, distanta infinit si parintele NULL, iar apoi introduc nodul din care
 * pornesc in coada. Apoi iau pe rand fiecare nod din coada si pentru toti vecinii cu culoarea alba ii introduc in coada, le pun culoarea grii
 * distanta este egala cu distanta nodului curent plus 1, iar parintele e nodul curent. Dupa ce termin cu toti vecinii nodului curent
 * ii schimb culoarea in negru.
 * Complexitate O(V + E).
 *
 * Pentru afisarea arborelui BFS afisez nodul curent si apelez recursiv functia pentru toti copii lui.
 * Complexitate O(n^2).
 *
 * Pentru determinarea celui mai scurt drum aplic BFS, iar apoi reconstruiesc drumul mergand pe parinte de la nodul destinatie pana la
 * nodul din care am pornit.
 * Complexitate O(V + E).
 *
 */
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "bfs.h"

using namespace std;

int get_neighbors(const Grid *grid, Point p, Point neighb[], bool knight) {
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    //knight problem
    const int dRowKnight[] = {1, -1, -2, -2, -1, 1, 2, 2};
    const int dColKnight[] = {-2, -2, -1, 1, 2, 2, 1, -1};
    const int nKnight = sizeof(dRowKnight) / sizeof(dRowKnight[0]);
    //normal
    const int dRowNormal[] = {-1, 0, 1, 0};
    const int dColNormal[] = {0, -1, 0, 1};
    const int nNormal = sizeof(dRowNormal) / sizeof(dRowNormal[0]);

    int dRow[8] = {0};
    int dCol[8] = {0};
    int n = 0;

    if(knight == true) {
        n = nKnight;
        copy(dRowKnight, dRowKnight + n, dRow);
        copy(dColKnight, dColKnight + n, dCol);
    } else {
        n = nNormal;
        copy(dRowNormal, dRowNormal + n, dRow);
        copy(dColNormal, dColNormal + n, dCol);
    }

    int cntNeighb = 0;

    for (int i = 0; i < n; i++) {
        if (p.row + dRow[i] >= 0 and p.col + dCol[i] >= 0 and p.row + dRow[i] < grid->rows and
            p.col + dCol[i] < grid->cols and grid->mat[p.row + dRow[i]][p.col + dCol[i]] == 0) {
            neighb[cntNeighb++] = Point(p.row + dRow[i], p.col + dCol[i]);
        }
    }

    return cntNeighb;
}

void grid_to_graph(const Grid *grid, Graph *graph, bool knight) {
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[8]; //i made it 8 for the horse problem

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node *) malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            } else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node **) malloc(graph->nrNodes * sizeof(Node *));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb, knight);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node **) malloc(graph->v[i]->adjSize * sizeof(Node *));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node **) realloc(graph->v[i]->adj, k * sizeof(Node *));
            }
        }
    }
}

void free_graph(Graph *graph) {
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

struct LinkedList {
    Node *key;
    LinkedList *next;
};

struct Queue {
    LinkedList *first = NULL;
    LinkedList *last = NULL;
};

LinkedList *create(Node *key) {
    LinkedList *p = new LinkedList;
    p->key = key;
    p->next = NULL;

    return p;
}

void enqueue(Queue &Q, Node *node) {
    LinkedList *p = create(node);
    if (Q.last == NULL) {
        Q.first = p;
        Q.last = p;
    } else {
        Q.last->next = p;
        Q.last = Q.last->next;
    }
}

Node *dequeue(Queue &Q) {
    if (Q.first == NULL)
        return NULL;

    LinkedList *node = Q.first;
    if (Q.first == Q.last) {
        Q.first = NULL;
        Q.last = NULL;
    } else {
        Q.first = Q.first->next;
    }

    Node *key = node->key;
    delete node;

    return key;
}

void bfs(Graph *graph, Node *s, Operation *op) {
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    for (int i = 0; i < graph->nrNodes; i++) {

        if (op != NULL) op->count(3);

        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INT_MAX;
        graph->v[i]->parent = NULL;
    }

    if (op != NULL) op->count(3);

    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;

    Queue Q;
    enqueue(Q, s);

    while (Q.first != NULL) {

        if (op != NULL) op->count();

        Node *u = dequeue(Q);
        for (int i = 0; i < u->adjSize; i++) {

            if (op != NULL) op->count();

            if (u->adj[i]->color == COLOR_WHITE) {

                if (op != NULL) op->count(3);

                u->adj[i]->color = COLOR_GRAY;
                u->adj[i]->dist = u->dist + 1;
                u->adj[i]->parent = u;
                enqueue(Q, u->adj[i]);
            }
        }
        if (op != NULL) op->count();
        u->color = COLOR_BLACK;
    }

}

void pretty_print_tree(int n, int parent, int *p, Point *repr, int level = 0) {
    for (int i = 0; i < level; i++) {
        printf("        ");
    }
    printf("(%d, %d)\n", repr[parent].row, repr[parent].col);
    for (int i = 0; i < n; i++) {
        if (p[i] == parent)
            pretty_print_tree(n, i, p, repr, level + 1);
    }
}

void print_bfs_tree(Graph *graph) {
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int *) malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        } else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int *) malloc(n * sizeof(int));
    repr = (Point *) malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            } else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                } else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        for (int i = 0; i < n; i++)
            if (p[i] == -1) {
                pretty_print_tree(n, i, p, repr);
                break;
            }
    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[]) {
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

    bfs(graph, start, NULL);
    if (end->color == COLOR_WHITE)
        return -1;

    int n = end->dist;
    Node *p = end;
    for (int i = n - 1; i >= 0; i--) {
        path[i] = p;
        p = p->parent;
    }
    return n;
}

struct NodeSet {
    int rank;
    NodeSet *parent;
};

NodeSet *makeSet() {
    NodeSet *x = new NodeSet;
    x->parent = x;
    x->rank = 0;

    return x;
}

NodeSet *findSet(NodeSet *x) {
    if (x != x->parent) {
        x->parent = findSet(x->parent);
    }
    return x->parent;
}

void unify(NodeSet *x, NodeSet *y) {
    if (x->rank > y->rank) {
        y->parent = x;
    } else {
        x->parent = y;
    }

    if (x->rank == y->rank) {
        (y->rank)++;
    }
}

void reunion(NodeSet *x, NodeSet *y) {
    unify(findSet(x), findSet(y));
}

void generate(Graph &graph, int nrEdges) {
    int **adMatrix = new int *[graph.nrNodes];
    for (int i = 0; i < graph.nrNodes; i++) {
        adMatrix[i] = new int[graph.nrNodes]{0};
    }

    int nComponents = graph.nrNodes;

    NodeSet **a = new NodeSet *[graph.nrNodes]{NULL};

    for (int i = 0; i < graph.nrNodes; i++) {
        a[i] = makeSet();
    }


    nrEdges = nrEdges < graph.nrNodes * (graph.nrNodes - 1) / 2 ? nrEdges : graph.nrNodes * (graph.nrNodes - 1) / 2;

    int cntEdges = 0;
    while (cntEdges < nrEdges - nComponents + 1) {
        int u = rand() % graph.nrNodes;
        int v = rand() % graph.nrNodes;

        if (u != v and adMatrix[u][v] == 0 and adMatrix[v][u] == 0) {
            adMatrix[u][v] = 1;
            adMatrix[v][u] = 1;
            graph.v[u]->adjSize++;
            graph.v[v]->adjSize++;
            cntEdges++;

            if (findSet(a[u]) != findSet(a[v])) {
                reunion(a[u], a[v]);
                nComponents--;
            }
        }
    }

    for (int i = 1; i < graph.nrNodes && nComponents > 1; i++) {
        if (findSet(a[0]) != findSet(a[i])) {
            adMatrix[0][i] = 1;
            adMatrix[i][0] = 1;
            graph.v[0]->adjSize++;
            graph.v[i]->adjSize++;

            reunion(a[0], a[i]);
            nComponents--;
        }
    }

    for (int i = 0; i < graph.nrNodes; i++) {
        if (graph.v[i]->adjSize > 0) {
            graph.v[i]->adj = new Node *[graph.v[i]->adjSize]{NULL};
            graph.v[i]->adjSize = 0;

            for (int j = 0; j < graph.nrNodes; j++) {
                if (adMatrix[i][j] == 1) {
                    graph.v[i]->adj[graph.v[i]->adjSize] = graph.v[j];
                    graph.v[i]->adjSize++;
                }
            }
        }
    }

    for (int i = 0; i < graph.nrNodes; i++) {
        delete adMatrix[i];
    }
    delete[] adMatrix;
}

void performance() {
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        generate(graph, n);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        generate(graph, 4500);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
