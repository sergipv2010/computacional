/////// MÉTODOS DE ISING Y MONTECARLO ///////
// Se va a llevar a cabo un problema para construir el método de Ising basado
//en la generación de números aleatorios mediante el método de Montecarlo

// En primer lugar, se van a declarar una serie de comandos que nos van a permitir generar nuestro 
// código así como utilizar las herramientas necesarias
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
// Ahora vamos a incluir una serie de librerías para poder generar números aleatorios
#include<stdlib.h>
#include<time.h>
using namespace std;

// Declaración de variables repetidas del sistema
#define TAM_MAX 1000 // Espacio reservado en el programa para la generación de las matrices

// Llamada a las funciones declaradas al final del código
void ConfiguracionSpindes(int s[TAM_MAX][TAM_MAX],int n);
void ConfiguracionSpinor(int s[TAM_MAX][TAM_MAX],int n);
void CContornoPeriodicas(int s[TAM_MAX][TAM_MAX],int n);
double Calculop(double energia,double T);
void MuestraMatriz(int s[TAM_MAX][TAM_MAX],int N);

int main()
{
    // Para poder generar números aleatorios se van a inicializar de la siguiente manera
    srand(time(NULL));

    // Declaración de variables del sistema
    double T, energia,ale,p;
    int s[TAM_MAX][TAM_MAX]; // Matriz de spin que puede tener dos valores 1 y -1
    int i, j, k, pasos, N, pasosm,aux;
    string elec; // Permite elegir la configuración inicial de spines
    int n, m; // Contadores para encontrar un punto al azar inicial
    ofstream fich_salida;

    // En primer lugar, el modelo de Ising se define en una red cuadrada bi-dimensional N²
    //donde en cada nudo existe una variable de spin. Por ello, vamos a introducir por teclado
    //el tamaño de esta cuadrícula.
    cout << "Introduce el tamaño de la red del modelo de Ising (N entero): " << endl;
    cin >> N;

    // Ahora se va a pedir por pantalla un valor para la temperatura entre [0,5]K
    cout << "Introduce un valor de temperatura en el intervalo [0,5] K: " << endl;
    cin >> T;

    // Se pide por pantalla el número de pasos Montecarlo
    cout << "Introduce el numero de pasos Montecarlo (entero): " << endl;
    cin >> pasos;
    // Ahora tal y como se ha explicado en clase se reescribe el número de pasos Montecarlo
    //a partir de las dimensiones de la red bidimensional del modelo de Ising
    pasosm=pasos*N*N;

    cout << endl;

    // Se pide por pantalla el tipo de configuración inicial de spines para la temperatura escogida
    cout << " Podemos iniciar con la configuracion spines incial ordenada u desordenada" << endl;
    cout << " Si la temperatura es alta, escoge la configuración ordenada. En caso contrario la otra." << endl;
    cout << "Introduce si la configuracion (inicial) es ordenada o desordenada: " << endl;
    cin >> elec;

    // Vamos a elegir si la configuración inicial es desordenada u ordenada
    if (elec=="ordenada"){
        
        // Se genera una configuración inicial de spines con valores de 1
        ConfiguracionSpinor(s,N);

        // Imponemos las condiciones de contorno periódicas
        CContornoPeriodicas(s,N);

        // Abrimos el fichero de datos
        fich_salida.open("Matricesorini.txt");



        // Algoritmo de Metropolis
        for(k=1;k<pasosm+1;k++){

            // Primero se elige un punto al azar del problema y se evalua la energía
            // Dado que en C++ las matrices reservan sitio desde la casilla o hasta la 9, se
            //van a tener que imponer unas condiciones especiales en la generación des estos 
            //números aleatorios
            n=rand()%(N-1);
            m=rand()%(N-1);

            //Calculamos la energía
            energia=2*s[n][m]*(s[n+1][m]+s[n-1][m]+s[n][m+1]+s[n][m-1]);

            // Calculamos p y lo comparamos con un número aleatorio entre 0 y 1
            //para estudiar si el spín cambia de signo
            p=Calculop(energia,T);

            // Ahora se va a generar un número aleatorio en el intervalo [0,1] y se va a 
            //comparar con el valor de p calculado
            aux=RAND_MAX;
            ale=1.0*rand()/aux; // Número aleatorio generado entre 0 y 1
            if (ale<p){
                s[n][m]=-s[n][m];
            }

            // Imponemos las condiciones de contorno periódicas
            CContornoPeriodicas(s,N);

            if (k%(N*N)==0)
            {

                // Rellenamos el fichero de datos 
                for (i=0;i<N-1;i++){
                    for (j=0;j<N-1;j++){
                        fich_salida << s[i][j] << ", ";
                    }
                    fich_salida << s[i][j] <<endl;
                }
                fich_salida << endl;
            }    
        }
        // Cerramos el fichero
        fich_salida.close();
    }
    else{
        
        // Se genera una configuración inicial de spines con valores aleatorios de 1 o -1
        ConfiguracionSpindes(s,N);

        // Imponemos las condiciones de contorno periódicas
        CContornoPeriodicas(s,N);

        // Abrimos el fichero de datos
        fich_salida.open("Matricesdesini.txt");


        
        // Algoritmo de Metropolis
        for(k=1;k<pasosm+1;k++){

            // Primero se elige un punto al azar del problema y se evalua la energía
            // Dado que en C++ las matrices reservan sitio desde la casilla o hasta la 9, se
            //van a tener que imponer unas condiciones especiales en la generación des estos 
            //números aleatorios
            n=rand()%(N-1);
            m=rand()%(N-1);

            //Calculamos la energía
            energia=2*s[n][m]*(s[n+1][m]+s[n-1][m]+s[n][m+1]+s[n][m-1]);

            // Calculamos p y lo comparamos con un número aleatorio entre 0 y 1
            //para estudiar si el spín cambia de signo
            p=Calculop(energia,T);

            // Ahora se va a generar un número aleatorio en el intervalo [0,1] y se va a 
            //comparar con el valor de p calculado
            aux=RAND_MAX;
            ale=1.0*rand()/aux; // Número aleatorio generado entre 0 y 1
            if (ale<p){
                s[n][m]=-s[n][m];
            }

            // Imponemos las condiciones de contorno periódicas
            CContornoPeriodicas(s,N);

            if (k%(N*N)==0)
            {

                // Rellenamos el fichero de datos 
                for (i=0;i<N-1;i++){
                    for (j=0;j<N-1;j++){
                        fich_salida << s[i][j] << ", ";
                    }
                    fich_salida << s[i][j] <<endl;
                }
                fich_salida << endl;
            }    
        }
        // Cerramos el fichero
        fich_salida.close();
    }

    return 0;
}

