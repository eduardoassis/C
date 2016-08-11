#include "Pessoa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pessoa * CriaPessoa(char nome[50], int idade, Chave id)
{
	struct Pessoa * pessoa = malloc(sizeof(struct Pessoa));

	strcpy(pessoa->nome, nome);
	pessoa->idade = idade;
	pessoa->id = id;
	pessoa->removido = 0;
	return pessoa;
}

char * getNome(struct Pessoa * pessoa)
{
	return pessoa->nome;
}

int getIdade(struct Pessoa * pessoa)
{
	return pessoa->idade;
}

Chave getId(struct Pessoa * pessoa)
{
	return pessoa->id;
}

int Chave_Maior(Chave A, Chave B)
{
	return A > B;
}

int Chave_Igual(Chave A, Chave B)
{
	return A == B;
}

int Chave_Menor(Chave A, Chave B)
{
	return A < B;
}

int Chave_Diferente(Chave A, Chave B)
{
	return A != B;
}
