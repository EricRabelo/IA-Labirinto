/*
	Autor: Luis Otavio Rigo Junior
	Objetivo: 
		Este arquivo destina-se a implementacao das estrategias de jogo dos agentes.
	
	Devem ser implementados os 4 prototipos:
		init_Player1 - executada uma unica vez e contem as funcoes de inicializacao da estrategia do jogador 1;
		run_Player1 - executado a cada passo do jogo e retorna uma string com a direcao de movimento do jogador 1;
		init_Player2 - executada uma unica vez e contem as funcoes de inicializacao da estrategia do jogador 2;
		run_Player2 - executado a cada passo do jogo e retorna uma string com a direcao de movimento do jogador 2.
	
	Funcoes que podem ser chamadas pelo jogador (agente):
		char *maze_VerAmbiente(char tipo[MAXLINE]);
			- Utilizada para verificar alguma informacao da celula. Ex.: id;
		bool maze_VerCaminho(const char *direcao);
			- Retorna se existe um caminho naquela direcao (verdadeiro ou falso).
		bool maze_VerMinotauro(const char *direcao);
			- Retorna se o minotauro estah naquela direcao (verdadeiro ou falso).
		float maze_CustoDoCaminho(const char *direcao);
			- Retorna o custo de executar um passo naquela direcao.
		float maze_HeuristicaDistEuclidiana(const char *direcao);
			- Retorna o a distancia heuclidiana da celula que estah naquela direcao ateh a saida.
	
	Constantes que podem ser usadas pelo jogador (agente):
		#define NUMCAMINHOS 4
		const char *id_Caminhos[NUMCAMINHOS] = {"norte", "sul", "oeste", "leste"};
		const char *id_Retornos[NUMCAMINHOS] = {"sul", "norte", "leste", "oeste"};
		typedef struct str_PosicaoPlano {
			int x,y;
		} tipo_PosicaoPlano;
*/

// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 1 ***
//	Implementacao da primeira estrategia de jogo.

#include <vector>
#include <float.h>
using namespace std;


/* --------------------------------ESTRATÉGIAS-----------------------------------------------------------

Agente 1 - Com uma matriz do tamanho do labirinto, mapeia os caminhos por onde passou
e através da estratégia de busca em profundidade, vai empilhando o caminho por onde percorreu
até encontrar um beco sem saída. Caso chegue nesse ponto, vai desempilhando até chegar em uma bifurcação
e segue pelo caminho que ele ainda nao tenha passado, voltando a empilhar seus passos.

Agente 2 - Com uma matriz do tamanho do labirinto, também mapeia por onde passou, porém acrescentando sempre
uma unidade por onde passa. Dessa forma ele busca sempre se mover para onde o peso (quantidade de passos dados)
do caminho disponível é menor, percorrendo então todo mapa

--------------------------------------------------------------------------------------------------------*/
#define passosParaFuga 70;

//Variaveis globais usadas no agente 1
tipo_PosicaoPlano coordPlayer1;
int trilhaLabirinto1[60][35];
vector<int> pilhaPlayer1;
vector<int> caminhosPossiveisP1;
int fugaP1=0, passosFugaP1=passosParaFuga;

//Variaveis globais usadas no agente 2
tipo_PosicaoPlano coordPlayer2;
int trilhaLabirinto2[60][35];
vector<int> pilhaPlayer2;
vector<int> caminhosPossiveisP2;
int fugaP2=0, passosFugaP2=passosParaFuga;

//Função que calcula a distancia de manhattan em relação ao labirinto
float tLab_HeuristicaDistManhattan(int x, int y) {
	float distX = 0.0, distY = 0.0, melhorDist = 2.0 * MAXCEL;
	
	for (int i=0; i<count_VetSaidas; i++) {
		// Calcula a distancia de Manhattan no eixo X.
		if (x < vet_Saidas[i].x) distX = vet_Saidas[i].x-x;
		else distX = x-vet_Saidas[i].x;

		// Calcula a distancia de Manhattan no eixo X.
		if (y < vet_Saidas[i].y) distY = vet_Saidas[i].y-y;
		else distY = y-vet_Saidas[i].y;
		
		// Atualize se for a menor distancia encontrada.
		if ((i == 0) || (distX+distY < melhorDist)) {
			melhorDist = distX+distY;
		}
	}
	
	agentes[agenteAtual].custoCalc++;
	return melhorDist;
}

//Função que calcula a distância de manhattam em relação ao caminho que o player deseja seguir

