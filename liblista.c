/*
 * TAD lista
 * Autores:
 *    André Ricardo Abed Gregio
 *    Marcos Alexandre Castilho
 *    Luis Carlos Erpen de Bona
 *    Luiz Carlos Pessoa Albini
 *
 * Versao 1.0.0 de 04/04/2022
*/

#include <stdio.h>
#include <malloc.h>
#include "liblista.h"

lista_t* lista_cria ()
{
    lista_t *l;
    nodo_l_t *nodo1, *nodo2;

    /* aloca espaco para a cabeca da lista_t */
    if (!(l = (lista_t *) malloc (sizeof (lista_t))))
        return NULL;

    if (!(nodo1 = (nodo_l_t *) malloc (sizeof (nodo_l_t))))
    {
        free (l);
        return NULL;
    }

    if (!(nodo2 = (nodo_l_t *) malloc (sizeof (nodo_l_t))))
    {
        free (nodo1);
        free (l);
        return NULL;
    }

    /* crio dois nodos "fantasmas" para evitar alguns ifs depois */
    /* a lista vazia eh quando l->ini->prox->prox == NULL        */
    l->tamanho= 0;
    l->ini= nodo1;
    nodo1->prox= nodo2;
    nodo2->prox= NULL;
    nodo1->elemento= 0;
    nodo2->elemento= 0;

    return l;
}

lista_t* lista_destroi (lista_t* l)
{
    nodo_l_t *aux;

    while (l->ini->prox->prox != NULL)
    {
        aux= l->ini->prox;
        l->ini->prox= aux->prox;
        aux->elemento= 0;
        free (aux);
    }

    free (l->ini->prox);
    free (l->ini);
    l->ini= NULL;
    l->tamanho= 0;
    free (l);

    return l;
}

int lista_tamanho (lista_t* l)
{
    return l->tamanho;
}

int lista_vazia (lista_t* l)
{
    return (lista_tamanho (l) == 0);
}

int lista_insere_inicio (lista_t* l, int elemento)
{
    nodo_l_t *novo;

    if (! (novo = (nodo_l_t *) malloc (sizeof (nodo_l_t))))
        return 0;

    novo->elemento= elemento;
    novo->prox= l->ini->prox;
    l->ini->prox= novo;
    (l->tamanho)++;

    return 1;
}

int lista_insere_fim (lista_t* l, int elemento)
{
    nodo_l_t *novo, *ptr;

    if (! (novo = (nodo_l_t *) malloc (sizeof (nodo_l_t))))
        return 0;

    /* procura o penultimo */
    ptr= l->ini;
    while (ptr->prox->prox != NULL)
        ptr= ptr->prox;
    
    novo->elemento= elemento;
    novo->prox= ptr->prox;
    ptr->prox= novo;
    (l->tamanho)++;

    return 1;
}

int lista_insere_ordenado (lista_t* l, int elemento)
{
    nodo_l_t *novo, *ptr, *prev;

    /* se a lista for vazia insere no inicio */
    if (lista_vazia (l))
    {
        lista_insere_inicio (l, elemento);
        return 1;
    }

    if (! (novo = (nodo_l_t *) malloc (sizeof (nodo_l_t))))
        return 0;

    novo->elemento= elemento;

    /* acha o lugar para inserir elemento */
    prev= l->ini;
    ptr= l->ini->prox;
    while ((ptr->prox != NULL) && (ptr->elemento <= elemento))
    {
        ptr= ptr->prox;
        prev= prev->prox;
    }

    /* tem que inserir entre prev e ptr */
    novo->prox= ptr;
    prev->prox= novo;
    (l->tamanho)++;

    return 1;
}

int lista_retira_inicio (lista_t* l, int* elemento)
{
    nodo_l_t *aux;

    if (lista_vazia (l))
        return 0;

    aux= l->ini->prox;
    l->ini->prox= aux->prox;
    *elemento= aux->elemento;
    aux->elemento= 0;
    free (aux);
    (l->tamanho)--;

    return 1;
}

int lista_retira_fim (lista_t* l, int* elemento)
{
    nodo_l_t *ptr, *prev;

    if (lista_vazia (l))
        return 0;

    /* procura o ultimo */
    prev= l->ini;
    ptr= l->ini->prox;
    while (ptr->prox->prox != NULL)
    {
        prev= prev->prox;
        ptr= ptr->prox;
    }
    
    *elemento= ptr->elemento;
    ptr->elemento= 0;
    prev->prox= ptr->prox;
    free (ptr);
    (l->tamanho)--;

    return 1;
}

int lista_retira_elemento (lista_t *l, int *elemento)
{
    nodo_l_t *ptr, *prev;

    if (lista_vazia (l))
        return 0;

    if (! (lista_pertence (l, *elemento)))
        return 0;

    /* primeiro localiza elemento e o anterior dele */
    prev= l->ini;
    ptr= l->ini->prox;
    while (ptr->elemento != *elemento)
    {
        prev= prev->prox;
        ptr= ptr->prox;
    }
    
    *elemento= ptr->elemento;
    ptr->elemento= 0;
    prev->prox= ptr->prox;
    (l->tamanho)--;
    free (ptr);

    return 1;
}

int lista_pertence (lista_t* l, int elemento)
{
    nodo_l_t* ptr;

    if (lista_vazia (l))
        return 0;

    /* localiza elemento */
    ptr= l->ini->prox;
    while ((ptr->prox != NULL) && (ptr->elemento != elemento))
        ptr= ptr->prox;
    
    if (ptr->prox != NULL)
        return 1;

    return 0;
}

void lista_imprime (lista_t* l)
{
    nodo_l_t* ptr;

    if (lista_vazia (l))
        return;

    ptr= l->ini->prox;
    while (ptr->prox->prox != NULL)
    {
        printf ("%d ", ptr->elemento);
        ptr= ptr->prox;
    }
    printf ("%d\n", ptr->elemento);
}