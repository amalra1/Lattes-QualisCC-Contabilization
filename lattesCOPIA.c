#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include "libcoleta.h"
#include "libescrita.h"
#define ARQUIVO "curriculoMENOTTI.xml"
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
    pesquisador_t *p = malloc(sizeof(pesquisador_t));

    inicia_pesquisador(p);

    nomePesquisador(arqXML, p);

    //printf("%s\n", p->nome);

    coletarTitulos(arqXML, p);

    //imprime_vetor(p->vPER, p->tamvPER);

    //imprime_vetor(p->vCONF, p->tamvCONF);

    //for (i = 0; i < p->tamvANOper; i++)
        //printf("%d\n", p->vANOper[i]);

    //for (i = 0; i < p->tamvANOconf; i++)
        //printf("%d\n", p->vANOconf[i]);
    
    // Nao eh necessario passar o nome das conferencias para maiusculo
    // pois no arquivo delas estao todas normais
    corrigirNomes(p);

    //imprime_vetor(p->vPER, p->tamvPER);

    //imprime_vetor(p->vCONF, p->tamvCONF);

    catalogarCONFS(arqCONF, p, DIST_MIN_CONF);
    catalogarPERS(arqPER, p, DIST_MIN_PER);
    
    //imprime_vetor(p->vCONF, p->tamvCONF);

    //imprime_vetor(p->vPER, p->tamvPER);

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de periodicos=---------------------------\n");

    imprimeSumarizadaPER(p); //(1)

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de conferencias=---------------------------\n");

    imprimeSumarizadaCONF(p); //(2)

    printf("\n---------------------------=Producao dos pesquisadores do grupo por ordem de autoria=---------------------------\n\n");

    imprimeSumarizadaAutoria(p); //(3)

    printf("\n---------------------------=Producao sumarizada do grupo por ano=---------------------------\n");

    imprimeSumarizadaAno(p); //(4)

    printf("\n---------------------------=Todos os periodicos e eventos classificados em nivel C=---------------------------\n\n");

    imprime_tudoC(p); //(5)

    printf("\n--------------------------=Todos os periodicos/eventos nao classificados=--------------------------\n\n");

    imprime_NaoClassificados(p); //(6)

    destroi_pesquisador(p);
    free(p);
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
