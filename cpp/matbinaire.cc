
#include "matbinaire.h"
#include <iostream>
using namespace std;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//constructeurs / destructeur

matbin::matbin()
{
  pCoeff = new bool[1];
  pCoeff[0]=false;
  cl=1;
  lg=1;
  //cout << " -> matrice [1x1] cree\n";
 
}



matbin::matbin(unsigned int lig, unsigned int col)
{
  pCoeff = new bool [lig*col];
  for (int i=0; i<lig*col; i++) pCoeff[i]=false; // initialisation des coeffs
  cl=col;
  lg=lig;
  //cout << " -> matrice [" <<lig<<"x"<<col<<"] de cree.\n";
}



matbin::~matbin()
{
  delete [] pCoeff;
  //cout << " -> matrice d�ruite\n";
}


// constructeur par copie
matbin::matbin(const matbin &source)
{
  int l=source.haut();
  int c=source.larg();

  pCoeff = new bool [l*c];
  for (int i=0; i<l*c; i++) pCoeff[i]=source[i]; // copie  des coeffs
  cl=c;
  lg=l;
  //cout << " -> matrice [" <<l<<"x"<<c<<"] de copiee.\n";
}



// operateur d'affectation
matbin matbin::operator= (const matbin &source)
{
  cl = source.larg();
  lg = source.haut();
  delete[] pCoeff; // on supprime pour reallouer car les tailles ne sont
                   // pas forcement egales

  pCoeff = new bool [lg*cl];
  for (int i=0; i<lg*cl; i++)
    pCoeff[i]=source[i];
  //cout << " -> matrice d'allouee\n";
  return *this;
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// accesseurs en ecriture 

void matbin::set_coeff(unsigned int indice, bool val)
{
  pCoeff[indice] = val;
}



void matbin::set_coeff(unsigned int lig, unsigned int col, bool val)
{
  set_coeff((col-1)+(lig-1)*cl, val);
}



void matbin::set_inverse(unsigned int lig, unsigned int col)
{
  
  set_coeff((col-1)+(lig-1)*cl, !get_coeff(lig, col));
}



void matbin::set_inverse(unsigned int indice)
{
  set_coeff(indice, ! get_coeff(indice));
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// accesseurs en lecture

bool matbin::get_coeff(unsigned int lig, unsigned int col) const
{
  if ((lig > lg) || (col > cl)) 
    return false;

  return(pCoeff[(col-1)+(lig-1)*cl]);
}



bool matbin::get_coeff(unsigned int coef) const
{
  if (coef > lg*cl) 
    return false;

  return (pCoeff[coef]);
}



bool * matbin::get_col(unsigned int colonne) const
{
  if (colonne > cl)
    return false;
  
  bool * col = new bool [lg];
  for (int i=1; i<=lg; i++)
    col[i-1] = get_coeff(i , colonne );
  return col;
  
}



bool * matbin::get_lig(unsigned int ligne) const
{
  if (ligne > lg)
    return false;
  
  bool * lig = new bool [cl];
  for (int i=1; i<=cl; i++)
    lig[i-1] = get_coeff(ligne , i );
  return lig;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Operations sur les lignes

void matbin::L_transposition(unsigned int ligne1, unsigned int ligne2)
{
  if (ligne1<=lg && ligne2<=lg)
    {
      // copie de la ligne nligne1
      bool * L_tmp = get_lig(ligne1);

      // ligne2 -> ligne1
      for (int i=1; i<=cl; i++)
	set_coeff(ligne1, i, get_coeff(ligne2, i));

      // ligne1 -> ligne2
      for (int i=1; i<=cl; i++)
	set_coeff(ligne2, i, L_tmp[i-1]);
     delete [] L_tmp;
    }
}



void matbin::L_inverse(unsigned int ligne)
{
  if (ligne<=lg)
    for (int i=1; i<=cl;i++)
      set_coeff(ligne,i,! get_coeff(ligne,i));
}



void matbin::L_add_et_remplace(unsigned int ligne1, unsigned int ligne2)
{
    if (ligne1<=lg && ligne2<=lg)
      for(int i=1; i<=cl;i++)
	//on additionne (xor) 
	set_coeff(ligne1, i, get_coeff(ligne1,i) ^ get_coeff(ligne2, i)); 
}


/*

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// operations sur la matrice

void matbin::inverse()
{
  if (cl==lg)  
 {
   
   matbin mat(lg,2*cl);
  // 2.1 On copie la matrice dans la premiere moitie de mat
      for (int iLg=1; iLg<=lg; iLg++)
	for (int iCl=1; iCl<=cl; iCl++)
	  mat.set_coeff(iLg, iCl, get_coeff(iLg, iCl));
  // 2.2 On copie l'identite dans la deuxieme moitie de mat
      for (int i=1; i<=lg; i++)
	mat.set_coeff(i, i+cl, 1);

      //mat.affiche("Mat-tmp");

  /* Maintenant, par une suite d'op sur les lignes, on 
     fait passer l'id dans l'autre moitie de la mat */
  // cf Rapport d'analyse page 8 pour l'explication detaille de l'algorithme
      for (int Lg_pivot=1; Lg_pivot<lg; Lg_pivot++)
	{
	  for (int iLg=Lg_pivot+1; iLg<=lg; iLg++) 
	    {
	      if(mat.get_coeff(iLg, Lg_pivot))             // si pivot potentiel 
		{
		  if (mat.get_coeff(Lg_pivot, Lg_pivot))   // si deja pivot
		      mat.L_add_et_remplace(iLg, Lg_pivot);
		  else                                     // sinon
		    mat.L_transposition(Lg_pivot, iLg);    
		}
	    }
	}
      //mat.affiche("Mat-trisup");

  // la matrice est maintenant triangulaire superieure
  //on la diagonalise
      for (int Lg_pivot=lg; Lg_pivot>1; Lg_pivot--)
	{
	  for (int iLg=Lg_pivot-1; iLg>=1; iLg--)
	      if(mat.get_coeff(iLg, Lg_pivot))       
		  mat.L_add_et_remplace(iLg, Lg_pivot);
	}
      //mat.affiche("Mat-diag");
  // on recopie la matrice inverse obtenue 
      for (int iLg=1; iLg<=lg; iLg++)
	for (int iCl=1; iCl<=cl; iCl++)
	  set_coeff(iLg, iCl, mat.get_coeff(iLg, cl+iCl));
 }
}


void matbin::affiche(char nom[10] = "") const
{
  cout << "\nAffichage de la matrice: " <<nom<<"["<<lg <<"x"<<cl<<"]:";
  for (unsigned int iL=0;iL<lg;iL++)
    {
      cout << "\n   ";
      for (unsigned int iC=0; iC<cl;iC++)
	{
	  if (pCoeff[iL*cl+iC])
	    {
	      printf("\033[0;0;31m");
	      cout <<" "<< pCoeff[iL*cl+iC];
	    }
	  else
	    {
	      printf("\033[0m");
	      cout <<" "<< pCoeff[iL*cl+iC];
	    }
	}
    }
  // remise par default du mode graphique de la console
  printf("\033[0m");
  cout << endl;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Surcharges des operateurs


matbin operator+ (const  matbin &gch,const  matbin &dt)
{
  if (gch.haut()!=dt.haut() || gch.larg()!=dt.larg() )
    {
      matbin res;
      cout << " -> Addition impossible : formats incompatibles.\n";
      return res ; // on renvoie la matrice nulle si impossible
    }

  matbin res(gch.haut(), gch.larg());
  unsigned int max=gch.haut()*gch.larg();

  for (unsigned int i=0; i<max ; i++)
    res[i] = gch[i] ^ dt[i];
  return res;
}



matbin operator* (const  matbin &gch,const  matbin &dt)
{
  if (gch.larg()!=dt.haut())
    {
      matbin res;
      cout << " -> Multiplication impossible : formats incompatibles.\n";
      return res ; // on renvoie la matrice nulle si impossible
    }

  matbin res(gch.haut(), dt.larg());
  bool coeff;
  for (unsigned int iL=1; iL<=gch.haut() ; iL++)
    {
      for (unsigned int iC=1; iC<=dt.larg(); iC++)
	{
	  coeff=false; // initialis��false car false=0 est neutre pour l'addition
	  // calcul du coeff res(iL,iC)
	  // ^  revient �l'addition de 2 binaires
	  // && revient �la multiplication de 2 binaires
	  for (unsigned int j=1; j<=gch.haut(); j++)
	    coeff=coeff ^ ( gch.get_coeff(iL,j) && dt.get_coeff(j,iC) );
	  res.set_coeff(iL, iC, coeff);
	}
    }
  return res;
}




ostream & operator << (ostream & os, const matbin & mat)
{
  unsigned int lg=mat.larg(), cl=mat.haut();
  for (unsigned int iL=0;iL<lg;iL++)
    {
      cout << "\n   ";
      for (unsigned int iC=0; iC<cl;iC++)
	{
	  if (mat[iL*cl+iC])
	    {
	      printf("\033[0;0;31m");
	      cout <<" "<< mat[iL*cl+iC];
	    }
	  else
	    {
	      printf("\033[0m");
	      cout <<" "<< mat[iL*cl+iC];
	    }
	}
    }
  // remise par default du mode graphique de la console
  printf("\033[0m");
  cout << endl;
}




/*

// Op�ateurs d'acc� :
bool operator()(unsigned int i, unsigned  int j)
{
    return pCoeff[(i-1)*cl+(j-1)];
}

const bool operator()(unsigned int i, unsigned int j) const
{
    return pCoeff[(i-1)*cl+(j-1)];
}


*/
