#include <limits.h>
typedef int  booleen;
#define faux 0
#define vrai 1
typedef char NomSom[20];  // nom d'un sommet
#define INFINI INT_MAX

typedef int* Matrice;

typedef struct {
  int      n;       // nombre de sommets
  int      nMax;    // nombre max de sommets
  booleen  value;   // graphe valu� ou non
  NomSom*  nomS;    // noms des sommets
  Matrice  element; // existence d'un arc (i, j)
  Matrice  valeur;  // cout de l'arc (i, j)
  booleen* marque;  // sommet marqu� (visit�) ou non
} GrapheMat;

GrapheMat* creerGrapheMat     (int nMax, int value);
void       detruireGraphe     (GrapheMat* graphe);
void       ajouterUnSommet    (GrapheMat* graphe, NomSom nom);
void       ajouterUnArc       (GrapheMat* graphe, NomSom somD, NomSom somA, int cout);
void       ecrireGraphe       (GrapheMat* graphe);

void       parcoursProfond    (GrapheMat* graphe);

void       floyd              (GrapheMat* graphe);

// Explorations non-informées
void exploration_largeur(GrapheMat* graphe, NomSom somBut);

void exploration_profondeur(GrapheMat* graphe, NomSom somBut);
void exploration_profondeur_limite(GrapheMat* graphe, NomSom somBut, int limite);
void exploration_profondeur_iterative(GrapheMat* graphe, NomSom somBut);

// Explorations informées
void exploration_a(GrapheMat* graphe, NomSom somBut, int* heuristic);
