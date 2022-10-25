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

int titulo_artigo(char c; FILE* arq)
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
    int achou, tamv_per = 1, tamstr = 0;
    char** v_per = malloc(sizeof(char*));  // Aloca vetor de strings para 1 string
    char str[1000] = "";  // String para armazenar cada nome de periodico
   
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
        if (titulo_artigo(c, arq))  // Se passou da verificacao, armazena o periodico
        {
            c = fgetc(arq);
            str = malloc(sizeof(char));  // Aloca para um char
            while (c != '\"')
            {
                //printf("%c", c);

                // Concatenando um caracter a mais na string do vetor de strings
                strncat(str, &c, 1);

                // Incrementa o tamanho da string e realoca memoria da mesma
                tamstr++;
                str = realloc(str, sizeof(char) * tamstr);

                // Pega o proximo caracter
                c = fgetc(arq);
            }

            printf("%s", str);

            // Incrementa o tamanho do vetor de titulos e realoca memoria do mesmo
            tamv_per++; 
            v_per = realloc(v_per, sizeof(char*) * tamv_per);

            printf("\n\n");
            strcpy(str, "");  // Zera a string
        }

        c = fgetc(arq);
    }

    fclose(arq);
    
    return 0;
}
