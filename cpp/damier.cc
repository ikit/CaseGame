
#include "damier.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Constructeurs

damier::damier(int taille): divin(taille< 3 ? 9 : taille*taille, taille < 3 ? 9 : taille*taille)
{
  //cout << " -> damier_cons";
  if (taille<3) taille=3;

  tail=taille;
  int Td = tail*tail;
  dami = new bool[Td];
  dami_dep = new bool[Td];
  dami_fin = new bool[Td];

  for (int i=0; i<Td; i++)
    {
      dami[i]=0;
      dami_dep[i]=0;
      dami_fin[i]=0;
    }

  if (tail==3)
    {
      type = 0;
      // remplissage spécial de divin pour les damiers 3x3
      divin[4]=1;  divin[10]=1;  divin[21]=1;  divin[28]=1;  divin[36]=1;  divin[46]=1;  divin[53]=1;  divin[66]=1;  divin[71]=1;
      divin[5]=1;  divin[11]=1;  divin[22]=1;  divin[30]=1;  divin[38]=1;  divin[47]=1;  divin[55]=1;  divin[67]=1;  divin[72]=1;
      divin[7]=1;  divin[12]=1;  divin[24]=1;  divin[31]=1;  divin[40]=1;  divin[49]=1;  divin[56]=1;  divin[68]=1;  divin[73]=1;
      divin[8]=1;  divin[13]=1;  divin[25]=1;  divin[33]=1;  divin[42]=1;  divin[50]=1;  divin[58]=1;  divin[69]=1;  divin[75]=1;
      divin[9]=1;  divin[14]=1;  divin[27]=1;  divin[34]=1;  divin[44]=1;  divin[52]=1;  divin[59]=1;  divin[70]=1;  divin[76]=1;
    }
  else
    {
      type = 1;
      // remplissage spécial de divin pour les damiers de taille >3
      // on inverse la case ainsi que ttes les cases se trouvant su la mm lg et la mm col

      // NB: si on veut faire un nouveau type de transfo (avec l'IA), il faut modifier le constructeur, car divin 
      //     trouve la solution, mais il faut que divin soi en remplis en fonction des transfos (cf rapport)

      for (int ict_L=1; ict_L<=tail; ict_L++)
	for(int ict_C=1; ict_C<=tail; ict_C++)
	  {
	    // on transforme ttes les cases placees sur la meme colonne que la ieme case-transfo
	    for (int iC=1; iC<=tail; iC++)
	      divin.set_coeff( (ict_L-1)*tail+iC, (ict_L-1)*tail+ict_C, true);
	    
	    // on transforme ttes les cases placees sur la meme ligne que la ieme cases-transfo
	    for (int iL=1; iL<=tail; iL++)
	      divin.set_coeff( (iL-1)*tail+ict_C, (ict_L-1)*tail+ict_C, true);
	    
	  }
      // On calcul l'inverse de divin
      divin.inverse();
    }
  //cout << " -> succés\n";
}
      

// destructeur

damier::~damier()
{
  //cout << " -> damier_destruct";
  delete [] dami;
  delete [] dami_dep;
  delete [] dami_fin;
  //cout << " -> succés\n";
}



// constructeur par copie
damier::damier(const damier &source):
  tail(source.tail),
  type(source.type),
  divin(source.divin)

{
  //cout << " -> damier_copie";
  int Td = tail*tail;
  dami = new bool[Td];
  dami_dep = new bool[Td];
  dami_fin = new bool[Td];

  for (int i=0; i<Td; i++)
    {
      dami[i]    = source.get_case(i);
      dami_dep[i]= source.get_case_dep(i);
      dami_fin[i]= source.get_case_fin(i);
    }
 
  //cout << " -> succés\n";
}

         
  
