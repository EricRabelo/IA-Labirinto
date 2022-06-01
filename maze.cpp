#include "maze.h"

// ------------------------------------------------------------------------
//	Funcoes que podem ser utilizadas pelo usuario.
// ------------------------------------------------------------------------
char *maze_VerAmbiente(char tipo[MAXLINE]) {
	return tLab_VerAmbiente(tipo);
}

bool maze_VerCaminho(const char *direcao) {
	return tLab_VerCaminho(direcao);
}

bool maze_VerMinotauro(const char *direcao) {
	return tLab_VerMinotauro(direcao);
}

float maze_CustoDoCaminho(const char *direcao) {
	return tLab_CustoDoCaminho(direcao);
}

float maze_HeuristicaDistEuclidiana(const char *direcao) {
	float dist = 0.0;
	
	if (!strcmp(direcao, "norte")) {
		if (agenteAtual == 1)
			dist = tLab_HeuristicaDistEuclidiana(posAtualP1.x, posAtualP1.y-1);
		else
			dist = tLab_HeuristicaDistEuclidiana(posAtualP2.x, posAtualP2.y-1);
	}
	else if (!strcmp(direcao, "sul")) {
		if(agenteAtual == 1)
			dist = tLab_HeuristicaDistEuclidiana(posAtualP1.x, posAtualP1.y+1);
		else
			dist = tLab_HeuristicaDistEuclidiana(posAtualP2.x, posAtualP2.y+1);
	}
	else if (!strcmp(direcao, "oeste")) {
		if(agenteAtual == 1)
			dist = tLab_HeuristicaDistEuclidiana(posAtualP1.x-1, posAtualP1.y);
		else
			dist = tLab_HeuristicaDistEuclidiana(posAtualP2.x-1, posAtualP2.y);
	}
	else if (!strcmp(direcao, "leste")) {
		if(agenteAtual == 1)
			dist = tLab_HeuristicaDistEuclidiana(posAtualP1.x+1, posAtualP1.y);
		else
			dist = tLab_HeuristicaDistEuclidiana(posAtualP2.x+1, posAtualP2.y);
	}
	
	return dist;
}


// ------------------------------------------------------------------------
//	Funcoes internas.
// ------------------------------------------------------------------------
void run_Players(int lixo) {
	const char *movimento;
	bool winnerP1 = false;
	bool winnerP2 = false;
	
	winnerP1 = saida(posAtualP1);
	winnerP2 = saida(posAtualP2);
//	cout << "P1: " << winnerP1 << " " << "P2: " << winnerP2 << endl;;
	
	if (winnerP1)
		cout << "Agente 1 VENCEDOR" << endl;
	if (winnerP2)
		cout << "Agente 2 VENCEDOR" << endl;
	
	if (!agentes[1].ativo && !agentes[2].ativo)
		cout << "MINOTAURO VENCEDOR" << endl;
	
	if ((!winnerP1) && (!winnerP2) && (agentes[1].ativo || agentes[2].ativo)) {
		if (agenteAtual == 0) {
			movimento = run_Minotauro();
		}
		else if (agenteAtual == 1) {
			movimento = run_Player1();
		}
		else {
			movimento = run_Player2();
		}
		
		// Verifica se o agente optou por capturar outro agente.
		if (!strcmp(movimento, "captura")) {
			tLab_CapturaAgente();
		}
		else {
			// Move o agente no labirinto.
			moveJogador(movimento);
			
			// Se for o minotauro, ele pode tentar capturar um agente ao entrar na sala.
			if ((agenteAtual == 0)) {
				tLab_CapturaAgente();
			}
		}
		
		// Passa a jogada para o proximo agente.
		tLab_PassaParaProximoAgente();
		
		glutPostRedisplay();
		
		glutTimerFunc(5, run_Players, 0);
	}
	else {
		// Impre relatorio do minotauro.
		agenteAtual = 0;
		tLab_ImprimeCustosDoAgente();
		
		// Impre relatorio do agente 1.
		agenteAtual = 1;
		tLab_ImprimeCustosDoAgente();
		
		// Impre relatorio do agente 2.
		agenteAtual = 2;
		tLab_ImprimeCustosDoAgente();
	}
}

bool saida(tipo_PosicaoPlano pos) {
	if(lab->matriz[pos.x][pos.y]->id[0]=='s')
		return true;
	return false;
}

