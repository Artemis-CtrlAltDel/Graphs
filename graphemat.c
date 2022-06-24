#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphemat.h"
#include "liste.h"

typedef struct
{
    int valeur;
} Indice;

typedef struct 
{
    NomSom nom;
    int cout;
    int indice;
    char chemin_optimal[100];
} Sommet;




booleen  grapheDetruit = vrai;

// remise � z�ro du tableau de marquage
static void razMarque (GrapheMat* graphe)
{

    for (int i=0; i<graphe->n; i++) graphe->marque [i] = faux;
}

GrapheMat* creerGrapheMat (int nMax, int value)
{
    // allocation de graphe
    GrapheMat* graphe  = (GrapheMat*) malloc (sizeof (GrapheMat));
    graphe->n       = 0;
    graphe->nMax    = nMax;
    graphe->value   = value; // value : vrai si le graphe est valu�
    graphe->nomS    = (NomSom*)  malloc (sizeof(NomSom)  *nMax);
    graphe->marque  = (booleen*) malloc (sizeof(booleen) *nMax);
    graphe->element = (int*)     malloc (sizeof(int)*nMax*nMax);
    graphe->valeur  = (int*)     malloc (sizeof(int)*nMax*nMax);
    // initialisation par d�faut
    for (int i=0; i<nMax; i++)
    {
        for (int j=0; j<nMax; j++)
        {
            graphe->element [i*nMax+j] = faux;
            graphe->valeur  [i*nMax+j] = INFINI;
        }
    }
    razMarque (graphe);
    grapheDetruit = faux;
    return graphe;
}


// d�sallocation d'un graphe
void detruireGraphe (GrapheMat* graphe)
{
    free (graphe->nomS);
    free (graphe->marque);
    free (graphe->element);
    free (graphe->valeur);
    free (graphe);
    grapheDetruit = vrai;
}

static int rang (GrapheMat* graphe, NomSom nom)
{
    int      i      = 0;
    booleen  trouve = faux;
    while ( (i<graphe->n) && !trouve)
    {
        trouve = strcmp (graphe->nomS [i], nom) == 0;
        if (!trouve) i++;
    }
    return trouve ? i : -1;
}

void ajouterUnSommet (GrapheMat* graphe, NomSom nom)
{
    if (rang (graphe, nom) == -1)
    {
        if (graphe->n < graphe->nMax)
        {
            strcpy (graphe->nomS [graphe->n++], nom);
        }
        else
        {
            printf ("\nNombre de sommets > %d\n", graphe->nMax);
        }
    }
    else
    {
        printf ("\n%s d�j� d�fini\n", nom);
    }
}

void ajouterUnArc (GrapheMat* graphe, NomSom somD, NomSom somA, int cout)
{
    int nMax = graphe->nMax;
    int rd = rang (graphe, somD);
    int rg = rang (graphe, somA);
    graphe->element [rd*nMax+rg] = vrai;
    graphe->valeur  [rd*nMax+rg] = cout;
}

void ecrireGraphe (GrapheMat* graphe)
{
    if (grapheDetruit == vrai)
    {
        printf ("Le graphe a ete detruit ou n'est pas encore cree\n");
    }
    else
    {
        int nMax = graphe->nMax;
        for (int i=0; i<graphe->n; i++) printf ("%s ", graphe->nomS[i]);
        printf (";\n");
        for (int i=0; i<graphe->n; i++)
        {
            printf ("\n%s : ", graphe->nomS[i]);
            for (int j=0; j<graphe->n; j++)
            {
                if (graphe->element [i*nMax+j] == vrai)
                {
                    printf ("%s ", graphe->nomS[j]) ;
                    if (graphe->value)
                    {
                        printf (" (%3d) ", graphe->valeur [i*nMax+j] );
                    }
                }
            }
            printf (";");
        }
    }
}

