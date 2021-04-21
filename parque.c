/* parque.c */

#include "parque.h"

void DefinirARG(int argc, char *argv[],int botao[3],char ficheiro[MAX])
{
	int i,e=0,d=0,np=0,f=0;

	for(i=0;i<argc;i=i+1)
		if(strcmp((argv[i]),"-np")==0)	
			np=i;

	for(i=0;i<argc;i=i+1)
		if(strcmp((argv[i]),"-e")==0)	
			e=i;

	for(i=0;i<argc;i=i+1)
		if(strcmp((argv[i]),"-d")==0)	
			d=i;

	for(i=0;i<argc;i=i+1)
		if(strcmp((argv[i]),"-f")==0)	
			f=i;

	if(np==0)
		botao[0]=0;
	else
		sscanf(argv[d+1],"%d",&(botao[0]));

	if(e==0)
		botao[1]=256;
	else
		sscanf(argv[e+1],"%d",&(botao[1]));

	if(d==0)
		botao[2]=1024;
	else
		sscanf(argv[d+1],"%d",&(botao[2]));

	if(f!=0)
		strcpy(ficheiro,argv[f+1]);
}

LUGARES *ProcuraCodigo(LUGARES *topo,char codigo[MAX])
{
	for(;topo!=NULL;topo=topo->prox)
		if(strcmp(topo->dado.codigo,codigo)==0)
			return topo;
	
	return (LUGARES *) NULL;
}

int NumeroConversoes(int realizado,int esperado,int linha)
{
	if(realizado>=esperado)
		return 0;

	printf("\n--- ERRO NO FICHEIRO DE CONFIGURACAO ---\n--- LINHA %d ---\n",linha);
	return 1;
}

LUGARES *AlocaLUGARES(LUGARES *topo,LUGARES *ap)
{
	ap=calloc(1,sizeof(LUGARES));
	if(ap==NULL)
	{
		printf("ERRO DE ALOCACAO\n");
		return ap;
	}

	if(topo!=NULL)
		topo->ant=ap;
	
	ap->ant=NULL;
	ap->prox=topo;

	return ap;
}

LUGARES *InsereLUGARES(LUGARES *topo, char info[MAX],char codigo[MAX],int linha)
{
	LUGARES *ap=NULL;
	int nconv,erro=0;

	ap=AlocaLUGARES(topo,ap);

	if(strcmp(codigo,"ES")==0||strcmp(codigo,"JG")==0||strcmp(codigo,"IN")==0||strcmp(codigo,"OUT")==0)
	{
		nconv=sscanf(info,"%s %d %d",ap->dado.codigo,&((*ap).dado.x),&((*ap).dado.y));	
		erro=NumeroConversoes(nconv,3,linha);
	}
	if(strcmp(codigo,"ZE")==0)
	{
		nconv=sscanf(info,"%s %d %d %s %s %s",ap->dado.codigo,&(ap->dado.x),&(ap->dado.y),ap->dado.nome, ap->dado.cor, ap->dado.matricula);
		erro=NumeroConversoes(nconv,5,linha);
	}
	if(strcmp(codigo,"ZR")==0)
	{	
		nconv=sscanf(info,"%s %d %d %d %d %s %s",ap->dado.codigo,&(ap->dado.x),&(ap->dado.y),&(ap->dado.largura), &(ap->dado.altura), ap->dado.nome,ap->dado.cor);
		erro=NumeroConversoes(nconv,6,linha);
	}
	if(strcmp(codigo,"ZER")==0)
	{
		nconv=sscanf(info,"%s %d %d %s",ap->dado.codigo,&(ap->dado.x),&(ap->dado.y), ap->dado.nome);	
		strcpy(ap->dado.cor,"amarelo");	
		erro=NumeroConversoes(nconv,4,linha);
	}
	if(erro==1)
		free(ap);
	else
		topo=ap;
	
	return topo;
}

LUGARES *LerFicheiro(char ficheiroABERTO[MAX],char ficheiro2[MAX],LUGARES *topo)
{
	FILE *fp;						
	int linha;			
	
	if(strcmp(ficheiro2,"")==0)			/* se nao for passado um ficheiro de configuracao lemos o config.txt */	
		strcpy(ficheiro2,ficheiroABERTO);

	fp=fopen(ficheiro2,"r");			
	if(fp==NULL)			/* se for nulo vamos abrir o config.txt, mesmo q ja o tenhamos tentado abrir */
	{
		strcpy(ficheiro2,ficheiroABERTO);

		fp=fopen(ficheiro2,"r");					
		if(fp==NULL)			/*se for nulo acaba o programa */			
		{
			printf("ERRO DE ABERTURA\n");
			return 0;
		}
	}
	strcpy(ficheiroABERTO,ficheiro2); /*o ficheiro aberto foi o ficheiro2, por isso passamo-lo para a string principal*/

	printf("ABERTURA DO FICHEIRO %s\n",ficheiroABERTO);

	for(linha=1;feof(fp)==0;linha++)				
	{
		char info[MAX]={'\0'};
		char codigo[MAX]={'\0'};

		fgets(info,MAX,fp);
		sscanf(info,"%s",codigo);	/* le a primeira string da linha,se for algo nao esperado avisa ERRO NO FICHEIRO */

		if(codigo[0]!='/' && codigo[0]!='\0')
		{
			if(strcmp(codigo,"ES")==0 || strcmp(codigo,"JG")==0 || strcmp(codigo,"IN")==0 || strcmp(codigo,"OUT")==0 || strcmp(codigo,"ZR")==0 || strcmp(codigo,"ZE")==0 || strcmp(codigo,"ZER")==0)
				topo=InsereLUGARES(topo,info,codigo,linha);
			else
				printf("\n--- ERRO NO FICHEIRO DE CONFIGURACAO ---\n--- LINHA %d ---\n",linha);
		}			
	}
	fclose(fp);

	return topo;
}

ROBOT *DadosROBOT(ROBOT *vetor,LUGARES *topo,int *dim)
{
	for(;topo!=NULL;topo=topo->prox)
		if(strcmp(topo->dado.codigo,"ZER")==0)
		{

			vetor=realloc(vetor,(*dim+1)*sizeof(ROBOT));
			if(vetor==NULL)
			{
				printf("ERRO DE REALOCACAO DE MEMORIA");
				return 0;
			}

			vetor[*dim].x=topo->dado.x;
			vetor[*dim].y=topo->dado.y;
			vetor[*dim].vel=3;
			vetor[*dim].vel2=3;
			vetor[*dim].mov=0;
			vetor[*dim].ocupado=0;
			vetor[*dim].choque=0;
			strcpy(vetor[*dim].cor,"verde");
			vetor[*dim].estacionamento=topo;
			vetor[*dim].cor_linha=(random()%25)+1;		/*cor da linha aleatoria */

			*dim=*dim+1;
		}
	return vetor;
}

LISTA *InsereLISTA(LISTA *lista,LUGARES *aux)
{
	LISTA *ap;

	ap=calloc(1,sizeof(LISTA));
	if(ap==NULL)
	{
		printf("ERRO NA ALOCACAO DE MEMORIA\n");
		return lista;
	}

	ap->carro.x=aux->dado.x;
	ap->carro.y=aux->dado.y;
	ap->carro.ocupado=1;		
	ap->carro.saida=0;
	ap->carro.entrada=0;
	ap->carro.estacionamento=aux;
	ap->carro.cor=rand()%26;			/*cor do carro aleatoria*/
	strcpy(ap->carro.matricula,aux->dado.matricula);

	ap->prox=lista;
	ap->ant=NULL;

	if(lista!=NULL)
		lista->ant=ap;

	return ap;
}

LISTA *DadosLISTA(LISTA *lista,LUGARES *topo)
{
	LUGARES *aux;

	for(aux=topo;aux!=NULL;aux=aux->prox)
	{
		aux=ProcuraCodigo(aux,"ZE");

		if(aux==NULL)
			break;
	
		lista=InsereLISTA(lista,aux); /*Insere carro na lista de carros*/
	}
	return lista;
}

