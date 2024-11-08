#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>



#define NUMBER_OF_COLUMNS 18
#define BINS 30
#define INFINITY_AND_BEYOND 100000


float coeffDevantSigma = 1;
bool suppressionDonneesAtypiquesParWafer = false;
bool supressionDonneesAtypiquesGeneraux = false;


typedef struct NoeudSeriesTemporelles
{
  float valeur;

  struct NoeudSeriesTemporelles *next;
}
NoeudSeriesTemporelles;

/*

Cette structure represente un noeud d'une liste chaine contenant les valeurs d'une colonne specifiques des fichiers csv (une valeur par noeud)

*/


typedef struct SeriesTemporelles
{
  int nodes_number[NUMBER_OF_COLUMNS];
  long int wafer_id;

  float moyenne[NUMBER_OF_COLUMNS];
  float min[NUMBER_OF_COLUMNS];
  float max[NUMBER_OF_COLUMNS];
  float ecart_type[NUMBER_OF_COLUMNS];
  float mediane[NUMBER_OF_COLUMNS];
  float quartile1[NUMBER_OF_COLUMNS];
  float quartile3[NUMBER_OF_COLUMNS];
  float frequenceBins[NUMBER_OF_COLUMNS][BINS];
  struct SeriesTemporelles *next;

  NoeudSeriesTemporelles *tete[NUMBER_OF_COLUMNS];
}
SeriesTemporelles;

/*

Cette structure contient les valeurs statistiques ainsi que les nombres des valeurs contenu dans une colonne specifique des fichiers csv
Elle contient aussi la variable wafer_id et qui sera utilise pour distinguer les differrents wafers.

Cette structure sera utilise en 2 modes differrents:

1 - Lorsqu'on cherche a calculer les statistiques generaux sans distinguer les wafers

Il y aura une seule structure SeriesTemporelles ou on stockera toutes les valeurs des 18 colonnes importantes des fichiers csv, 
grace aux 18 liste chaines dont leur tete se trouve dans la liste tete[]
On utilise pas les variables SeriesTemporelles *next et wafer_id.

2 - Lorsqu'on cherche a calculer les statistiques generaux en distinguant les wafers

A ce moment, la structure SeriesTemporelle pourra etre considere comme un noeud d'une liste chainee qui contient tout les wafers.
La variable SeriesTemporelles *next sera utilise donc pour le prochain noeud, et wafer_id contiendra l'id de son wafer.
Les statistiques contenues dans chaque structure SeriesTemporelles corresponderont alors a un seul wafer.

*/


typedef struct Wafers
{
  int wafers_number;

  SeriesTemporelles *tete;
}
Wafers;

/*

Cette structure sera utilise uniquement lorsqu'on distingue les wafers.
Elle contient le nombre des wafers ainsi que la tete de la liste chaine contenant tous les wafers differrent, dont j'ai
parle a la structure differrente

*/


void AjoutNoeudEnOrdre(SeriesTemporelles *seriesTemporelles, float *listeDonnees);

/*

Cette fonction ajoute une ligne de valeurs et ordonne chaque colonne par ordre croissant. Elle sera utilisée pour le calcul des
minimum, maximum, médiane, quartiles

@param seriesTemporelles – liste chainée à remplir et à ordonner

@param listeDonnees – tableau correspondant à une ligne de 18 valeurs 


Cette fonction rend le code tres lent lorsqu'on lit tous les fichiers.
Elle n'est plus utilise dans le code

*/

void AjoutNoeud(SeriesTemporelles *seriesTemporelles, float *listeDonnees);

/* 

Cette fonction ajoute une ligne de valeurs et sera appelée à la lecture du fichier pour remplir la liste chainée avec toutes les
données des fichiers csv concaténés

@param seriesTemporelles – liste chainée à remplir

@param listeDonnees – tableau correspondant à une ligne de 18 valeurs 

*/

void LireFic(Wafers *wafers, SeriesTemporelles* statsGeneraux, char* nom);

