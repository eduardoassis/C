#include <stdio.h>
#include "Dicionario.h"

int MAX(int A, int B)
{
	if(A > B) return A;
	else return B;
}

struct Dicionario * CriaDicionario()
{
	struct Dicionario * D = malloc(sizeof(struct Dicionario));
	D->raiz = NULL;
	return D;
}

struct No * CriaNo(struct Item I)
{
	struct No * NovoNo = malloc(sizeof(struct No));	
	NovoNo->I = I;
	NovoNo->altura;
	NovoNo->esquerda = NULL;
	NovoNo->direita = NULL;
	return NovoNo;
}

struct No * InserirRecursivo(struct No * anda, struct Item I)
{
	if(anda == NULL)
	{
		anda =  CriaNo(I);
	}
	else if(ChaveMenor(GetChave(I), GetChave(anda->I)))
	{
		anda->esquerda = InserirRecursivo(anda->esquerda, I);
	}
	else
	{
		anda->direita = InserirRecursivo(anda->direita, I);
	}
	return anda;
}

int BuscarRecursivo(struct No * anda, Chave c)
{
	if(anda == NULL) 
		return 0; 
	else if(ChaveIgual(GetChave(anda->I), c))
		return 1;
	else if(ChaveMenor(c, GetChave(anda->I)))
		return 1 + BuscarRecursivo(anda->esquerda, c);
	else
		return 1 + BuscarRecursivo(anda->direita, c);
}

int Buscar(struct Dicionario * D, Chave c)
{
	return BuscarRecursivo(D->raiz, c);
}

