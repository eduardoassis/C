#include "Arquivo.h"
#include "Pessoa.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>

struct Arquivo * CriaArquivo(char * nome)
{
	struct Arquivo * arquivo = malloc(sizeof(struct Arquivo));

	arquivo->nomeArq = nome;

	arquivo->arquivo = fopen(arquivo->nomeArq, "rb");

	 if (!arquivo->arquivo)
	 {
		arquivo->arquivo = fopen(nome, "w");
	 }

	fclose(arquivo->arquivo);

	if( !(arquivo->arquivo = fopen(arquivo->nomeArq, "rb")) )
	{
		fprintf(stderr, "Erro ao abrir o arquivo");
		exit(EXIT_FAILURE);
	}

	char * line = NULL;
	ssize_t read;
	size_t len = 0;

	int i =0;
	while ((read = getline(&line, &len, arquivo->arquivo)) != -1) {
		i++;
	}

	arquivo->numRegistros = i;

	return arquivo;
}

int InsereArquivo(struct Arquivo * arquivo, struct Pessoa * pessoa)
{
	const char * nome = arquivo->nomeArq;

	arquivo->arquivo = fopen(nome, "a");

	if(arquivo->arquivo == NULL)
	{
		printf("ERRO AO ABRIR O ARQUIVO\n");
		exit(EXIT_FAILURE);
	}

	fprintf(arquivo->arquivo, "%d\t%s\t%d\t%d\n", pessoa->id, pessoa->nome, pessoa->idade, pessoa->removido);

	fclose(arquivo->arquivo);

	arquivo->numRegistros++;

	return arquivo->numRegistros;
}


int NumRegistros(struct Arquivo * arquivo)
{
	if (arquivo == NULL) {
		return 0;
	}

	return arquivo->numRegistros;
}

struct Pessoa * Le(struct Arquivo * arquivo, int posicao)
{
	if (posicao > 0 && posicao <= NumRegistros(arquivo))
	{
		int numeroDeRegistros = NumRegistros(arquivo);
		struct Pessoa pessoas[numeroDeRegistros];

		if( !(arquivo->arquivo = fopen(arquivo->nomeArq, "rb")) )
		{
			fprintf(stderr, "Erro ao abrir o arquivo");
			exit(EXIT_FAILURE);
		}

		int i = 0;
		while(!feof(arquivo->arquivo))
		{

			int id, idade, removido;
			struct Pessoa * pessoa = malloc(sizeof(struct Pessoa));;

			fscanf(arquivo->arquivo, "%d\t%s\t%d\t%d\n", &id, pessoa->nome, &idade, &removido);

			pessoa->id = id;
			pessoa->idade = idade;
			pessoa->removido = removido;

			pessoas[i] = (*pessoa);

			i++;

		}

		if( fclose(arquivo->arquivo) == EOF )
		{
			fprintf(stderr, "Erro ao fechar o arquivo");
			exit(EXIT_FAILURE);
		}

		if (posicao == 0 || pessoas[posicao-1].removido == 1)
		{
			return NULL;
		}
		else
		{
			struct Pessoa * pessoaEncontrada = malloc(sizeof(struct Pessoa));
			pessoaEncontrada->id = pessoas[posicao-1].id;
			pessoaEncontrada->idade = pessoas[posicao-1].idade;
			strcpy(pessoaEncontrada->nome, pessoas[posicao-1].nome);
			pessoaEncontrada->removido = pessoas[posicao-1].removido;

			return pessoaEncontrada;
		}

	}
	else
	{
		return NULL;
	}

}

void Escreve(struct Arquivo * arquivo, struct Pessoa * pessoa, int posicao)
{
	// Nova insercao
	if (posicao > NumRegistros(arquivo))
	{
		InsereArquivo(arquivo, pessoa);
	}
	else
	{

		int foiRemovido = pessoa->removido;
		int idade = pessoa->idade;
		char nome [50];
		strcpy(nome, (*pessoa).nome);

		// Caso contrario e atualizado o registro, como no caso de uma remocao de registro
		int numeroDeRegistros = NumRegistros(arquivo);
		struct Pessoa pessoas[numeroDeRegistros];

		if( !(arquivo->arquivo = fopen(arquivo->nomeArq, "rb")) )
		{
			fprintf(stderr, "Erro ao abrir o arquivo");
			exit(EXIT_FAILURE);
		}

		int i = 0;
		while(!feof(arquivo->arquivo))
		{

			struct Pessoa * p = malloc(sizeof(struct Pessoa));

			fscanf(arquivo->arquivo, "%d\t%s\t%d\t%d\n", &(*p).id, (*p).nome, &(*p).idade, &(*p).removido);

			pessoas[i] = (*p);

			i++;

		}

		if(pessoas[posicao-1].id == pessoa->id)
		{
			pessoas[posicao-1].idade = idade;
			strcpy(pessoas[posicao-1].nome, nome);
			pessoas[posicao-1].removido = foiRemovido;
		}

		if( fclose(arquivo->arquivo) == EOF )
		{
			fprintf(stderr, "Erro ao fechar o arquivo");
			exit(EXIT_FAILURE);
		}

		arquivo->arquivo = fopen (arquivo->nomeArq, "w");

		fclose(arquivo->arquivo);

		if( !(arquivo->arquivo = fopen(arquivo->nomeArq, "w+b")) )
		{
			fprintf(stderr, "Erro ao abrir o arquivo");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < numeroDeRegistros; i++)
		{

			int removido = pessoas[i].removido;

			fprintf(arquivo->arquivo, "%d\t%s\t%d\t%d\n", pessoas[i].id, pessoas[i].nome, pessoas[i].idade, pessoas[i].removido);
		}

		if( fclose(arquivo->arquivo) == EOF )
		{
			fprintf(stderr, "Erro ao fechar o arquivo");
			exit(EXIT_FAILURE);
		}

	}
}
