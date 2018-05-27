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
int buscabinaria(no_arvoreB *raiz, int valor){
    return 1;
}

// TODO arrumar esses parametros aqui que tao uma loucura
void insereChave(NO_ARVOREB *raiz, int chave, NO_ARVOREB *filhoDireita) {
    int i = nchaves;
    while(no->chaves[i-1] > chave){
        no->chaves[i] =no->chaves[i-1];
        no->filhos[i+1] = no->chaves[i];
        i--;
    }
    no->chaves[i] = chave;
    no->filhos[i+1] = no->filhos[i];
    nchaves ++;
}

bool insercao(NO_ARVOREB *raiz, int valor, bool *estaNaFolha, int *chaveRetorno){
    int i, j, pos, meio;
    no_arvoreB *temp, *filhoDireita;

    // nao existe raiz
    if (raiz == NULL) {
        *estaNaFolha = true;
        *chaveRetorno = c;
        return false;
    }
    else {
        pos = buscabinaria(raiz, c);
        if (raiz ->chaves[pos] == c){
            printf("chave ja existe na arvore");
            *estaNaFolha = false;
        }
        else{
            filhoDireita = insere( raiz->filhos[pos], c, estaNaFolha, chaveRetorno);

            if( *estaNaFolha && (raiz ->nchaves < MAX)){
                insereChave(raiz, *chaveRetorno, filhoDireita);
                *estaNaFolha = false;
            } else {
                temp = (NO_ARVOREB*)malloc(sizeof(NO_ARVOREB));
                temp->nchaves = 0;

                for( i=0; i < MAX+1; i++){
                    temp->filhos[i] = NULL;
                }

                int l = (int)MAX/2;
                meio = raiz->chaves[l];

                for(i = l+1; i < MAX; i++){
                    insereChave(tmp, raiz->chaves[i], raiz->filhos[i+1]);
                    raiz->chaves[i] = 0;
                    raiz->filhos[i+1] = NULL;
                }

                raiz->nchaves = l;

                if(pos <= l){
                    insereChave(raiz, *chaveRetorno, filhoDireita);
                } else{
                    insereChave(temp, *chaveRetorno, filhoDireita);
                }
                *chaveRetorno = meio;
                return tue;
            }
        }
    }
    return false;
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
bool fim(){
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

    // TODO colocar isso dentro de um for pra ler todas as linhas do arquivo
        fscanf(entrada, "%s %d", comando, &valor);
        printf("%s \n ", comando);

        bool operacao;

        if (strcmp (comando, "insere") == 0){
            printf("insere");
            operacao = insercao(valor);
        }
        else if (strcmp (comando, "imprime") == 0){
            printf("imprime");
            impressao();
        }
        else if (strcmp (comando, "remove") == 0){
            printf("remove");
            operacao = remocao(valor);
        }
        else if (strcmp (comando, "fim") == 0){
            printf("fim");
            operacao = fim();
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
