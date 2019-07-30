#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int obtenerNumero(int opcMin, int opcMax)
{

    // Declaracion de variables
    int opcion = 0;
    string line;

    // Metodo para prevenir que el usuario ingrese una opcion invalida
    // como opciones fuera de un rango de opciones o texto
    while (getline(cin, line))
    {
        stringstream linestream(line);
        if (!(linestream >> opcion))
        {
            cout << "Opcion invalida. Intentalo de nuevo." << endl;
            continue;
        }
        if ((opcion < opcMin) || (opcion > opcMax))
        {
            cout << "Opcion invalida. Intentalo de nuevo." << endl;
            continue;
        }
        char errorTest;
        if (linestream >> errorTest)
        {
            cout << "Opcion invalida. Intentalo de nuevo." << endl;
            continue;
        }
        break;
    }

    return opcion;
}

int **generarMatrizAleatoria(unsigned tamano)
{
    // Establecer semilla del random
    srand(time(NULL));
    // Esta funcion genera un array de dos dimensiones
    // Empieza generando un puntero
    int **arr = new int *[tamano];
    int inicializadores = 2;
    do
    {
        inicializadores = 2;
        for (int i = 0; i < tamano; i++)
        {
            arr[i] = new int[tamano];
            for (int j = 0; j < tamano; j++)
            {
                // Por probabilidad asignar un 2 en una de cada
                // cinco celdas de la matriz, solamente a dos celdas
                // de toda la matriz
                int num;
                if (inicializadores > 0)
                {
                    num = ((rand() % 100) < 20) ? 2 : 0;
                    // Si el numero aleatorio fue 2, entonces queda un
                    // numero inicializador menos que poner en la
                    // matriz
                    if (num == 2)
                        inicializadores--;
                }
                else
                {
                    num = 0;
                }
                arr[i][j] = num;
            }
        }
        // Si los inicializadores no fueron correctamente puestos, entonces
        // reiniciar el proceso de generacion de matriz
    } while (inicializadores > 0);
    return arr;
}

void imprimirMatriz(int **array, int tamano)
{
    // Funcion auxiliar para imprimir en consola un
    // arreglo de dos dimensiones
    for (int i = 0; i < tamano; i++)
    {
        for (int j = 0; j < tamano; j++)
        {
            cout << array[i][j] << ", ";
        }
        cout << endl;
    }
}

string obtenerFecha()
{
    // Funcion que devuelve la fecha y hora actual
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", timeinfo);
    string str(buffer);

    return str;
}

string crearCSV(int **matrizJuego, int tamano)
{
    // Funcion donde en base a la fecha y hora se genera un CSV
    // con la matriz de juego brindada
    string nombreArchivo = "..\\data\\juego_" + obtenerFecha() + ".csv";
    ofstream myfile;
    myfile.open(nombreArchivo);

    // Llenar de datos el CSV con la matriz brindada
    for (int i = 0; i < tamano; i++)
    {
        for (int j = 0; j < tamano; j++)
        {
            if (j == (tamano - 1))
            {
                myfile << matrizJuego[i][j] << "\n";
            }
            else
            {
                myfile << matrizJuego[i][j] << ",";
            }
        }
    }

    myfile.close();
    return nombreArchivo;
}

string generarTablero()
{
    // Funcion para generar el tablero del juego
    // el cual se guardara en un CSV para poderse
    // volver a cargar si se guarda el juego

    // Limpiando pantalla
    system("CLS");

    // Ingresar el tamano del tablero de tipo N x N
    cout << "Ingresar el tamano de la tabla:" << endl;
    int tamano;
    cin >> tamano;

    // Generar matiz de numeros con dos numeros inicializadores
    // del juego
    int **arr = generarMatrizAleatoria(tamano);

    // Crear archivo nuevo CSV para guardar el tablero
    string nombreTablero = crearCSV(arr, tamano);

    return nombreTablero;
}