static void profondeur (GrapheMat* graphe, int numSommet)
{
    int nMax = graphe->nMax;
    graphe->marque [numSommet] = vrai;
    printf ("%s\n", graphe->nomS [numSommet]);
    for (int i=0; i<graphe->n; i++)
    {
        if ( (graphe->element [numSommet*nMax+i] == vrai)
                && !graphe->marque [i] )
        {
            profondeur (graphe, i);
        }
    }
}


void parcoursProfond (GrapheMat* graphe)
{
    razMarque (graphe);
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i]) profondeur (graphe, i);
    }
}

static void ecrireEtape (Matrice a, Matrice p, int k, int ns, int nMax)
{
    printf ("Passage par le sommet num�ro %d\n", k);

    for (int i=0; i<ns; i++)
    {
        for (int j=0; j<ns; j++)
        {
            if (a [i*nMax+j]==INFINI)
            {
                printf (" %3s", "*");

            }
            else
            {
                printf (" %3d", a [i*nMax+j]);
            }
        }
        printf ("%6s", " ");
        for (int j=0; j<ns; j++)
        {
            printf ("%3d", p [i*nMax+j]);
        }
        printf ("\n");
    }
    printf ("\n");
}

// initialiser les matrices a et p � partir de graphe
static void initFloyd (GrapheMat* graphe, Matrice* a, Matrice* p)
{
    int nMax = graphe->nMax;

    Matrice ta = (int*) malloc (sizeof(int)*nMax*nMax);
    Matrice tp = (int*) malloc (sizeof(int)*nMax*nMax);


    for (int i=0; i<graphe->n; i++)
    {
        for (int j=0; j<graphe->n; j++)
        {
            if(i==j){
                ta [i*nMax+j]=0;
                tp [i*nMax+j] = i;
            }
            else{
                ta [i*nMax+j] = graphe->valeur [i*nMax+j];
                tp [i*nMax+j] = i;
            }
        }
    }
    *a = ta;
    *p = tp;
}

void floyd (GrapheMat* graphe)
{
    Matrice a, p;
    int n = graphe->n;
    int nMax = graphe->nMax;

    initFloyd (graphe, &a, &p);


    for (int k=0; k<n; k++)
    {
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                if ( (a [i*nMax+k] != INFINI) &&
                        (a [k*nMax+j] != INFINI) &&
                        (a [i*nMax+k] + a [k*nMax+j] < a [i*nMax+j]) )
                {
                    a [i*nMax+j] = a [i*nMax+k] + a [k*nMax+j];
                    p [i*nMax+j] = p [k*nMax+j];
                }
            }
        }
        ecrireEtape (a, p, k, n, nMax);
    }
}


// Explorations non-informées :

void exploration_largeur(GrapheMat* graphe, NomSom somBut) {
    razMarque(graphe);
    Liste* li = creerListe();

    int nMax = graphe->nMax;
    int trouver = faux;

    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {
            Indice* indice = malloc(sizeof(Indice));
            indice->valeur = i;
            insererEnFinDeListe(li, indice);
        }
        while (!listeVide(li) && !trouver)
        {
            Indice* indice = extraireEnTeteDeListe(li);
            int extrait = indice->valeur;
            graphe->marque[extrait] = vrai;
            if (strcmp(somBut, graphe->nomS[extrait]) == 0)
            {
                printf(" %s ", somBut);
                trouver = vrai;
                return;
            }
            printf(" %s ->", graphe->nomS[extrait]);
            
            for (int j = 0; j < graphe->n; j++)
            {
                if (graphe->element[extrait * nMax + j] && !graphe->marque[j])
                {
                    Indice* indice_j = malloc(sizeof(Indice));
                    indice_j->valeur = j;
                    insererEnFinDeListe(li, indice_j);
                    graphe->marque[j] = vrai;
                }
                
            }
            
        }

    }

    if (!trouver)
    {
        printf("\nLe noeud est introuvable.\n");
    }
    
}

