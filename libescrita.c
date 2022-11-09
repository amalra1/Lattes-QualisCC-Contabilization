#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprime_vetor(char** v, int tamv)
{
    int i;

    for (i = 0; i < tamv; i++)
        printf("%s\n", v[i]);
}

void imprimeCatalogados(char** v, int tamv)
{
    int i, j, k, ind = 0;
    char niveis[20][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3" ,"B4"};
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
        if(!strcmp(str, v[i]))
            cont++;     
    }

    // Se cont retornar 0, eh porque nao achou a string no vetor
    return cont;
}

// Funcao que verifica se uma string pertence a um vetor de strings.
// Retorna 1 se achou e 0 se nao
int achou(char** v, int tam, char* str)
{
    int i;

    for (i = 0; i < tam; i++)
    {
        if (strstr(v[i], str))
            return 1;
    }

    // Se chegou aqui eh porque nao achou
    return 0;
}

void imprimeSumarizada(char** v, int tam)
{
    int i, j, k, ult, ind = 0;
    int tamlvl = 8, tamv_aux = 0, cont = 0;
    char vlvl[20][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};
    char* straux = malloc(sizeof(char) * 512);
    char** v_aux = malloc(sizeof(char*) * TAMSTRING);

    // Zera a string auxiliar
    strcpy(straux, "");

    // Percorrendo de nivel a nivel
    for (i = 0; i < tamlvl; i++)
    {
        printf("\nEstrato %s:\n\n", vlvl[i]);

        for (j = 0; j < tam; j++)
        {
            // Se o titulo corresponde ao nivel da vez
            if (strstr(v[j], vlvl[i]))
            {
                if (!achou(v_aux, tamv_aux, v[j]))
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
                    v_aux[tamv_aux] = malloc(sizeof(char) * (strlen(straux) + 1));
                    strcpy(v_aux[tamv_aux], straux);
                    tamv_aux++;

                    cont = seRepete(straux, v, tam);

                    printf(": %d\n", cont);

                    // Zera a string
                    strcpy(straux, "");

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
            if (!achou(v_aux, tamv_aux, v[i]))
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
                v_aux[tamv_aux] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(v_aux[tamv_aux], straux);
                tamv_aux++;

                cont = seRepete(straux, v, tam);

                printf(": %d\n", cont);
            }

            // Zera a string
            strcpy(straux, "");

            // Zera o indice
            ind = 0;
        }
    }

    free(straux);

    // Da free em todos os espacos alocados da string 'v_aux'
    for (i = 0; i < tamv_aux; i++)
        free(v_aux[i]);

    free(v_aux);
}

void imprime_tudoC(char** v, int tamv)
{
    int i, k, ind, tamv_aux = 0;
    char* straux = malloc(sizeof(char) * 512);
    char** v_aux = malloc(sizeof(char*) * TAMSTRING);
    int ult = 0;

    for (i = 0; i < tamv; i++)
    {
        // Zera o indice
        ind = 0;

        // Zera a string
        strcpy(straux, "");

        // Copia o nome para uma string auxiliar
        strcpy(straux, v[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C', imprime
        if (straux[ind - 1] == '-' || straux[ind - 1] == 'C')
        {
            //if (!seRepete(straux, v_aux, tamv_aux))
            if (!achou(v_aux, tamv_aux, straux))
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

                // Adiciona o nome no v_aux e incrementa seu
                v_aux[tamv_aux] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(v_aux[tamv_aux], straux);
                tamv_aux++;

                // Zera a string
                strcpy(straux, "");

                printf("\n");
            }
        }
    }

    // Da free em todos os espacos alocados da string 'v_aux'
    for (i = 0; i < tamv_aux; i++)
        free(v_aux[i]);

    free(v_aux);
    free(straux);
}

void imprime_NaoClassificados(char** v, int tam)
{
    int i, k, ind, tamv_aux = 0;
    char* straux = malloc(sizeof(char) * 512);
    char** v_aux = malloc(sizeof(char*) * TAMSTRING);

    for (i = 0; i < tam; i++)
    {
        // Zera o indice
        ind = 0;

        // Zera a string
        strcpy(straux, "");

        // Copia o nome para uma string auxiliar
        strcpy(straux, v[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C', imprime
        if (straux[ind - 1] == '-')
        {
            //if (!seRepete(straux, v_aux, tamv_aux))
            if (!achou(v_aux, tamv_aux, straux))
            {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito, neste caso, 'C-'
                while (k < (ind - 2))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no v_aux e incrementa seu
                v_aux[tamv_aux] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(v_aux[tamv_aux], straux);
                tamv_aux++;

                // Zera a string
                strcpy(straux, "");

                printf("\n");
            }
        }
    }

    // Da free em todos os espacos alocados da string 'v_aux'
    for (i = 0; i < tamv_aux; i++)
        free(v_aux[i]);

    free(v_aux);
    free(straux);
}
