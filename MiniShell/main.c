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
int parsing();
int commande(int fin, int fout, char* com, char* param, int* bg);
int delimiteur(int c);
char* resP[SIZEMAX];

int main(int argc, const char * argv[]) {
    // Création d'un fichier récuperant les impressions intermediaires (deboggage)
    int fmess = open("imp", O_WRONLY | O_TRUNC | O_CREAT, 0640);
    close(2);
    dup(fmess);
    close(fmess);
    char * com[20], param[20]; //VAR param ajoutée
    int status, bg, eof = 0;
    
    printf("DAUPHINE> ");
    fflush(stdout);

    while (1) {
        if (1 == 0) { // TO-DO
            eof = 0 ;
            printf("DAUPHINE> ");
            fflush(stdout);
        }
        else
            commande(0,1,&com,&param,&bg);
        
        return 0;
    }
}

int commande(int fin, int fout, char* com, char* param, int* bg){
    int s, res=0, status, eof; // VAR status && eof ajoutées
    
    s=parsing();
    fprintf(stderr, "résultat parsing %d\n",s);
    
    switch (s) {
        case 0: // NL
            res = 2;
            pid_t pid = fork();
            if (pid == 0)
                execvp(resP[0], resP);
            else{
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
            break;
            
        case 3: // <
             //TO-DO
            break;
            
        case 4: // >
             //TO-DO
            break;
            
        case 5: // |
             //TO-DO
            break;
            
        case 7: // EOF
             //TO-DO
            break;
            
        case 10: // mot
            if (strcmp(&com[0], "exit")==0) {
                fin=1;
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
    int symboleP; // VAR ajoutée
    int i=0;
    int cmot = 0;
    fprintf(stderr, "debut parsing\n");
    
    while (1) {
        int c = getchar();
        fprintf(stderr,"caractère lu %d %c\n",c,c);
        
        if (c == '\n') {
            symboleP = 0;
            return 0;
        }
        else if (c == ';') {
            symboleP = 1;
            return 1;
        }
        else if (c == '&') {
            symboleP = 2;
            return 2;
        }
        else if (c == '<') {
            symboleP = 3;
            return 3;
        }
        else if (c == '>') {
            symboleP = 4;
            return 4;
        }
        else if (c == '|') {
            symboleP = 5;
            return 5;
        }
        else if (c == 'EOF') {
            symboleP = 7;
            return 7;
        }
        else if (c != " ") {
            // On rentre dans une sequence
            symboleP = 10;
            //fprintf(stderr, "caractère lu 10\n");
            while( c != '\n' && !strchr(delimiteur, c)){ // Pas compris, VAR delimiteurs inconnue
                i=0;
                while (c!=32) {
                    if ((c != "\n") && (delimiteur(c) == 0)) {
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
            mot[i] = "\0"; // Pas compris, VAR mot inconnue
            resP[cmot++] = strdup(mot);
            fprintf(stderr, "element comm lue %s %s\n", resP[0],resP[1]);
            if(c == '\n' || strchr(delimiteurs,c)){ // Pas compris, VAR delimiteurs inconnue
                resP[cmot]=0;
                if(c!='\n')
                    ungetc(c, stdin);
                return 10;
            }
        }
    }
    fprintf(stderr, "element comm lue %s %s\n", resP[0],resP[1]);
}
                        
int delimiteur(int c){
    if(c == '&' || c == ';' || c == '<' || c == '>' || c == '|') // Rearangement plus propre
        return 1;
    return 0;
}