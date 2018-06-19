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
#define MIN (MAX/2)

typedef int bool;

// estrutura de no da arvore B
// permite mais filhos e chaves que o maximo estipulado para operacoes intermediarias entre insercao e remocao
typedef struct no_arvoreB {
    int nchaves;
    int chaves[MAX+1];
    struct no_arvoreB *filhos[MAX + 2];
} NO_ARVOREB;

// retorna a posicao que o elemento esta na arvore ou que deve ser inserido
int buscaBinaria(NO_ARVOREB *no, int valor) {
    int inicio, fim, meio;

    if (no->nchaves == 0) {
        return 0;
    }

    inicio = 0;
    fim = no->nchaves - 1;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;

        if (valor == no->chaves[meio]) {
            return meio;
        } else if (valor < no->chaves[meio]) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return inicio;
}

// TODO ajustar pra retornar um array com as chaves em ordem pra usar na func fim
void impressao(NO_ARVOREB *no, int nivel) {
    if (no == NULL) {
        return;
    }
    int i;
    for (i = 0; i < no->nchaves; i++) {
        impressao(no->filhos[i], nivel + 1);
        for (int j=0; j<nivel; j++) {
            printf("    ");
        }
        printf("%d \n", no->chaves[i]);
    }
    impressao(no->filhos[no->nchaves], nivel + 1);
}

// insercao da chave (valor) dado que o no correto ja foi encontrado
void insereChave(NO_ARVOREB *no, int valor, NO_ARVOREB *filho) {
    int i = no->nchaves;

    while (no->chaves[i - 1] > valor && i > 0) {
        no->chaves[i] = no->chaves[i - 1];
        no->filhos[i + 1] = no->filhos[i];
        i--;
    }
    no->chaves[i] = valor;
    no->filhos[i + 1] = filho;
    no->nchaves++;
}


void split(NO_ARVOREB *no, int valor, int posicao, int *chavePromovida, NO_ARVOREB **filho) {
    // grava todas chaves num array temporario pra facilitar o split
    int chaves[MAX + 1];
    NO_ARVOREB *filhos[MAX + 2];
    int i;

    for (i = 0; i < posicao; i++) {
        chaves[i] = no->chaves[i];
        filhos[i] = no->filhos[i];
    }
    filhos[i] = no->filhos[i];
    filhos[i + 1] = *filho;

    chaves[posicao] = valor;
    for (i = posicao; i < MAX; i++) {
        chaves[i + 1] = no->chaves[i];
        filhos[i + 2] = no->filhos[i + 1];
    }

    // limpa o no
    no->nchaves = MAX / 2;
    for (i = MAX / 2; i < MAX; i++) {
        no->chaves[i] = 0;
        no->filhos[i] = NULL;
    }
    no->filhos[MAX] = NULL;

    // aloca o filho
    *filho = malloc(sizeof(NO_ARVOREB));

    // grava as chaves e os filhos nos nos corretos
    no->filhos[MAX / 2] = filhos[MAX / 2];
    (*filho)->nchaves = MAX / 2;
    (*filho)->filhos[0] = filhos[MAX / 2 + 1];
    for (i = 0; i < MAX / 2; i++) {
        no->chaves[i] = chaves[i];
        no->filhos[i] = filhos[i];

        (*filho)->chaves[i] = chaves[MAX / 2 + i + 1];
        (*filho)->filhos[i + 1] = filhos[MAX / 2 + i + 2];
    }

    // promove a chave do meio
    *chavePromovida = chaves[MAX / 2];
}


// encontra a posicao que a chave deve ser inserido
// retorna true caso inseriu corretamente, false caso houve promocao
bool insereRec(NO_ARVOREB *no, int valor, int *chavePromovida, NO_ARVOREB **filho) {
    if (no == NULL) {
        *chavePromovida = valor;
        return false;
    }

    int posicao = buscaBinaria(no, valor);


    bool inseriu = insereRec(no->filhos[posicao], valor, chavePromovida, filho);
    if (inseriu) {
        // chave já foi inserida
        return true;
    } else {
        // chave foi promovida e deve ser inserida caso haja espaco ou fazer split caso contrario

        if (no->nchaves < MAX) {
            // existe espaco, entao insere
            insereChave(no, *chavePromovida, *filho);
            return true;
        } else {
            // nao ha espaco suficiente, necessario um split
            split(no, *chavePromovida, posicao, chavePromovida, filho);
            return false;
        }

    }
}

// cuida de promocoes
void insercao(NO_ARVOREB **no, int valor) {
    int chavePromovida = 0;
    NO_ARVOREB *filho = NULL;
    bool inseriu = insereRec(*no, valor, &chavePromovida, &filho);

    // se nao inseriu, houve promocao
    if (!inseriu) {
        NO_ARVOREB *novaRaiz = malloc(sizeof(NO_ARVOREB));
        novaRaiz->nchaves = 1;
        novaRaiz->chaves[0] = chavePromovida;
        novaRaiz->filhos[0] = *no;
        novaRaiz->filhos[1] = filho;

        *no = novaRaiz;
    }
}

// delecao simples
void deletaChave(NO_ARVOREB *no, int posicao) {
    for (int i=posicao; i<no->nchaves; i++) {
        no->chaves[i] = no->chaves[i+1];
        no->filhos[i+1] = no->filhos[i+2];
    }
    no->nchaves--;
}