/* 

Cette fonction lit le fichier appelé nom et remplit la liste chainée avec toutes ses données. 
Cette fonction sera appelée plusieurs fois pour obtenir une liste chainée seriesTemporelles des données de tous les fichiers
concaténés. 
La fonction fait appel à la fonction conversionLigne (voir prototype ci-dessous) pour convertir chaque ligne de caractère en float 
et à la fonction ajoutNoeudEnOrdre afin de directement stocker les valeurs par ordre croissant.

@param seriesTemporelles – liste chainée à remplir

@param nom : nom du fichier csv dont on extrait les données.  

*/

void LireToutLesFic(Wafers *wafers, SeriesTemporelles *statsGeneraux);

/*

Cette fonction appelle plusieurs fois la fonction precedente pour lire tous les 59 fichiers csv.

@param wafers - la liste chaine qui contiendra tous les wafers chaqun avec ses statistiques, trouves dans les fichiers csv. A l'appel cette fonction elle contient aucun wafer, sauf si on a fait une lecture d'un seul fichier dans le menu.

@param statsGeneraux - structure qui contiendra tous les statistiques sur tous les donnees trouves dans les fichiers. A l'appel de cette fonction elle contient pas des donnees, sauf si on a fait une lecture d'un seul fichier dans le menu.

*/

void InitSeriesTemporelles(SeriesTemporelles *seriesTemporelles);

/* 

Cette fonction permet d’initialiser au départ la liste chainée.

@param seriesTemporelles – liste chainée à initialiser

*/

void InitWafers(Wafers *wafers);
/*

Cette fonction initialise la structure wafer

@param wafers - la structure en question

*/

void SupprimeAtypiques(SeriesTemporelles *seriesTemporelles);

/* 

Cette fonction permet de supprimer pour chaque série (ie colonne) les données supérieures à m+kσ et inférieures à m-kσ. L’entier
k sera choisi par l’utilisateur.

 

@param seriesTemporelles – liste chainée dont on souhaite supprimer quelques maillons 

@param k – entier appartenant à {1,2}

*/

void CalculsStatsGeneraux(SeriesTemporelles* seriesTemporelles);

/* 

Cette fonction permet de calculer pour chaque série : 
le minimum, le maximum, la médiane, la moyenne, l’écart-type et les quartiles.

Elle ne distingue pas les statistiques en fonction de wafer.
La liste chainée a déjà été ordonnée lors de son remplissage. 
Les statistiques descriptives sont stockées dans les tableaux.

SeriesTemporelles->moyenne[18];

SeriesTemporelles->min[18];

SeriesTemporelles->max[18];

SeriesTemporelles->ecart_type[18];

SeriesTemporelles->mediane[18];

SeriesTemporelles->quartile1[18];

SeriesTemporelles->quartile3[18];

On agrège également chaque série en 30 bins égaux et on calcul la fréquence pour chaque bin.

SeriesTemporelles->frequenceBins[18][BINS];

@param seriesTemporelles – liste chainée à exploiter pour calculer les statistiques

*/

void CalculsParWafer(Wafers *wafers);

/*

Meme chose que la fonction precedente, sauf qu'on distingue chaque wafer grace a leurs id

@param wafers - la liste chainee contenant les wafers deja trouves dans les fichiers csv

*/

void AjoutWafer(Wafers *wafers, long int wafer_id);

/*
 
Cette fonction ajoute un nouveau wafer a la liste chaine contenant les autres wafers deja trouves dans les fichiers csv.
Lorsqu'on ajoute un wafer on introduira que que son wafer_id dans la structure.
 
@param wafers - la liste chainee contenant ls wafers

@param wafer_id - l'id de nouveau wafer qu'on va introduire

*/

void CalculsParWafer(Wafers *wafers);

/*

 Cette fonction est identique à la précédente mais sera utilisée pour une liste chainée agrégée par wafers

@param wafers - liste chainee contenant les wafers toruves dans les fichiers csv

*/

long int ConversionLigne(char *ligne,float *listeDonnees);

