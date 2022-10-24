/*
    Contabilização da produção científica em termos de
    artigos científicos publicados em periódicos e 
    conferências de um grupo de pesquisadores por meio
    da análise de seus currículos Lattes.
    
    Desenvolvido por Pedro Amaral Chapelin
    Data de finalização -> XX/XX/XXXX
    
    1 -> lê um arq .xml e imprime seu conteudo --FEITO--
    2 -> imprimir na tela apenas o nome dos periodicos (vêm depois do 'TITULO-DO-ARTIGO=' e vai até 'ANO-DO-ARTIGO')
    3 -> imprimir na tela apenas o nome das conferencias
    4 -> catalogar periodico(s) e imprimi-lo(s) na tela
    5 -> mesma coisa com conferencia
    6 -> abre um diretório, le todos os arquivos e imprime seus conteudos 
*/
#include <stdio.h>
#include <stdlib.h>

int main ()
{
    FILE* arq;
    int c;
   
    arq = fopen("curriculoCASTILHO.xml", "r");

    // Testa se o arquivo abre
    if (arq == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    c = fgetc(arq);
    // Pega char a char ate o fim do arquivo
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(arq);
    }

    fclose(arq);
    
    return 0;
}
