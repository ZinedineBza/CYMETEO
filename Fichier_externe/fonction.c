#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>  // Pour les fichiers en arguments
#include <string.h>     // Pour strcmp (--abr et tout en argument)
#include <unistd.h>     // Pour acces (verifie si on a le droit de lecture sur un fichier en argument)
#include <math.h>
#define BUFFER_SIZE 512
#include "fonction.h"

//----------------------------------------ABR-------------------------
pArbre creerArbre(float a1,float a2,float a3,float a4,float a5){        //creation d'un ABR
    pArbre nouveau = malloc(sizeof(Arbre));
    if (nouveau==NULL){
        exit(4);
    }
    nouveau->c1=a1;
    nouveau->c2=a2;
    nouveau->c3=a3;
    nouveau->c4=a4;
    nouveau->c5=a5;
    nouveau->fg=NULL;
    nouveau->fd=NULL;
    nouveau->fm=NULL;
    nouveau->equilibre=0;
    return nouveau;
}

pArbre insertionABR(pArbre a,float a1, float a2,float a3,float a4,float a5)     //insertion d'un noeud dans un ABR selon la colonne1
{
    if(a==NULL)                             
    {
        return a=creerArbre(a1,a2,a3,a4,a5);
    }
    else if(a1 <a->c1)                                      //SI l'element à inserer doit etre dans le fils gauche de a
    {
        a->fg=insertionABR(a->fg,a1,a2,a3,a4,a5);
    }
    else if(a1 > a->c1)                                     //SI l'element à inserer doit etre dans le fils droit de a
    {
        a->fd=insertionABR(a->fd,a1,a2,a3,a4,a5);
    }
    else                                                    //SI il y a un doublon
    {
        if(a->fg!=NULL)                                 //SI il existe fils gauche de a, on insere le noeud entre a et son fils gauche
        {
            pArbre doublon=creerArbre(a1,a2,a3,a4,a5);
            doublon->fg=a->fg;
            a->fg=doublon;
        }
        else                                            //si il n'existe pas de fils gauche à a, on insere le noeud en tant que fils gauche de a
        {
            pArbre doublon=creerArbre(a1,a2,a3,a4,a5);
            a->fg=doublon;
        }
    }
    return a;
}

void parcoursInfixe(pArbre a, FILE* f) {        //Ecris dans un fichier de sortie les valeurs des noeuds de l'ABR dans l'ordre croissant par rapport à la colonne 1
    if(a!=NULL) 
    {
        parcoursInfixe(a->fg,f);
        fprintf(f, "%f %f %f %f %f\n", a->c1,a->c2,a->c3,a->c4,a->c5);
        parcoursInfixe(a->fd,f);
    }
}
void parcoursInfixeinverse(pArbre a, FILE* f) {     //Ecris dans un fichier de sortie les valeurs des noeuds de l'ABR dans l'ordre décroissant par rapport à la colonne 1
    if(a!=NULL) 
    {
        parcoursInfixeinverse(a->fd,f);
        fprintf(f, "%f %f %f %f %f\n", a->c1,a->c2,a->c3,a->c4,a->c5);
        parcoursInfixeinverse(a->fg,f);
    }
}

//---------------------------------------Chainon-----------------------------

Chainon *creationChainon(float a1,float a2,float a3,float a4,float a5){     //creation d'un chainon
    Chainon * c = (Chainon *) malloc(sizeof(Chainon));
    if(c==NULL){
        exit(4);
    }
    c->c1=a1;
    c->c2=a2;
    c->c3=a3;
    c->c4=a4;
    c->c5=a5;
    c->suivant=NULL;
    return c;
}

Chainon * insertDebut(Chainon* pliste,float a1,float a2,float a3,float a4,float a5){        //insertion d'un noeud en debut d'une liste chainée
    Chainon* nouveau=creationChainon(a1,a2,a3,a4,a5);
    nouveau -> suivant = pliste;
    return nouveau;
}

