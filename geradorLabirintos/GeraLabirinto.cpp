#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fstream>
#include "globais.h"

/*
@autor: Welber Wendler
Objetivo: Programa que gera labirintos baseados no tamanho informado pelo usuário.
AINDA DEVE SER FEITO: Escrever DO LABIRINTO NO ARQUIVO.
*/


/*
Estrutura Labirinto:
    Campos: int **mat: Matriz de inteiro que guarda informacoes de licação de uma celula
            int tx, ty: Quantidade de linhas tx, e colunas ty
*/
typedef struct{
    int** mat;
    int tx, ty;
}Labirinto;

Labirinto * geraLabirinto(int tx, int ty);
void geraLabirintoRecursivo(Labirinto * lab, int px, int py);
int celulasVisinhasIntactas(Labirinto * lab, int px, int py);
bool gravaLabirinto(Labirinto * lab);
bool celulaIntecta(int cel);
bool imprimeLab(Labirinto * lab, char path[MAXLINE]);
void sorteiaSaida(Labirinto * lab);

/*
FUNCAO PRINCIPAL
Objetivo: Gerar um novo labirinto e retornar um ponteiro para a nova estrutura
Entrada:    tx = linhas do labirinto
            ty = Colunas do lairinto
Saida:      Ponteiro para a estrutura gerada.
*/

Labirinto * geraLabirinto(int tx, int ty){

    Labirinto * lab = new Labirinto;    //Aloca e inicializa estrutura Labirinto
    lab->tx = tx;
	lab->ty = ty;
    int **mat = new int*[tx];
    for(int i=0; i<tx; i++){
        mat[i] = new int[ty];
        for(int j=0; j<ty; j++){
            mat[i][j] = 0;
        }
    }

    lab->mat = mat;
    srand (time (NULL));

	geraLabirintoRecursivo(lab, 0, 0);  //Funcao faz a ligacao do labirinto
    //Sorteia a saída
    sorteiaSaida(lab);
    return lab;
}
/*
Funcao secundaria
Objetivo: Fazer a construção aleatoria de caminhos em uma labirinto
Entrada: lab: Estrutura Labirinto ja adequadamente alocada
         px: posicao linha atual
         py: posicao coluna atual
Efeito colateral: criacao das ligacoes do labirinto
**/
void geraLabirintoRecursivo(Labirinto * lab, int px, int py){

    const unsigned int mask_Norte = 1;	// same as binary 0001
	const unsigned int mask_Sul = 2;	// same as binary 0010
	const unsigned int mask_Oeste = 4;	// same as binary 0100
	const unsigned int mask_Leste = 8;	// same as binary 1000

    int candidata = celulasVisinhasIntactas(lab, px, py);   //Quais sao as celuas visinhas noa visitadas
    int prox;                       //armazena a proxima celula a ser visitada
    while(candidata>0){             //Enquanto houver celulas visinhas a serem visitadas
        do{                         //Escolhe uma celula visinha aleatoria
           prox = 1;
           prox = prox << rand()%4; //Move bit a bit
        }while(!(prox & candidata));

        if(prox == mask_Norte){  //Se a proxima celula for para Norte
            lab->mat[px][py]+= mask_Norte;   //faz a ligacao das celulas
            lab->mat[px-1][py]+= mask_Sul;
            geraLabirintoRecursivo(lab,px-1,py);    //Chama o proximo passo recursivo para celula a Norte

        }else if(prox == mask_Sul){
            lab->mat[px][py]+= mask_Sul;
            lab->mat[px+1][py]+= mask_Norte;
            geraLabirintoRecursivo(lab,px+1,py);

        }else if(prox == mask_Oeste){
            lab->mat[px][py]+= mask_Oeste;
            lab->mat[px][py-1]+= mask_Leste;
            geraLabirintoRecursivo(lab,px,py-1);

        }else if(prox == mask_Leste){
            lab->mat[px][py]+= mask_Leste;
            lab->mat[px][py+1]+= mask_Oeste;
            geraLabirintoRecursivo(lab,px,py+1);
        }
        candidata = celulasVisinhasIntactas(lab, px, py); //Procura visinhos que ainda não foram visitados
    }
}

