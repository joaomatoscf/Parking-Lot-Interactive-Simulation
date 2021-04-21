/* main.c */

#include "parque.h"

int main(int argc, char *argv[])
{	
	char ficheiroABERTO[MAX]="config.txt",ficheiro2[MAX]={'\0'};	/* ficheiros de configuracao */ 					
	LUGARES *topo=NULL;
	ROBOT *vetor=NULL;
	LISTA *lista=NULL;
	int dimv=0;
	int botao[3];

	DefinirARG(argc,argv,botao,ficheiro2);	/* Define os argumentos passados para a main */

	topo=LerFicheiro(ficheiroABERTO,ficheiro2,topo); 
	vetor=DadosROBOT(vetor,topo,&dimv);	
	lista=DadosLISTA(lista,topo);

	AbreGrafico(botao,&topo,vetor,lista,dimv);

	AtualizaFicheiro(topo,ficheiroABERTO);
	topo=ApagarLUGARES(topo);
	ApagarLINHA(vetor,dimv);
	lista=ApagarLISTA(lista);
	free(vetor);

	return 0;
}
