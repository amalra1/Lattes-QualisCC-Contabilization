#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libcoleta.h"

#define TAMSTRING 512

void imprime_vetor(char** v, int tamv)
{
    int i;

    for (i = 0; i < tamv; i++)
        printf("%s\n", v[i]);
}

// Funcao que retorna quantas vezes uma string se repete em um vetor de strings
int seRepete(char* str, char** v, int tam)
{
    int i = 0;
    int cont = 0;

    for (i = 0; i < tam; i++)
    {
        if(!strcmp(v[i], str))
            cont++;     
    }

    // Se cont retornar 0, eh porque nao achou a string no vetor
    return cont;
}

void imprimeSumarizadaPER(pesquisador_t **vp, int tamvp)
{
    int i, x = 0, j = 0, k, ult, tamvPER = 0, ind = 0;
    int tamlvl = 8, tamvAUX = 0, cont = 0;
    char vlvl[9][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vAUX = malloc(sizeof(char*) * 512);
    char** vPER;

    // Vendo qual sera o tamanho necessario do vetor vPER
    for (i = 0; i < tamvp; i++)
        tamvPER = tamvPER + vp[i]->tamvPER;

    vPER = malloc(sizeof(char*) * tamvPER + 1);

    // Aqui juntamos os periodicos de todos os pesquisadores em um 
    // vetor de strings só, para aí podermos imprimi-los catalogados
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvPER; j++)
        {
            vPER[x] = malloc(sizeof(char) * (strlen(vp[i]->vPER[j]) + 1));
            strcpy(vPER[x], vp[i]->vPER[j]);
            //printf("\n\n%s\n\n", vPER[x]);
            x++;
        }
    }

    // Percorrendo de nivel a nivel
    for (i = 0; i < tamlvl; i++)
    {
        printf("\nEstrato %s:\n\n", vlvl[i]);

        for (j = 0; j < tamvPER; j++)
        {
            // Se o titulo corresponde ao nivel da vez
            if (strstr(vPER[j], vlvl[i]))
            {
                if (!seRepete(vPER[j], vAUX, tamvAUX))
                {
                    // Copia o nome para a string auxiliar
                    strcpy(straux, vPER[j]);

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

                    cont = seRepete(vPER[j], vPER, tamvPER);

                    printf(": %d\n", cont);

                    // Zera o indice
                    ind = 0;
                }
            }
        }
    }

    printf("\nEstrato C:\n\n");
    ind = 0;

    for (i = 0; i < tamvPER; i++)
    {
        // Copia o nome para a string auxiliar
        strcpy(straux, vPER[i]);

        // Pega o indice do ultimo caracter
        while(straux[ind] != '\0')
            ind++;

        if(straux[ind - 1] == 'C' || strstr(straux, "C-"))
        {
            if (!seRepete(vPER[i], vAUX, tamvAUX))
            {
                if (strstr(straux, "C-"))
                    ult = 2;
                
                else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
                    ult = 1;

                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito, nesse caso ou 'C' ou 'C-'
                while (k < (ind - ult))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no vAUX e incrementa seu tamanho
                vAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(vAUX[tamvAUX], straux);
                tamvAUX++;
 
                cont = seRepete(vPER[i], vPER, tamvPER);

                printf(": %d\n", cont);
            }

            // Zera o indice
            ind = 0;
        }
    }

    // Da free em todos os espacos alocados da string 'vAUX'
    for (i = 0; i < tamvAUX; i++)
        free(vAUX[i]);

    // Da free em todos os espacos alocados da string 'vPER'
    for (i = 0; i < tamvPER; i++)
        free(vPER[i]);

    free(vPER);
    free(vAUX);
    free(straux);
}

