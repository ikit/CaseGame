
#include "damier.h"
#include <iostream>
#include <cstdlib> 
#include <cstdio>
#include <time.h>  
using namespace std;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                        Variables globales                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
damier damier_jeu;    // damier sur lequel on va jouer
int* sol_jeu;         // contient la solution de la partie
bool adv=0;           // indique si on joue contre une IA
int niv_IA=0;         // si IA, indique le niveau de difficulte
int maxcoup_IA=0;     // calculer en fct° du niveau de l'IA et du nbr
                      // de coup minimum pour gagner
int nbr_coup=0;       // compte le nbr de coups joue par le jouer humain
int nbr_aide=0;       // nbr de fois que le joueur a demande de l'aide


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                      Prototypes des fonctions                       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// NB: a lire comme une arborescence (indique qui utilise qui)

int menu_princ();             // menu principal du jeu
int menu_perdu();             // menu quand on perd une partie
int menu_gagne();             // menu quand on gagne une partie

int menu_select(int, int );   // permet la selection controle 
void affiche_soluce();        // affiche la solution optimale d'une 
                              // partie convenablement configuree
damier init_auto();           // configure aleatoirement une partie
damier init_perso();          // configuration manuelle d'une partie

bool jouer();                 // pour jouer (true = gagne, false = perdu)
   bool  jouer_coup();        // pour que le joueur entre un coup
      void jouer_aide();      // affiche l'aide (pour un tour donnée)
   void transfo_3x3(int, int);// applique une transfo pr 1 damier 3x3
   void transfo_nxn_typ1(int, int);// pareil mais pr 1 nxn de typ 1 
   void erreur(char* );       // affiche un msg d' erreur à l'ecran



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                    Implementation des fonctions                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void affiche_soluce()
{
  damier_jeu.init_debut(); // pour etre sur qu'on part bien du depart
  int taille=damier_jeu.get_taille();
  int max_jeu = sol_jeu[0];
  cout << "SOLUTION OPTIMALE \n\n";
  damier_jeu.affiche(0);
  int iL,iC,i, choix;
  for (int i=1; i<= max_jeu; i++)
    {
      int s = sol_jeu[i];
      int iC = (s-1)%taille+1;      // on trouve l'indice de colonne 
      int iL = (s-iC)/taille+1;     // on trouve l'indice de ligne
      cout << "coup n°"<<i<<", transformation n°"<< s <<" => (" << iL << " , " << iC << ")\n";
      if (taille==3) transfo_3x3(iL, iC);
      else transfo_nxn_typ1(iL, iC);
      if (i!=max_jeu)
	{
	  damier_jeu.affiche(1);
	  cout << "0 pour quitter, tt les autres entiers  pour continuer: ";
	  cin >> choix;
	  if (choix == 1) break;
	  cout << endl << endl;
	}
    }
  damier_jeu.affiche(2);
}



int menu_select(int min, int max)
{
  int select=min;
  bool select_encours = 1;
  cout << "Votre sélection : ";
  printf("\033[0;0;35m");  // on ecrit ses choix en violet
  while ( ! ( cin >> select ) || select < min || select > 6 )
    { 
      if ( cin.fail() )
        { 
	  printf("\033[0m");
	  cout << "Saisie incorrecte, recommencez : "; 
	  cin.clear(); 
	  cin.ignore( numeric_limits<streamsize>::max(), '\n' );
	  printf("\033[0;0;35m"); 
        }
      else
        {
	  printf("\033[0m");
	  cout << "Le chiffre n'est pas entre "<<min<<" et "<<max<<", recommencez : ";
	  printf("\033[0;0;35m"); 
        }
    }
  printf("\033[0m");
  return select;
}





int menu_princ()
{
  for (int i=0; i<30; i++) cout << endl;
  // presentation du titre general
  FILE * pSkin;
  pSkin = fopen ( "skin.ik" , "r" );
  char ligne_txt[100];
  while (!feof(pSkin)) 
    {
      fgets (ligne_txt , 100 , pSkin);
      cout << ligne_txt ;
    }
  fclose(pSkin);

  cout << " 1) Jouer [aleatoire]\n 2) Jouer [personnaliser]\n 3) Quitter\n\n";
  return menu_select(1,3);
}