void DescobreESJG(LUGARES **topo,int esjg[5]) 
{
	LUGARES *aux;

	aux=ProcuraCodigo(*topo,"ES");
	esjg[0]=aux->dado.x;
	esjg[1]=aux->dado.y;

	aux=ProcuraCodigo(*topo,"JG");
	esjg[2]=aux->dado.x*esjg[0];
	esjg[3]=aux->dado.y*esjg[1];

	if(esjg[0]<=esjg[1])
		esjg[4]=esjg[0];

	else
		esjg[4]=esjg[1];

}

void MultiplicaPorES(ROBOT *vetor, int dim, LISTA *lista,int esjg[5])
{
	LINHA *ap;
	int i;

	for(i=0;i<dim;i++)
	{
		vetor[i].x=(vetor[i].x+0.5)*esjg[0];
		vetor[i].y=(vetor[i].y+0.5)*esjg[1];

		ap=calloc(1,sizeof(LINHA));

		ap->ponto.x=vetor[i].x;
		ap->ponto.y=vetor[i].y;

		vetor[i].linha=ap;
	}

	for(;lista!=NULL;lista=lista->prox)
	{	
		lista->carro.x=(lista->carro.x+0.5)*esjg[0];
		lista->carro.y=(lista->carro.y+0.5)*esjg[1];
	}
}

int EstacionadosROBOT(ROBOT *vetor,int dim,int esjg[5])		/*verifica se todos os robots sao estacionados */
{
	int i;

	for(i=0;i<dim;i++)
		if(vetor[i].x!=((vetor[i].estacionamento)->dado.x+0.5)*esjg[0] || vetor[i].y != ((vetor[i].estacionamento)->dado.y+0.5)*esjg[1])
			return 1;

	return 0;
}

int Clique(int d,int dev, int mouse[3],int botao[3])	/*retorna 0 se nao clicar em nada, 1 no esquerdo e 2 no direito*/
{
	double x_mouse, y_mouse;
	unsigned int button_mouse;

	g2_query_pointer (dev, &x_mouse, &y_mouse, &button_mouse);	

	mouse[1] = (int) x_mouse;
	mouse[2] = (int) y_mouse;

	if(button_mouse!=botao[0] && button_mouse!=botao[1] && button_mouse!=botao[2])
		printf("Valor de Rato Desconhecido = %d \n",button_mouse);

	if(button_mouse == botao[1])	
		return 1;

	if(button_mouse == botao[2])	
		return 2;

	return 0;
}

int CliquePosicao(int clique, int mouse[3], int dir,int cima, int esq, int baixo)	/*retorna 1 se clicar no sitio, ou 0 se nao*/
{
	if(mouse[0]!=clique || esq>mouse[1] || mouse[1]>dir || baixo>mouse[2] || mouse[2]>cima)
		return 0;

	return 1;
}

LISTA *TrocaLISTA(LISTA *a,LISTA* b) /*troca ordem na lista*/
{
	if(a->ant!=NULL)
	(a->ant)->prox=b;

	if(b->prox!=NULL)
	(b->prox)->ant=a;

	a->prox=b->prox;
	b->ant=a->ant;
	b->prox=a;
	a->ant=b;

	return b;
}

LISTA *OrdenaLISTA(LISTA *topo) /*organiza a lista, tendo prioridade as matriculas e depois o nome dos estacionamentos */
{
	int b;
	LISTA *aux;

	for(b=0;b==0;)
	{
		b=1;	
		for(aux=topo;aux!=NULL && aux->prox!=NULL;aux=aux->prox)
		{	 
			if((strcmp(aux->carro.matricula,"")==0 && strcmp((aux->prox)->carro.matricula,"")!=0) || (strcmp(aux->carro.matricula,"")!=0 && strcmp((aux->prox)->carro.matricula,"")!=0 && strcmp(aux->carro.matricula,(aux->prox)->carro.matricula)>0) || (strcmp(aux->carro.matricula,"")==0 && strcmp((aux->prox)->carro.matricula,"")==0 && strcmp((aux->carro.estacionamento)->dado.nome,((aux->prox)->carro.estacionamento)->dado.nome)>0))
			{
				b=0;
				if(aux==topo)
					topo=TrocaLISTA(aux,aux->prox);
	
				else
					TrocaLISTA(aux,aux->prox);
			}
		}
	}
	return topo;
}

int NumeroCor(char cor[MAX])		/* Transforma a string na respetiva cor */
{
	if(strcmp(cor,"azul")==0)
		return 15;

	if(strcmp(cor,"amarelo")==0)
		return 25;

	if(strcmp(cor,"amarelo2")==0)
		return 26;

	if(strcmp(cor,"verde")==0)
		return 8;	

	if(strcmp(cor,"vermelho")==0)
		return 19;	

	if(strcmp(cor,"branco")==0)
		return 0;

	if(strcmp(cor,"laranja")==0)
		return 22;

	if(strcmp(cor,"cinzento")==0)
		return 14;

	printf("ERRO A LER A COR %s\nCOR PRETA POSTA EM REPOSICAO\n",cor);

	return 1;
}

void DesenhaFicheiro(int d,LUGARES **topo,int esjg[5])
{
	LUGARES *aux;

	g2_set_font_size(d, esjg[4]*1/2);

	for(aux=*topo;aux!=NULL;aux=aux->prox)
	{
		if(strcmp(aux->dado.codigo,"IN")==0 || strcmp(aux->dado.codigo,"OUT")==0)
		{
			g2_pen(d,1);
			g2_circle(d,(aux->dado.x+0.5)*esjg[0],(aux->dado.y+0.5)*esjg[1],esjg[4]/2);
			g2_string(d,aux->dado.x*esjg[0],aux->dado.y*esjg[1],aux->dado.codigo);					
		}
		if(strcmp(aux->dado.codigo,"ZR")==0)
		{
			g2_pen(d,NumeroCor(aux->dado.cor));						
			g2_filled_rectangle(d,(aux->dado.x+aux->dado.largura)*esjg[0],(aux->dado.y+aux->dado.altura)*esjg[1],aux->dado.x*esjg[0],aux->dado.y*esjg[1]);
			g2_pen(d,1);						
			g2_rectangle(d,(aux->dado.x+aux->dado.largura)*esjg[0],(aux->dado.y+aux->dado.altura)*esjg[1],aux->dado.x*esjg[0],aux->dado.y*esjg[1]);
		}		
		if(strcmp(aux->dado.codigo,"ZE")==0)
		{	
			g2_pen(d,NumeroCor(aux->dado.cor));					
			g2_filled_rectangle(d,aux->dado.x*esjg[0]+esjg[0],aux->dado.y*esjg[1]+esjg[1],aux->dado.x*esjg[0],aux->dado.y*esjg[1]);
			g2_pen(d,1);					
			g2_rectangle(d,aux->dado.x*esjg[0]+esjg[0],aux->dado.y*esjg[1]+esjg[1],aux->dado.x*esjg[0],aux->dado.y*esjg[1]);
			g2_string(d,aux->dado.x*esjg[0],aux->dado.y*esjg[1],aux->dado.nome );	
		}	
		if(strcmp(aux->dado.codigo,"ZER")==0)
		{
			g2_pen(d,NumeroCor(aux->dado.cor));						
			g2_filled_rectangle(d,aux->dado.x*esjg[0]+esjg[0],aux->dado.y*esjg[1]+esjg[1],aux->dado.x*esjg[0],aux->dado.y*esjg[1]);
			g2_pen(d,1);					
			g2_rectangle(d,aux->dado.x*esjg[0]+esjg[0],aux->dado.y*esjg[1]+esjg[1],aux->dado.x*esjg[0],aux->dado.y*esjg[1]);
			g2_string(d,aux->dado.x*esjg[0],aux->dado.y*esjg[1],aux->dado.nome );	
		}
	}
}

