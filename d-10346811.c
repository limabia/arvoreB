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
#define MAX 4

typedef int bool;

typedef struct no_arvoreB{
    int nchaves;
    int chaves[MAX];
    struct NO_ARVOREB *filhos[MAX+1];
} NO_ARVOREB;

// retorna a posicao que o elemento esta na arvore ou que deve ser inserido
// TODO implementar
int buscaBinaria(NO_ARVOREB *no, int valor){
    int inicio, fim, meio;
    
    if (no->nchaves == 0) {
        return 0;
    }

    inicio = 0;
    fim = no->nchaves - 1;
    
    while(inicio < fim) {
        meio = (inicio + fim) / 2;
        
        if(valor > no->chaves[meio]){
            inicio = meio + 1;
        } else if(valor < no->chaves[meio]) {
            fim = meio - 1;
        }
        else { 
            return meio; 
        }
    }
    return inicio;
}

void insereChave(NO_ARVOREB *no, int valor, NO_ARVOREB *filho) {
    int i = no->nchaves;

    while(no->chaves[i-1] > valor){
        no->chaves[i] =no->chaves[i-1];
        no->filhos[i+1] = no->chaves[i];
        i--;
    }
    no->chaves[i] = valor;
    no->filhos[i+1] = filho;
    no->nchaves ++;
}

bool insereRec(NO_ARVOREB *no, int valor, int *chavePromovida){
    if (no == NULL) {
        *chavePromovida = valor;
        return true;
    }
    int posicao = buscaBinaria(no, valor);
    int novaChavePromovida;
    bool houvePromocao = insereRec(no->filhos[posicao], valor, &novaChavePromovida);
    if( houvePromocao){
        if (no->nchaves < MAX) {
            insereChave(no, valor, NULL);
        }

    }
    return false;
}

void insercao(NO_ARVOREB *no, int valor){
    int chavePromovida = 0;
    bool promove = insereRec(no, valor, chavePromovida);
}

// TODO ajustar pra retornar um array com as chaves em ordem
void impressao(NO_ARVOREB *no){
    if(!no){
        printf("nao existem nos para serem exibidos");
        return;
    }
    int i;
    for(i=0, i<no->nchaves; i++){
        impressao(no->filhos[i]);
        printf(" - %d", no->chaves[i]);
    }
    impressao(no->filhos[no->chaves]);
}

// remove o elemento da arvore B
bool remocao(int valor){
    return false;
}

// coloca em um arquivo saida.txt a arvore que existe no momento de sua execucao
void fim(){
    return false;
}

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

    NO_ARVOREB * no = (NO_ARVOREB *) malloc(sizeof(NO_ARVOREB));

    // TODO colocar isso dentro de um for pra ler todas as linhas do arquivo
    while(fscanf(entrada, "%s", comando)) {
        fscanf(entrada, "%s %d", comando, &valor);
        printf("%s \n ", comando);

        if (strcmp (comando, "insere") == 0){
            printf("insere");
            insercao(*no, valor);
        }
        else if (strcmp (comando, "imprime") == 0){
            printf("imprime");
            impressao(no);
        }
        else if (strcmp (comando, "remove") == 0){
            printf("remove");
            remocao(valor);
        }
        else if (strcmp (comando, "fim") == 0){
            printf("fim");
            fim();
        }
        else {
            printf("linha invalida");
        }
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