void merge(NO_ARVOREB *noEsquerda, NO_ARVOREB *noDireita, int chaveMeio) {
    noEsquerda->chaves[noEsquerda->nchaves] = chaveMeio;
    for (int i=0; i<noDireita->nchaves; i++) {
        noEsquerda->chaves[noEsquerda->nchaves + i + 1] = noDireita->chaves[i];
        noEsquerda->filhos[noEsquerda->nchaves + i + 1] = noDireita->filhos[i];
    }
    noEsquerda->filhos[noEsquerda->nchaves + noDireita->nchaves + 1] = noDireita->filhos[noDireita->nchaves];
    noEsquerda->nchaves = noEsquerda->nchaves + noDireita->nchaves + 1;
    free(noDireita);
}


// remove o elemento da arvore B
// https://medium.com/@vijinimallawaarachchi/all-you-need-to-know-about-deleting-keys-from-b-trees-9090f3334b5c
bool remocao(NO_ARVOREB **no, int valor) {
    int posicao = 0;
    int posPai = 0;
    NO_ARVOREB *noAtual = *no;
    NO_ARVOREB *noPai = NULL;

    // procura o valor
    while(noAtual != NULL) {
        posicao = buscaBinaria(noAtual, valor);
        if (noAtual->chaves[posicao] == valor) {
            // encontrou
            break;
        } else {
            noPai = noAtual;
            posPai = posicao;
            noAtual = noAtual->filhos[posicao];
        }
    }

    // se noAtual eh null, nao encontrou a chave
    if (noAtual == NULL)
        return false;

    // verifica se no é uma folha
    bool ehFolha = noAtual->filhos[posicao] == NULL;

    // caso 1, eh folha e n > MIN
    if (ehFolha && noAtual->nchaves > MIN) {
        deletaChave(noAtual, posicao);
    }
    // caso 2, eh folha e n == MIN
    else if (ehFolha && noAtual->nchaves == MIN) {
        // verifica se existe vizinho a esquerda
        if (posPai > 0) {
            NO_ARVOREB *vizinhoEsquerda = noPai->filhos[posPai-1];

            // verifica se ele pode emprestar uma chave
            if (vizinhoEsquerda->nchaves > MIN) {
                deletaChave(noAtual, posicao);
                insereChave(noAtual, noPai->chaves[posPai-1], NULL);
                noPai->chaves[posPai-1] = vizinhoEsquerda->chaves[vizinhoEsquerda->nchaves-1];
                deletaChave(vizinhoEsquerda, vizinhoEsquerda->nchaves-1);
            } else {
                // faz um merge
                deletaChave(noAtual, posicao);
                merge(vizinhoEsquerda, noAtual, noPai->chaves[posPai-1]);
                // TODO eh uma chave interna, pode precisar de balanceamento
                deletaChave(noPai, posPai);
            }
        }
        // verifica se existe vizinho a direita
        else if (posPai < MAX) {
            NO_ARVOREB *vizinhoDireita = noPai->filhos[posPai+1];

            // verifica se ele pode emprestar uma chave
            if (vizinhoDireita->nchaves > MIN) {
                deletaChave(noAtual, posicao);
                insereChave(noAtual, noPai->chaves[posPai], NULL);
                noPai->chaves[posPai] = vizinhoDireita->chaves[0];
                deletaChave(vizinhoDireita, 0);
            } else {
                // faz um merge
                deletaChave(noAtual, posicao);
                merge(noAtual, vizinhoDireita, noPai->chaves[posPai]);
                // TODO eh uma chave interna, pode precisar de balanceamento
                deletaChave(noPai, posPai);
            }
        }
    }
    // caso 3, no interno
    else if (!ehFolha) {
        NO_ARVOREB *filhoEsquerdo = noAtual->filhos[posicao];
        NO_ARVOREB *filhoDireito = noAtual->filhos[posicao+1];

        // caso 3a
        if (filhoEsquerdo->nchaves > MIN) {
            int chaveImediatamenteAnterior = filhoEsquerdo->chaves[filhoEsquerdo->nchaves-1];
            remocao(no, chaveImediatamenteAnterior);
            noAtual->chaves[posicao] = chaveImediatamenteAnterior;
        } else if (filhoDireito->nchaves > MIN) {
            int chaveImediatamenteSeguinte = filhoDireito->chaves[0];
            remocao(no, chaveImediatamenteSeguinte);
            noAtual->chaves[posicao] = chaveImediatamenteSeguinte;
        } else {
            merge(filhoEsquerdo, filhoDireito, valor);
            // TODO eh uma chave interna, pode precisar de balanceamento
            remocao(no, valor);
            //deletaChave(noAtual, posicao);
            // merge k and z to y
            //     Free memory of node z
            //     Recursively delete k from y
        }

    }

    return true;
}

// coloca em um arquivo saida.txt a arvore que existe no momento de sua execucao
void fim() {
    return;
}

void leEntrada(char *nomeEntrada) {

    printf("%s \n", nomeEntrada);

    FILE *entrada;

    entrada = fopen(nomeEntrada, "r");
    // verifica se existe conteudo no arquivo
    if (entrada == NULL) {
        printf("Arquivo invalido\n");
        return;
    }

    char comando[20];
    int valor;

    NO_ARVOREB *no = (NO_ARVOREB *) malloc(sizeof(NO_ARVOREB));

    while (fscanf(entrada, "%s %d", comando, &valor) != EOF) {

        //printf("%s %d \n ", comando, valor);

        if (strcmp(comando, "insere") == 0) {
            printf("entrou no if inserir\n");
            insercao(&no, valor);
        } else if (strcmp(comando, "imprime") == 0) {
            printf("entrou no if da impressao \n");
            impressao(no, 0);
        } else if (strcmp(comando, "remove") == 0) {
            printf("remove \n");
            remocao(&no, valor);
        } else if (strcmp(comando, "fim") == 0) {
            //printf("fim \n");
            //fim();
        } else {
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
