#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ARQUIVO "curriculoMENOTTI.xml"
#define ARQUIVO2 "qualis-periodicos.txt"
#define TAMSTRING 700
#define CHAVE "STA="

// "qualis-conf.txt"

// CONFERENCIAS "NOME-DO-EVENTO="
// PERIODICOS "STA="


/*
    Contabilização da produção científica em termos de
    artigos científicos publicados em periódicos e 
    conferências de um grupo de pesquisadores por meio
    da análise de seus currículos Lattes.
    
    Desenvolvido por Pedro Amaral Chapelin
    Data de finalização -> XX/XX/XXXX
    


    os nao presentes na lista marquei como 'C-'

    FAZER A SAIDA FICAR IGUAL NO SITE

    Fazer um loop que imprimira os nomes de acordo com a ordem dos niveis.
    primeiro a1
    depois a2
    .
    .
    .
    C
    Dentro do loop, fazer outro loop que vai percorrer o vetor geral, um a um nome,
    verificar seu nivel, se for o certo, se ja nao estiver la, copiar seu nome para
    outro vetor, imprimir o nome, e dai contar quantas vezes apareceu no vetor geral.

    
    //////////////
    LIDAR COM CARACTERES ESPECIAIS QUE NAO FUNCIONAM NA FUNCAO TOUPPER (Ç, ^, ~ ETC)
    //////////////

*/

/*
 Funcao que verifica os caracteres anteriores aos titulos dos periodicos
 ou das conferencias de acordo com a chave passada.
 Retorna 1 se todos foram encontrados, e 0 senao 
*/
int eh_titulo(char c, FILE* arq, char* chave)
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

// Funcao que imprime um vetor
void imprime_vetor(char** v, int tamv)
{
    int i;

    for (i = 0; i < tamv; i++)
        printf("%s\n", v[i]);
}

// Funcao interna que verifica se uma string pertence a um vetor de strings
// retorna 1 se achou e 0 se nao
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

// Funcao que imprime conferencias e periodicos catalogados no nivel C
void imprime_tudoC(char** v, int tamv)
{
    int i, k, ind, tamv_aux = 0;
    char* straux = malloc(sizeof(char) * TAMSTRING);
    char** v_aux = malloc(sizeof(char*) * TAMSTRING);

    printf("\n---------- Todos os periodicos e eventos classificados em nivel C ----------\n\n");

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
        if (straux[ind - 1] == 'C')
        {
            if (!achou(v_aux, tamv_aux, straux))
            {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito
                while (k < (ind - 1))
                {
                    printf("%c", straux[k]);
                    k++;

                    // Adiciona o nome no v_aux e incrementa seu
                    v_aux[tamv_aux] = malloc(sizeof(char) * TAMSTRING);
                    strcpy(v_aux[tamv_aux], straux);
                    tamv_aux++;

                    // Zera a string
                    strcpy(straux, "");
                }

                printf("\n");
            }
        }

        if (straux[ind - 1] == '-')
        {
            if (!achou(v_aux, tamv_aux, straux))
            {
                k = 0;

                // Imprimindo a string toda menos o nivel nela escrito
                while (k < (ind - 2))
                {
                    printf("%c", straux[k]);
                    k++;

                    // Adiciona o nome no v_aux e incrementa seu
                    v_aux[tamv_aux] = malloc(sizeof(char) * TAMSTRING);
                    strcpy(v_aux[tamv_aux], straux);
                    tamv_aux++;

                    // Zera a string
                    strcpy(straux, "");
                }

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
  
// Funcao que substitui uma dada palavra de uma string por outra
// MUDAR PARAMETROS, FAZER ENTRAR UMA STRING POR REFERENCIA SO AO INVES DE UMA DE ENTRADA E UMA DE SAIDA
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

// Funcao que corrige nomes de alguns periodicos, caracteres especiais e etc
// Por exemplo o caracter '&' eh escrito como '&amp;'
void corrigirNomes(char** v, int tamv)
{   
    int i;

    for (i = 0; i < tamv; i++)
    {
        // Se tiver um '&amp', substitui por '&'
        if (strstr(v[i], "&amp;"))
            substituiPalavra(&v[i], v[i], "&amp;", "&");
    }

    paraMaiusculo(v, tamv);
}

// Funcao que imprime os periodicos de acordo com seus niveis
// niveis: A1, A2, A3, A4, B1, B2, B3, B4 e C.
void separarSelecionados(char** v, int tamv, FILE* arq2)
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

                break;
            }

            fgets(linha, TAMSTRING, arq2);
        }

        // Se o nome nao estiver na lista, cataloga como 'C'
        if (!NaLista)
            strcat(v[indV], " C-");

        rewind(arq2);
    }

    imprime_vetor(v, tamv);

    //free(linha);
}

// Funcao que imprime os periodicos de acordo com seus niveis
void imprimeCatalogados(char** v, int tamv)
{
    int i, j, k, ind = 0;
    char niveis[20][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3" ,"B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);

    /*for (i = 0; i < 9; i++)
        char[i] = malloc(sizeof(char) * 3);*/

    printf("\n\n ---------- Ordem de periodicos, discriminando os estratos. ----------\n");

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

    /*for (i = 0; i < 9; i++)
        free(niveis[i]);

    free(niveis);*/
    free(straux);
}

