#include <stdio.h>
#include <stdlib.h>


typedef struct nodo{
    int dato;
    struct nodo* siguiente;
}nodo;


void encola ( nodo** vertice , int var )
{
    struct nodo* nodo_nuevo = (struct nodo*)malloc(sizeof(struct nodo));
  
    if(nodo_nuevo == NULL)
    {
        printf("no hay espacio en el sistema \n");
        return;
    }

    *vertice = nodo_nuevo; 
	(*vertice)->siguiente = NULL;
	nodo_nuevo->dato = var;
    printf("encola : %d \n", (*vertice)->dato);
};


int desencola ( nodo** comienzo ) 
{
	if ( *comienzo == NULL )
    {
		printf("la cola se encuentra vacia,no hay elementos que eliminar...\n");
		return -1;
	}

    // condicion para evaluar la cola del BFS
    // se adapto la funcion al pedido del algoritmo BFS,
    // donde no se tiene mas de un nodo en la cola,
    // por que solo se analiza un nodo a la vez.
    struct nodo* temporal  = *comienzo;
    int var1;

    while ( temporal != NULL ){
        if ( temporal->siguiente == NULL )
        {
            var1 = (*comienzo)->dato;    
            (*comienzo)->siguiente = NULL;
            (*comienzo) = NULL;
            break;
        }
        temporal = temporal->siguiente;   
    }       
    
    printf("descola : %d \n " , var1);
    return var1;
};


int esta_vacia ( nodo** comienzo )
{
    if ( *comienzo == NULL ) return 1;
    else return 0;
};