int menu_perdu()
{
  int* sol_perso = damier_jeu.solution();
  int max_jeu = sol_jeu[0];
  int max_perso = sol_perso[0];

  cout << "\n\n------------------------- Bein c'est bien dommage tout ça ----------------------------\n";
  cout << "\nAlala ! z'êtes mauvais quand même !\n";
  cout << "Vite fait les stats:\n  .nombre de coup que vous avez mis pour perdre: " <<nbr_coup;
  cout << "\n  .vous avez utilisé l'aide " << nbr_aide << " fois";
  cout << "\n  .le chemin le plus court fait "    << max_jeu << " étapes";
  cout << "\n   les voici: ";
  printf("\033[0;0;31m");  
  for (int i=1; i<=max_jeu; i++) cout << sol_jeu[i] << "  ";
  printf("\033[0m");
  cout << "\n  .Si vous aviez continué, vous auriez pu gagner en faisant: ";
  printf("\033[0;0;35m");
  for (int i=1; i<=max_perso; i++) cout << sol_perso[i] << "  ";
  printf("\033[0m");
  
  cout << "\n\nQue désirez vous faire maintenant ?\n  1) Menu principal\n  2) Voir meilleur solution\n ";
  int choix = menu_select(1,2);

  if (choix == 2) affiche_soluce();
  delete[] sol_perso;
}



