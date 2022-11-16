#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "levenshtein.h"
#include <ctype.h>

#define TAMSTRING 512
#define QUANT_PESQ 2


// Funcao que verifica os caracteres anteriores aos titulos de acordo com a chave passada.
// Retorna 1 se os caracteres sao a chave, e 0 senao 
int eh_titulo(FILE* arq, char c, char* chave)
{
    int i = 0;
    int tam_chave = strlen(chave);

    //printf("%s\n", chave);    

    // Verifica se os caracteres sao iguais aos da chave passada
    while (i < tam_chave)
    {
        if (c != chave[i])
            return 0;

        c = fgetc(arq);
        //printf("%c", c);
        i++;
    }

    // Se chegou ate aqui, eh porque achou o titulo do artigo             
    return 1;
}

void nomePesquisador(FILE* arq, char* str)
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
            c = fgetc(arq);

            // Le ate chegar no fim das aspas duplas
            while (c != '\"')
            {
                // Concatenando caracter por caracter na string 'pesquisador'
                strncat(str, &c, 1);

                // Pega o proximo caracter
                c = fgetc(arq);
            }

            achou = 1;
        }

        c = fgetc(arq);
    }

    // Depois de armazenado o nome do pesquisador, volta do inicio
    rewind(arq);
}

/*void coletarTitulos(FILE* arq, char** vPER, int *tamvPER, char** vCONF, int *tamvCONF, int* vANOper, int *tamvANOper, int* vANOconf, int *tamvANOconf)
{
    char c;
    char *str = malloc(sizeof(char) * TAMSTRING);  // String para armazenar cada nome
    char chavePER[5] = "STA=";
    char chaveANOp[15] = "ANO-DO-ARTIGO=";
    char chaveCONF[16] = "NOME-DO-EVENTO=";
    
    // Inicializa a string 'str'
    strcpy(str, "");

    c = fgetc(arq);

    while (c != EOF)
    {
        c = fgetc(arq);

        // Se estamos perto de um periodico
        if (eh_titulo(arq, c, chavePER))
        {
            c = fgetc(arq);

            // Le ate chegar no fim das aspas duplas
            while (c != '\"')
            {
                // Concatenando caracter por caracter na string do vetor de strings
                strncat(str, &c, 1);

                // Pega o proximo caracter
                c = fgetc(arq);
            }
            
            // Adiciona o nome do periodico no vetor e incrementa seu tamanho
            vPER[*tamvPER] = malloc(sizeof(char) * TAMSTRING);
            strcpy(vPER[*tamvPER], str);
            (*tamvPER)++;

            // Zera a string
            strcpy(str, "");
        }

        // Se estamos perto de uma conferencia
        else if (eh_titulo(arq, c, chaveCONF))
        {
            c = fgetc(arq);

            // Le ate chegar no fim das aspas duplas
            while (c != '\"')
            {
                // Concatenando caracter por caracter na string do vetor de strings
                strncat(str, &c, 1);

                // Pega o proximo caracter
                c = fgetc(arq);
            }
            
            // Se a string nao for vazia
            if (strcmp(str,""))
            {
                // Adiciona o nome da conferencia no vetor e incrementa seu tamanho
                vCONF[*tamvCONF] = malloc(sizeof(char) * TAMSTRING);
                strcpy(vCONF[*tamvCONF], str);
                (*tamvCONF)++;

                // Zera a string
                strcpy(str, "");
            }
        }

        // Se estamos perto de um ano de periodico, armazena o nome no vetor
        else if (eh_titulo(arq, c, chaveANOp))
        {
            c = fgetc(arq);

            // Le ate chegar no fim das aspas duplas
            while (c != '\"')
            {
                // Concatenando caracter por caracter na string do vetor de strings
                strncat(str, &c, 1);

                // Pega o proximo caracter
                c = fgetc(arq);
            }
            
            // Adiciona o nome do periodico no vetor e incrementa seu tamanho
            vANO[*tamvANO] = atoi(str);
            (*tamvANO)++;

            // Zera a string
            strcpy(str, "");
        }
    }

    free(str);
}*/

