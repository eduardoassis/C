#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "Pessoa.h"
#include "Arquivo.h"
#include "ArquivoIndexado.h"

int main()
{
	struct ArquivoIndexado * arquivoIndexado = CriaArquivoIndexado("/tmp/text.txt");
	int continuar=1;

	do
	{
		printf("1. Inserir uma pessoa\n");
		printf("2. Remover uma pessoa\n");
		printf("3. Buscar uma pessoa\n");
		printf("4. Alterar uma pessoa\n");
		printf("0. Sair\n");

		scanf("%d", &continuar);
		system("clear");

		char nome[50];
		int id;
		int idade;
		struct Pessoa * pessoa;

		switch(continuar)
		{
			case 1:

				printf("Id (um numero):\n");
				scanf("%d", &id);
				printf("Nome:\n");
				scanf("%s", nome);
				printf("Idade:\n");
				scanf("%d", &idade);

				pessoa = CriaPessoa(nome, idade, id);

				Insere(arquivoIndexado, pessoa);

				break;

			case 2:

				printf("Insira o id da pessoa:\n");
				scanf("%d", &id);

				pessoa = Remover(arquivoIndexado, id);

				if (pessoa != NULL)
				{
					printf("Pessoa removida\n");
				}
				else
				{
					printf("Pessoa nao encontrada para remocao\n");
				}

				break;

			case 3:

				printf("Insira o id da pessoa:\n");
				scanf("%d", &id);

				struct Pessoa * p = Buscar(arquivoIndexado, id);

				if (p != NULL)
				{
					printf("Id: %d\n", p->id);
					printf("Nome: %s\n", p->nome);
					printf("Idade: %d\n", p->idade);
				}
				else
				{
					printf("Registro nao encontrado\n");
				}

				break;
			case 4:

				printf("Insira o id da pessoa:\n");
				scanf("%d", &id);

				struct Pessoa * pessoa_atualizar = Buscar(arquivoIndexado, id);

				if (pessoa_atualizar != NULL)
				{
					struct Pessoa * p = malloc(sizeof(struct Pessoa));
					printf("Nome para atualizar:\n");
					scanf("%s", nome);
					printf("Idade para atualizar:\n");
					scanf("%d", &idade);

					p->id = id;
					p->idade = idade;
					strcpy(p->nome, nome);
					p->removido = 0;

					Insere(arquivoIndexado, p);

				}
				else
				{
					printf("Registro nao encontrado\n");
				}



				break;
			case 0:
				break;

			default:
				printf("Digite uma opcao valida\n");
		}
	} while(continuar);
}
