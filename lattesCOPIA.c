#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include "libcoleta.h"
#include "libescrita.h"
#define ARQUIVO "curriculoZIVIANI.xml"
#define TAM_PESQUISADOR 64
#define PER_MAX 600
#define CONF_MAX 600
#define DIST_MIN_PER 0.145  // Distancias relativas minimas relativas aos nomes de periodicos e conferencias
#define DIST_MIN_CONF 0.21  // da funcao de distancia de edicao (ver SepararSelecionados -> libcoleta.c)

/*
    Contabilização da produção científica em termos de
    artigos científicos publicados em periódicos e 
    conferências de um grupo de pesquisadores por meio
    da análise de seus currículos Lattes.
    
    Desenvolvido por Pedro Amaral Chapelin
    Data de finalização -> XX/XX/XXXX

    utilizei a funcao de distancia de edicao do algoritmo de Levenshtein,
    peguei a implementacao do link: https://github.com/wooorm/levenshtein.c
    

    os nao presentes na lista marquei como 'C-'
    
    //////////////
    LIDAR COM CARACTERES ESPECIAIS QUE NAO FUNCIONAM NA FUNCAO TOUPPER (Ç, ^, ~ ETC)
    //////////////

    // 98,473 allocs, 98,473 frees, 37,974,414 bytes allocated - Menotti

*/

// Funcao que imprime a quantidade de periodicos na tela, separados por niveis
void pegaDados(FILE* arqXML, FILE* arqPER, FILE* arqCONF)
{
    int tamvPER = 0, tamvCONF = 0, tamvANOper = 0, tamvANOconf = 0, i;
    char** vPER = malloc(sizeof(char*) * PER_MAX);  // Aloca vetor de strings para X titulos de periodicos
    int* vANOper = malloc(sizeof(int) * PER_MAX);  // Aloca vetor de strings para X anos de periodicos
    char** vCONF = malloc(sizeof(char*) * CONF_MAX);  // Aloca vetor de strings para X titulos de conferencias
    int* vANOconf = malloc(sizeof(int) * CONF_MAX);  // Aloca vetor de strings para X anos de conferencias
    //char *pesquisador = malloc(sizeof(char) * 64); // String para armazenar o nome do pesquisador
    char pesquisador[TAM_PESQUISADOR];

    // Inicializa a string 'pesquisador'
    strcpy(pesquisador, "");

    nomePesquisador(arqXML, pesquisador);

    coletarTitulos2(arqXML, vPER, &tamvPER, vCONF, &tamvCONF, vANOper, &tamvANOper, vANOconf, &tamvANOconf);

    //imprime_vetor(vPER, tamvPER);

    //for (i = 0; i < tamvANOper; i++)
        //printf("%d\n", vANOper[i]);

    //imprime_vetor(vCONF, tamvCONF);

    //for (i = 0; i < tamvANOconf; i++)
        //printf("%d\n", vANOconf[i]);

    
    // Nao eh necessario passar o nome das conferencias para maiusculo
    // pois no arquivo delas estao todas normais
    corrigirNomes(vPER, tamvPER, "per");
    corrigirNomes(vCONF, tamvCONF, "conf");

    //imprime_vetor(vPER, tamvPER);

    //imprime_vetor(vCONF, tamvCONF);

    //for (i = 0; i < tamvANO; i++)
        //printf("%d\n", vANO[i]);

    //printf("\n\n\n\n");

    //separarSelecionados(arqPER, vPER, tamvPER);

    //separarSelecionados(arqCONF, vCONF, tamvCONF);

    separarSelecionadosDIST(arqCONF, vCONF, tamvCONF, DIST_MIN_CONF);

    separarSelecionadosDIST(arqPER, vPER, tamvPER, DIST_MIN_PER);

    //imprime_vetor(vCONF, tamvCONF);

    //imprime_vetor(vPER, tamvPER);

    //printf("\n\n\n\n");

    //imprimeCatalogados(vPER, tamvPER);

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de periodicos=---------------------------\n");

    imprimeSumarizada(vPER, tamvPER); //(1)

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de conferencias=---------------------------\n");

    imprimeSumarizada(vCONF, tamvCONF); //(2)

    printf("\n---------------------------=Producao dos pesquisadores do grupo por ordem de autoria=---------------------------\n\n");

    imprimeSumarizadaAutoria(pesquisador, vPER, tamvPER, vCONF, tamvCONF); //(3)

    printf("\n---------------------------=Producao sumarizada do grupo por ano=---------------------------\n");

    //imprimeSumarizadaAno(vPER, tamvPER, vCONF, tamvCONF, vANOper, tamvANOper, vANOconf, tamvANOconf); //(4)

    //for (i = 0; i < tamvANO; i++)
        //printf("%d\n", vANO[i]);

    printf("\n---------------------------=Todos os periodicos e eventos classificados em nivel C=---------------------------\n\n");

    imprime_tudoC(vPER, tamvPER, vCONF, tamvCONF); //(5)

    printf("\n--------------------------=Todos os periodicos/eventos nao classificados=--------------------------\n\n");

    imprime_NaoClassificados(vPER, tamvPER, vCONF, tamvCONF); //(6)

    // Da free em todos os espacos alocados da string 'vPER'
    for (i = 0; i < tamvPER; i++)
        free(vPER[i]);

    // Da free em todos os espacos alocados da string 'vCONF'
    for (i = 0; i < tamvCONF; i++)
        free(vCONF[i]);

    free(vPER);
    free(vCONF);
    free(vANOper);
    free(vANOconf);
}

int main (int argc, char** argv)
{
    FILE* arqXML, *arqPER, *arqCONF;
    DIR* dir;
    char* nome_arqPER;
    char* nome_arqCONF;
    char* nome_dir; 
    int opt;

    while ((opt = getopt(argc, argv, "d:c:p:")) != -1) 
    {
        switch (opt) 
        {
         case 'd':
            //printf("opcao d tem arg: %s\n", optarg);
            nome_dir = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(nome_dir, optarg);            
            break;

         case 'c':
            //printf("opcao c tem arg: %s\n", optarg);
            nome_arqCONF = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(nome_arqCONF, optarg);
            break;

         case 'p':
            //printf("opcao p tem arg: %s\n", optarg);
            nome_arqPER = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(nome_arqPER, optarg);
            break;
        }
    }

    // Abre o arquivo
    arqXML = fopen(ARQUIVO, "r");

    // Testa se o arquivo abre
    if (arqXML == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    // Abre o arquivo contendo os periodicos classificados
    arqPER = fopen(nome_arqPER, "r");

    // Testa se o arquivo abre
    if (arqPER == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    // Abre o arquivo contendo as conferencias classificadas
    arqCONF = fopen(nome_arqCONF, "r");

    // Testa se o arquivo abre
    if (arqCONF == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }
    
    pegaDados(arqXML, arqPER, arqCONF);

    fclose(arqXML);
    fclose(arqPER);
    fclose(arqCONF);

    free(nome_dir);
    free(nome_arqCONF);
    free(nome_arqPER);
    return 0;
}