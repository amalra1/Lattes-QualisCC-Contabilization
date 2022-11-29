#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "levenshtein.h"
#include "libcoleta.h"
#include <ctype.h>

#define TAMSTRING 512
#define PER_MAX 600
#define CONF_MAX 1000

void inicia_pesquisador(pesquisador_t *p)
{
    p->nome = malloc(sizeof(char) * 64);
    strcpy(p->nome, "");

    p->vPER = malloc(sizeof(char*) * PER_MAX);
    p->vPERorg = malloc(sizeof(char*) * PER_MAX);
    p->tamvPER = 0;

    p->vCONF =  malloc(sizeof(char*) * CONF_MAX);
    p->tamvCONF = 0;

    p->vANOper = malloc(sizeof(int) * PER_MAX);
    p->tamvANOper = 0;

    p->vANOconf =  malloc(sizeof(char*) * CONF_MAX);
    p->tamvANOconf = 0;
}

void destroi_pesquisador(pesquisador_t *p)
{
    int i;

    free(p->nome);

    // Dar free em cada string dos vetores de strings

    for (i = 0; i < p->tamvPER; i++)
        free(p->vPER[i]);
    free(p->vPER);

    for (i = 0; i < p->tamvPER; i++)
        free(p->vPERorg[i]);
    free(p->vPERorg);

     for (i = 0; i < p->tamvCONF; i++)
        free(p->vCONF[i]);
    free(p->vCONF);

    free(p->vANOper);
    free(p->vANOconf);
}

// Funcao que verifica se os caracteres atualmente lidos no arquivo sao iguais a chave passada
// Retorna 1 se os caracteres sao a chave, e 0 senao 
int eh_titulo(FILE* arq, char c, char* chave)
{
    int i = 0, pos;
    int tam_chave = strlen(chave);

    // Pega a posicao atual do arquivo
    pos = ftell(arq);    

    // Verifica se os caracteres sao iguais aos da chave passada
    while (i < tam_chave)
    {
        if (c != chave[i])
        {
            // Volta para o inicio da frase e retorna
            fseek(arq, pos, SEEK_SET);

            return 0;
        }

        c = fgetc(arq);
        i++;
    }

    // Volta para o inicio da frase
    fseek(arq, pos, SEEK_SET);

    // Se chegou ate aqui, eh porque achou o titulo do artigo             
    return 1;
}

void nomePesquisador(FILE* arq, pesquisador_t *p)
{
    char c;
    int achou = 0;

    c = fgetc(arq);

    // Enquanto nao chegou no final do arquivo, faz 
    while (!achou)
    {
        // Se estamos perto do nome do pesquisador, o armazena
        if (eh_titulo(arq, c, "S NOME-COMPLETO="))
        {
            // Ate chegar no começo das aspas duplas
            while (c != '\"')
                c = fgetc(arq);

            c = fgetc(arq);

            // Le ate chegar no fim das aspas duplas
            while (c != '\"')
            {
                // Concatenando caracter por caracter na string 'pesquisador'
                strncat(p->nome, &c, 1);

                c = fgetc(arq);
            }

            achou = 1;
        }

        c = fgetc(arq);
    }

    // Depois de armazenado o nome do pesquisador, volta do inicio
    rewind(arq);
}

