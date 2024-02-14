/*
 *  Simina Dan-Marius
 *  Grupa 2
 *
 *  In acest lab am implementat operatia de inserare, cautare si stergere pentru tabele de dispersie cu verificare patratica care e de
 *  forma h(k) = (hp(k) + c1 * i + c2 * i^2) mod m, unde hp = k mod m.
 *
 *  La inserare incerc sa pun elementul pe pozitia din tabela care e data de functia de dispersie, daca pozitia e ocupata cresc variabila i
 *  si repet acest lucru pana cand gasesc o pozitie care nu e ocupata caz in care pun elementul pe acea pozitie, sau pana cand am verificat
 *  toate pozitiile pe care poate fi pus, in acest caz semnalez overflow.
 *  Complexitate: caz mediu O(1), caz defavorabil O(n).
 *
 *  La cautare verific daca elementul e pe pozitia data de functia de dispersie, iar daca da returnez pozitia daca nu cresc variabila i
 *  si continui cautarea pana cand il gasesc, sau verific toate locurile posibile sau dau de o pozitie neocupata caz in care semnalez
 *  ca elementul cautat nu se afla in tabela.
 *  Complexitate: caz mediu O(1), caz defavorabil O(n).
 *
 *  La stergere trimit pozitia elementului pe care doresc sa il sterg care e determinata de functia de dispersie si il semnez ca sters
 *  Complexitate: caz mediu O(1), caz defavorabil O(n).
 *
 *  Din tabelul generat de functia de performance se observa ca in cazul functiei de cautare, cu cat factorul de umplere e mai mic cu
 *  atat numarul de accese e mai mic atat pentru elementele gasite cat si pentru cele pe care nu reuseste sa le gaseasca.
 *  Totodata numarul de accese e mai mare in cazul cautarii elementelor negasite decat in cazul celor gasite.
 *
 *  Iar pentru tabelul din care au fost sterse elemente pana cand factorul de umplere a ajuns de la 0.99 la 0.8 efortul pentru cautare
 *  ramane foarte mare, valorile sun foarte asemanatoare cu cele pentru factorul de umplere 0.99, car daca tabela a ajuns la factorul
 *  de umplere 0.8, deoarece tot sunt parcurse elementele setate ca sterse.
 */
#include <iostream>
#include <fstream>
#include "Profiler.h"

using namespace std;

Profiler p("Hash table");

#define N 10007
#define mElemente 3000
#define nrTests 5

#define c1 1
#define c2 1

long long int efortMediuGasite, efortMaximGasite, efortMediuNegasite, efortMaximNegasite;
long long int nrOperatii, nrElementeGasite, nrElementeNegasite;

struct Entry {
    int id;
    char name[30];
};

struct HashTable {
    Entry *T[N];
    Entry *del = new Entry(-1, "DEL");
    int m;
};

int hp(HashTable T, int k) {
    return k % T.m;
}

int h(HashTable T, int k, int i) {
    return (hp(T, k) + c1 * i + c2 * i * i) % T.m;
}

Entry *create(Entry key) {
    Entry *p = new Entry;
    p->id = key.id;
    strcpy(p->name, key.name);

    return p;
}

int hashInsert(HashTable &T, Entry k) {
    int i = 0, q = 0;
    do {
        q = h(T, k.id, i);

        if (T.T[q] == NULL || T.T[q] == T.del) {
            T.T[q] = create(k);
            return q;
        } else i++;

    } while (i != T.m);

    return -1; //overflow
}

int hashSearch(HashTable T, Entry k) {
    int i = 0, q = 0;
    do {
        nrOperatii++;

        q = h(T, k.id, i);

        if (T.T[q] != NULL and T.T[q] != T.del and T.T[q]->id == k.id) {
            return q;
        }
        i++;
    } while (T.T[q] != NULL and i != T.m);

    return -1;
}

void hashDelete(HashTable &T, int q) {
    if (q != -1) {
        delete T.T[q];
        T.T[q] = T.del;
    }
}

void afisareHashTable(HashTable T) {
    for (int i = 0; i < T.m; i++) {
        if (T.T[i] != NULL)
            cout << T.T[i]->id << " " << T.T[i]->name << " | ";
        else
            cout << "NULL" << " | ";
    }
    cout << "\n";
}

void inserareVector(HashTable &T, int vElementeHashTable[], int n) {
    for (int i = 0; i < n; i++) {
        Entry k;
        k.id = vElementeHashTable[i];
        strcpy(k.name, "HT");
        hashInsert(T, k);
    }
}

