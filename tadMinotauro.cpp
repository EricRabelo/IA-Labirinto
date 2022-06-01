#include "tadMinotauro.h"

/* TAD MINOTAURO
	Objetivo: TAD contendo metodos do minotauro no labirinto.
*/

// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 1 ***
//	Implementacao da primeira estrategia: BUSCA EM PROFUNDIDADE ALEATORIA.
int minot_Reticulado[MAXCEL][MAXCEL];
tipo_PosicaoPlano minot_Coord;
std::vector<const char *> minot_PilhaHistorico;

void init_Minotauro() {
	// Inicializa reticulado com valor negativo,
	// que indica que o minotauro ainda nao passou pela sala.
	for(int y=0; y<MAXCEL; y++) {
		for(int x=0; x<MAXCEL; x++) {
			minot_Reticulado[y][x]= -1;
		}
	}
	
	// Desmonta a pilha.
	while (minot_PilhaHistorico.size() > 0) {
		minot_PilhaHistorico.pop_back();
	}
	
	// Registra posicao atual do minotauro.
	minot_Coord.x = posAtualMin.x;
	minot_Coord.y = posAtualMin.y;
}

const char *run_Minotauro() {
	const char *movimento = "null";
	
	char outrojogador[MAXLINE];
	strcpy(outrojogador, "player");
	
	if (!strcmp(tLab_VerAmbiente(outrojogador),"sim")) {
		movimento = "captura";
		return movimento;
	}
	
	// Enquanto nao terminar o tempo de latencia.
	if (varTadMinot.count < 2) {
		// Conta mais rapido caso esteja em modo de perseguicao.
		if (varTadMinot.flag_Perseguir) {
			varTadMinot.count += 2;
		}
		else {
			varTadMinot.count++;
		}
		
		return movimento;
	}
	else {
		varTadMinot.count = 0;
	}
	
	if (!strcmp(tLab_VerAmbiente(outrojogador),"sim")) {
		movimento = "captura";
	}
	else {
		// Se nao estah em modo de perseguicao.
		if (!varTadMinot.flag_Perseguir) {
			// Verifica se existe algum jogador por perto.
			varTadMinot.flag_Perseguir = tadMinotauro_VerificaJogadorPerto();
			
			// Se entrou em modo de perseguicao.
			if (varTadMinot.flag_Perseguir) {
				// Apaga memoria da busca em profundidade.
				init_Minotauro();
			}
		}
		else {
			// Verifica se o jogador ainda estah visivel.
			varTadMinot.flag_Perseguir = tadMinotauro_VerificaJogadorVisivel();
		}
		
		// Verifica se o minotauro estah em modo de perseguicao.
		if (varTadMinot.flag_Perseguir) {
			// Persegue o jogador que foi localizado.
			movimento = tadMinotauro_PerseguicaoGulosa();
		}
		else {
			// Executa busca em profundidade.
			movimento = tadMinotauro_BuscaEmProfundidade();
		}
	}
	
	return movimento;
}

const char *tadMinotauro_BuscaEmProfundidade() {
	const char *movimento = "null";
	
	// Verifica se a sala ainda nao foi visitada.
	if (minot_Reticulado[minot_Coord.x][minot_Coord.y] == -1) {
		const char *origem = "null";
		// Verifica o caminho de origem.
		if (minot_PilhaHistorico.size() > 0) {
			origem = minot_PilhaHistorico[minot_PilhaHistorico.size()-1];
		}
		
		// Verifica novos caminhos, ignorando origem.
		minot_Reticulado[minot_Coord.x][minot_Coord.y] = 0;
		for (int c=0; c<NUMCAMINHOS; c++) {
			if ((strcmp(origem, id_Caminhos[c])) && (tLab_VerCaminho(id_Caminhos[c]) == CAMINHO)) {
				minot_Reticulado[minot_Coord.x][minot_Coord.y] += pow(2,c);
			}
		}
	}
	
	// Enquanto houver opcoes de seguir em frente, escolha um aleatoriamente.
	if (minot_Reticulado[minot_Coord.x][minot_Coord.y] > 0) {
		// Verifique quais caminhos estao abertos.
		int vet_Caminhos[NUMCAMINHOS];
		int count_Aux = 0;
		for (int c=0; c<NUMCAMINHOS; c++) {
			// bits: 1 Norte / 2 Sul / 4 Oeste / 8 Leste
			unsigned int mask = pow(2,c);
			if ((minot_Reticulado[minot_Coord.x][minot_Coord.y] & mask) == mask) {
				vet_Caminhos[count_Aux] = c;
				count_Aux++;
			}
		}
		int escolha = vet_Caminhos[rand()%count_Aux];
		
		// Grava movimento.
		movimento = id_Caminhos[escolha];
		
		// Fecha caminho escolhido.
		minot_Reticulado[minot_Coord.x][minot_Coord.y] -= pow(2,escolha);
		
		// Atualiza posicao atual.
		if (escolha == 0) minot_Coord.y--;
		else if (escolha == 1) minot_Coord.y++;
		else if (escolha == 2) minot_Coord.x--;
		else if (escolha == 3) minot_Coord.x++;
		
		// Empilha movimento de retorno.
		minot_PilhaHistorico.push_back(id_Retornos[escolha]);
	}
	else {
		// Se nao houver opcao de prosseguir, verifica retorno no caminho.
		movimento = minot_PilhaHistorico[minot_PilhaHistorico.size()-1];
		
		// Atualiza coordenada com base no movimento de retorno.
		if (!strcmp(movimento, "norte")) {
			minot_Coord.y--;
		}
		if (!strcmp(movimento, "sul")) {
			minot_Coord.y++;
		}
		if (!strcmp(movimento, "oeste")) {
			minot_Coord.x--;
		}
		if (!strcmp(movimento, "leste")) {
			minot_Coord.x++;
		}
		
		// Desempilha.
		minot_PilhaHistorico.pop_back();
	}
	
	return movimento;
}