void coletarTitulos(FILE* arq, pesquisador_t *p)
{
    int PegouDados;
    char c;
    char* str = malloc(sizeof(char) * TAMSTRING);  // String auxiliar para armazenar cada nome
    
    // Inicializa a string 'str'
    strcpy(str, "");

    c = fgetc(arq);

    while (c != EOF)
    {
        // Se estamos perto de um periodico, entra na tag que contem os dados
        if (eh_titulo(arq, c, "DADOS-BASICOS-DO-ARTIGO"))
        {
            PegouDados = 0;
            while(!PegouDados)
            {
                c = fgetc(arq);

                if(eh_titulo(arq, c, "ANO-DO-ARTIGO="))
                {
                    // Ate chegar no começo das aspas duplas
                    while (c != '\"')
                        c = fgetc(arq);

                    // Pega o primeiro caracter dentro das aspas                        
                    c = fgetc(arq);

                    // Le ate chegar no fim das aspas duplas
                    while (c != '\"')
                    {
                        // Concatenando caracter por caracter na string auxilair
                        strncat(str, &c, 1);

                        c = fgetc(arq);
                    }
            
                    // Adiciona ano do periodico no vetor e incrementa seu tamanho
                    p->vANOper[p->tamvANOper] = atoi(str);
                    p->tamvANOper++;

                    // Zera a string
                    strcpy(str, "");
                }

                // Esta perto do titulo do periodico
                if (eh_titulo(arq, c, "STA="))
                {
                    while (c != '\"')
                        c = fgetc(arq);

                    c = fgetc(arq);

                    while (c != '\"')
                    {
                        strncat(str, &c, 1);

                        c = fgetc(arq);
                    }

                    // Adiciona o nome do periodico no vetor e incrementa seu tamanho
                    p->vPER[p->tamvPER] = malloc(sizeof(char) * TAMSTRING);
                    p->vPERorg[p->tamvPER] = malloc(sizeof(char) * TAMSTRING);
                    strcpy(p->vPER[p->tamvPER], str);
                    strcpy(p->vPERorg[p->tamvPER], str);
                    p->tamvPER++;

                    // Zera a string
                    strcpy(str, "");

                    PegouDados = 1;
                }
            }
        }

        // Se estamos perto de uma conferencia
        else if (eh_titulo(arq, c, "TRABALHO-EM-EVENTOS "))
        {
            PegouDados = 0;
            while(!PegouDados)
            {
                c = fgetc(arq);

                // Se estamos perto do ano, armazena o no vetor de ano
                if(eh_titulo(arq, c, "ANO-DO-TRABALHO="))
                {
                    while (c != '\"')
                        c = fgetc(arq);
                      
                    c = fgetc(arq);

                    while (c != '\"')
                    {
                        strncat(str, &c, 1);

                        c = fgetc(arq);
                    }
            
                    // Adiciona ano da conferencia no vetor e incrementa seu tamanho
                    p->vANOconf[p->tamvCONF] = atoi(str);
                    p->tamvANOconf++;

                    // Zera a string
                    strcpy(str, "");
                }

                // Esta perto do titulo da conferencia
                if (eh_titulo(arq, c, "NOME-DO-EVENTO="))
                {
                    while (c != '\"')
                        c = fgetc(arq);
                        
                    c = fgetc(arq);

                    while (c != '\"')
                    {
                        strncat(str, &c, 1);

                        c = fgetc(arq);
                    }
            
                   
                    // Adiciona o nome da conferencia no vetor e incrementa seu tamanho
                    p->vCONF[p->tamvCONF] = malloc(sizeof(char) * TAMSTRING);
                    strcpy(p->vCONF[p->tamvCONF], str);
                    p->tamvCONF++;

                    // Zera a string
                    strcpy(str, "");

                    PegouDados = 1;
                }
            }
        }

        c = fgetc(arq);
    }

    free(str);
}

// Funcao que transforma todas as strings do vetor em letras maiusculas
void paraMaiusculo(char** v, int tamv)
{
    int ind, indV; 
    char* straux = malloc (sizeof(char) * TAMSTRING);

    for (indV = 0; indV < tamv; indV++)
    { 
        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v[indV]);

        // Zera o indice que apontará para os caracteres da string
        ind = 0;

        // Varrendo a string ate ela chegar no ultimo indice
        while (straux[ind] != '\0')
        {
            // Elevando o caracter para caixa alta com 'toupper'
            straux[ind] = toupper(straux[ind]);
            ind++;
        }

        // Transfere o auxiliar de volta para o vetor de strings
        strcpy(v[indV], straux);
        strcpy(straux, "");
    }

    free(straux);
}

// Funcao que substitui uma dada palavra de uma string por outra
char* substituiPalavra(char** strf, char* str, const char* Pvelha, const char* Pnova) 
{ 
    char* stringf; 
    int i, cnt = 0; 
    int tam_Pnova = strlen(Pnova); 
    int tam_Pvelha = strlen(Pvelha); 
  
    // Contando quantas vezes 'Pvelha' aparece na string 'str' 
    for (i = 0; str[i] != '\0'; i++) 
    { 
        if (strstr(&str[i], Pvelha) == &str[i]) 
        { 
            cnt++; 
  
            // Indo para o indice logo apos 'Pvelha' 
            i = i + tam_Pvelha - 1; 
        } 
    } 
  
    // Criando a string final 'stringf' com o tamanho adequado 
    stringf = (char*)malloc(i + cnt * (tam_Pnova - tam_Pvelha) + 1); 
  
    i = 0; 
    while (*str) 
    { 
        // Compara a substring com o resultado 
        if (strstr(str, Pvelha) == str) 
        { 
            strcpy(&stringf[i], Pnova); 
            i = i + tam_Pnova; 
            str = str + tam_Pvelha; 
        } 
        else
            stringf[i++] = *str++; 
    } 
  
    stringf[i] = '\0';
    strcpy(*strf, stringf);
    free(stringf);
    return *strf;
}