bool moveJogador(const char *direcao) {
	bool flag_Movimento = false;
	
	// Movimenta jogador na direcao escolhida.
	if (maze_VerCaminho(direcao) == CAMINHO) {
		// Movimenta jogador de acordo com a estrategia (coordenada) escolhida.
		if (tLab_MoveJogador(direcao)) {
			// Manipula visualmente o jogador.
			if (!strcmp(direcao, "norte")) {
				if (agenteAtual == 0)
					posAtualMin.y = posAtualMin.y-1;
				else if (agenteAtual == 1)
					posAtualP1.y = posAtualP1.y-1;
				else
					posAtualP2.y = posAtualP2.y-1;
			}
			else if (!strcmp(direcao, "sul")) {
				if (agenteAtual == 0)
					posAtualMin.y = posAtualMin.y+1;
				else if (agenteAtual == 1)
					posAtualP1.y = posAtualP1.y+1;
				else
					posAtualP2.y = posAtualP2.y+1;
			}
			else if (!strcmp(direcao, "oeste")) {
				if (agenteAtual == 0)
					posAtualMin.x = posAtualMin.x-1;
				else if (agenteAtual == 1)
					posAtualP1.x = posAtualP1.x-1;
				else
					posAtualP2.x = posAtualP2.x-1;
			}
			else if (!strcmp(direcao, "leste")) {
				if (agenteAtual == 0)
					posAtualMin.x = posAtualMin.x+1;
				else if (agenteAtual == 1)
					posAtualP1.x = posAtualP1.x+1;
				else
					posAtualP2.x = posAtualP2.x+1;
			}
			
			flag_Movimento = true;
		}
	}
/*	if (flag_Movimento) {
		if (flag_Player1) {
			cout << "Jogador 01 - Foi para o " << direcao << endl;
		}
		else {
			cout << "Jogador 02 - Foi para o " << direcao << endl;
		}
	}
	else {
		cout << "Movimento para o " << direcao << " mal sucedido!!!" << endl;
	}
*/	
	return flag_Movimento;
}

