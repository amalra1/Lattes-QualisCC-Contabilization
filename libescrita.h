#ifndef __LIBESCRITA__
#define __LIBESCRITA__

/* 
    Biblioteca feita para funcoes especificas de impressao de dados
    especificos coletados de um curriculoXML

    Feita por Pedro Amaral Chapelin
    Data de finalizacao -> XX/XX/XXX
*/

// Funcao que imprime um vetor de strings
void imprime_vetor(char** v, int tamv);

// Funcao que imprime os titulos passados de acordo com seus niveis
void imprimeCatalogados(char** v, int tamv);

// Funcao que imprime os titulos catalogados de acordo com seus niveis
// e tambem quantas vezes eles apareceram no arquivo do curriculo
void imprimeSumarizada(char** v, int tam);

// Funcao que imprime todos os titulos catalogados no nivel C
void imprime_tudoC(char** v, int tamv);

// Funcao que imprime todos os titulos 
// que nao estavam nas listas de classificacao
void imprime_NaoClassificados(char** v, int tam);

#endif