float maze_HeuristicaDistManhattan(const char *direcao) {
	float dist = 0.0;
	
	if (!strcmp(direcao, "norte")) {
		if (agenteAtual == 1)
			dist = tLab_HeuristicaDistManhattan(posAtualP1.x, posAtualP1.y-1);
		else
			dist = tLab_HeuristicaDistManhattan(posAtualP2.x, posAtualP2.y-1);
	}
	else if (!strcmp(direcao, "sul")) {
		if(agenteAtual == 1)
			dist = tLab_HeuristicaDistManhattan(posAtualP1.x, posAtualP1.y+1);
		else
			dist = tLab_HeuristicaDistManhattan(posAtualP2.x, posAtualP2.y+1);
	}
	else if (!strcmp(direcao, "oeste")) {
		if(agenteAtual == 1)
			dist = tLab_HeuristicaDistManhattan(posAtualP1.x-1, posAtualP1.y);
		else
			dist = tLab_HeuristicaDistManhattan(posAtualP2.x-1, posAtualP2.y);
	}
	else if (!strcmp(direcao, "leste")) {
		if(agenteAtual == 1)
			dist = tLab_HeuristicaDistManhattan(posAtualP1.x+1, posAtualP1.y);
		else
			dist = tLab_HeuristicaDistManhattan(posAtualP2.x+1, posAtualP2.y);
	}
	
	return dist;
}

//Funcao que verifica quais caminhos disponiveis ainda nao foram visitados
int verificaCaminhosEuclidiana(){
	int direcao;
	float funcaoEuclid, disEuclid = 2.0 *MAXCEL; //70 é o maior valor aproximado de distancia que existe no mapa usando teorema de pitagoras (a²=b²+c²)
	int movimento = 4;
	while (caminhosPossiveisP1.size() > 0){
		direcao = caminhosPossiveisP1.back();
		caminhosPossiveisP1.pop_back();
		switch(direcao){
			case 0:	//pode ir para o Norte
				if(trilhaLabirinto1[coordPlayer1.x][coordPlayer1.y-1]==0){
					funcaoEuclid = maze_HeuristicaDistEuclidiana(id_Caminhos[direcao]);
					if(funcaoEuclid < disEuclid){
						movimento=direcao;
						disEuclid=funcaoEuclid;
					}
				}
				break;
			case 1:	//pode ir para o Sul
				if(trilhaLabirinto1[coordPlayer1.x][coordPlayer1.y+1]==0){
					funcaoEuclid = maze_HeuristicaDistEuclidiana(id_Caminhos[direcao]);
					if(funcaoEuclid < disEuclid){
						movimento=direcao;
						disEuclid=funcaoEuclid;
					}
				}
				break;
			case 2:	//pode ir para o oeste
				if(trilhaLabirinto1[coordPlayer1.x-1][coordPlayer1.y]==0){
					funcaoEuclid = maze_HeuristicaDistEuclidiana(id_Caminhos[direcao]);
					if(funcaoEuclid < disEuclid){
						movimento=direcao;
						disEuclid=funcaoEuclid;
					}
				}
				break;
			case 3:	//pode ir para o leste
				if(trilhaLabirinto1[coordPlayer1.x+1][coordPlayer1.y]==0){
					funcaoEuclid = maze_HeuristicaDistEuclidiana(id_Caminhos[direcao]);
					if(funcaoEuclid < disEuclid){
						movimento=direcao;
						disEuclid=funcaoEuclid;
					}
				}
				break;
			default:
				break;
		}
		
	}
	return movimento;	//Caso nao exista caminhos possiveis que ainda nao foram visitados, é retornado 4
}

int verificaCaminhosManhattan(){
	int direcao;
	float funcaoManhattan, disManhattan = 2.0 *MAXCEL; //70 é o maior valor aproximado de distancia que existe no mapa usando teorema de pitagoras (a²=b²+c²)
	int movimento = 4;
	while (caminhosPossiveisP2.size() > 0){
		direcao = caminhosPossiveisP2.back();
		caminhosPossiveisP2.pop_back();
		switch(direcao){
			case 0:	//pode ir para o Norte
				if(trilhaLabirinto2[coordPlayer2.x][coordPlayer2.y-1]==0){
					funcaoManhattan = maze_HeuristicaDistManhattan(id_Caminhos[direcao]);
					if(funcaoManhattan < disManhattan){
						movimento=direcao;
						disManhattan=funcaoManhattan;
					}
				}
				break;
			case 1:	//pode ir para o Sul
				if(trilhaLabirinto2[coordPlayer2.x][coordPlayer2.y+1]==0){
					funcaoManhattan = maze_HeuristicaDistManhattan(id_Caminhos[direcao]);
					if(funcaoManhattan < disManhattan){
						movimento=direcao;
						disManhattan=funcaoManhattan;
					}
				}
				break;
			case 2:	//pode ir para o oeste
				if(trilhaLabirinto2[coordPlayer2.x-1][coordPlayer2.y]==0){
					funcaoManhattan = maze_HeuristicaDistManhattan(id_Caminhos[direcao]);
					if(funcaoManhattan < disManhattan){
						movimento=direcao;
						disManhattan=funcaoManhattan;
					}
				}
				break;
			case 3:	//pode ir para o leste
				if(trilhaLabirinto2[coordPlayer2.x+1][coordPlayer2.y]==0){
					funcaoManhattan = maze_HeuristicaDistManhattan(id_Caminhos[direcao]);
					if(funcaoManhattan < disManhattan){
						movimento=direcao;
						disManhattan=funcaoManhattan;
					}
				}
				break;
			default:
				break;
		}
		
	}
	return movimento;	//Caso nao exista caminhos possiveis que ainda nao foram visitados, é retornado 4
}