Chainon *ajouterCroissant(Chainon *pliste, float a1, float a2,float a3,float a4,float a5) { //ajoute dans la liste dans l'ordre croissant du premier element du chainon
    Chainon *nouveau=creationChainon(a1,a2,a3,a4,a5);
    Chainon *p1 =pliste;
    if(pliste==NULL){// si la liste est vide
        pliste=nouveau;
    }
    else if (pliste->c1 > a1) { // dans ce cas il faut placer l'element au début
        pliste=insertDebut(pliste,a1,a2,a3,a4,a5);
    }
    else{
        //on parcours la liste jusqu'à trouver où doit aller le nouveau élément
        while(p1->suivant!= NULL && p1->suivant->c1 < a1){ 
            p1=p1->suivant;
        }
        if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
            p1->suivant=nouveau;
        }
        else{ // il faut inserer le maillon en millieu de chaîne, après p1
            nouveau->suivant=p1->suivant;
            p1->suivant = nouveau;
        }
    }
    return pliste;
}

Chainon *ajouterCroissantinverse(Chainon *pliste, float a1, float a2,float a3,float a4,float a5) {  //ajoute dans la liste dans l'ordre décroissant du premier element du chainon
    Chainon *nouveau=creationChainon(a1,a2,a3,a4,a5);
    Chainon *p1 =pliste;
    if(pliste==NULL){// si la liste est vide
        pliste=nouveau;
    }
    else if (pliste->c1 < a1) { // dans ce cas il faut placer l'element au début
        pliste=insertDebut(pliste,a1,a2,a3,a4,a5);
    }
    else{
        //on parcours la liste jusqu'à trouver où doit aller le nouveau élément
        while(p1->suivant!= NULL && p1->suivant->c1 > a1){ 
            p1=p1->suivant;
        }
        if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
            p1->suivant=nouveau;
        }
        else{ // il faut inserer le maillon en millieu de chaîne, après p1
            nouveau->suivant=p1->suivant;
            p1->suivant = nouveau;
        }
    }
    return pliste;
}
//------------------------------------------AVL----------------------------------
int max(int a, int b) {         //retourne le maximum entre deux entiers
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

int min(int a, int b) {         //retourne le minimum entre deux tniers
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

pArbre rotationGauche(pArbre a) {       //rotation gauche du noeud
    pArbre pivot=a->fd;
    a->fd=pivot->fg;
    pivot->fg=a;
    int eq_a=a->equilibre;
    int eq_p=pivot->equilibre;

   a->equilibre = eq_a - max(eq_p,0) - 1;
   pivot->equilibre = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));     
   a = pivot;

   return a;
}

pArbre rotationDroite(pArbre a) {       //rotation droite du noeud
   pArbre pivot=a->fg;
    a->fg=pivot->fd;
    pivot->fd=a;
    int eq_a=a->equilibre;
    int eq_p=pivot->equilibre;
   a->equilibre = eq_a - min(eq_p, 0) + 1;
   pivot->equilibre = max(eq_a + 2, max(eq_a + eq_p + 2, eq_p + 1));
   a = pivot;

   return a;
}

pArbre doubleRotationGauche(pArbre a) {     //double rotation gauche du noeud
   a->fd = rotationDroite(a->fd);           //rotation droite du fils droit de a
   return rotationGauche(a);                //rotation gauche de a
}

pArbre doubleRotationDroite(pArbre a) {     //double rotation droite du noeud
   a->fg = rotationGauche(a->fg);           //rotation gauche du fils droit de a
   return rotationDroite(a);                //rotation droite de a
}