void grid() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glLineWidth(3);
	float red = 0.4;
	float green = 0.4;
	float blue = 0.8;
	
	for(int i=0;i<blocosX;i++) {
		glPushMatrix();
		for(int j=0;j<blocosY;j++) {
			glColor3f(labGraf[i][j].red,labGraf[i][j].green,labGraf[i][j].blue);
			glRectf(labGraf[i][j].x1,labGraf[i][j].y1,labGraf[i][j].x2,labGraf[i][j].y2);
			
			glPushMatrix();
			if(labGraf[i][j].pNorte) {
				glBegin(GL_LINES);
				glColor3f(red,green,blue);
				glVertex2f(labGraf[i][j].x1,labGraf[i][j].y1);
				glVertex2f(labGraf[i][j].x2,labGraf[i][j].y1);
				glEnd();
			}
			
			if(labGraf[i][j].pSul) {
				glBegin(GL_LINES);
				glColor3f(red,green,blue);
				glVertex2f(labGraf[i][j].x1,labGraf[i][j].y2);
				glVertex2f(labGraf[i][j].x2,labGraf[i][j].y2);
				glEnd();
			}
			
			if(labGraf[i][j].pLeste) {
				glBegin(GL_LINES);
				glColor3f(red,green,blue);
				glVertex2f(labGraf[i][j].x2,labGraf[i][j].y1);
				glVertex2f(labGraf[i][j].x2,labGraf[i][j].y2);
				glEnd();
			}
			
			if(labGraf[i][j].pOeste) {
				glBegin(GL_LINES);
				glColor3f(red,green,blue);
				glVertex2f(labGraf[i][j].x1,labGraf[i][j].y1);
				glVertex2f(labGraf[i][j].x1,labGraf[i][j].y2);
				glEnd();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
}

void desenha_player1() {
	glPointSize(4);
	int i = posAtualP1.x;
	int j = posAtualP1.y;
	
	if (labGraf[i][j].flag_P1Visita) {
		if (labGraf[i][j].flag_P2Visita) {
			labGraf[i][j].red -= 0.2;
			labGraf[i][j].green += 0.3;
			labGraf[i][j].blue -= 0.1;
		}
		else {
			labGraf[i][j].red -= 0.2;
			labGraf[i][j].green += 0.1;
			labGraf[i][j].blue -= 0.2;
		}
	}
	else {
		labGraf[i][j].flag_P1Visita = true;
		if (labGraf[i][j].flag_P2Visita) {
			labGraf[i][j].red -= 0.2;
			labGraf[i][j].green += 0.3;
			labGraf[i][j].blue -= 0.1;
		}
		else {
			labGraf[i][j].red = 0.5;
			labGraf[i][j].green = 1.0;
			labGraf[i][j].blue = 0.5;
		}
	}
	
	glBegin(GL_POINTS);
	glColor3f(0.2, 0.6, 0.2);
	glVertex2f(labGraf[i][j].cX,labGraf[i][j].cY);
	glEnd();
}

void desenha_player2() {
	glPointSize(4);
	int i = posAtualP2.x;
	int j = posAtualP2.y;
	
	if (labGraf[i][j].flag_P2Visita) {
		if (labGraf[i][j].flag_P1Visita) {
			labGraf[i][j].red += 0.4;
			labGraf[i][j].green -= 0.3;	
			labGraf[i][j].blue -= 0.1;
		}
		else {
			labGraf[i][j].red += 0.1;
			labGraf[i][j].green -= 0.2;
			labGraf[i][j].blue -= 0.2;
		}
	}
	else {
		labGraf[i][j].flag_P2Visita = true;
		if (labGraf[i][j].flag_P1Visita) {
			labGraf[i][j].red += 0.4;
			labGraf[i][j].green -= 0.3;	
			labGraf[i][j].blue -= 0.1;
		}
		else {
			labGraf[i][j].red = 1.0;
			labGraf[i][j].green = 0.5;
			labGraf[i][j].blue = 0.5;
		}
	}
	
	glBegin(GL_POINTS);
	glColor3f(0.6, 0.2, 0.2);
	glVertex2f(labGraf[i][j].cX,labGraf[i][j].cY);
	glEnd();
}

void desenha_miontauro() {
	glPointSize(6);
	int i = posAtualMin.x;
	int j = posAtualMin.y;
	
	if(!saida(posAtualMin)) {
		labGraf[i][j].red = 0.8;
		labGraf[i][j].green = 0.8;
		labGraf[i][j].blue = 0.8;
	}
	
	glBegin(GL_POINTS);
	glColor3f(0.2, 0.2, 0.2);
	glVertex2f(labGraf[i][j].cX,labGraf[i][j].cY);
	glEnd();
}

void display(void) {
	grid();
	
	if (agenteAtual == 0) desenha_miontauro();
	else if (agenteAtual == 1) desenha_player1();
	else desenha_player2();

	glFlush();
}

void wasd (unsigned char key, int x, int y) {
	switch (key) {
		case 'D': // Direita
		case 'd':
			if((posAtualP1.x >= 0)&&(posAtualP1.x < blocosX-1)&&(!labGraf[posAtualP1.x][posAtualP1.y].pLeste))
				posAtualP1.x = posAtualP1.x+1;
			break;

		case 'A': // Esquerda
		case 'a':
			if((posAtualP1.x > 0)&&(posAtualP1.x < blocosX)&&(!labGraf[posAtualP1.x][posAtualP1.y].pOeste))
				posAtualP1.x = posAtualP1.x-1;
			break;

		case 'W': // Norte
		case 'w':
			if((posAtualP1.y > 0)&&(posAtualP1.y < blocosY)&&(!labGraf[posAtualP1.x][posAtualP1.y].pNorte))
				posAtualP1.y = posAtualP1.y-1;
			break;

		case 'S': //Sul
		case 's':
			if((posAtualP1.y >= 0)&&(posAtualP1.y < blocosY-1)&&(!labGraf[posAtualP1.x][posAtualP1.y].pSul))
				posAtualP1.y = posAtualP1.y+1;
			break;
	}
	
	glutPostRedisplay();
}

void setas(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_DOWN:
			if((posAtualP2.y >= 0)&&(posAtualP2.y < blocosY-1)&&(!labGraf[posAtualP2.x][posAtualP2.y].pSul))
				posAtualP2.y = posAtualP2.y+1;
			break;

		case GLUT_KEY_UP:
			if((posAtualP2.y > 0)&&(posAtualP2.y < blocosY)&&(!labGraf[posAtualP2.x][posAtualP2.y].pNorte))
				posAtualP2.y = posAtualP2.y-1;
			break;

		case GLUT_KEY_RIGHT:
			if((posAtualP2.x >= 0)&&(posAtualP2.x < blocosX-1)&&(!labGraf[posAtualP2.x][posAtualP2.y].pLeste))
				posAtualP2.x = posAtualP2.x+1;
			break;

		case GLUT_KEY_LEFT:
			if((posAtualP2.x > 0)&&(posAtualP2.x < blocosX)&&(!labGraf[posAtualP2.x][posAtualP2.y].pOeste))
				posAtualP2.x = posAtualP2.x-1;
			break;
	}
	
	glutPostRedisplay();
}

void cria_LabirintoGrafico(char* path) {
	if (!tLab_CarregaArquivoMapa(path)) {
		printf("ERRO: Labirinto invalido!\n");
		exit(1);
	}
	
	// Inicializa posicao do Minotauro (agente 0).
	agenteAtual = 0;
	posAtualMin.x = rand() % lab->num_CoordX;
	posAtualMin.y = rand() % lab->num_CoordY;
	//posAtualMin.x = 12;
	//posAtualMin.y = 1;
	tLab_InicializaAgente(posAtualMin.x, posAtualMin.y);

	// Inicializa posicao da primeira estrategia do usuario (agente 1).
	agenteAtual = 1;
	posAtualP1.x = 0;
	posAtualP1.y = 0;
	tLab_InicializaAgente(posAtualP1.x, posAtualP1.y);

	// Inicializa posicao da segunda estrategia do usuario (agente 2).
	agenteAtual = 2;
	posAtualP2.x = 0;
	posAtualP2.y = 0;
	tLab_InicializaAgente(posAtualP2.x, posAtualP2.y);
	
	// Define o primeiro agente a efetuar o movimento.
	agenteAtual = 0;
	
	blocosX = lab->num_CoordX;
	blocosY = lab->num_CoordY;

	labGraf.resize(MAXCEL);
	for(int i=0;i<labGraf.size();i++)
		labGraf[i].resize(MAXCEL);

	for(int i=0;i<MAXCEL;i++) {
		for(int j=0;j<MAXCEL;j++) {
			labGraf[i][j].pNorte=false;
			labGraf[i][j].pSul=false;
			labGraf[i][j].pLeste=false;
			labGraf[i][j].pOeste=false;
		}
	}
	
	for(int j=0; j<blocosX;j++) {
		for(int i=0; i<blocosY;i++) {
			labGraf[j][i].x1 = xInicialA+j*(passoX);
			labGraf[j][i].x2 = xInicialB+j*(passoX);

			labGraf[j][i].y1 = yInicialA-i*(passoY);
			labGraf[j][i].y2 = yInicialB-i*(passoY);

			labGraf[j][i].cX = (labGraf[j][i].x1+labGraf[j][i].x2)/2.0;
			labGraf[j][i].cY = (labGraf[j][i].y1+labGraf[j][i].y2)/2.0;
			
			labGraf[j][i].red = 0.9;
			labGraf[j][i].green = 1.0;
			labGraf[j][i].blue = 0.9;
			labGraf[j][i].flag_MinVisita = false;
			labGraf[j][i].flag_P1Visita = false;
			labGraf[j][i].flag_P2Visita = false;
			
			if(!tCel_LinkAtivo(lab->matriz[j][i],id_Caminhos[0]))
				labGraf[j][i].pNorte=true;
			
			if(!tCel_LinkAtivo(lab->matriz[j][i],id_Caminhos[1]))
				labGraf[j][i].pSul=true;
			
			if(!tCel_LinkAtivo(lab->matriz[j][i],id_Caminhos[2]))
				labGraf[j][i].pOeste=true;
			
			if(!tCel_LinkAtivo(lab->matriz[j][i],id_Caminhos[3]))
				labGraf[j][i].pLeste=true;
			
			if(lab->matriz[j][i]->id[0]=='s') {
				labGraf[j][i].red = 0.8;
				labGraf[j][i].green = 0.8;
				labGraf[j][i].blue = 0.0;
			}
		}
	}
}