/*

Cette fonction lit la ligne de caractères ligne et convertie les caractères en réels. Elle remplit le tableau listeDonnes avec 
les valeurs réelles de la ligne de et retourne l’identifiant du wafer auquel la ligne correspond.



@param ligne – ligne de caractère à convertir en float

@param listeDonnees – tableau à remplir les valeurs réelles issues de la conversion

@return – ID du wafer correspondant a la ligne en entree

*/

void ExportationGeneraux(SeriesTemporelles *statsGeneraux);

/*

Cette fonction exporte tout les statistiques generaux (sans distinction de wafer) contenus dans la structure statsGeneraux, dans un fichier appelle StatistiquesGeneraux.txt

@param statsGeneraux - structure contenant tout les statistiques 

*/

void ExportationParWafer(Wafers *wafers);

/*

Meme chose que la fonction precedente, sauf que on separe les donnnees par wafer
Ces donnees sont stockees dans  le fichier appelle StatistiquesParWafer.txt

@param wafers - structure contenant les wafers, chacun aves ses statistiques, trouves dans les fichiers csv

*/

void Fusionner(float tab[], int deb, int fin1, int fin);

void Trier (float tab[], int deb, int fin);

/*

Fonctions faites aux seance 4.
Elles vont servir pour le tri d'un tableau.

*/

void FusionStatsGeneraux(SeriesTemporelles *statsGeneraux);

/*

Cette fonction va trier les valeurs de chaque liste chainee contenant les valeurs d'une colonne ds fichiers csv
Elle utiliser la fonction Trier() presentee precedemment

@param statsGeeneraux - structure contenant tout les statistiques, ainsi que toutes les valeurs de chaque colonne de fichiers csv lus,  sans distinction des wafers

*/

void FusionParWafer(Wafers *wafers);

/*

Cette fonction fais la meme chose que la precedente, sauf que les donnees sont classes par wafer

@param wafers - liste chaines contenant tout les wafers

*/

void Menu();

/*

Fonction qui lancera le menu dont l'utilisateur se servira pour utiliser ce programmme.

*/


void InitWafers(Wafers *wafers)
{
  wafers->wafers_number = 0;
  wafers->tete = NULL;

}

void AjoutWafer(Wafers *wafers, long int wafer_id)
{
  SeriesTemporelles *newWafer = malloc(sizeof(SeriesTemporelles));
  newWafer->wafer_id = wafer_id;
  InitSeriesTemporelles(newWafer);
  newWafer->next = wafers->tete;
  wafers->tete = newWafer;
  wafers->wafers_number++;
}

void InitSeriesTemporelles(SeriesTemporelles *seriesTemporelles)
{


  for(int k = 0; k<NUMBER_OF_COLUMNS; k++)
  {
    for(int p = 0; p < BINS; p++)
    {
      seriesTemporelles->frequenceBins[k][p] = 0;
    }
    seriesTemporelles->tete[k] = NULL;
    seriesTemporelles->nodes_number[k] = 0;
    seriesTemporelles->moyenne[k] = 0;
    seriesTemporelles->min[k] = INFINITY_AND_BEYOND;
    seriesTemporelles->max[k] = -INFINITY_AND_BEYOND;
    seriesTemporelles->ecart_type[k] = 0;
    seriesTemporelles->mediane[k] = 0;
    seriesTemporelles->quartile1[k] = 0;
    seriesTemporelles->quartile3[k] = 0;
  }
}

void AjoutNoeud(SeriesTemporelles *seriesTemporelles, float *listeDonnees)
{
  NoeudSeriesTemporelles **nouveauNoeud = malloc(sizeof(NoeudSeriesTemporelles*) * NUMBER_OF_COLUMNS);

  for(int k = 0; k<NUMBER_OF_COLUMNS; k++)
  {
    nouveauNoeud[k] = malloc(sizeof(NoeudSeriesTemporelles));
    nouveauNoeud[k]->valeur = listeDonnees[k];
    nouveauNoeud[k]->next = seriesTemporelles->tete[k]; //le nouveau element sera en tete
    seriesTemporelles->tete[k] = nouveauNoeud[k];
    seriesTemporelles->nodes_number[k]++;
  }

}