pArbre equilibrerAVL(pArbre a) {            //equilibrage d'un AVL
   if (a->equilibre >= 2) {
      // sous-arbre droit plus profond
      if (a->fd->equilibre >= 0) {
         return rotationGauche(a);
      } else {
         return doubleRotationGauche(a);
      }
   } else if (a->equilibre <= -2) {
      // sous-arbre gauche plus profond
      if (a->fg->equilibre <= 0) {
         return rotationDroite(a);
      } else {
         return doubleRotationDroite(a);
      }
   }

   return a;
}
pArbre insertionAVL(pArbre a,float a1, float a2,float a3,float a4,float a5, int* h) {       //insere d'un noeud dans un AVL dans l'ordre croissant par rapport à la colonne 1 du noeud
    if (a == NULL) 
    {
        *h = 1;
        return creerArbre(a1,a2,a3,a4,a5);
    }
    else if (a1 < a->c1)        //si le noeud doit etre inserer dans le fils gauche de a 
    {
        a->fg = insertionAVL(a->fg,a1,a2,a3,a4,a5, h);
        *h = -*h;
    } 
    else if (a1 > a->c1)        //si le noeud doit etre inserer dans le fils droit de a 
    {
        a->fd = insertionAVL(a->fd,a1,a2,a3,a4,a5, h);
    } 
    else                        //si il y a un doublon
    {
        *h=1;
        if(a->fg!=NULL)     //si il extiste un fils gauche à a, on insere le noeud entre les deux
        {
            pArbre doublon=creerArbre(a1,a2,a3,a4,a5);
            doublon->fg=a->fg;
            a->fg=doublon;
        }
        else                //si il n'y a pas de fils gauche, on insere le noeud en tant que fils gauche de a
        {
            pArbre doublon=creerArbre(a1,a2,a3,a4,a5);
            a->fg=doublon;
        }
        return a;
    }

   if (*h != 0)             //equilibrage de l'arbre
   {
      a->equilibre = a->equilibre + *h;
      a=equilibrerAVL(a);
      if (a->equilibre == 0) 
      {
         *h = 0;
      } 
    else 
    {
        *h = 1;
    }
   }

   return a;
}
//---------------------------------------------o p t i o n T3-----------------------
//----------------------------------------AVL t3-------------------------

pArbret3 creerArbret3(char* c1, float c2, float c3, float c4, float c5){        //creation d'un AVL
    pArbret3 nouveau = malloc(sizeof(Arbret3));
    if (nouveau==NULL){
        exit(4);
    }
    nouveau->fg=NULL;
    nouveau->fd=NULL;
    nouveau -> fm=NULL;
    nouveau->c1=malloc(sizeof(char)*128);
    if(nouveau->c1==NULL){
        exit(4);
    }
    strcpy(nouveau->c1,c1);
    nouveau->c2=c2;
    nouveau->c3=c3;
    nouveau->c4=c4;
    nouveau->c5=c5;
    nouveau->equilibre=0;
    return nouveau;
}

pArbret3 rotationGauchet3(pArbret3 a) {         //rotation gauche de a
    pArbret3 pivot=a->fd;
    a->fd=pivot->fg;
    pivot->fg=a;
    int eq_a=a->equilibre;
    int eq_p=pivot->equilibre;

   a->equilibre = eq_a - max(eq_p,0) - 1;
   pivot->equilibre = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));     
   a = pivot;
   return a;
}

pArbret3 rotationDroitet3(pArbret3 a) {     //rotation droite de a
   pArbret3 pivot=a->fg;
    a->fg=pivot->fd;
    pivot->fd=a;
    int eq_a=a->equilibre;
    int eq_p=pivot->equilibre;
   a->equilibre = eq_a - min(eq_p, 0) + 1;
   pivot->equilibre = max(eq_a + 2, max(eq_a + eq_p + 2, eq_p + 1));
   a = pivot;

   return a;
}

pArbret3 doubleRotationGauchet3(pArbret3 a) {       //double rotation gauche de a
   a->fd = rotationDroitet3(a->fd);
   return rotationGauchet3(a);
}

pArbret3 doubleRotationDroitet3(pArbret3 a) {       //double rotation droite de a
   a->fg = rotationGauchet3(a->fg);
   return rotationDroitet3(a);
}

pArbret3 equilibrerAVLt3(pArbret3 a) {          //equilibrage de l'AVL
   if (a->equilibre >= 2) {
      // sous-arbre droit plus profond
      if (a->fd->equilibre >= 0) {
         return rotationGauchet3(a);
      } else {
         return doubleRotationGauchet3(a);
      }
   } else if (a->equilibre <= -2) {
      // sous-arbre gauche plus profond
      if (a->fg->equilibre <= 0) {
         return rotationDroitet3(a);
      } else {
         return doubleRotationDroitet3(a);
      }
   }

   return a;
}

