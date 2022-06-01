#ifndef TADMINOTAURO_H
#define TADMINOTAURO_H

/* TAD MINOTAURO
	Objetivo: TAD contendo metodos do minotauro no labirinto.
*/
#define DIST_PERTO 2.0
#define DIST_VISIVEL 6.0

typedef struct str_VarTadMinot {
	int count;
	bool flag_Perseguir;
} tipo_VarTadMinot;

tipo_VarTadMinot varTadMinot;

void init_Minotauro();
const char *run_Minotauro();

const char *tadMinotauro_BuscaEmProfundidade();
const char *tadMinotauro_PerseguicaoGulosa();
float tadMinotauro_DistEuclidianaParaAgentes(const char *direcao);
bool tadMinotauro_VerificaJogadorPerto();
bool tadMinotauro_VerificaJogadorVisivel();

#endif // TADMINOTAURO_H

