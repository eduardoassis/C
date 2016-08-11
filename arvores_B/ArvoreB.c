#include "ArvoreB.h"
#include "Pessoa.h"
#include <limits.h>

#define REMOCAO_NORMAL 1
#define REMOCAO_MENOR 0
#define NAO_PROMOVE 0
#define PROMOVE 1
#define CHAVE_JA_EXISTE 2

struct ArvoreB * CriaArvoreB()
{
	struct ArvoreB *A = malloc(sizeof(struct ArvoreB));
	A->raiz = malloc(sizeof(struct No));
	A->raiz->numElementos = 0;
	int i;
	for(i = 0; i < ORDEM; i++)
		A->raiz->Filhos[i] = NULL;

	return A;
}

int BuscaArvoreB(struct ArvoreB *A, Chave chave)
{
	//Árvore não foi criada
	if(A == NULL) return 0;

	return BuscaInterno(A->raiz, chave);
}

int BuscaInterno(struct No *N, Chave chave)
{
	if(N==NULL) return 0;
	
	//Percorre as Chave do nó enquanto e for maior que a chave atual
	int i = 0;
	while(i < N->numElementos && Chave_Maior(chave, N->chaves[i]))
		i++;

	//Se não encontrou a chave, busca no filho
	if(i == N->numElementos ||  Chave_Diferente(chave, N->chaves[i]))
	{
		return BuscaInterno(N->Filhos[i], chave);
	}

	// Nó encontrado, então é retornado o valor associado a essa chave, que no caso representa a posicao do registro pessoa no arquivo
	return N->valoresAssociados[i];
}

void InsereArvoreB(struct ArvoreB *A, Chave chave, int valor)
{
	//Árvore não foi criada ainda
	if(A == NULL) return;

	int itemPromovido, valorAssociadoItemPromovido;
	struct No *filhoDItemPromovido;
	if( InsereInterno(A->raiz, chave, valor, &itemPromovido, &valorAssociadoItemPromovido, &filhoDItemPromovido) == PROMOVE)
	{
		struct No *aux = A->raiz;
		A->raiz = malloc(sizeof(struct No));
		A->raiz->chaves[0] = itemPromovido;
		A->raiz->valoresAssociados[0] = valorAssociadoItemPromovido;
		A->raiz->Filhos[0] = aux;
		A->raiz->Filhos[1] = filhoDItemPromovido;
		A->raiz->numElementos = 1;
	}
}

void Divide(struct No *N, int *itemPromovido, int *valorAssociadoItemPromovido, struct No **filhoDItemPromovido)
{
	//Declara vetores para armazenar as Chave e também os Filho da página cheia
	int Chave[ORDEM];
	int ValoresAssociados[ORDEM];
	struct No *Filho[ORDEM+1];

	//Copia os elementos para os vetores auxiliares
	int i;
	for(i = 0; i < N->numElementos && (*itemPromovido) > N->chaves[i]; i++)
	{
		Chave[i] = N->chaves[i];
		ValoresAssociados[i] = N->valoresAssociados[i];
		Filho[i] = N->Filhos[i];
	}

	Filho[i] = N->Filhos[i];
	Chave[i] = *itemPromovido;
	ValoresAssociados[i] = *valorAssociadoItemPromovido;
	Filho[i+1] = *filhoDItemPromovido;
	i++;

	for(; i < ORDEM; i++)
	{
		Chave[i] = N->chaves[i-1];
		ValoresAssociados[i] = N->valoresAssociados[i-1];
		Filho[i+1] = N->Filhos[i];
	} 

	//Declara uma nova página para fazer o split
	int meio = ORDEM/2;
	*filhoDItemPromovido = malloc(sizeof(struct No));
	(*filhoDItemPromovido)->numElementos = ORDEM-1-meio;

	//Seleciona o elemento do meio da página estourada
	*itemPromovido = Chave[meio];
	*valorAssociadoItemPromovido = ValoresAssociados[meio];

	//Copia a primeira metade para a página da esquerda
	N->numElementos = meio;
	for(i = 0; i < meio; i++)
	{
		N->chaves[i] = Chave[i];
		N->valoresAssociados[i] = ValoresAssociados[i];
		N->Filhos[i] = Filho[i];
	}
	N->Filhos[i] = Filho[i];

	//Coopia a segunda metade para apágina da direita
	int j;
	for(i = meio+1, j = 0; i < ORDEM; i++, j++)
	{
		(*filhoDItemPromovido)->chaves[j] = Chave[i];
		(*filhoDItemPromovido)->valoresAssociados[j] = ValoresAssociados[i];
		(*filhoDItemPromovido)->Filhos[j] = Filho[i];
	}
	(*filhoDItemPromovido)->Filhos[j] = Filho[i];
}