void DesenhaLinha(int d, ROBOT *vetor,int dim,int robotn)
{
	int i;
	LINHA *aux;
		
	for(i=0;i<dim;i++)
	{
		for(aux=vetor[i].linha;aux!=NULL && aux->prox!=NULL && (vetor[i].choque==0 || vetor[i].mov || i==robotn );aux=aux->prox)
		{
			g2_set_line_width(d,(rand()%3)+1);
			g2_pen(d,vetor[i].cor_linha);
			g2_line(d,aux->ponto.x,aux->ponto.y,(aux->prox)->ponto.x,(aux->prox)->ponto.y);
			g2_set_line_width(d,1);
		}
	}
}

void DesenhaRobot(int d,int esjg[5],ROBOT *vetor,int dimv)
{
	int i;
	
	for(i=0;i<dimv;i++)
	{
		g2_pen(d,NumeroCor(vetor[i].cor));
		g2_filled_circle(d,vetor[i].x,vetor[i].y,esjg[4]*2/5);
		g2_pen(d,1);
		g2_circle(d,vetor[i].x,vetor[i].y,esjg[4]*2/5);
	}
}

void DesenhaCarros(int d,LISTA *lista,int esjg[5])
{
	for(;lista!=NULL;lista=lista->prox)
	{
		if(strcmp(lista->carro.matricula,"")!=0)
		{
			g2_pen(d,lista->carro.cor);
			g2_filled_circle(d,lista->carro.x,lista->carro.y,esjg[4]*1/3);
			g2_pen(d,1);
			g2_circle(d,lista->carro.x,lista->carro.y,esjg[4]*1/3);
		}
	}
}

void DesenhaMenuParte1(int d,int esjg[5],ROBOT *vetor,int robotm)
{
	int resto=vetor[robotm].vel*10;
	char velocidade[MAX];

	g2_set_line_width(d,3);
	g2_pen(d,19);
	g2_filled_rectangle(d,esjg[2]+80,esjg[3],esjg[2],0);
	g2_filled_rectangle(d,esjg[2]+80,esjg[3]+80,0,esjg[3]);

	g2_pen(d,14);
	g2_filled_circle(d,esjg[2]+40,esjg[3]/2+80,20);
	g2_filled_circle(d,esjg[2]+40,esjg[3]/2-80,20);

	g2_pen(d,1);
	g2_rectangle(d,esjg[2]+80,esjg[3],esjg[2],0);
	g2_rectangle(d,esjg[2]+80,esjg[3]+80,0,esjg[3]);

	g2_circle(d,esjg[2]+40,esjg[3]/2+80,20);
	g2_circle(d,esjg[2]+40,esjg[3]/2-80,20);

	g2_triangle(d,esjg[2]+40,esjg[3]/2+100,esjg[2]+25,esjg[3]/2+68,esjg[2]+55,esjg[3]/2+68);
	g2_triangle(d,esjg[2]+40,esjg[3]/2-100,esjg[2]+25,esjg[3]/2-68,esjg[2]+55,esjg[3]/2-68);

	g2_line(d,(esjg[2]+80)/4,esjg[3],(esjg[2]+80)/4,esjg[3]+80);
	g2_line(d,2*(esjg[2]+80)/4,esjg[3],2*(esjg[2]+80)/4,esjg[3]+80);
	g2_line(d,3*(esjg[2]+80)/4,esjg[3],3*(esjg[2]+80)/4,esjg[3]+80);
	g2_set_line_width(d,1);

	g2_set_font_size(d, 40);
	g2_string(d,1*(esjg[2]+80)/8,esjg[3]+20,"IN" );
	g2_string(d,3*(esjg[2]+80)/8,esjg[3]+20,"OUT" );
	g2_string(d,5*(esjg[2]+80)/8,esjg[3]+20,"LIST" );
	g2_string(d,7*(esjg[2]+80)/8,esjg[3]+20,"END" );

	if(resto%10==0)
	{
		snprintf(velocidade,2,"%f",vetor[robotm].vel);
		g2_string(d,esjg[2]+30,esjg[3]/2-15,velocidade);
	}
	else
	{
		snprintf(velocidade,4,"%f",vetor[robotm].vel);
		g2_string(d,esjg[2]+15,esjg[3]/2-15,velocidade);
	}
}

void DesenhaMenuParte2(int d,int esjg[5],LUGARES *topo,LISTA *lista)
{
	LUGARES *aux;
	LISTA *ap;
	int lugares=0,carros=0;
	char vagas[MAX];

	for(aux=topo;aux!=NULL;aux=aux->prox)
	{
		aux=ProcuraCodigo(aux,"ZE");

		if(aux==NULL)
			break;

		if(strcmp(aux->dado.cor,"cinzento")!=0 || strcmp(aux->dado.matricula,"")!=0)
			lugares++;
	}
	for(ap=lista;ap!=NULL;ap=ap->prox)
		if(strcmp(ap->carro.matricula,"")!=0)
			carros++;
	
	if(lugares>carros)
	{
		snprintf(vagas,MAX,"%d",lugares-carros);
		g2_pen(d,8);
		g2_string(d,esjg[2]+20,30,vagas);
		g2_set_font_size(d,15);
		g2_string(d,esjg[2]+15,15,"LIVRES");
	}
	else
	{
		g2_pen(d,1);
		g2_string(d,esjg[2]+30,45,"0");
		g2_set_font_size(d,15);
		g2_string(d,esjg[2]+10,30,"PARQUE");
		g2_string(d,esjg[2]+10,15,"CHEIO");
	}
}

int MovimentoROBOT(ROBOT *vetor,int robotn)	/*move robot usando o cos=(x-xi)/sqrt(x-xi)²+(y-yi)² e sen*/
{
	LINHA *aux, *ap;

	for(ap=vetor[robotn].linha;ap!=NULL && ap->prox!=NULL;ap=ap->prox);
	for(;sqrt(((ap)->ponto.x-vetor[robotn].x)*((ap)->ponto.x- vetor[robotn].x)+((ap)->ponto.y-vetor[robotn].y)*((ap)->ponto.y-vetor[robotn].y))<vetor[robotn].vel+.001 && (ap)->ant!=NULL; ap=(ap)->ant)
	{
		(ap->ant)->prox=NULL;
		aux=ap;
		free(aux);
		aux=NULL;		
	}	
	if(ap==vetor[robotn].linha && sqrt(((ap)->ponto.x-vetor[robotn].x)*((ap)->ponto.x- vetor[robotn].x)+((ap)->ponto.y-vetor[robotn].y)*((ap)->ponto.y-vetor[robotn].y))<vetor[robotn].vel+.001)
	{
		vetor[robotn].x=(ap)->ponto.x;
		vetor[robotn].y=(ap)->ponto.y;
		vetor[robotn].choque=0;
		vetor[robotn].vel=vetor[robotn].vel2;
		return 0;
	}
	if(sqrt(((ap)->ponto.x-vetor[robotn].x)*((ap)->ponto.x- vetor[robotn].x)+((ap)->ponto.y-vetor[robotn].y)*((ap)->ponto.y-vetor[robotn].y))<vetor[robotn].vel+.001|| (vetor[robotn].linha)!=NULL)	
	{
		vetor[robotn].x=vetor[robotn].x+vetor[robotn].vel*((ap->ponto.x-vetor[robotn].x)/sqrt((ap->ponto.x-vetor[robotn].x)*(ap->ponto.x-vetor[robotn].x)+(ap->ponto.y-vetor[robotn].y)*(ap->ponto.y-vetor[robotn].y)));

		vetor[robotn].y=vetor[robotn].y+vetor[robotn].vel*((ap->ponto.y-vetor[robotn].y)/sqrt((ap->ponto.y-vetor[robotn].y)*(ap->ponto.y-vetor[robotn].y)+(ap->ponto.x-vetor[robotn].x)*(ap->ponto.x-vetor[robotn].x)));		
	}
	return 1;	
}

