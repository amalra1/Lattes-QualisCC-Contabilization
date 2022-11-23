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
void pegaDados(FILE* arqXML, FILE* arqPER, FILE* arqCONF, pesquisador_t *p)
{

    nomePesquisador(arqXML, p);

    coletarTitulos(arqXML, p);

    //imprime_vetor(p->vPER, p->tamvPER);

    //imprime_vetor(p->vCONF, p->tamvCONF);

    //printf("A\nA\nA\nA\nA\n");

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

    /*printf("\n---------------------------=Todos os periodicos e eventos classificados em nivel C=---------------------------\n\n");

    imprime_tudoC(p); //(5)

    printf("\n--------------------------=Todos os periodicos/eventos nao classificados=--------------------------\n\n");

    imprime_NaoClassificados(p); //(6)*/
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

    // Criando o path para o arquivo
    strcpy(path, "");
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

    // Testa se o arquivo abre
    if (arqCONF == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    // Abre o diretorio
    dir = opendir(nome_dir);

    // Testa se o diretorio abre
    if (dir == NULL)
    {
        printf("Impossivel abrir diretorio\n");
        exit(1);  // Fecha o programa com status 1
    }




    // Conta quantos pesquisadores terão
    while ((entry = readdir(dir)) != NULL)
    {
        // Evitando pegar os diretorios ocultos '.' e '..'
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
            tamvp++;
    }

    rewinddir(dir);

    pesquisador_t *p[tamvp];  // Alocando vetor de 3 ponteiros para structs
    pesquisador_t *(*vp)[] = &p;  // Alocando ponteiro para o vetor de structs

    // Inicializa cada pesquisador
    for(i = 0; i < tamvp; i++)
        p[i] = malloc(sizeof(pesquisador_t));

    for(i = 0; i < tamvp; i++)
        inicia_pesquisador((*vp)[i]);

    i = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        // Evitando pegar os diretorios ocultos '.' e '..'
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            // Concatenando o nome do arquivo no PATH
            strcat(path, entry->d_name);

            // Abre o arquivo
            arqXML = fopen(path, "r");

            // Testa se o arquivo abre
            if (arqXML == NULL)
            {
                printf("Impossivel abrir arquivo\n");
                exit(1);  // Fecha o programa com status 1
            }

            pegaDados(arqXML, arqPER, arqCONF, (*vp)[i]);
            i++;

            // Voltando o path para sem o nome do arquivo
            strcpy(path, pathORIGINAL);
            fclose(arqXML);
        }
    }
    
    closedir(dir);

    printf("\n\n\n---------------------------=Producao sumarizada do grupo por ordem de periodicos=---------------------------\n\n");

    imprimeSumarizadaPER(*vp, tamvp); //(1)

    printf("\n---------------------------=Producao sumarizada do grupo por ordem de conferencias=---------------------------\n");

    imprimeSumarizadaCONF(*vp, tamvp); //(2)

    printf("\n---------------------------=Producao dos pesquisadores do grupo por ordem de autoria=---------------------------\n\n");

    for (i = 0; i < tamvp; i++)
        imprimeSumarizadaAutoria((*vp)[i]); //(3)

    printf("\n---------------------------=Producao sumarizada do grupo por ano=---------------------------\n");

    imprimeSumarizadaAno(*vp, tamvp); //(4)


    



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
