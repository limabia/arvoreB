// Nome: Bianca Lima Santos
// Num USP: 10346811
// Created by bianca on 26/05/18.
// objetivo: inserção, remoção e impressão de arvore B/ Btree


#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#define true 1
#define false 0

typedef int bool;

bool leEntrada(char *nomeEntrada){

    printf("%s \n ", nomeEntrada);

    FILE *entrada;
    entrada = fopen(nomeEntrada, "r");

    // verifica se existe conteudo no arquivo
    if (entrada == NULL){
        printf("Arquivo invalido");
        return false;
    }
    char comando[20];
    int valor;
    fscanf(entrada, "%s %d", comando, &valor);
    printf("%s \n ", comando);

    
    if (strcmp (comando, "insere") == 0){
        printf("insere");
    }
    else if (strcmp (comando, "imprime") == 0){
        printf("imprime");
    }
    else if (strcmp (comando, "remove") == 0){
        printf("remove");
    }
    else if (strcmp (comando, "fim") == 0){
        printf("fim");
    }
    else {
        printf("linha invalida");
    }

    fclose(entrada);

    return true;
}

int main(int argc, char **argv) {
    // se existe arquivo de entrada o le, caso contrario nao le
    if (argc == 2) {
        bool executou = leEntrada(argv[1]);
        printf("%i  \n", executou);
    } else {
        printf("Informe o arquivo de entrada para prosseguir com a execucao do algoritmo. \n");
    }

    return 0;
}