pArbret3 insertionAVLt3v2(pArbret3 a,char* a1, float a2,float a3,float a4,float a5, int* h) {       //insere dans l'arbre par rapport à la colonne 2 d'un noeud
    if (a == NULL) 
    {
        *h = 1;
        return creerArbret3(a1,a2,a3,a4,a5);
    }
    else if (a2 < a->c2)        //insertion du noeud dans le fils gauche de a
    {
        a->fg = insertionAVLt3v2(a->fg,a1,a2,a3,a4,a5, h);
        *h = -*h;
    } 
    else if (a2 > a->c2)        //insertion du noeud dans le fils droite de a
    {
        a->fd = insertionAVLt3v2(a->fd,a1,a2,a3,a4,a5, h);
    } 
    else                        // si il y a un doublon du noeud par rapport à la colonne 2
    {
        *h = 1;
         if(a->fd!=NULL)            //insertion du noeud entre a et son fils droit
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);
            doublon->fd=a->fd;
            a->fd=doublon;
        }
        else                        //si il n'existe pas de fils droit à a, on insere le noeud en tant que fils droit de a
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);
            a->fd=doublon;
        }
        return a;
    }

   if (*h != 0)         //equilibrage de l'arbre
   {
      a->equilibre = a->equilibre + *h;
      a=equilibrerAVLt3(a);
      if (a->equilibre == 0) 
      {
         *h = 0;
      } 
    else 
    {
        *h = 1;
    }
   }

   return a;
}

pArbret3 insertionAVLt3AVL(pArbret3 a,char* a1,float a2,float a3,float a4,float a5, int* h) {   //insere dans l'arbre par rapport à la colonne 1 (date)
    if (a == NULL) 
    {
        *h = 1;
        return a=creerArbret3(a1,a2,a3,a4,a5);
    }
    else if (strcmp(a1,a->c1)<0)        // si la date du noeud à inserer est + ancienne que celle du noeud
    {
        a->fg = insertionAVLt3AVL(a->fg,a1,a2,a3,a4,a5, h);
        *h = -(*h);
    } 
    else if (strcmp(a1,a->c1)>0)        // si la date du noeud à inserer est + récente que celle du noeud
    {
        a->fd = insertionAVLt3AVL(a->fd,a1,a2,a3,a4,a5, h);
    } 
    else 
    {
        *h = 0;                       
        a->fm=insertionAVLt3v2(a->fm,a1,a2,a3,a4,a5,h);         //si doublon, on insere dans l'arbre du fils du milieu dans l'ordre croissant par rapport à la colonne 2
        return a;
    }

   if (*h != 0)                 //si il faut réequilibrer l'arbre
   {
      a->equilibre = a->equilibre + *h;
      a=equilibrerAVLt3(a);
      if (a->equilibre == 0) 
      {
         *h = 0;
      } 
    else 
    {
        *h = 1;
    }
   }

   return a;
}

void parcoursInfixet3avl(pArbret3 a, FILE* f) {         //affichage avl dans l'ordre croissant
    if(a!=NULL) 
    {
        int h=0;
        parcoursInfixet3avl(a->fg,f);
        if(a->fm==NULL){                                                                    // si pas de doublons
            fprintf(f, "%s %f %f %f %f\n", a->c1,a->c2,a->c3,a->c4,a->c5);
        }
        else{                                                                               //si il y a des doublons de ce noeud
            a->fm=insertionAVLt3v2(a->fm,a->c1,a->c2,a->c3,a->c4,a->c5,&h);     
            parcoursInfixet3avl(a->fm,f);                                                 //affichage des doublons dans l'ordre croissant de la 2eme valeurs
        }
        parcoursInfixet3avl(a->fd,f);
    }
}

void parcoursInfixet3avlinverse(pArbret3 a, FILE* f) {          //ecris dans le fichier de sorti des noeuds dans l'odre décroissant de la colonne 1(date)
    if(a!=NULL) 
    {
        int h=0;
        parcoursInfixet3avlinverse(a->fd,f);
        if(a->fm==NULL){
            fprintf(f, "%s %f %f %f %f\n", a->c1,a->c2,a->c3,a->c4,a->c5);
        }
        else{
            a->fm=insertionAVLt3v2(a->fm,a->c1,a->c2,a->c3,a->c4,a->c5,&h);
            parcoursInfixet3avlinverse(a->fm,f);                                //ecris dans le fichier de sorti les doublons dans l'ordre decroissant de la 2eme valeurs
        }
        parcoursInfixet3avlinverse(a->fg,f);
    }
}
//-----------------------------------------t3 abr------------------------------------------------------------