// Funcao que armazena uma linha do XML e retorna uma string a contendo
void armazena_linha(FILE* arq, char c, char* linha)
{
    char* str = malloc(sizeof(char) * 700);  // String para armazenar cada nome

    // Inicializa a string 'str'
    strcpy(str, "");
    
    while (c != '>')
    {
        // Armazena o caracter na string
        strncat(str, &c, 1);

        c = fgetc(arq);
    }

    // Armazena o '>' na string
    strncat(str, &c, 1);

    strcpy(linha, str);

    free(str);
}

void coletarTitulos2(FILE* arq, char** vPER, int *tamvPER, char** vCONF, int *tamvCONF, int* vANOper, int *tamvANOper, int* vANOconf, int *tamvANOconf)
{
    int PegouDados;
    char c;
    char* str = malloc(sizeof(char) * TAMSTRING);  // String para armazenar cada nome
    char* linha = malloc(sizeof(char) * 1024); // String reservada para o tamanho da linha (caracteres entre '<' e '>')
    char chavePER[24] = "DADOS-BASICOS-DO-ARTIGO";
    char chaveCONF[16] = "NOME-DO-EVENTO=";
    
    // Inicializa a string 'str'
    strcpy(str, "");

    // Inicializa a string 'linha'
    strcpy(linha, "");

    // FAZER AQUILO DE COLOCAR O ARQUIVO INTEIRO EM UMA STRING E DEPOIS IR FAZENDO AS LINHAS
    ////////////////////////////////////////////////////////////////////////////////////////


    c = fgetc(arq);

    /*armazena_linha(arq, c, linha);
    c = fgetc(arq);

    printf("%s\n", linha);
    printf("%c\n", c);*/

    while (c != EOF)
    {
        armazena_linha(arq, c, linha);
        c = fgetc(arq);
        printf("%s\n", linha);
    }

    /*while (c != EOF)
    {
        // Se estamos perto de um periodico
        if (strstr(linha, "DADOS-BASICOS-DO-ARTIGO"))
        {
            printf("A\n");
            /*PegouDados = 0;
            while(!PegouDados)
            {
                c = fgetc(arq);

                // Se estamos perto de um ano de periodico, armazena o nome no vetor
                if(eh_titulo(arq, c, "ANO-DO-ARTIGO="))
                {
                    c = fgetc(arq);

                    // Le ate chegar no fim das aspas duplas
                    while (c != '\"')
                    {
                        // Concatenando caracter por caracter na string do vetor de strings
                        strncat(str, &c, 1);

                        // Pega o proximo caracter
                        c = fgetc(arq);
                    }
            
                    // Adiciona o nome do periodico no vetor e incrementa seu tamanho
                    vANOper[*tamvANOper] = atoi(str);
                    (*tamvANOper)++;

                    // Zera a string
                    strcpy(str, "");
                }

                // Esta perto do titulo do periodico
                else if (eh_titulo(arq, c, "STA="))
                {
                    c = fgetc(arq);

                    // Le ate chegar no fim das aspas duplas
                    while (c != '\"')
                    {
                        // Concatenando caracter por caracter na string do vetor de strings
                        strncat(str, &c, 1);

                        // Pega o proximo caracter
                        c = fgetc(arq);
                    }
            
                    // Adiciona o nome do periodico no vetor e incrementa seu tamanho
                    vPER[*tamvPER] = malloc(sizeof(char) * TAMSTRING);
                    strcpy(vPER[*tamvPER], str);
                    (*tamvPER)++;

                    // Zera a string
                    strcpy(str, "");

                    PegouDados = 1;
                }
            }
        }*/

        /*// Se estamos perto de uma conferencia
        else if (eh_titulo(arq, c, "OS-DA-PARTICIPACAO-EM-C") || eh_titulo(arq, c, "OS-DA-PARTICIPACAO-EM-S") || eh_titulo(arq, c, "OS-DA-PARTICIPACAO-EM-E"))
        {
            PegouDados = 0;
            while(!PegouDados)
            {
                c = fgetc(arq);

                // Se estamos perto de um ano de periodico, armazena o nome no vetor
                if(eh_titulo(arq, c, "ANO="))
                {
                    c = fgetc(arq);

                    // Le ate chegar no fim das aspas duplas
                    while (c != '\"')
                    {
                        // Concatenando caracter por caracter na string do vetor de strings
                        strncat(str, &c, 1);

                        // Pega o proximo caracter
                        c = fgetc(arq);
                    }
            
                    // Adiciona o nome do periodico no vetor e incrementa seu tamanho
                    vANOconf[*tamvANOconf] = atoi(str);
                    (*tamvANOconf)++;

                    // Zera a string
                    strcpy(str, "");
                }

                // Esta perto do titulo do periodico
                else if (eh_titulo(arq, c, "NOME-DO-EVENTO="))
                {
                    c = fgetc(arq);

                    // Le ate chegar no fim das aspas duplas
                    while (c != '\"')
                    {
                        // Concatenando caracter por caracter na string do vetor de strings
                        strncat(str, &c, 1);

                        // Pega o proximo caracter
                        c = fgetc(arq);
                    }
            
                    // Adiciona o nome do periodico no vetor e incrementa seu tamanho
                    vCONF[*tamvCONF] = malloc(sizeof(char) * TAMSTRING);
                    strcpy(vCONF[*tamvCONF], str);
                    (*tamvCONF)++;

                    // Zera a string
                    strcpy(str, "");

                    PegouDados = 1;
                }
            }
        }*/
    //}

    free(str);
    free(linha);
}

