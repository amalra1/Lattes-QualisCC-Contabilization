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
    int i, cont = 0;

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
    int i, j, x = 0, ind = 0, cont = 0;
    int tamlvl = 8, tamvAUX = 0, tamvPER = 0;
    char vlvl[9][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vAUX = malloc(sizeof(char*) * 512);
    char** vPER;
    char** vPERorg;

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
            x++;
        }
    }

    vPERorg = malloc(sizeof(char*) * tamvPER);
    x = 0;

    // Faz o mesmo com o 'vPERorg'
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvPER; j++)
        {
            vPERorg[x] = malloc(sizeof(char) * (strlen(vp[i]->vPERorg[j]) + 1));
            strcpy(vPERorg[x], vp[i]->vPERorg[j]);
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
                    // Adiciona o nome no v_aux e incrementa seu tamanho
                    vAUX[tamvAUX] = malloc(sizeof(char) * (strlen(vPER[j]) + 1));
                    strcpy(vAUX[tamvAUX], vPER[j]);
                    tamvAUX++;

                    cont = seRepete(vPER[j], vPER, tamvPER);

                    printf("%s: %d\n", vPERorg[j], cont);
                }
            }
        }
    }

    printf("\nEstrato C:\n\n");

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
                // Adiciona o nome no vAUX e incrementa seu tamanho
                vAUX[tamvAUX] = malloc(sizeof(char) * (strlen(vPER[i]) + 1));
                strcpy(vAUX[tamvAUX], vPER[i]);
                tamvAUX++;
 
                cont = seRepete(vPER[i], vPER, tamvPER);

                printf("%s: %d\n", vPERorg[i], cont);
            }

            // Zera o indice
            ind = 0;
        }
    }

    //printf("TAMANHO PERIODICOS --> %d\n", tamvPER);

    // Da free em todos os espacos alocados nos vetores de strings
    for (i = 0; i < tamvAUX; i++)
        free(vAUX[i]);

    for (i = 0; i < tamvPER; i++)
        free(vPERorg[i]);

    for (i = 0; i < tamvPER; i++)
        free(vPER[i]);

    free(vPER);
    free(vPERorg);
    free(vAUX);
    free(straux);
}

void imprimeSumarizadaCONF(pesquisador_t **vp, int tamvp)
{
    int i, j, x = 0, k, ult, ind = 0, cont = 0;
    int tamlvl = 8, tamvAUX = 0, tamvCONF = 0;
    char vlvl[9][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vAUX = malloc(sizeof(char*) * 1000);
    char** vCONF;

    // Vendo qual sera o tamanho necessario do vetor vCONF
    for (i = 0; i < tamvp; i++)
        tamvCONF = tamvCONF + vp[i]->tamvCONF;

    vCONF = malloc(sizeof(char*) * tamvCONF + 1);

    // Aqui juntamos as conferencias de todos os pesquisadores em um 
    // vetor de strings só, para aí podermos imprimi-las catalogadas
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvCONF; j++)
        {
            vCONF[x] = malloc(sizeof(char) * (strlen(vp[i]->vCONF[j]) + 1));
            strcpy(vCONF[x], vp[i]->vCONF[j]);
            x++;
        }
    }
 
    // Percorrendo de nivel a nivel
    for (i = 0; i < tamlvl; i++)
    {
        printf("\nEstrato %s:\n\n", vlvl[i]);

        for (j = 0; j < tamvCONF; j++)
        {
            // Se o titulo corresponde ao nivel da vez
            if (strstr(vCONF[j], vlvl[i]))
            {
                if (!seRepete(vCONF[j], vAUX, tamvAUX))
                {
                    // Copia o nome para a string auxiliar
                    strcpy(straux, vCONF[j]);

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

                    cont = seRepete(vCONF[j], vCONF, tamvCONF);

                    printf(": %d\n", cont);

                    // Zera o indice
                    ind = 0;
                }
            }
        }
    }

    printf("\nEstrato C:\n\n");
    ind = 0;

    for (i = 0; i < tamvCONF; i++)
    {
        // Copia o nome para a string auxiliar
        strcpy(straux, vCONF[i]);

        // Pega o indice do ultimo caracter
        while(straux[ind] != '\0')
            ind++;

        if(straux[ind - 1] == 'C' || strstr(straux, "C-"))
        {
            if (!seRepete(vCONF[i], vAUX, tamvAUX))
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
 
                cont = seRepete(vCONF[i], vCONF, tamvCONF);

                printf(": %d\n", cont);
            }

            // Zera o indice
            ind = 0;
        }
    }

    //printf("TAMANHO CONFERENCIAS --> %d\n", tamvCONF);

    // Da free em todos os espacos alocados nos vetores de strings
    for (i = 0; i < tamvAUX; i++)
        free(vAUX[i]);

    for (i = 0; i < tamvCONF; i++)
        free(vCONF[i]);

    free(vCONF);
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

        // Faz o mesmo para conferencias
        for (j = 0; j < p->tamvCONF; j++)
        {
            if (strstr(p->vCONF[j], vlvl[i]))
                contlvlCONF++;
        }

        printf("| %s : %d     | %s : %d     |\n", vlvl[i], contlvlCONF, vlvl[i], contlvlPER);

        contlvlPER = 0;
        contlvlCONF = 0;
    }

    for (j = 0; j < p->tamvPER; j++)
    {
        ind = 0;

        // Copia a string para uma auxiliar
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

        strcpy(straux, p->vCONF[j]);

        while(straux[ind] != '\0')
            ind++;

        if (strstr(straux, "C-"))
            contlvlCONF++;
                
        else if (straux[ind - 1] == 'C' && straux[ind - 2] == ' ')
            contlvlCONF++;     
    }

    printf("| %s  : %d     | %s  : %d     |\n", "C", contlvlCONF, "C", contlvlPER);
    printf("+------------+------------+\n\n\n");

    free(straux);
}