void AjoutNoeudEnOrdre(SeriesTemporelles *seriesTemporelles, float *listeDonnees)
{
  NoeudSeriesTemporelles **nouveauNoeud = malloc(sizeof(NoeudSeriesTemporelles*) * NUMBER_OF_COLUMNS);

  for(int k = 0; k<NUMBER_OF_COLUMNS; k++)
  {
    NoeudSeriesTemporelles *ancien = seriesTemporelles->tete[k];
    NoeudSeriesTemporelles *present = seriesTemporelles->tete[k];
    nouveauNoeud[k] = malloc(sizeof(NoeudSeriesTemporelles));
    nouveauNoeud[k]->valeur = listeDonnees[k];

    while(present != NULL && present->valeur > nouveauNoeud[k] -> valeur)
    {
      ancien = present;
      present = present->next;
    }
    if(present == seriesTemporelles->tete[k])
    {
      nouveauNoeud[k]->next = seriesTemporelles->tete[k];
      seriesTemporelles->tete[k] = nouveauNoeud[k];
      seriesTemporelles->nodes_number[k]++;
      continue;
    }

    ancien->next = nouveauNoeud[k];
    nouveauNoeud[k]->next = present;
    seriesTemporelles->nodes_number[k]++;
  }

}

long int ConversionLigne(char *ligne,float *listeDonnees)
{
  long int waferId;
  char **charNum = malloc(sizeof(char*) * 30);
  char *endptr;
  const char separation[2] = "\t";
  int count = 0;
  for(int k = 0; k<25; k++)
  {
    charNum[k] = malloc(sizeof(char) * 200);
  }
  charNum[0] = strtok(ligne,separation);
  for(int k = 1; k<25; k++)
  {
    charNum[k] = strtok(NULL,separation);
  }

  for(int k = 0; k<25; k++)
  {
    if(k == 3)
      waferId = strtol(charNum[k],&endptr,10);
    if(k == 0 || k == 1 || k == 2 || k == 3 || k == 4 || k == 5 || k == 23)  //elimination des colonnes inutiles
      continue;
    listeDonnees[count] = atof(charNum[k]);
    count++;

  }
  return waferId;
}

void LireFic(Wafers *wafers, SeriesTemporelles *statsGeneraux, char *nom)
{
  FILE *fic;
  fic=fopen(nom,"r");
  int wafer_id;
  if((long int)fic==0)
  {
    printf("\n Erreur à la lecture du fichier \n");
    return;
  }
  char ligne[1000];
  float listeDonnees[NUMBER_OF_COLUMNS];
  fgets(ligne,1000,fic);  //On saute la premiere ligne qui contient les titres
  while(fgets(ligne,1000,fic))
  {
    wafer_id = ConversionLigne(ligne,listeDonnees);
    SeriesTemporelles *temp = wafers->tete;
    while(temp != NULL && temp->wafer_id != wafer_id)
      temp = temp->next;
    if(temp == NULL)
    {
      AjoutWafer(wafers,wafer_id);
      temp = wafers->tete;
    }
    AjoutNoeud(temp,listeDonnees);
    AjoutNoeud(statsGeneraux,listeDonnees);
  }
    fclose(fic);
}

