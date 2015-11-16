//
//  main.c
//  MiniShell
//
//  Created by AntoineAuer on 12/11/2015.
//  Copyright © 2015 AntoineAuer. All rights reserved.
//
#define SIZEMAX 100
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// entete de fonctions
int parsing();
int commande(int *fin, int *fout, char* com, char* param, int* bg);
int delimiteur(int c);



// variables globales
// on changera rescommande pour la mettre dans le main
int rescommande = 0;
char* resP[SIZEMAX];

int main(int argc, const char * argv[]) {
    int fin[1];     fin[0]=0;
    int fout[1];    fout[0]=1;
    int bg[1];
    // Création d'un fichier récuperant les impressions intermediaires (deboggage)
    int fmess = open("imp", O_WRONLY | O_TRUNC | O_CREAT, 0640);
    close(2);
    dup(fmess);
    close(fmess);
    char com[20];
    char param[20]; //VAR param ajoutée
    int status, eof = 0;
    
    printf("DAUPHINE> ");
    fflush(stdout);

    while (1) {
        if (rescommande == 1 ) {
            rescommande=0;
            eof = 0 ;
            printf("DAUPHINE> ");
            fflush(stdout);
        }
        else{
            //plus tard : rescommande = commander ();
            commande(fin,fout,com,param,bg);
        }
        return 0;
    }
}

// pour plus tard mettre des pointeurs sur fin fout, le retour sera rescommande
int commande(int *fin, int *fout, char* com, char* param, int* bg){
    int s, res=0, status, eof; // VAR status && eof ajoutées
    
    s=parsing();
    fprintf(stderr, "résultat parsing %d\n",s);
    
    switch (s) {
        case 0: // NL
            res = 2;
            pid_t pid = fork();
            if (pid == 0){
                rescommande =1; //cela permettra de ne pas de reecrire
                execvp(resP[0], resP);
            }
            else{
                rescommande = 2 ;
                wait(&status); // VAR status ajoutée
                eof = 2;
                break;
            }
            break;
            
        case 1: // ;
             //TO-DO
            break;
            
        case 2: // &
             //TO-DO
            *bg=1;
            break;
            
        case 3: // <
             //TO-DO
            break;
            
        case 4: // >
             //TO-DO
            parsing();
            int x=0; // a remplacer par ce qu il y a en dessous
            //int x=open(,);
            *fout = x ;
            break;
            
        case 5: // |
             //TO-DO
            break;
            
        case 7: // EOF
             //TO-DO
            exit(5); // le prgm doit s arreter
            break;
            
        case 10: // mot
            if (strcmp(&com[0], "exit")==0) {
                *fin=1;
                break;
            }
            break;
            
        default:
            fprintf(stderr, "erreur au switch de commande\n");
            break;
    }
    return res;
}

int parsing(){
    char mot[100];
    int i=0;
    int cmot = 0;
    fprintf(stderr, "debut parsing\n");
    
    while (1) {
        int c = getchar();
        fprintf(stderr,"caractère lu %d %c\n",c,c);
        
        if (c == '\n') {
            return 0;
        }
        else if (c == ';') {
            return 1;
        }
        else if (c == '&') {
            return 2;
        }
        else if (c == '<') {
            return 3;
        }
        else if (c == '>') {
            return 4;
        }
        else if (c == '|') {
            return 5;
        }
        else if (c == EOF) {
            return 7;
        }
        else if (c != ' ') {
            // On rentre dans une sequence
            //fprintf(stderr, "caractère lu 10\n");
            while( c != '\n' && !delimiteur(c)){
                i=0;
                while (c!=32) {
                    if ((c != '\n') && (delimiteur(c) == 0)) {
                        mot[i] = c;
                        i++;
                        c=getchar();
                        //fprintf(stderr, "valeur du caractère lu %d\n",c);
                    }
                    else{
                        //fprintf(stderr, "break\n");
                        break;
                    }
                }
                    break;
            }
                        
            while(c == ' ')
                c = getchar();
            
            ungetc(c, stdin);
            mot[i] = '\0';
            resP[cmot++] = strdup(mot);
            fprintf(stderr, "element comm lue %s %s\n", resP[0],resP[1]);
            if(c == '\n' || delimiteur(c)){
                resP[cmot]=0;
                if(c!='\n')
                    ungetc(c, stdin);
                return 10;
            }
        }
    }
    //erreur a traiter
    fprintf(stderr, "element comm lue %s %s\n", resP[0],resP[1]);
    return 404;
}
                        
int delimiteur(int c){
    if(c == '&' || c == ';' || c == '<' || c == '>' || c == '|') // Rearangement plus propre
        return 1;
    return 0;
}
