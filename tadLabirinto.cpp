/* TAD LABIRINTO
	Objetivo: TAD contendo metodos de manipulacao de um labirinto.
		Entrada: tabuleiro com mascarah.
		Saida: coordenadas e acao.
*/
#include "tadLabirinto.h"

bool tLab_CarregaArquivoMapa(char path[MAXLINE]) {
	int mapa[MAXCEL][MAXCEL];
	int tX, tY;
	
	FILE *fp;
	char buffer[MAXLINE];
	char *token;
	
	// Abre arquivo com instancia.
	if ((fp = fopen(path,"r")) == NULL) {
		printf("ERRO: ARQUIVO COM O MAPA NAO PODE SER ABERTO\n");
		exit(1);
	}
	
	// Leitura do cabecalho do arquivo.
	
	// Leitura das informacoes iniciais (num de linhas e colunas).
	if (!(fgets(buffer, MAXLINE, fp) == NULL)) {
		sscanf(buffer, "c %i %i", &tX, &tY);
	}
	if ((tX < 1) || (tX >= MAXCEL) || (tY < 1) || (tY >= MAXCEL)) {
		printf("ERRO: Limite no numero de celulas excedido!\n");
		return 0;
	}
	
	// Leitura do mapa.
	// Y eh a coordenada Vertical (linha).
	// X eh a coordenada Horizontal (coluna).
	for (int y=0; y<tY; y++) {
		if ((fgets(buffer, MAXLINE, fp) == NULL)) {
			printf("ERRO: Linhas faltando!\n");
			exit(1);
		}
		token=strtok(buffer,DELIM);
		sscanf(token, "%i", &mapa[0][y]);
		if ((mapa[0][y] < -15) || (mapa[0][y] > 15)) {
			printf("ERRO: Valor de celula invalido [0][%i]!\n", y);
			exit(1);
		}
		for (int x=1; x<tX; x++) {
			token=strtok(NULL, DELIM);
			sscanf(token, "%i", &mapa[x][y]);
			if ((mapa[x][y] < -15) || (mapa[x][y] > 15)) {
				printf("ERRO: Valor de celula invalido [%i][%i]!\n", x, y);
				exit(1);
			}
		}
	}
	
	// Fechamento do arquivo.
	fclose(fp);
	
	// Verifica informacoes existentes no mapa e cria o labirinto, caso o mapa seja valido.
	if (tLab_VerificaMapa2D(mapa, tX, tY)) {
		if (!tLab_CriaLabirinto(mapa, tX, tY)) {
			printf("ERRO: Labirinto nao foi criado!\n");
			return false;
		}
	}
	else {
		printf("ERRO: Mapa invalido!\n");
		return false;
	}
	
	return true;
}

// Verifica se os codigos armazenados nas celulas da matriz sao compativeis com a codificacao exigida.
bool tLab_VerificaMapa2D(int mapa[MAXCEL][MAXCEL], int tX, int tY) {
	unsigned int codigo, codigoViz;
	// bits: 1 Norte / 2 Sul / 4 Oeste / 8 Leste
	const unsigned int mask_Norte = 1;	// same as binary 0001
	const unsigned int mask_Sul = 2;	// same as binary 0010
	const unsigned int mask_Oeste = 4;	// same as binary 0100
	const unsigned int mask_Leste = 8;	// same as binary 1000
	
	// Y eh a coordenada Vertical (linha).
	// X eh a coordenada Horizontal (coluna).
	for (int x=0; x<tX; x++) {
		for (int y=0; y<tY; y++) {
			codigo = abs(mapa[x][y]);
			if ((y > 0) && ((codigo & mask_Norte) == mask_Norte)) {
				codigoViz = abs(mapa[x][y-1]);
				if ((codigoViz & mask_Sul) != mask_Sul) {
					printf("ERRO: link entre celulas nao eh bidirecional! [%d][%d]: Sul\n", x, y-1);
					return false;
				}
			}
			if ((y < (tY-1)) && ((codigo & mask_Sul) == mask_Sul)) {
				codigoViz = abs(mapa[x][y+1]);
				if ((codigoViz & mask_Norte) != mask_Norte) {
					printf("ERRO: link entre celulas nao eh bidirecional! [%d][%d]: Norte\n", x, y+1);
					return false;
				}
			}
			if ((x > 0) && ((codigo & mask_Oeste) == mask_Oeste)) {
				codigoViz = abs(mapa[x-1][y]);
				if ((codigoViz & mask_Leste) != mask_Leste) {
					printf("ERRO: link entre celulas nao eh bidirecional! [%d][%d]: Leste\n", x-1, y);
					return false;
				}
			}
			if ((x < (tX-1)) && ((codigo & mask_Leste) == mask_Leste)) {
				codigoViz = abs(mapa[x+1][y]);
				if ((codigoViz & mask_Oeste) != mask_Oeste) {
					printf("ERRO: link entre celulas nao eh bidirecional! [%d][%d]: Oeste\n", x+1, y);
					return false;
				}
			}
		}
	}
	
	return true;
}

