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
using namespace std;

#define passosParaFuga 50;

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


/*--------------------------------------------------------------ESTRAT??GIAS-----------------------------------------------------------------//

Agentes 1 e 2: Ambos utilizam a mesma estrat??gia. Inicialmente ?? feita uma matriz do tamanho do mapa onde todas as posi????es sao setadas como 0.
Sua posi????o atual ?? setada como 0 e ?? realizada uma busca por possiveis caminhos disponiveis e armazenados em um vetor. Ent??o ?? chamada uma fun????o
para definir qual caminho o player deve seguir. Tal escolha ?? feita baseada na dist??ncia do player at?? a sa??da, caso ele ande nas dire????es 
disponiveis. Para realizar tal decis??o, o player 1 utiliza como escolha gulosa a menor dist??ncia euclidiana e o player 2 utiliza a menor dist??ncia
de Manhattan. Antes de realmente dar um passo na dire????o escolhida, o player verifica se o minotauro est?? nela. Caso ele n??o esteja, o player se 
move para tal dire????o e altera sua posi????o na matriz de posi????es para 1. Caso ele esteja, o player come??a
a desempilhar seus passos anteriores, retornando por onde veio e alterando a posi????o do mapa como 0 novamente, para que quando termine de dar os passos
setados para fugir do minotauro, ele volte novamente a sua busca pela saida.
//------------------------------------------------------------------------------------------------------------------------------------------*/




//-----------------------------------Fun????o respons??vel por calcular a Heur??stica (Dist??ncia de Manhattan)-----------------------------------//



//Fun????o que calcula a distancia de manhattan em rela????o ao labirinto
float tLab_HeuristicaDistManhattan(int x, int y) {
	float distX = 0.0, distY = 0.0, melhorDist = 2.0 * MAXCEL;
	
	for (int i=0; i<count_VetSaidas; i++) {
		distX = abs(x-vet_Saidas[i].x); // Calcula a distancia de Manhattan no eixo X.
		distY = abs(y-vet_Saidas[i].y); // Calcula a distancia de Manhattan no eixo X.
		
		if ((i == 0) || (distX+distY < melhorDist)) { // Atualize se for a menor distancia encontrada.
			melhorDist = distX+distY;
		}
	}
	
	agentes[agenteAtual].custoCalc++;
	return melhorDist;
}


//Fun????o que calcula a dist??ncia de manhattam em rela????o ao caminho que o player deseja seguir
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







//----------------------------------Fun????es que escolhe o caminho que o player vai seguir mediante a escolha gulosa------------------------------------------//


//Fun????o respons??vel por retornar o melhor caminho baseado na escolha gulosa da distancia Euclidiana
int verificaCaminhosEuclidiana(){
	int direcao;
	float funcaoEuclid=0.0, disEuclid = 2.0 *MAXCEL; //valor de disEuclid nunca ser?? menor que 2*MAXCEL
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
	return movimento;	//Caso nao exista caminhos possiveis que ainda nao foram visitados, ?? retornado 4
}


//Fun????o respons??vel por retornar o melhor caminho baseado na escolha gulosa da distancia de Manhattan
int verificaCaminhosManhattan(){
	int direcao;
	float funcaoManhattan=0.0, disManhattan = 2.0 *MAXCEL; //70 ?? o maior valor aproximado de distancia que existe no mapa usando teorema de pitagoras (a??=b??+c??)
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
	return movimento;	//Caso nao exista caminhos possiveis que ainda nao foram visitados, ?? retornado 4
}







//---------------------------------------Fun????es fugir, avan??ar e retroceder----------------------------------------//