void ApagarFimLINHA(ROBOT vetor) /*apaga linha toda se nao tiver chocado, deixa o destino de se tiver*/
{
	LINHA *aux, *ap;

	if(vetor.choque==0)
	{
		for(aux=vetor.linha;aux!=NULL && aux->prox!=NULL;aux=aux->prox);

		while(aux!=vetor.linha)
		{
			ap=aux;
			aux=aux->ant;
			aux->prox=NULL;
			free(ap);
			ap=NULL;
		}
		(vetor.linha)->ponto.x=vetor.x;
		(vetor.linha)->ponto.y=vetor.y;
	}
	else
	{
		for(aux=vetor.linha;aux!=NULL && aux->prox!=NULL;aux=aux->prox);
	
		while(aux!=vetor.linha)
		{
			ap=aux;
			aux=aux->ant;
			aux->prox=NULL;
			free(ap);
			ap=NULL;
		}
		ap=calloc(1,sizeof(LINHA));
		aux->prox=ap;
		ap->ponto.x=vetor.x;
		ap->ponto.y=vetor.y;
	}
}

LINHA *ProximosMovimentos(ROBOT vetor,int movimentos) /*descobre os proximosmovimentos*/
{
	LINHA *ap;

	for(ap=vetor.linha;ap!=NULL && ap->prox!=NULL;ap=ap->prox);
	for(;sqrt((ap->ponto.x-vetor.x)*(ap->ponto.x- vetor.x)+(ap->ponto.y-vetor.y)*(ap->ponto.y-vetor.y))<vetor.vel*movimentos+.001 && ap->ant!=NULL; ap=ap->ant);

	return ap;
}

int RestricoesJanela(ROBOT *vetor,int i,int esjg[5])	/*para se sair na janela*/
{
	LINHA *ap;

	ap=ProximosMovimentos(vetor[i],1);

	if(vetor[i].x!=((vetor[i].estacionamento)->dado.x+0.5)*esjg[0] || vetor[i].y!=((vetor[i].estacionamento)->dado.y+0.5)*esjg[1])
	{
		if(esjg[2]<=vetor[i].x+vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))+esjg[4]*2/5 && ((vetor[i].estacionamento)->dado.x!=vetor[i].x || ((vetor[i].estacionamento)->dado.y!=vetor[i].y)))
		{
			vetor[i].x=vetor[i].x-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
		if(0>=vetor[i].x+vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))-esjg[4]*2/5 && ((vetor[i].estacionamento)->dado.x!=vetor[i].x || ((vetor[i].estacionamento)->dado.y!=vetor[i].y)))
		{
			vetor[i].x=vetor[i].x-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
		if(esjg[3]<=vetor[i].y+vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))+esjg[4]*2/5 && ((vetor[i].estacionamento)->dado.x!=vetor[i].x || ((vetor[i].estacionamento)->dado.y!=vetor[i].y)))
		{
			vetor[i].y=vetor[i].y-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
		if(0>=vetor[i].y+vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))-esjg[4]*2/5 && ((vetor[i].estacionamento)->dado.x!=vetor[i].x || ((vetor[i].estacionamento)->dado.y!=vetor[i].y)))
		{
			vetor[i].y=vetor[i].y-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
	}
	return 0;
}

int RestricoesZR(LUGARES **topo,ROBOT *vetor,int i,int esjg[5])		/*para se bater numa ZR*/
{
	LUGARES *aux;
	LINHA *ap;

	ap=ProximosMovimentos(vetor[i],1);

	for(aux=*topo;aux!=NULL;aux=aux->prox)
	{
		aux=ProcuraCodigo(aux,"ZR");
	
		if(aux==NULL)
			break;

		if((aux->dado.x+aux->dado.largura)*esjg[0]-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))+esjg[4]*2/5 >= vetor[i].x && vetor[i].x >=(aux->dado.x+aux->dado.largura)*esjg[0] && (aux->dado.y+aux->dado.altura)*esjg[1]>=vetor[i].y-esjg[4]*2/5 && vetor[i].y+esjg[4]*2/5>=aux->dado.y*esjg[1])	
		{
			vetor[i].x=vetor[i].x-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
		if((aux->dado.x)*esjg[0]-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))-esjg[4]*2/5<=vetor[i].x && vetor[i].x<=(aux->dado.x)*esjg[0] && (aux->dado.y+aux->dado.altura)*esjg[1]>=vetor[i].y-esjg[4]*2/5 && vetor[i].y+esjg[4]*2/5>=aux->dado.y*esjg[1])	
		{
			vetor[i].x=vetor[i].x-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
		if((aux->dado.y+aux->dado.altura)*esjg[1]-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))+esjg[4]*2/5 >= vetor[i].y && vetor[i].y >=(aux->dado.y+aux->dado.altura)*esjg[1] && (aux->dado.x+aux->dado.largura)*esjg[0]>=vetor[i].x-esjg[4]*2/5 && vetor[i].x+esjg[4]*2/5>=aux->dado.x*esjg[0])	
		{
			vetor[i].y=vetor[i].y-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
		if((aux->dado.y)*esjg[1]-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))-esjg[4]*2/5<=vetor[i].y && vetor[i].y<=(aux->dado.y)*esjg[1] && (aux->dado.x+aux->dado.largura)*esjg[0]>=vetor[i].x-esjg[4]*2/5 && vetor[i].x+esjg[4]*2/5>=aux->dado.x*esjg[0])	
		{
			vetor[i].y=vetor[i].y-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
			return 1;
		}
	}
	return 0;
}

int RestricoesZE(LUGARES **topo,ROBOT *vetor,int i,int esjg[5])
{
	LUGARES *aux;
	LINHA *ap;

	ap=ProximosMovimentos(vetor[i],1);

	for(aux=*topo;aux!=NULL;aux=aux->prox)
	{
		aux=ProcuraCodigo(aux,"ZE");
	
		if(aux==NULL)
			break;

		if(strcmp(aux->dado.cor,"vermelho")==0 || strcmp(aux->dado.cor,"cinzento")==0)
		{
			if((aux->dado.x+1)*esjg[0]-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))+esjg[4]*2/5>=vetor[i].x && vetor[i].x>=(aux->dado.x+1)*esjg[0] && (aux->dado.y+1)*esjg[1]>=vetor[i].y-esjg[4]*2/5 && vetor[i].y+esjg[4]*2/5>=aux->dado.y*esjg[1])	
			{
				vetor[i].x=vetor[i].x-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
				return 1;
			}
			if((aux->dado.x)*esjg[0]-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))-esjg[4]*2/5<=vetor[i].x && vetor[i].x<=(aux->dado.x)*esjg[0] && (aux->dado.y+1)*esjg[1]>=vetor[i].y-esjg[4]*2/5 && vetor[i].y+esjg[4]*2/5>=aux->dado.y*esjg[1])	
			{
				vetor[i].x=vetor[i].x-vetor[i].vel*((ap->ponto.x-vetor[i].x)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
				return 1;
			}
			if((aux->dado.y+1)*esjg[1]-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))+esjg[4]*2/5>=vetor[i].y && vetor[i].y>=(aux->dado.y+1)*esjg[1] && (aux->dado.x+1)*esjg[0]>=vetor[i].x-esjg[4]*2/5 && vetor[i].x+esjg[4]*2/5>=aux->dado.x*esjg[0])	
			{
				vetor[i].y=vetor[i].y-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
				return 1;
			}
			if((aux->dado.y)*esjg[1]-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)))-esjg[4]*2/5<=vetor[i].y && vetor[i].y<=(aux->dado.y)*esjg[1] && (aux->dado.x+1)*esjg[0]>=vetor[i].x-esjg[4]*2/5 && vetor[i].x+esjg[4]*2/5>=aux->dado.x*esjg[0])	
			{
				vetor[i].y=vetor[i].y-vetor[i].vel*((ap->ponto.y-vetor[i].y)/sqrt((ap->ponto.x-vetor[i].x)*(ap->ponto.x-vetor[i].x)+(ap->ponto.y-vetor[i].y)*(ap->ponto.y-vetor[i].y)));
				return 1;
			}
		}
	}
	return 0;
}