vector<vector<int>> leerTablero(string nombreTablero)
{
    ifstream in(nombreTablero);

    string line, field;

    vector<vector<int>> tablero;
    vector<int> v;

    while (getline(in, line))
    {
        v.clear();
        stringstream ss(line);

        while (getline(ss, field, ','))
        {
            int num = atoi(field.c_str());
            v.push_back(num);
        }

        tablero.push_back(v);
    }

    return tablero;
}

int checarLleno(vector<vector<int>>& tablero) {
    int tamano = tablero.size();
    int lleno = 1;
    for (int i = 1; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (tablero[i][j] == 0) {
                lleno = 0;
            }
        }
    }
    return lleno;
}

int moverArriba(vector<vector<int>>& tablero)
{
    // Status juego es el estado en el que se encuentre el juego
    // 0 -> Normal
    // 1 -> Perdio
    // 2 -> Gano
    int statusJuego = 0;

    int tamano = tablero.size();

    // Recorrer desde la parte de abajo los numeros
    // si un numero es igual al de arriba se suma
    // de lo contrario se queda ahi el numero
    // si no hay numero sube un espacio
    for (int i = 1; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            int tmpIndex = i;
            for (int k = 0; k < i; k++) {
                // Si ya hay una celda con un numero antes al presente
                // actualizar con la suma de ambos
                if (tablero[tmpIndex][j] == 0) continue;
                if (tablero[tmpIndex - 1][j] == 0) {
                    tablero[tmpIndex - 1][j] = tablero[tmpIndex][j];
                    tablero[tmpIndex][j] = 0;
                } else if (tablero[tmpIndex - 1][j] == tablero[tmpIndex][j]) {
                    tablero[tmpIndex - 1][j] += tablero[tmpIndex][j];
                    if (tablero[tmpIndex - 1][j] == 2048) {
                        statusJuego = 2;
                    }
                    tablero[tmpIndex][j] = 0;
                }
                tmpIndex--;
            }
        }
    }

    // Poner nuevo numero en el tablero (2 o 4)
    int numeroPuesto = 0;
    while (numeroPuesto == 0)
    {
        if (checarLleno(tablero) == 1) {
            break;
        }
        int renglon = rand() % (tamano);
        int columna = rand() % (tamano);
        if (tablero[renglon][columna] == 0) {
            int numAleatorio = ((rand() % 100) < 50) ? 2 : 4;
            tablero[renglon][columna] = numAleatorio;
            numeroPuesto = 1;
        }
    }

    return statusJuego;
}

int moverIzquierda(vector<vector<int>>& tablero)
{
    // Status juego es el estado en el que se encuentre el juego
    // 0 -> Normal
    // 1 -> Perdio
    // 2 -> Gano
    int statusJuego = 0;

    int tamano = tablero.size();

    // Recorrer desde la parte de abajo los numeros
    // si un numero es igual al de arriba se suma
    // de lo contrario se queda ahi el numero
    // si no hay numero sube un espacio
    for (int i = 0; i < tamano; i++) {
        for (int j = 1; j < tamano; j++) {
            int tmpIndex = j;
            for (int k = 0; k < j; k++) {
                // Si ya hay una celda con un numero antes al presente
                // actualizar con la suma de ambos
                if (tablero[i][tmpIndex] == 0) continue;
                if (tablero[i][tmpIndex - 1] == 0) {
                    tablero[i][tmpIndex - 1] = tablero[i][tmpIndex];
                    tablero[i][tmpIndex] = 0;
                } else if (tablero[i][tmpIndex - 1] == tablero[i][tmpIndex]) {
                    tablero[i][tmpIndex - 1] += tablero[i][tmpIndex];
                    if (tablero[i][tmpIndex - 1] == 2048) {
                        statusJuego = 2;
                    }
                    tablero[i][tmpIndex] = 0;
                }
                tmpIndex--;
            }
        }
    }

    // Poner nuevo numero en el tablero (2 o 4)
    int numeroPuesto = 0;
    while (numeroPuesto == 0)
    {
        if (checarLleno(tablero) == 1) {
            break;
        }
        int renglon = rand() % (tamano);
        int columna = rand() % (tamano);
        if (tablero[renglon][columna] == 0) {
            int numAleatorio = ((rand() % 100) < 50) ? 2 : 4;
            tablero[renglon][columna] = numAleatorio;
            numeroPuesto = 1;
        }
    }

    return statusJuego;
}