bool tLab_AlocaLabirinto2D(int tX, int tY) {
	char id[MAXLINE];
	int flag_Link;
	
	lab = (tipo_Labirinto *)malloc(sizeof(tipo_Labirinto));
	
	// X eh a coordenada Horizontal (coluna).
	// Y eh a coordenada Vertical (linha).
	lab->num_CoordX = tX;
	lab->num_CoordY = tY;
	
	// Aloca celulas.
	for (int x=0; x<tX; x++) {
		for (int y=0; y<tY; y++) {
			lab->matriz[x][y] = tCel_CriaCelula();
		}
	}
	
	for (int x=0; x<tX; x++) {
		for (int y=0; y<tY; y++) {
			// Define o ID da celula.
			sprintf(id, "%i*%i", x, y);
			tCel_CarregaID(lab->matriz[x][y], id);
			
			// Cria conexoes entre as celulas.
			if (y > 0) {
				flag_Link = tCel_LigaCelulas(lab->matriz[x][y], lab->matriz[x][y-1], id_Caminhos[0]);
				if (!flag_Link) {
					printf("ERRO: nao foi possivel estabelecer um link ao %s.\n", id_Caminhos[0]);
					return false;
				}
			}
			if (y < (tY-1)) {
				flag_Link = tCel_LigaCelulas(lab->matriz[x][y], lab->matriz[x][y+1], id_Caminhos[1]);
				if (!flag_Link) {
					printf("ERRO: nao foi possivel estabelecer um link ao %s.\n", id_Caminhos[1]);
					return false;
				}
			}
			if (x > 0) {
				flag_Link = tCel_LigaCelulas(lab->matriz[x][y], lab->matriz[x-1][y], id_Caminhos[2]);
				if (!flag_Link) {
					printf("ERRO: nao foi possivel estabelecer um link ao %s.\n", id_Caminhos[2]);
					return false;
				}
			}
			if (x < (tX-1)) {
				flag_Link = tCel_LigaCelulas(lab->matriz[x][y], lab->matriz[x+1][y], id_Caminhos[3]);
				if (!flag_Link) {
					printf("ERRO: nao foi possivel estabelecer um link ao %s.\n", id_Caminhos[3]);
					return false;
				}
			}
		}
	}
	
	return true;
}

// Cria labirinto.
bool tLab_CriaLabirinto(int mapa[MAXCEL][MAXCEL], int tX, int tY) {
	// Aloca todas as celulas do labirinto e cria conexoes entre todas as suas celulas.
	if (!tLab_AlocaLabirinto2D(tX, tY)) {
		printf("ERRO: nao foi possivel alocar memoria para o labirinto.\n");
		return false;
	}
	
	// Apaga conexoes (cria paredes) entre celulas.
	// *** Trecho que serah ativado de acordo com o labirinto ***
	// X eh a coordenada Horizontal (coluna).
	// Y eh a coordenada Vertical (linha).
	count_VetSaidas = 0;
	for (int x=0; x<tX; x++) {
		for (int y=0; y<tY; y++) {
			for (int c=0; c<NUMCAMINHOS; c++) {
				// bits: 1 Norte / 2 Sul / 4 Oeste / 8 Leste
				unsigned int mask = pow(2,c);
				
				unsigned int codigo = abs(mapa[x][y]);
				if ((codigo & mask) != mask) {
					tCel_ApagaLink(lab->matriz[x][y], id_Caminhos[c]);
				}
				// Trata celulas de saida.
				if (mapa[x][y] < 0) {
					char id[MAXLINE];
					sprintf(id, "saida_%i*%i", x, y);
					tCel_CarregaID(lab->matriz[x][y], id);
					
					// Grava coordenadas das salas de saida.
					// Este trecho deverah ser alterado para permitir mais de uma saida.
					vet_Saidas[count_VetSaidas].x = x;
					vet_Saidas[count_VetSaidas].y = y;
					count_VetSaidas++;
				}
			}
		}
	}
	
	return true;
}

// Libera memoria reservada para o labirinto.
bool tLab_ApagaLabirinto() {
	for (int x=0; x<lab->num_CoordX; x++) {
		for (int y=0; y<lab->num_CoordY; y++) {
			tCel_LiberaCelula(lab->matriz[x][y]);
			lab->matriz[x][y] = NULL;
		}
	}
	
	free(lab);
	lab = NULL;
	
	return true;
}

// Posiciona agentes no labirinto.
void tLab_InicializaAgente(int x, int y) {
	agentes[agenteAtual].sala = lab->matriz[x][y];
	agentes[agenteAtual].custoPasso = 0;
	agentes[agenteAtual].custoVisao = 0;
	agentes[agenteAtual].custoCalc = 0;
	agentes[agenteAtual].ativo = true;
}

// Passa o direito de atuar para o proximo agente.
void tLab_PassaParaProximoAgente() {
	agenteAtual++;
	
	if (agenteAtual == NUM_AGENTES) {
		agenteAtual = 0;
	}
}

