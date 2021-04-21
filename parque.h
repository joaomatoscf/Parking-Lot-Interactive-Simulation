/* parque.h */

#ifndef PARQUE_H_
#define PARQUE_H_
#include <stdio.h>
#include <math.h>
#include <g2.h>
#include <g2_X11.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 200

typedef struct parque{
	char codigo[MAX];
	int x;
	int y;
	int largura;
	int altura;
	char cor[MAX];
	char nome[MAX];
	char matricula[MAX];
} PARQUE;

typedef struct lugares {
	PARQUE dado;
	struct lugares *prox;
	struct lugares *ant;
} LUGARES;

typedef struct ponto {
	float x;
	float y;
} PONTO;

typedef struct linha {
	PONTO ponto;
	struct linha *prox;
	struct linha *ant;
} LINHA;

typedef struct carro {
	char matricula[MAX];
	LUGARES *estacionamento;	/*lugar onde foi estacionado*/
	float x;
	float y;
	int cor;
	int ocupado;	/*se tem um robot associado ou um estacionamento*/
	int entrada;	/*se esta a entrar*/
	int saida;	/*se esta a sair*/
} CARRO;

typedef struct lista {
	CARRO carro;
	struct lista *prox;
	struct lista *ant;
} LISTA;

typedef struct robot {
	float x;
	float y;
	float vel;			/*velocidade a que se move*/
	float vel2;			/*velocidade antes da alteracao */
	int mov;			/*se esta em movimento */
	int ocupado;			/*se transporta carro */
	int choque;			/*se ia chocar */
	int cor_linha;
	char cor[MAX];
	LINHA *linha;			/*linha do robot */
	LUGARES *estacionamento;	/*ZER do robot*/
	LISTA *carro;			/*carro a transportar */
} ROBOT;

void DefinirARG(int argc, char *argv[],int botao[3],char ficheiro[MAX]);

LUGARES *ProcuraCodigo(LUGARES *topo,char codigo[MAX]);

int NumeroConversoes(int realizado,int esperado,int linha);

LUGARES *AlocaLUGARES(LUGARES *topo,LUGARES *ap);

LUGARES *InsereLUGARES(LUGARES *topo, char info[MAX],char codigo[MAX],int linha);

LUGARES *LerFicheiro(char ficheiroABERTO[MAX],char ficheiro2[MAX],LUGARES *topo);

ROBOT *DadosROBOT(ROBOT *vetor,LUGARES *topo,int *dim);

LISTA *InsereLISTA(LISTA *lista,LUGARES *aux);

LISTA *DadosLISTA(LISTA *lista,LUGARES *topo);

void DescobreESJG(LUGARES **topo,int esjg[5]);

void MultiplicaPorES(ROBOT *vetor, int dim, LISTA *lista,int esjg[5]);

int EstacionadosROBOT(ROBOT *vetor,int dim,int esjg[5]);

int Clique(int d,int dev, int mouse[3],int botao[3]);

int CliquePosicao(int clique, int mouse[3], int dir,int cima, int esq, int baixo);

LISTA *TrocaLISTA(LISTA *a,LISTA* b);

LISTA *OrdenaLISTA(LISTA *topo);

int NumeroCor(char cor[MAX]);

void DesenhaFicheiro(int d,LUGARES **topo,int esjg[5]);

void DesenhaLinha(int d, ROBOT *vetor,int dim,int robotn);

void DesenhaRobot(int d,int esjg[5],ROBOT *vetor,int dimv);

void DesenhaCarros(int d,LISTA *lista,int esjg[5]);

void DesenhaMenuParte1(int d,int esjg[5],ROBOT *vetor,int robotm);

void DesenhaMenuParte2(int d,int esjg[5],LUGARES *topo,LISTA *lista);

int MovimentoROBOT(ROBOT *vetor,int robotn);

void ApagarFimLINHA(ROBOT vetor);

int RestricoesJanela(ROBOT *vetor,int i,int esjg[5]);

int RestricoesZR(LUGARES **topo,ROBOT *vetor,int i,int esjg[5]);

int RestricoesZE(LUGARES **topo,ROBOT *vetor,int i,int esjg[5]);

void MovimentoRestricoes(LUGARES **topo,ROBOT *vetor,int dim,int esjg[5],int robotn);

LINHA *InsereLINHA(LINHA *topo,int x ,int y,int para);

int FazLINHA(LUGARES **topo,int mouse[3],int esjg[5],ROBOT *vetor,int dim,int robotn);

void RobotMOVECarro(LISTA *lista,ROBOT *vetor,int dimv,int esjg[5]);

LISTA *EstacionaCarro(LISTA *lista,ROBOT *vetor,int i,LUGARES *ap,int esjg[5]);

int RestricoesEstacionar(LUGARES *aux,ROBOT *vetor,int i,int esjg[5]);

LISTA *RobotINCarro(LUGARES *topo,LISTA *lista,ROBOT *vetor,int dimv,int esjg[5]);

LISTA *RobotOUTCarro(LUGARES *topo,LISTA *lista,ROBOT *vetor,int dimv,int esjg[5]);

int SelecionarROBOT(ROBOT *vetor,int dim,int mouse[3],int esjg[5],int robotm,int robotn);

int MudaVelocidadeManual(ROBOT *vetor,int robotm,int mouse[3],int esjg[5],int a);

void ControlaVelocidade(ROBOT *vetor,int dim);

int ParaROBOT(ROBOT *vetor,int dim,int esjg[5]);

void MudaVelocidadeAutomatico(ROBOT *vetor,int dim,int esjg[5]);

LISTA *RetiraCarro(LISTA *lista,LISTA *aux,int esjg[5]);

LISTA *CorZE(LUGARES *topo,LISTA *lista,int esjg[5],int pisca);

void PiscaROBOT(ROBOT *vetor,int robot,int pisca);

void CorROBOT(ROBOT *vetor,int dim,int esjg[5],int pisca,int robotn);

void MostrarLIST(LISTA *lista);

int LIST(LISTA *lista,int mouse[3], int esjg[5],int a);

int VerificarMatricula(char matricula[MAX],LISTA *lista);

int VerificarIN(LISTA *lista,LUGARES **topo,int esjg[5]);

int VerificarEstacionamento(LISTA *lista,LUGARES **topo);

LISTA * IN(LISTA *lista,LUGARES **topo,int esjg[5],int mouse[3]);

LISTA *OUT(LISTA *lista,int esjg[5],int mouse[3]);

void AtualizaFicheiro(LUGARES *topo,char ficheiro[MAX]);

LUGARES *ApagarLUGARES(LUGARES *topo);

void ApagarLINHA(ROBOT *vetor,int dim);

LISTA *ApagarLISTA(LISTA *topo);

void DesenhaGrafico(int d, int dev, int mouse[3], int esjg[5],int botao[2], LUGARES **topo,ROBOT *vetor, int dimv,LISTA **lista,int pisca,int *robotn,int *robotm,int *list,int *velocidade);
	
void AbreGrafico(int botao[2],LUGARES **topo,ROBOT *vetor,LISTA *lista,int dimv);

#endif