pArbret3 insertionABRt3v2(pArbret3 a,char* a1, float a2,float a3,float a4,float a5)     //insertion d'un noeud dans l'abr selon la colonne 2
{
    if(a==NULL)
    {
        return a=creerArbret3(a1,a2,a3,a4,a5);
    }
    else if(a2<a->c2)           //noeud inseré dans le fils gauche 
    {
        a->fg=insertionABRt3v2(a->fg,a1,a2,a3,a4,a5);
    }
    else if(a2>a->c2)           //noeud inseré dans le fils droit
    {
        a->fd=insertionABRt3v2(a->fd,a1,a2,a3,a4,a5);
    }
    else                    // si doublon
    {
        if(a->fg!=NULL)     //SI il existe fils gauche à a,insertion du noeud entre a et le fils gauche de a
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);
            doublon->fg=a->fg;
            a->fg=doublon;
        }
        else                //sinon noeud inseré en tant que fils gauche de a
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);
            a->fg=doublon;
        }
    }
    return a;
}

pArbret3 insertionABRt3(pArbret3 a,char* a1, float a2,float a3,float a4,float a5)       //insertion d'un noeud dans l'abr selon la date (colonne 1)
{
    if(a==NULL)
    {
        return a=creerArbret3(a1,a2,a3,a4,a5);
    }
    else if(strcmp(a1,a->c1)<0)     // si date du noeud est + ancienne que celle de a, on insere le noeud dans la partie gauche de a
    {
        a->fg=insertionABRt3(a->fg,a1,a2,a3,a4,a5);
    }
    else if(strcmp(a1,a->c1)>0)     // si date du noeud est + recente que celle de a, on insere le noeud dans la partie droite de a
    {
        a->fd=insertionABRt3(a->fd,a1,a2,a3,a4,a5);
    }
    else                            // SI c'est la meme date, on insere le noeud dans le fils du milieu de a dans l'ordre de la colonne 2 du noeud
    {
        a->fm=insertionABRt3v2(a->fm,a1,a2,a3,a4,a5);
    }
    return a;
}

void parcoursInfixet3abr(pArbret3 a, FILE* f) {             //Ecris sur le fichier de sortie des valeurs des noeuds dans l'ordre croissant par rapport à la colonne 1
    if(a!=NULL) 
    {
        parcoursInfixet3abr(a->fg,f);
        if(a->fm==NULL){                                    // SI il n'y a pas de doublons par rapport à la date (colonne 1)
            fprintf(f, "%s %f %f %f %f\n", a->c1,a->c2,a->c3,a->c4,a->c5);
        }
        else{                                               //Si il y en a 
            a->fm=insertionABRt3v2(a->fm,a->c1,a->c2,a->c3,a->c4,a->c5);
            parcoursInfixet3abr(a->fm,f);                   //ecris sur le fichier de sortie pour chaque date les valeurs dans l'ordre croissant par rapport à la colonne 2
        }
        parcoursInfixet3abr(a->fd,f);
    }
}

void parcoursInfixet3abrinverse(pArbret3 a, FILE* f) {          ////Ecris sur le fichier de sortie des valeurs des noeuds dans l'ordre decroissant par rapport à la colonne 1
    if(a!=NULL) 
    {
        parcoursInfixet3abrinverse(a->fd,f);
        if(a->fm==NULL){                                    // SI il n'y a pas de doublons par rapport à la date (colonne 1)
            fprintf(f, "%s %f %f %f %f\n", a->c1,a->c2,a->c3,a->c4,a->c5);
        }
        else{                                                  //Si il y en a 
            a->fm=insertionABRt3v2(a->fm,a->c1,a->c2,a->c3,a->c4,a->c5);
            parcoursInfixet3abrinverse(a->fm,f);                //ecris sur le fichier de sortie pour chaque date les valeurs dans l'ordre décroissant par rapport à la colonne 2
        }
        parcoursInfixet3abrinverse(a->fg,f);
    }
}