int moverAbajo(vector<vector<int>>& tablero)
{
    // Status juego es el estado en el que se encuentre el juego
    // 0 -> Normal
    // 1 -> Perdio
    // 2 -> Gano
    int statusJuego = 0;

    int tamano = tablero.size();

    // Recorrer desde la parte de abajo los numeros
    // si un numero es igual al de arriba se suma
    // de lo contrario se queda ahi el numero
    // si no hay numero sube un espacio
    for (int i = tamano - 2; i >= 0; i--) {
        for (int j = 0; j < tamano; j++) {
            int tmpIndex = i;
            for (int k = 0; k < (tamano - 1 - i); k++) {
                // Si ya hay una celda con un numero antes al presente
                // actualizar con la suma de ambos
                if (tablero[tmpIndex][j] == 0) continue;
                if (tablero[tmpIndex + 1][j] == 0) {
                    tablero[tmpIndex + 1][j] = tablero[tmpIndex][j];
                    tablero[tmpIndex][j] = 0;
                } else if (tablero[tmpIndex + 1][j] == tablero[tmpIndex][j]) {
                    tablero[tmpIndex + 1][j] += tablero[tmpIndex][j];
                    if (tablero[tmpIndex + 1][j] == 2048) {
                        statusJuego = 2;
                    }
                    tablero[tmpIndex][j] = 0;
                }
                tmpIndex++;
            }
        }
    }

    // Poner nuevo numero en el tablero (2 o 4)
    int numeroPuesto = 0;
    while (numeroPuesto == 0)
    {
        if (checarLleno(tablero) == 1) {
            break;
        }
        int renglon = rand() % (tamano);
        int columna = rand() % (tamano);
        if (tablero[renglon][columna] == 0) {
            int numAleatorio = ((rand() % 100) < 50) ? 2 : 4;
            tablero[renglon][columna] = numAleatorio;
            numeroPuesto = 1;
        }
    }

    return statusJuego;
}

int moverDerecha(vector<vector<int>>& tablero)
{
    // Status juego es el estado en el que se encuentre el juego
    // 0 -> Normal
    // 1 -> Perdio
    // 2 -> Gano
    int statusJuego = 0;

    int tamano = tablero.size();

    // Recorrer desde la parte de abajo los numeros
    // si un numero es igual al de arriba se suma
    // de lo contrario se queda ahi el numero
    // si no hay numero sube un espacio
    for (int i = 0; i < tamano; i++) {
        for (int j = tamano - 2; j >= 0; j--) {
            int tmpIndex = j;
            for (int k = 0; k < (tamano - 1 - j); k++) {
                // Si ya hay una celda con un numero antes al presente
                // actualizar con la suma de ambos
                if (tablero[i][tmpIndex] == 0) continue;
                if (tablero[i][tmpIndex + 1] == 0) {
                    tablero[i][tmpIndex + 1] = tablero[i][tmpIndex];
                    tablero[i][tmpIndex] = 0;
                } else if (tablero[i][tmpIndex + 1] == tablero[i][tmpIndex]) {
                    tablero[i][tmpIndex + 1] += tablero[i][tmpIndex];
                    if (tablero[i][tmpIndex + 1] == 2048) {
                        statusJuego = 2;
                    }
                    tablero[i][tmpIndex] = 0;
                }
                tmpIndex++;
            }
        }
    }

    // Poner nuevo numero en el tablero (2 o 4)
    int numeroPuesto = 0;
    while (numeroPuesto == 0)
    {
        if (checarLleno(tablero) == 1) {
            break;
        }
        int renglon = rand() % (tamano);
        int columna = rand() % (tamano);
        if (tablero[renglon][columna] == 0) {
            int numAleatorio = ((rand() % 100) < 50) ? 2 : 4;
            tablero[renglon][columna] = numAleatorio;
            numeroPuesto = 1;
        }
    }

    return statusJuego;
}


