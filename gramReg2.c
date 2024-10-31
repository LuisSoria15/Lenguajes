#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAM 300

typedef struct nodo1
{
    char *identificador;
    char *produccion;
    struct nodo1 *sig;
} TNodo;

typedef struct nodoN
{
    char *identificador;
    char **producciones;
    int numP;
    struct nodoN *sig;
} TNodo2;

TNodo *crea_nodo(char *identificador, char *produccion);
void inserta_final(TNodo **cab, char *identificador, char *produccion);
TNodo *leer_archivo(char *nombre_archivo);
void imprime(TNodo *cab);
void imprime2(TNodo2 *cab);
void inserta2(TNodo2 **cab, char *identificador, char *produccion);
void juntar_producciones(TNodo **cab, TNodo2 **cabN);
void eliminar_recursividad(TNodo2 **cab1, TNodo2 **cab2);
int main()
{
    char *nombre_archivo = "gramatica1.txt";
    TNodo *lista = leer_archivo(nombre_archivo);
    TNodo2 *cabN = NULL;
    TNodo2 *cab2 = NULL;

    imprime(lista);
    juntar_producciones(&lista, &cabN);
    imprime2(cabN);
    eliminar_recursividad(&cabN, &cab2);
    imprime2(cab2);

    TNodo *temp;
    while (lista != NULL)
    {
        temp = lista;
        lista = lista->sig;
        free(temp->identificador);
        free(temp->produccion);
        free(temp);
    }
    return 0;
}

TNodo *crea_nodo(char *identificador, char *produccion)
{
    TNodo *aux = (TNodo *)malloc(sizeof(TNodo));
    if (aux)
    {
        aux->identificador = strdup(identificador);
        aux->produccion = strdup(produccion);
        aux->sig = NULL;
    }
    return aux;
}

void inserta_final(TNodo **cab, char *identificador, char *produccion)
{
    TNodo *aux = crea_nodo(identificador, produccion);
    if (aux)
    {
        if (*cab == NULL)
        {
            *cab = aux;
        }
        else
        {
            TNodo *corre = *cab;
            while (corre->sig != NULL)
            {
                corre = corre->sig;
            }
            corre->sig = aux;
        }
    }
}

void inserta2(TNodo2 **cab, char *identificador, char *produccion)
{
    TNodo2 *nodo2 = (TNodo2 *)malloc(sizeof(TNodo2));
    if (nodo2 == NULL)
    {
        perror("Error al asignar memoria para el nodo");
        exit(EXIT_FAILURE);
    }
    nodo2->identificador = strdup(identificador);
    nodo2->producciones = (char **)malloc(sizeof(char *));
    nodo2->producciones[0] = strdup(produccion);
    nodo2->numP = 1;
    nodo2->sig = NULL;

    if (*cab == NULL)
    {
        *cab = nodo2;
    }
    else
    {
        TNodo2 *corre = *cab;
        while (corre->sig != NULL)
        {
            corre = corre->sig;
        }
        corre->sig = nodo2;
    }
}

TNodo *leer_archivo(char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    TNodo *cabeza = NULL;
    char linea[TAM];
    char identificador[TAM];
    char productor[TAM];

    while (fgets(linea, TAM, archivo))
    {
        linea[strcspn(linea, "\n")] = '\0';
        const char *delimitador = strstr(linea, "->");
        if (delimitador != NULL)
        {
            strncpy(identificador, linea, delimitador - linea);
            identificador[delimitador - linea] = '\0';
            strcpy(productor, delimitador + 2);
            inserta_final(&cabeza, identificador, productor);
        }
    }
    fclose(archivo);
    return cabeza;
}

void recorre(TNodo **cab, char *arr)
{
    TNodo *corre = *cab;
    int cont = 0;
    while (corre != NULL)
    {
        if ((*cab)->identificador == corre->identificador)
        {
            cont++;
            corre = corre->sig;
        }
    }
}   

void imprime(TNodo *cab)
{
    TNodo *aux = cab;
    printf("Lista:\n");
    while (aux != NULL)
    {
        printf("identificador: %s, produccion: %s\n", aux->identificador, aux->produccion);
        aux = aux->sig;
    }
}

void imprime2(TNodo2 *cab)
{
    TNodo2 *aux = cab;
    printf("Lista:\n");
    while (aux != NULL)
    {
        printf("identificador: %s, producciones ", aux->identificador);
        for (int i = 0; i < aux->numP; i++)
            printf("%s ", aux->producciones[i]);
        printf("\n");
        aux = aux->sig;
    }
}

void juntar_producciones(TNodo **cab, TNodo2 **cabN)
{
    TNodo *corre = *cab;
    TNodo *anterior = NULL;
    while (corre)
    {
        TNodo2 *exi = *cabN;
        while (exi != NULL && strcmp(exi->identificador, corre->identificador) != 0)
        {
            exi = exi->sig;
        }
        if (exi == NULL)
        {
            inserta2(cabN, corre->identificador, corre->produccion);
        }
        else
        {
            exi->producciones = realloc(exi->producciones, (exi->numP + 1) * sizeof(char *));
            if (exi->producciones != NULL)
            {
                exi->producciones[exi->numP] = strdup(corre->produccion);
                exi->numP++;
            }
        }
        TNodo *temp = corre;
        corre = corre->sig;
        if (anterior == NULL)
        {
            *cab = corre;
        }
        else
        {
            anterior->sig = corre;
        }
        free(temp->identificador);
        free(temp->produccion);
        free(temp);
        if (anterior == NULL)
        {
            anterior = *cab;
        }
        else
        {
            anterior = anterior->sig;
        }
    }
}

void eliminar_recursividad(TNodo2 **cab1, TNodo2 **cab2)
{
    TNodo2 *aux1 = *cab1;
    while (aux1)
    {
        TNodo2 *nodo2 = (TNodo2 *)malloc(sizeof(TNodo2));
        if (nodo2 == NULL)
        {
            perror("Error");
        }
        nodo2->identificador = strdup(aux1->identificador);
        nodo2->producciones = NULL;
        nodo2->numP = 0;
        int esRec = 0;
        for (int i = 0; i < aux1->numP; i++)
        {
            char *produccion = aux1->producciones[i];
            char *NuevaProd = strdup(produccion);
            if (strchr(produccion, aux1->identificador[0]) != NULL)
            {
                esRec = 1;
                for (int j = 0; NuevaProd[j] != '\0'; j++)
                {
                    if (NuevaProd[j] == aux1->identificador[0])
                    {
                        NuevaProd[j] = '*';
                    }
                }
            }
            nodo2->producciones = realloc(nodo2->producciones, (nodo2->numP + 1) * sizeof(char *));
            nodo2->producciones[nodo2->numP++] = NuevaProd;
        }
        nodo2->sig = NULL;
        if (*cab2 == NULL)
        {
            *cab2 = nodo2;
        }
        else
        {
            TNodo2 *temp = *cab2;
            while (temp->sig != NULL)
            {
                temp = temp->sig;
            }
            temp->sig = nodo2;
        }
        aux1 = aux1->sig;
    }
    aux1 = *cab1;
    while (aux1)
    {
        TNodo2 *temp = aux1;
        aux1 = aux1->sig;
        free(temp->identificador);
        free(temp->producciones);
        free(temp);
    }
}