void MovimentoRestricoes(LUGARES **topo,ROBOT *vetor,int dim,int esjg[5],int robotn)
{
	int i,restricao=0;

	for(i=0;i<dim;i=i+1)
	{
		if(vetor[i].mov)	
			vetor[i].mov=MovimentoROBOT(vetor,i);

		if((vetor[i].estacionamento)->dado.x*esjg[0]<=vetor[i].x && vetor[i].x<=((vetor[i].estacionamento)->dado.x+1)*esjg[0] && (vetor[i].estacionamento)->dado.y*esjg[1]<=vetor[i].y && vetor[i].y<=((vetor[i].estacionamento)->dado.y+1)*esjg[1]  && vetor[i].mov==0 && vetor[i].ocupado==0 && vetor[i].choque==0)		/*se o robot tiver parado no seu estacionamento, estaciona-o*/
		{
			vetor[i].x=((vetor[i].estacionamento)->dado.x+0.5)*esjg[0];
			vetor[i].y=((vetor[i].estacionamento)->dado.y+0.5)*esjg[1];
		}
		if(vetor[i].mov)
		{
			if(RestricoesJanela(vetor,i,esjg))
				restricao=1;

			if(RestricoesZR(topo,vetor,i,esjg))
				restricao=1;			

			if(RestricoesZE(topo,vetor,i,esjg))
				restricao=1;

			if(restricao)
			{
				vetor[i].mov=0;
				vetor[i].choque=0;
				vetor[i].vel=vetor[i].vel2;
				ApagarFimLINHA(vetor[i]);
			}
		}
	}
}

LINHA *InsereLINHA(LINHA *topo,int x ,int y,int para)
{
	LINHA *aux;

	aux=(LINHA *)calloc(1,sizeof(LINHA));

	if(para==0)
	{
		if(topo!=NULL)
			topo->ant=aux;
	
		aux->ant=NULL;
		aux->prox=topo;
		aux->ponto.x=x;
		aux->ponto.y=y;

		topo=aux;

		return topo;
	}
	else
	{
		if(topo->prox!=NULL)
			(topo->prox)->ant=aux;	
		
		aux->ponto.x=x;
		aux->ponto.y=y;


		aux->prox=topo->prox;
		aux->ant=topo;
		topo->prox=aux;

		return topo;
	}
}

int FazLINHA(LUGARES **topo,int mouse[3],int esjg[5],ROBOT *vetor,int dim,int robotn)
{
	int i;

	if(CliquePosicao(2,mouse,esjg[2],esjg[3],0,0))
	{
		for(i=0;i<dim;i++)
		{
			if(((vetor[i].x-mouse[1])*(vetor[i].x-mouse[1])+(vetor[i].y-mouse[2])*(vetor[i].y-mouse[2])<=(esjg[4]*2/5)*(esjg[4]*2/5)) && robotn==-1 && vetor[i].mov==0)	/*seleciona o robotn em q se vai fazer a linha,muda ligeiramente a cor do seu estacionamento para ajudar*/
			{
				robotn=i;	
				strcpy((vetor[i].estacionamento)->dado.cor,"amarelo2");
			}
		}
		if(robotn!=-1 && ((vetor[robotn].x-mouse[1])*(vetor[robotn].x-mouse[1])+(vetor[robotn].y-mouse[2])*(vetor[robotn].y-mouse[2])>=(esjg[4]*2/5)*(esjg[4]*2/5)))		/*insere linha*/
			vetor[robotn].linha=InsereLINHA(vetor[robotn].linha,mouse[1],mouse[2],vetor[robotn].choque);			

	}
	else if(robotn!=-1)		/*mete o robotn a andar e altera o seu valor para -1*/
	{
		strcpy((vetor[robotn].estacionamento)->dado.cor,"amarelo");
		vetor[robotn].mov=1;
		robotn=-1;
	}	
	return robotn;
}

void RobotMOVECarro(LISTA *lista,ROBOT *vetor,int dimv,int esjg[5])	/*se o robot e o carro estiverem livres associa-os, e move o carro em cima do robot*/
{
	LISTA *aux;
	int i;

	for(i=0;i<dimv;i++)
	{
		for(aux=lista; aux!=NULL ;aux=aux->prox)
		{
			if(((aux)->carro.entrada==1 || (aux)->carro.saida==1) && ((vetor[i].x-aux->carro.x)*(vetor[i].x-aux->carro.x)+(vetor[i].y-aux->carro.y)*(vetor[i].y-aux->carro.y)<=(esjg[4]*2/5)*(esjg[4]*2/5)) && vetor[i].ocupado==0 && aux->carro.ocupado==0)
			{
				vetor[i].ocupado=1;
				vetor[i].carro=aux;
				aux->carro.x=vetor[i].x;
				aux->carro.y=vetor[i].y;
				aux->carro.ocupado=1;
			}
			if(vetor[i].ocupado)
			{
				(vetor[i].carro)->carro.x=vetor[i].x;
				(vetor[i].carro)->carro.y=vetor[i].y;
			}
		}
	}
}

LISTA *EstacionaCarro(LISTA *lista,ROBOT *vetor,int i,LUGARES *ap,int esjg[5])		/*estaciona o carro*/
{
	LISTA *aux;

	(vetor[i].carro)->carro.estacionamento=NULL;
	vetor[i].ocupado=0;

	strcpy(ap->dado.matricula,(vetor[i].carro)->carro.matricula);

	for(aux=lista;aux!=NULL;aux=aux->prox)
		if(aux->carro.estacionamento!=NULL)
			if(strcmp(((aux->carro.estacionamento)->dado.nome),(ap->dado.nome))==0)
			{
				strcpy(aux->carro.matricula,(vetor[i].carro)->carro.matricula);
				aux->carro.cor=(vetor[i].carro)->carro.cor;
				aux->carro.x=(ap->dado.x+0.5)*esjg[0];
				aux->carro.y=(ap->dado.y+0.5)*esjg[1];
				aux->carro.entrada=0;
			}

	(vetor[i].carro)->carro.estacionamento=ap;

	if((vetor[i].carro)->ant!=NULL)
		((vetor[i].carro)->ant)->prox=(vetor[i].carro)->prox;
	else
		lista=(vetor[i].carro)->prox;

	if((vetor[i].carro)->prox!=NULL)
		((vetor[i].carro)->prox)->ant=(vetor[i].carro)->ant;

	free(vetor[i].carro);

	return lista;
}

int RestricoesEstacionar(LUGARES *aux,ROBOT *vetor,int i,int esjg[5])
{
	if((vetor[i].ocupado && (vetor[i].carro)->carro.entrada && vetor[i].mov && strcmp(aux->dado.cor,"branco")==0 && strcmp(aux->dado.matricula,"")==0 && aux->dado.x*esjg[0]<=(vetor[i].linha)->ponto.x && (aux->dado.x+1)*esjg[0]>=(vetor[i].linha)->ponto.x && aux->dado.y*esjg[1]<=(vetor[i].linha)->ponto.y && (aux->dado.y+1)*esjg[1]>=(vetor[i].linha)->ponto.y))
	{
		if((aux->dado.x+1)*esjg[0]+vetor[i].vel+esjg[4]*2/5>=vetor[i].x && vetor[i].x>=(aux->dado.x+1)*esjg[0] && (aux->dado.y+1)*esjg[1]>=vetor[i].y-esjg[4]*2/5 && vetor[i].y+esjg[4]*2/5>=aux->dado.y*esjg[1])	
		{
			vetor[i].x=vetor[i].x+vetor[i].vel/2;
			return 1;
		}
		if((aux->dado.x)*esjg[0]-vetor[i].vel-esjg[4]*2/5<=vetor[i].x && vetor[i].x<=(aux->dado.x)*esjg[0] && (aux->dado.y+1)*esjg[1]>=vetor[i].y-esjg[4]*2/5 && vetor[i].y+esjg[4]*2/5>=aux->dado.y*esjg[1])	
		{
			vetor[i].x=vetor[i].x-vetor[i].vel/2;
			return 1;
		}
		if((aux->dado.y+1)*esjg[1]+vetor[i].vel+esjg[4]*2/5>=vetor[i].y && vetor[i].y>=(aux->dado.y+1)*esjg[1] && (aux->dado.x+1)*esjg[0]>=vetor[i].x-esjg[4]*2/5 && vetor[i].x+esjg[4]*2/5>=aux->dado.x*esjg[0])	
		{
			vetor[i].y=vetor[i].y+vetor[i].vel/2;
			return 1;
		}
		if((aux->dado.y)*esjg[1]-vetor[i].vel-esjg[4]*2/5<=vetor[i].y && vetor[i].y<=(aux->dado.y)*esjg[1] && (aux->dado.x+1)*esjg[0]>=vetor[i].x-esjg[4]*2/5 && vetor[i].x+esjg[4]*2/5>=aux->dado.x*esjg[0])	
		{
			vetor[i].y=vetor[i].y-vetor[i].vel/2;
			return 1;
		}
	}
	return 0;
}