void CalculsStatsGeneraux(SeriesTemporelles *seriesTemporelles)
{
  for(int k = 0; k < NUMBER_OF_COLUMNS; k++)
  {
    for(int p = 0; p < BINS; p++)
    {
      seriesTemporelles->frequenceBins[k][p] = 0;
    }
    seriesTemporelles->moyenne[k] = 0;
    seriesTemporelles->min[k] = INFINITY_AND_BEYOND;
    seriesTemporelles->max[k] = -INFINITY_AND_BEYOND;
    seriesTemporelles->ecart_type[k] = 0;

    NoeudSeriesTemporelles *temp = seriesTemporelles->tete[k];
    int count = 0;

    while(temp != NULL)
    {
      seriesTemporelles->moyenne[k] += temp->valeur;

      if(seriesTemporelles->min[k] > temp->valeur) //cette facon de retrouver le min et le max n'est pas optimale car la liste est deja triée si on utilise AjoutEnOrdre()
        seriesTemporelles->min[k] = temp->valeur;//de toute facon le script est obligé de parcourir cette boucle pour calculer la moyenne 

      if(seriesTemporelles->max[k] < temp->valeur)
        seriesTemporelles->max[k] = temp->valeur;

      if(count == seriesTemporelles->nodes_number[k] / 2)
        seriesTemporelles->mediane[k] = temp->valeur;

      if(count == seriesTemporelles->nodes_number[k] / 4)
        seriesTemporelles->quartile1[k] = temp->valeur;

      if(count == 3 * seriesTemporelles->nodes_number[k] / 4)
        seriesTemporelles->quartile3[k] = temp->valeur;

      temp = temp->next;
      count++;
    }
    seriesTemporelles->moyenne[k] /= seriesTemporelles->nodes_number[k];
    temp = seriesTemporelles->tete[k];
    float pas = (seriesTemporelles->max[k] - seriesTemporelles->min[k]) / BINS;
    while(temp != NULL)
    {
      int coord = (temp->valeur - seriesTemporelles->min[k])/pas;
      if(coord > BINS -1 ) 
        coord = BINS - 1;
      if(coord < 0) 
        coord = 0;
      seriesTemporelles->frequenceBins[k][coord] += (float) 1 / seriesTemporelles->nodes_number[k];
      seriesTemporelles->ecart_type[k] += pow(temp->valeur - seriesTemporelles->moyenne[k], 2);
      if(seriesTemporelles->frequenceBins[k][coord] > 1)
        seriesTemporelles->frequenceBins[k][coord] = 1;
      temp = temp->next;
    }
    seriesTemporelles->ecart_type[k] = pow(seriesTemporelles->ecart_type[k]/seriesTemporelles->nodes_number[k],0.5);
  }
}

void CalculsParWafer(Wafers *wafers)
{
  SeriesTemporelles *seriesTemporelles = wafers->tete;
  while(seriesTemporelles != NULL)
  {
    for(int k = 0; k < NUMBER_OF_COLUMNS; k++)
    {
      for(int p = 0; p < BINS; p++)
      {
        seriesTemporelles->frequenceBins[k][p] = 0;
      }
      seriesTemporelles->moyenne[k] = 0;
      seriesTemporelles->min[k] = INFINITY_AND_BEYOND;
      seriesTemporelles->max[k] = -INFINITY_AND_BEYOND;
      seriesTemporelles->ecart_type[k] = 0;

      NoeudSeriesTemporelles *temp = seriesTemporelles->tete[k];
      int count = 0;

      while(temp != NULL)
      {
        seriesTemporelles->moyenne[k] += temp->valeur;

        if(seriesTemporelles->min[k] > temp->valeur) //cette facon de retrouver le min et le max n'est pas optimale si la liste est deja triée 
          seriesTemporelles->min[k] = temp->valeur;//de toute facon le script est obligé de parcourir cette boucle pour calculer la moyenne 

        if(seriesTemporelles->max[k] < temp->valeur)
          seriesTemporelles->max[k] = temp->valeur;

        if(count == seriesTemporelles->nodes_number[k] / 2)
          seriesTemporelles->mediane[k] = temp->valeur;

        if(count == seriesTemporelles->nodes_number[k] / 4)
          seriesTemporelles->quartile1[k] = temp->valeur;

        if(count == 3 * seriesTemporelles->nodes_number[k] / 4)
          seriesTemporelles->quartile3[k] = temp->valeur;

        temp = temp->next;
        count++;
      }
      seriesTemporelles->moyenne[k] /= seriesTemporelles->nodes_number[k];
      temp = seriesTemporelles->tete[k];
      float pas = (seriesTemporelles->max[k] - seriesTemporelles->min[k]) / BINS;
      while(temp != NULL)
      {
        int coord = (temp->valeur - seriesTemporelles->min[k])/pas;
        if(coord > BINS -1 ) coord = BINS - 1;
        if(coord < 0) coord = 0;
        seriesTemporelles->frequenceBins[k][coord] += (float) 1 / seriesTemporelles->nodes_number[k];
        seriesTemporelles->ecart_type[k] += pow(temp->valeur - seriesTemporelles->moyenne[k], 2);
        temp = temp->next;
      }
      seriesTemporelles->ecart_type[k] = pow(seriesTemporelles->ecart_type[k]/seriesTemporelles->nodes_number[k],0.5);
    }
    seriesTemporelles = seriesTemporelles->next;
  }
}