/*
Funcao secundaria
Objetivo: verificar as celulas adjacentes a celula[px][py] que ainda não foram visitadas
Entrada: lab: Estrutura Labirinto
         px: linha atual
         py: Coluna atual
Saida: inteiro correspondente as celulas adjacentes que ainda nao foram visitadas.
**/
int celulasVisinhasIntactas(Labirinto * lab, int px, int py){

    int candidata = 0;

    const unsigned int mask_Norte = 1;	// same as binary 0001
	const unsigned int mask_Sul = 2;	// same as binary 0010
	const unsigned int mask_Oeste = 4;	// same as binary 0100
	const unsigned int mask_Leste = 8;	// same as binary 1000

	if(px > 0 && celulaIntecta(lab->mat[px-1][py])){    //Se a celula nao esta na primeira linha E a celula de Norte nao foi visitada
        candidata += mask_Norte;                         //Adiciona celula de Norte como candidata
	}
	if(px < lab->tx-1 && celulaIntecta(lab->mat[px+1][py])){
        candidata += mask_Sul;
	}
	if(py > 0 && celulaIntecta(lab->mat[px][py-1])){
        candidata += mask_Oeste;
	}
    if(py < lab->ty-1 && celulaIntecta(lab->mat[px][py+1])){
        candidata += mask_Leste;
    }
    return candidata; //Retorna candidata
}

/*
Funcao secundaria
Objetivo: verificar se a celula nao foi visitada.
Entrada: cel: valor da celula
Saida:  true: Celula ainda nao visitada
        false: Celula ja visitada
*/

bool celulaIntecta(int cel){
    if(cel == 0){
        return true;
    }else{
        return false;
    }
}
/*
Funcao extra
Objetivo: criar uma saida aleatoria para o labirinto na parte inferior Leste do labirinto
Entrada: lab: Estrutura labirinto
Efeito colateral: Uma celula ira ter uma ligacao para fora do mapa
*/
void sorteiaSaida(Labirinto * lab){

    const unsigned int mask_Sul = 2;	// same as binary 0010
	const unsigned int mask_Leste = 8;	// same as binary 1000

    int x,y,sort = rand()%2;    //Escolhe a saida pela Leste ou por Sul
    if(sort){
        x = lab->tx-1;
        y = lab->ty/2 + rand()%lab->ty/2;   //Escolhe uma coluna do meio para o fim para ser a saida do labirinto
        lab->mat[x][y] += mask_Sul;
    }else{
        x = lab->tx/2 + rand()%lab->tx/2;
        y = lab->ty-1;
        lab->mat[x][y] += mask_Leste;
    }

}
/*
Funcao de impressao do labirinto.
Objetivo: Imprimir o labirinto em um arquivo.
Entrada: lab: Estrutura labirinto e caminho/nome do arquivo de saida com labirinto.
Efeito colateral: Uma celula ira ter uma ligacao para fora do mapa
*/
bool imprimeLab(Labirinto * lab, char path[MAXLINE]){
	FILE *fp;
	
	// Cria arquivo para armazenar o labirinto.
	if ((fp = fopen(path,"w")) == NULL) {
		printf("ERRO: ARQUIVO PARA IMPRESSAO DO MAPA NAO PODE SER CRIADO\n");
		exit(1);
	}
	
	fprintf(fp, "c %i %i\n", lab->tx, lab->ty);
	for(int i=0; i< lab->tx; i++){
		fprintf(fp, "%i", lab->mat[i][0]);
		for(int j=1; j<lab->ty; j++){
			// Verificar informaceo na celula e armazenar codigo no arquivo do mapa.
			fprintf(fp, "%s%i", DELIM, lab->mat[i][j]);
		}
		fprintf(fp,"\n");
	}
	
	fclose(fp);
	
	return true;
}

/*
Objetivo: Destruir a estrutura do labirinto
*/
void destroiLabirinto(Labirinto * lab){
    for(int i=0; i<lab->tx; i++){
        delete [] lab->mat[i];
    }
    delete [] lab->mat;
    delete lab;
    lab = NULL;
}

int main(int numParametros, char *arg_V[100]) {
	char path[MAXLINE];
	if (numParametros != 4) {
		printf("ERRO: ESPECIFIQUE OS PARAMETROS DE CONFIGURACAO!\n");
		printf("Parametros:\n");
		printf("\t1 - Linhas;\n\t2 - Colunas;\n");
		printf("\t3 - Arquivo de saida com mapa.\n");
		exit(1);
	}
	int x = atoi(arg_V[1]);
	int y = atoi(arg_V[2]);
	strncpy(path, arg_V[3], MAXLINE);
	
	Labirinto * lab = geraLabirinto(x,y);

	imprimeLab(lab, path);
	
	destroiLabirinto(lab);
	
	return 0;
}




