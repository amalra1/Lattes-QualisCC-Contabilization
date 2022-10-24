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
