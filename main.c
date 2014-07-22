/* 
 * File:   main.c
 * Author: maynor Gabriel Lopez Jimenez
 * Created on 17 de diciembre de 2013, 05:18 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//define sirve para declarar constantes
#define SEPARADORES " \t\n"
/* Maximo de caracteres por linea. */
#define MAX_BUFFER 1024
/*Maximo de caracteres de una palabra*/
#define MAX_PALABRA 64
typedef struct{
 /*
Declaramos la estructura a utilizar para los productos
la palabra typedef nos permite asignarle un alias o
nombre a la estructura permitiendonos dar la impresion
que es un tipo de datos y asi que no sea necesario poner
"struct miestructura nombre;"
al declarar una variable del tipo de estructura
en este caso para crear una variable de tipo producto seria
"producto nuevo;"
*/
char *codigo;
char *nombre;
float precio;
int cantidad;
} producto;
int contarLineas(char dir[]){
    /*
    Esta es una funcion la cual devuelve el numero de lineas que
    hay en un fichero especificado por la direccion la cual es
    el parametro dir
    */
    int cont=0;
    FILE *tmp;
    char buffer[MAX_BUFFER];
    /*
    la funcion fgets recibe 3 parametros
    el primero es el arreglo donde se guardara la linea leida del archivo
    la segunda es la cantidad de char que se leeran de la linea
    el tercero es el puntero que apunta al archivo que se esta leyendo
    esto se logra mediante fopen el cual recibe dos parametros la direccion
    del archivo que sera leido y un segundo comando que determina si el
    archivo se abrira como de lectura, escritura, etc.
    */
    tmp=fopen(dir,"r");
     while (fgets(buffer,MAX_BUFFER,tmp)!=NULL)
    {
        //fgets retornara null cuando no hayan mas lineas en el archivo
        cont=cont+1;
    }
    fclose(tmp);
return cont;
}
void mostrarProductos(char dir[]){
    printf("***LISTADO DE PRODUCTOS***\n");
    printf("<codigo> <nombre> <precio> <cantidad>\n");
    FILE *tmp;
    char buffer[512];
    tmp=fopen(dir,"r");
     while (fgets(buffer,256,tmp)!=NULL)
    {
        //fgets retornara null cuando no hayan mas lineas en el archivo
        printf("%s",buffer);

    }
    fclose(tmp);
}
void introducirProductos(char dir[]){
char linea[MAX_BUFFER];
printf("Introduzca los valores del nuevo producto:\n");
printf("(Formato: <codigo> <nombre> <precio> <cantidad>)\n");
while (1) {
fgets(linea, MAX_BUFFER, stdin );
if(!strcmp(linea, "\n")) break;
        FILE *inventario;
        inventario=fopen(dir,"a");
        fputs(linea,inventario);
        fclose(inventario);
}
}

