#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER 400000

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
    int c;
    char linha[BUFFER], *tok;
   
    arq = fopen("curriculoCASTILHO.xml", "r");

    // Testa se o arquivo abre
    if (arq == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    
    fgets(linha, BUFFER, arq);


    tok = strtok(linha, ">");
    /*while (tok != NULL)
    {
        printf("%s\n\n", tok);
        tok = strtok(NULL, "<");
    }*/

    printf("%s\n\n", tok);

    fclose(arq);
    
    return 0;
}