//Fun????o respons??vel por fazer com que o player fuja do minotauro
int fugir(int player){
	int voltar;
	if(player==1){ //Para o player 1
		fugaP1=1;
		while(passosFugaP1>0 && pilhaPlayer1.size()>0){
			trilhaLabirinto1[coordPlayer1.x][coordPlayer1.y] = 0;
			passosFugaP1--;
			voltar = pilhaPlayer1.back();
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

			pilhaPlayer1.pop_back();  //desempilha o id da dire????o

			if(passosFugaP1==0 || pilhaPlayer1.size()==0){ //Verifica se ?? o ultimo passo da fuga e altera para voltar ao normal
				passosFugaP1=passosParaFuga;
				fugaP1=0;
				printf("<PLAYER 1> RETORNANDO AO CURSO NORMAL...\n");
			}
		}
	}else{ //Para o player 2
		fugaP2=2;
		while(passosFugaP2>0 && pilhaPlayer2.size()>0){
			trilhaLabirinto2[coordPlayer2.x][coordPlayer2.y] = 0;
			passosFugaP2--;
			voltar = pilhaPlayer2.back();
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
			
			pilhaPlayer2.pop_back(); //desempilha o id da dire????o

			if(passosFugaP2==0 || pilhaPlayer2.size()==0){ //Verifica se ?? o ultimo passo da fuga e altera para voltar ao normal
				passosFugaP2=passosParaFuga;
				fugaP2=0;
				printf("<PLAYER 2> RETORNANDO AO CURSO NORMAL...\n");
			}
		}
	}
	return voltar;	
}

//Fun????o responsavel por avan??ar o player no mapa
int avancar(int player, int andar){
	if(player==1){
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
		pilhaPlayer1.push_back(andar);	//empilha o id da dire????o
	}else{
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
		pilhaPlayer2.push_back(andar);	//empilha o id da dire????o
	}
	return andar;
}


//Fun????o responsavel por fazer o player retroceder por onde veio
int retroceder(int player){
	int voltar;
	if(player==1){
		voltar = pilhaPlayer1.back();
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
		pilhaPlayer1.pop_back(); //desempilha o id da dire????o
	}else{
		voltar = pilhaPlayer2.back();
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
		pilhaPlayer2.pop_back(); //desempilha o id da dire????o 
	}
	return voltar;
}









//--------------------------------------Fun????es referentes aos players ----------------------------------------------//



void init_Player1() {
	
	for(int x=0; x<60; x++){ //inicia a matriz de posi????es do player 1 com todas as posi????es sendo 0
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

	//Verifica se o player 1 est?? em fuga
	if(fugaP1==1){
		return id_Retornos[fugir(1)];
	}
	
	//Verifica????o dos caminhos possiveis
	for(int i=0; i<NUMCAMINHOS; i++){
		if(maze_VerCaminho(id_Caminhos[i])==CAMINHO){
			caminhosPossiveisP1.push_back(i);
		}
	}

	//funcao que verifica se existe um caminho possivel que ainda nao foi visitado e retorna a melhor alternativa
	int andar = verificaCaminhosEuclidiana();
	caminhosPossiveisP1.clear();

	//Verifica se o minotauro esta naquela dire????o
	if(maze_VerMinotauro(id_Caminhos[andar])){
		return id_Retornos[fugir(1)];
	}

	if(andar!=4){	//Se tem caminho disponivel empilha e altera a posi????o do P1
		return id_Caminhos[avancar(1, andar)];
	}else{	//Se nao tem desempilha e altera a posi????o do P1
		return id_Retornos[retroceder(1)];
	}
}

// *** 	FUNCOES DE INICIALIZACAO E EXECUCAO DO JOGADOR 2 ***
//	Implementacao da segunda estrategia de jogo.
void init_Player2() {
	//inicia a matriz de posi????es do player 2 com todas as posi????es sendo 0
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

	//Verifica se o player 1 est?? em fuga
	if(fugaP2==1){
		return id_Retornos[fugir(2)];
	}
	
	//Verifica????o dos caminhos possiveis
	for(int i=0; i<NUMCAMINHOS; i++){
		if(maze_VerCaminho(id_Caminhos[i])==CAMINHO){
			caminhosPossiveisP2.push_back(i);
		}
	}

	//funcao que verifica se existe um caminho possivel que ainda nao foi visitado e retorna a melhor alternativa
	int andar = verificaCaminhosManhattan();
	caminhosPossiveisP2.clear();

	//Verifica se o minotauro esta naquela dire????o
	if(maze_VerMinotauro(id_Caminhos[andar])){
		return id_Retornos[fugir(2)];
	}

	if(andar!=4){	//Se tem caminho disponivel empilha e altera a posi????o do P1
		return id_Caminhos[avancar(2, andar)];
		
	}else{	//Se nao tem desempilha e altera a posi????o do P1
		
		return id_Retornos[retroceder(2)];
	}
}