LISTA *RobotINCarro(LUGARES *topo,LISTA *lista,ROBOT *vetor,int dimv,int esjg[5])	/*ve se pode estacionar o carro*/
{
	LUGARES *ap;
	int i;

	for(i=0;i<dimv;i++)
	{
		for(ap=topo;ap!=NULL;ap=ap->prox)	
		{
			ap=ProcuraCodigo(ap,"ZE");

			if(ap==NULL)
				break;

			if(RestricoesEstacionar(ap,vetor,i,esjg))
			{
				vetor[i].mov=0;
				vetor[i].choque=0;
				vetor[i].vel=vetor[i].vel2;
				ApagarFimLINHA(vetor[i]);
				lista=EstacionaCarro(lista,vetor,i,ap,esjg);
			}
		}

	}
	return lista;
}

LISTA *RobotOUTCarro(LUGARES *topo,LISTA *lista,ROBOT *vetor,int dimv,int esjg[5])	/*tira o carro*/
{
	int i;

	topo=ProcuraCodigo(topo,"OUT");

	for(i=0;i<dimv;i++)
	{
		if(vetor[i].mov==0 && vetor[i].ocupado && (vetor[i].carro)->carro.saida && (vetor[i].carro)->carro.entrada==0 && ((vetor[i].x-(topo->dado.x+0.5)*esjg[0])*(vetor[i].x-(topo->dado.x+0.5)*esjg[0])+(vetor[i].y-(topo->dado.y+0.5)*esjg[1])*(vetor[i].y-(topo->dado.y+0.5)*esjg[1])<=(esjg[4]/2)*(esjg[4]/2)))
		{
			vetor[i].ocupado=0;

			if((vetor[i].carro)->ant!=NULL)
				((vetor[i].carro)->ant)->prox=(vetor[i].carro)->prox;

			else
				lista=(vetor[i].carro)->prox;

			if((vetor[i].carro)->prox!=NULL)
				((vetor[i].carro)->prox)->ant=(vetor[i].carro)->ant;

			free(vetor[i].carro);

			vetor[i].carro=NULL;
		}
	}
	return lista;
}

int SelecionarROBOT(ROBOT *vetor,int dim,int mouse[3],int esjg[5],int robotm,int robotn)	/*seleciona um robot, ou por clicar nele com esquerdo ou direito ou por clicar no seu estacionamento */
{
	int i;

	for(i=0;i<dim;i++)
	{
		if(robotn==-1)
		{
			if(mouse[0]!=0 && (mouse[1]-vetor[i].x)*(mouse[1]-vetor[i].x)+(mouse[2]-vetor[i].y)*(mouse[2]-vetor[i].y)<=(esjg[4]*2/5)*(esjg[4]*2/5))
				return i;

			if(CliquePosicao(1,mouse,((vetor[i].estacionamento)->dado.x+1)*esjg[0],((vetor[i].estacionamento)->dado.y+1)*esjg[1],((vetor[i].estacionamento)->dado.x)*esjg[0],((vetor[i].estacionamento)->dado.y)*esjg[1]))
				return i;
		}
		else
			return robotn;
	}
	return robotm;
}


int MudaVelocidadeManual(ROBOT *vetor,int robotm,int mouse[3],int esjg[5],int a)	/*altera a velocidade do robot selecionado em 0.5*/
{
	if(mouse[0]==1 && (mouse[1]-esjg[2]-40)*(mouse[1]-esjg[2]-40)+(mouse[2]-esjg[3]/2-80)*(mouse[2]-esjg[3]/2-80)<=20*20 && a==0)
	{
		if(vetor[robotm].vel>=7.5)
		{
			printf("ALERTA VELOCIDADE MAXIMA\n");
			return 1;
		}
		vetor[robotm].vel=vetor[robotm].vel+0.5;
		vetor[robotm].vel2=vetor[robotm].vel;
		return 1;
	}
	if(mouse[0]==1 && (mouse[1]-esjg[2]-40)*(mouse[1]-esjg[2]-40)+(mouse[2]-esjg[3]/2+80)*(mouse[2]-esjg[3]/2+80)<=20*20 && a==0 )
	{
		if(vetor[robotm].vel<=0.5)
		{
			printf("ALERTA VELOCIDADE MINIMA\n");
			return 1;
		}
		vetor[robotm].vel=vetor[robotm].vel-0.5;
		vetor[robotm].vel2=vetor[robotm].vel;
		return 1;
	}
	if((mouse[0]!=1 || (mouse[1]-esjg[2]-40)*(mouse[1]-esjg[2]-40)+(mouse[2]-esjg[3]/2-80)*(mouse[2]-esjg[3]/2-80)>=20*20) && (mouse[0]!=1 || (mouse[1]-esjg[2]-40)*(mouse[1]-esjg[2]-40)+(mouse[2]-esjg[3]/2+80)*(mouse[2]-esjg[3]/2+80)>=20*20))
		return 0;

	return a;
}

void ControlaVelocidade(ROBOT *vetor,int dim)		/*controla a velocidade para ter um valor divisil por 0.5*/
{
	int velocidade;
	int i,n;

	for(i=0;i<dim;i++)
	{
		velocidade=vetor[i].vel*10;
	
		for(n=0;velocidade%5!=0;n++)
			velocidade=velocidade+1;

		vetor[i].vel=velocidade;
		vetor[i].vel=vetor[i].vel/10;

		if(vetor[i].vel<1)
			vetor[i].vel=0.5;

		if(vetor[i].vel>7)
			vetor[i].vel=7.5;
	}
}

int ChoqueROBOT(ROBOT *vetor,int dim,int esjg[5])		/*quando em n movimentos 2 robots vao chocar eles param e piscam*/
{
	LINHA *api,*apn;
	int i,n,movimentos=2,distancia=esjg[4]*4/5;
	
	for(i=0;i<dim;i++)		
		for(n=0;n<dim;n++)
		{
			if(n==i)
				n=n+1;
			if(n==dim)
				return 1;

			api=ProximosMovimentos(vetor[i],movimentos);
			apn=ProximosMovimentos(vetor[n],movimentos);

			if((api->ponto.x-apn->ponto.x)*(api->ponto.x-apn->ponto.x)+(api->ponto.y-apn->ponto.y)*(api->ponto.y-apn->ponto.y)<=distancia*distancia)
			{
				if(vetor[i].mov)
				{
					vetor[i].choque=1;
					ApagarFimLINHA(vetor[i]);
				}
				if(vetor[n].mov)
				{
					vetor[n].choque=1;
					ApagarFimLINHA(vetor[n]);
				}
				vetor[i].vel=vetor[i].vel2;
				vetor[n].vel=vetor[n].vel2;
				vetor[i].mov=0;
				vetor[n].mov=0;

				return 0;
			}
		}
	return 1;
}

