//////////Programa de simulación del Sistema Solar/////////

// En primer lugar, se van a declarar una serie de comandos que nos van a permitir generar nuestro 
// código así como utilizar las herramientas necesarias
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
using namespace std;

// Declaración de las variables físicas:
#define G 6.67*pow(10,-11) // Constante universal gravitatoria (Nm²/kg²) 
#define Ms 1.99*pow(10,30) // Masa del Sol (kg)
#define c 1.496*pow(10,11) // Distancia entre la tierra y el Sol (m)
#define TAM_MAX 100 // Dimensión del vector (Número de planetas del Sistema Solar)

// Llamada a las funciones declaradas al final del código
bool LeerFichero (double x[], string nombre, int n);
bool MostrarFichero (string nombre, int n);
void AlmacenaDatos (double x[], double y[], string nombre, int n);
void ReescalamientoMasa (double m[], int n);
void ReescalamientoPosicion (double x[], int n);
void ReescalamientoVelociad (double v[],int n);
void CalculaAceleracion (double ax[],double ay[],double m [],double x[],double y[],int n );
void CalculaPosicion(double x[],double v[],double a[],int n,double h);
void CalculaVelocidad(double v[],double a1[],double a2[],int n,double h);
void Intercambio(double a1x[],double a1y[],double a2x[],double a2y[],int n);
void MuestraVector (double x[],int n);
double CalculoEnergia (double x[],double y[],double vx[],double vy[],double m[],int n);
double CalculoMomento (double x[],double y[],double vx[],double vy[],double m[],int n);
void CalculaPeriodo (double y[],int cont [], double T[], double tvuelta [], int n,double t);

