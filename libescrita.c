#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMSTRING 128

void imprime_vetor(char** v, int tamv)
{
    int i;

    for (i = 0; i < tamv; i++)
        printf("%s\n", v[i]);
}

void imprimeCatalogados(char** v, int tamv)
{
    int i, j, k, ind = 0;
    char niveis[9][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3" ,"B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);

    // Loop com todos os niveis menos o 'C', pois eh preciso um teste mais elaborado
    // devido a ser um caracter so, todas os periodicos podem apresentar a letra 'C'
    for (j = 0; j < 8; j++)
    {
        printf("\nEstrato %s:\n", niveis[j]);
        for (i = 0; i < tamv; i++)
        {
            // Copia o nome do periodico para uma string auxiliar
            strcpy(straux, v[i]);

            // Pega o indice do fim da string
            while (straux[ind] != '\0')
                ind++;

            // Se estiver com o nivel no fim da string, imprima
            if (strstr(v[i], niveis[j]))
            {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito
                while (k < (ind - 2))
                {
                    printf("%c", straux[k]);
                    k++;
                }
                printf("\n");
            }

            ind = 0;
        }
    }

    printf("\nEstrato C:\n");
    for (i = 0; i < tamv; i++)
    {
        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v[i]);

        // Pega o indice do fim da string
        while (straux[ind] != '\0')
            ind++;

        // Verifica se tem o nivel C na string
        // Neste caso, nao temos o '\n', entao '[ind - 1]' eh o C
        if (straux[ind - 1] == 'C')
        {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito
                while (k < (ind - 1))
                {
                    printf("%c", straux[k]);
                    k++;
                }
                printf("\n");
        }

        // Se terminar com 'C-', eh porque eh nao presente na lista, tambem se encaixa
        if (straux[ind - 1] == '-')
        {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito
                while (k < (ind - 2))
                {
                    printf("%c", straux[k]);
                    k++;
                }
                printf("\n");
        }

        ind = 0;
    }

    printf("\n");
    free(straux);
}

// Funcao que retorna quantas vezes uma string se repete em um vetor de strings
int seRepete(char* str, char** v, int tam)
{
    int i = 0;
    int cont = 0;

    for (i = 0; i < tam; i++)
    {
        if(strstr(v[i], str))
            cont++;     
    }

    // Se cont retornar 0, eh porque nao achou a string no vetor
    return cont;
}

// Funcao que converte todos os caracteres do vetor de strings para minusculo
void paraMinusculo(char** v, int tam)
{
    int ind, indV; 
    char* straux = malloc (sizeof(char) * TAMSTRING);

    // Necessario passar os nomes das strings para letras minusculas
    for (indV = 0; indV < tam; indV++)
    { 
        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v[indV]);

        // Zera o indice que apontara para os caracteres da string
        ind = 0;

        // Varrendo a string ate ela chegar no final
        while (straux[ind] != '\0')
        {
            // Rebaixando o caracter para caixa alta com 'tolower'
            straux[ind] = tolower(straux[ind]);
            ind++;
        }

        // Transfere o auxiliar de volta para o vetor de strings
        strcpy(v[indV], straux);
        strcpy(straux, "");
    }

    free(straux);
}