void SupprimeAtypiques(SeriesTemporelles *seriesTemporelles)
{
  if(seriesTemporelles->moyenne[0] == 0)
  {
    printf("\nIl faut d'abord lire au moins un fichier\n");
    return;
  }
  for(int k = 0; k < NUMBER_OF_COLUMNS; k++)
  {
    NoeudSeriesTemporelles *present = seriesTemporelles->tete[k];
    NoeudSeriesTemporelles *ancien = seriesTemporelles->tete[k];
    while(present != NULL)
    {
      if(present->valeur > seriesTemporelles->moyenne[k] + coeffDevantSigma *seriesTemporelles->ecart_type[k] || present->valeur < seriesTemporelles->moyenne[k] - coeffDevantSigma * seriesTemporelles->ecart_type[k])
      {
        if(present == seriesTemporelles->tete[k])
        {
          seriesTemporelles->tete[k] = present->next;
          continue;
        }
        ancien->next = present->next;
        present = ancien->next;
        seriesTemporelles->nodes_number[k]--;
        continue;
      }
      ancien = present;
      present = present->next;
    }
  }


}

void afficherCsv(SeriesTemporelles *seriesTemporelles, int colonne)
{
  NoeudSeriesTemporelles *temp = seriesTemporelles->tete[colonne];
  while(temp != NULL)
  {
    printf("%f  ",temp->valeur);
    temp = temp->next;
    printf("\n");
  }

}

void LireToutLesFic(Wafers *wafers, SeriesTemporelles *statsGeneraux)
{
  char nom[25] = "CMP-training-000.csv";
  for(int k = 0; k < 6; k++)
  {
    nom[14] = (char) (k + 48) ; //Conversion ascii (on veut avoir le numero k en char)
    for(int p = 0; p < 10; p++)
    {
      if(k == 5 && p > 8)
      {
        printf("Tous les fichiers ont ete lus avec succes\n");
        return;
      }
      nom[15] = (char) (p + 48); //Conversion ascii (on veut avoir le numero p en char)
      printf("%s est en train d'etre lu\n", nom);
      char nomComplet[25] = "Data/";
      strcat(nomComplet,nom);
      LireFic(wafers,statsGeneraux,nomComplet);
    }
  }

}

void ExportationGeneraux(SeriesTemporelles *statsGeneraux)
{
  FILE *fic;
  if(suppressionDonneesAtypiquesParWafer)
  {
    fic = fopen("StatistiquesGenerauxApresSupressionAtypiques.txt","w");
  }
  else
  {
    fic = fopen("StatistiquesGenerauxAvantSupressionAtypiques.txt","w");
  }
  char nomDonnee[35][35] ={"Usage of backing film","Usage of dresser","Usage of polishing table","Usage of dresser table","Pressurized chamber pressure","Main outer air bag pressure","Center air bag pressure","Retainer ring pressure","Ripple air bag pressure","Usage of membrane","Usage of pressurized sheet","Slurry flow line A", "Slurry flow line B", "Slurry flow line C","Wafer rotation","Stage rotation","Head rotation","Edge air bag pressure"};
  if(fic == NULL)
  {
    printf("Erreur a l'ouverture \n");
    return;
  }
  for(int k = 0; k < NUMBER_OF_COLUMNS; k++)
  {
    fprintf(fic,"  \n\n%s\n\n    ",nomDonnee[k]);
    fprintf(fic,"Moyenne: %f\n   ",statsGeneraux->moyenne[k]);
    fprintf(fic,"Mediane: %f\n   ",statsGeneraux->mediane[k]);
    fprintf(fic,"Quartile 1: %f\n   ",statsGeneraux->quartile1[k]);
    fprintf(fic,"Quartile 3: %f\n   ",statsGeneraux->quartile3[k]);
    fprintf(fic,"Maximum: %f\n   ",statsGeneraux->max[k]);
    fprintf(fic,"Minimum: %f\n   ",statsGeneraux->min[k]);
    fprintf(fic,"Ecart-type: %f\n   ",statsGeneraux->ecart_type[k]);
    fprintf(fic,"\n\n   Distribution des bins\n");
    for(int p = 0; p < BINS; p++)
    {
      fprintf(fic,"    Bin %d: %f \n",p,statsGeneraux->frequenceBins[k][p]);
    }

  }
  fclose(fic);
  printf("Exportation reussie dans le fichier StatistiquesGeneraux.txt\n");
}