// Función principal donde se va a ejucutar el código
int main()
{
    // Declaración de variables para el programa
    double h,t; // Valores del paso y el tiempo
    double x[TAM_MAX], y[TAM_MAX], vx[TAM_MAX], vy[TAM_MAX]; // Vectores de coordenadas
    double a1x[TAM_MAX],a1y[TAM_MAX],a2x[TAM_MAX],a2y[TAM_MAX]; // Vectores de aceleración
    double mplanetas[TAM_MAX];// Masa de los planetas
    double T[TAM_MAX], tvuelta[TAM_MAX]; // Periodo de los planetas
    int cont[TAM_MAX]; // Contador para el cálculo del período
    int i,k,nplanetas,ital,num; //Contadores para el código
    // ital va a ser el contador que nos va a determinar las it. del Algoritmo de Verlet
    ofstream fich_salida1, fich_salida2,fich_salida3,fich_salida4,fich_salida5;

    cout << " PROGRAMA DE SIMULACION DEL SISTEMA SOLAR " << endl;
    cout << endl;

    // Pedimos por pantalla el número de planetas del Sistema Solar que vamos a estudiar
    cout << "Escribe el numero de planetas del sistema solar (generalmente seran 10): ";
    cin >> nplanetas;

    // Inicializamos los vectores de trabajo que vamos a utilizar
    for (i=0;i<nplanetas;i++)
    {
        mplanetas[i]=0;
        x[i]=0;
        y[i]=0;
        vx[i]=0;
        vy[i]=0;
        a1x[i]=0;
        a1y[i]=0;
        T[i]=0;
        tvuelta[i]=0;
        cont[i]=0;
    }
    
    // Pedimos el paso del algoritmo de Verlet por pantalla
    cout << "Introduce el paso del Algoritmo de Verlet: " << endl;
    cin >> h;

    // Pedimos el tiempo inicial del Algoritmo
    cout << "Introduce el tiempo inicial (generalmente t=0): " << endl;
    cin >> t;

    //Pedimos el número de iteraciones del Algoritmo de Verlet
    cout << "Introduce el numero de iteraciones del Algoritmo: " << endl;
    cin >> ital;

    
    // Se va a tomar como origen de nuestro sistema de referencia el Sol para nuestra simulación
    //y los datos se guardan en Posicion.txt. Para el caso en el que la Tierra sea el origen del
    //sistema de referencia los datos se guardan en Posiciongeo.txt

    // Almacenaje de datos por fichero de la masa (kg) de los planetas. Cada posición del 
    //vector va a corresponder con un planeta comenzando por el Sol.
    LeerFichero(mplanetas,"MasaP.txt",nplanetas);
    // Reescalamos la masa. Ahora vamos a tener los planetas en Masas solares
    ReescalamientoMasa(mplanetas,nplanetas);
    

    // Almacenaje de datos por fichero de la posición inicial (m) de los planetas. Cada posición del
    //vector va a corresponder con un planeta comenzando por el Sol.
    LeerFichero(x,"Posinix.txt",nplanetas);
    LeerFichero(y,"Posiniy.txt",nplanetas);
    // Reescalamos las posiciones. Ahora tenemos la posición en unidades astronómicas (UA)
    ReescalamientoPosicion(x,nplanetas);
    ReescalamientoPosicion(y,nplanetas);

    
    //Almacenaje de datos por fichero de la velocidad inicial (m/s) de los planetas. Cada velocidad del
    //vector va a corresponder con un planeta comenzando por el Sol.
    LeerFichero(vx,"Vinix.txt",nplanetas);
    LeerFichero(vy,"Viniy.txt",nplanetas);
    //Reescalamos la velocidad en la unidades adecuadas (según lo anterior)
    ReescalamientoVelociad(vx,nplanetas);
    ReescalamientoVelociad(vy,nplanetas);

    // Vamos a calcular la aceleración a partir de los datos inciales.
    CalculaAceleracion(a1x,a1y,mplanetas,x,y,nplanetas);

    //Abrimos los ficheros para guardar los datos de la simulación
    fich_salida1.open("Posicion.txt"); // Posicion (x,y) con el Sol en el origen
    fich_salida2.open("Energia.txt"); // Energia
    fich_salida3.open("Momento.txt"); // Momento
    fich_salida4.open("Posiciongeo.txt"); // Posición (x,y) con la Tierra en el origen
    fich_salida5.open("Periodos.txt"); // Periodo

    // Algoritmo de Verlet (PROGRAMA DE SIMULACION DE NUESTRO SISTEMA SOLAR)
    for (k=0;k<ital;k++){

        // Almecenamiento de datos en un fichero
        if(k%10==0){ // El condicional se ha realizado para poder moderar el número de datos introducidos al fichero
            // (1) Orbitas de los planetas (Sol en el centro):
            for(i=0;i<nplanetas;i++){
                fich_salida1 << x[i]-x[0] << ", " << y[i]-y[0] << endl;
            }
            fich_salida1 << endl;
            
            // (2) Energia:
            fich_salida2 << t << " " << CalculoEnergia(x,y,vx,vy,mplanetas,nplanetas) << endl;
            
            // (3) Momento:
            fich_salida3 << t << " " << CalculoMomento(x,y,vx,vy,mplanetas,nplanetas) << endl;

            // (4) Orbitas de los planetas (Tierra en el centro):
            for(i=0;i<nplanetas;i++){
            fich_salida4 << x[i]-x[3] << ", " << y[i]-y[3] << endl;
            }
            fich_salida4 << endl;
        }

        // Para cada iteración del algoritmo se va a actualizar el tiempo
        t=t+h;
                
        // Posición de cada planeta
        CalculaPosicion(x,vx,a1x,nplanetas,h);
        CalculaPosicion(y,vy,a1y,nplanetas,h);
        
        // Inicializamos el vector de la aceleración en a(t+h) para cada iteración del código
        for(i=0;i<nplanetas;i++){
            a2x[i]=0;
            a2y[i]=0;
        }

        // Calculamos de nuevo las aceleraciones
        CalculaAceleracion(a2x,a2y,mplanetas,x,y,nplanetas);

        // Velocidad de los planetas
        CalculaVelocidad(vx,a1x,a2x,nplanetas,h);
        CalculaVelocidad(vy,a1y,a2y,nplanetas,h);


        // Intercambios el valor de la aceleración para la posterior iteración
        Intercambio(a1x,a1y,a2x,a2y,nplanetas);

        // Periodo de los planetas
        CalculaPeriodo(y,cont,T,tvuelta,nplanetas,t);

    }

    // Ahora vamos a guardar los datos del periodo en un fichero
    for(i=1;i<nplanetas;i++){
        fich_salida5 << (T[i]/6.2834) << endl; //Aquí obtenemos el periodo en años
    }

    // Cerramos los ficheros
    fich_salida1.close();
    fich_salida2.close();
    fich_salida3.close();
    fich_salida4.close();


    return 0;
    
}

//////Declaración de Funciones para construir el algoritmo de Verlet///////

// 1) Función que nos va a permitir reescalar la masa
void ReescalamientoMasa (double m[], int n)
{
    int i;
    
    for (i=0;i<n;i++){
        m[i]=m[i]/(Ms);
    }

    return;
}

// 2) Función que nos va a permitir reescalar la posición
void ReescalamientoPosicion (double x[], int n)
{
    int i;

    for (i=0;i<n; i++){
        x[i]=x[i]/(c);
    }
    return;
}

// 3) Función para reescalar la velocidad
void ReescalamientoVelociad (double v[],int n)
{
    int i;

    for(i=0;i<n;i++){
        v[i]=sqrt((c)/((G)*(Ms)))*v[i];
    }

    return;
}