int menu_gagne()
{
  damier_jeu.affiche(2);
  int max_jeu = sol_jeu[0];
  cout << "\n\n------------------------- Félicitation ! Superbe victoire ----------------------------\n";
  cout << "Vite fait les stats:\n  .nombre de coup que vous avez mis: " <<nbr_coup;
  cout << "\n  .vous avez utilisé l'aide " << nbr_aide << " fois";
  cout << "\n  .le chemin le plus court fait "    << max_jeu << "étapes";
  cout << "\n   les voici: ";
  printf("\033[0;0;31m");  
  for (int i=1; i<=max_jeu; i++) cout << sol_jeu[i] << "  ";
  printf("\033[0m");
  cout << "\n\nQue désirez vous faire maintenant ?\n  1) Menu principal\n  2) Voir meilleur solution\n ";
  int choix = menu_select(1,2);

  if (choix == 2) affiche_soluce();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Initialisation personnel de la partie (damier + adversaire) 

damier init_perso()
{
  int choix, choix1, choix2;
  // param d'un damier
  int Dtail, Dtyp;
  

  cout << "Configuration d'une partie:\n";
  printf("\033[0;0;35m");
  cout << "\n\nQuelle taille pour le damier [3,30]?";
  printf("\033[0m");
  cout << "\nNB: pour les damier de taille > 3, la taille doit être un nombre pair : \n";
  bool menu =1;
  do {
    Dtail = menu_select(3, 30);
    if (Dtail%2 == 0 || Dtail==3) menu=0;
    else cout << "Un nombre pair on a dit ...\n";
  }
  while (menu);

  damier Dfinal(Dtail);
  int modif=1; // 1= damier_dep, 2=damier_fin, 3=termine
  do 
    {
      printf("\033[0;0;35m");
      cout << (modif==1 ? "\nDamier de départ:\n  " : "Damier d'arrivé:\n  ") ;
      printf("\033[0;0;37m");
      cout << "1] Vide\n  2] Plein\n  3] Aléatoire\n  4] Personnaliser\n";
      choix = menu_select(1, 4);
      cout << endl;
      switch(choix)
	{
	case 1:
	  // rien à changer;
	  modif++;
	  cout << endl;
	break;
	
      case 2:
	// on inverse ttes les cases
	for (int iL=1; iL <= Dtail; iL++)
	  for (int iC=1; iC <= Dtail; iC++)
	    {
	      if (modif==1) Dfinal.set_case_dep(iL, iC, true);
	      else Dfinal.set_case_fin(iL, iC, true);
	    }
	modif++;
	cout << endl;
	break;
	
      case 3:
	// aleatoire
	for (int iL=1; iL <= Dtail; iL++)
	  for (int iC=1; iC <= Dtail; iC++)
	    if (rand()%2==0)
	      {
		if (modif==1) Dfinal.set_case_dep(iL, iC, true);
		else Dfinal.set_case_fin(iL, iC, true);
	      }
	modif++;
	cout << endl;
	break;

      case 4:
      default:
	cout << "entrer des n° de ligne et de colonne erronés pour terminer\n";
	bool modif_perso=1; 
	int iL,iC;
	do {
	  Dfinal.affiche(modif==1? 0:2);
	  cout <<"Inverser la case:\n   ligne:   ";
	  printf("\033[0;0;35m");
	  cin >> iL;
	  printf("\033[0m");
	  cout << "   colonne: ";
	  printf("\033[0;0;35m");
	  cin >> iC;
	  printf("\033[0m");	  
	  if ( (iL<1 || iL>Dtail) && (iC<1 || iC>Dtail) )
	    {
	      modif++ ;
	      modif_perso=0;
	      cout << endl;
	    }
	  switch(modif)
	    {
	    case 1:
	      Dfinal.set_case_dep(iL, iC, Dfinal.get_case_dep(iL, iC)==1 ? 0 : 1);
	      break;
	    case 2:
	      Dfinal.set_case_fin(iL, iC, Dfinal.get_case_fin(iL, iC)==1 ? 0 : 1);
	      break;
	    }
	}
	while (modif_perso);
	
      } // fin de case 4: (qd on choisi de modifier soit-même les cases des damier _fin ou _dep)
    
    } // fin de la bcl while qui gère les damier _dep et _fin
  while (modif<3);

  printf("\033[0;0;35m");
  cout << "\nVoulez vous un adversaire:\n  " ;
  printf("\033[0m");
  cout << "1] Jouer contre Bebeto [facile]\n  2] Jouer contre Speedy [Dur]\n";
  cout << "  3] Jouer contre Yoda [Impossible]\n  4] Pas d'adversaire\n";
  niv_IA = menu_select(1,4);

  // on copie le damier_depar dans le damier courant et on calcul la solution optimale
  Dfinal.init_debut();
  sol_jeu = Dfinal.solution();

  // mintenant que l'on a la solution, on calcul l'IA
  adv = false;
  if (niv_IA!=4) {
    adv=true;
    maxcoup_IA= (4-niv_IA)*sol_jeu[0] + (rand()%(4-niv_IA))*2;
    }
  return Dfinal;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Initialisation du damier automatiquement



damier auto_damier()
{
  cout << "Creation de la partie en cours ... [1 minute au maximum]\n";
  int Dtail, Dtyp;
  bool test = true;

  // random taille [3,30], sachant que la taille doit etre paire
  Dtail = rand()%16*2;
  if (Dtail==0) Dtail = 3; 

  // adversaire ?
  if (rand()%2 == 0) adv=1; // une chance sur 2 de tomber sur un ennemis
  if (adv) niv_IA = rand()%3+1;
  
  // on previens ....
  cout << "\n-> Configuration en cours...\n Taille : "<<Dtail<<"x"<<Dtail;
  cout << "\n Type   : "<<(Dtail==3 ? "version minimal" : "ligne-colonne"); 
  cout << "\n Ennemis : "<<(adv? "oui\n":"non\n");
  damier Dfinal(Dtail);

  // random depart et random arrivé
  // arrivee
  for (int iL=1; iL <= Dtail; iL++)
    for (int iC=1; iC <= Dtail; iC++)
      if (rand()%2 == 0) Dfinal.set_case_fin(iL, iC, true);

  // depart
    for (int iL=1; iL <= Dtail; iL++)
      for (int iC=1; iC <= Dtail; iC++)
	if (rand()%2 == 0) Dfinal.set_case_dep(iL, iC, true);


  // histoire d'etre sur que les damiers sont differents
  int iL=rand()%(Dtail-1)+1;
  int iC=rand()%(Dtail-1)+1;
  if (Dfinal.get_case_dep(iL,iC) == Dfinal.get_case_fin(iL, iC) )
    {
      Dfinal.set_case_dep(iL, iC, true);
      Dfinal.set_case_fin(iL, iC, false);
    }

  // on copie le damier_depar dans le damier courant
  Dfinal.init_debut();

  // calcul sol et maxcoup_IA 
  // si niv_IA=4, maxcoup_IA sera faux mais c pas grave
  sol_jeu = Dfinal.solution();
  maxcoup_IA= (4-niv_IA)*sol_jeu[0] + (rand()%(4-niv_IA))*2;
    

  return Dfinal;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Fonctions du jeu


// NB: les verifications sur ligne et colonne sont faites dans jouer_coup();
void transfo_3x3(int ligne, int colonne)
{
  int case_i = (ligne-1)*damier_jeu.get_taille() + colonne-1;
  switch(case_i)
    {
    case 0: // case 1.1 du damier
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne , colonne+1);      
      damier_jeu.inverse_case(ligne+1 , colonne);
      damier_jeu.inverse_case(ligne+1 , colonne+1);
      break;
    case 1: // case 1.2 du damier
      damier_jeu.inverse_case(ligne , colonne-1);
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne , colonne+1);
      break;
    case 2: // case 1.3 du damier
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne , colonne-1);
      damier_jeu.inverse_case(ligne+1 , colonne);
      damier_jeu.inverse_case(ligne+1 , colonne-1);
      break;
    case 3: // case 2.1 du damier
      damier_jeu.inverse_case(ligne-1 , colonne);
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne+1 , colonne);
      break;
    case 4: // case 2.2 du damier
      damier_jeu.inverse_case(ligne , colonne-1);
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne , colonne+1);
      damier_jeu.inverse_case(ligne-1 , colonne-1);
      damier_jeu.inverse_case(ligne-1 , colonne);
      damier_jeu.inverse_case(ligne-1 , colonne+1);
      damier_jeu.inverse_case(ligne+1 , colonne-1);
      damier_jeu.inverse_case(ligne+1 , colonne);
      damier_jeu.inverse_case(ligne+1 , colonne+1);
      break;
    case 5: // case 2.3 du damier
      damier_jeu.inverse_case(ligne-1 , colonne);
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne+1 , colonne);
      break;        
    case 6: // case 3.1 du damier
      damier_jeu.inverse_case(ligne-1 , colonne);
      damier_jeu.inverse_case(ligne-1 , colonne+1);
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne , colonne+1);
      break;
    case 7: // case 3.2 du damier
      damier_jeu.inverse_case(ligne , colonne-1);
      damier_jeu.inverse_case(ligne , colonne);
      damier_jeu.inverse_case(ligne , colonne+1);
      break;
    case 8: // case 3.3 du damier
      damier_jeu.inverse_case(ligne-1 , colonne-1);
      damier_jeu.inverse_case(ligne-1 , colonne);
      damier_jeu.inverse_case(ligne , colonne-1);
      damier_jeu.inverse_case(ligne , colonne);
      break;
    default:
      char * err = "Coordonnee de la case invalide pour le damier [3x3]";
      erreur(err);
      break;
    }
}


