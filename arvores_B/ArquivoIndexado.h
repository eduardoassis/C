#ifndef ARQUIVO_INDEXADO_H
#define ARQUIVO_INDEXADO_H

#include "Pessoa.h"
#include <stdlib.h>

#define TAM_MAX 1000

typedef struct
{
	Chave c;
	int pos;

} entradaIndice;

struct ArquivoIndexado
{

	struct Arquivo * arquivo;
	struct ArvoreB * indice;

};

struct ArquivoIndexado * CriaArquivoIndexado(char * nome);
void Insere(struct ArquivoIndexado * A, struct Pessoa * pessoa);
struct Pessoa * Buscar(struct ArquivoIndexado * A, Chave chave);
struct Pessoa * Remover(struct ArquivoIndexado * A, Chave chave);

#endif