// 4) Función para leer los datos de un fichero de texto
bool LeerFichero (double x[], string nombre, int n)
{
    ifstream fich;
    int i;
    
    fich.open(nombre.c_str());
    if(fich.is_open())
    {
        while(!fich.eof())
        {
            for (i=0;i<n;i++){
                fich >> x[i];
            }
        }
        fich.close();
        return true;
    }
    else return false;
}

// 5) Función para almacenar datos en un fichero (2 columnas)
void AlmacenaDatos (double x[], double y[], string nombre, int n)
{
    int i;
    ofstream fich;

    fich.open(nombre.c_str());
    if(fich.is_open() == true)
    {
        for (i=0;i<n;i++){
            fich << x[i] << " , " << y[i] << endl;
        }

        fich.close();
    }
    else cout << "Error al abrir el fichero";

    return;
}

// 6) Función para mostrar los datos de un fichero
bool MostrarFichero (string nombre, int n)
{
    ifstream fich;
    int i;
    double x[n];

    fich.open(nombre.c_str());
    if (fich.is_open())
    {
        while (!fich.eof()){
            for(i=0;i<n;i++){
                fich >> x[i];
                cout << x[i] << endl;
            }
        }
        fich.close();
        return true;
    }
    else return false;
}

// 7) Función para mostrar un Vector
void MuestraVector (double x[],int n)
{
    int i;
    
    for (i=0;i<n;i++){
        cout << x[i] << endl;
    }

    return;
}

// 8) Función que nos va a permitir calcular la aceleración
//utilizando la segunda ecuación de Newton
void CalculaAceleracion (double ax[],double ay[],double m [],double x[],double y[],int n )
{
    int i,j;

    for (i=0;i<n;i++){
        for (j=0;j<n;j++){
            if(i!=j){
                ax[i]=ax[i]-m[j]*(x[i]-x[j])*pow(pow(x[i]-x[j],2)+pow(y[i]-y[j],2),-1.5);
                ay[i]=ay[i]-m[j]*(y[i]-y[j])*pow(pow(x[i]-x[j],2)+ pow(y[i]-y[j],2),-1.5);
            }
        }
    }

    return;
}

// 9) Función que va a calcular la posición con el paso dado por teclado 
// para la órbita de los planetas
void CalculaPosicion(double x[],double v[],double a[],int n,double h)
{
    int i;

    // Comenzamos en 1 al considerar al Sol en el origen de coordenadas
    for (i=0;i<n;i++){
        x[i]=x[i]+h*v[i]+0.5*pow(h,2)*a[i];
    }

    return;
}

// 10) Función para calcular la velocidad
void CalculaVelocidad(double v[],double a1[],double a2[],int n,double h)
{
    int i;

    for (i=0;i<n;i++){
        v[i]=v[i]+0.5*h*(a1[i]+a2[i]);
    }

    return;
}

// 11) Función para intercambiar datos
void Intercambio(double a1x[],double a1y[],double a2x[],double a2y[],int n)
{
    int i;

    for (i=0;i<n;i++){
        a1x[i]=a2x[i];
        a1y[i]=a2y[i];
    }

    return;
}

// 12) Función para el cálculo de la energía
double CalculoEnergia (double x[],double y[],double vx[],double vy[],double m[],int n)
{
    int i,j;
    double en;
    
    en=0;
    //Ahora vamos a calcular la energia a partir de la suma de cinética y potencial
    for (i=0;i<n;i++){
        if(i!=0){ //Hacemos esto debido a que el Sol no se mueve
            en=en+0.5*m[i]*(pow(vx[i],2)+pow(vy[i],2));
            for (j=0;j<n;j++){
                if (i!=j){
                    en=en-m[i]*m[j]*pow(pow(x[i]-x[j],2)+pow(y[i]-y[j],2),-0.5);
                }
                
            }
        }
    }

    //Devolvemos el valor de la energia
    return en;
}

// 13) Función para calcular el momento el momento angular
double CalculoMomento (double x[],double y[],double vx[],double vy[],double m[],int n)
{
    int i;
    double L;

    L=0;

    for (i=0;i<n;i++){
        L=L+m[i]*(x[i]*vy[i]-y[i]*vx[i]);
    }

    return L;
}

// 14) Función para calcular el periodo de los planetas
void CalculaPeriodo (double y[],int cont [], double T[], double tvuelta [], int n,double t)
{
    int i;

    for(i=1;i<n;i++){
        if(cont[i]%2==1){
            if(y[i]-y[0]>0){
                cont[i]++;
                T[i]=((cont[i]/2-1)*T[i]+(t-tvuelta[i]))/(cont[i]*0.5);
                tvuelta[i]=t;
            }
        }

        if (cont[i]%2==0){
            if(y[i]-y[0]<0){
                cont[i]++;
            }
        }
    }

    return;
}