void ExportationParWafer(Wafers *wafers)
{
  FILE *fic;
  SeriesTemporelles *temp = wafers->tete;
  char nomDonnee[35][35] ={"Usage of backing film","Usage of dresser","Usage of polishing table","Usage of dresser table","Pressurized chamber pressure","Main outer air bag pressure","Center air bag pressure","Retainer ring pressure","Ripple air bag pressure","Usage of membrane","Usage of pressurized sheet","Slurry flow line A", "Slurry flow line B", "Slurry flow line C","Wafer rotation","Stage rotation","Head rotation","Edge air bag pressure"};
  if(suppressionDonneesAtypiquesParWafer)
  {
    fic = fopen("StatistiquesParWaferApresSupressionAtypiques.txt","w");
  }
  else
  {
    fic = fopen("StatistiquesParWaferAvantSupressionAtypiques.txt","w");
  }
  if(fic == NULL)
  {
    printf("Erreur a l'ouverture \n");
    return;
  }
  while(temp != NULL)
  {
    fprintf(fic,"\nWafer: %ld",temp->wafer_id);
    for(int k = 0; k < NUMBER_OF_COLUMNS; k++)
    {
      fprintf(fic,"  \n\n%s\n\n   ",nomDonnee[k]);
      fprintf(fic,"Moyenne: %f\n   ",temp->moyenne[k]);
      fprintf(fic,"Mediane: %f\n   ",temp->mediane[k]);
      fprintf(fic,"Quartile 1: %f\n   ",temp->quartile1[k]);
      fprintf(fic,"Quartile 3: %f\n   ",temp->quartile3[k]);
      fprintf(fic,"Maximum: %f\n   ",temp->max[k]);
      fprintf(fic,"Minimum: %f\n   ",temp->min[k]);
      fprintf(fic,"Ecart-type: %f\n   ",temp->ecart_type[k]);
      fprintf(fic,"\n\n   Distribution des bins\n    ");
      for(int p = 0; p < BINS; p++)
      {
        fprintf(fic,"Bin %d: %f \n    ",p,temp->frequenceBins[k][p]);
      }
    }
    temp = temp->next;
  }
  fclose(fic);
  printf("Exportation reussie dans le fichier StatistiquesParWafer.txt\n");
}

void Fusionner(float tab[], int deb, int fin1, int fin)
{
  int l1,l2;
  float tab1[fin];
  l1=deb;
  l2=fin1+1;
  for(int k=deb;k<=fin;k++)
  {
    if(l1>fin1 && l2<=fin)
    {
      tab1[k]=tab[l2];
      l2++;
      continue;
    }
    if(l2>fin && l1<=fin1)
    {
      tab1[k]=tab[l1];
      l1++;
      continue;
    }
    if(l2<=fin && l1<=fin1 && tab[l1]>tab[l2])
    {
      tab1[k]=tab[l1];
        l1++;
      continue;
    }
    if(l1<=fin1 && l2<=fin && tab[l2]>=tab[l1])
    {
      tab1[k]=tab[l2];
      l2++;
      continue;
    }
  }
  for(int p=deb;p<=fin;p++)
  {
    tab[p]=tab1[p];
  }
}

void Trier (float tab[], int deb, int fin)
{
  if(fin-deb>1)
  {
    Trier(tab,deb,(deb+fin)/2);
    Trier(tab,(deb+fin)/2+1,fin);
    Fusionner(tab,deb,(deb+fin)/2,fin);
  }
  if(fin-deb==1)
    Fusionner(tab,deb,deb,fin);
  return;
}

