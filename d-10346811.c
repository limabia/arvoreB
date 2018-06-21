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
#define MAX 3
#define MIN (MAX-1)/2

typedef int bool;

// estrutura de no da arvore B
// permite mais filhos e chaves que o maximo estipulado para operacoes intermediarias entre insercao e remocao
typedef struct no_arvoreB {
    int nchaves;
    int chaves[MAX + 1];
    struct no_arvoreB *filhos[MAX + 2];
} NO_ARVOREB;

NO_ARVOREB *criaArvoreB();

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
        for (int j = 0; j < nivel; j++) {
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
    for (int i = posicao; i < no->nchaves; i++) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }
    no->nchaves--;
}


// faz um merge e retorna a nova posicao da chave do meio
int merge(NO_ARVOREB *noEsquerda, NO_ARVOREB *noDireita, int chaveMeio) {
    noEsquerda->chaves[noEsquerda->nchaves] = chaveMeio;
    for (int i = 0; i < noDireita->nchaves; i++) {
        noEsquerda->chaves[noEsquerda->nchaves + i + 1] = noDireita->chaves[i];
        noEsquerda->filhos[noEsquerda->nchaves + i + 1] = noDireita->filhos[i];
    }
    noEsquerda->filhos[noEsquerda->nchaves + noDireita->nchaves + 1] = noDireita->filhos[noDireita->nchaves];
    noEsquerda->nchaves = noEsquerda->nchaves + noDireita->nchaves + 1;
    free(noDireita);;
}


void remocaoRec(NO_ARVOREB *no, int k) {
    int posicao = buscaBinaria(no, k);

    // verifica se no é uma folha
    bool ehFolha = no->filhos[0] == NULL;

    // caso 1, eh folha e n > MIN
    if (ehFolha) {
        if (no->chaves[posicao] == k) {
            deletaChave(no, posicao);
        }
    } else {
        if (no->chaves[posicao] == k) {
            // caso2, é nó interno e k está no nó
            NO_ARVOREB *filho_esquerdo = no->filhos[posicao];
            NO_ARVOREB *filho_direito = no->filhos[posicao+1];

            if (filho_esquerdo->nchaves > MIN) {
                // caso 2a - o filho predecessor tem mais que o mínimo de chave
                int predecessor = filho_esquerdo->chaves[filho_esquerdo->nchaves - 1];
                remocaoRec(filho_esquerdo, predecessor);

                // substitui k pelo predecessor
                no->chaves[posicao] = predecessor;

            } else if (filho_direito->nchaves > MIN) {
                // caso 2b - filho sucessor tem mais que o mínimo de chave
                int sucessor = filho_direito->chaves[0];
                remocaoRec(filho_esquerdo, sucessor);

                // substitui k pelo predecessor
                no->chaves[posicao] = sucessor;

            } else {
                //  caso 2c - necessario um merge
                merge(filho_esquerdo, filho_direito, k);
                for (int i = posicao + 1; i < no->nchaves; i++) {
                    no->chaves[i - 1] = no->chaves[i];
                    no->filhos[i] = no->filhos[i + 1];
                }
                no->filhos[no->nchaves] = NULL;
                no->nchaves--;

                remocaoRec(filho_esquerdo, k);
            }
        } else {
            // caso 3 - k nao esta no no

            NO_ARVOREB *xci = no->filhos[posicao];

            if (xci->nchaves == MIN) {
                NO_ARVOREB *esquerda = NULL;
                NO_ARVOREB *direita = NULL;

                if (posicao > 0) {
                    esquerda = no->filhos[posicao - 1];
                }
                if (posicao < MAX) {
                    direita = no->filhos[posicao + 1];
                }

                if (esquerda != NULL && esquerda->nchaves > MIN) {
                    // desce uma chave do no para xci e sobe uma
                    int chave_desce = no->chaves[posicao - 1];
                    int chave_sobe = esquerda->chaves[esquerda->nchaves - 1];
                    NO_ARVOREB *filho_move = esquerda->filhos[esquerda->nchaves];

                    // desloca chaves  e filhos de xci para direita
                    xci->filhos[xci->nchaves + 1] = xci->filhos[xci->nchaves];
                    for (int i = xci->nchaves; i > 0; i--) {
                        xci->chaves[i] = xci->chaves[i - 1];
                        xci->filhos[i] = xci->filhos[i - 1];
                    }
                    xci->chaves[0] = chave_desce;
                    xci->filhos[0] = filho_move;
                    xci->nchaves++;

                    // sobe a chave da esquerda
                    no->chaves[posicao - 1] = chave_sobe;
                    esquerda->chaves[esquerda->nchaves - 1] = 0;
                    esquerda->filhos[esquerda->nchaves] = NULL;
                    esquerda->nchaves--;

                } else if (direita != NULL && direita->nchaves > MIN) {
                    // desce uma chave do no para xci e sobe uma
                    int chave_desce = no->chaves[posicao];
                    int chave_sobe = direita->chaves[0];
                    NO_ARVOREB *filho_move = direita->filhos[0];

                    // desce chave na ultima posicao de xci
                    xci->chaves[xci->nchaves] = chave_desce;
                    xci->filhos[xci->nchaves + 1] = filho_move;
                    xci->nchaves++;


                    // sobe a chave da direita
                    no->chaves[posicao] = chave_sobe;

                    // desloca chaves da direita
                    for (int i = 0; i < direita->nchaves - 1; i++) {
                        direita->chaves[i] = direita->chaves[i + 1];
                        direita->filhos[i] = direita->filhos[i + 1];
                    }
                    direita->filhos[direita->nchaves - 1] = direita->filhos[direita->nchaves];
                    direita->filhos[direita->nchaves] = NULL;
                    direita->nchaves--;
                } else {
                    // faz um merge
                    if (esquerda != NULL) {
                        merge(esquerda, xci, no->chaves[posicao - 1]);

                        for (int i = posicao; i < no->nchaves; i++) {
                            no->chaves[i - 1] = no->chaves[i];
                            no->filhos[i] = no->filhos[i + 1];
                        }
                        no->filhos[no->nchaves] = NULL;
                        no->nchaves--;
                    } else {
                        merge(xci, direita, no->chaves[posicao]);

                        for (int i = posicao + 1; i < no->nchaves; i++) {
                            no->chaves[i - 1] = no->chaves[i];
                            no->filhos[i] = no->filhos[i + 1];
                        }
                        no->filhos[no->nchaves] = NULL;
                        no->nchaves--;
                    }
                }
            }

            remocaoRec(xci, k);
        }
    }
}


// remove o elemento da arvore B
bool remocao(NO_ARVOREB **raiz, int valor) {
    remocaoRec(*raiz, valor);

    if ((*raiz)->nchaves == 0) {
        *raiz = (*raiz)->filhos[0];
    }
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

    NO_ARVOREB *no = criaArvoreB();

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

NO_ARVOREB *criaArvoreB() {
    NO_ARVOREB *arvore = (NO_ARVOREB *) malloc(sizeof(NO_ARVOREB));

    for (int i=0; i<MAX; i++) {
        arvore->chaves[i] = 0;
        arvore->filhos[i] = NULL;
    }
    arvore->filhos[MAX] = NULL;

    return arvore;
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