void corrigirNomes(pesquisador_t *p)
{   
    int i;

    // Corrigindo nomes de periodicos
    for (i = 0; i < p->tamvPER; i++)
    {   
        // Se tiver um '&amp;', substitui por '&'
        if (strstr(p->vPER[i], "&amp;"))
            substituiPalavra(&(p->vPER[i]), p->vPER[i], "&amp;", "&");
    }

    // Corrigindo os nomes originais dos periodicos
    for (i = 0; i < p->tamvPER; i++)
    {   
        if (strstr(p->vPERorg[i], "&amp;"))
            substituiPalavra(&(p->vPERorg[i]), p->vPERorg[i], "&amp;", "&");
    }

    // Alterando os nomes dos periodicos para todas maiusculas
    paraMaiusculo(p->vPER, p->tamvPER);

    // Corrigindo o nome das conferencias
    for (i = 0; i < p->tamvCONF; i++)
    {
        if (strstr(p->vCONF[i], "&amp;"))
            substituiPalavra(&(p->vCONF[i]), p->vCONF[i], "&amp;", "&");
    }
}

// Funcao que divide a distancia pelo tamanho da string a ser modificada
// retorna no minimo 0 (nenhuma edicao necessaria)
double dist_relativaMIN(int distEdit, int tamstr)
{
    double result;

    result = (double) distEdit/tamstr;

    return result;
}

void catalogarCONFS(FILE* arq, pesquisador_t *p, double dist_min)
{
    // ANOTACOES

    // O numero ABSURDO de allocs na memoria depois de passar por 
    // essa funcao eh devido a funcao do levenshtein

    // periodicos, a maioria encontra no arquivo com o mesmo nome,
    // por isso a distancia relativa sempre acaba com um valor pequeno

    // conferencias, a maioria encontra no arquivo com um nome diferente, 
    // abreviacoes e etc, alguns com uma distancia relativa enorme


    int indV, j;
    double x;
    int ind;
    int distEdit = 0;
    char* straux = malloc(sizeof(char) * 512);
    char linha[TAMSTRING];
    int NaLista;

    fgets(linha, TAMSTRING, arq);

    // Varrendo todos os titulos encontrados
    for (indV = 0; indV < p->tamvCONF; indV++)
    {
        // Zera a flag 'NaLista'
        NaLista = 0;

        // Zera o indice
        ind = 0;

        // Varrendo todo o arquivo com os titulos
        while (!feof(arq))
        {
            // Zera a string auxiliar
            strcpy(straux, "");

            // Pegando o indice do ultimo caracter
            while (linha[ind] != '\0')
                ind++;

            // Pega a string do arquivo mas deixa ela sem o nivel e o '\n'
            if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ')
            {
                for(j = 0; j < (ind - 4); j++)
                    strncat(straux, &linha[j], 1);
            } 
            else
            {
                for(j = 0; j < (ind - 3); j++)
                    strncat(straux, &linha[j], 1);
            }

            distEdit = levenshtein(straux, p->vCONF[indV]);          

            /*x = dist_relativaMIN(straux, distEdit, strlen(p->vCONF[indV]) - 1);

            if (x < dist_min)
            {
            printf("%s\n", p->vCONF[indV]);
            printf("do arquivo -> %s", linha);
            printf("reduzida do arq -> %s\n", straux);
            printf("distancia -> %d\n", distEdit);
            printf("distancia relativa --> %lf\n\n", x);
            }*/

            x = dist_relativaMIN(distEdit, strlen(straux));

            // Se os nomes forem iguais, adiciona o nivel no final de 'v[i]'
            // Quanto menor dist_relativaMIN eh, mais proximo eh da string
            if (x < dist_min)
            {
                while (linha[ind] != '\0')
                    ind++;

                // Se o nivel nao for C, armazena dois caracteres
                if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ') 
                {
                    // Concatenando o espaço em branco e os dois caracteres na string
                    strncat(p->vCONF[indV], &linha[ind - 4], 1);
                    strncat(p->vCONF[indV], &linha[ind - 3], 1);
                    strncat(p->vCONF[indV], &linha[ind - 2], 1);
                    NaLista = 1;
                }

                // Se for, armazena um
                else 
                {
                    // Concatenando o espaço
                    strncat(p->vCONF[indV], &linha[ind - 3], 1);

                    // Aqui precisamos fazer '[ind - 2]' porque a linha alem de conter
                    // o '\0', tambem contem o '\n' 
                    strncat(p->vCONF[indV], &linha[ind - 2], 1);

                    NaLista = 1;
                }

                // Se ja achou e catalogou, sai do loop
                break;
            }

            // Zera o indice
            ind = 0;

            fgets(linha, TAMSTRING, arq);
        }

        // Se o nome nao estiver na lista, cataloga como 'C'
        if (!NaLista)
            strcat(p->vCONF[indV], " C-");

        rewind(arq);
    }

    free(straux);
}