// Dans cette version du jeu il n'y a qu'un type de transfo pour les damier quelconque
void transfo_nxn_typ1(int ligne, int colonne)
{
  int tail = damier_jeu.get_taille();

  for (int iL=1; iL<=tail; iL++)
    damier_jeu.inverse_case(iL, colonne);
  for (int iC=1; iC<=tail; iC++)
    damier_jeu.inverse_case(ligne, iC);

  damier_jeu.inverse_case(ligne,  colonne); // car les 2 bcl for sont passees 2 fois dessus et se donc annulees
}



bool jouer_coup()
{
  nbr_coup++;
  cout << "\nCoup n°" << nbr_coup << " :----------------------------------------------------------------------------------------\n";
  damier_jeu.affiche(1);
  int lig, col;
  int aide=0;
  int tail = damier_jeu.get_taille();

  // on previent s'il y a lieux le joueur que l'IA va bientôt gagner
  if (adv && maxcoup_IA <= nbr_coup+2)
    {
      printf("\033[0;0;31m"); // WARNING, texte en rouge
      cout << "ATTENTION";
      printf("\033[0m");
      if (maxcoup_IA-nbr_coup==0)
	cout << " Votre dernière chance de gagner c'est maintenant !\n";
      else
	cout << " l'IA va bientôt gagner (vous avez encore "<<maxcoup_IA-nbr_coup+1<<" coups)\n";
    }

  // on demande les coord de la case en prettant attention aux erreurs ...
  do
    {
      cout << "Entrer les coordonnees de la case liee a la transformation voulu";
      cout << "\n  ligne   : ";
      cin >> lig;
      if (lig <1)  // demande d'aide ou de quitter
	{
	  printf("\033[0;0;32m"); // texte en vert
	  cout << "Besoin d'aide ? ";
	  printf("\033[0m");
	  if (aide != 1)  // premiere fois qu'on demande de l'aide a ce tour
	    {
	      cout << "(oui=1, non=2, damier solution=3, quitter=4) ";
	      cin >> aide;
	      switch (aide)
		{
		case 1:
		  jouer_aide();
		  break;
		case 3:
		  damier_jeu.affiche(2);
		  damier_jeu.affiche(1);
		  break;
		case 4:
		  return false; // on quitte
		  break;        // pas necessaire ?
		case 2:
		default:
		  // bah rien il se passe rien alors ;-)
		  break;
		}
	    }
	  else
	    cout << "Faudrait p'tet pas abuser non plus !\n";
	}
    }
  while (lig>tail || lig <1);
  do // la même chose mais pour les colonnes
    {
      cout << "  colonne : ";
      cin >> col;
    }
  while (col>tail || col <1);

  // Maintenant que l'on a des coordonnees valide pour jouer, on modifie le damier
  int typ = damier_jeu.get_type();
  switch(typ)
    {
    case 0:
      transfo_3x3(lig, col);
      return true;
    case 1:
    default:
      transfo_nxn_typ1(lig, col);
      return true;
    }
}