void imprimeSumarizada(char** v, int tam)
{
    int i, j, k, ult, ind = 0;
    int tamlvl = 8, tamvAUX = 0, cont = 0;
    char vlvl[9][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vAUX = malloc(sizeof(char*) * 512);

    // Percorrendo de nivel a nivel
    for (i = 0; i < tamlvl; i++)
    {
        printf("\nEstrato %s:\n\n", vlvl[i]);

        for (j = 0; j < tam; j++)
        {
            // Se o titulo corresponde ao nivel da vez
            if (strstr(v[j], vlvl[i]))
            {
                if (!seRepete(v[j], vAUX, tamvAUX))
                {
                    // Copia o nome para a string auxiliar
                    strcpy(straux, v[j]);

                    // Pega o indice do ultimo caracter
                    while(straux[ind] != '\0')
                        ind++;

                    k = 0;

                    // Imprimindo a string toda menos o nivel nela escrito, nesse caso 'vlvl[i]'
                    while (k < (ind - 2))
                    {
                        printf("%c", straux[k]);
                        k++;
                    }

                    // Adiciona o nome no v_aux e incrementa seu tamanho
                    vAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                    strcpy(vAUX[tamvAUX], straux);
                    tamvAUX++;

                    cont = seRepete(straux, v, tam);

                    printf(": %d\n", cont);

                    // Zera o indice
                    ind = 0;
                }
            }
        }
    }

    printf("\nEstrato C:\n\n");

    for (i = 0; i < tam; i++)
    {
        // Copia o nome para a string auxiliar
        strcpy(straux, v[i]);

        // Pega o indice do ultimo caracter
        while(straux[ind] != '\0')
            ind++;

        if(straux[ind - 1] == '-' || straux[ind - 1] == 'C')
        {
            if (!seRepete(v[i], vAUX, tamvAUX))
            {
                 if (straux[ind - 1] == '-')
                    ult = 2;
                
                if (straux[ind - 1] == 'C')
                    ult = 1;

                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito, nesse caso ou 'C' ou 'C-'
                while (k < (ind - ult))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no v_aux e incrementa seu tamanho
                vAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(vAUX[tamvAUX], straux);
                tamvAUX++;

                cont = seRepete(straux, v, tam);

                printf(": %d\n", cont);
            }

            // Zera o indice
            ind = 0;
        }
    }

    // Da free em todos os espacos alocados da string 'vAUX'
    for (i = 0; i < tamvAUX; i++)
        free(vAUX[i]);

    free(vAUX);
    free(straux);
}

void imprimeSumarizadaAutoria(char* pesquisador, char** vPER, int tamvPER, char** vCONF, int tamvCONF)
{
    int tamlvl = 8, i, ind, j;
    int contlvlPER = 0, contlvlCONF = 0;
    char* straux = malloc(sizeof(char) * 300);
    char vlvl[9][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};

    printf("Pesquisador: %s\n", pesquisador);

    printf("+------------+------------+\n|Conferencias| Periódicos |\n+------------+------------+\n");

    for (i = 0; i < tamlvl; i++)
    {
        for (j = 0; j < tamvPER; j++)
        {
            // Se o titulo do periodico corresponde ao nivel da vez
            if (strstr(vPER[j], vlvl[i]))
                contlvlPER++;
        }

        for (j = 0; j < tamvCONF; j++)
        {
            // Se o titulo da conferencia corresponde ao nivel da vez
            if (strstr(vCONF[j], vlvl[i]))
                contlvlCONF++;
        }

        printf("| %s : %d     | %s : %d     |\n", vlvl[i], contlvlCONF, vlvl[i], contlvlPER);

        // Zera as variaveis que armazenam quantidade de titulos
        contlvlPER = 0;
        contlvlCONF = 0;
    }

    for (j = 0; j < tamvPER; j++)
    {
        ind = 0;

        // Copia a string para uma auxilair
        strcpy(straux, vPER[j]);

        // Pega o ultimo indice da string
        while(straux[ind] != '\0')
            ind++;

        if (straux[ind - 1] == '-')
            contlvlPER++;
                
        else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
            contlvlPER++;     
    }

    for (j = 0; j < tamvCONF; j++)
    {
        ind = 0;

        // Copia a string para uma auxilair
        strcpy(straux, vCONF[j]);

        // Pega o ultimo indice da string
        while(straux[ind] != '\0')
            ind++;

        if (straux[ind - 1] == '-')
            contlvlCONF++;
                
        else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
            contlvlCONF++;     
    }

    printf("| %s  : %d    | %s  : %d     |\n", "C", contlvlCONF, "C", contlvlPER);
    printf("+------------+------------+\n");

    free(straux);
}

void imprime_tudoC(char** vPER, int tamvPER, char** vCONF, int tamvCONF)
{
    int i, k, ind = 0, tamvAUX = 0;
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vperAUX = malloc(sizeof(char*) * 512);
    char** vconfAUX = malloc(sizeof(char*) * 512);
    int ult = 0;

    printf("Periodicos:\n\n");

    for (i = 0; i < tamvPER; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, vPER[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C', imprime
        if (straux[ind - 1] == '-' || straux[ind - 1] == 'C')
        {
            if (!seRepete(straux, vperAUX, tamvAUX))
            {
                k = 0;

                if (straux[ind - 1] == '-')
                    ult = 2;
                
                if (straux[ind - 1] == 'C')
                    ult = 1;

                // Imprimindo a string toda menos o nivel nela escrito
                while (k < (ind - ult))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no vAUX e incrementa seu tamanho
                vperAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(vperAUX[tamvAUX], straux);
                tamvAUX++;

                printf("\n");
            }
        }

        // Zera o indice
        ind = 0;
    }

    // Da free em todos os espacos alocados da string 'vperAUX'
    for (i = 0; i < tamvAUX; i++)
        free(vperAUX[i]);
    free(vperAUX);

    tamvAUX = 0;

    printf("\n\nConferencias:\n\n");

    for (i = 0; i < tamvCONF; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, vCONF[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C', imprime
        if (straux[ind - 1] == '-' || straux[ind - 1] == 'C')
        {
            if (!seRepete(straux, vconfAUX, tamvAUX))
            {
                k = 0;

                if (straux[ind - 1] == '-')
                    ult = 2;
                
                if (straux[ind - 1] == 'C')
                    ult = 1;

                // Imprimindo a string toda menos o nivel nela escrito
                while (k < (ind - ult))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no vAUX e incrementa seu tamanho
                vconfAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(vconfAUX[tamvAUX], straux);
                tamvAUX++;

                printf("\n");
            }
        }

        // Zera o indice
        ind = 0;
    }

    // Da free em todos os espacos alocados da string 'vconfAUX'
    for (i = 0; i < tamvAUX; i++)
        free(vconfAUX[i]);
    free(vconfAUX);
    free(straux);
}

void imprime_NaoClassificados(char** vPER, int tamvPER, char** vCONF, int tamvCONF)
{
    int i, k, ind = 0, tamvAUX = 0;
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vperAUX = malloc(sizeof(char*) * 512);
    char** vconfAUX = malloc(sizeof(char*) * 512);

    printf("Periodicos:\n\n");

    for (i = 0; i < tamvPER; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, vPER[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C', imprime
        if (straux[ind - 1] == '-')
        {
            if (!seRepete(straux, vperAUX, tamvAUX))
            {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito, neste caso, 'C-'
                while (k < (ind - 2))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no v_aux e incrementa seu
                vperAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(vperAUX[tamvAUX], straux);
                tamvAUX++;

                printf("\n");
            }
        }

        // Zera o indice
        ind = 0;
    }

    // Da free em todos os espacos alocados da string 'vperAUX'
    for (i = 0; i < tamvAUX; i++)
        free(vperAUX[i]);
    free(vperAUX);

    tamvAUX = 0;

    printf("\n\nConferencias:\n\n");

    for (i = 0; i < tamvCONF; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, vCONF[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C', imprime
        if (straux[ind - 1] == '-')
        {
            if (!seRepete(straux, vconfAUX, tamvAUX))
            {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito, neste caso, 'C-'
                while (k < (ind - 2))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no v_aux e incrementa seu
                vconfAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(vconfAUX[tamvAUX], straux);
                tamvAUX++;

                printf("\n");
            }
        }

        // Zera o indice
        ind = 0;
    }

    // Da free em todos os espacos alocados da string 'vperAUX'
    for (i = 0; i < tamvAUX; i++)
        free(vconfAUX[i]);
    free(vconfAUX);
    
    free(straux);
}