void imprime_tudoC(pesquisador_t **vp, int tamvp)
{
    int i, j, k, x = 0, ind = 0, ult = 0;
    int tamvAUX = 0, tamvCONF = 0, tamvPER = 0;
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vperAUX;
    char** vPER;
    char** vconfAUX;
    char** vCONF;
    char** vPERorg;

    // Vendo qual sera o tamanho necessario do vetor vCONF
    for (i = 0; i < tamvp; i++)
        tamvCONF = tamvCONF + vp[i]->tamvCONF;

    vCONF = malloc(sizeof(char*) * tamvCONF + 1);
    vconfAUX = malloc(sizeof(char*) * tamvCONF + 1);

    // Aqui juntamos as conferencias de todos os pesquisadores em um 
    // vetor de strings só, para aí podermos imprimi-las catalogadas
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvCONF; j++)
        {
            vCONF[x] = malloc(sizeof(char) * (strlen(vp[i]->vCONF[j]) + 1));
            strcpy(vCONF[x], vp[i]->vCONF[j]);
            x++;
        }
    }

    // Vendo qual sera o tamanho necessario do vetor vPER
    for (i = 0; i < tamvp; i++)
        tamvPER = tamvPER + vp[i]->tamvPER;

    vPER = malloc(sizeof(char*) * tamvPER);
    vperAUX = malloc(sizeof(char*) * tamvPER + 1);

    x = 0;

    // Aqui juntamos os periodicos de todos os pesquisadores em um 
    // vetor de strings só, para aí podermos imprimi-los catalogados
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvPER; j++)
        {
            vPER[x] = malloc(sizeof(char) * (strlen(vp[i]->vPER[j]) + 1));
            strcpy(vPER[x], vp[i]->vPER[j]);
            x++;
        }
    }

    vPERorg = malloc(sizeof(char*) * tamvPER + 1);
    x = 0;

    // Faz o mesmo para o 'vPERorg'
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvPER; j++)
        {
            vPERorg[x] = malloc(sizeof(char) * (strlen(vp[i]->vPERorg[j]) + 1));
            strcpy(vPERorg[x], vp[i]->vPERorg[j]);
            x++;
        }
    }

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
            if (!seRepete(vPER[i], vperAUX, tamvAUX))
            {
                // Adiciona o nome no vAUX e incrementa seu tamanho
                vperAUX[tamvAUX] = malloc(sizeof(char) * (strlen(vPER[i]) + 1));
                strcpy(vperAUX[tamvAUX], straux);
                tamvAUX++;

                printf("%s\n", vPERorg[i]);
            }
        }

        // Zera o indice
        ind = 0;
    }

    // Da free em todos os espacos alocados nos vetores de strings
    for (i = 0; i < tamvAUX; i++)
        free(vperAUX[i]);
    free(vperAUX);

    for (i = 0; i < tamvPER; i++)
        free(vPERorg[i]);
    free(vPERorg);

    for (i = 0; i < tamvPER; i++)
        free(vPER[i]);
    free(vPER);

    tamvAUX = 0;

    printf("\n\nConferencias:\n\n");

    for (i = 0; i < tamvCONF; i++)
    {
        strcpy(straux, vCONF[i]);

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

        ind = 0;
    }

    for (i = 0; i < tamvAUX; i++)
        free(vconfAUX[i]);

    for (i = 0; i < tamvCONF; i++)
        free(vCONF[i]);
    free(vCONF);

    free(vconfAUX);
    free(straux);
}