void jouer_aide()
{
  nbr_aide ++;
  int* sol = damier_jeu.solution();
  int max = sol[0]-1;
  int taille = damier_jeu.get_taille();
  printf("\033[0;0;34m"); // texte en jaune
  if (sol[0]>1)
    {
      cout << "  Vous pouvez terminer en " << sol[0] << " coups\n";
      int i=(rand()%max)+1;
      int s = sol[i];
      int iC = (s-1)%taille+1;    
      int iL = (s-iC)/taille+1; 
      cout << "  Une des possibilites est la tranformation n°" << s <<" => (" << iL << " , " << iC << ")\n";
    }
  else
    cout << "  Vous exagerez un peu là .... c'est hyper simple";
  cout << endl;
  delete [] sol; // IMPORTANT car jeu.solution() utilise new mais pas delete []
                 // On aurait pu palier ce probleme en utilisant std::vector
                 // qui se charge tt seul de "new" et "delete"
  printf("\033[0m");
  cout << endl;
}


// renvoie vrai ou faux respectivement pour gagner ou perdu
bool jouer()
{
  nbr_coup=0; // nbr de coups init à 0
  nbr_aide=0; 
  bool coup;
  do
    {
      // 1) On fait jouer le joueur
      coup = jouer_coup();
      
      // 2) On verifie si le damier est resolu ou pas
      bool res = damier_jeu.resolu();
      if (res)
	{
	  cout << "Felicitation vous avez gagner";
	  return true; // on gagne
	}
      else
	{
	  // 3) On verifie si l'IA a gagne
	  if (adv && maxcoup_IA == nbr_coup)
	    {
	      cout << " L'IA a gagne avant vous en " << nbr_coup << " coups\n";
	      return false; // on perd
	    }
	}
    }
  while (coup);
  cout << "Partie abandonnée...\n";
  return false; // l'utilisateur desire quitter
}



void erreur(char *msg)
{
  printf("\033[0;0;31m");
  cout << "ERREUR -> " << msg;
  printf("\033[0m"); 
  cout << endl;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                             M A I N                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

main()
{
  srand ( time(NULL)%RAND_MAX );
  cout << "jfkdjfg";
  int choix_menu, parti;
  do
    {
      choix_menu = menu_princ();
      cout << endl;
      switch (choix_menu)
	{
	case 1:
	  damier_jeu = auto_damier();
	  break;
	case 2:
	  damier_jeu = init_perso();
	  break;
	case 3:
	default:
	  cout << "Au revoir !";
	  choix_menu=3; // a cause du default, pr etre sur qu'on quitte 
	  break;
	}
      if (choix_menu != 3)
	{
	  cout << "\n\n----------------------------------- Présentation de la partie ------------------------------------\n";
	  damier_jeu.affiche(0);
	  damier_jeu.affiche(2);
	  cout << "Type de transformation : "<<(damier_jeu.get_taille() == 3 ? "version-minimal":"ligne-colonne");
	  cout << "\nAdversaire             : ";
	  if (niv_IA == 1) cout << "Bebeto";
	  if (niv_IA == 2) cout << "Speedy";
	  if (niv_IA == 3) cout << "Yoda";
	  if (niv_IA == 4) cout << "Aucun";
	  cout << endl << endl ;
	  
	  bool parti = jouer();
	  if (parti)
	    menu_gagne();
	  else
	    menu_perdu();
	  // ne pas oublier sol_jeu
	  delete[] sol_jeu;
	  cout << endl << endl;
	}
    }
  while (choix_menu != 3);

  printf("\033[0;0;34m");
  cout << "\n\nJeu cree dans le cadre d'un projet d'informatique Deug MIAS I2";
  cout << "\nGueudelot Olivier - gueudelotolive@wanado.fr, 2004\n";
  printf("\033[0m");
}