//----------------------------------------t3 chainon----------------------------------------------
Chainont3 *creationChainont3(char *a1,float a2,float a3,float a4,float a5){
    Chainont3 * c = (Chainont3 *) malloc(sizeof(Chainont3));
    if(c==NULL){
        exit(4);
    }

    c->c1=malloc(sizeof(char)*128);
    if(c->c1==NULL){
        exit(4);
    }
    sprintf(c->c1,"%s",a1);
    c->c2=a2;
    c->c3=a3;
    c->c4=a4;
    c->c5=a5;
    c->suivant=NULL;
    return c;
}
Chainont3 * insertDebutt3(Chainont3* pliste,char *a1,float a2,float a3,float a4,float a5){      //insertion du chainon en debut de liste
    Chainont3* nouveau=creationChainont3(a1,a2,a3,a4,a5);
    nouveau -> suivant = pliste;
    return nouveau;
}
Chainont3 *ajouterCroissantt3boss(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5) {       //insertion du chainon dans une liste dans l'ordre de l'option t3
    Chainont3 *nouveau=creationChainont3(a1,a2,a3,a4,a5);
    Chainont3 *p1 =pliste;
    if(pliste==NULL){// si la liste est vide
        pliste=nouveau;
    }
    else if (strcmp(pliste->c1,a1)<0) { // dans ce cas il faut placer l'element au début
        pliste=insertDebutt3(pliste,a1,a2,a3,a4,a5);
    }
    else{
        //on parcours la liste jusqu'à trouver où doit aller le nouveau élément
        while(p1->suivant!= NULL && strcmp(p1->suivant->c1,a1)>0){ 
            p1=p1->suivant;
        }
        if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
            p1->suivant=nouveau;
        }
        else{ // il faut inserer le maillon en millieu de chaîne, après p1
            while(p1->suivant->c2 < a2 && strcmp(p1->suivant->c1,a1)==0){  //on insere le chainon dans l'ordre croissant par rapport à la colonne 2
                p1=p1->suivant;
            }
            nouveau->suivant=p1->suivant;
            p1->suivant = nouveau;
        }
    }
    return pliste;
}
Chainont3 *ajouterCroissantt3inverseboss(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5) {        //insertion du chainon selon l'option -t3 -r
    Chainont3 *nouveau=creationChainont3(a1,a2,a3,a4,a5);
    Chainont3 *p1 =pliste;
    if(pliste==NULL){// si la liste est vide
        pliste=nouveau;
    }
    else if (strcmp(pliste->c1,a1)>0) { // dans ce cas il faut placer l'element au début
        pliste=insertDebutt3(pliste,a1,a2,a3,a4,a5);
    }
    else{
        //on parcours la liste jusqu'à trouver où doit aller le nouveau élément
        while(p1->suivant!= NULL && strcmp(p1->suivant->c1,a1)<0){ 
            p1=p1->suivant;
        }
        if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
            p1->suivant=nouveau;
        }
        else{ // il faut inserer le maillon en millieu de chaîne, après p1
            while(p1->suivant->c2 > a2 && strcmp(p1->suivant->c1,a1)==0){       //on insere le chainon dans l'ordre décroissant par rapport à la colonne 2
                p1=p1->suivant;
            }
            nouveau->suivant=p1->suivant;
            p1->suivant = nouveau;
        }
    }
    return pliste;
}

