#ifndef MAZE_H
#define MAZE_H

#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include "tadLabirinto.cpp"
#include "tadMinotauro.h"

#define width 800
#define height 600
#define xInicialA -0.99
#define yInicialA 0.99
#define xInicialB -0.96
#define yInicialB 0.94
#define passoX 0.03
#define passoY 0.05

using namespace std;

typedef struct structSala
{
	int id;
	bool pNorte, pSul, pLeste, pOeste; // true -> existe parede, false -> não existe parede.
	float x1, y1, x2, y2, cX, cY;
	float red, green, blue;
	bool flag_P1Visita, flag_P2Visita, flag_MinVisita;
} CoordSala;

vector<vector<CoordSala> > labGraf;
tipo_PosicaoPlano posAtualP1, posAtualP2, posAtualMin;

int blocosX;
int blocosY;

// ------------------------------------------------------------------------
//	Estrategias que devem ser implementadas pelo usuario.
// ------------------------------------------------------------------------
void init_Player1();
const char *run_Player1();
void init_Player2();
const char *run_Player2();

// ------------------------------------------------------------------------
//	Funcoes que podem ser utilizadas pelo usuario.
// ------------------------------------------------------------------------
char *maze_VerAmbiente(char tipo[MAXLINE]);
bool maze_VerCaminho(const char *direcao);
bool maze_VerMinotauro(const char *direcao);
float maze_CustoDoCaminho(const char *direcao);
float maze_HeuristicaDistEuclidiana(const char *direcao);


// ------------------------------------------------------------------------
//	Funcoes internas.
// ------------------------------------------------------------------------
void run_Players(int lixo);
bool saida(tipo_PosicaoPlano pos);
bool moveJogador(const char *direcao);
void grid();
void desenha_player1();
void desenha_player2();
void desenha_minotauro();
void display(void);
void wasd (unsigned char key, int x, int y);
void setas(int key, int x, int y);
void cria_LabirintoGrafico(char* path);

#include "tadMinotauro.cpp"

#endif // MAZE_H