// Funcao que transforma todas as strings do vetor em letras maiusculas
void paraMaiusculo(char** v, int tamv)
{
    int ind, indV; 
    char* straux = malloc (sizeof(char) * TAMSTRING);

    // Necessario passar os nomes das strings para letras maiusculas
    for (indV = 0; indV < tamv; indV++)
    { 
        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v[indV]);

        // Zera o indice que apontara para os caracteres da string
        ind = 0;

        // Varrendo a string ate ela chegar no final
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
char* substituiPalavra(char** strf, const char* str, const char* Pvelha, const char* Pnova) 
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

void corrigirNomes(char** v, int tamv, char* chave)
{   
    int i;

    for (i = 0; i < tamv; i++)
    {
        // Se tiver um '&amp;', substitui por '&'
        if (strstr(v[i], "&amp;"))
            substituiPalavra(&v[i], v[i], "&amp;", "&");
    }

    if (!strcmp(chave, "per"))
        paraMaiusculo(v, tamv);
}

void separarSelecionados(FILE* arq, char** v, int tamv)
{
    int indV;
    int ind;
    char linha[TAMSTRING];
    int NaLista;

    fgets(linha, TAMSTRING, arq);

    // Varrendo todos os titulos encontrados
    for (indV = 0; indV < tamv; indV++)
    {
        // Zera a flag 'NaLista'
        NaLista = 0;

        // Zera o indice
        ind = 0;

        // Varrendo todo o arquivo com os titulos
        while (!feof(arq))
        {
            //printf("%s\n", linha);
            // Se os nomes forem iguais, adiciona o nivel no final de 'v[i]'
            // Aqui que eu adicionaria a funcao edit dist ---------------------------------------------------------------->
            // Utilizar o tamanho da string tambem, se a do arquivo tiver tamanho 10,
            // mas precisar de 10 edicoes, eh a string inteira, mas se ela tiver tamanho 100,
            // e precisar de 10, ja sao poucas edicoes
            if (strstr(linha, v[indV]))
            {
                while (linha[ind] != '\0')
                    ind++;

                // Se o nivel nao for C, armazena dois caracteres
                if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ') 
                {
                    // Concatenando o espaço em branco e os dois caracteres na string
                    strncat(v[indV], &linha[ind - 4], 1);
                    strncat(v[indV], &linha[ind - 3], 1);
                    strncat(v[indV], &linha[ind - 2], 1);
                    NaLista = 1;
                }

                // Se for, armazena um
                else 
                {
                    // Concatenando o espaço
                    strncat(v[indV], &linha[ind - 3], 1);

                    // Aqui precisamos fazer '[ind - 2]' porque a linha alem de conter
                    // o '\0', tambem contem o '\n' 
                    strncat(v[indV], &linha[ind - 2], 1);

                    NaLista = 1;
                }

                // Se ja achou e catalogou, sai do loop
                break;
            }

            fgets(linha, TAMSTRING, arq);
        }

        // Se o nome nao estiver na lista, cataloga como 'C'
        if (!NaLista)
            strcat(v[indV], " C-");

        rewind(arq);
    }
}

// Funcao que divide a distancia pelo tamanho da string a ser comparada,
// retorna no minimo 0 (nenhuma edicao necessaria)
double dist_relativaMIN(char* linha, int distEdit)
{
    int ind = 0, i = 0;
    int tamstr;
    double result;

    while(linha[ind] != '\0')
        ind++;

    // Se o ultimo nivel nao for C, para tirar o nivel da string, precisamos
    // que i == 4
    if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ') 
        i = 4;
    // Se nao, i precisa ser == 3
    else
        i = 3;

    tamstr = strlen(linha) - i;

    result = (double) distEdit/tamstr;

    return result;
}

/*void separarSelecionadosDIST(FILE* arq, char** v, int tamv)
{

    // ANOTACOES

    // periodicos, a maioria encontra no arquivo com o mesmo nome,
    // por isso a distancia relativa sempre acaba com um valor pequeno

    // conferencias, a maioria encontra no arquivo com um nome diferente, 
    // abreviacoes e etc, alguns com uma distancia relativa enorme


    int indV;
    int ind, lim = 0;
    double x;
    int distEdit = 0;
    char linha[TAMSTRING];
    int NaLista;
    double dist_min = 0.28;

    fgets(linha, TAMSTRING, arq);

    // Varrendo todos os titulos encontrados
    for (indV = 0; indV < tamv; indV++)
    {
        // Zera a flag 'NaLista'
        NaLista = 0;

        // Zera o indice
        ind = 0;

        // Varrendo todo o arquivo com os titulos
        while (!feof(arq))
        {
            // Pegando o indice do ultimo caracter
            while (linha[ind] != '\0')
                ind++;

            if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ') 
                lim = 4;
            else
                lim = 3;

            distEdit = levenshtein(linha, v[indV]);
            distEdit = distEdit - lim;          

            // Se os nomes forem iguais, adiciona o nivel no final de 'v[i]'
            // Quanto menor dist_relativaMIN eh, mais proximo eh da string
            if (dist_relativaMIN(linha, distEdit) < dist_min)
            {
                x = dist_relativaMIN(linha, distEdit);
                //printf("%s\n", v[indV]);
    
                while (linha[ind] != '\0')
                    ind++;

                // Se o nivel nao for C, armazena dois caracteres
                if (linha[ind - 2] != 'C' && linha[ind - 3] != ' ') 
                {
                    // Concatenando o espaço em branco e os dois caracteres na string
                    strncat(v[indV], &linha[ind - 4], 1);
                    strncat(v[indV], &linha[ind - 3], 1);
                    strncat(v[indV], &linha[ind - 2], 1);
                    NaLista = 1;
                }

                // Se for, armazena um
                else 
                {
                    // Concatenando o espaço
                    strncat(v[indV], &linha[ind - 3], 1);

                    // Aqui precisamos fazer '[ind - 2]' porque a linha alem de conter
                    // o '\0', tambem contem o '\n' 
                    strncat(v[indV], &linha[ind - 2], 1);

                    NaLista = 1;
                }

                //printf("do arquivo -> %s", linha);
                //printf("distancia -> %d\n", distEdit);
                //printf("distancia relativa --> %lf\n\n", x);

                // Se ja achou e catalogou, sai do loop
                break;
            }

            // Zera o indice
            ind = 0;

            fgets(linha, TAMSTRING, arq);
        }

        // Se o nome nao estiver na lista, cataloga como 'C'
        if (!NaLista)
            strcat(v[indV], " C-");

        rewind(arq);
    }


}
*/