/*/ Funcao que imprime quantos periodicos o autor possui de cada nivel
void imprimeQuantPeriodicos(char** v, int tamv)
{
    // Indices do vetor niveis:
    // [0] - A1 / [1] - A2 / [2] - A3 / [3] - A4 / [4] - B1 / [5] - B2 / [6] - B3 / [7] - B4 / [8] - C  
    char niveis[20][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3" , "B4", "C"};
    char* straux = malloc(sizeof(char) * TAMSTRING);
    int* quantlvl = malloc(sizeof(int) * 9);
    int i, j, ind = 0;

    // Inicializando vetor de quantidade de periodicos com tais niveis
    for (i = 0; i < 9; i++)
        quantlvl[i] = 0;

    for (i = 0; i < tamv; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (strstr(v[i], niveis[j]))
                quantlvl[j]++;
        }

        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v[i]);

        // Pega o indice do fim da string
        while (straux[ind] != '\0')
            ind++;

        if (straux[ind - 1] == 'C')
            quantlvl[j]++;

        ind = 0;  
    }

    printf("O pesquisador x possui:\n\n");

    for (i = 0; i < 9; i++)
    {
        if (quantlvl[i] == 1)
            printf("--> %d periodico catalogado em %s\n", quantlvl[i], niveis[i]);
        else
            printf("--> %d periodicos catalogados em %s\n", quantlvl[i], niveis[i]);
    }   

    free(straux);
    free(quantlvl);
}*/

// Funcao que imprime a quantidade de periodicos na tela, separados por niveis
void pegaDados(FILE* arq)
{
    FILE* arq2;
    char c;
    int tamv = 0, i;
    char *str = malloc(sizeof(char) * TAMSTRING);  // String para armazenar cada nome de periodico
    char** v = malloc(sizeof(str) * 512);  // Aloca vetor de strings para 100 strings
    char *pesquisador = malloc(sizeof(char) * TAMSTRING); // String para armazenar o nome do pesquisador

    // Pega o primeiro caracter do arquivo
    c = fgetc(arq);

    // Inicializa a string 'str'
    strcpy(str, "");

    // Inicializa a string 'pesquisador'
    strcpy(pesquisador, "");

    // Enquanto nao chegou no final do arquivo, faz 
    while (c != EOF)
    {   
        // Se estamos perto do nome do pesquisador, o armazena
        if (eh_titulo(c, arq, "S NOME-COMPLETO="))
        {
            c = fgetc(arq);

            // Le ate chegar no fim das aspas duplas
            while (c != '\"')
            {
                // Concatenando caracter por caracter na string 'pesquisador'
                strncat(pesquisador, &c, 1);

                // Pega o proximo caracter
                c = fgetc(arq);
            }
        }

        // Se estamos perto de um titulo, armazena o periodico
        if (eh_titulo(c, arq, CHAVE))
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
            v[tamv] = malloc(sizeof(char) * TAMSTRING);
            strcpy(v[tamv], str);
            tamv++;

            // Zera a string
            strcpy(str, "");
        }

        c = fgetc(arq);
    }

    //imprime_vetor(v, tamv);

    //printf("\n\n\n\n");

    corrigirNomes(v, tamv);

    imprime_vetor(v, tamv);

    printf("\n\n\n\n");

    // Abre o arquivo contendo os periodicos classificados
    arq2 = fopen(ARQUIVO2, "r");

    // Testa se o arquivo abre
    if (arq2 == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    //verificaLVL_e_imprime(v, tamv, arq2);

    separarSelecionados(v, tamv, arq2);

    //imprime_vetor(v, tamv);

    //imprimeQuantPeriodicos(v, tamv);

    printf("Pesquisador: %s\n", pesquisador);
    printf("------------------------------------------\n\n");

    imprimeCatalogados(v, tamv);

    //imprime_vetor(v, tamv);

    imprime_tudoC(v, tamv);

    //imprime_NaoClassificados(v, tamv);

    // Da free em todos os espacos alocados da string 'v'
    for (i = 0; i < tamv; i++)
        free(v[i]);

    free(str);
    free(v);
    free(pesquisador);

    // Fecha o segundo arquivo
    fclose(arq2);
}

int main (int argc, char** argv)
{
    FILE* arqXML, arqPER, arqCONF;
    DIR* dir;
    char* nome_arqPER, nome_arqCONF, nome_dir;
    int opt;

    while ((opt = getopt(argc, argv, "d:c:p:")) != -1) 
    {
        switch (opt) 
        {
         case 'd':
            printf("opcao d tem arg: %s\n", optarg);
            nome_dir = malloc(sizeof(char) * strlen(optarg));
            strcpy(nome_dir, "");
            strcpy(nome_dir, optarg);            
            break;

         case 'c':
            printf("opcao c tem arg: %s\n", optarg);
            nome_arqCONF = malloc(sizeof(char) * strlen(optarg));
            strcpy(nome_arqCONF, "");
            strcpy(nome_arqCONF, optarg);
            break;

         case 'p':
            printf("opcao p tem arg: %s\n", optarg);
            nome_arqPER = malloc(sizeof(char) * strlen(optarg));
            strcpy(nome_arqPER, "");
            strcpy(nome_arqPER, optarg);
            break;
        }
    }

    // Abre o arquivo
    arqXML = fopen(ARQUIVO, "r");

    // Testa se o arquivo abre
    if (arqXML == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }
    
    pegaDados(arqXML);

    fclose(arqXML);

    free(nome_dir);
    free(nome_arqCONF);
    free(nome_arqPER);
    return 0;
}
