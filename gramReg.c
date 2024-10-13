#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM 250

typedef struct nodo1 
{
    char *info;              
    struct nodo1 *sig; 
} TNodo;

TNodo *crea_nodo(char *dato);
void inserta_final(TNodo **cab, char *dato);
void imprime(TNodo *cab);
TNodo* leer_archivo(char *nombre_archivo);

int main() 
{
    char *nombre_archivo = "gramatica1.txt";
    TNodo *lista = leer_archivo(nombre_archivo);

    imprime(lista);

    TNodo *temp;
    while (lista != NULL) 
    {
        temp = lista;
        lista = lista->sig;
        free(temp->info); 
        free(temp);    
    }
    return 0;
}

TNodo *crea_nodo(char *dato) 
{
    TNodo *aux = (TNodo *)malloc(sizeof(TNodo));
    if (aux) 
    {
        aux->info = strdup(dato);
        aux->sig = NULL;
    }
    return aux;
}

void inserta_final(TNodo **cab, char *dato) 
{
    TNodo *aux = crea_nodo(dato);
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

void imprime(TNodo *cab) 
{
    TNodo *aux = cab;
    while (aux != NULL) 
    {
        printf("%s\n", aux->info);
        aux = aux->sig;
    }
}

TNodo* leer_archivo(char *nombre_archivo) 
{
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) 
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    TNodo *cabeza = NULL;
    char linea[TAM];

    while (fgets(linea, TAM, archivo)) 
    {
        linea[strcspn(linea, "\n")] = '\0';
        inserta_final(&cabeza, linea);  
    }
    fclose(archivo);
    return cabeza;
}