int InsereInterno(struct No *N, Chave chave, int valor, int *itemPromovido, int *valorAssociadoItemPromovido, struct No **filhoDItemPromovido)
{
	//Se for um nó nulo, insere na página pai
	if(N == NULL)
	{
		*itemPromovido = chave;
		*valorAssociadoItemPromovido = valor;
		*filhoDItemPromovido = NULL;
		return PROMOVE;
	}

	//Senão, procura o ponto i onde e deveria estar na página atual
	int i;
	for(i = 0; i < N->numElementos && Chave_Maior(chave, N->chaves[i]); i++);

	//Se o item e já estiver na página, retorna
	if(i < N->numElementos && Chave_Igual(chave, N->chaves[i]) )
	{
		printf("ERRO, item ja existente\n");
		return CHAVE_JA_EXISTE;
	}
	//Senão, manda inserir na página filha adequada
	int retorno = InsereInterno(N->Filhos[i], chave, valor, itemPromovido, valorAssociadoItemPromovido, filhoDItemPromovido);

	//Se nenhum item for promovido da página filha, retorna
	if(retorno == NAO_PROMOVE || retorno == CHAVE_JA_EXISTE)
		return retorno;

	//Se um item for promovido e houver espaço nesta página
	if(N->numElementos < ORDEM-1)
	{
		int pos = i;
		for(i = N->numElementos; i > pos; i--)
		{
			N->chaves[i] = N->chaves[i-1];
			N->valoresAssociados[i] = N->valoresAssociados[i-1];
			N->Filhos[i+1] = N->Filhos[i];
		} 
		N->chaves[pos] = *itemPromovido;
		N->valoresAssociados[pos] = *valorAssociadoItemPromovido;
		N->Filhos[pos+1] = *filhoDItemPromovido;
		N->numElementos++;
		return NAO_PROMOVE;
	}
	//Se não houver espaço, faz a divisão de página
	//itemPromovido será o pivô e filhoDItemPromovido será a nova página, o pivô será inserido na página pai
	Divide(N, itemPromovido, valorAssociadoItemPromovido, filhoDItemPromovido);
	return PROMOVE; 
}

