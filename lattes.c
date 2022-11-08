#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#define ARQUIVO "curriculoMENOTTI.xml"
#define ARQUIVO2 "qualis-periodicos.txt"
#define ARQUIVO3 "qualis-conf.txt"
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

// Funcao que pega o nome do pesquisador e armazena na string
void nomePesquisador(FILE* arq, char* str)
{
    char c;
    int achou = 0;

    c = fgetc(arq);

    // Enquanto nao chegou no final do arquivo, faz 
    while (!achou)
    {
        // Se estamos perto do nome do pesquisador, o armazena
        if (eh_titulo(c, arq, "S NOME-COMPLETO="))
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

// Funcao que imprime conferencias e periodicos catalogados no nivel C
void imprime_tudoC(char** v, int tamv)
{
    int i, k, ind, tamv_aux = 0;
    char* straux = malloc(sizeof(char) * 512);
    char** v_aux = malloc(sizeof(char*) * TAMSTRING);
    int ult = 0;

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
        if (straux[ind - 1] == '-' || straux[ind - 1] == 'C')
        {
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

    printf("\n---------- Todos os periodicos/eventos nao classificados ----------\n\n");

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

    //imprime_vetor(v, tamv);
}

// Funcao que imprime os periodicos de acordo com seus niveis
void imprimeCatalogados(char** v, int tamv)
{
    int i, j, k, ind = 0;
    char niveis[20][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3" ,"B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);

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
    free(straux);
}

// Funcao que retorna quantas vezes uma string se repete 
// em um vetor de strings
int seRepete(char* str, char** v, int tam)
{
    int i = 0;
    int cont = 0;

    for (i = 0; i < tam; i++)
    {
        if(!strcmp(str, v[i]))
            cont++;     
    }

    return cont;
}

// Funcao que imprime a quantidade de titulos de acordo com o vetor de strings passado
void imprimeSumarizada(char** v, int tam)
{
    int i, j, k, ind = 0;
    int tamlvl = 8, tamv_aux = 0, cont = 0;
    char vlvl[20][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3", "B4"};
    char* straux = malloc(sizeof(char) * 512);
    char** v_aux = malloc(sizeof(char*) * TAMSTRING);

    // Zera a string auxiliar
    strcpy(straux, "");

    // Percorrendo de nivel a nivel
    for (i = 0; i < 8; i++)
    {
        printf("Estrato %s\n:", vlvl[i]);

        for (j = 0; j < tam; j++)
        {
            // Se o titulo corresponde ao nivel da vez
            if (strstr(vlvl[i], v[j]))
            {
                if (!achou(v_aux, tamv_aux, v[j]))
                {
                    // Copia o nome para a string auxiliar
                    strcpy(straux, v[j]);

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
                }
            }
        }
    }

    printf("Estrato C:\n");

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

                // Zera a string
                strcpy(straux, "");
            }
        }
    }

    free(straux);

    // Da free em todos os espacos alocados da string 'v_aux'
    for (i = 0; i < tamv_aux; i++)
        free(v_aux[i]);
}

// Funcao que preenche um vetor de strings com os dados escolhidos
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
        //printf("%d\n\n", eh_titulo(c, arq, chave));
        // Se estamos perto de um titulo, armazena o nome no vetor
        if (eh_titulo(c, arq, chave))
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

// Funcao que imprime a quantidade de periodicos na tela, separados por niveis
void pegaDados(FILE* arqXML, FILE* arqPER, FILE* arqCONF)
{
    int tamv_per = 0, i;
    char *str = malloc(sizeof(char) * TAMSTRING);  // String para armazenar cada nome
    char** v_per = malloc(sizeof(str) * 512);  // Aloca vetor de strings para X titulos de periodicos
    char *pesquisador = malloc(sizeof(char) * TAMSTRING); // String para armazenar o nome do pesquisador

    // Inicializa a string 'pesquisador'
    strcpy(pesquisador, "");

    nomePesquisador(arqXML, pesquisador);

    coletarTitulos(arqXML, v_per, &tamv_per, "periodicos"); // PER

    //coletarTitulos(arqXML, v_per, &tamv_per, "conferencias"); // CONF

    //imprime_vetor(v_per, tamv_per);

    //printf("\n\n\n\n");

    corrigirNomes(v_per, tamv_per);

    //imprime_vetor(v_per, tamv_per);

    //printf("\n\n\n\n");

    separarSelecionados(v_per, tamv_per, arqPER); // PER

    //separarSelecionados(v_conf, tamv-conf, arqCONF); //CONF

    //imprime_vetor(v_per, tamv_per);

    printf("\nPesquisador: %s\n", pesquisador);

    //imprimeCatalogados(v_per, tamv_per);

    imprimeSumarizada(v_per, tamv_per); //(1)

    //imprimeSumarizada(v_conf, tamv_conf); //(2)

    //imprime_vetor(v_per, tamv_per);

    imprime_tudoC(v_per, tamv_per); //(5)

    imprime_NaoClassificados(v_per, tamv_per); //(6)

    // Da free em todos os espacos alocados da string 'v'
    for (i = 0; i < tamv_per; i++)
        free(v_per[i]);

    free(str);
    free(v_per);
    free(pesquisador);
}

int main (int argc, char** argv)
{
    FILE* arqXML;/*, *arqPER, *arqCONF;
    DIR* dir;
    char* nome_arqPER;
    char* nome_arqCONF;
    char* nome_dir;
    int opt;*/

    // Inicializa as strings
    /*strcpy(nome_dir, "");
    strcpy(nome_arqCONF, "");
    strcpy(nome_arqPER, "");*/

    /*while ((opt = getopt(argc, argv, "d:c:p:")) != -1) 
    {
        switch (opt) 
        {
         case 'd':
            printf("opcao d tem arg: %s\n", optarg);
            nome_dir = malloc(sizeof(char) * strlen(optarg));
            strcpy(nome_dir, optarg);            
            break;

         case 'c':
            printf("opcao c tem arg: %s\n", optarg);
            nome_arqCONF = malloc(sizeof(char) * strlen(optarg));
            strcpy(nome_arqCONF, optarg);
            break;

         case 'p':
            printf("opcao p tem arg: %s\n", optarg);
            nome_arqPER = malloc(sizeof(char) * strlen(optarg));
            strcpy(nome_arqPER, optarg);
            break;
        }
    }*/

    // Abre o arquivo
    arqXML = fopen(ARQUIVO, "r");

    // Testa se o arquivo abre
    if (arqXML == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    // Abre o arquivo contendo os periodicos classificados
    arqPER = fopen(ARQUIVO2, "r");

    // Testa se o arquivo abre
    if (arqPER == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    // Abre o arquivo contendo as conferencias classificadas
    arqCONF = fopen(ARQUIVO3, "r");

    // Testa se o arquivo abre
    if (arqCONF == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }
    
    pegaDados(arqXML, arqPER, arqCONF);

    fclose(arqXML);
    fclose(arqPER);
    fclose(arqCONF);

    //free(nome_dir);
    //free(nome_arqCONF);
    //free(nome_arqPER);
    return 0;
}