void MudaVelocidadeAutomatico(ROBOT *vetor,int dim,int esjg[5])		/*quando em n movimentos 2 robots vao chocar eles ajustam a velocidade*/
{
	LINHA *api,*apn;
	int i,n,movimentos=30,distancia=esjg[4]*4/5;
	
	for(i=0;i<dim;i++)
		if(vetor[i].mov==1)
			for(n=0;n<dim;n++)
			{
				if(n==i)
					n=n+1;
				if(n==dim)
					return;

				if(vetor[n].mov==1)
				{
					api=ProximosMovimentos(vetor[i],movimentos);
					apn=ProximosMovimentos(vetor[n],movimentos);

					if((api->ponto.x-apn->ponto.x)*(api->ponto.x-apn->ponto.x)+(api->ponto.y-apn->ponto.y)*(api->ponto.y-apn->ponto.y)<=distancia*distancia)
					{
						if(vetor[i].vel>0.5)
							vetor[i].vel=vetor[i].vel/2;
						else if(vetor[n].vel>0.5)
							vetor[n].vel=vetor[n].vel/2;
						else
							vetor[i].vel=vetor[i].vel*5;
					}
				}
			}
}

LISTA *RetiraCarro(LISTA *lista,LISTA *aux,int esjg[5])		/*"solta" carro*/
{
	LISTA *ap;

	ap=calloc(1,sizeof(LISTA));
	if(ap==NULL)
	{
		printf("ERRO NA ALOCACAO DE MEMORIA");
		return lista;
	}		
	strcpy((aux->carro.estacionamento)->dado.cor,"branco");			

	(ap->carro.estacionamento)=aux->carro.estacionamento;

	ap->carro.x=(ap->carro.estacionamento)->dado.x*esjg[0]+0.5*esjg[0];
	ap->carro.y=(ap->carro.estacionamento)->dado.y*esjg[1]+0.5*esjg[1];
	ap->carro.cor=rand()%26;	

	strcpy(ap->carro.matricula,"");
	
	(aux->carro.estacionamento)=NULL;

	ap->prox=lista;
	ap->ant=NULL;

	if(lista!=NULL)
		lista->ant=ap;

	return ap;
}

LISTA *CorZE(LUGARES *topo,LISTA *lista,int esjg[5],int pisca)		/*altera a cor dos ZE e retira carros dos estacionamentos*/
{
	LISTA *aux;

	if(pisca==0)
		for(;topo!=NULL;topo=topo->prox)
		{
			topo=ProcuraCodigo(topo,"ZE");
			if(topo==NULL)
				break;
			if(strcmp(topo->dado.cor,"cinzento")!=0)
			{
				if(strcmp(topo->dado.matricula,"")==0)
					strcpy(topo->dado.cor,"branco");

				else
					strcpy(topo->dado.cor,"vermelho");
			}
		}		
	else
		for(aux=lista;aux!=NULL;aux=aux->prox)
			if((aux->carro.estacionamento)!=NULL)
			{
				if(strcmp(aux->carro.matricula,"")!=0 && strcmp((aux->carro.estacionamento)->dado.matricula,"")!=0)
					strcpy((aux->carro.estacionamento)->dado.cor,"vermelho");

				else if(strcmp(aux->carro.matricula,"")!=0 && strcmp((aux->carro.estacionamento)->dado.matricula,"")==0 && strcmp((aux->carro.estacionamento)->dado.cor,"laranja")==0 && (((aux->carro.estacionamento)->dado.x+0.5)*esjg[0]!=aux->carro.x || ((aux->carro.estacionamento)->dado.y+0.5)*esjg[1]!=aux->carro.y))	
					lista=RetiraCarro(lista,aux,esjg);		
			}
	return lista;
}

void PiscaROBOT(ROBOT *vetor,int robot,int pisca)	/*altera a cor dos robots de modo a piscar*/
{
	if(pisca%80==0 && vetor[robot].choque==0)
		strcpy(vetor[robot].cor,"branco");

	else if(pisca%80==0 && vetor[robot].choque)
		strcpy(vetor[robot].cor,"amarelo");

	else
		strcpy(vetor[robot].cor,"verde");
}

void CorROBOT(ROBOT *vetor,int dim,int esjg[5],int pisca,int robotn)		/*altera a cor dos robots*/
{
	int i;

	for(i=0;i<dim;i++)	
	{
		if(pisca%40==0 && vetor[i].mov==0 && (((vetor[i].estacionamento)->dado.x+0.5)*esjg[0]!=vetor[i].x || ((vetor[i].estacionamento)->dado.y+0.5)*esjg[1]!=vetor[i].y) && robotn!=i)
			PiscaROBOT(vetor,i,pisca);

		if(vetor[i].mov || robotn==i || (((vetor[i].estacionamento)->dado.x+0.5)*esjg[0]==vetor[i].x && ((vetor[i].estacionamento)->dado.y+0.5)*esjg[1]==vetor[i].y))
			strcpy(vetor[i].cor,"verde");

		if((vetor[i].carro)!=NULL)
		{
			if((vetor[i].carro)->carro.entrada==1)
				strcpy(vetor[i].cor,"vermelho");

			if((vetor[i].carro)->carro.saida==1)
				strcpy(vetor[i].cor,"azul");
		}
	}
}

void MostrarLIST(LISTA *lista)
{
	printf("\n----  ESTADO  | ESTACIONAMENTO | MATRICULA ----\n");

	for(;lista!=NULL;lista=lista->prox)
	{
		if((lista->carro.estacionamento)!=NULL && strcmp(lista->carro.matricula,"")!=0 && (lista->carro.saida==0))
			printf("  ESTACIONADO | %14s | %s\n",(lista->carro.estacionamento)->dado.nome,lista->carro.matricula);

		else if((lista->carro.estacionamento)!=NULL && strcmp(lista->carro.matricula,"")==0)
			printf("     VAZIO    | %14s | %s\n",(lista->carro.estacionamento)->dado.nome,lista->carro.matricula);
		
		else if(lista->carro.entrada==1)
			printf(" A ESTACIONAR |      A DEFINIR | %s\n",lista->carro.matricula);

		else
			printf("   A RETIRAR  |            OUT | %s\n",lista->carro.matricula);
	}
	printf("\n\n\n");
}

int LIST(LISTA *lista,int mouse[3], int esjg[5],int a)		/*se clicar no esquerdo mostra lista uma vez*/
{
	if(CliquePosicao(1,mouse,3*(esjg[2]+80)/4,esjg[3]+80,2*(esjg[2]+80)/4,esjg[3]) && a==0)
	{
		MostrarLIST(lista);
		return 1;
	}
	if(CliquePosicao(1,mouse,3*(esjg[2]+80)/4,esjg[3]+80,2*(esjg[2]+80)/4,esjg[3])==0 && a==1)
		 return 0;

	return a;
}

int VerificarMatricula(char matricula[MAX],LISTA *lista)	/*veridica se a matricula ja se encontra no parque*/
{
	for(;lista!=NULL&&lista->prox!=NULL;lista=lista->prox)
		if(strcmp(lista->carro.matricula,matricula)==0)
			return 0;

	return 1;
}

int VerificarIN(LISTA *lista,LUGARES **topo,int esjg[5])	/*verifica se algum carro esta ja a entrar*/
{
	LUGARES *aux;

	aux=ProcuraCodigo(*topo,"IN");

	for(;lista!=NULL;lista=lista->prox)
		if((aux->dado.x+0.5)*esjg[0]==lista->carro.x && (aux->dado.y+0.5)*esjg[1]==lista->carro.y)
			return 1;
		
	return 0;
}

int VerificarEstacionamento(LISTA *lista,LUGARES **topo)	/*verifica se o estacionamento esta completo*/
{
	LUGARES *aux;
	LISTA *ap;
	int lugares=0,carros=0;

	for(aux=*topo;aux!=NULL;aux=aux->prox)
	{
		aux=ProcuraCodigo(aux,"ZE");

		if(aux==NULL)
			break;

		if(strcmp(aux->dado.cor,"cinzento")!=0 || strcmp(aux->dado.matricula,"")!=0)
			lugares++;
	}
	for(ap=lista;ap!=NULL;ap=ap->prox)
		if(strcmp(ap->carro.matricula,"")!=0)
			carros++;

	if(lugares<=carros)
		return 1;

	return 0;
}