void jugar(string nombreTablero)
{
    // Obtener tablero previamente guardado
    vector<vector<int>> tablero = leerTablero(nombreTablero);

    // Guardar tamano del tablero en variable
    int tamano = tablero.size();

    // Variable para marcar si el juego ha terminado
    bool juegoTerminado = false;

    // Jugar mientras sea posible
    do {
        // Imprimir el tablero
        system("CLS");
        cout << "Tablero Actual" << endl;
        for (int i = 0; i < tamano; i++)
        {
            for (int j = 0; j < tamano; j++)
            {
                if (tablero[i][j] == 0)
                {
                    cout << "."
                        << "\t";
                }
                else
                {
                    cout << tablero[i][j] << "\t";
                }
            }
            cout << endl;
        }
        cout << "OPCIONES: (w: arriba) (s: abajo) (a: izquierda) (d: derecha) (q: salir)" << endl;

        // Leer movimiento y de acuerdo a este seleccionar una accion
        // w: arriba
        // a: izquierda
        // s: abajo
        // d: derecha
        string movimiento;
        cin >> movimiento;

        // Status juego es el estado en el que se encuentre el juego
        // 0 -> Normal
        // 1 -> Perdio
        // 2 -> Gano
        int statusJuego = 0;

        if (movimiento == "w")
        {
            statusJuego = moverArriba(tablero);
        } else if (movimiento == "s")
        {
            statusJuego = moverAbajo(tablero);
        } else if (movimiento == "a")
        {
            statusJuego = moverIzquierda(tablero);
        } else if (movimiento == "d")
        {
            statusJuego = moverDerecha(tablero);
        } else if (movimiento == "q")
        {
            break;
        } else {
            cout << "Opcion invalida. Vuelva a intentarlo" << endl;
            system("pause");
        }
        // Checar Status del juego
        if (statusJuego == 2) {
            cout << "Felicidades! Usted acaba de ganar." << endl;
        }
    } while(!juegoTerminado);
    
}

void nuevoJuego()
{
    // Limpiando pantalla
    system("CLS");

    // Mensajes de bienvenida al juego
    cout << "Se inicio un nuevo juego." << endl;
    cout << "Generando tabla nueva aleatoria" << endl;

    // Se genera y obtiene el nombre del tablero (CSV) en el que se va
    // a jugar
    string nombreTablero = generarTablero();

    // Logica del juego, requiere del nombre del archivo CSV (que sirve de tablero)
    // para estar haciendo movimientos y guardarlos constantemente
    jugar(nombreTablero);
}

void cargarJuego()
{
    cout << "Cargando juego." << endl;
}

void puntajesJuego()
{
    cout << "Viendo los puntajes del juego." << endl;
}

void instruccionesJuego()
{
    cout << "Viendo instrucciones del juego." << endl;
}

void menu()
{
    // Declaracion de variables
    int opcion = 0;

    while (true)
    {
        // Limpiar pantalla
        system("CLS");

        // Menu de opciones
        cout << "...Bienvenido al juego 2048..." << endl;
        cout << "Selecciona una de las siguientes opciones:" << endl;
        cout << "1. Nuevo juego" << endl;
        cout << "2. Cargar juego" << endl;
        cout << "3. Ver puntajes" << endl;
        cout << "4. Instrucciones" << endl;
        cout << "5. Salir" << endl;
        cout << "Ingresa una de las opciones anteriores a continuacion: " << endl;

        // Obtener respuesta del jugador a traves del teclado...
        opcion = obtenerNumero(1, 5);

        // Serie de opciones del juego a llamar
        switch (opcion)
        {
        case 1:
            nuevoJuego();
            break;
        case 2:
            cargarJuego();
            break;
        case 3:
            puntajesJuego();
            break;
        case 4:
            instruccionesJuego();
            break;
        case 5:
            cout << "Gracias por jugar. Adios." << endl;
            return;
        default:
            cout << "Opcion invalida. Intentalo de nuevo." << endl;
            break;
        }
    }
}

int main()
{
    // Llamar funcion de menu del juego
    menu();

    // Terminar aplicacion correctamente
    return 0;
}