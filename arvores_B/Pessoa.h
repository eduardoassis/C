#ifndef PESSOA_H
#define PESSOA_H

typedef int Chave;

struct Pessoa
{
	char nome[50];
	int id;
	int idade;
	int removido;
};

struct Pessoa * CriaPessoa(char nome[50], int idade, Chave id);
char * getNome(struct Pessoa * pessoa);
int getIdade(struct Pessoa * pessoa);
Chave getId(struct Pessoa * pessoa);
int Chave_Maior(Chave A, Chave B);
int Chave_Igual(Chave A, Chave B);
int Chave_Menor(Chave A, Chave B);
int Chave_Diferente(Chave A, Chave B);

#endif