//////Declaración de Funciones para construir el algoritmo de Metropolis///////

// 1) Función para generar una configuración inicial de spines de manera aleatoria
void ConfiguracionSpindes(int s[TAM_MAX][TAM_MAX],int n)
{
    int i,j;
    int aux; // Este parámetro nos va a determinar la aleatoriedad de la matriz de spin

    // En primer lugar, se va a inicializar la matriz entera de spin a 1
    for (i=0;i<=n;i++){
        for (j=0;j<=n;j++){
            s[i][j]=1;
        }
    }
    
    // Inicializamos el valor del parámetro aux
    aux=0;
    // Ahora en función del valor del parámetro aux, algunos términos van a pasar de tener un 1
    //a un -1
    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            aux= rand(); //Generamos el número aleatorio y se impone una condición
            if (aux%2==0){ // Se ha imouesto esta condición de aletoriedad para generar la matriz s
                s[i][j]=-1;
            }
        }
    }

    return;
}

// 2) Función para generar una configuración inicial de spines solo formada por 1
void ConfiguracionSpinor(int s[TAM_MAX][TAM_MAX],int n)
{
    int i,j;
    int aux; // Este parámetro nos va a determinar la aleatoriedad de la matriz de spin

    // En primer lugar, se va a inicializar la matriz entera de spin a 1
    for (i=0;i<=n;i++){
        for (j=0;j<=n;j++){
            s[i][j]=1;
        }
    }

    return;
}

// 3) Función para establecer en la matriz de spin las condiciones de contorno periódicas
void CContornoPeriodicas (int s[TAM_MAX][TAM_MAX],int n)
{
    int i,j;

    // Se van a establecer las condiciones de contorno periódicas tanto para las filas
    //como para las columnas
    
    // Columnas:
    for (i=0;i<=n;i++){
        s[i][n-1]=s[i][0];
        s[i][1]=s[i][n]; 
    }

    // Filas:
    for (j=0;j<=n;j++){
        s[0][j]=s[n-1][j];
        s[n][j]=s[1][j]; 
    }

    return;
}

// 4) Función p para el algoritmo de Metrópolis
double Calculop(double energia,double T)
{
    double E;

    // Evaluamos p y comparamos con el valor de 1
    E=exp(-1.0*energia/T);
    if (E>1){
        return 1;
    }
    else{
        return E;
    }
}

// 5) Función para mostrar por pantalla los valores de una matriz
void MuestraMatriz (int s[TAM_MAX][TAM_MAX],int N)
{
    int i,j;

    for(i=0;i<N;i++){
    for(j=0;j<N;j++){  
        cout << s[i][j];
    }
    cout << endl;
    }
    cout << " " << endl;

    return;
}









