#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ARQUIVO "curriculoCASTILHO.xml"
#define ARQUIVO2 "qualis-periodicos.txt"
#define ARQUIVO3 "qualis-conf.txt"
#define TAMSTRING 512

/*
    Contabilização da produção científica em termos de
    artigos científicos publicados em periódicos e 
    conferências de um grupo de pesquisadores por meio
    da análise de seus currículos Lattes.
    
    Desenvolvido por Pedro Amaral Chapelin
    Data de finalização -> XX/XX/XXXX
    
    LIDAR COM CARACTERES ESPECIAIS QUE NAO FUNCIONAM NA FUNCAO TOUPPER (Ç, ^, ~ ETC)
    //////////////
    LIDAR COM PERIODICOS QUE SE REPETEM, FAZER ADICIONAR 1 SÓ NO VETOR
    //////////////
    RESOLVER O CASO DA REVISTA QUE TA COM DOIS NIVEIS NO CURRICULO DO MENOTTI
    //////////////
    CONFERENCIAS ESTAO LOGO APOS 'NOME-DO-EVENTO='
    //////////////

*/

/*
 Funcao que verifica os caracteres anteriores dos titulos dos artigosaaaaaaaaaaa
 'TITULO-DO-PERIODICO-OU-REVISTA=' sao eles, mas somente eh necessario
 testar o fim, 'STA=', em seguida vem os nomes.
 retorna 1 se todos foram encontrados, e 0 senao 
*/
int titulo_artigo(char c, FILE* arq)
{
    // Verificacoes para ver se esta na parte anterior ao nome do periodico buscado
    if (c != 'S')
        return 0;
    
    c = fgetc(arq);

    if (c != 'T')
        return 0;

    c = fgetc(arq);

    if (c != 'A')
        return 0;

    c = fgetc(arq);

    if (c != '=')
        return 0;

    // Se chegou ate aqui, eh porque achou o titulo do artigo             
    c = fgetc(arq);
    return 1;
}

// Funcao que imprime o menu de operacoes
void imprimir_menu()
{
    printf("[1] -> Apresentar periodicos de todos, catalogados pelos niveis;\n");
    printf("[2] -> Apresentar conferencias de todos, catalogadas pelos niveis;\n");
    printf("[3] -> Apresentar periodicos separados por autores, catalogados pelos niveis;\n");
    printf("[4] -> Apresentar conferencias separadas por autores, catalogadas pelos niveis;\n");
    printf("[5] -> Apresentar periodicos sumarizados do grupo por ano, catalogados pelos niveis;\n");
    printf("[6] -> Apresentar conferencias sumarizadas do grupo por ano, catalogadas pelos niveis;\n");
    printf("[7] -> Listar aqueles periódicos e eventos(conferencias) classificados no nível C;\n");
    printf("[8] -> Discriminar entre aqueles encontrados como nível C e não encontrados nas listas fornecidas.\n\n");

    printf("Atualmente, o QUALIS está dividido em 8 níveis/estratos em ordem descrente de valor {A1, A2, A3, A4, B1, B2, B3 e B4}.\n");
    printf("existe também o estrato C que é dedicado aos veículos de irrelevância ou sem classificação.\n\n\n");
}

// Funcao que imprime um vetor
void imprime_vetor(char** v_per, int tamv_per)
{
    int i;

    for (i = 0; i < tamv_per; i++)
        printf("%s\n", v_per[i]);
}

// Funcao que le uma opcao do usuario
int le_opt()
{
    int opt;

    scanf("%d", &opt);

    // Se nao for uma opcao valida, pede pra digitar de novo
    while (opt < 1 || opt > 8)
    {
        printf("Opcao invalida, digite novamente: ");
        scanf("%d", &opt);
    }

    return opt;
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
void paraMaiusculo(char** v_per, int tamv_per)
{
    int ind, ind_vper; 
    char* straux = malloc (sizeof(char) * TAMSTRING);

    // Necessario passar os nomes das strings para letras maiusculas
    for (ind_vper = 0; ind_vper < tamv_per; ind_vper++)
    { 
        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v_per[ind_vper]);

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
        strcpy(v_per[ind_vper], straux);
        strcpy(straux, "");
    }

    free(straux);
}

