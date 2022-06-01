/*
	Autor: Luis Otavio Rigo Junior
	Objetivo: 
		Programa Principal do labirinto.

*/
#include "maze.cpp"
#include "userEstrategia.cpp"

// ------------------------------------------------------------------------
//	MAIN - PROGRAMA PRINCIPAL
// ------------------------------------------------------------------------
// Controle do jogo de labirinto.
// E manipulacao do jogo para 2 jogadores.
int main(int argc, char **argv) {
	char *path = argv[1];
	
	srand(time(NULL));
	
	cria_LabirintoGrafico(path);
	
	// Gatilho de Inicializacao dos Players.
	init_Player1();
	init_Player2();
	init_Minotauro();
	
	glutInit(&argc, argv);

	//Setting up  The Display
	//-RGB color model + Alpha Channel = GLUT_RGBA
	//
	glutInitDisplayMode(GLUT_RGBA|GLUT_SINGLE);

	//Configure Window Postion
	glutInitWindowPosition(50, 25);

	//Configure Window Size
	glutInitWindowSize(width,height);

	//Create Window
	glutCreateWindow("Labirinto I.A. 2017-04");
	
	//Call to the drawing function
	glutDisplayFunc(display);
//	glutKeyboardFunc(wasd);
//	glutSpecialFunc(setas);
//	glutIdleFunc(display);
	
	// Gatilho de Execucao dos Players.
	glutTimerFunc(100, run_Players, 0);
	
	
	// Loop require by OpenGL
	glutMainLoop();
	
	// Libera memoria alocada para o labirinto.
	tLab_ApagaLabirinto();
	
	return true;
}