void demo() {
    HashTable T = {NULL};
    T.m = 8;

//    int v[] = {1, 3, 9, 17, 2};
//    int n = sizeof(v) / sizeof(v[0]);
//    inserareVector(T, v, n);
//
//    afisareHashTable(T);
//
//    cout << hashSearch(T, Entry(7, "HT")) << "\n";
//
//    hashDelete(T, hashSearch(T, Entry(3, "HT")));
//    afisareHashTable(T);
//    cout << hashSearch(T, Entry(3, "HT")) << "\n";
//
//    hashInsert(T, Entry(3, "HT"));
//    afisareHashTable(T);

    hashInsert(T, Entry(1, "Ionel"));
    hashInsert(T, Entry(2, "Gigel"));
    hashInsert(T, Entry(3, "Marcel"));
    hashInsert(T, Entry(9, "Ana"));
    hashInsert(T, Entry(17, "Seb"));
    cout << hashInsert(T, Entry(25, "Kimi")) << "\n";

    cout << "Tabela de dispersie:\n";
    afisareHashTable(T);

    cout << "Caut persoana cu id 2: " << hashSearch(T, Entry(2, "Gigel")) << "\n";
    cout << "Caut persoana cu id 6: " << hashSearch(T, Entry(6, "Max")) << "\n";

    cout << "Sterg persoana cu id 9: \n";
    hashDelete(T, hashSearch(T, Entry(9, "Ana")));
    afisareHashTable(T);

    cout << "Caut persoana cu id 9: " << hashSearch(T, Entry(9, "Ana")) << "\n";
    cout << "Caut persoana cu id 17: " << hashSearch(T, Entry(17, "Seb")) << "\n";

    cout << "Sterg persoana cu id 2: \n";
    hashDelete(T, hashSearch(T, Entry(2, "Gigel")));
    afisareHashTable(T);
    cout << "Caut persoana cu id 2: " << hashSearch(T, Entry(2, "Gigel")) << "\n";

    cout << "Inserez o noua persoana cu id 25: " << hashInsert(T, Entry(25, "Kimi")) << "\n";
    afisareHashTable(T);
    cout << "Caut persoana cu id 25: " << hashSearch(T, Entry(25, "Kimi")) << "\n";
    cout << "Caut persoana cu id 17: " << hashSearch(T, Entry(17, "Seb")) << "\n";

    cout << "Inserez o noua persoana cu id 10: " << hashInsert(T, Entry(10, "Lewis")) << "\n";
    cout << "Inserez o noua persoana cu id 18: " << hashInsert(T, Entry(18, "Charles")) << "\n";
    afisareHashTable(T);
    cout << "Caut persoana cu id 18: " << hashSearch(T, Entry(18, "Charles")) << "\n";

    cout << "Sterg persoana cu id 10: \n";
    hashDelete(T, hashSearch(T, Entry(10, "Lewis")));
    afisareHashTable(T);
    cout << "Caut persoana cu id 18: " << hashSearch(T, Entry(18, "Charles")) << "\n";

}

void reset() {
    efortMediuGasite = 0;
    efortMaximGasite = 0;
    efortMediuNegasite = 0;
    efortMaximNegasite = 0;
    nrOperatii = 0;
    nrElementeGasite = 0;
    nrElementeNegasite = 0;
}

int fSortare(int a, int b) {
    return a % N < b % N;
}

