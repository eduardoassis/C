#include "Dicionario.h"

int MAX(int A, int B)
{
	if(A > B) return A;
	else return B;
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

struct Dicionario * CriaDicionario()
{
	struct Dicionario * D = malloc(sizeof(struct Dicionario));
	D->raiz = NULL;
	return D;
}

struct No * CriaNo(struct Item I)
{
	struct No * NovoNo = malloc(sizeof(struct No));	
	NovoNo->numeroDeAcesso = 0;
	NovoNo->encontrado = 0;
	NovoNo->altura = 0;
	NovoNo->I = I;
	NovoNo->esquerda = NULL;
	NovoNo->direita = NULL;
	return NovoNo;
}

struct No * InserirRecursivo(struct No * anda, struct Item I)
{
	if(anda == NULL)
	{
		anda =  CriaNo(I);
		anda->altura = 1 + maior( RecuperaAlturaNo(anda->esquerda), RecuperaAlturaNo(anda->direita) );
	}
	else if(ChaveMenor(GetChave(I), GetChave(anda->I)))
	{
		anda->esquerda = InserirRecursivo(anda->esquerda, I);
		anda->altura = 1 + maior( RecuperaAlturaNo(anda->esquerda), RecuperaAlturaNo(anda->direita) );
	}
	else
	{
		anda->direita = InserirRecursivo(anda->direita, I);
		anda->altura = 1 + maior( RecuperaAlturaNo(anda->esquerda), RecuperaAlturaNo(anda->direita) );
	}
	return anda;
}

void Inserir(struct Dicionario * D, struct Item I)
{
	D->raiz = InserirRecursivo(D->raiz, I);
}

// Rotação LL
void RotacaoSimplesADireita(struct No ** noRaiz)
{
	struct No * no;
	no = (*noRaiz)->esquerda;
	(*noRaiz)->esquerda = no->direita;
	no->direita = (*noRaiz);
	no->pai = (*noRaiz)->pai;
	(*noRaiz)->pai = &no;

	int alturaAux = no->altura;
	no->altura = (*noRaiz)->altura;
	(*noRaiz)->altura = alturaAux;

	(*noRaiz) = no;

}

// Rotação RR
void RotacaoSimplesAEsquerda(struct No ** noRaiz)
{
	struct No *no;
	no = (*noRaiz)->direita;

	(*noRaiz)->direita = no->esquerda;
	no->esquerda = (*noRaiz);

	int alturaAux = no->altura;
	no->altura = (*noRaiz)->altura;
	(*noRaiz)->altura = alturaAux;

	(*noRaiz) = no;
}

int BuscarRecursivo(struct No ** anda, Chave c, struct No * procurado)
{
	int nivel = 0;

	if(anda == NULL) 
	{
		printf("nao encontrado\n");
		return 0; 
	}
	else if(ChaveIgual(GetChave((*anda)->I), c))
	{
		(*anda)->numeroDeAcesso++;
		printf("encontrado\n");
		procurado->encontrado = 1;
		return 1;
	}
	else if(ChaveMenor(c, GetChave((*anda)->I)))
	{
		nivel += 1 + BuscarRecursivo(&(*anda)->esquerda, c, procurado);

		if (procurado->encontrado == 1)
		{
			while((*anda)->esquerda !=NULL && (*anda)->esquerda->numeroDeAcesso > (*anda)->numeroDeAcesso)
			{
				RotacaoSimplesADireita(&(*anda));
			}
		}

	}
	else
	{
		nivel += 1 + BuscarRecursivo(&(*anda)->direita, c, procurado);

		if (procurado->encontrado == 1)
		{
			while((*anda)->direita !=NULL && (*anda)->direita->numeroDeAcesso > (*anda)->numeroDeAcesso)
			{
				RotacaoSimplesAEsquerda(&(*anda));
			}
		}

	}
	return nivel;
}

int Buscar(struct Dicionario * D, Chave c)
{
	struct Item I;
	SetChave(&I, c);
	return BuscarRecursivo(&D->raiz, c, CriaNo(I));
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

struct Item Remover(struct Dicionario * D, Chave c)
{
	struct Item I;
	D->raiz = RemoverRecursivo(D->raiz, c, &I);
	return I;
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