char *obtenerCodigo(){
    char codigo[MAX_PALABRA];
    printf("Ingrese el codigo:");
    scanf("%s",&codigo);   
    return &codigo;
}
void eliminarProducto(char *codigo, char dir[]){
    int eliminados=0;
    FILE *inventario;
    FILE *inventarioAux;
    inventario=fopen(dir,"r");
    inventarioAux=fopen("InventarioAux","w");
    char buffer[MAX_BUFFER];
    while (fgets(buffer,MAX_BUFFER,inventario)!=NULL)
    {
        fputs(buffer,inventarioAux);
    }
    fclose(inventarioAux);
    fclose(inventario);
    //con lo anterior hacemos un backup de nuestros productos antes de modificarlos
    inventario=fopen(dir,"w");//sobreescribimos el archivo de inventario original
    inventarioAux=fopen("InventarioAux","r");//abrimos como lectura el backup
    while (fgets(buffer,MAX_BUFFER,inventarioAux)!=NULL)
    {
        /*
         * con fgets obtenemos todos los caracteres definidos por el MAX_BUFFER
         * estos caracteres son leidos del Backup de productos
         * y almacenandolos en el buffer
         */
        char bufferAux[MAX_BUFFER];
        /*
         * creamos un bufferAuxiliar debido a que al tokenizar una cadena
         * esta queda corrupta debido a la funcion strtok que separa una cadena de acuerdo
         * a las coincidencias con las que queramos separar la cadena
         * en este caso cada vez que encuentre un espacio, una tabulacion o un salto de linea
         * separa la cadena
         */
        strcpy(bufferAux,buffer);
        /*
         * un puntero de producto para construir un producto cada vez que leamos una linea
         * asi podemos ver como se llenaria toda la estructura y tambien
         * nos servira para ver como acceder al codigo del producto
         */
        producto *p;
        //creamos un arreglo de cadenas mediante un arreglo de punteros de cadenas
        char *tokens[5];
        int a=0;
        tokens[0] = strtok(bufferAux, SEPARADORES);
        /*
         * primero verificamos que no sea NULL el token lo cual significaria que hemos llegado al ultimo token
         * de la cadena que tokenizamos
         * la ventaja de utilizar tokens y la instruccion strtok es que esta es capaz de recordar la ultima
         * cadena que se estaba tokenizando para lo cual ya no es necesario poner la cadena de nuevo pues
         * esta esta corrupta debido a la primera ejecucioin de strtok por lo que en las siguientes ejecuciones
         * pasamos como parametro NULL lo que le indica que siga en la ultima cadena que se estaba
         * tokenizando
         */
        while(tokens[a] != NULL)
        {
            a++;
            tokens[a] = strtok(NULL, SEPARADORES);
        }
        tokens[4] = NULL;
        /*
         * la instruccion malloc sirve para asignacion dinamica de memoria con ella le decimos que reserve memoria
         * con un valor numerico, por lo cual se ocupa sizeof con el cual podemos ver cuanta memoria requiere un tipo
         * de dato en este caso sizeof vereificara cuanta memoria necesita o ocupa un dato de tipo producto y con malloc
         * con lo cual nos aseguramos que el producto tenga suficiente memoria
         */
        p= malloc(sizeof(producto));
        /*
         * la forma de acceder a los datos de una estructura depende de si se trata de un puntero o una variable
         * de ser un puntero lo hacemos asi dato->propiedad, pero al ser una variable debemos utlizar el * para indicar
         * que queremos acceder a un dato de esa variable de la manera siguiente (*dato).propiedad
         * "atof" es una funcion que recibe una cadena y devuelve el float equivalente
         * "atoi" es una funcion que recibe una cadena y devuelve el int equivalente
         */
        p->codigo = tokens[0];
        p->nombre = tokens[1];
        p->precio = atof(tokens[2]);
        p->cantidad = atoi(tokens[3]);
        if(!strcmp(codigo,p->codigo)){
          eliminados++;           
        }
        else{
            fputs(buffer,inventario);
           
        }
        
    }
    if(eliminados!=0){
        printf("Eliminado Satisfactoriamente el producto codigo %s\n",codigo);    
        }
        else{
            printf("No se encontro el producto codigo %s\n",codigo);
        }
    fclose(inventarioAux);
    fclose(inventario);
}
void modificarProducto(char *codigo, char dir[]){
    int modificados=0;
    FILE *inventario;
    FILE *inventarioAux;
    inventario=fopen(dir,"r");
    inventarioAux=fopen("InventarioAux","w");
    char buffer[MAX_BUFFER];
    while (fgets(buffer,MAX_BUFFER,inventario)!=NULL)
    {
        fputs(buffer,inventarioAux);
    }
    fclose(inventarioAux);
    fclose(inventario);
    inventario=fopen(dir,"w");
    inventarioAux=fopen("InventarioAux","r");
    while (fgets(buffer,MAX_BUFFER,inventarioAux)!=NULL)
    {
        char bufferAux[MAX_BUFFER];
        strcpy(bufferAux,buffer);
        producto *p;
        char *tokens[5];
        int a=0;
        tokens[0] = strtok(bufferAux, SEPARADORES);
        while(tokens[a] != NULL)
        {
            a++;
            tokens[a] = strtok(NULL, SEPARADORES);
        }
        tokens[4] = NULL;
        p= malloc(sizeof(producto));
        p->codigo = tokens[0];
        p->nombre = tokens[1];
        p->precio = atof(tokens[2]);
        p->cantidad = atoi(tokens[3]);
        if(!strcmp(codigo,p->codigo)){
          printf("Ingrese el nuevo nombre:");
          scanf("%s",p->nombre);
          printf("Ingrese la nueva cantidad:");
          scanf("%i",p->cantidad);
          printf("Ingrese el nuevo precio:");
          scanf("%s",p->precio);
          char *linea;
          char *tmp;
          sprintf(tmp, "%i", p->codigo);
          strcpy(linea,tmp);
          strcat(linea, " ");
          strcat(linea,p->nombre);
          sprintf(tmp, "%f", p->precio);
          strcat(linea," ");
          strcat(linea,tmp);
          strcat(linea, " ");
          sprintf(tmp,"%i",p->cantidad);
          strcat(linea,tmp);  
          fputs(linea,inventario);
          modificados++;           
        }
        else{
            fputs(buffer,inventario);           
        }        
    }
    if(modificados!=0){
        printf("Modificado Satisfactoriamente el producto codigo %s\n",codigo);    
        }
        else{
            printf("No se encontro el producto codigo %s\n",codigo);
        }
    fclose(inventarioAux);
    fclose(inventario);
    
}
void mostrarMenu(){
    char msj[MAX_BUFFER];
    strcpy(msj,"***MANEJO DE INVENTARIO***\n");
    strcat(msj,"Opciones:\n");
    strcat(msj,"1-Mostrar Productos.-\n");
    strcat(msj,"2-Eliminar Productos.-\n");
    strcat(msj,"3-Salir.-\n");
    strcat(msj,"Elija una opcion:\n");
    printf("%s",msj);
    
}


int main(int argc, char** argv) {
    printf("***MANEJO DE INVENTARIO***\n");
    introducirProductos("Inventario");
    while(1){
        int opcion;
        mostrarMenu();
        scanf("%i",&opcion);
        if(opcion==1){
         mostrarProductos("Inventario");   
        }
        if(opcion==2){
         char *codigo;
         codigo=obtenerCodigo();
         eliminarProducto(codigo,"Inventario");   
        }
        if(opcion==3){
            printf("GRACIAS!!!");
            return (EXIT_SUCCESS);
        }
            
    }
}


