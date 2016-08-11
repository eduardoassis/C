#include "ArquivoIndexado.h"
#include "Arquivo.h"
#include "ArvoreB.h"

struct ArquivoIndexado * CriaArquivoIndexado(char * nome)
{
	struct ArquivoIndexado * arquivoIndexado = malloc(sizeof(struct ArquivoIndexado));

	arquivoIndexado->arquivo = CriaArquivo(nome);
	arquivoIndexado->indice = CriaArvoreB();

	int numeroDeRegistros  = NumRegistros(arquivoIndexado->arquivo);

	int i;

	for (i = 1; i <= numeroDeRegistros; i++)
	{
		struct Pessoa * pessoa = Le(arquivoIndexado->arquivo, i);


		if (pessoa != NULL && pessoa->removido == 0)
		{
			InsereArvoreB(arquivoIndexado->indice, pessoa->id, i);
		}
	}

	return arquivoIndexado;

}

void Insere(struct ArquivoIndexado * arquivoIndexado, struct Pessoa * pessoa)
{

	int posicao;

	// Se nao for encontrada a pessoa na arvore/arquivo, considera-se que esta sendo realizado o cadastro de uma nova pessoa
	if(Buscar(arquivoIndexado, pessoa->id) == NULL)
	{
		posicao = InsereArquivo(arquivoIndexado->arquivo, pessoa);
		InsereArvoreB(arquivoIndexado->indice, pessoa->id, posicao);
	}
	else
	{
		// Senao considera-se que esta sendo realizado a atualizacao dos dados de uma pessoa

		int posicao = BuscaArvoreB(arquivoIndexado->indice, pessoa->id);
		Escreve(arquivoIndexado->arquivo, pessoa, posicao);

	}

}

struct Pessoa * Buscar(struct ArquivoIndexado * arquivoIndexado, Chave chave)
{
	int posicao = BuscaArvoreB(arquivoIndexado->indice, chave);
	struct Pessoa * pessoa = Le(arquivoIndexado->arquivo, posicao);

	return pessoa;
}

struct Pessoa * Remover(struct ArquivoIndexado * arquivoIndexado, Chave chave)
{

	int posicao = RemoveArvoreB(arquivoIndexado->indice, chave);

	struct Pessoa * pessoa = Le(arquivoIndexado->arquivo, posicao);

	if (pessoa != NULL) {
		pessoa->removido = 1;

		Escreve(arquivoIndexado->arquivo, pessoa, posicao);

		return pessoa;
	}


	return NULL;
}
