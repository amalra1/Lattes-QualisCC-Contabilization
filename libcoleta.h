#ifndef __LIBCOLETA__
#define __LIBCOLETA__

/* 
    Biblioteca feita para funcoes especificas sobre coleta de 
    dados dos curriculos.xml

    Feita por Pedro Amaral Chapelin
    Data de finalizacao -> XX/XX/XXX
*/

// Estrutura de dados contendo todos os dados
// necessarios de cada pesquisador
typedef struct pesquisador {

    char* nome;     // Seu nome

    char** vPER;    // Vetor com o nome dos Periodicos
    int tamvPER;  
    char** vPERorg;  // Vetor para armazenar os nomes sem mudanças

    char** vCONF;   // Vetor com o nome das Conferencias
    int tamvCONF;

    int* vANOper;   // Vetor com o ano de publicacao em cada Periodico
    int tamvANOper;

    int* vANOconf;  // Vetor com o ano de participacao em cada Conferencia
    int tamvANOconf;

} pesquisador_t;

// Funcao que inicializa a struct de cada pesquisador
void inicia_pesquisador(pesquisador_t *p);

// Funcao que libera o espaço alocado para cada pesquisador na struct
void destroi_pesquisador(pesquisador_t *p);

// Funcao que pega o nome do pesquisador do arquivo e armazena na string
void nomePesquisador(FILE* arq, pesquisador_t *p);

// Coleta os titulos com strtok, ao inves de caracter por caracter
void coletarTitulos(FILE* arq, pesquisador_t *p);

// Funcao que corrige  os nomes de alguns titulos, caracteres especiais e etc
// Por exemplo o caracter '&' eh escrito como '&amp;' no xml. Tambem passa os
// nomes dos periodicos para maiusculo, pois no arquivo 'qualis-periodicos.txt'
// estao em maiusculas
void corrigirNomes(pesquisador_t *p);

// Separa as conferencias selecionadas com distancia de edicao
void catalogarCONFS(FILE* arq, pesquisador_t *p, double dist_min);

// Separa os periodicos selecionados com distancia de edicao
void catalogarPERS(FILE* arq, pesquisador_t *p, double dist_min);

#endif