void imprime_NaoClassificados(pesquisador_t **vp, int tamvp)
{
    int i, j, x = 0, k, ind = 0;
    int tamvPER = 0, tamvCONF = 0, tamvAUX = 0;
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** vperAUX;
    char** vconfAUX;
    char** vPER;
    char** vCONF;
    char** vPERorg;

    // Vendo qual sera o tamanho necessario do vetor vCONF
    for (i = 0; i < tamvp; i++)
        tamvCONF = tamvCONF + vp[i]->tamvCONF;

    vCONF = malloc(sizeof(char*) * tamvCONF + 1);
    vconfAUX = malloc(sizeof(char*) * tamvCONF + 1);

    // Aqui juntamos as conferencias de todos os pesquisadores em um 
    // vetor de strings só, para aí podermos imprimi-las catalogadas
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvCONF; j++)
        {
            vCONF[x] = malloc(sizeof(char) * (strlen(vp[i]->vCONF[j]) + 1));
            strcpy(vCONF[x], vp[i]->vCONF[j]);
            x++;
        }
    }


    // Vendo qual sera o tamanho necessario do vetor vPER
    for (i = 0; i < tamvp; i++)
        tamvPER = tamvPER + vp[i]->tamvPER;

    vPER = malloc(sizeof(char*) * tamvPER);
    vperAUX = malloc(sizeof(char*) * tamvPER + 1);


    x = 0;

    // Faz o mesmo para os periodicos
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvPER; j++)
        {
            vPER[x] = malloc(sizeof(char) * (strlen(vp[i]->vPER[j]) + 1));
            strcpy(vPER[x], vp[i]->vPER[j]);
            x++;
        }
    }

    vPERorg = malloc(sizeof(char*) * tamvPER + 1);
    x = 0;

    // E para o 'vPERorg'
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvPER; j++)
        {
            vPERorg[x] = malloc(sizeof(char) * (strlen(vp[i]->vPERorg[j]) + 1));
            strcpy(vPERorg[x], vp[i]->vPERorg[j]);
            x++;
        }
    }


    printf("Periodicos:\n\n");

    for (i = 0; i < tamvPER; i++)
    {
        // Se for 'C-', imprime
        if (strstr(vPER[i], "C-"))
        {
            if (!seRepete(vPER[i], vperAUX, tamvAUX))
            {
                // Adiciona o nome no v_aux e incrementa seu
                vperAUX[tamvAUX] = malloc(sizeof(char) * (strlen(vPER[i]) + 1));
                strcpy(vperAUX[tamvAUX], vPER[i]);
                tamvAUX++;

                printf("%s\n", vPERorg[i]);
            }
        }
    }

    // Da free em todos os espacos alocados nos vetores de strings
    for (i = 0; i < tamvAUX; i++)
        free(vperAUX[i]);
    free(vperAUX);

    for (i = 0; i < tamvPER; i++)
        free(vPERorg[i]);
    free(vPERorg);

    for (i = 0; i < tamvPER; i++)
        free(vPER[i]);
    free(vPER);

    tamvAUX = 0;

    printf("\n\nConferencias:\n\n");

    for (i = 0; i < tamvCONF; i++)
    {
        // Copia o nome para uma string auxiliar
        strcpy(straux, vCONF[i]);

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

        ind = 0;
    }

    // Da free em todos os espacos alocados nos vetores de strings
    for (i = 0; i < tamvAUX; i++)
        free(vconfAUX[i]);
    free(vconfAUX);

    for (i = 0; i < tamvCONF; i++)
        free(vCONF[i]);
    free(vCONF);
    
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

