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

// estrutura de no da arvore B
typedef struct no_arvoreB{
    int nchaves;
    int chaves[MAX];
    struct no_arvoreB *filhos[MAX+1];
} NO_ARVOREB;

// retorna a posicao que o elemento esta na arvore ou que deve ser inserido
int buscaBinaria(NO_ARVOREB *no, int valor){
    int inicio, fim, meio;
    
    if (no->nchaves == 0) {
        return 0;
    }

    inicio = 0;
    fim = no->nchaves - 1;
    
    while (inicio < fim) {
        meio = (inicio + fim)/2;
        
        if (valor > no->chaves[meio]){
            inicio = meio+1;
        } else if (valor < no->chaves[meio]) {
            fim = meio-1;
        } else {
            return meio; 
        }
    }
    return inicio;
}

// TODO ajustar pra retornar um array com as chaves em ordem pra usar na func fim
void impressao(NO_ARVOREB *no){
    if(no == NULL){
        printf("\n");
        return;
    }
    int i;
    for(i=0; i < no->nchaves; i++){
        impressao(no->filhos[i]);
        printf("-> %d ", no->chaves[i]);
    }
    impressao(no->filhos[no->nchaves]);
}

// insercao da chave (valor) dado que o no correto ja foi encontrado
void insereChave(NO_ARVOREB *no, int valor, NO_ARVOREB *filho) {
    int i = no->nchaves;

    while(no->chaves[i-1] > valor){
        no->chaves[i] =no->chaves[i-1];
        no->filhos[i+1] = no->filhos[i];
        i--;
    }
    no->chaves[i] = valor;
    no->filhos[i+1] = filho;
    no->nchaves ++;
}

// encontra a posicao que a chave deve ser inserido
bool insereRec(NO_ARVOREB *no, int valor, int *chavePromovida){
    if (no == NULL) {
        *chavePromovida = valor;
        return true;
    }
    int posicao = buscaBinaria(no, valor);
    int novaChavePromovida;
    NO_ARVOREB *filho = NULL;

    bool houvePromocao = insereRec(no->filhos[posicao], valor, &novaChavePromovida);
    if( houvePromocao){
        if (no->nchaves < MAX) {
            insereChave(no, valor, filho);
        }

    }
    return false;
}

// cuida de promocoes
void insercao(NO_ARVOREB *no, int valor){
    int chavePromovida = 0;
    bool promove = insereRec(no, valor, &chavePromovida);
}

// remove o elemento da arvore B
bool remocao(int valor){
    return false;
}

// coloca em um arquivo saida.txt a arvore que existe no momento de sua execucao
void fim(){
    return;
}

void leEntrada(char *nomeEntrada){

    printf("%s \n", nomeEntrada);

    FILE *entrada;

    entrada = fopen(nomeEntrada, "r");
    // verifica se existe conteudo no arquivo
    if (entrada == NULL){
        printf("Arquivo invalido\n");
        return;
    }

    char comando[20];
    int valor;

    NO_ARVOREB * no = (NO_ARVOREB *) malloc(sizeof(NO_ARVOREB));

    while(fscanf(entrada, "%s %d", comando, &valor) != EOF) {

        //printf("%s %d \n ", comando, valor);

        if (strcmp (comando, "insere") == 0){
            printf("entrou no if inserir\n");
            insercao(no, valor);
        }
        else if (strcmp (comando, "imprime") == 0){
            printf("entrou no if da impressao \n");
            impressao(no);
        }
        else if (strcmp (comando, "remove") == 0){
            //printf("remove \n");
            //remocao(valor);
        }
        else if (strcmp (comando, "fim") == 0){
            //printf("fim \n");
            //fim();
        }
        else {
            //printf("linha invalida \n");
        }
    }

    fclose(entrada);
}

int main(int argc, char **argv) {

    // se existe arquivo de entrada o le, caso contrario nao le
    if (argc == 2) {
        leEntrada(argv[1]);
    } else {
        printf("Informe o arquivo de entrada para prosseguir com a execucao do algoritmo. \n");
    }

    return 0;
}