// Funcao que corrige nomes de alguns periodicos, caracteres especiais e etc
// Por exemplo o caracter '&' eh escrito como '&amp;'
void corrigirNomes(char** v_per, int tamv_per)
{   
    int i;

    for (i = 0; i < tamv_per; i++)
    {
        // Se tiver um '&amp', substitui por '&'
        if (strstr(v_per[i], "&amp;"))
            substituiPalavra(&v_per[i], v_per[i], "&amp;", "&");
    }

    paraMaiusculo(v_per, tamv_per);
}

// Funcao que imprime os periodicos de acordo com seus niveis
// niveis: A1, A2, A3, A4, B1, B2, B3, B4 e C.
void separarPERIODICOS(char** v_per, int tamv_per, FILE* arq2)
{
    int ind_vper;
    int ind;
    char linha[TAMSTRING];

    fgets(linha, TAMSTRING, arq2);

    // Varrendo todos os periodicos encontrados
    for (ind_vper = 0; ind_vper < tamv_per; ind_vper++)
    {
        // Zera o indice
        ind = 0;

        // Varrendo todo o arquivo com os periodicos
        while (!feof(arq2))
        {
            // Se os nomes forem iguais, adiciona o nivel no final de 'v_per[i]'
            if (strstr(linha, v_per[ind_vper]))
            {
                while (linha[ind] != '\0')
                    ind++;

                // Se o nivel nao for C, armazena dois caracteres
                if (linha[ind - 2] != 'C') {

                    // Concatenando o espaço em branco e os dois caracteres na string
                    strncat(v_per[ind_vper], &linha[ind - 4], 1);
                    strncat(v_per[ind_vper], &linha[ind - 3], 1);
                    strncat(v_per[ind_vper], &linha[ind - 2], 1);
                }

                // Se for, armazena um
                else {

                    // Concatenando o espaço e um caracter na string
                    strncat(v_per[ind_vper], &linha[ind - 3], 1);

                    // Aqui precisamos fazer '[ind - 2]' porque a linha alem de conter
                    // o '\0', tambem contem o '\n' 
                    strncat(v_per[ind_vper], &linha[ind - 2], 1);
                }
            }

            fgets(linha, TAMSTRING, arq2);
        }

        rewind(arq2);
    }

    //imprime_vetor(v_per, tamv_per);

    //free(linha);
}

// Funcao que imprime os periodicos de acordo com seus niveis
void imprime_periodicos_catalogados(char**v_per, int tamv_per)
{
    int i, j, k, ind = 0;
    char niveis[20][3] = {"A1", "A2", "A3", "A4", "B1", "B2", "B3" ,"B4"};
    char* straux = malloc(sizeof(char) * TAMSTRING);

    /*for (i = 0; i < 9; i++)
        char[i] = malloc(sizeof(char) * 3);*/

    // Loop com todos os niveis menos o 'C', pois eh preciso um teste mais elaborado
    // devido a ser um caracter so, todas os periodicos podem apresentar a letra 'C'
    for (j = 0; j < 8; j++)
    {
        printf("\n%s:\n", niveis[j]);
        for (i = 0; i < tamv_per; i++)
        {
            // Copia o nome do periodico para uma string auxiliar
            strcpy(straux, v_per[i]);

            // Pega o indice do fim da string
            while (straux[ind] != '\0')
                ind++;

            // Se estiver com o nivel no fim da string, imprima
            if (strstr(v_per[i], niveis[j]))
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

    printf("\nC:\n");
    for (i = 0; i < tamv_per; i++)
    {
        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v_per[i]);

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

        ind = 0;
    }

    printf("\n");

    /*for (i = 0; i < 9; i++)
        free(niveis[i]);

    free(niveis);*/
    free(straux);
}

// Funcao que imprime quantos periodicos o autor possui de cada nivel
void imprimeQuantPeriodicos(char** v_per, int tamv_per)
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

    for (i = 0; i < tamv_per; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (strstr(v_per[i], niveis[j]))
                quantlvl[j]++;
        }

        // Copia o nome do periodico para uma string auxiliar
        strcpy(straux, v_per[i]);

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
}