LISTA * IN(LISTA *lista,LUGARES **topo,int esjg[5],int mouse[3])	/*pede matricula do carro*/
{
	LUGARES *aux;
	LISTA *ap;

	if(CliquePosicao(1,mouse,1*(esjg[2]+80)/4,esjg[3]+80,0,esjg[3]))
	{
		if(VerificarIN(lista,topo,esjg))
		{
			printf("UM CARRO EM ESPERA, VOLTE A POR O CARRO DAQUI A POUCO\n");
			return lista;
		}
		if(VerificarEstacionamento(lista,topo))
		{
			printf("ESTACIONAMENTO CHEIO, VOLTE A POR O CARRO DAQUI A POUCO\n");
			return lista;
		}
		ap=calloc(1,sizeof(LISTA));

		printf("IN->MATRICULA=");
		scanf("%s",ap->carro.matricula);
		
		if(VerificarMatricula(ap->carro.matricula,lista))
		{
			aux=ProcuraCodigo(*topo,"IN");
			ap->carro.x=aux->dado.x*esjg[0]+0.5*esjg[0];
			ap->carro.y=aux->dado.y*esjg[1]+0.5*esjg[1];
			ap->carro.cor=rand()%26;
			ap->carro.entrada=1;	

			ap->prox=lista;
			ap->ant=NULL;

			if(lista!=NULL)
				lista->ant=ap;

			lista=ap;
		}
		else
		{
			printf("CARRO COM A MATRICULA INDICADA JA ESTACIONADO\n");
			free(ap);
		}
	}
	return lista;
}

LISTA *OUT(LISTA *lista,int esjg[5],int mouse[3])	/*pede matricula do carro*/
{
	LISTA *aux;
	char matricula[MAX];

	if(CliquePosicao(1,mouse,2*(esjg[2]+80)/4,esjg[3]+80,1*(esjg[2]+80)/4,esjg[3]))
	{
		printf("OUT->MATRICULA=");
		scanf("%s",matricula);

		for(aux=lista;aux!=NULL;aux=aux->prox)
			if(strcmp(aux->carro.matricula,matricula)==0)
			{
				strcpy((aux->carro.estacionamento)->dado.cor,"laranja");

				aux->carro.ocupado=0;
				aux->carro.saida=1;

				strcpy((aux->carro.estacionamento)->dado.matricula,"");

				return lista;
			}
		printf("CARRO NAO ENCONTRADO\nCARRO NAO FOI RETIRADO\n");
	}
	return lista;
}

void AtualizaFicheiro(LUGARES *topo,char ficheiro[MAX])		/*guarda dados no ficheiro*/
{
	FILE *fp;									

	fp=fopen(ficheiro,"w");					
	if(fp==NULL)						
	{
		printf("ERRO NA ESCRITA NO FICHEIRO\n");
		return ;
	}

	for(;topo!=NULL;topo=topo->prox)
	{
		if(strcmp(topo->dado.codigo,"ES")==0||strcmp(topo->dado.codigo,"JG")==0||strcmp(topo->dado.codigo,"IN")==0||strcmp(topo->dado.codigo,"OUT")==0)
			fprintf(fp,"%s %d %d\n",topo->dado.codigo,topo->dado.x,topo->dado.y);

		if(strcmp(topo->dado.codigo,"ZE")==0)
			fprintf(fp,"%s %d %d %s %s %s\n",topo->dado.codigo,topo->dado.x,topo->dado.y,topo->dado.nome,topo->dado.cor, topo->dado.matricula);

		if(strcmp(topo->dado.codigo,"ZR")==0)
			fprintf(fp,"%s %d %d %d %d %s %s\n",topo->dado.codigo,topo->dado.x,topo->dado.y,topo->dado.largura,topo->dado.altura,topo->dado.nome,topo->dado.cor);

		if(strcmp(topo->dado.codigo,"ZER")==0)
			fprintf(fp,"%s %d %d %s\n",topo->dado.codigo,topo->dado.x,topo->dado.y, topo->dado.nome);
	}
	fclose(fp);
}

LUGARES *ApagarLUGARES(LUGARES *topo)
{
	LUGARES *ap;
	while(topo!=NULL)
	{
		ap=topo;
		topo=topo->prox;
		free(ap);
	}
	return topo;
}

void ApagarLINHA(ROBOT *vetor,int dim)
{
	LINHA *ap,*aux;
	int i;

	for(i=0;i<dim;i++)
		for(ap=vetor[i].linha;ap==NULL;ap=ap->prox)
		{
			aux=ap;
			ap=ap->prox;
			free(aux);
		}
}

LISTA *ApagarLISTA(LISTA *topo)
{
	LISTA *ap;
	while(topo!=NULL)
	{
		ap=topo;
		topo=topo->prox;
		free(ap);
	}
	return topo;
}

void DesenhaGrafico(int d, int dev, int mouse[3], int esjg[5],int botao[2], LUGARES **topo,ROBOT *vetor, int dimv,LISTA **lista,int pisca,int *robotn,int *robotm,int *list,int *velocidade)
{
	mouse[0]=Clique(d,dev,mouse,botao);


	DesenhaFicheiro(d,topo,esjg);
	DesenhaLinha(d,vetor,dimv,*robotn);
	DesenhaRobot(d,esjg,vetor,dimv);
	DesenhaCarros(d,*lista,esjg);
	DesenhaMenuParte1(d,esjg,vetor,*robotm);
	DesenhaMenuParte2(d,esjg,*topo,*lista);

	MovimentoRestricoes(topo,vetor,dimv,esjg,*robotn);
	*robotn=FazLINHA(topo,mouse,esjg,vetor,dimv,*robotn);

	RobotMOVECarro(*lista,vetor,dimv,esjg);
	*lista=RobotINCarro(*topo,*lista,vetor,dimv,esjg);
	*lista=RobotOUTCarro(*topo,*lista,vetor,dimv,esjg);

	*robotm=SelecionarROBOT(vetor,dimv,mouse,esjg,*robotm,*robotn);
	*velocidade=MudaVelocidadeManual(vetor,*robotm,mouse,esjg,*velocidade);
	MudaVelocidadeAutomatico(vetor,dimv,esjg);
	ChoqueROBOT(vetor,dimv,esjg);
	ControlaVelocidade(vetor,dimv);

	*lista=CorZE(*topo,*lista,esjg,pisca);
	CorROBOT(vetor,dimv,esjg,pisca,*robotn);

	fflush(stdout);
	g2_flush(d);

	*list=LIST(*lista,mouse,esjg,*list);
	*lista=IN(*lista,topo,esjg,mouse);
	*lista=OUT(*lista,esjg,mouse);	
}

void Raios(int esjg[5])
{
	if(esjg[0]<=esjg[1])
		esjg[4]=esjg[0];

	else
		esjg[4]=esjg[1];

}

void AbreGrafico(int botao[2],LUGARES **topo,ROBOT *vetor,LISTA *lista,int dimv)
{	
	int d,dev;
	int esjg[5];
	int mouse[3]={0,0,0};
	int robotn=-1,robotm=0;
	int list=0,velocidade=0;
	int pisca;
	
	DescobreESJG(topo,esjg);	
	MultiplicaPorES(vetor,dimv,lista,esjg);

	dev=g2_open_X11X(esjg[2]+80,esjg[3]+80,0,0,"parque2015","","",0,0);	
	d=g2_open_vd();
	g2_clear(d);
	g2_attach(d, dev);
	g2_set_auto_flush(d,0);
	g2_set_font_size(d, 1);

	for(pisca=0;CliquePosicao(1,mouse,(esjg[2]+80),esjg[3]+80,3*(esjg[2]+80)/4,esjg[3])==0 || EstacionadosROBOT(vetor,dimv,esjg);pisca++)
	{	
		lista=OrdenaLISTA(lista);
		DesenhaGrafico(d,dev,mouse,esjg,botao,topo,vetor,dimv,&lista,pisca,&robotn,&robotm,&list,&velocidade);

		usleep(10000);
		g2_clear(d);
	}
	g2_close(d);
}
