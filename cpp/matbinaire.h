/*  Nom       : Librairie matbinaire.h
 *  Utilite   : Permet d'effectuer des operations mathematique
 *              avec des matrices binaires quelconques
 *  Auteur    : GUEUDELOT Olivier - gueudelotolive@wanadoo.fr
 *  Date      : Creee octobre - novembre 2004
 *
 *  Remarques : - 
 */

#ifndef MATBINAIRE_H
#define MATBINAIRE_H

#include <iostream>
using namespace std;
class matbin
{
  // Les operateurs de base:
  friend matbin operator*(const matbin &, const matbin &);
  friend matbin operator+(const matbin &, const matbin &); 


 public:
  // constructeurs / destructeurs
  matbin();
  matbin(unsigned int, unsigned int);
  ~matbin();
  matbin (const matbin &);             // constructeur de copie
  matbin operator=(const matbin &);   // operateur d'affectation

  // accesseurs en ecriture
  void set_coeff(unsigned int,bool);
  void set_coeff(unsigned int, unsigned int, bool);
  void set_inverse(unsigned int);
  void set_inverse(unsigned int, unsigned int);

  // accesseurs en lecture
  bool get_coeff(unsigned int ,unsigned int) const;
  bool get_coeff(unsigned int) const;
  bool &operator[](unsigned int indice)
    { return pCoeff[indice]; };
  const bool &operator[](unsigned int indice) const 
    { return pCoeff[indice]; };

  bool * get_col(unsigned int) const;
  bool * get_lig(unsigned int) const;
  unsigned int larg() const
    { return cl; };
  unsigned int haut() const
    { return lg; };

  // operations sur les lignes 
  void L_transposition(unsigned int, unsigned int);
  void L_inverse(unsigned int);
  void L_add_et_remplace(unsigned int, unsigned int);

  // operations sur la matrice
  void  inverse();
  void affiche(char nom[10]) const;


 private:
  unsigned int cl;    // nbr de colonnes
  unsigned int lg;    // nbr de lignes
  bool * pCoeff;      // tableau de pointeurs des lg*cl coeff de la matric
};

ostream & operator << (ostream &, const matbin &);

#endif

