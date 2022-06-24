typedef void Objet;

// un élément de la liste
typedef struct element {
 Objet* reference;
 struct element* suivant;
} Element;
// le type Liste
typedef struct {
 Element* premier; // premier élément de la liste
 Element* dernier; // dernier élément de la liste
 Element* courant; // élément en cours de traitement (parcours de liste
 int nbElt;
 int type;
 char* (*afficher) (Objet*);
 int (*comparer) (Objet*, Objet*);
} Liste;

Liste* creerListe ();
int listeVide (Liste* li);
void insererEnTeteDeListe (Liste* li, Objet* objet);
void insererEnFinDeListe (Liste* li, Objet* objet);
Objet* extraireEnTeteDeListe (Liste* li);
void insererEnOrdre (Liste* li, Objet* objet);