void FusionParWafer(Wafers *wafers)
{
  SeriesTemporelles *temp1 = wafers->tete;
  while(temp1 != NULL)
  {
    for(int k = 0; k < NUMBER_OF_COLUMNS; k++)
    {
      int count = 0;
      float *tempArray = malloc(sizeof(float) * temp1->nodes_number[k]);
      NoeudSeriesTemporelles *temp = temp1->tete[k];
      while(temp != NULL)
      {
        tempArray[count] = temp->valeur;
        temp = temp->next;
        count++;
      }
      Trier(tempArray,0,temp1->nodes_number[k] - 1);
    
      temp = temp1->tete[k];
      count = 0;
      while(temp != NULL)
      {
        temp->valeur = tempArray[count];
        temp = temp->next;
        count++;
      }
      free(tempArray);
    }
    temp1 = temp1->next;
  }
}

void FusionStatsGeneraux(SeriesTemporelles *statsGeneraux)
{
  for(int k = 0; k < NUMBER_OF_COLUMNS; k++)
  {
    int count = 0;
    float *tempArray = malloc(sizeof(float) * statsGeneraux->nodes_number[k]);
    NoeudSeriesTemporelles *temp = statsGeneraux->tete[k];
    while(temp != NULL)
    {
      tempArray[count] = temp->valeur;
      temp = temp->next;
      count++;
    }
    Trier(tempArray,0,statsGeneraux->nodes_number[k] - 1);
    
    temp = statsGeneraux->tete[k];
    count = 0;
    while(temp != NULL)
    {
      temp->valeur = tempArray[count];
      temp = temp->next;
      count++;
    }
    free(tempArray);
  }
}

void Menu()
{
  int choix = 0;
  char nomFic[30];
  Wafers wafers;
  InitWafers(&wafers);
  SeriesTemporelles statsGeneraux;
  InitSeriesTemporelles(&statsGeneraux);
  SeriesTemporelles *temp = wafers.tete;
  
  while(1)
  {
    temp = wafers.tete;
    printf("\n\n1 - Lire tout les fichiers \n2 - Lire un seul fichier\n3 - Enlever les observations atypiques pour les statistiques generaux\n4 - Enlever les observations atypiques pour les statistiques par wafer\n5 - Exporter les statistiques generaux\n6 - Exporter les statistiques par wafer \n7 - Quitter\n");
    scanf("%d",&choix);
    switch(choix)
    {
      case 1:
      LireToutLesFic(&wafers, &statsGeneraux);
      printf("\nLes liste chaines sont en train d'etre tries. Cette operation peut prendre plusieurs minutes\n");
      FusionStatsGeneraux(&statsGeneraux);
      FusionParWafer(&wafers);
      CalculsStatsGeneraux(&statsGeneraux);
      CalculsParWafer(&wafers);
      printf("\nTri reussi\n");
      break;

      case 2:
      printf("Endroit du fichier? \n");
      scanf("%s",nomFic);
      LireFic(&wafers,&statsGeneraux,nomFic);
      FusionStatsGeneraux(&statsGeneraux);
      FusionParWafer(&wafers);
      CalculsStatsGeneraux(&statsGeneraux);
      CalculsParWafer(&wafers);
      break;

      case 3:
      SupprimeAtypiques(&statsGeneraux);
      CalculsStatsGeneraux(&statsGeneraux);
      supressionDonneesAtypiquesGeneraux = true;
      printf("Supression des donnes reussie\n");
      break;

      case 4:
      while (temp != NULL)
      {
        SupprimeAtypiques(temp);
        CalculsStatsGeneraux(temp);
        temp = temp->next;
      }
      suppressionDonneesAtypiquesParWafer = true;
      printf("Supression des donnes reussie\n");
      break;

      case 5:
      ExportationGeneraux(&statsGeneraux);
      break;

      case 6:
      ExportationParWafer(&wafers);
      break;

      case 7:
      return;
      break;

    }
  }
}










int main()
{

  Menu();
}