void imprimeSumarizadaAno(pesquisador_t **vp, int tamvp)
{
    int i = 0, j = 0, ind = 0, x = 0, tamvANOper = 0, tamvANOconf = 0, tamvANOordem = 0;
    int tamvPER = 0, tamvCONF = 0;
    int* vANOordem;  // Vetor de todos os anos em ordem sem repeticoes
    char** vPER;
    char** vCONF;
    int* vANOper;
    int* vANOconf;

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


    // Vendo qual sera o tamanho necessario do vetor vCONF
    for (i = 0; i < tamvp; i++)
        tamvCONF = tamvCONF + vp[i]->tamvCONF;

    vCONF = malloc(sizeof(char*) * tamvCONF + 1);

    // Aqui juntamos as conferencias de todos os pesquisadores em um 
    // vetor de strings só, para aí podermos imprimi-las catalogadas
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvCONF; j++)
        {
            vCONF[x] = malloc(sizeof(char) * (strlen(vp[i]->vCONF[j]) + 1));
            strcpy(vCONF[x], vp[i]->vCONF[j]);
            //printf("\n\n%s\n\n", vCONF[x]);
            x++;
        }
    }


    // Vendo qual sera o tamanho necessario do vetor vPER
    for (i = 0; i < tamvp; i++)
        tamvPER = tamvPER + vp[i]->tamvPER;

    vPER = malloc(sizeof(char*) * tamvPER + 1);


    x = 0;

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


    // Vendo qual sera o tamanho necessario do vetor vANOper
    for (i = 0; i < tamvp; i++)
        tamvANOper = tamvANOper + vp[i]->tamvANOper;

    vANOper = malloc(sizeof(int) * tamvANOper);

    x = 0;

    // Juntando vetores de ANOper
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvANOper; j++)
        {
            vANOper[x] = vp[i]->vANOper[j];
            //printf("\n\n%s\n\n", vPER[x]);
            x++;
        }
    }



    // Vendo qual sera o tamanho necessario do vetor vANOconf
    for (i = 0; i < tamvp; i++)
        tamvANOconf = tamvANOconf + vp[i]->tamvANOconf;

    vANOconf = malloc(sizeof(int) * tamvANOconf);

    x = 0;

    // Juntando vetores de ANOconf
    for (i = 0; i < tamvp; i++)
    {
        for (j = 0; j < vp[i]->tamvANOconf; j++)
        {
            vANOconf[x] = vp[i]->vANOconf[j];
            //printf("\n\n%s\n\n", vPER[x]);
            x++;
        }
    }
 

    // Vendo qual sera o tamanho maximo necessario do vetor vANOordem
    vANOordem = malloc(sizeof(int) * (tamvANOper + tamvANOconf));

    // Adiciona os anos dos periodicos no vetor de ordem evitando repeticoes
    for (i = 0; i < tamvANOper; i++)
    {
        if (!seRepeteINT(vANOper[i], vANOordem, tamvANOordem))
        {
            vANOordem[tamvANOordem] = vANOper[i];
            tamvANOordem++;
        }
    }

    // Adiciona os anos das conferencias no vetor de ordem evitando repeticoes
    for (i = 0; i < tamvANOconf; i++)
    {
        if (!seRepeteINT(vANOconf[i], vANOordem, tamvANOordem))
        {
            vANOordem[tamvANOordem] = vANOconf[i];
            tamvANOordem++;
        }
    }


    // Ordena o vetor vANO de anos com 'qsort'
    qsort(vANOordem, tamvANOordem, sizeof(int), cmpfunc);

    // Passando pelo ordenado sem repeticoes
    for(i = 0; i < tamvANOordem; i++)
    {
        // Passando pelo nao ordenado em que as msms posicoes sao dos titulos
        for (j = 0; j < tamvANOper; j++)
        {
            // Se eh o ano da vez
            if(vANOper[j] == vANOordem[i])
            {
                // Checando nivel do titulo para incrementar na variavel
                if (strstr(vPER[j], "A1"))
                    PERqntA1++;
                else if (strstr(vPER[j], "A2"))
                    PERqntA2++;
                else if (strstr(vPER[j], "A3"))
                    PERqntA3++;
                else if (strstr(vPER[j], "A4"))
                    PERqntA4++;
                else if (strstr(vPER[j], "B1"))
                    PERqntB1++;
                else if (strstr(vPER[j], "B2"))
                    PERqntB2++;
                else if (strstr(vPER[j], "B3"))
                    PERqntB3++;
                else if (strstr(vPER[j], "B4"))
                    PERqntB4++;

                ind = 0;

                // Copia a string para uma auxilair
                strcpy(straux, vPER[j]);

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
        for (j = 0; j < tamvANOconf; j++)
        {
            // Se eh o ano da vez
            if(vANOconf[j] == vANOordem[i])
            {
                // Checando nivel do titulo para incrementar na variavel
                if (strstr(vCONF[j], "A1"))
                    CONFqntA1++;
                else if (strstr(vCONF[j], "A2"))
                    CONFqntA2++;
                else if (strstr(vCONF[j], "A3"))
                    CONFqntA3++;
                else if (strstr(vCONF[j], "A4"))
                    CONFqntA4++;
                else if (strstr(vCONF[j], "B1"))
                    CONFqntB1++;
                else if (strstr(vCONF[j], "B2"))
                    CONFqntB2++;
                else if (strstr(vCONF[j], "B3"))
                    CONFqntB3++;
                else if (strstr(vCONF[j], "B4"))
                    CONFqntB4++;

                ind = 0;

                // Copia a string para uma auxilair
                strcpy(straux, vCONF[j]);

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


    // Da free em todos os espacos alocados da string 'vCONF'
    for (i = 0; i < tamvCONF; i++)
        free(vCONF[i]);

    // Da free em todos os espacos alocados da string 'vPER'
    for (i = 0; i < tamvPER; i++)
        free(vPER[i]);


    free(vCONF);
    free(vPER);
    free(vANOconf);
    free(vANOper);
    free(vANOordem);
    free(straux);
}
