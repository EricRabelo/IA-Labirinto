#include "tadCelula.h"

/* TAD CELULA
	Objetivo: TAD contendo metodos de manipulacao das celulas de um labirinto.
*/

// Aloca memoria para armazenamento da celula.
tipo_Celula *tCel_CriaCelula() {
	tipo_Celula *cel = NULL;
	cel = (tipo_Celula *)malloc(sizeof(tipo_Celula));
	
	for (int i=0; i<NUMCAMINHOS; i++) {
		cel->pt_Caminhos[i] = NULL;
	}
	return cel;
}

// Libera memoria reservada para a celula.
void tCel_LiberaCelula(tipo_Celula *cel) {
	for (int i=0; i<NUMCAMINHOS; i++) {
		cel->pt_Caminhos[i] = NULL;
	}
	
	free(cel);
	cel = NULL;
}

// Cria link direcionado entre duas celulas.
bool tCel_LigaCelulas(tipo_Celula *cel1, tipo_Celula *cel2, const char *direcao) {
	bool flag_Link = false;
	
	if (cel2) {
		int i = 0;
		while ((i<NUMCAMINHOS) && (!flag_Link)) {
			if (!strcmp(id_Caminhos[i], direcao)) {
				cel1->pt_Caminhos[i] = cel2;
				flag_Link = true;
			}
			i++;
		}
	}
	return flag_Link;
}

// Apaga link direcionado de uma celula.
bool tCel_ApagaLink(tipo_Celula *cel, const char *direcao) {
	bool flag_Link = false;
	
	int i = 0;
	while ((i<NUMCAMINHOS) && (!flag_Link)) {
		if (!strcmp(id_Caminhos[i], direcao)) {
			cel->pt_Caminhos[i] = NULL;
			flag_Link = true;
		}
		i++;
	}
	
	return flag_Link;
}

bool tCel_LinkAtivo(tipo_Celula *cel, const char *direcao) {
	bool flag_Link = false;
	int i = 0;
	while ((i<NUMCAMINHOS) && (!flag_Link)) {
		if (!strcmp(id_Caminhos[i], direcao)) {
			if (cel->pt_Caminhos[i]) {
				flag_Link = true;
			}
		}
		i++;
	}
	return flag_Link;
}

bool tCel_CarregaID(tipo_Celula *cel, char *id) {
	strcpy(cel->id, id);
	
	return true;
}

char *tCel_VerCelula(tipo_Celula *cel, char *tipo) {
	char *inf;
	
	// Nome da celula.
	if (!strcmp(tipo, "id")) {
		strcpy(inf, cel->id);
	}
	else {
		strcpy(inf, "");
	}
	
	return inf;
}

tipo_Celula *tCel_VisitarCelula(tipo_Celula *cel, const char *direcao) {
	tipo_Celula *pt_Link = NULL;
	
	int i = 0;
	while ((i<NUMCAMINHOS) && (!pt_Link)) {
		if (!strcmp(id_Caminhos[i], direcao)) {
			pt_Link = cel->pt_Caminhos[i];
		}
		i++;
	}
	return pt_Link;
}




