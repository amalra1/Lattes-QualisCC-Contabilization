#ifndef __LIBCOLETA__
#define __LIBCOLETA__

/* 
    Biblioteca feita para funcoes especificas sobre coleta de 
    dados de um curriculoXML

    Feita por Pedro Amaral Chapelin
    Data de finalizacao -> XX/XX/XXX
*/

// Funcao que pega o nome do pesquisador do arquivo e armazena na string
void nomePesquisador(FILE* arq, char* str);

// Funcao que preenche um vetor de strings com os titulos escolhidos (periodicos/conferencias)
void coletarTitulos(FILE* arq, char** vPER, int *tamvPER, char** vCONF, int *tamvCONF, int* vANOper, int *tamvANOper, int* vANOconf, int *tamvANOconf);

// Coleta os titulos com strtok, ao inves de caracter por caracter
void coletarTitulos2(FILE* arq, char** vPER, int *tamvPER, char** vCONF, int *tamvCONF, int* vANOper, int *tamvANOper, int* vANOconf, int *tamvANOconf);

// Funcao que corrige  os nomes de alguns titulos, caracteres especiais e etc
// Por exemplo o caracter '&' eh escrito como '&amp;'
// Tambem passa os nomes dos periodicos para maiusculo, pois no arquivo
// 'qualis-periodicos.txt' estao em maiusculas
void corrigirNomes(char** v, int tamv, char* chave);

// Funcao que classifica os periodicos de acordo com seus niveis
// niveis: A1, A2, A3, A4, B1, B2, B3, B4, C e C-.
// A classificacao 'C-' eh para aqueles que nao foram encontrados na lista
void separarSelecionados(FILE* arq, char** v, int tamv);

// Separa selecionados com distancia de edicao
void separarSelecionadosDIST(FILE* arq, char** v, int tamv, double dist_min);

#endif
