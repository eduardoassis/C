#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Pessoa.h"

struct Arquivo
{
	FILE * arquivo;
	char * nomeArq;
	int numRegistros;
};

struct Arquivo * CriaArquivo(char * nome);
int InsereArquivo(struct Arquivo * A, struct Pessoa * pessoa);
int NumRegistros(struct Arquivo * A);
struct Pessoa * Le(struct Arquivo *A, int p);
void Escreve(struct Arquivo *A, struct Pessoa * pessoa, int p);

#endif
