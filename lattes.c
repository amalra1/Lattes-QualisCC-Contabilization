#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include "libcoleta.h"
#include "libescrita.h"
#define DIST_MIN_PER  0.145   //0.145  // Distancias relativas minimas relativas aos nomes de periodicos e conferências
#define DIST_MIN_CONF 0.21  // da funcao de distancia de edicao (ver SepararSelecionados -> libcoleta.c)

/*
    Contabilização da produção científica em termos de artigos científicos publicados em
    periódicos e conferências de um grupo de pesquisadores por meio da análise de seus
    currículos Lattes.

    Desenvolvido por Pedro Amaral Chapelin
    Data de finalização -> 02/12/2022

    --O programa funciona da seguinte maneira:--

        (1) --> Recebe uma pasta com arquivos.XML referentes a curriculos de pesquisadores;

        (2) --> Nestes arquivos, coleta o nome dos periódicos em que o pesquisador publicou,
        e também o nome dos eventos(conferências) que participou;

        (3) --> Depois de coletados, cataloga os titulos de acordo com seus níveis na QUALIS
        {A1, A2, A3, A4, B1, B2, B3, B4, C}, e também 'C-', nivel criado para se referir a 
        aqueles que não estão na lista de classificação, portanto, sem classificação;

        (4) --> Imprime as informações sumarizadas do grupo de pesquisadores.
    
    Este programa utiliza-se de um algoritmo especifico de distância de edição, coletado do 
    link: https://github.com/wooorm/levenshtein.c
*/

// Funcao que percorre o diretorio e devolve quantos arquivos possui
int conta_pesquisadores(DIR* dir)
{
    int cont = 0;
    struct dirent *entry;

    // Conta quantos pesquisadores terão
    while ((entry = readdir(dir)) != NULL)
    {
        // Evitando pegar os diretorios ocultos '.' e '..'
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
            cont++;
    }

    // Volta pro começo do arquivo
    rewinddir(dir);

    return cont;
}


// Funcao que coleta os dados de cada pesquisador
void pegaDados(FILE* arqXML, FILE* arqPER, FILE* arqCONF, pesquisador_t *p)
{
    nomePesquisador(arqXML, p);

    coletarTitulos(arqXML, p);
    
    // Nao eh necessario passar o nome das conferencias para maiusculo
    // pois no arquivo delas estao todas normais
    corrigirNomes(p);

    catalogarCONFS(arqCONF, p, DIST_MIN_CONF);
    catalogarPERS(arqPER, p, DIST_MIN_PER);
}

// Funcao que imprime os dados coletados de cada pesquisador
void imprime_dados(pesquisador_t **vp, int tamvp)
{
    int i;

    printf("\n\n\n---------------------------=Producao sumarizada do grupo por ordem de periodicos=---------------------------\n\n");

    imprimeSumarizadaPER(vp, tamvp); // (1)

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de conferencias=---------------------------\n");

    imprimeSumarizadaCONF(vp, tamvp); // (2)

    printf("\n---------------------------=Producao dos pesquisadores do grupo por ordem de autoria=---------------------------\n\n");

    for (i = 0; i < tamvp; i++)
        imprimeSumarizadaAutoria(vp[i]); // (3)

    printf("\n---------------------------=Producao sumarizada do grupo por ano=---------------------------\n");

    imprimeSumarizadaAno(vp, tamvp); // (4)

    printf("\n---------------------------=Todos os periodicos e eventos classificados em nivel C=---------------------------\n\n");

    imprime_tudoC(vp, tamvp); // (5)

    printf("\n--------------------------=Todos os periodicos/eventos nao classificados=--------------------------\n\n");

    imprime_NaoClassificados(vp, tamvp); // (6)
}

int main (int argc, char** argv)
{
    FILE* arqXML, *arqPER, *arqCONF;
    DIR* dir;
    struct dirent *entry;

    char* nome_arqPER;
    char* nome_arqCONF;
    char* nome_dir;
    char* path = malloc(sizeof(char) * 128);
    char* pathORIGINAL = malloc(sizeof(char) * 128);
    int tamvp = 0; 
    int opt, i = 0;

    // Trata os argumentos passados
    while ((opt = getopt(argc, argv, "d:c:p:")) != -1) 
    {
        switch (opt) 
        {
         case 'd':
            nome_dir = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(nome_dir, optarg);            
            break;

         case 'c':
            nome_arqCONF = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(nome_arqCONF, optarg);
            break;

         case 'p':
            nome_arqPER = malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(nome_arqPER, optarg);
            break;
        }
    }

    // Criando o caminho para o arquivo para utilizar na função 'fopen'
    strcpy(path, "./");
    strcat(path, nome_dir);
    strcat(path, "/");
    strcpy(pathORIGINAL, path);


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
    if (arqCONF == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }


    // Abre o diretorio
    dir = opendir(nome_dir);
    if (dir == NULL)
    {
        printf("Impossivel abrir diretorio\n");
        exit(1);  // Fecha o programa com status 1
    }


    tamvp = conta_pesquisadores(dir);

    pesquisador_t *p[tamvp];  // Vetor de 3 ponteiros para structs
    pesquisador_t *(*vp)[] = &p;  // Ponteiro para o vetor de structs

    // Inicializa cada pesquisador
    for(i = 0; i < tamvp; i++)
        p[i] = malloc(sizeof(pesquisador_t));
    for(i = 0; i < tamvp; i++)
        inicia_pesquisador((*vp)[i]);

    // Varrendo todo o diretorio
    i = 0;

    printf("\nObtendo dados do grupo de pesquisadores: %s\n\n", nome_dir);
    while ((entry = readdir(dir)) != NULL)
    {
        // Evitando pegar os diretorios ocultos '.' e '..'
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            // Concatenando o nome do arquivo no PATH
            strcat(path, entry->d_name);

            // Abre o arquivo
            arqXML = fopen(path, "r");
            if (arqXML == NULL)
            {
                printf("Impossivel abrir arquivo\n");
                exit(1);  // Fecha o programa com status 1
            }

            printf("Pesquisador (%d/%d).... ", i + 1, tamvp);
            pegaDados(arqXML, arqPER, arqCONF, (*vp)[i]);
            printf("Concluido.\n");
            i++;

            // Voltando o path para sem o nome do arquivo
            strcpy(path, pathORIGINAL);
            fclose(arqXML);
        }
    }

    printf("\nVarredura dos dados concluída, realizando impressões:\n");
    
    closedir(dir);

    imprime_dados(*vp, tamvp);

    for(i = 0; i < tamvp; i++)
        destroi_pesquisador((*vp)[i]);

    for(i = 0; i < tamvp; i++)
        free(p[i]);

    fclose(arqPER);
    fclose(arqCONF);

    free(nome_dir);
    free(nome_arqCONF);
    free(nome_arqPER);
    
    free(path);
    free(pathORIGINAL);

    return 0;
}
