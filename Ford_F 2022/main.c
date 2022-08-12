#include <limits.h>
#include <string.h>
#include "tail.h"


// úNmero de vertices en el grafico dado
#define V 6
#define true 1
#define false 0

// inicia siendo igual que Grafo , pero se iran modificando los pesos
// segun el flujo
int Grafo_aux [ V ][ V ];

// Se armaran los semicaminos de s a T usando a los nodos como indices
// en el pseudocodigo lo llamamos p
int semicamino [ V ];

// macro que compara dos numeros y retorna el menor
#define min (X , Y) ((( X ) < ( Y ) ) ? ( X ) : ( Y ) )

// Definimos la red
int red [ V ][ V ] = { {0,4,6,0,0,0},
                       {0,0,0,3,5,0},
                       {0,0,0,0,6,0},
                       {0,0,9,0,0,5},
                       {0,0,0,0,0,4},
                       {0,0,0,0,0,0} };



/* Devuelve true si hay un camino desde la fuente de ’s ’
bajando a ’t ’ en
red auxiliar . éTambin se llena de los semicamino [] para
almacenar el camino */
int busqueda_camino ( int red_aux [V][V] , int s , int t , int semicamino []) 
{
    //printf("se llamo a la funcion \n");
    // Crear vector visitado y marcar todos los évrtices como no visitados
    int visitado [ V ];
    memset ( visitado , 0 , sizeof ( visitado ) );
    memset ( semicamino , 0 , sizeof ( V ) );

    // óCreacin de una cola , évrtice fuente encola y marcar vertices fuente
    // como visitado
    printf("procesamiento \n");
    struct nodo* vertice = NULL;
    encola( &vertice , s); 
    visitado  [s] = 1;
    int aristasEnSemicamino = 0;
    
    // Se recorre red_aux para encontrar las adyacencias y los semicaminos
    // Consejo : Buscar , investigar sobre algoritmo de recorrido de grafos BFS
    int verticeOrigen = 0; // guarda el ultimo vertice adyacente encontrado
    while ( (esta_vacia(&vertice)) == false )
    {
        int temp = desencola(&vertice);      
        printf("vertice a analizar : %d \n" , temp);
        
        // no hace falta usar esta condicion
        // if (temp == t) break; 
        
        // busca los vertices adyacentes del vertice analizado,
        // se valida si es posible extender la arista entre los dos vertices,
        // se extiende solo si la capacidad restante de la arista es mayor a 0 (capacidad completada)
        // o si el vertice no ha sido visitado
        for (int i = 0; i < V; i++)
        {
            // se verifica que la arista adyacente analizada
            // no este visitada y que el flujo disponible sea distinto a cero
            if ( red[temp][i] != 0 && visitado[i] == 0 && Grafo_aux[temp][i] != 0 )
            {            
                // guardo el ultimo vertice analizado -> "vertice origen"
                verticeOrigen = temp;
                semicamino[verticeOrigen] = i;
                printf("vertice adyacente encontrado : %d \n", i);
                visitado  [i] = 1;
                aristasEnSemicamino++; // conteo de las aristas en el semicamino
    
                // este valor en el grafo aux se va a modificar cuando 
                // se ajuste el flujo , en el recorrido inverso sobre 
                // el camino encontrado
                Grafo_aux[verticeOrigen][i] = Grafo_aux[verticeOrigen][i] - red[verticeOrigen][i];        
                encola( &vertice , i );
                break;
            }
        }
    }

    printf("ultimo vertice del semicamino hallado : %d \n", verticeOrigen);

    // busco el flujo menor en las aristas del camino , SOLO SI SE ENCONTRO UN CAMINO  
    // empezando desde la ultima arista, hacia atras
    if ( visitado [ t ] == true ) 
    {
        int j = t; // valor del ultimo vertice alcanzado (T)
        int flujoMenor = 0;
        int verticeAnterior = 0;
        
        while ( j != 0)
        {
            // busco el vertice anterior ,adyacente al vertice actual,
            // los cuales forman la arista anterior a la ultima arista
            int k = 0;       
            while ( semicamino[k] != verticeOrigen )
            {
                k++;
            }
        
            // guardo el vertice anterior ,adyacente al vertice actual
            verticeAnterior = k;

            // se inicializa la variable "flujo menor" ,
            // con el valor de flujo de la ultima arista,
            // para luego comparar con el flujo de la arista anterior
            // y asi , guardar el menor valor.    
            if ( flujoMenor == 0 ) // bandera
            {
                flujoMenor = red[semicamino[verticeAnterior]][semicamino[verticeOrigen]];
            }

            if ( red[verticeAnterior][verticeOrigen] < flujoMenor && red[verticeAnterior][verticeOrigen] != 0  )
            {
                flujoMenor = red[verticeAnterior][verticeOrigen];
                printf("flujo menor : %d \n" , flujoMenor);    
            }
        
            // retrocedo a la arista anterior del camino
            verticeOrigen = verticeAnterior;
            j = semicamino[verticeAnterior];
        }

        printf("flujo posible encontrado del camino : %d \n" , flujoMenor);
        printf("\n");
        
        // imprimo el camino encontrado
        // y asigno el "flujo menor" a el camino hallado
        // ( todas las aristas que lo conforman )
        int i = 0;
        printf("semicamino encontrado : \n");
        printf("cantidad de aristas que lo conforman : %d \n",aristasEnSemicamino);
        while ( semicamino[i] != 0 )
        {
            Grafo_aux[i][semicamino[i]] = flujoMenor;
            
            // se ajusta el flujo disponible en la red original,
            // despues de restar la capacidad de la arista
            // y el flujo circulando en ella
            // esto ayuda para validar la busqueda de mas posibles semicaminos,
            // si no hay capacidad restante,no es posible la busqueda
            // y no se repiten semicaminos ya explorados
            red[i][semicamino[i]] = red[i][semicamino[i]] - flujoMenor;
            printf("arista : ( %d %d ) , flujo : %d \n",i,semicamino[i] , Grafo_aux[i][semicamino[i]] );
            i = semicamino[i];
        }
    }
    else
    {
        printf("no se ha encontrado un nuevo camino \n");
    }
    

    return ( visitado [ t ] == true ) ;
}



