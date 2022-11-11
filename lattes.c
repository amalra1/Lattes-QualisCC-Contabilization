#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include "libcoleta.h"
#include "libescrita.h"
#define ARQUIVO "curriculoCASTILHO.xml"

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

    FAZER A SAIDA FICAR IGUAL NO SITE

    Fazer um loop que imprimira os nomes de acordo com a ordem dos niveis.
    primeiro a1
    depois a2
    .
    .
    .
    C
    Dentro do loop, fazer outro loop que vai percorrer o vetor geral, um a um nome,
    verificar seu nivel, se for o certo, se ja nao estiver la, copiar seu nome para
    outro vetor, imprimir o nome, e dai contar quantas vezes apareceu no vetor geral.

    
    //////////////
    LIDAR COM CARACTERES ESPECIAIS QUE NAO FUNCIONAM NA FUNCAO TOUPPER (Ç, ^, ~ ETC)
    //////////////

*/

// Funcao que imprime a quantidade de periodicos na tela, separados por niveis
void pegaDados(FILE* arqXML, FILE* arqPER, FILE* arqCONF)
{
    int tamvPER = 0, tamvCONF = 0,  i;
    char** vPER = malloc(sizeof(char*) * 150);  // Aloca vetor de strings para X titulos de periodicos
    char** vCONF = malloc(sizeof(char*) * 512);  // Aloca vetor de strings para X titulos de conferencias
    char *pesquisador = malloc(sizeof(char) * 64); // String para armazenar o nome do pesquisador

    // Inicializa a string 'pesquisador'
    strcpy(pesquisador, "");

    nomePesquisador(arqXML, pesquisador);

    coletarTitulos(arqXML, vPER, &tamvPER, vCONF, &tamvCONF); // PER

    //imprime_vetor(vCONF, tamvCONF);

    //printf("\n\n\n\n");

    // Nao eh necessario passar o nome das conferencias para maiusculo
    // pois no arquivo delas estao todas normais
    corrigirNomes(vPER, tamvPER, "per");

    corrigirNomes(vCONF, tamvCONF, "conf");

    //imprime_vetor(vCONF, tamvCONF);

    printf("\n\n\n\n");

    separarSelecionados(arqPER, vPER, tamvPER);

    //separarSelecionados(arqCONF, vCONF, tamvCONF);

    separarSelecionadosDIST(arqCONF, vCONF, tamvCONF);

    //imprime_vetor(vCONF, tamvCONF);

    //printf("\n\n\nCOM DISTANCIA DE EDICAO:\n\n");

    //imprime_vetor(vPER, tamvPER);

    //printf("\n\n\n\n");

    //imprimeCatalogados(vPER, tamvPER);

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de periodicos=---------------------------\n");

    imprimeSumarizada(vPER, tamvPER); //(1)

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de conferencias=---------------------------\n");

    imprimeSumarizada(vCONF, tamvCONF); //(2)

    printf("\n---------------------------=Producao dos pesquisadores do grupo por ordem de autoria=---------------------------\n\n");

    //imprimeSumarizadaAutoria(pesquisador, vPER, tamvPER, vCONF, tamvCONF); //(3)

    printf("\n---------------------------=Producao sumarizada do grupo por ano=---------------------------\n");

    //imprimeSumarizadaAno(); //(4)

    printf("\n---------------------------=Todos os periodicos e eventos classificados em nivel C=---------------------------\n\n");

    //imprime_tudoC(vPER, tamvPER, vCONF, tamvCONF); //(5)

    printf("\n--------------------------=Todos os periodicos/eventos nao classificados=--------------------------\n\n");

    //imprime_NaoClassificados(vPER, tamvPER, vCONF, tamvCONF); //(6)

    // Da free em todos os espacos alocados da string 'vPER'
    for (i = 0; i < tamvPER; i++)
        free(vPER[i]);

    // Da free em todos os espacos alocados da string 'vCONF'
    for (i = 0; i < tamvCONF; i++)
        free(vCONF[i]);

    free(vPER);
    free(vCONF);
    free(pesquisador);
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
