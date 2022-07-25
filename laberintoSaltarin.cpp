#include <iostream>
#include <cmath>
using namespace std;
//estructura de nodos para recorrer el laberinto
typedef struct Nodo{
    int h;                  //heuristica
    int coord[2];           //coordenadas en el laberinto
    int val_salto;          //valor en el laberinto
    bool visitado=false;    //si ha sido visitado anteriormente
    Nodo** siguientes;      //arreglo de Nodo* que apunta a los nodos posibles para saltar
    Nodo* anterior=NULL;    //nodo anterior para volver en caso de no poder saltar
};
//se crea el nodo inicial
Nodo* crearNodoInicio(int fil, int col, int valSalto){
    Nodo* nodo=new Nodo;
    nodo->coord[0]=fil;         //coordenada fila del nodo inicial
    nodo->coord[1]=col;         //coordenada columna del nodo inicial
    nodo->val_salto=valSalto;   //valor en el laberinto
    return nodo;
}
//se crea un nodo
Nodo* crearNodo(int fil, int col,int valSalto, Nodo* nodoAct){
    Nodo* nodo=new Nodo;
    nodo->coord[0]=fil;         //coordenada fila del nodo
    nodo->coord[1]=col;         //coordenada columna del nodo
    nodo->anterior=nodoAct;     //puntero al nodo anterior en caso de necesitar volver
    nodo->val_salto=valSalto;   //valor en el laberinto
    return nodo;
}
//obtiene los aprametros ingresados por el usuario
int getParams(string input, int* params){
    int inputSize=input.length(), i=0, k=0;
    string aux;
    while(i<inputSize){ //Guardo uno a uno los parametros en el string aux
        while(input[i]!=' ' && i<inputSize){ // mientras no se encuentre un espacio y no sobrepase el tamaño del input
            //cout<<"input "<<input[i]<<endl;
            aux.push_back(input[i]);
            i++;
        }
        if(aux.size()!=0){ //si hay algún parametro
            params[k]= stoi(aux); //lo transformo de string a int y lo guardo en params
            aux.clear();
            k++;
        }
        i++;
    }
    if(k!=6){   //verifico que hayan 6 parámetros
        cout<<"Numero incorrecto de parametros: "<<k<<" de 6"<<endl;
        return 1;
    }
    for(int z=0;z<6;z++){ //verifico que sean valores positivos
        if(params[z]<0){
            cout<<"Ingrese valores positivos -> "<<params[z]<<endl;
            return 1;
        }
    }
    //verifico que sean valores validos dentro del laberinto
    if(params[0]==0){
        cout<<"Indice [m] invalido -> "<<params[0]<<" Ingrese un valor mayor que cero"<<endl;
        return 1;
    }else if(params[1]==0){
        cout<<"Indice [n] invalido -> "<<params[1]<<" Ingrese un valor mayor que cero"<<endl;
        return 1;
    }else if(params[2]>=params[0]){
        cout<<"Indice [Fila inicio] invalido -> "<<params[2]<<" Ingrese un valor menor que "<<params[0]<<endl;
        return 1;
    }else if(params[3]>=params[1]){
        cout<<"Indice [Columna inicio] invalido -> "<<params[3]<<" Ingrese un valor menor que "<<params[1]<<endl;
        return 1;
    }else if(params[4]>=params[0]){
        cout<<"Indice [Fila destino] invalido -> "<<params[4]<<" Ingrese un valor menor que "<<params[0]<<endl;
        return 1;
    }else if(params[5]>=params[1]){
        cout<<"Indice [Columna dstino] invalido -> "<<params[5]<<" Ingrese un valor menor que "<<params[1]<<endl;
        return 1;
    }
    return 0;
}
int** crearLaberinto(int* params){
    int m=params[0],n=params[1], filSol=params[4],colSol=params[5];
    int** labyrinth=new int*[m];
    for(int z=0;z<m;z++){
        labyrinth[z]=new int[n];
    }
    if(n==1){   //Si n es 1 el rand es %n +1 osea coloca solo 1
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                labyrinth[i][j] = rand()%n +1;
            }
        }
    }else if(m==1){ //Si m es 1 el rand es %m +1 osea coloca solo 1
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                labyrinth[i][j] = rand()%m +1;
            }
        }
    }else if(m<n){  //Si n es mayor, entonces el rango de numeros será desde 1 hasta m-1
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                labyrinth[i][j]=rand()%(m-1) +1;
            }
        }
    }else if(n<m){//Si m es mayor, entonces el rango de numeros será desde 1 hasta n-1
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                labyrinth[i][j]=rand()%(n-1) +1;
            }
        }
    }else{//Si ambos son iguales, entonces el rango de numeros será desde 1 hasta n-1
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                labyrinth[i][j]=rand()%(n-1) +1;
            }
        }
    }
    labyrinth[filSol][colSol]=0; //Coloco un 0 representando el objetivo en el laberinto
    return labyrinth;
}
/*
void ejemplo(){
    int labyrinth[5][5]={
            {4,3,2,4,2},
            {4,3,1,2,2},
            {3,4,3,4,4},
            {3,1,3,1,1},
            {4,1,4,2,0}
    };
    cout<<"LABERINTO"<<endl;
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            cout<<labyrinth[i][j]<<' ';
        }
        cout<<endl;
    }
}
*/
//imprime el laberinto
void printLaberinto(int** l, int m, int n) {
    cout<<"LABERINTO"<<endl;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cout<<l[i][j]<<' ';
        }
        cout<<endl;
    }
}
//Calcula la heuristica para escoger el nodo a saltar
void heuristica(int filSol,int colSol, Nodo* nodo){
    nodo->h=sqrt(pow(filSol-(nodo->coord[0]),2)+pow(colSol-(nodo->coord[1]),2)); //Distancia entre 2 puntos
}
//algoritmo de búsqueda
int algBusq(int** l, int* params, Nodo* nodoInicio){
    Nodo* saltarin=nodoInicio;
    Nodo* arriba=NULL;
    Nodo* abajo=NULL;
    Nodo* izquierda=NULL;
    Nodo* derecha=NULL;
    Nodo* aux=NULL;
    int valSalto=saltarin->val_salto, m=params[0], n=params[1], saltos=0, num_nodos=0, i=0;
    int filSalto, colSalto, filSol=params[4], colSol=params[5];
    int opciones=0;
    //creo una matriz para guardar los nodos visitados
    Nodo*** lvisitado=new Nodo**[m];
    for(int z=0;z<m;z++){
        lvisitado[z]=new Nodo*[n];
        for(int j=0;j<n;j++){
            lvisitado[z][j]=NULL;
        }
    }
    //salta por el laberinto hasta encontrar el 0, o sea, la solucion
    while( l[saltarin->coord[0]][saltarin->coord[1]]!=0 ){
        //verifico si se pueden generar nodos
        valSalto=saltarin->val_salto;
        if(saltarin->coord[0]+valSalto<m && lvisitado[saltarin->coord[0]+valSalto][saltarin->coord[1]]==NULL){
            //crear nodo abajo
            filSalto=saltarin->coord[0]+valSalto;
            colSalto=saltarin->coord[1];
            abajo=crearNodo(filSalto,colSalto,l[filSalto][colSalto],saltarin);
            num_nodos++;
        }
        if(saltarin->coord[0]-valSalto>=0 && lvisitado[saltarin->coord[0]-valSalto][saltarin->coord[1]]==NULL){
            //crear nodo arriba
            filSalto=saltarin->coord[0]-valSalto;
            colSalto=saltarin->coord[1];
            arriba=crearNodo(filSalto,colSalto,l[filSalto][colSalto],saltarin);
            num_nodos++;
        }
        if(saltarin->coord[1]+valSalto<n && lvisitado[saltarin->coord[0]][saltarin->coord[1]+valSalto]==NULL){
            //crear nodo derecha
            filSalto=saltarin->coord[0];
            colSalto=saltarin->coord[1]+valSalto;
            derecha=crearNodo(filSalto,colSalto,l[filSalto][colSalto],saltarin);
            num_nodos++;
        }
        if(saltarin->coord[1]-valSalto>=0 && lvisitado[saltarin->coord[0]][saltarin->coord[1]-valSalto]==NULL){
            //crear nodo izquierda
            filSalto=saltarin->coord[0];
            colSalto=saltarin->coord[1]-valSalto;
            izquierda=crearNodo(filSalto,colSalto,l[filSalto][colSalto],saltarin);
            num_nodos++;
        }
        //creo el arreglo de nodos generados
        //si se genera a lo menos 1 nodo
        if(num_nodos>=1){
            if(saltarin->siguientes==NULL){
                saltarin->siguientes=new Nodo*[num_nodos];
            }
            i=0;
            //almaceno los punteros a los nodos en el arreglo siguientes
            if(abajo!=NULL){
                saltarin->siguientes[i]=abajo;
                i++;
            }
            if(arriba!=NULL){
                saltarin->siguientes[i]=arriba;
                i++;
            }
            if(derecha!=NULL){
                saltarin->siguientes[i]=derecha;
                i++;
            }
            if(izquierda!=NULL){
                saltarin->siguientes[i]=izquierda;
                i++;
            }
            //calculo heurística de nodos
            for(i=0;i<num_nodos;i++){
                heuristica(filSol,colSol,saltarin->siguientes[i]);
            }
            //ordeno los nodos de menor a mayor heuristica
            for(int z=0;z<num_nodos-1;z++){
                for(int j=1;j<num_nodos;j++){
                    if(saltarin->siguientes[z]->h>saltarin->siguientes[j]->h){
                        aux=saltarin->siguientes[z];
                        saltarin->siguientes[z]=saltarin->siguientes[j];
                        saltarin->siguientes[j]=aux;
                    }
                }
            }
            saltarin->visitado=true;
            lvisitado[saltarin->coord[0]][saltarin->coord[1]]=saltarin;
            //en caso de haber vuelto al salto anterior
            while(saltarin->siguientes[opciones]->visitado==true && opciones< num_nodos){
                opciones++;
            }
            //si todas las opciones ya fueron visitadas
            if(opciones==num_nodos){
                saltarin=saltarin->anterior;
                saltos--;
            }else if(saltarin->siguientes[opciones]->visitado==false){
                saltarin=saltarin->siguientes[opciones];
                saltos++;

            }
        }else{
            //vuelvo a atras;
            saltarin->visitado=true;
            lvisitado[saltarin->coord[0]][saltarin->coord[1]]=saltarin;
            saltarin=saltarin->anterior;
            saltos--;
            if(saltos<0){
                return 0;
            }
        }
        //REINICIO DE VARIABLES
        arriba=NULL;
        abajo=NULL;
        izquierda=NULL;
        derecha=NULL;
        opciones=0;
        num_nodos=0;
    }
    delete[] nodoInicio->siguientes;
    delete[] lvisitado;
    return saltos;
}