// operateur d'affectation
damier damier::operator= (const damier &source) 
{ 
  //cout << " -> damier_=";
  tail = source.get_taille(); 
  type = source.get_type(); 
  
  divin =  source.get_divin();
  delete[] dami; // on supprime pour reallouer car les tailles ne sont
                 // pas forcement egales
  delete[] dami_dep;
  delete[] dami_fin;

  int Td = tail*tail;
  dami = new bool[Td];
  dami_dep = new bool[Td];
  dami_fin = new bool[Td];
  for (int i=0; i<Td; i++)
    {
      dami[i]    = source.get_case(i);
      dami_dep[i]= source.get_case_dep(i);
      dami_fin[i]= source.get_case_fin(i);
    }
      
  //cout << "  -> succés\n";
  return *this;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Accesseur en lecture pour les damiers

bool damier::get_case(int lig, int col) const
{
  if ((lig-1)*tail+col-1 >= tail*tail) return false;
  return dami[(lig-1)*tail+col-1];
}



bool damier::get_case_dep(int lig, int col) const
{
  if ((lig-1)*tail+col-1 >= tail*tail) return false;
  return dami_dep[(lig-1)*tail+col-1];
}



bool damier::get_case_fin(int lig, int col) const
{
  if ((lig-1)*tail+col-1 >= tail*tail) return false;
  return dami_fin[(lig-1)*tail+col-1];
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Accesseur en ecritur pour les damiers

void damier::set_case(int lig, int col, bool val)
{
  if ((lig-1)*tail+col-1 <= tail*tail)
    dami[(lig-1)*tail+col-1] = val;
}



void damier::set_case_fin(int lig, int col, bool val)
{
  if ((lig-1)*tail+col-1 <= tail*tail)
    dami_fin[(lig-1)*tail+col-1] = val;
}



void damier::set_case_dep(int lig, int col, bool val)
{
  if ((lig-1)*tail+col-1 <= tail*tail)
    dami_dep[(lig-1)*tail+col-1] = val;
}



void damier::inverse_case(int lig, int col)
{
  if ((lig-1)*tail+col-1 <= tail*tail)
    dami[(lig-1)*tail+col-1] = !dami[(lig-1)*tail+col-1];
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Fonctions sur les damiers

void damier::affiche(int dam)
{  
  char* nomdam;
  bool* damselect;
  char* coul_bordure = "\033[0;47;30m"; // noir sur fond blanc
  char* coul_case_1  = "\033[0;41;31m";  // rouge sur fond rouge
  char* coul_case_0  = "\033[0;40;30m";  // noir sur fond noir
  char* coul_default = "\033[0m";       // interface par default de la console
  switch (dam)
    {
    case 0:
      nomdam= "Damier de depart";
      damselect = dami_dep;
      break;
    case 2:
      nomdam= "Damier solution à obtenir";
      damselect = dami_fin;      
      coul_bordure = "\033[0;47;34m";
      coul_case_1  = "\033[0;43;31m";
      coul_case_0  = "\033[0;40;30m"; 
      coul_default = "\033[0m";       
      break;
    default:
      nomdam = "Damier en cours";
      damselect = dami;
      break;
    }


  cout << nomdam <<":\n  ";
  printf(coul_bordure);
  cout << "   "; // 2 espace + 2espaces pour les indices des lignes

  // on affiche les indices des colonnes
  for (int i=0; i<tail; i++)
    {
      cout << i+1;
      if (i<9) cout << "  ";
      else cout << " ";
    }
  cout << " ";

  // Affichage du reste du tableau
  for (int iL=0;iL<tail;iL++)
    {
      printf(coul_default);
      cout << endl;
      printf(coul_default);
      cout << "  "; // le tableau est legerement indente vers la droite
      printf(coul_bordure);     
      cout << iL+1; 
      if (iL<9) cout << "  ";
      else cout << " ";

      //pour la ligne en cours, on affiche toutes les cases (colonnes)
      for (int iC=0; iC<tail; iC++)
	{
	  if (damselect[iL*tail+iC])
	    {
	      printf(coul_case_1);
	      cout << "   ";
	    }
	  else
	    {
	      printf(coul_case_0);
	      cout << "   ";
	    }
	}
      // la bordure du tableau
      printf(coul_bordure);
      cout << " ";
      
    }
  printf(coul_default);
  cout << endl << "  ";
  printf(coul_bordure);
  for (int i=0; i<=tail; i++)
    cout << "   ";
  cout << " ";
  // on remet par defaut, et on repasse à la ligne
  printf(coul_default);
  cout << "\n\n";
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// fonctions tests et ia

bool damier::resolu() const
{
  for (int i=0; i<tail*tail; i++)
    if (dami[i]!=dami_fin[i]) return false;
  return true;
}


int* damier::solution()
{
  int max = tail*tail;
  // d'abord on détermine les cases qu'il faudra changer
  matbin resultat(max,1);
  for (int i=0; i<max; i++)
    if (dami[i]!=dami_fin[i]) resultat[i]=true;

  // ensuite il suffit de multiplier divin par resultat pour connaitre les coup
  matbin transfo = divin * resultat ;

  // maintenant on ne garde que les n° des transfo à faire
  int res[max];
  int indice=0;
  for (int i=0; i<max;i++)
    if (transfo[i]==1)
      {
	res[indice]=i+1;
	indice++;
      }
  
  // tant qu'a faire, autant optimiser res, 
  // la premiere case du tableau indiquera le nbr de coup
  // les autres contiendront les n° des coups à faire
  int* soluce= new int[indice+1];
  soluce[0]=indice;
  
  for (int i=1; i<indice+1; i++)
    soluce[i]=res[i-1];

  return soluce;
}



void damier::init_debut()
{
  for (int i=0; i<tail*tail; i++)
    dami[i]=dami_dep[i];
}