static int _profondeur(GrapheMat* graphe, int indice, NomSom somBut){
    if (strcmp(somBut, graphe->nomS[indice])== 0)
    {
        printf(" %s ", somBut);
        return indice;
    }
    printf(" %s ->", graphe->nomS[indice]);
    graphe->marque[indice] = vrai;
    
    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i] && graphe->element[indice * graphe->nMax + i])
        {
            if (strcmp(somBut, graphe->nomS[i]) == 0)
            {
                int _indice = _profondeur(graphe, i, somBut);
                if (_indice != -1)
                    return _indice;
            }
                                
        }

    }
    return -1;
}
void exploration_profondeur(GrapheMat* graphe, NomSom somBut) {
    razMarque(graphe);
    int indice = -1;

    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i])
        {
            indice = _profondeur(graphe, i, somBut);
            if (indice != -1)
                break;

        }
        
    }

    if (indice == -1)
        printf("\nLe noeud est introuvable\n");
    
}

static int _profondeur_limite(GrapheMat* graphe, NomSom somBut, int extrait, int limite){
    if (limite < 0) return -1;

    if (strcmp(somBut, graphe->nomS[extrait]) == 0)
    {
        printf(" %s ", somBut);
        return extrait;
    }
    printf(" %s ->", graphe->nomS[extrait]);
    graphe->marque[extrait] = vrai;
    
    for (int i = 0; i < graphe->n; i++)
    {
        if (!graphe->marque[i] && graphe->element[extrait * graphe->nMax + i])
        {
                int _extrait = _profondeur_limite(graphe, somBut, i, limite);
                if (_extrait != -1)
                    return _extrait;
                                
        }

    }

    return -1;

}
void exploration_profondeur_limite(GrapheMat* graphe, NomSom somBut, int limite) {
    razMarque(graphe);
    int indice = _profondeur_limite(graphe, somBut, 0, limite);

    if (indice == -1)
        printf("\nLe noeud est introuvable\n");
}

void exploration_profondeur_iterative(GrapheMat* graphe, NomSom somBut) {
    razMarque(graphe);

    int indice = -1;
    
    for (int limite = 0; limite < graphe->n; limite++)
    {
        indice = _profondeur_limite(graphe, somBut, 0, limite);
        if (indice != -1)
            return;
        
    }
    

    if (indice == -1)
        printf("\nLe noeud est introuvable\n");
}

int comparer_cout(Objet* a, Objet* b) {
    return ((Sommet*) a)->cout - ((Sommet*) b)->cout;
}

// Explorations informées : (heuristics - A*)
void exploration_a(GrapheMat* graphe, NomSom somBut, int* heuristic) {
    razMarque(graphe);
    Liste* li = creerListe();
    li->comparer = comparer_cout;
    Sommet* som = malloc(sizeof(Sommet));
    strcpy(som->nom, graphe->nomS[0]);
    som->cout = 0 + heuristic[0];
    som->indice = 0;
    strcpy(som->chemin_optimal, graphe->nomS[0]);
    insererEnFinDeListe(li, som);
    while (!listeVide(li))
    {
        Sommet* _som = extraireEnTeteDeListe(li);
        if (strcmp(_som->nom, somBut))
        {
            printf(" %s ", somBut);
            return;
        }
        printf(" %s ->", _som->nom);
        graphe->marque[_som->indice] = vrai;
        for (int i = 0; i < graphe->n; i++)
        {
            if (!graphe->marque[i] && graphe->element[_som->indice * graphe->nMax + i])
            {
                Sommet* __som = malloc(sizeof(Sommet));
                strcpy(__som->nom, graphe->nomS[i]);
                strcpy(__som->chemin_optimal, _som->chemin_optimal);
                strcat(__som->chemin_optimal, " ->");
                strcat(__som->chemin_optimal, graphe->nomS[i]);
                __som->indice = i;
                __som->cout = graphe->valeur[_som->indice*graphe->nMax+i] + _som->cout + heuristic[_som->indice];
                graphe->marque[i] = vrai;
                insererEnOrdre(li, __som);
            }
            
        }
        
    }
    
}
