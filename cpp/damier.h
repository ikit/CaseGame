/*  Nom       : Librairie damier.h
 *  Utilite   : Librairie creant des objets damier permettant
 *              de jouer au jeu des pions
 *  Auteur    : GUEUDELOT Olivier - gueudelotolive@wanadoo.fr
 *  Date      : Creee d'octobre à novembre 2004
 *
 *  Remarques : - 
 */

#ifndef DAMIER_H
#define DAMIER_H

#include "matbinaire.h"  

class damier
{

 public:
  damier(int =3);  // construit damier nxn vierge 
  ~damier();
  damier operator= (const damier &);
  damier(const damier &);

  // accesseur en lecture pour les damiers
  bool get_case(int, int) const;
  bool get_case_dep(int, int) const;  
  bool get_case_fin(int, int) const;
  int get_taille() const {return tail;};
  int get_type() const {return type;};
  matbin get_divin() const {return divin;};

  bool get_case(int i) const {return dami[i];};
  bool get_case_dep(int i) const {return dami_dep[i];};  
  bool get_case_fin(int i) const {return dami_fin[i];};

  // accesseur en ecriture pour les damiers
  void set_case(int, int, bool);
  void set_case_fin(int, int, bool);
  void set_case_dep(int, int, bool);
  void inverse_case(int, int);

  // fonction sur les damiers
  void affiche(int=0);

  // fonctions tests et ia
  bool resolu() const;
  int* solution();
  void init_debut();

 private:
  int tail;           // hauteur et largeur
  bool * dami;        // pointeur car on ne connait pas la taille 
  matbin divin;       // matrice qui une fois calculé donne le chemin le plus court
  bool * dami_dep;    // damier de départ
  bool * dami_fin;    // damier pour gagner
  int type;           // pour connaîre le type de transformations
  
};

#endif