int Remove_Interno(struct No *r, int e, int tipo)
{
	if(r == NULL) return e;

	int i = 0;
	if(tipo == REMOCAO_NORMAL) 
		while(i < r->numElementos && Chave_Maior(e,r->chaves[i]))
			i++;

	if((tipo == REMOCAO_MENOR && r->Filhos[i] == NULL)
		|| (tipo == REMOCAO_NORMAL && i < r->numElementos && Chave_Igual(e, r->chaves[i])))
	{
		//Elemento encontrado
		if(r->Filhos[i] == NULL)
		{
			//Folha, puxar todo mundo uma posição para trás
			int f = r->chaves[i];
			for(++i; i < r->numElementos; i++)
			{
				r->chaves[i-1] = r->chaves[i];
				r->valoresAssociados[i-1] = r->valoresAssociados[i];
			}
			r->numElementos--;
			return f;
		}

		//remove pos i
		r->chaves[i] = Remove_Interno(r->Filhos[i+1], e, REMOCAO_MENOR);
		i++;
	}
	else
	{
		//Elemento não encontrado
		Remove_Interno(r->Filhos[i], e, tipo);
	}

	//Verifica se o filho onde ocorreu a remoção têm o número mínimo de elementos
	if(r->Filhos[i] && r->Filhos[i]->numElementos < (ORDEM-1)/2)
	{
		if(i > 0 && r->Filhos[i-1]->numElementos > (ORDEM-1)/2)
		{
			//Recebe um item da esquerda
			int j;
			struct No *f = r->Filhos[i];
			struct No *g = r->Filhos[i-1];
			f->Filhos[f->numElementos+1] = f->Filhos[f->numElementos];
			for(j = f->numElementos; j > 0; j--)
			{
				f->chaves[j] = f->chaves[j-1];
				f->valoresAssociados[j] = f->valoresAssociados[j-1];
				f->Filhos[j] = f->Filhos[j-1];
			}
			f->chaves[0] = r->chaves[i-1];
			f->valoresAssociados[0] = f->valoresAssociados[i-1];
			f->Filhos[0] = g->Filhos[g->numElementos];
			(f->numElementos)++;
			r->chaves[i-1] = g->chaves[g->numElementos-1];
			r->valoresAssociados[i-1] = g->valoresAssociados[g->numElementos-1];
			(g->numElementos)--;
		}
		else if(i < r->numElementos && r->Filhos[i+1]->numElementos > (ORDEM-1)/2)
		{
			//Recebe um item da direita
			int j;
			struct No *f = r->Filhos[i];
			struct No *g = r->Filhos[i+1];
	
			f->chaves[f->numElementos] = r->chaves[i];
			f->valoresAssociados[f->numElementos] = r->valoresAssociados[i];
			f->Filhos[f->numElementos+1] = g->Filhos[0];
			(f->numElementos)++;
			r->chaves[i] = g->chaves[0];
			for(j = 1; j < g->numElementos; j++)
			{
				g->chaves[j-1] = g->chaves[j];
				g->valoresAssociados[j-1] = g->valoresAssociados[j];
				g->Filhos[j-1] = g->Filhos[j];
			}
			g->Filhos[j-1] = g->Filhos[j];
			(g->numElementos)--; 
		}
		else
		{
			//Merge
			int j, k;

			struct No *f, *g;

			if(i>0)
			{
				f = r->Filhos[i];
				g = r->Filhos[i-1];
				i--;
			}
			else
			{
				g = r->Filhos[i];
				f = r->Filhos[i+1];
			}

			g->chaves[g->numElementos] = r->chaves[i];
			(g->numElementos)++;
			for(j = 0; j < f->numElementos; j++)
			{
				g->chaves[g->numElementos] = f->chaves[j];
				g->valoresAssociados[g->numElementos] = f->valoresAssociados[j];
				g->Filhos[g->numElementos] = f->Filhos[j];
				(g->numElementos)++;
			}
			g->Filhos[g->numElementos] = f->Filhos[j];
			free(f);
			for(; i < r->numElementos-1; i++)
			{
				r->chaves[i] = r->chaves[i+1];
				r->valoresAssociados[i] = r->valoresAssociados[i+1];
				r->Filhos[i+1] = r->Filhos[i+2];
			}
			r->numElementos--;
		}
	}
	return e;
}

int RemoveArvoreB(struct ArvoreB *arvore, Chave e)
{

	if(arvore == NULL) return 0;

	int posicaoNoArquivo = BuscaArvoreB(arvore, e);

	Remove_Interno(arvore->raiz, e, REMOCAO_NORMAL);

	if(arvore->raiz->numElementos == 0)
	{
		struct No *r = arvore->raiz;
		arvore->raiz = arvore->raiz->Filhos[0];
		free(r);
	}


	return posicaoNoArquivo;

}