//-------------------------------------------------t2 avl---------------------------------------
pArbret3 insertionAVLt2AVL(pArbret3 a,char* a1,float a2,float a3,float a4,float a5, int* h) {       //insertion d'un noeud dans l'AVL selon l'option t2
    if (a == NULL) 
    {
        *h = 1;
        return a=creerArbret3(a1,a2,a3,a4,a5);
    }
    else if (strcmp(a1,a->c1)<0)        //si la date du noeud est + ancienne que celle de a, on regarde dans le fils gauche de a
    {
        a->fg = insertionAVLt3AVL(a->fg,a1,a2,a3,a4,a5, h);
        *h = -(*h);
    } 
    else if (strcmp(a1,a->c1)>0)        //si la date du noeud est + recente que celle de a, on regarde dans le fils droit de a
    {
        a->fd = insertionAVLt3AVL(a->fd,a1,a2,a3,a4,a5, h);
    } 
    else                                // si il y a un doublon
    {
        *h = 1;
         if(a->fd!=NULL)            // si a possede un fils droit: on insere le noeud entre le fils droit de a et a
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);
            doublon->fd=a->fd;
            a->fd=doublon;
        }
        else            // si a n'a pas de fils droit: on insere le noeud directement en tant que fils droit de a
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);
            a->fd=doublon;
        }
        return a;
    }

   if (*h != 0)                     //equilibrage de l'arbre
   {
      a->equilibre = a->equilibre + *h;
      a=equilibrerAVLt3(a);
      if (a->equilibre == 0) 
      {
         *h = 0;
      } 
    else 
    {
        *h = 1;
    }
   }

   return a;
}
//------------------------------------------------------ abr t2-----------------
pArbret3 insertionABRt2(pArbret3 a,char* a1, float a2,float a3,float a4,float a5)               //insertion d'un noeud dans un ABR selon l'otpion t2
{
    if(a==NULL)
    {
        return a=creerArbret3(a1,a2,a3,a4,a5);
    }
    else if(strcmp(a1,a->c1)<0)                         //si la date du noeud est + ancienne que celle de a, on regarde dans le fils gauche de a
    {
        a->fg=insertionABRt2(a->fg,a1,a2,a3,a4,a5);
    }
    else if(strcmp(a1,a->c1)>0)                         //si la date du noeud est + recente que celle de a, on regarde dans le fils droit de a
    {
        a->fd=insertionABRt2(a->fd,a1,a2,a3,a4,a5);
    }
    else                                                         // si il y a un doublon
    {
        if(a->fg!=NULL)                                         // si a possede un fils gauche: on insere le noeud entre le fils gauche de a et a
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);
            doublon->fg=a->fg;
            a->fg=doublon;
        }
        else                                                    // si a n'a pas de fils gauche: on insere le noeud directement en tant que fils gauche de a
        {
            pArbret3 doublon=creerArbret3(a1,a2,a3,a4,a5);          
            a->fg=doublon;
        }
    }
    return a;
}
//----------------------------------------------------- t2 tab-------------------------

Chainont3 *ajouterCroissantt2(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5) {       //insertion du chainon dans la liste selon l'option t2
    Chainont3 *nouveau=creationChainont3(a1,a2,a3,a4,a5);
    Chainont3 *p1 =pliste;
    if(pliste==NULL){// si la liste est vide
        pliste=nouveau;
    }
    else if (strcmp(pliste->c1,a1)>0) {      //si la date du chainon à inserer est + ancienne que celle de l'endroit oû on est dans la liste  
        pliste=insertDebutt3(pliste,a1,a2,a3,a4,a5);
    }
    else{
        //on parcours la liste jusqu'à trouver où doit aller le nouveau élément
        while(p1->suivant!= NULL && strcmp(p1->suivant->c1,a1)<0){ 
            p1=p1->suivant;
        }
        if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
            p1->suivant=nouveau;
        }
        else{ // il faut inserer le maillon en millieu de chaîne, après p1
            nouveau->suivant=p1->suivant;
            p1->suivant = nouveau;
        }
    }
    return pliste;
}

Chainont3 *ajouterCroissantt2inverse(Chainont3 *pliste, char *a1, float a2,float a3,float a4,float a5) {            //insertion du chainon dans la liste selon l'option -t2 -r
    Chainont3 *nouveau=creationChainont3(a1,a2,a3,a4,a5);
    Chainont3 *p1 =pliste;
    if(pliste==NULL){// si la liste est vide
        pliste=nouveau;
    }
    else if (strcmp(pliste->c1,a1)<0) { // dans ce cas il faut placer l'element au début
        pliste=insertDebutt3(pliste,a1,a2,a3,a4,a5);
    }
    else{
        //on parcours la liste jusqu'à trouver où doit aller le nouveau élément
        while(p1->suivant!= NULL && strcmp(p1->suivant->c1,a1)>0){ 
            p1=p1->suivant;
        }
        if(p1->suivant==NULL){ // s'il faut placer le nouvel élément en fin de chaîne
            p1->suivant=nouveau;
        }
        else{ // il faut inserer le maillon en millieu de chaîne, après p1
            nouveau->suivant=p1->suivant;
            p1->suivant = nouveau;
        }
    }
    return pliste;
}