int main() {
    int** laberinto;
    int parametros[6];
    int sol;
    string input;
    cout<<"Ingrese parametros: (Numero de filas)[m] (Numero de columnas)[n] [Fila inicio] [Columna inicio] [Fila destino] [Columna destino] separados por un espacio"<<endl;
    getline(cin,input);
    //se obtienen los parametros ingresados por el usuario
    if(getParams(input,parametros)){
        return 0;
    }
    //se crea e imprime el laberinto
    laberinto=crearLaberinto(parametros);
    printLaberinto(laberinto,parametros[0],parametros[1]);
    //Se crea el nodo de inicio del laberinto
    Nodo* nodoInicio=crearNodoInicio(parametros[2],parametros[3],laberinto[parametros[2]][parametros[3]]);
    //se busca el camino a la solución
    if(parametros[2]==parametros[4] && parametros[3]==parametros[5]){
        cout<<"Saltos: 0 -> Celda inicio es igual a celda solucion"<<endl;
        delete[] laberinto;
        delete[] nodoInicio;
        return 0;
    }else{
        sol=algBusq(laberinto,parametros,nodoInicio);
    }
    if(sol){
        cout<<"Saltos: "<<sol<<endl;
    }else{
        cout<<"No se encontro solucion"<<endl;
    }
    delete[] laberinto;
    delete[] nodoInicio;
    return 0;
}