int maior(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

// Função para calcular a altura de um dado nó
int RecuperaAlturaNo(struct No * no)
{
	if (no == NULL)
		return -1;
	else
		return no->altura;
}

struct No * RemoverRecursivo(struct No * anda, Chave c, struct Item *I)
{
	if(anda == NULL)
	{
		return NULL;
	}
	else if(ChaveMenor(c, GetChave(anda->I)))
	{
		anda->esquerda = RemoverRecursivo(anda->esquerda, c, I);
		anda->altura = maior(RecuperaAlturaNo(anda->esquerda), RecuperaAlturaNo(anda->direita)) + 1;
		return anda;
	}
	else if(ChaveMaior(c, GetChave(anda->I)))
	{
		anda->direita = RemoverRecursivo(anda->direita, c, I);
		anda->altura = maior(RecuperaAlturaNo(anda->esquerda), RecuperaAlturaNo(anda->direita)) + 1;
		return anda;
	}
	else if(anda->esquerda == NULL && anda->direita == NULL)
	{
		*I = anda->I;
		free(anda);
		return NULL;
	}
	else if(anda->esquerda != NULL && anda->direita == NULL)
	{
		*I = anda->I;
		struct No* esq = anda->esquerda;
		free(anda);
		return esq;
	}
	else if(anda->esquerda == NULL && anda->direita != NULL)
	{
		*I = anda->I;
		struct No* dir = anda->direita;
		free(anda);
		return dir;
	}
	else
	{
		*I = anda->I;
		struct No * maioresquerda = anda->esquerda;
		while(maioresquerda->direita != NULL)
			maioresquerda = maioresquerda->direita;
		anda->esquerda = RemoverRecursivo(anda->esquerda, GetChave(maioresquerda->I), &(anda->I));
		return anda;
	}
}

void ImprimeRecursivo(struct No * anda)
{
	if(anda == NULL) return;

	ImprimeRecursivo(anda->esquerda);
	ImprimeChave(GetChave(anda->I));
	printf(" ");
	ImprimeRecursivo(anda->direita);
}

void Imprime(struct Dicionario * D)
{
	ImprimeRecursivo(D->raiz);	
	printf("\n");
}

struct No * RotacionaDireita(struct No * Pai)
{
	struct No * Filho = Pai->esquerda;
	Pai->esquerda = Filho->direita;
	Filho->direita = Pai;
	return Filho;
}

struct No * RotacionaEsquerda(struct No * Pai)
{
	struct No * Filho = Pai->direita;
	Pai->direita = Filho->esquerda;
	Filho->esquerda = Pai;
	return Filho;
}

// Função que calcula o fator de balanceamento de um nó
int FatorDeBalanceamentoDoNo(struct No * no)
{
	return labs(RecuperaAlturaNo(no->esquerda) - RecuperaAlturaNo(no->direita));
}

// Rotação LL
void RotacaoSimplesADireita(struct No ** noRaiz)
{
	struct No * no;

	no = (*noRaiz)->esquerda;
	(*noRaiz)->esquerda = no->direita;
	no->direita = (*noRaiz);

	(*noRaiz)->altura = maior( RecuperaAlturaNo((*noRaiz)->esquerda), RecuperaAlturaNo((*noRaiz)->direita) ) +1;

	no->altura = maior(RecuperaAlturaNo(no->esquerda), (*noRaiz)->altura) + 1;

	(*noRaiz) = no;
}

// Rotação RR
void RotacaoSimplesAEsquerda(struct No ** noRaiz)
{
	struct No *no;

	no = (*noRaiz)->direita;
	(*noRaiz)->direita = no->esquerda;
	no->esquerda = (*noRaiz);

	(*noRaiz)->altura = maior(RecuperaAlturaNo((*noRaiz)->esquerda), RecuperaAlturaNo((*noRaiz)->direita)) + 1;

	no->altura = maior(RecuperaAlturaNo(no->direita), (*noRaiz)->altura) + 1;

	(*noRaiz) = no;
}

// Rotação LR
void RotacaoDuplaADireita(struct No ** noRaiz)
{
	RotacaoSimplesAEsquerda(&(*noRaiz)->esquerda);
	RotacaoSimplesADireita(noRaiz);
}

// Rotação RL
void RotacaoDuplaAEsquerda(struct No ** noRaiz)
{
	RotacaoSimplesADireita(&(*noRaiz)->direita);
	RotacaoSimplesAEsquerda(noRaiz);
}

int InsereAVL(struct No ** noRaiz, struct Item I)
{
	int resultado;

	if (*noRaiz == NULL)
	{

		struct No * NovoNo = CriaNo(I);

		if (NovoNo == NULL)
		{
			return 0;
		}

		*noRaiz = NovoNo;
		return 1;
	}

	struct No * noAtual = * noRaiz;

	if (ChaveMenor(GetChave(I), GetChave(noAtual->I)))
	{
		if ((resultado = InsereAVL(&(noAtual->esquerda), I)) == 1)
		{
			if (FatorDeBalanceamentoDoNo(noAtual) >= 2)
			{
				if ( ChaveMenor(GetChave(I), GetChave((*noRaiz)->esquerda->I)))
				{
					RotacaoSimplesADireita(noRaiz);
				}
				else
				{
					RotacaoDuplaADireita(noRaiz);
				}
			}
		}
	}
	else
	{
		if ((resultado = InsereAVL(&(noAtual->direita), I)) == 1)
		{
			if (FatorDeBalanceamentoDoNo(noAtual) >= 2)
			{
				if ( ChaveMenor(GetChave( (*noRaiz)->direita->I ), GetChave(I)))
				{
					RotacaoSimplesAEsquerda(noRaiz);
				}
				else
				{
					RotacaoDuplaAEsquerda(noRaiz);
				}
			}
		}

	}

	noAtual->altura = maior( RecuperaAlturaNo(noAtual->esquerda), RecuperaAlturaNo(noAtual->direita) ) + 1;

	return resultado;
}

struct No* buscaMenorFilho(struct No* noPai)
{
    struct No *auxiliar1 = noPai;

    struct No *auxiliar2 = noPai->esquerda;

    while(auxiliar2 != NULL)
    {

        auxiliar1 = auxiliar2;
        auxiliar2 = auxiliar2->esquerda;

    }

    return auxiliar1;
}

int RemoveAVL(struct No ** noRaiz, struct Item I){

	// Tramatamento para valor inexistente
	if(*noRaiz == NULL)
	{
	    return 0;
	}

    int resultado;

	if(ChaveMenor(GetChave(I), GetChave((*noRaiz)->I)))
	{
	    if((resultado = RemoveAVL(&(*noRaiz)->esquerda, I)) == 1)
	    {
            if(FatorDeBalanceamentoDoNo(*noRaiz) >= 2)
            {
                if(RecuperaAlturaNo((*noRaiz)->direita->esquerda) <= RecuperaAlturaNo((*noRaiz)->direita->direita))
                {
                    RotacaoSimplesAEsquerda(noRaiz);
                }
                else
                {
                	RotacaoDuplaAEsquerda(noRaiz);
                }
            }
	    }
	}

	if(ChaveMenor(GetChave((*noRaiz)->I), GetChave(I)))
	{
	    if((resultado = RemoveAVL(&(*noRaiz)->direita, I)) == 1)
	    {
            if(FatorDeBalanceamentoDoNo(*noRaiz) >= 2)
            {
            	if(RecuperaAlturaNo((*noRaiz)->esquerda->direita) <= RecuperaAlturaNo((*noRaiz)->esquerda->esquerda))
                {
                	RotacaoSimplesADireita(noRaiz);
                }
                else
                {
                	RotacaoDuplaADireita(noRaiz);
                }
            }
	    }
	}

	if( ChaveIgual( GetChave((*noRaiz)->I), GetChave(I)))
	{
		// Se o nó possuir 1 ou nenhum filho
	    if(((*noRaiz)->esquerda == NULL || (*noRaiz)->direita == NULL))
	    {

			struct No *antigaRaiz = (*noRaiz);

			if((*noRaiz)->esquerda != NULL)
			{
                *noRaiz = (*noRaiz)->esquerda;
			}
            else
            {
                *noRaiz = (*noRaiz)->direita;
            }
			free(antigaRaiz);

		}else { 
			// Se o nó possuim 2 filhos

			struct No* menorFilhoEncontrado = buscaMenorFilho((*noRaiz)->direita);

			(*noRaiz)->I = menorFilhoEncontrado->I;

			RemoveAVL(&(*noRaiz)->direita, (*noRaiz)->I);

            if(FatorDeBalanceamentoDoNo(*noRaiz) >= 2)
            {
				if(RecuperaAlturaNo((*noRaiz)->esquerda->direita) <= RecuperaAlturaNo((*noRaiz)->esquerda->esquerda))
				{
					RotacaoSimplesADireita(noRaiz);
				}
				else
				{
					RotacaoDuplaADireita(noRaiz);
				}
			}
		}
		if (*noRaiz != NULL)
            (*noRaiz)->altura = maior( RecuperaAlturaNo((*noRaiz)->esquerda), RecuperaAlturaNo((*noRaiz)->direita) ) + 1;
		return 1;
	}

	(*noRaiz)->altura = maior(RecuperaAlturaNo((*noRaiz)->esquerda), RecuperaAlturaNo((*noRaiz)->direita)) + 1;

	return resultado;
}

void Inserir(struct Dicionario * D, struct Item I)
{
	// D->raiz = InserirRecursivo(D->raiz, I);
	InsereAVL(&D->raiz, I);
}

struct Item Remover(struct Dicionario * D, Chave c)
{
	struct Item I;
	// D->raiz = RemoverRecursivo(D->raiz, c, &I);
	SetChave(&I, c);
	RemoveAVL(&D->raiz, I);
	return I;
}