// Funcao que imprime a quantidade de periodicos na tela, separados por niveis
void sumarizacaoPeriodicos(FILE* arq)
{
    FILE* arq2;
    char c;
    int tamv_per = 0, i;
    char *str = malloc(sizeof(char) * TAMSTRING);  // String para armazenar cada nome de periodico
    char** v_per = malloc(sizeof(str) * 100);  // Aloca vetor de strings para 100 strings

    // Pega o primeiro caracter do arquivo
    c = fgetc(arq);

    // Inicializa a string 'str'
    strcpy(str, "");

    // Enquanto nao chegou no final do arquivo, faz 
    while (c != EOF)
    {
         // Se estamos perto de um titulo, armazena o periodico
        if (titulo_artigo(c, arq))
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
            v_per[tamv_per] = malloc(sizeof(char) * TAMSTRING);
            strcpy(v_per[tamv_per], str);
            tamv_per++;

            // Zera a string
            strcpy(str, "");
        }

        c = fgetc(arq);
    }

    //imprime_vetor(v_per, tamv_per);

    corrigirNomes(v_per, tamv_per);

    //imprime_vetor(v_per, tamv_per);

    // Abre o arquivo contendo os periodicos classificados
    arq2 = fopen(ARQUIVO2, "r");

    // Testa se o arquivo abre
    if (arq2 == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    separarPERIODICOS(v_per, tamv_per, arq2);

    imprimeQuantPeriodicos(v_per, tamv_per);

    //imprime_periodicos_catalogados(v_per, tamv_per);

    //imprime_vetor(v_per, tamv_per);

    // Da free em todos os espacos alocados da string 'v_per'
    for (i = 0; i < tamv_per; i++)
        free(v_per[i]);

    free(str);
    free(v_per);

    // Fecha o segundo arquivo
    fclose(arq2);
}

// Funcao que imprime todas as conferencias na tela, separadas por niveis
void imprime_conferencias(FILE* arq)
{

}

// Funcao que imprime todos os periodicos na tela, separados por autores
void imprime_periodicosAUTORES(FILE* arq)
{

}

// Funcao que imprime todas as conferencias na tela, separados por autores
void imprime_conferenciasAUTORES(FILE* arq)
{

}

// Funcao que imprime todos os periodicos na tela, separados por ano
void imprime_periodicosANO(FILE* arq)
{

}

// Funcao que imprime todas as conferencias na tela, separadas por ano
void imprime_conferenciasANO(FILE* arq)
{

}

// Funcao que imprime conferencias e periodicos catalogados no nivel C
void imprime_tudoC(FILE* arq)
{

}

// Funcao que imprime conferencias e periodicos catalogados no nivel C e os que nao foram encontrados
void imprime_tudoC_e_nao_encontrados(FILE* arq)
{

}


int main ()
{
    FILE* arq;
    int opt;
   
    printf("\n\nBem vindo! Digite a opcao que desejar:\n\n\n");

    imprimir_menu();
    opt = le_opt();

    // Abre o arquivo
    arq = fopen(ARQUIVO, "r");

    // Testa se o arquivo abre
    if (arq == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    if (opt == 1)
        sumarizacaoPeriodicos(arq);

    if (opt == 2)
        imprime_conferencias(arq);

    if (opt == 3)
        imprime_periodicosAUTORES(arq);

    if (opt == 4)
        imprime_conferenciasAUTORES(arq);

    if (opt == 5)
        imprime_periodicosANO(arq);

    if (opt == 6)
        imprime_conferenciasANO(arq);

    if (opt == 7)
        imprime_tudoC(arq);

    if (opt == 8)
        imprime_tudoC_e_nao_encontrados(arq);

    fclose(arq);
    return 0;
}
