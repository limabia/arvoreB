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
#define T 2
#define MAX (2*T - 1)
#define MIN (T - 1)

typedef int bool;

// estrutura de no da arvore B
// permite mais filhos e chaves que o maximo estipulado para operacoes intermediarias entre insercao e remocao
typedef struct no_arvoreB {
    int nchaves;
    int chaves[MAX];
    struct no_arvoreB *filhos[MAX + 1];
} NO_ARVOREB;


NO_ARVOREB *allocateNode() {
    NO_ARVOREB *node = (NO_ARVOREB *) malloc(sizeof(NO_ARVOREB));
    node->nchaves = 0;

    for (int i=0; i<MAX; i++) {
        node->chaves[i] = 0;
        node->filhos[i] = NULL;
    }
    node->filhos[MAX] = NULL;

    return node;
}


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


bool isLeaf(NO_ARVOREB *no) {
    return no->filhos[0] == NULL;
}


void bTreeSplitChild(NO_ARVOREB *x, int i) {
    NO_ARVOREB *z = allocateNode();
    NO_ARVOREB *y = x->filhos[i];

    z->nchaves = MIN;
    for  (int j=0; j<MIN; j++) {
        z->chaves[j] = y->chaves[MIN+j+1];
        z->filhos[j] = y->filhos[MIN+j+1];
    }
    z->filhos[MIN] = y->filhos[MAX];
    for (int j=x->nchaves; j>i; j--) {
        x->chaves[j] = x->chaves[j-1];
        x->filhos[j+1] = x->filhos[j];
    }
    x->filhos[i+1] = z;
    x->chaves[i] = y->chaves[MIN];
    x->nchaves++;
    y->nchaves = MIN;
}


void bTreeInsertNonfull(NO_ARVOREB *x, int k) {
    int i=x->nchaves;

    if (isLeaf(x)) {
        while (i>0 && x->chaves[i-1] > k) {
            x->chaves[i] = x->chaves[i-1];
            i--;
        }
        x->chaves[i] = k;
        x->nchaves++;
    } else {
        i = buscaBinaria(x, k);

        if (x->filhos[i]->nchaves == MAX) {
            bTreeSplitChild(x, i);

            if (k > x->chaves[i]) {
                i++;
            }
        }
        bTreeInsertNonfull(x->filhos[i], k);
    }
}


void bTreeInsert(NO_ARVOREB **raiz, int k) {
    if ((*raiz)->nchaves == MAX) {
        NO_ARVOREB *s = allocateNode();
        s->filhos[0] = *raiz;
        *raiz = s;

        bTreeSplitChild(s, 0);
        bTreeInsertNonfull(s, k);
    } else {
        bTreeInsertNonfull(*raiz, k);
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


int achaMaximo(NO_ARVOREB *no) {
    if (isLeaf(no)) {
        return no->chaves[no->nchaves-1];
    } else {
        return achaMaximo(no->filhos[no->nchaves]);
    }
}


int achaMinimo(NO_ARVOREB *no) {
    if (isLeaf(no)) {
        return no->chaves[0];
    } else {
        return achaMinimo(no->filhos[0]);
    }
}


void remocaoRec(NO_ARVOREB *no, int k) {
    int posicao = buscaBinaria(no, k);

    // verifica se no é uma folha
    bool ehFolha = isLeaf(no);

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
                int predecessor = achaMaximo(filho_esquerdo);
                remocaoRec(filho_esquerdo, predecessor);

                // substitui k pelo predecessor
                no->chaves[posicao] = predecessor;

            } else if (filho_direito->nchaves > MIN) {
                // caso 2b - filho sucessor tem mais que o mínimo de chave
                int sucessor = achaMinimo(filho_direito);
                remocaoRec(filho_direito, sucessor);

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
                        xci = esquerda;
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


void impressaoArq(FILE *f, NO_ARVOREB *no, int nivel) {
    if (no == NULL) {
        return;
    }
    int i;
    for (i = 0; i < no->nchaves; i++) {
        impressaoArq(f, no->filhos[i], nivel + 1);
        for (int j = 0; j < nivel; j++) {
            fprintf(f, "    ");
        }
        fprintf(f, "%d \n", no->chaves[i]);
    }
    impressaoArq(f, no->filhos[no->nchaves], nivel + 1);
}


// coloca em um arquivo saida.txt a arvore que existe no momento de sua execucao
void fim(NO_ARVOREB *no) {
    FILE *f = fopen("saida.txt", "w");
    if (f == NULL) {
        printf("Erro ao abrir arquivo de saida.\n");
        exit(1);
    }
    impressaoArq(f, no, 0);
    fclose(f);
    exit(0);
}

void leEntrada(char *nomeEntrada) {
    FILE *entrada;
    entrada = fopen(nomeEntrada, "r");

    // verifica se existe conteudo no arquivo
    if (entrada == NULL) {
        printf("Arquivo invalido\n");
        exit(1);
    }

    char comando[20];
    int valor;

    NO_ARVOREB *no = allocateNode();

    while (fscanf(entrada, "%s %d", comando, &valor) != EOF) {
        if (strcmp(comando, "insere") == 0) {
            bTreeInsert(&no, valor);
        } else if (strcmp(comando, "imprime") == 0) {
            printf("Arvore:\n");
            impressao(no, 0);
            printf("\n");
        } else if (strcmp(comando, "remove") == 0) {
            remocao(&no, valor);
        } else if (strcmp(comando, "fim") == 0) {
            fim(no);
        } else {
            printf("linha invalida \n");
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