void catalogarPERS(FILE* arq, pesquisador_t *p, double dist_min)
{
    int indV, j;
    int ind;
    double x;
   //double y;
    int distEdit = 0;
    char* straux = malloc(sizeof(char) * 512);
    char linha[TAMSTRING];
    int NaLista;

    fgets(linha, TAMSTRING, arq);

    // Varrendo todos os titulos encontrados
    for (indV = 0; indV < p->tamvPER; indV++)
    {
        // Zera a flag 'NaLista'
        NaLista = 0;

        // Zera o indice
        ind = 0;

        // Varrendo todo o arquivo com os titulos
        while (!feof(arq))
        {
            // Zera a string auxiliar
            strcpy(straux, "");

            // Pegando o indice do ultimo caracter
            while (linha[ind] != '\0')
                ind++;

            // Pega a string do arquivo mas deixa ela sem o nivel e o '\n'
            if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ')
            {
                for(j = 0; j < (ind - 4); j++)
                    strncat(straux, &linha[j], 1);
            } 
            else
            {
                for(j = 0; j < (ind - 3); j++)
                    strncat(straux, &linha[j], 1);
            }

            distEdit = levenshtein(straux, p->vPER[indV]);

            //y = dist_relativaMIN(straux, distEdit, strlen(p->vPER[indV]) - 1);

            /*if (x < dist_min)
            {
            printf("%s\n", p->vPER[indV]);
            printf("do arquivo -> %s", linha);
            printf("reduzida do arq -> %s\n", straux);
            printf("distancia -> %d\n", distEdit);
            printf("distancia relativa com o tamstring do pesquisador --> %lf\n\n", x);
            printf("distancia relativa com o tamstring do arquivo --> %lf\n\n", y);
            }*/          

            x = dist_relativaMIN(distEdit, strlen(straux));

            // Se os nomes forem iguais, adiciona o nivel no final de 'v[i]'
            // quanto menor dist_relativaMIN eh, mais proximo eh da string
            if (x < dist_min)
            {
                while (linha[ind] != '\0')
                    ind++;

                // Se o nivel nao for C, armazena dois caracteres
                if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ') 
                {
                    // Concatenando o espaço em branco e os dois caracteres na string
                    strncat(p->vPER[indV], &linha[ind - 4], 1);
                    strncat(p->vPER[indV], &linha[ind - 3], 1);
                    strncat(p->vPER[indV], &linha[ind - 2], 1);
                    NaLista = 1;
                }

                // Se for, armazena um
                else 
                {
                    // Concatenando o espaço
                    strncat(p->vPER[indV], &linha[ind - 3], 1);

                    // Aqui precisamos fazer '[ind - 2]' porque a linha alem de conter
                    // o '\0', tambem contem o '\n' 
                    strncat(p->vPER[indV], &linha[ind - 2], 1);

                    NaLista = 1;
                }

                // Se ja achou e catalogou, sai do loop
                break;
            }

            // Zera o indice
            ind = 0;

            fgets(linha, TAMSTRING, arq);
        }

        // Se o nome nao estiver na lista, cataloga como 'C'
        if (!NaLista)
            strcat(p->vPER[indV], " C-");

        rewind(arq);
    }

    free(straux);
}