// Retorna el maximo flujo total de s a t en el grafico dado
int fordFulkerson ( int red [V][V] , int s , int t ) 
{
    /* no le veo utilidad a esto
    for ( int i = 0; i < V; i++ )
        for (int j = 0; j < V; i++)
        {
            if ( Grafo_aux[i][j] != 0 )
                red [i][j] = 0;
                red [j][i] = 0;
            
        }
    */ 
    
    int cf;    
    int i = 0;
    while ( semicamino[i] != 0 )
    {
        if ( i == 0 ) // bandera
            cf = red[i][semicamino[i]];

        if ( red[i][semicamino[i]] < cf && i != 0 )
            cf = red[i][semicamino[i]];

        i++;    
    }

    int j = 0;
    int flujoMaximoDelCamino = Grafo_aux[j][semicamino[j]] + cf;
    while ( semicamino[j] != 0 )
    {
        Grafo_aux[j][semicamino[j]] = Grafo_aux[j][semicamino[j]] + cf;
        Grafo_aux[semicamino[j]][j] = - Grafo_aux[j][semicamino[j]];
        j++;
    }
            
    
    return flujoMaximoDelCamino;
}



int main(int argc, char const *argv[])
{
    
    // guardo el flujo circulante total 
    // del grafico dado
    int flujoMaximoTotal = 0;
    
    // relleno valores en red aux
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
        {
            Grafo_aux [i][j] = red[i][j];
        }
    
    
    while ( busqueda_camino( Grafo_aux , 0 , 5 , semicamino ) )
    {
        printf ("MAIN : 1 camino encontrado \n \n");
        printf( "flujo maximo total posible circulando en el grafico: %d \n \n", 
                flujoMaximoTotal += fordFulkerson (red,0,5));
    }
    

    return 0;
}