void perf() {
    ofstream fout("tabel.csv");

    double factorUmplere95 = 0.95;
    double vFactoriUmplere[] = {0.5, 0.8, 0.85, 0.9, 0.95, 0.99, 1};
    int nFactoriUmplere = sizeof(vFactoriUmplere) / sizeof(vFactoriUmplere[0]);
    double factorUmplereInainteStergere = 0.99;
    double factorUmplereDupaStergere = 0.8;

    int vElementeHashTable[N];
    int mElementeDeCautat[mElemente];

//    fout << "Factor de umplere, Efort mediu gasite, Efort maxim gasite, Efort mediu negasite, Efort maxim negasite \n";

//    Evaluarea operației de căutare pentru un singur factor de umplere 95%,
//    folosind o selecție non-uniformă a elementelor.

    fout
            << "\nEvaluarea operatiei de cautare pentru un singur factor de umplere 95%, \nfolosind o selectie non-uniforma a elementelor. \n";
    fout
            << "\nFactor de umplere, Efort mediu gasite, Efort maxim gasite, Efort mediu negasite, Efort maxim negasite \n";

    reset();
    for (int test = 0; test < nrTests; test++) {

        int n = (double) N * factorUmplere95;
        HashTable T = {NULL};
        T.m = N;

        int a[40000] = {0};

        for (int i = 0; i < n;) {
            int nrDeAdaugat = rand() % 40000;

            while (a[nrDeAdaugat] != 0)
                nrDeAdaugat = rand() % 40000;
            a[nrDeAdaugat] = 1;

            int rHashInsert = hashInsert(T, Entry(nrDeAdaugat, ""));

            if (rHashInsert != -1) {
                vElementeHashTable[i] = nrDeAdaugat;
                i++;
            }
        }

        FillRandomArray(mElementeDeCautat, mElemente, 40000, 50000, FALSE, UNSORTED);

        for (int i = 0; i < mElemente / 2; i++) {
            mElementeDeCautat[i] = vElementeHashTable[i];
        }

        for (int i = 0; i < mElemente; i++) {
            nrOperatii = 0;

            int rHeapSearch = hashSearch(T, Entry(mElementeDeCautat[i], "HT"));

            if (rHeapSearch == -1) {
                nrElementeNegasite++;
                efortMediuNegasite += nrOperatii;
                if (efortMaximNegasite < nrOperatii) {
                    efortMaximNegasite = nrOperatii;
                }
            } else {
                nrElementeGasite++;
                efortMediuGasite += nrOperatii;
                if (efortMaximGasite < nrOperatii) {
                    efortMaximGasite = nrOperatii;
                }
            }
        }

        //Verificare umplere corecta
        int cnt = 0;
        for (int i = 0; i < T.m; i++)
            if (T.T[i] == NULL) cnt++;
        if (cnt != N - n) {
            cout << "Greseala";
            exit(0);
        }
    }
    if (nrElementeGasite > 0)
        efortMediuGasite = efortMediuGasite / nrElementeGasite;
    if (nrElementeNegasite > 0)
        efortMediuNegasite = efortMediuNegasite / nrElementeNegasite;

    //Verificare gasire corecta
    if (nrElementeGasite / nrTests != mElemente / 2 || nrElementeNegasite / nrTests != mElemente / 2) {
        cout << "Greseala";
        exit(0);
    }
    fout << factorUmplere95 << "," << efortMediuGasite << "," << efortMaximGasite << ","
         << efortMediuNegasite << ","
         << efortMaximNegasite << "\n";

//    Completarea evaluării pentru toți factorii de umplere folosind selecție
//    uniformă a elementelor găsite.

    fout
            << "\nCompletarea evaluarii pentru toti factorii de umplere folosind selectie \n uniforma a elementelor gasite. \n";
    fout
            << "\nFactor de umplere, Efort mediu gasite, Efort maxim gasite, Efort mediu negasite, Efort maxim negasite \n";

    for (int iFactor = 0; iFactor < nFactoriUmplere; iFactor++) {
        reset();
        for (int test = 0; test < nrTests; test++) {

            int n = (double) N * vFactoriUmplere[iFactor];
            HashTable T = {NULL};
            T.m = N;

            int a[40000] = {0};

            for (int i = 0; i < n;) {
                int nrDeAdaugat = rand() % 40000;

                while (a[nrDeAdaugat] != 0)
                    nrDeAdaugat = rand() % 40000;
                a[nrDeAdaugat] = 1;

                int rHashInsert = hashInsert(T, Entry(nrDeAdaugat, ""));

                if (rHashInsert != -1) {
                    vElementeHashTable[i] = nrDeAdaugat;
                    i++;
                }
            }

            FillRandomArray(mElementeDeCautat, mElemente, 40000, 50000, FALSE, UNSORTED);

            stable_sort(vElementeHashTable, vElementeHashTable + n, fSortare);

            for (int i = 0; i < mElemente; i += 2) {
                mElementeDeCautat[i] = vElementeHashTable[(n / mElemente) * i];
            }

            for (int i = 0; i < mElemente; i++) {
                nrOperatii = 0;

                int rHeapSearch = hashSearch(T, Entry(mElementeDeCautat[i], "HT"));

                if (rHeapSearch == -1) {
                    nrElementeNegasite++;
                    efortMediuNegasite += nrOperatii;
                    if (efortMaximNegasite < nrOperatii) {
                        efortMaximNegasite = nrOperatii;
                    }
                } else {
                    nrElementeGasite++;
                    efortMediuGasite += nrOperatii;
                    if (efortMaximGasite < nrOperatii) {
                        efortMaximGasite = nrOperatii;
                    }
                }
            }

            //Verificare umplere corecta
            int cnt = 0;
            for (int i = 0; i < T.m; i++)
                if (T.T[i] == NULL) cnt++;
            if (cnt != N - n) {
                cout << "Greseala";
                exit(0);
            }
        }
        if (nrElementeGasite > 0)
            efortMediuGasite = efortMediuGasite / nrElementeGasite;
        if (nrElementeNegasite > 0)
            efortMediuNegasite = efortMediuNegasite / nrElementeNegasite;

        //Verificare gasire corecta
        if (nrElementeGasite / nrTests != mElemente / 2 || nrElementeNegasite / nrTests != mElemente / 2) {
            cout << "Greseala";
            exit(0);
        }

        fout << vFactoriUmplere[iFactor] << "," << efortMediuGasite << "," << efortMaximGasite << ","
             << efortMediuNegasite << ","
             << efortMaximNegasite << "\n";
    }

//    Evaluare operație căutare după ștergerea unor elemente.
    fout << "\nEvaluare operatie cautare dupa stergerea unor elemente.\n";
    fout
            << "\nFactor de umplere, Efort mediu gasite, Efort maxim gasite, Efort mediu negasite, Efort maxim negasite \n";

    reset();
    for (int test = 0; test < nrTests; test++) {

        int n = (double) N * factorUmplereInainteStergere;
        int nf = (double) N * factorUmplereDupaStergere;
        HashTable T = {NULL};
        T.m = N;

        int a[40000] = {0};

        for (int i = 0; i < n;) {
            int nrDeAdaugat = rand() % 40000;

            while (a[nrDeAdaugat] != 0)
                nrDeAdaugat = rand() % 40000;
            a[nrDeAdaugat] = 1;

            int rHashInsert = hashInsert(T, Entry(nrDeAdaugat, ""));

            if (rHashInsert != -1) {
                vElementeHashTable[i] = nrDeAdaugat;
                i++;
            }
        }

        FillRandomArray(mElementeDeCautat, mElemente, 40000, 50000, FALSE, UNSORTED);

        for (int i = n - 1; i >= nf; i--) {
            int elemDeSters = rand() % i;

            hashDelete(T, hashSearch(T, Entry(vElementeHashTable[elemDeSters], "")));
            vElementeHashTable[elemDeSters] = -1;
            swap(vElementeHashTable[elemDeSters], vElementeHashTable[i]);
        }

        stable_sort(vElementeHashTable, vElementeHashTable + nf, fSortare);

        for (int i = 0; i < mElemente; i += 2) {
            mElementeDeCautat[i] = vElementeHashTable[(nf / mElemente) * i];
        }

        for (int i = 0; i < mElemente; i++) {
            nrOperatii = 0;

            int rHeapSearch = hashSearch(T, Entry(mElementeDeCautat[i], "HT"));

            if (rHeapSearch == -1) {
                nrElementeNegasite++;
                efortMediuNegasite += nrOperatii;
                if (efortMaximNegasite < nrOperatii) {
                    efortMaximNegasite = nrOperatii;
                }
            } else {
                nrElementeGasite++;
                efortMediuGasite += nrOperatii;
                if (efortMaximGasite < nrOperatii) {
                    efortMaximGasite = nrOperatii;
                }
            }
        }

        //Verificare umplere corecta
        int cnt = 0;
        for (int i = 0; i < T.m; i++)
            if (T.T[i] == NULL || T.T[i] == T.del) cnt++;
        if (cnt != N - nf) {
            cout << "Greseala" << cnt << " " << N - nf;
            exit(0);
        }
    }
    if (nrElementeGasite > 0)
        efortMediuGasite = efortMediuGasite / nrElementeGasite;
    if (nrElementeNegasite > 0)
        efortMediuNegasite = efortMediuNegasite / nrElementeNegasite;

    //Verificare gasire corecta
    if (nrElementeGasite / nrTests != mElemente / 2 || nrElementeNegasite / nrTests != mElemente / 2) {
        cout << "Greseala ";
        exit(0);
    }

    fout << factorUmplereDupaStergere << "," << efortMediuGasite << "," << efortMaximGasite << ","
         << efortMediuNegasite << ","
         << efortMaximNegasite << "\n";

}

int main() {
    demo();
//    perf();
}
