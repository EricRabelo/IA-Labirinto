#ifndef TADCELULA_H
#define TADCELULA_H

/* TAD CELULA
	Objetivo: TAD contendo metodos de manipulacao das celulas de um labirinto.
*/
#include "globais.h"

typedef struct str_Celula {
	// Informacoes da celula.
	char id[MAXLINE];
	
	str_Celula *pt_Caminhos[NUMCAMINHOS];
} tipo_Celula;



tipo_Celula *tCel_CriaCelula();
void tCel_LiberaCelula(tipo_Celula *cel);
bool tCel_LigaCelulas(tipo_Celula *cel1, tipo_Celula *cel2, const char *direcao);
bool tCel_ApagaLink(tipo_Celula *cel, const char *direcao);
bool tCel_LinkAtivo(tipo_Celula *cel, const char *direcao);
bool tCel_CarregaID(tipo_Celula *cel, char *id);
char *tCel_VerCelula(tipo_Celula *cel, char *tipo);
tipo_Celula *tCel_VisitarCelula(tipo_Celula *cel, const char *direcao);

//#include "tadCelula.cpp"

#endif // TADCELULA_H

