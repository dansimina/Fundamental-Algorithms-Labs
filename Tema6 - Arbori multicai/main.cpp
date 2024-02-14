/*
 * Simina Dan-Marius
 * Grupa 2
 *
 * Pentru pretty-print la arborele reprezentat prin vector de tati parcurg vectorul si pentru fiecare fiu al nodului curent il afisez
 * si apelez pretty-print pentru el. Complexitate O(n^2).
 *
 * Pentru transformarea din arbore reprezentat prin vector de tati in arbore multicai pun mai intai toate nodurile intr-o lista, iar apoi
 * parcurg lista de noduri si in functie de vectorul de tati fac legaturile. Complexitate O(n).
 * Folosesc memorie aditionala la stocarea arborelui multicai, pe langa valoare mai tin minte numarul de copii, si legaturile cu copii;
 * Mai folosesc memorie aditionala la transformarea in arbore multicai folosind un vector de frecventa de dimensiune n pentru a numara
 * cati copii are fiecare nod ca sa pot aloca memoria exact.
 *
 * Pentru pretty-print la arborele multicai afisez nodul curent, iar apoi apelez recursiv pretty-print pentru toti copii lui. Complexitate O(n).
 *
 * Pentru transforamrea din arborele multicai in arbore binar in ramura din stanga a ficarui nod pun primul sau fiu, iar pe ramura din dreapta
 * a fiului sau pun restul de fii(fratii nodului), iar pentru fiecare fiu si apelez recursiv pentru fiecare fiu. Copmplexitate O(n).
 * Folosesc memorie aditionala la stocarea arborelui, pe langa valoare mai tin minte legatura cu primul copil si primul frate.
 *
 * Pentru pretty-print la arborele binar parcurg arborele in preordine si afisez. Complexitate O(n).
 */

#include <iostream>

using namespace std;

#define MAX_SIZE 10000


struct nodArboreMulticai {
    int val;
    int nrCopii;
    nodArboreMulticai** copii;
};

struct nodArboreBinar {
    int val;
    nodArboreBinar *primulCopil, *frate;
};

void prettyPrintR1(int R1[], int marimeArbore, int nod, int nivel) {
    for(int i = 0; i < marimeArbore; i++){
        if(R1[i] == nod) {
            for(int j = 0; j < nivel; j++)
                cout << "  ";
            cout << i + 1 << "\n";
            prettyPrintR1(R1, marimeArbore, i + 1, nivel + 1);
        }
    }
}

nodArboreMulticai* transformare1(int R1[], int marimeArbore) {
    nodArboreMulticai *radacina = NULL;
    nodArboreMulticai **listaNoduri = new nodArboreMulticai*[MAX_SIZE]{NULL};
    int *a = new int[MAX_SIZE]{0};

    for(int i = 0; i < marimeArbore; i++) {
        if(R1[i] - 1 >= 0){
            a[R1[i] - 1]++;
        }
    }

    for(int i = 0; i < marimeArbore; i++) {
        listaNoduri[i] = new nodArboreMulticai{NULL};
        listaNoduri[i]->val = i + 1;
        if(a[i] > 0) {
            listaNoduri[i]->copii = new nodArboreMulticai *[a[i]]{NULL};
            a[i] = 0;
        }
    }
    delete[] a;

    for(int i = 0; i < marimeArbore; i++) {
        if(R1[i] == -1){
            radacina = listaNoduri[i];
        }
        else{
            (listaNoduri[R1[i] - 1]->copii)[listaNoduri[R1[i] - 1]->nrCopii] = listaNoduri[i];
            (listaNoduri[R1[i] - 1]->nrCopii)++;
        }
    }

    return radacina;
}

void prettyPrintR2(nodArboreMulticai* radacina, int nivel) {
    if(radacina == NULL)
        return;

    for(int i = 0; i < nivel; i++)
        cout << "  ";
    cout << radacina->val << "\n";

    for(int i = 0; i < radacina->nrCopii; i++){
        prettyPrintR2(radacina->copii[i], nivel + 1);
    }
}

nodArboreBinar* creare(int val){
    nodArboreBinar* nod = new nodArboreBinar;
    nod->val = val;
    nod->primulCopil = NULL;
    nod->frate = NULL;

    return nod;
}

nodArboreBinar* transformare2(nodArboreMulticai* radacina) {
    if(radacina == NULL)
        return NULL;

    nodArboreBinar* nod = creare(radacina->val);
    if(radacina->nrCopii > 0) {
        nod->primulCopil = transformare2(radacina->copii[0]);

        nodArboreBinar* last = nod->primulCopil;
        for(int i = 1; i < radacina->nrCopii; i++) {
            last->frate = transformare2(radacina->copii[i]);
            last = last -> frate;
        }
    }

    return nod;
}

void prettyPrintR3(nodArboreBinar* radacina, int nivel) {
    if(radacina == NULL)
        return;

    for(int i = 0; i < nivel; i++)
        cout << "  ";
    cout << radacina->val << "\n";

    prettyPrintR3(radacina->primulCopil, nivel + 1);
    prettyPrintR3(radacina->frate, nivel);

}

void demo() {
    int R1[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
    int lungimeR1 = sizeof(R1) / sizeof(R1[0]);
    prettyPrintR1(R1, lungimeR1, -1, 0);

    nodArboreMulticai* R2 = transformare1(R1, lungimeR1);
    prettyPrintR2(R2, 0);

    nodArboreBinar* R3= transformare2(R2);
    prettyPrintR3(R3, 0);
}

int main() {
    demo();
    return 0;
}