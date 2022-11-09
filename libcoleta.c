#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funcao que verifica os caracteres anteriores aos titulos de acordo com a chave passada.
// Retorna 1 se os caracteres sao a chave, e 0 senao 
int eh_titulo(FILE* arq, char c, char* chave)
{
    int i = 0;
    int tam_chave = strlen(chave);

    // Verifica se os caracteres sao iguais aos da chave passada
    while (i < tam_chave)
    {
        if (c != chave[i])
            return 0;

        c = fgetc(arq);
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

void coletarTitulos(FILE* arq, char** v, int *tam, char* opt)
{
    char c;
    char *str = malloc(sizeof(char) * TAMSTRING);  // String para armazenar cada nome
    char *chave = malloc(sizeof(char) * TAMSTRING);

    // Inicializa a string 'chave'
    strcpy(chave, "");
    
    // Ajustando a chave de busca conforme o que foi digitado
    if (!strcmp(opt, "periodicos"))
        strcpy(chave, "STA=");

    if (!strcmp(opt, "conferencias"))
        strcpy(chave, "NOME-DO-EVENTO=");

    // Inicializa a string 'str'
    strcpy(str, "");

    c = fgetc(arq);

    while (c != EOF)
    {   
        // Se estamos perto de um titulo, armazena o nome no vetor
        if (eh_titulo(arq, c, chave))
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
            v[*tam] = malloc(sizeof(char) * TAMSTRING);
            strcpy(v[*tam], str);
            (*tam)++;

            // Zera a string
            strcpy(str, "");
        }

        c = fgetc(arq);
    }

    free(str);
    free(chave);
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

void corrigirNomes(char** vPER, int tamvPER)
{   
    int i;

    for (i = 0; i < tamvPER; i++)
    {
        // Se tiver um '&amp;', substitui por '&'
        if (strstr(vPER[i], "&amp;"))
            substituiPalavra(&vPER[i], vper[i], "&amp;", "&");
    }

    paraMaiusculo(v, tamv);
}

void separarSelecionados(FILE* arq2, char** v, int tamv)
{
    int indV;
    int ind;
    char linha[TAMSTRING];
    int NaLista;

    fgets(linha, TAMSTRING, arq2);

    // Varrendo todos os periodicos encontrados
    for (indV = 0; indV < tamv; indV++)
    {
        // Zera a flag NaLista
        NaLista = 0;

        // Zera o indice
        ind = 0;

        // Varrendo todo o arquivo com os periodicos
        while (!feof(arq2))
        {
            // Se os nomes forem iguais, adiciona o nivel no final de 'v[i]'
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

            fgets(linha, TAMSTRING, arq2);
        }

        // Se o nome nao estiver na lista, cataloga como 'C'
        if (!NaLista)
            strcat(v[indV], " C-");

        rewind(arq2);
    }
}
