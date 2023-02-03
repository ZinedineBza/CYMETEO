#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>  // Pour les fichiers en arguments
#include <string.h>     // Pour strcmp (--abr et tout en argument)
#include <unistd.h>     // Pour acces (verifie si on a le droit de lecture sur un fichier en argument)
#include <math.h>
#define BUFFER_SIZE 512
#include"fonction.h"

//-----------------------------------------M A I N -----------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

    // Vérifie qu'on a bien entre 4 et 7 arguments
    if (argc < 5 || argc > 8) {
        fprintf(stderr, "Utilisation : %s fichier\n", argv[0]);
        return 1;
    }

    // Vérifie qu'on ait bien 2 fichiers en argument
    struct stat file1_stat, file2_stat;
    if (stat(argv[2], &file1_stat) != 0 || stat(argv[4], &file2_stat) != 0) {
        fprintf(stderr, "Erreur : l'un des fichiers spécifiés n'existe pas ou ne peut être lu.\n");
        return 1;
    }

    if (!S_ISREG(file1_stat.st_mode) || !S_ISREG(file2_stat.st_mode)) {     
        fprintf(stderr, "Erreur : l'un des fichiers spécifiés n'est pas un fichier régulier.\n");
        return 1;
    }

    // Vérifie si le troisième argument est "--abr", "--avl" ou "--tab" ou "-r" ou "-t2" ou "-t3"
    if(argc==6){
        if (strcmp(argv[5], "--abr") != 0 && strcmp(argv[5], "--avl") != 0 && strcmp(argv[5], "--tab") != 0 && strcmp(argv[5], "-t3")!=0 && strcmp(argv[5],"-r")!=0 && strcmp(argv[5],"-t2")!=0) {
            fprintf(stderr, "Erreur : le type de tri doit être '--abr', '--avl' ou '--tab'.\n");                 
            return 1;
        }
    }  

    // Verif si droit de lecture du fichier d'entrée
    if (access(argv[2], R_OK) != 0) {
        fprintf(stderr, "Erreur : le fichier '%s' ne peut pas être lu.\n", argv[2]);
        return 2;
    }

    // Ouvre le fichier d'entrée en mode lecture
    FILE* fIn = fopen(argv[2], "r");
    if (fIn == NULL) {      // le fichier peut pas etre ouvert
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 2;
    }

    
    // Verif si droit d'ecriture du fichier de sortie 
    if (access(argv[4], W_OK) != 0) {
        fprintf(stderr, "Erreur : le fichier '%s' ne peut pas être modifié.\n", argv[4]);
        fclose(fIn);
        return 3;
    }

    // Ouvre le fichier de sortie en mode ecriture
    FILE* fOut = fopen(argv[4], "w");
    if (fOut == NULL) {     // le fichier peut pas etre ouvert
        perror("Erreur lors de l'ouverture du fichier de sortie");
        fclose(fIn);
        return 3;
    }

    int h=0;
    if(argc==5)     // Il n'y a que 4 arguments: tri AVL par defaut
    {
                                                         //tri AVL
        printf("tri AVL realisé\n");
        pArbre a=NULL;
        int i=0;
        char buffer[BUFFER_SIZE];
        int NUM_LINES=40000;
        double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];            //alloue des tableaux qui prendront les valeurs de chaque colonne
        while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
        {
            if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide   + recuperation des valeurs de chaque colonne de la ligne
            {
                break;
            }
            a=insertionAVL(a,x[i],y[i],z[i],w[i],v[i],&h);             //insertion du nouveau noeud dans l'arbre a
            i++;
        }
        parcoursInfixe(a,fOut);                                         // affichage de tous les noeuds de a dans l'ordre croissant de c1
        fclose(fIn);
        fclose(fOut);
        return 0;
    }
    else if(argc==6)    //5 arguments 
    {
        if(strcmp(argv[5], "--abr") == 0)
        {
                                                                    // tri ABR
            printf("tri ABR realisé\n");
            pArbre a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionABR(a,x[i],y[i],z[i],w[i],v[i]);
                i++;
            }
            parcoursInfixe(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if(strcmp(argv[5], "--tab") == 0)
        {
                                                                     // tri TAB
            printf("tri tableau ralisé\n");
            int i=0;
            Chainon* pliste=NULL;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];

            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                pliste=ajouterCroissant(pliste,x[i],y[i],z[i],w[i],v[i]);
                i++;
            }
        
            Chainon*p1=pliste;
            while(p1!=NULL)
            {   
                fprintf(fOut, "%f %f %f %f %f\n", p1->c1,p1->c2,p1->c3,p1->c4,p1->c5);
                p1=p1->suivant;
            }
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if(strcmp(argv[5], "--avl") == 0){
                                                                     // tri AVL
            printf("tri AVL realisé\n");
            pArbre a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVL(a,x[i],y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixe(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if(strcmp(argv[5], "-t3") == 0){         //tri avl t3
            printf("tri avl option t3\n");
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt3AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avl(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if(strcmp(argv[5], "-r") == 0){        //tri avl inverse
            printf("tri avl inverse\n");
            pArbre a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVL(a,x[i],y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixeinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if(strcmp(argv[5], "-t2") == 0){       // tri avl t2
            printf("tri avl option t2\n");
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt2AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avl(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0; 
        }
    }
    else if(argc==7){                          //6 arguments
        if ((strcmp(argv[5], "-t3") == 0) && (strcmp(argv[6], "-r") == 0)){
                                                                                        //tri avl t3 inverse
            printf("tri avl t3 option inverse\n");
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt3AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avlinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0; 
        }
        else if(strcmp(argv[5],"-t2")==0 && strcmp(argv[6],"-r")==0){            //tri avl t2 inverse

            printf("avl t2 inverse\n");
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt2AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avlinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;                       
        }
        else if((strcmp(argv[5], "--abr") == 0) && (strcmp(argv[6], "-r") == 0)){
                                                                                        //tri abr inverse
            printf("tri abr option inverse\n");
            pArbre a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionABR(a,x[i],y[i],z[i],w[i],v[i]);
                i++;
            }
            parcoursInfixeinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if((strcmp(argv[5], "--tab") == 0) && (strcmp(argv[6], "-r" )== 0)){
                                                                                                //tri tab inverse
            printf("tri tab option inverse\n");
            int i=0;
            Chainon* pliste=NULL;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];

            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                pliste=ajouterCroissantinverse(pliste,x[i],y[i],z[i],w[i],v[i]);
                i++;
            }
        
            Chainon*p1=pliste;
            while(p1!=NULL)
            {   
                fprintf(fOut, "%f %f %f %f %f\n", p1->c1,p1->c2,p1->c3,p1->c4,p1->c5);
                p1=p1->suivant;
            }
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if((strcmp(argv[5], "--avl") == 0) && (strcmp(argv[6], "-r") == 0)){
                                                                                                    //tri avl inverse
            printf("tri avl option inverse\n");
            pArbre a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            double x[NUM_LINES], y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%lf %lf %lf %lf %lf", &x[i], &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVL(a,x[i],y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixeinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if((strcmp(argv[5], "--abr") == 0) && (strcmp(argv[6], "-t3") == 0)){      //abr t3
            printf("tri abr option t3\n");
            pArbret3 a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionABRt3(a,x,y[i],z[i],w[i],v[i]);
                i++;
            }
            parcoursInfixet3abr(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if((strcmp(argv[5], "--abr") == 0) && (strcmp(argv[6], "-t2") == 0)){          //abr t2
            printf("tri abr option t2\n");
            pArbret3 a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionABRt2(a,x,y[i],z[i],w[i],v[i]);
                i++;
            }
            parcoursInfixet3abr(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;                                                                                       
        }
        else if((strcmp(argv[5], "--avl") == 0) && (strcmp(argv[6], "-t3") == 0)){          // avl t3
            printf("tri avl option t3\n");
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt3AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avl(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0; 
        }
        else if((strcmp(argv[5], "--avl") == 0) && (strcmp(argv[6], "-t2") == 0)){       // avl t2
            printf("tri avl option t2\n");
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt2AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avl(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0; 
        }
        else if((strcmp(argv[5], "--tab") == 0) && (strcmp(argv[6], "-t3") == 0)){          //tab t3
            printf("tri tab option t3\n");
            int i=0;
            Chainont3 * pliste=NULL;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];

            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                pliste=ajouterCroissantt3boss(pliste,x,y[i],z[i],w[i],v[i]);     
                i++;
            }
            Chainont3*p2=pliste;
            
            while(p2!=NULL)
            {   
                fprintf(fOut, "%s %f %f %f %f\n", p2->c1,p2->c2,p2->c3,p2->c4,p2->c5);
                p2=p2->suivant;
            }
            fclose(fIn);
            fclose(fOut);  
            return 0;  
        }
        else if((strcmp(argv[5], "--tab") == 0) && (strcmp(argv[6], "-t2") == 0)){      //tab t2
            printf("tri tab option t2\n");
            int i=0;
            Chainont3 * pliste=NULL;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];

            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                pliste=ajouterCroissantt2(pliste,x,y[i],z[i],w[i],v[i]);
                i++;
            }

            Chainont3* p2=pliste;
            while(p2!=NULL)
            {   
                fprintf(fOut, "%s %f %f %f %f\n", p2->c1,p2->c2,p2->c3,p2->c4,p2->c5);
                p2=p2->suivant;
            }
            fclose(fIn);
            fclose(fOut);  
            return 0;
        }
    }
    else{                                                                                // TOUS LES ARGUMENTS
        if(strcmp(argv[5], "--abr") == 0 && strcmp(argv[6], "-t3") == 0 && strcmp(argv[7],"-r")==0)
        {
                                                                                            // tri ABR t3 inverse
            printf("tri ABR realisé option t3 inverse\n");
            pArbret3 a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionABRt3(a,x,y[i],z[i],w[i],v[i]);
                i++;
            }
            parcoursInfixet3abrinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;
        }
        else if(strcmp(argv[5], "--tab") == 0 && (strcmp(argv[6], "-t3") == 0) && strcmp(argv[7],"-r")==0)
        {
                                                                                            // tri TAB t3 inverse
            printf("tri tab option t3 inverse\n");
            int i=0;
            Chainont3 * pliste=NULL;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];

            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                pliste=ajouterCroissantt3inverseboss(pliste,x,y[i],z[i],w[i],v[i]);
                i++;
            }
            Chainont3*p2=pliste;
            while(p2!=NULL)
            {   
                fprintf(fOut, "%s %f %f %f %f\n", p2->c1,p2->c2,p2->c3,p2->c4,p2->c5);
                p2=p2->suivant;
            }
            fclose(fIn);
            fclose(fOut);  
            return 0;
        }
        else if(strcmp(argv[5], "--avl") == 0 && (strcmp(argv[6], "-t3") == 0) && strcmp(argv[7],"-r")==0){
                                                                                                // tri AVL t3 inverse
            printf("tri avl realisé option t3 inverse\n");
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt3AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avlinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0; 
        }
        else if(strcmp(argv[5], "--avl") == 0 && (strcmp(argv[6], "-t2") == 0)){            // avl t2 inverse
            printf("avl t2 inverse\n"); 
            pArbret3 a=NULL;
            int i=0;
            int h=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionAVLt2AVL(a,x,y[i],z[i],w[i],v[i],&h);
                i++;
            }
            parcoursInfixet3avlinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0; 
        }
        else if(strcmp(argv[5], "--abr") == 0 && (strcmp(argv[6], "-t2") == 0)){            // abr t2 inverse
            printf("tri abr option t2 inverse\n");
            pArbret3 a=NULL;
            int i=0;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];
            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                a=insertionABRt2(a,x,y[i],z[i],w[i],v[i]);
                i++;
            }
            parcoursInfixet3abrinverse(a,fOut);
            fclose(fIn);
            fclose(fOut);
            return 0;                                               
        }
        else if(strcmp(argv[5], "--tab") == 0 && (strcmp(argv[6], "-t2") == 0)){                // tab t2 inverse
             printf("tri tab option t2 inverse\n");
            int i=0;
            Chainont3 * pliste=NULL;
            char buffer[BUFFER_SIZE];
            int NUM_LINES=40000;
            char *x;
            x=malloc(sizeof(char)*128);
            if(x==NULL){
                exit(1);
            }
            double y[NUM_LINES], z[NUM_LINES], w[NUM_LINES], v[NUM_LINES];

            while (fgets(buffer, BUFFER_SIZE, fIn) != NULL) 
            {
                if (sscanf(buffer, "%s %lf %lf %lf %lf", x, &y[i], &z[i], &w[i], &v[i]) != 5) //si ligne vide
                {
                    break;
                }
                pliste=ajouterCroissantt2inverse(pliste,x,y[i],z[i],w[i],v[i]);
                i++;
            }

            Chainont3* p2=pliste;
            while(p2!=NULL)
            {   
                fprintf(fOut, "%s %f %f %f %f\n", p2->c1,p2->c2,p2->c3,p2->c4,p2->c5);
                p2=p2->suivant;
            }
            fclose(fIn);
            fclose(fOut);  
            return 0;                                     
        }
    }
    return 0;
}