void imprimeSumarizadaCONF(pesquisador_t *p)
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

        for (j = 0; j < p->tamvCONF; j++)
        {
            // Se o titulo corresponde ao nivel da vez
            if (strstr(p->vCONF[j], vlvl[i]))
            {
                if (!seRepete(p->vCONF[j], vAUX, tamvAUX))
                {
                    // Copia o nome para a string auxiliar
                    strcpy(straux, p->vCONF[j]);

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

                    cont = seRepete(p->vCONF[j], p->vCONF, p->tamvCONF);

                    printf(": %d\n", cont);

                    // Zera o indice
                    ind = 0;
                }
            }
        }
    }

    printf("\nEstrato C:\n\n");
    ind = 0;

    for (i = 0; i < p->tamvCONF; i++)
    {
        // Copia o nome para a string auxiliar
        strcpy(straux, p->vCONF[i]);

        // Pega o indice do ultimo caracter
        while(straux[ind] != '\0')
            ind++;

        if(straux[ind - 1] == 'C' || strstr(straux, "C-"))
        {
            if (!seRepete(p->vCONF[i], vAUX, tamvAUX))
            {
                if (strstr(straux, "C-"))
                    ult = 2;
                
                else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
                    ult = 1;

                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito, nesse caso ou 'C' ou 'C-'
                while (k < (ind - ult))
                {
                    printf("%c", straux[k]);
                    k++;
                }

                // Adiciona o nome no vAUX e incrementa seu tamanho
                vAUX[tamvAUX] = malloc(sizeof(char) * (strlen(straux) + 1));
                strcpy(vAUX[tamvAUX], straux);
                tamvAUX++;
 
                cont = seRepete(p->vCONF[i], p->vCONF, p->tamvCONF);

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

void imprimeSumarizadaAutoria(pesquisador_t *p)
{
    int tamlvl = 8, i, ind, j;
    int contlvlPER = 0, contlvlCONF = 0;
    char* straux = malloc(sizeof(char) * 300);
    char vlvl[9][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};

    printf("Pesquisador: %s\n", p->nome);

    printf("+------------+------------+\n|Conferencias| Periódicos |\n+------------+------------+\n");

    for (i = 0; i < tamlvl; i++)
    {
        for (j = 0; j < p->tamvPER; j++)
        {
            // Se o titulo do periodico corresponde ao nivel da vez
            if (strstr(p->vPER[j], vlvl[i]))
                contlvlPER++;
        }

        for (j = 0; j < p->tamvCONF; j++)
        {
            // Se o titulo da conferencia corresponde ao nivel da vez
            if (strstr(p->vCONF[j], vlvl[i]))
                contlvlCONF++;
        }

        printf("| %s : %d     | %s : %d     |\n", vlvl[i], contlvlCONF, vlvl[i], contlvlPER);

        // Zera as variaveis que armazenam quantidade de titulos
        contlvlPER = 0;
        contlvlCONF = 0;
    }

    for (j = 0; j < p->tamvPER; j++)
    {
        ind = 0;

        // Copia a string para uma auxilair
        strcpy(straux, p->vPER[j]);

        // Pega o ultimo indice da string
        while(straux[ind] != '\0')
            ind++;

        if (strstr(straux, "C-"))
            contlvlPER++;
                
        else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
            contlvlPER++;     
    }

    for (j = 0; j < p->tamvCONF; j++)
    {
        ind = 0;

        // Copia a string para uma auxilair
        strcpy(straux, p->vCONF[j]);

        // Pega o ultimo indice da string
        while(straux[ind] != '\0')
            ind++;

        if (strstr(straux, "C-"))
            contlvlCONF++;
                
        else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
            contlvlCONF++;     
    }

    printf("| %s  : %d     | %s  : %d     |\n", "C", contlvlCONF, "C", contlvlPER);
    printf("+------------+------------+\n");

    free(straux);
}

void imprime_tudoC(pesquisador_t *p)
{
    int i, k, ind = 0, tamvAUX = 0;
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vperAUX = malloc(sizeof(char*) * 512);
    char** vconfAUX = malloc(sizeof(char*) * 512);
    int ult = 0;

    printf("Periodicos:\n\n");

    for (i = 0; i < p->tamvPER; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, p->vPER[i]);

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

    for (i = 0; i < p->tamvCONF; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, p->vCONF[i]);

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

void imprime_NaoClassificados(pesquisador_t *p)
{
    int i, k, ind = 0, tamvAUX = 0;
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vperAUX = malloc(sizeof(char*) * 512);
    char** vconfAUX = malloc(sizeof(char*) * 512);

    printf("Periodicos:\n\n");

    for (i = 0; i < p->tamvPER; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, p->vPER[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C-', imprime
        if (strstr(straux, "C-"))
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

    for (i = 0; i < p->tamvCONF; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, p->vCONF[i]);

        // Pega o ultimo indice
        while (straux[ind] != '\0')
            ind++;

        // Se for 'C-', imprime
        if (strstr(straux, "C-"))
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

int cmpfunc (const void * a, const void * b) 
{
   return (*(int*)a - *(int*)b);
}

// Funcao que retorna quantas vezes um valor se repete em um vetor
int seRepeteINT(int x, int* v, int tam)
{
    int i = 0;
    int cont = 0;

    for (i = 0; i < tam; i++)
    {
        if(v[i] == x)
            cont++;     
    }

    // Se cont retornar 0, eh porque nao achou a string no vetor
    return cont;
}

void imprimeSumarizadaAno(pesquisador_t *p)
{
    int i, j, ind = 0;
    int tamvANOordem = 0;
    int* vANOordem = malloc(sizeof(int) * (p->tamvANOper + p->tamvANOconf));  // Vetor de todos os anos em ordem sem repeticoes
    char* straux = malloc(sizeof(char) * 512);
    int PERqntA1 = 0;
    int PERqntA2 = 0;
    int PERqntA3 = 0;
    int PERqntA4 = 0;
    int PERqntB1 = 0;
    int PERqntB2 = 0;
    int PERqntB3 = 0;
    int PERqntB4 = 0;
    int PERqntC = 0;
    int CONFqntA1 = 0;
    int CONFqntA2 = 0;
    int CONFqntA3 = 0;
    int CONFqntA4 = 0;
    int CONFqntB1 = 0;
    int CONFqntB2 = 0;
    int CONFqntB3 = 0;
    int CONFqntB4 = 0;
    int CONFqntC = 0;

    // Adiciona os anos dos periodicos no vetor de ordem evitando repeticoes
    for (i = 0; i < p->tamvANOper; i++)
    {
        if (!seRepeteINT(p->vANOper[i], vANOordem, tamvANOordem))
        {
            vANOordem[tamvANOordem] = p->vANOper[i];
            tamvANOordem++;
        }
    }

    // Adiciona os anos das conferencias no vetor de ordem evitando repeticoes
    for (i = 0; i < p->tamvANOconf; i++)
    {
        if (!seRepeteINT(p->vANOconf[i], vANOordem, tamvANOordem))
        {
            vANOordem[tamvANOordem] = p->vANOconf[i];
            tamvANOordem++;
        }
    }

    // Ordena o vetor vANO de anos com 'qsort'
    qsort(vANOordem, tamvANOordem, sizeof(int), cmpfunc);

    // Passando pelo ordenado sem repeticoes
    for(i = 0; i < tamvANOordem; i++)
    {
        // Passando pelo nao ordenado em que as msms posicoes sao dos titulos
        for (j = 0; j < p->tamvANOper; j++)
        {
            // Se eh o ano da vez
            if(p->vANOper[j] == vANOordem[i])
            {
                // Checando nivel do titulo para incrementar na variavel
                if (strstr(p->vPER[j], "A1"))
                    PERqntA1++;
                else if (strstr(p->vPER[j], "A2"))
                    PERqntA2++;
                else if (strstr(p->vPER[j], "A3"))
                    PERqntA3++;
                else if (strstr(p->vPER[j], "A4"))
                    PERqntA4++;
                else if (strstr(p->vPER[j], "B1"))
                    PERqntB1++;
                else if (strstr(p->vPER[j], "B2"))
                    PERqntB2++;
                else if (strstr(p->vPER[j], "B3"))
                    PERqntB3++;
                else if (strstr(p->vPER[j], "B4"))
                    PERqntB4++;

                ind = 0;

                // Copia a string para uma auxilair
                strcpy(straux, p->vPER[j]);

                // Pega o ultimo indice da string
                while(straux[ind] != '\0')
                    ind++;

                if (strstr(straux, "C-"))
                    PERqntC++;
                
                else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
                    PERqntC++;     
            }
        }

        // Passando pelo nao ordenado das conferencias em que as msms posicoes sao dos titulos
        for (j = 0; j < p->tamvANOconf; j++)
        {
            // Se eh o ano da vez
            if(p->vANOconf[j] == vANOordem[i])
            {
                // Checando nivel do titulo para incrementar na variavel
                if (strstr(p->vCONF[j], "A1"))
                    CONFqntA1++;
                else if (strstr(p->vCONF[j], "A2"))
                    CONFqntA2++;
                else if (strstr(p->vCONF[j], "A3"))
                    CONFqntA3++;
                else if (strstr(p->vCONF[j], "A4"))
                    CONFqntA4++;
                else if (strstr(p->vCONF[j], "B1"))
                    CONFqntB1++;
                else if (strstr(p->vCONF[j], "B2"))
                    CONFqntB2++;
                else if (strstr(p->vCONF[j], "B3"))
                    CONFqntB3++;
                else if (strstr(p->vCONF[j], "B4"))
                    CONFqntB4++;

                ind = 0;

                // Copia a string para uma auxilair
                strcpy(straux, p->vCONF[j]);

                // Pega o ultimo indice da string
                while(straux[ind] != '\0')
                    ind++;

                if (strstr(straux, "C-"))
                    CONFqntC++;
                
                else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
                    CONFqntC++;     
            }
        }

        printf("\nAno %d\n", vANOordem[i]);
        printf("+------------+------------+\n|Conferencias| Periódicos |\n+------------+------------+\n");
        printf("| A1  : %d    | A1  : %d    |\n", CONFqntA1, PERqntA1);
        printf("| A2  : %d    | A2  : %d    |\n", CONFqntA2, PERqntA2);
        printf("| A3  : %d    | A3  : %d    |\n", CONFqntA3, PERqntA3);
        printf("| A4  : %d    | A4  : %d    |\n", CONFqntA4, PERqntA4);
        printf("| B1  : %d    | B1  : %d    |\n", CONFqntB1, PERqntB1);
        printf("| B2  : %d    | B2  : %d    |\n", CONFqntB2, PERqntB2);
        printf("| B3  : %d    | B3  : %d    |\n", CONFqntB3, PERqntB3);
        printf("| B4  : %d    | B4  : %d    |\n", CONFqntB4, PERqntB4);
        printf("| C   : %d    | C   : %d    |\n", CONFqntC, PERqntC);
        printf("+------------+------------+\n");

        // Zera as variaveis contadoras
        PERqntA1 = 0;
        PERqntA2 = 0;
        PERqntA3 = 0;
        PERqntA4 = 0;
        PERqntB1 = 0;
        PERqntB2 = 0;
        PERqntB3 = 0;
        PERqntB4 = 0;
        PERqntC = 0;
        CONFqntA1 = 0;
        CONFqntA2 = 0;
        CONFqntA3 = 0;
        CONFqntA4 = 0;
        CONFqntB1 = 0;
        CONFqntB2 = 0;
        CONFqntB3 = 0;
        CONFqntB4 = 0;
        CONFqntC = 0;
    }

    free(vANOordem);
    free(straux);
}
