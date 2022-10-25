#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARQUIVO "curriculoCASTILHO.xml"

/*
    Contabilização da produção científica em termos de
    artigos científicos publicados em periódicos e 
    conferências de um grupo de pesquisadores por meio
    da análise de seus currículos Lattes.
    
    Desenvolvido por Pedro Amaral Chapelin
    Data de finalização -> XX/XX/XXXX
    
    1 -> lê um arq .xml e imprime seu conteudo --FEITO--
    2 -> imprimir na tela apenas o nome dos periodicos (vêm depois do 'TITULO-DO-PERIODICO-OU-REVISTA=' e vai até "")
         while !EOF (if 'TITULO-DO-PERIODICO-OU-REVISTA=' armazenar até a última ")
         armazenar num vetor de strings (?)
    3 -> imprimir na tela apenas o nome das conferencias
    4 -> catalogar periodico(s) e imprimi-lo(s) na tela
    5 -> mesma coisa com conferencia
    6 -> abre um diretório, le todos os arquivos e imprime seus conteudos 
*/

int titulo_artigo(char c; FILE* arq; int *tamv_per)
{
    // Verificacoes para ver se esta na parte anterior ao nome do periodico buscado
    if (c != 'S')
        return 0;
    
    c = fgetc(arq);

    if (c != 'T')
        return 0;

    c = fgetc(arq);

    if (c != 'A')
        return 0;

    c = fgetc(arq);

    if (c != '=')
        return 0;

    // Se chegou ate aqui, eh porque achou o titulo do artigo             
    c = fgetc(arq);
    return 1;
}

int main ()
{
    FILE* arq;
    char c;
    int tamv_per = 0;
    char** v_per = malloc(sizeof(char*) * 256);  // Aloca vetor de strings para 256 strings
    char *str = malloc(sizeof(char) * 512);  // String para armazenar cada nome de periodico
   
    // Abre o arquivo
    arq = fopen(ARQUIVO, "r");

    // Testa se o arquivo abre
    if (arq == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }
    
    c = fgetc(arq);

    // Enquanto nao chegou no final do arquivo, faz 
    while (c != EOF)
    {
         // Se passou da verificacao, armazena o periodico
        if (titulo_artigo(c, arq))
        {
            c = fgetc(arq);

            while (c != '\"')
            {
                //printf("%c", c);

                // Concatenando um caracter a mais na string do vetor de strings
                strncat(str, &c, 1);

                // Pega o proximo caracter
                c = fgetc(arq);
            }

            printf("%s", str);
            printf("\n\n");
            
            // Adiciona o nome do periodico no vetor e incrementa seu tamanho
            strcpy(v_per[tamv_per], str);
            tamv_per++;

            // Zera a string
            strcpy(str, "");
        }

        c = fgetc(arq);
    }

    for (i = 0; i < tamv_per; i++)
        printf("%s\n\n", v_per[i]);

    free(str);
    free(v_per);
    fclose(arq);
    
    return 0;
}
