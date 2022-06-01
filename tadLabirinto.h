#ifndef TADLABIRINTO_H
#define TADLABIRINTO_H

/* TAD LABIRINTO
	Objetivo: TAD contendo metodos de manipulacao de um labirinto.
		Entrada: tabuleiro com mascarah.
		Saida: coordenadas e acao.
*/
#include "tadCelula.cpp"

typedef struct str_Labirinto {
	tipo_Celula *matriz[MAXCEL][MAXCEL];
	int num_CoordX, num_CoordY;
	
} tipo_Labirinto;
tipo_Labirinto *lab = NULL;

typedef struct str_Agente {
	tipo_Celula *sala = NULL;
	int custoPasso, custoVisao, custoCalc;
	bool ativo;
} tipo_Agente;

// Vetor com coordanadas das saidas.
tipo_PosicaoPlano vet_Saidas[MAXCEL];
int count_VetSaidas = 0;

#define NUM_AGENTES 3
tipo_Agente agentes[NUM_AGENTES];
int agenteAtual = 0;

// Funcoes de manipulacao do labirinto.
bool tLab_CarregaArquivoMapa(char path[MAXLINE]);
bool tLab_VerificaMapa2D(int mapa[MAXCEL][MAXCEL], int tX, int tY);
bool tLab_AlocaLabirinto2D(int tX, int tY);
bool tLab_CriaLabirinto(int mapa[MAXCEL][MAXCEL], int tX, int tY);
bool tLab_ApagaLabirinto();
void tLab_InicializaAgente(int x, int y);
void tLab_PassaParaProximoAgente();
bool tLab_CapturaAgente();

// Informacoes do ambiente.
char *tLab_VerAmbiente(char tipo[MAXLINE]);
bool tLab_VerCaminho(const char *direcao);
bool tLab_VerMinotauro(const char *direcao);
bool tLab_MoveJogador(const char *direcao);
bool tLab_CapturaAgente();
float tLab_CustoDoCaminho(const char *direcao);
float tLab_HeuristicaDistEuclidiana(int x, int y);
void tLab_ImprimeCustosDoAgente();

//#include "tadLabirinto.cpp"

#endif // TADLABIRINTO_H
