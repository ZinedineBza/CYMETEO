#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>  // Pour les fichiers en arguments
#include <string.h>     // Pour strcmp (--abr et tout en argument)
#include <unistd.h>     // Pour acces (verifie si on a le droit de lecture sur un fichier en argument)
#include <math.h>
#define BUFFER_SIZE 512

//----------------------------------------ABR-------------------------
typedef struct arbre{       //structure de l'arbre binaire de recherche
    float c1;
    float c2;
    float c3;
    float c4;
    float c5;
    struct arbre* fg;
    struct arbre* fd;
    struct arbre* fm;
    int equilibre;
    int hauteur;
}Arbre;

typedef Arbre* pArbre;

pArbre creerArbre(float a1,float a2,float a3,float a4,float a5);        //creation d'un ABR
pArbre insertionABR(pArbre a,float a1, float a2,float a3,float a4,float a5);     //insertion d'un noeud dans un ABR selon la colonne1
void parcoursInfixe(pArbre a, FILE* f);        //Ecris dans un fichier de sortie les valeurs des noeuds de l'ABR dans l'ordre croissant par rapport à la colonne 1
void parcoursInfixeinverse(pArbre a, FILE* f);      //Ecris dans un fichier de sortie les valeurs des noeuds de l'ABR dans l'ordre décroissant par rapport à la colonne 1

//---------------------------------------Chainon-----------------------------
typedef struct chainon{         //structure Chainon
    float c1;
    float c2;
    float c3;
    float c4;
    float c5;
    struct chainon* suivant;
}Chainon;

Chainon *creationChainon(float a1,float a2,float a3,float a4,float a5);     //creation d'un chainon
Chainon * insertDebut(Chainon* pliste,float a1,float a2,float a3,float a4,float a5);        //insertion d'un noeud en debut d'une liste chainée
Chainon *ajouterCroissant(Chainon *pliste, float a1, float a2,float a3,float a4,float a5);  //ajoute dans la liste dans l'ordre croissant du premier element du chainon
Chainon *ajouterCroissantinverse(Chainon *pliste, float a1, float a2,float a3,float a4,float a5);   //ajoute dans la liste dans l'ordre décroissant du premier element du chainon

//------------------------------------------AVL----------------------------------
int max(int a, int b);          //retourne le maximum entre deux entiers
int min(int a, int b);          //retourne le minimum entre deux entiers
pArbre rotationGauche(pArbre a);        //rotation gauche du noeud
pArbre rotationDroite(pArbre a);        //rotation droite du noeud
pArbre doubleRotationGauche(pArbre a);      //double rotation gauche du noeud
pArbre doubleRotationDroite(pArbre a);      //double rotation droite du noeud
pArbre equilibrerAVL(pArbre a);             //equilibrage d'un AVL
pArbre insertionAVL(pArbre a,float a1, float a2,float a3,float a4,float a5, int* h);        //insere d'un noeud dans un AVL dans l'ordre croissant par rapport à la colonne 1 du noeud

//---------------------------------------------o p t i o n T3-----------------------
//----------------------------------------AVL t3-------------------------
typedef struct arbret3{     //structure AVL option t3
    struct arbret3* fg;
    struct arbret3* fd;
    struct arbret3*fm;
    char* c1;
    float c2;
    float c3;
    float c4;
    float c5;
    int equilibre;
    int hauteur;
}Arbret3;

typedef Arbret3* pArbret3;

pArbret3 creerArbret3(char* c1, float c2, float c3, float c4, float c5);        //creation d'un AVL
pArbret3 rotationGauchet3(pArbret3 a);          //rotation gauche de a
pArbret3 rotationDroitet3(pArbret3 a);      //rotation droite de a
pArbret3 doubleRotationGauchet3(pArbret3 a);        //double rotation gauche de a
pArbret3 doubleRotationDroitet3(pArbret3 a);        //double rotation droite de a
pArbret3 equilibrerAVLt3(pArbret3 a);           //equilibrage de l'AVL
pArbret3 insertionAVLt3v2(pArbret3 a,char* a1, float a2,float a3,float a4,float a5, int* h);        //insere dans l'arbre par rapport à la colonne 2 d'un noeud
pArbret3 insertionAVLt3AVL(pArbret3 a,char* a1,float a2,float a3,float a4,float a5, int* h);    //insere dans l'arbre par rapport à la colonne 1 (date)
void parcoursInfixet3avl(pArbret3 a, FILE* f);          //affichage avl dans l'ordre croissant
void parcoursInfixet3avlinverse(pArbret3 a, FILE* f);           //ecris dans le fichier de sorti des noeuds dans l'odre décroissant de la colonne 1(date)

//-----------------------------------------t3 abr------------------------------------------------------------

pArbret3 insertionABRt3v2(pArbret3 a,char* a1, float a2,float a3,float a4,float a5);     //insertion d'un noeud dans l'abr selon la colonne 2
pArbret3 insertionABRt3(pArbret3 a,char* a1, float a2,float a3,float a4,float a5);       //insertion d'un noeud dans l'abr selon la date (colonne 1)
void parcoursInfixet3abr(pArbret3 a, FILE* f);              //Ecris sur le fichier de sortie des valeurs des noeuds dans l'ordre croissant par rapport à la colonne 1
void parcoursInfixet3abrinverse(pArbret3 a, FILE* f);           ////Ecris sur le fichier de sortie des valeurs des noeuds dans l'ordre decroissant par rapport à la colonne 1

//----------------------------------------t3 chainon----------------------------------------------
typedef struct chainont3{       //structure de chainon pour les options t3
    char* c1;
    float c2;
    float c3;
    float c4;
    float c5;
    struct chainont3* suivant;
}Chainont3;

Chainont3 *creationChainont3(char *a1,float a2,float a3,float a4,float a5);
Chainont3 * insertDebutt3(Chainont3* pliste,char *a1,float a2,float a3,float a4,float a5);      //insertion du chainon en debut de liste
Chainont3 *ajouterCroissantt3boss(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5);        //insertion du chainon dans une liste dans l'ordre de l'option t3
Chainont3 *ajouterCroissantt3inverseboss(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5);         //insertion du chainon selon l'option -t3 -r

//-------------------------------------------------t2 avl---------------------------------------
pArbret3 insertionAVLt2AVL(pArbret3 a,char* a1,float a2,float a3,float a4,float a5, int* h);        //insertion d'un noeud dans l'AVL selon l'option t2

//------------------------------------------------------ abr t2-----------------
pArbret3 insertionABRt2(pArbret3 a,char* a1, float a2,float a3,float a4,float a5);               //insertion d'un noeud dans un ABR selon l'otpion t2

//----------------------------------------------------- t2 tab-------------------------

Chainont3 *ajouterCroissantt2(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5);        //insertion du chainon dans la liste selon l'option t2
Chainont3 *ajouterCroissantt2inverse(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5);             //insertion du chainon dans la liste selon l'option -t2 -r