void init_Player1() {
	//inicia a matriz de posições do player 1 com todas as posições sendo 0
	for(int x=0; x<60; x++){
		for(int y=0; y<35; y++){
			trilhaLabirinto1[x][y]=0;
		}
	}

	//limpando os vetores
	pilhaPlayer1.clear();
	caminhosPossiveisP1.clear();
	
	//Setando coordenadas iniciais como (0,0)
	coordPlayer1.x = 0;
	coordPlayer1.y = 0;
	
}
const char *run_Player1() {

	//Seta posicao atual como 1
	trilhaLabirinto1[coordPlayer1.x][coordPlayer1.y] = 1;

	//Verifica se o player 1 está em fuga
	if(fugaP1==1){
		while(passosFugaP1>0 && pilhaPlayer1.size()>0){
			trilhaLabirinto1[coordPlayer1.x][coordPlayer1.y] = 0;
			passosFugaP1--;
			int voltar = pilhaPlayer1.back();
			switch(voltar){
				case 0:	//pode ir para o Norte
					coordPlayer1.y++;
					break;
				case 1:	//pode ir para o Sul
					coordPlayer1.y--;
					break;
				case 2:	//pode ir para o oeste
					coordPlayer1.x++;
					break;
				case 3:	//pode ir para o leste
					coordPlayer1.x--;
					break;
				default:
					break;
			}
			
			//desempilha o id da direção
			pilhaPlayer1.pop_back(); 

			//Verifica se é o ultimo passo da fuga e altera para voltar ao normal
			if(passosFugaP1==0 || pilhaPlayer1.size()==0){
				passosFugaP1=passosParaFuga;
				fugaP1=0;
				printf("<PLAYER 1> RETORNANDO AO CURSO NORMAL...\n");
			}
			
			return id_Retornos[voltar];
		}
	}
	
	//Verificação dos caminhos possiveis
	for(int i=0; i<NUMCAMINHOS; i++){
		if(maze_VerCaminho(id_Caminhos[i])==CAMINHO){
			caminhosPossiveisP1.push_back(i);
		}
	}

	//funcao que verifica se existe um caminho possivel que ainda nao foi visitado e retorna a melhor alternativa
	int andar = verificaCaminhosEuclidiana();
	caminhosPossiveisP1.clear();

	//Verifica se o minotauro esta naquela direção
	if(maze_VerMinotauro(id_Caminhos[andar])){
		printf("<PLAYER 1> INICIANDO FUGA...\n");
		fugaP1=1;
		trilhaLabirinto1[coordPlayer1.x][coordPlayer1.y] = 0;
		int voltar = pilhaPlayer1.back();
		switch(voltar){
			case 0:	//pode ir para o Norte
				coordPlayer1.y++;
				break;
			case 1:	//pode ir para o Sul
				coordPlayer1.y--;
				break;
			case 2:	//pode ir para o oeste
				coordPlayer1.x++;
				break;
			case 3:	//pode ir para o leste
				coordPlayer1.x--;
				break;
			default:
				break;
		}
		pilhaPlayer1.pop_back(); //desempilha o id da direção 
		return id_Retornos[voltar];
	}

	if(andar!=4){	//Se tem caminho disponivel empilha e altera a posição do P1
		switch(andar){
			case 0:	//pode ir para o Norte
				coordPlayer1.y--;
				break;
			case 1:	//pode ir para o Sul
				coordPlayer1.y++;
				break;
			case 2:	//pode ir para o oeste
				coordPlayer1.x--;
				break;
			case 3:	//pode ir para o leste
				coordPlayer1.x++;
				break;
			default:
				break;
		}
		//printf("Incrementando\n");
		pilhaPlayer1.push_back(andar);	//empilha o id da direção
		return id_Caminhos[andar];
		
	}else{	//Se nao tem desempilha e altera a posição do P1
		int voltar = pilhaPlayer1.back();
		switch(voltar){
			case 0:	//pode ir para o Norte
				coordPlayer1.y++;
				break;
			case 1:	//pode ir para o Sul
				coordPlayer1.y--;
				break;
			case 2:	//pode ir para o oeste
				coordPlayer1.x++;
				break;
			case 3:	//pode ir para o leste
				coordPlayer1.x--;
				break;
			default:
				break;
		}
		pilhaPlayer1.pop_back(); //desempilha o id da direção 
		return id_Retornos[voltar];
	}
}

// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 2 ***
//	Implementacao da segunda estrategia de jogo.
void init_Player2() {
	//inicia a matriz de posições do player 2 com todas as posições sendo 0
	for(int x=0; x<60; x++){
		for(int y=0; y<35; y++){
			trilhaLabirinto2[x][y]=0;
		}
	}

	//limpando os vetores
	pilhaPlayer2.clear();
	caminhosPossiveisP2.clear();
	
	//Setando coordenadas iniciais como (0,0)
	coordPlayer2.x = 0;
	coordPlayer2.y = 0;
}
const char *run_Player2() {
	
	//Seta posicao atual como 1
	trilhaLabirinto2[coordPlayer2.x][coordPlayer2.y] = 1;

	//Verifica se o player 1 está em fuga
	if(fugaP2==1){
		while(passosFugaP2>0 && pilhaPlayer2.size()>0){
			trilhaLabirinto2[coordPlayer2.x][coordPlayer2.y] = 0;
			passosFugaP2--;
			int voltar = pilhaPlayer2.back();
			switch(voltar){
				case 0:	//pode ir para o Norte
					coordPlayer2.y++;
					break;
				case 1:	//pode ir para o Sul
					coordPlayer2.y--;
					break;
				case 2:	//pode ir para o oeste
					coordPlayer2.x++;
					break;
				case 3:	//pode ir para o leste
					coordPlayer2.x--;
					break;
				default:
					break;
			}
			
			//desempilha o id da direção
			pilhaPlayer2.pop_back(); 

			//Verifica se é o ultimo passo da fuga e altera para voltar ao normal
			if(passosFugaP2==0 || pilhaPlayer2.size()==0){
				passosFugaP2=passosParaFuga;
				fugaP2=0;
				printf("<PLAYER 2> RETORNANDO AO CURSO NORMAL...\n");
			}
			
			return id_Retornos[voltar];
		}
	}
	
	//Verificação dos caminhos possiveis
	for(int i=0; i<NUMCAMINHOS; i++){
		if(maze_VerCaminho(id_Caminhos[i])==CAMINHO){
			caminhosPossiveisP2.push_back(i);
		}
	}

	//funcao que verifica se existe um caminho possivel que ainda nao foi visitado e retorna a melhor alternativa
	int andar = verificaCaminhosManhattan();
	caminhosPossiveisP2.clear();

	//Verifica se o minotauro esta naquela direção
	if(maze_VerMinotauro(id_Caminhos[andar])){
		printf("<PLAYER 2> INICIANDO FUGA...\n");
		fugaP2=1;
		trilhaLabirinto2[coordPlayer2.x][coordPlayer2.y] = 0;
		int voltar = pilhaPlayer2.back();
		switch(voltar){
			case 0:	//pode ir para o Norte
				coordPlayer2.y++;
				break;
			case 1:	//pode ir para o Sul
				coordPlayer2.y--;
				break;
			case 2:	//pode ir para o oeste
				coordPlayer2.x++;
				break;
			case 3:	//pode ir para o leste
				coordPlayer2.x--;
				break;
			default:
				break;
		}
		pilhaPlayer2.pop_back(); //desempilha o id da direção 
		return id_Retornos[voltar];
	}

	if(andar!=4){	//Se tem caminho disponivel empilha e altera a posição do P1
		switch(andar){
			case 0:	//pode ir para o Norte
				coordPlayer2.y--;
				break;
			case 1:	//pode ir para o Sul
				coordPlayer2.y++;
				break;
			case 2:	//pode ir para o oeste
				coordPlayer2.x--;
				break;
			case 3:	//pode ir para o leste
				coordPlayer2.x++;
				break;
			default:
				break;
		}
		//printf("Incrementando\n");
		pilhaPlayer2.push_back(andar);	//empilha o id da direção
		return id_Caminhos[andar];
		
	}else{	//Se nao tem desempilha e altera a posição do P1
		int voltar = pilhaPlayer2.back();
		switch(voltar){
			case 0:	//pode ir para o Norte
				coordPlayer2.y++;
				break;
			case 1:	//pode ir para o Sul
				coordPlayer2.y--;
				break;
			case 2:	//pode ir para o oeste
				coordPlayer2.x++;
				break;
			case 3:	//pode ir para o leste
				coordPlayer2.x--;
				break;
			default:
				break;
		}
		pilhaPlayer2.pop_back(); //desempilha o id da direção 
		return id_Retornos[voltar];
	}
}