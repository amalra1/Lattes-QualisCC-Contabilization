#ifndef __LIBESCRITA__
#define __LIBESCRITA__

/* 
    Biblioteca feita para funcoes especificas de impressao de dados
    especificos coletados dos pesquisadores de curriculos.xml

    Feita por Pedro Amaral Chapelin
    Data de finalizacao -> XX/XX/XXX
*/

// Funcao que imprime um vetor de strings
void imprime_vetor(char** v, int tamv);

// Funcao que imprime os periodicos catalogados de acordo com seus niveis
// e tambem quantas vezes eles apareceram no arquivo do curriculo
void imprimeSumarizadaPER(pesquisador_t **vp, int tamvp);

// Funcao que imprime as conferencias catalogadas de acordo com seus niveis
// e tambem quantas vezes elas apareceram no arquivo do curriculo
void imprimeSumarizadaCONF(pesquisador_t **vp, int tamvp);

// Funcao que imprime a quantidade periodicos e conferencias de cada autor
// discrimanados pelos niveis
void imprimeSumarizadaAutoria(pesquisador_t *p);

// Funcao que imprime todos os titulos catalogados no nivel C
void imprime_tudoC(pesquisador_t **vp, int tamvp);

// Funcao que imprime todos os titulos que nao estavam nas listas de
// classificacao
void imprime_NaoClassificados(pesquisador_t **vp, int tamvp);

// Funcao que imprime a quantidade de periodicos e conferencias
// catalogadas por anos de publicacao
void imprimeSumarizadaAno(pesquisador_t **vp, int tamvp);

#endif