// Visualiza alguma informacao especifica do ambiente em que o jogador estah.
char *tLab_VerAmbiente(char tipo[MAXLINE]) {
	char *info = NULL;
	info = (char *)malloc(sizeof(char));
	
	if (agenteAtual == 0) {
		// Verifica se existe outro jogador na sala.
		if (!strcmp(tipo, "player")) {
			if ((agentes[0].sala == agentes[1].sala) && agentes[1].ativo) {
				strcpy(info, "sim");
			}
			else if ((agentes[0].sala == agentes[2].sala) && agentes[2].ativo) {
				strcpy(info, "sim");
			}
			else {
				strcpy(info, "nao");
			}
		}
		else {
			strcpy(info, tCel_VerCelula(agentes[0].sala, tipo));
		}
	}
	else {
		// Verifica se o minotauro estah na sala.
		if (!strcmp(tipo, "minotauro")) {
			if (agentes[agenteAtual].sala == agentes[0].sala) {
				strcpy(info, "sim");
			}
			else {
				strcpy(info, "nao");
			}
		}
		else {
			strcpy(info, tCel_VerCelula(agentes[agenteAtual].sala, tipo));
		}
	}
	
	agentes[agenteAtual].custoVisao++;
	return info;
}

// Ver um caminho especifico no ambiente em que o jogador estah.
// Saida: valor booleando que indica se existe uma parede ou um caminho.
bool tLab_VerCaminho(const char *direcao) {
	if (tCel_LinkAtivo(agentes[agenteAtual].sala, direcao)) {
		return CAMINHO;
	}
	
	agentes[agenteAtual].custoVisao++;
	return PAREDE;
}

// Verifica se o minotauro estah na sala ao lado do agente,
// indicando a direcao da sala.
bool tLab_VerMinotauro(const char *direcao) {
	bool flag_Achou = false;
	tipo_Celula *salaAoLado = NULL;
	
	if (agentes[agenteAtual].ativo) {
		// Verifica a sala ao lado.
		salaAoLado = tCel_VisitarCelula(agentes[agenteAtual].sala, direcao);
		
		// Verifica se o minotauro estah nesta sala.
		if (salaAoLado == agentes[0].sala) {
			flag_Achou = true;
		}
	}
	
	agentes[agenteAtual].custoVisao++;
	return flag_Achou;
}

// Mover o jogador para uma direcao especifica.
bool tLab_MoveJogador(const char *direcao) {
	bool flag_Movimento = false;
	tipo_Celula *proximo = NULL;
	
	if (agentes[agenteAtual].ativo) {
		proximo = tCel_VisitarCelula(agentes[agenteAtual].sala, direcao);
		if (proximo) {
			agentes[agenteAtual].sala = proximo;
			flag_Movimento = true;
		}
	}
	
	agentes[agenteAtual].custoPasso++;
	return flag_Movimento;
}

// Captura outro jogador que estah na mesma sala.
bool tLab_CapturaAgente() {
	bool flag_Captura = false;
	int i, outroagente;
	
	// Procura agente (exceto o Minotauro) diferente na mesma sala.
	i = 1;
	do {
		if ((i != agenteAtual) && (agentes[i].sala == agentes[agenteAtual].sala) && (agentes[i].ativo)) {
			flag_Captura = true;
			outroagente = i;
		}
		i++;
	} while ((i < NUM_AGENTES) && (!flag_Captura));
	
	// Desativa o agente capturado.
	if (flag_Captura) {
		agentes[agenteAtual].custoPasso++;
		agentes[outroagente].ativo = false;
	}
	
	return flag_Captura;
}

float tLab_CustoDoCaminho(const char *direcao) {
	agentes[agenteAtual].custoCalc++;
	return 1.0;
}

float tLab_HeuristicaDistEuclidiana(int x, int y) {
	float dist = 0.0, melhorDist = 2.0 * MAXCEL;
	
	for (int i=0; i<count_VetSaidas; i++) {
		// Calcula a distancia euclidiana para a saida atual.
		dist = pow((x - vet_Saidas[i].x), 2);
		dist = dist + pow((y - vet_Saidas[i].y), 2);
		dist = sqrt(dist);
		
		// Atualize se for a menor distancia encontrada.
		if ((i == 0) || (dist < melhorDist)) {
			melhorDist = dist;
		}
	}
	
	agentes[agenteAtual].custoCalc++;
	return melhorDist;
}



void tLab_ImprimeCustosDoAgente() {
	// Imprime cabecalho.
	if (agenteAtual == 0) {
		printf("==========================\n");
		printf(" Desempenho do Minotauro:\n");
	}
	else {
		printf("**************************\n");
		printf(" Desempenho do Agente %i:\n", agenteAtual);
	}
	
	printf("\t# de Passos: %i\n", agentes[agenteAtual].custoPasso);
	printf("\t# de Visualizacoes: %i\n", agentes[agenteAtual].custoVisao);
	printf("\t# de Calculos: %i\n\n", agentes[agenteAtual].custoCalc);
}

