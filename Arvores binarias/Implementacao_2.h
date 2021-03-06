#ifndef DICIONARIO_H
#define DICIONARIO_H

#include <stdio.h>
#include <stdlib.h>
#include "Item.h"

struct No
{
	struct Item I;
	int altura;
	int encontrado;
	struct No **pai;
	struct No *esquerda;
	struct No *direita;
};

struct Dicionario
{
	struct No * raiz;
};

struct Dicionario * CriaDicionario();
void Inserir(struct Dicionario * D, struct Item I);
struct Item Remover(struct Dicionario * D, Chave c);
int Buscar(struct Dicionario * D, Chave c);
void Imprime(struct Dicionario * D);

#endif
