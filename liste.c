#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "liste.h"


// initialiser la liste pointée par li (cas général)
void initListe (Liste* li) {
            li->premier = NULL;
            li->dernier = NULL; 
            li->courant = NULL; 
            li->nbElt = 0;
            li->type = 1;
}

static Element* creerElement () {
  return (Element*) malloc (sizeof (Element));
}

Liste* creerListe () {
  Liste* li = (Liste*) malloc (sizeof (Liste));
  initListe (li);
  return li;
}

int listeVide (Liste* li) {
  return li->nbElt == 0;
}

void insererEnTeteDeListe (Liste* li, Objet* objet) {
    Element* nouveau = creerElement () ;
    nouveau->reference = objet; 
    nouveau->suivant = li->premier;
    li->premier = nouveau;
    if (li->dernier == NULL) li->dernier = nouveau;
    li->nbElt++;
}

static void insererApres (Liste* li, Element* precedent, Objet* objet) {
 if (precedent == NULL) {
    insererEnTeteDeListe (li, objet);
   } else {
    Element* nouveau = creerElement ();
    nouveau->reference = objet;
    nouveau->suivant = precedent->suivant;
    precedent->suivant = nouveau;
    if (precedent == li->dernier) li->dernier = nouveau;
    li->nbElt++;
   }
}

void insererEnFinDeListe (Liste* li, Objet* objet) {
  insererApres (li, li->dernier, objet);
}

Objet* extraireEnTeteDeListe (Liste* li) {
    Element* extrait = li->premier;
    if (!listeVide (li)) {
        li->premier = li->premier->suivant;
        if (li->premier==NULL) li->dernier=NULL; // Liste devenue           
        li->nbElt--;
  }
  return extrait != NULL ? extrait->reference : NULL;

}

static int enOrdre (Objet* objet1, Objet* objet2, int ordreCroissant,
                 int (*comparer) (Objet*, Objet*)) {
  int ordre = comparer (objet1, objet2) < 0;
  if (!ordreCroissant) ordre = !ordre;
  return ordre;
}

void insererEnOrdre (Liste* li, Objet* objet) {
  if (listeVide (li) ) {   // liste vide
    insererEnTeteDeListe (li, objet);
  } else {
    Element* ptc = li->premier;
    if ( enOrdre (objet, ptc->reference, li->type==1, li->comparer) ) {
      // insertion avant le premier �l�ment
      insererEnTeteDeListe (li, objet);
    } else {
	  // insertion en milieu ou fin de liste
      int  trouve = 0;
      Element* prec   = NULL;
      while (ptc != NULL && !trouve) {
        prec   = ptc;
        ptc    = ptc->suivant;
        if (ptc!=NULL) trouve = enOrdre (objet, ptc->reference, li->type==1, li->comparer);
      }
      insererApres (li, prec, objet);
    }
  }
}