const char *tadMinotauro_PerseguicaoGulosa() {
	const char *movimento = "null";
	int escolha = 0;
	float melhor_H = 2.0 * MAXCEL;
	
	// Verifique o melhor caminho aberto para capturar o agente.
	for (int c=0; c<NUMCAMINHOS; c++) {
		if (maze_VerCaminho(id_Caminhos[c]) == CAMINHO) {
			float h = tadMinotauro_DistEuclidianaParaAgentes(id_Caminhos[c]);
			if (h < melhor_H) {
				escolha = c;
				melhor_H = h;
			}
		}
	}
	
	// Grava movimento.
	movimento = id_Caminhos[escolha];
	
	return movimento;
}
float tadMinotauro_DistEuclidianaParaAgentes(const char *direcao) {
	float distP1 = 2.0 * MAXCEL;
	float distP2 = 2.0 * MAXCEL;
	tipo_PosicaoPlano posFutura;
	
	// Armazena a possivel posicao futura do minotauro.
	if (!strcmp(direcao, "norte")) {
		posFutura.x = posAtualMin.x;
		posFutura.y = posAtualMin.y-1;
	}
	else if (!strcmp(direcao, "sul")) {
		posFutura.x = posAtualMin.x;
		posFutura.y = posAtualMin.y+1;
	}
	else if (!strcmp(direcao, "oeste")) {
		posFutura.x = posAtualMin.x-1;
		posFutura.y = posAtualMin.y;
	}
	else if (!strcmp(direcao, "leste")) {
		posFutura.x = posAtualMin.x+1;
		posFutura.y = posAtualMin.y;
	}
	
	// Calcula a distancia euclidiana para os dois agentes.
	if (agentes[1].ativo) {
		distP1 = pow((posFutura.x - posAtualP1.x),2) + pow((posFutura.y - posAtualP1.y),2);
		distP1 = sqrt(distP1);
	}
	if (agentes[2].ativo) {
		distP2 = pow((posFutura.x - posAtualP2.x),2) + pow((posFutura.y - posAtualP2.y),2);
		distP2 = sqrt(distP2);
	}
	
	if (distP1 < distP2) {
		return distP1;
	}
	
	return distP2;
}

bool tadMinotauro_VerificaJogadorPerto() {
	bool flag_Achou = false;
	float distP1 = 2.0 * MAXCEL;
	float distP2 = 2.0 * MAXCEL;
	
	// Calcula a distancia euclidiana para os dois jogadores.
	if (agentes[1].ativo) {
		distP1 = pow((posAtualMin.x - posAtualP1.x),2) + pow((posAtualMin.y - posAtualP1.y),2);
		distP1 = sqrt(distP1);
	}
	if (agentes[2].ativo) {
		distP2 = pow((posAtualMin.x - posAtualP2.x),2) + pow((posAtualMin.y - posAtualP2.y),2);
		distP2 = sqrt(distP2);
	}
	
	// Se a distancia de um dos jogadores de no maximo 1 sala.
	if ((distP1 < DIST_PERTO) || (distP2 < DIST_PERTO)) {
		flag_Achou = true;
	}
	
	return flag_Achou;
}

bool tadMinotauro_VerificaJogadorVisivel() {
	bool flag_Achou = false;
	float distP1 = 2.0 * MAXCEL;
	float distP2 = 2.0 * MAXCEL;
	
	// Calcula a distancia euclidiana para os dois jogadores.
	if (agentes[1].ativo) {
		distP1 = pow((posAtualMin.x - posAtualP1.x),2) + pow((posAtualMin.y - posAtualP1.y),2);
		distP1 = sqrt(distP1);
	}
	if (agentes[2].ativo) {
		distP2 = pow((posAtualMin.x - posAtualP2.x),2) + pow((posAtualMin.y - posAtualP2.y),2);
		distP2 = sqrt(distP2);
	}
	
	// Se a distancia de um dos jogadores de no maximo 3 salas.
	if ((distP1 < DIST_VISIVEL) || (distP2 < DIST_VISIVEL)) {
		flag_Achou = true;
	}
	
	return flag_Achou;
}


