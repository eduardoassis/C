#ifndef ARVOREB_H
#define ARVOREB_H

#define ORDEM 4

#include <stdio.h>
#include <stdlib.h>


typedef int Chave;

struct No
{
	Chave chaves[ORDEM-1];

	// Valores que indicam a posicao no arquivo de cada chave
	int valoresAssociados[ORDEM-1];
	struct No *Filhos[ORDEM];
	int numElementos;
};


struct ArvoreB
{
	struct No *raiz;
};

struct ArvoreB* CriaArvoreB();
void InsereArvoreB(struct ArvoreB *A, Chave chave, int valor);
int RemoveArvoreB(struct ArvoreB *A, Chave chave);
int BuscaArvoreB(struct ArvoreB *A, Chave chave);
int BuscaInterno(struct No *N, Chave chave);
int InsereInterno(struct No *N, Chave chave, int valor, int *itemPromovido, int *valorAssociadoItemPromovido, struct No **filhoDItemPromovido);
int Remove_Interno(struct No *r, int e, int tipo);

#endif
