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
    
    1 -> lê um arq .xml e imprime seu conteudo --FEITO--
    2 -> imprimir na tela apenas o nome dos periodicos (vêm depois do 'TITULO-DO-PERIODICO-OU-REVISTA=' e vai até "")
         while !EOF (if 'TITULO-DO-PERIODICO-OU-REVISTA=' armazenar até a última ")
         armazenar num vetor de strings (?)
    3 -> imprimir na tela apenas o nome das conferencias
    4 -> catalogar periodico(s) e imprimi-lo(s) na tela
    5 -> mesma coisa com conferencia
    6 -> abre um diretório, le todos os arquivos e imprime seus conteudos 
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
char* substitui_palavra(char** strf, const char* str, const char* Pvelha, const char* Pnova) 
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
        // compara a substring com o resultado 
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

// Funcao que corrige nomes de alguns periodicos, caracteres especiais e etc
// Por exemplo o caracter '&' eh escrito como '&amp;'
void corrigir_nomes(char** v_per, int tamv_per)
{   
    int i;

    for (i = 0; i < tamv_per; i++)
    {
        // Se tiver um '&amp', substitui por '&'
        if (strstr(v_per[i], "&amp;"))
            substitui_palavra(&v_per[i], v_per[i], "&amp;", "&");
    }
}

// Funcao que transforma todas as strings do vetor em letras maiusculas
void para_maiusculo(char** v_per, int tamv_per)
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

// Funcao que imprime os periodicos de acordo com seus niveis
// niveis: A1, A2, A3, A4, B1, B2, B3, B4 e C.
void separar_e_imprimirPERIODICOS(char** v_per, int tamv_per, FILE* arq2)
{
    int ind_vper;
    int ind;
    char linha[TAMSTRING];

    para_maiusculo(v_per, tamv_per);

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
                    strncat(v_per[ind_vper], &linha[ind - 2], 1);
                }
            }

            fgets(linha, TAMSTRING, arq2);
        }

        rewind(arq2);
    }
    //free(linha);
}


// Funcao que imprime todos os periodicos na tela, separados por niveis
void imprime_periodicos(FILE* arq)
{
    FILE* arq2;
    char c;
    int tamv_per = 0, i;
    char *str = malloc(sizeof(char) * TAMSTRING);  // String para armazenar cada nome de periodico
    char** v_per = malloc(sizeof(str) * 100);  // Aloca vetor de strings para 100 strings

    // Pega o primeiro caracter do arquivo
    c = fgetc(arq);

    // Zera a string 'str'
    strcpy(str, "");

    // Enquanto nao chegou no final do arquivo, faz 
    while (c != EOF)
    {
         // Se passou da verificacao, armazena o periodico
        if (titulo_artigo(c, arq))
        {
            c = fgetc(arq);

            while (c != '\"')
            {
                // Concatenando um caracter a mais na string do vetor de strings
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

    corrigir_nomes(v_per, tamv_per);

    //imprime_vetor(v_per, tamv_per);

    // Abre o arquivo contendo os periodicos classificados
    arq2 = fopen(ARQUIVO2, "r");

    // Testa se o arquivo abre
    if (arq2 == NULL)
    {
        printf("Impossivel abrir arquivo\n");
        exit(1);  // Fecha o programa com status 1
    }

    separar_e_imprimirPERIODICOS(v_per, tamv_per, arq2);

    imprime_vetor(v_per, tamv_per);

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
        imprime_periodicos(arq);

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
