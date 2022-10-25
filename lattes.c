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

int main ()
{
    FILE* arq;
    char c;
    int achou, n_per = 0, i = 0, tamstr = 0;
    char** v_per;
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
        achou = 0;

        if (c == 'S')  // Verificacoes para ver se esta na parte anterior ao nome do periodico buscado
        {
            c = fgetc(arq);
            if (c == 'T')
            {
                c = fgetc(arq);
                if (c == 'A')
                {
                    c = fgetc(arq);
                    if (c == '=')
                    {
                        achou = 1;
                        n_per++;  // Incrementa o numero de periodicos encontrados
                        c = fgetc(arq);
                    }
                }
            }
        }

        if (achou == 1)  // Se passou da verificacao, armazena o periodico
        {
            c = fgetc(arq);
            while (c != '\"')
            {
                //printf("%c", c);
                strncat(str, &c, 1);  // Concatenando um caracter a mais na string do vetor de strings
                tamstr++;
                c = fgetc(arq);
            }
            
            printf("%s", str);
            printf("\n\n");
            strcpy(str, "");  // Zera a string
        }

        c = fgetc(arq);
    }

    fclose(arq);
    
    return 0;
}
