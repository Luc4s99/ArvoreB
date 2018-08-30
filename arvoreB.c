/**********************************************************************************
1ª Prática Estrutura de dados 2 

Autor: Lucas Mateus Menezes Silva - 0035334
IFMG - Instituto Federal de Educação Ciência e Tecnologia de Minas Gerais - Campus Formiga
**********************************************************************************/ 

#include <stdlib.h>
#include <stdio.h>
#define M 2		
#define MM 4	//Definindo ordem da árvore

typedef struct TipoPagina *TipoApontador;	//Apontador para páginas da árvore

typedef struct TipoPagina{	//Estrutura com elementos da página
	int n;					//Numero de elementos da pagina
	int r[MM];				//Vetor de elementos da pagina
	TipoApontador p[MM+1];	//Vetor de apontadores da pagina
}TipoPagina;

void Inicializa(TipoApontador *Ap);		//Inicialização do ponteiro
int Pesquisa(int x, TipoApontador Ap);	//Função de pesquisa	
void Ins(int Reg, TipoApontador Ap, short *cresceu, int *RegRetorno, TipoApontador *ApRetorno);
void InsereNaPagina(TipoApontador Ap,int Reg, TipoApontador ApDir);	//
void Insere(int Reg, TipoApontador *Ap);

int main(){
	int num, opcao;
	TipoApontador *ptr;
	Inicializa(ptr);

	printf("Simulador de arvore B\n\n");
	while(1){
		printf("1- Inserir 2- Buscar: \n");
		scanf("%d", &opcao);

		if (opcao == 1){
			printf("\n\nDigite o numero a ser inserido: \n");
			scanf("%d", &num);

			Insere(num, ptr);
		}else if(opcao == 2){
			printf("\n\nDigite o numero a ser pesquisado: \n");
			scanf("%d", &num);

			if(Pesquisa(num, *ptr)){
				printf("Elemento %d encontrado\n", num);
				getchar();
			}else{
				printf("Elemento %d nao encontrado\n", num);
				getchar();
			}
		}else{
			break;
		}
	}
}

void Inicializa(TipoApontador *Ap){	//Inicializando ponteiro
	*Ap = NULL;
}

/*Procedimento de pesquisa*/

int Pesquisa(int x, TipoApontador Ap){
	int i = 1;
	if(Ap == NULL){	//Verifica se o apontador para a árvore está referenciado
		return 0;	//Caso não esteja o valor não foi encontrado
	}
	while((i < Ap->n) && (x > Ap->r[i-1]))	//Enquanto for menor que o numero de registros e menor que o elementos na posicao i-1
		i++;	//O i é incrementado para percorrer o vetor
	if(x == Ap->r[i-1]){
		return 1;	//Caso o dado seja encontrado, retorna-se 1
	}
	//Caso não tenha sido encontradol chamadas recursivas são iniciadas
	if(x < Ap->r[i-1]){	//Chamada recursiva caso o numero seja menor
		Pesquisa(x, Ap->p[i-1]);
	}else{	//Chamada recursiva caso o numero seja maior
		Pesquisa(x, Ap->p[i]);
	}
}

/*Primeiro refinamento Insere*/

void Ins(int Reg, TipoApontador Ap, short *cresceu, int *RegRetorno, TipoApontador *ApRetorno){
	long i = 1;
	long j;
	int Aux;
	TipoApontador ApTemp;
	if (Ap == NULL){
		*cresceu = 1;
		*RegRetorno = Reg;
		*ApRetorno = NULL;
		return;
	}
	while((i < Ap->n) && (Reg > Ap->r[i - 1])){ //Procurando pelo local a ser inserido o dado
		i++;
	}
	if (Reg == Ap->r[i - 1]){	//Caso dois elementos iguais sejam inseridos
		printf("ERRO: Registro %d ja existente\n", Reg);
		*cresceu = 0;	//Variael cresceu fica com falso, pois o elemento nao foi adicionado
		return;
	}
	if (Reg < Ap->r[i - 1]){
		Ins(Reg, Ap->p[i - 1], cresceu, RegRetorno, ApRetorno);
	}else{
		Ins(Reg, Ap->p[i], cresceu, RegRetorno, ApRetorno);
	}
	if (!*cresceu){
		return;
	}
	if (Ap->n < MM){  // Verificando se a pagina tem espaco 
    	InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    	*cresceu = 0;
    	return;
  	}
  	//Split ou cisão na pagina
  	ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));	//Criando nova pagina
  	ApTemp->n = 0;
  	ApTemp->p[0] = NULL;
  	//Transferindo metade dos elementos de Ap para ApTemp
  	if (i <= M + 1){
  		InsereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM]);
  		Ap->n--;
  		InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  	}else{
  		InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  	}
  	for (j = M + 2; j <= MM; j++){
    	InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);
  	}
    //Atribuindo registro do meio a RegRetorno
  	Ap->n = M;
  	ApTemp->p[0] = Ap->p[M + 1];
  	*RegRetorno = Ap->r[M];
  	//Atribuindo ApTemp a ApRetorno
  	*ApRetorno = ApTemp;
}

/*Inserindo na pagina*/

void InsereNaPagina(TipoApontador Ap,int Reg, TipoApontador ApDir){
	int NaoAchou, k;	
	k = Ap->n;	//Recebe o tamanho da pagina
	NaoAchou = (k > 0);	//Caso saj verdadeiro recebe 1, senao recebe 0
	while(NaoAchou){	//Caso o tamanho da pagina seja maior que 0, é inicida a inserção
		if (Reg >= Ap->r[k-1]){	//Se o numero for igual ou maior sua posição de inserção foi encontrada
			NaoAchou = 0;
			break;
		}
		Ap->r[k] = Ap->r[k-1];	//Deslocando dados no vetor
		Ap->p[k+1] = Ap->p[k];
		k--;
		if (k < 1){	//Se k for menor que 1, a pagina foi percorrida e a posição nao foi encontrada
			NaoAchou = 0;
		}
	}
	Ap->r[k] = Reg;
	Ap->p[k+1] = ApDir;
	Ap->n++;
}

/*Refinamento final*/

void Insere(int Reg, TipoApontador *Ap){
	short cresceu;
	int RegRetorno;
	TipoPagina *ApRetorno, *ApTemp;
	Ins(Reg, *Ap, &cresceu, &RegRetorno, &ApRetorno);
	if (cresceu){
		ApTemp = (TipoPagina*) malloc(sizeof(TipoPagina));
		ApTemp->n = 1;
		ApTemp->r[0] = RegRetorno;
		ApTemp->p[1] = ApRetorno;
		ApTemp->p[0] = *Ap;
		*Ap = ApTemp;
	}
}