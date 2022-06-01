#ifndef GLOBAIS_H
#define GLOBAIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXLINE 1000
#define DELIM	" "
#define MAXCEL 65

#define PAREDE false
#define CAMINHO true

#define NUMCAMINHOS 4
const char *id_Caminhos[NUMCAMINHOS] = {"norte", "sul", "oeste", "leste"};
const char *id_Retornos[NUMCAMINHOS] = {"sul", "norte", "leste", "oeste"};

// Estrutura que indica posicao no plano cartesiano.
typedef struct str_PosicaoPlano {
	int x,y;
} tipo_PosicaoPlano;

#endif // GLOBAIS_H

