#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>

// Prototipos de funcion
void menu();
void consultar_producto();
void encontrar_producto_mas_barato();
void reporte_bajo_stock();
void actualizar_stock();
int verificar_numero_entero(std::istream&);
void agregar_producto();
void guardar_archivo(std::string);
bool codigo_existe(int);
void leer_archivo(std::string);

// Variables globales
const int TAM_MAX = 100;

int prod_codigos[TAM_MAX];
std::string prod_nombres[TAM_MAX];
int stocks[TAM_MAX];
float precios_unitarios[TAM_MAX];
std::string cod_ubicacion[TAM_MAX];

int productos_existentes;

int main() {
    int caso;
    bool loop = true;
    std::cout << "--- Bienvenido al Sistema de Inventario de El Martillo ---" << std::endl;
    std::string nombre_archivo = "inventario.csv";

    // Lectura de archivo
    leer_archivo(nombre_archivo);

    // Aplicaciones Principal
    do {
        menu();
        caso = verificar_numero_entero(std::cin);
        switch (caso) {
            case 1: // Consultar Producto
                consultar_producto();
                break;
            case 2: // Actualizar inventario por ubicacion
                actualizar_stock();
                break;
            case 3: // Registrar nuevo producto
                agregar_producto();
                break;
            case 4: // Generar reporte de bajo stock
                reporte_bajo_stock();
                break;
            case 5: // Encontrar el producto mas barato
                encontrar_producto_mas_barato();
                break;
            case 6:
                guardar_archivo(nombre_archivo);
                std::cout << "Cambios guardados. Saliendo del sistema." << std::endl;
                loop = false;
                break;
            default:
                std::cout << "WARNING: La Opcion " << caso << " no esta dentro del menu." << std::endl;
                break;
        }
    } while (loop);

    return 0;
}

// Funciones
void menu() {
    std::cout << "\n================ Menú principal ===================" << std::endl;
    std::cout << "[1] Consultar un producto" << std::endl;
    std::cout << "[2] Actualizar inventario por ubicación" << std::endl;
    std::cout << "[3] Registrar nuevo producto" << std::endl;
    std::cout << "[4] Generar reporte de bajo stock" << std::endl;
    std::cout << "[5] Encontrar el producto más barato" << std::endl;
    std::cout << "[6] Guardar y salir" << std::endl;
    std::cout << "===================================================" << std::endl;
    std::cout << "\nOpcion Seleccionada: ";
}

/**
 * Funcion para leer un archivo y guardar los datos en vectores paralelos.
 * @param nombre Nombre del archivo que se desea leer.
 */
void leer_archivo(std::string nombre) {
    std::ifstream archivo(nombre);
    std::cout << "\nCargando Inventario desde '" + nombre + "'..." << std::endl;
    if (archivo.is_open()) {
        std::string line;
        char delim = ',';
        int contador_productos = 0, contador_linea = 0;
        // getline(archivo, line);

        while (getline(archivo, line)) {

            if (contador_productos >= 100) {
                std::cerr << "WARNING: Se alcanzo el limite de 100 productos. No se leeran mas datos." << std::endl;
                break;
            }
            std::stringstream ss(line);
            std::string codigo_string, nombre, cantidad_string, precio_string, ubicacion;

            std::getline(ss, codigo_string, delim);
            std::getline(ss, ubicacion, delim);
            std::getline(ss, nombre, delim);
            std::getline(ss, cantidad_string, delim);
            std::getline(ss, precio_string, delim);

            try {
                int codigo_aux = std::stoi(codigo_string);
                int stock_aux = std::stoi(cantidad_string);
                float precio_aux = std::stof(precio_string);

                if (codigo_existe(codigo_aux)) {
                    std::cerr << "WARNING: Codigo de producto ya existente en la linea " << contador_linea << ": " << line << ". Se ha omitido la linea." << std::endl;
                    contador_linea++;
                    continue;
                }

                if (codigo_aux <= 0 || stock_aux < 0 || precio_aux < 0.0) {
                    std::cerr << "WARNING: Valores negativos dentro de la linea: " << contador_linea << ": " << line << ". Se ha omitido la linea." << std::endl;
                    contador_linea++;
                    continue;
                }

                if (stock_aux > 50) {
                    std::cerr << "WARNING: Hay sobre stock de el producto: " << nombre << std::endl;
                }

                prod_codigos[contador_productos] = std::stoi(codigo_string);
                prod_nombres[contador_productos] = nombre;
                stocks[contador_productos] = std::stoi(cantidad_string);
                precios_unitarios[contador_productos] = std::stof(precio_string);
                cod_ubicacion[contador_productos] = ubicacion;

                contador_productos++;
                contador_linea++;
                productos_existentes = contador_productos;
            } catch (const std::invalid_argument& e) {
                std::cerr << "WARNING: Error de formato en la linea " << contador_linea << ": " << line << ". Se ha omitido la linea." << std::endl;
                contador_linea++;
            } catch (const std::out_of_range& e) {
                std::cerr << "WARNING: Valor fuera del rango en la linea " << contador_linea << ": " << line << ". Se ha omitido la linea." << std::endl;
                contador_linea++;
            }
        }
        archivo.close();
        std::cout << "Inventario cargado exitosamente. " << productos_existentes << " productos encontrados." << std::endl;
    } else {
        std::cout << "WARNING: El archivo no se pudo cargar. Se inicializara la aplicacion con 0 productos." << std::endl;
    }
}

/**
 * FUncion para guardar datos de vectores paralelos en un archivo con formato
 * @param nombre_archivo Nombre del archivo que se desea crear o guardar
 */
void guardar_archivo(std::string nombre_archivo) {
    std::cout << "\nGuardando cambios en '" + nombre_archivo + "'..." << std::endl;
    std::ofstream archivo;
    archivo.open(nombre_archivo);
    if (archivo.is_open()) {
        archivo << "Código,Ubicacion,Nombre,Cantidad,Precio\n";
        for (int i = 0; i < productos_existentes; i++) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << precios_unitarios[i];
            std::string precio = ss.str();
            std::string line = std::to_string(prod_codigos[i]) + "," + cod_ubicacion[i] + "," + prod_nombres[i] + "," + std::to_string(stocks[i]) + "," + precio + "\n";
            archivo << line;
        }
        archivo.close();
    }
}

bool ubicacion_existe(std::string cod) {
    for (int i=0; i < productos_existentes; i++) {
        if (cod_ubicacion[i] == cod) return true;
    }
    return false;
}

bool codigo_existe(int cod) {
    for (int i=0; i < productos_existentes; i++) {
        if (prod_codigos[i] == cod) return true;
    }
    return false;
}

/**
 * Funcion para encontrar un indice de un array mediante la comparacion de codigos
 * @param cod Codigo de la sucursal del producto a buscar
 * @return Indice del producto encontrado o -1 si el codigo no existe
 */
int encontrar_indice_por_ubicacion(std::string cod) {
    for (int i=0; i < productos_existentes; i++) {
        if (cod_ubicacion[i] == cod) return i;
    }
    return -1;
}

/**
 * Funcion para encontrar un indice de un array mediante la comparacion de codigos
 * @param cod Codigo del producto
 * @return Indice del producto encontrado o -1 si el codigo no existe
 */
int encontrar_indice_por_codigo_producto(int cod) {
    for (int i=0; i < productos_existentes; i++) {
        if (prod_codigos[i] == cod) return i;
    }
    return -1;
}

/**
 * Funcion para actualizar el stock de un producto validando que no quede en negativo
 * @param index Indice del producto en los vectores paralelos
 * @param num Cantidad a agregar (Puede ser negativa para reducir el stock)
 */
void actualiza_inventario(int index, int num) {
    int aux;
    do {
        aux = stocks[index];
        aux += num;
        if (aux < 0) {
            std::cout << "El stock restante no puede ser negativo. Ingrese otro numero: ";
            num = verificar_numero_entero(std::cin);
        }

    } while (aux < 0);
    stocks[index] += num;
    std::cout << "Nuevo stock: " << stocks[index] << std::endl;
}

/**
 * Funcion para validar la entrada del usuario garantizando un numero entero valido.
 * @param cin Flujo de entrada para leer datos
 * @return Numero entero valido
 */
int verificar_numero_entero(std::istream& cin){
    int aux = 0;
    while(true) {
        cin >> aux;
        if(cin.fail()){
            cin.clear();
            // Descarta la linea incorrecta del buffer de la entrada
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "WARNING: Entrada invalida. Necesita ingresar un numero entero valido: ";
        } else {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return aux;
        }
    }
}

/**
 * Funcion para validar la entrada del usuario garantizando un numero flotante valido.
 * @param cin Flujo de entrada para leer datos
 * @return Numero flotante valido
 */
float verificar_numero_flotante(std::istream& cin){
    float aux = 0;
    while(true) {
        cin >> aux;
        if(cin.fail()){
            cin.clear();
            // Descarta la linea incorrecta del buffer de la entrada
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "WARNING: Entrada invalida. Necesita ingresar un numero flotante valido: ";
        } else {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return aux;
        }
    }
}

void agregar_producto() {
    if (productos_existentes == TAM_MAX) {
        std::cout << "ERROR: No se pueden agregar mas productos. El inventario esta lleno." << std::endl;
        return;
    }
    std::string nombre, ubicacion;
    int cod, stock;
    float precio;

    std::cout << "\n--- Agregar Nuevo Producto ---" << std::endl;
    do {
        std::cout << "Ingre el codigo del producto: ";
        cod = verificar_numero_entero(std::cin);
        if (codigo_existe(cod)) {
            std::cout << "WARNING: Ya existe un producto con ese codigo." << std::endl;
        }
        if (cod <= 0) {
            std::cout << "WARNING: El codigo no puede ser un numero negativo o 0." << std::endl;
        }
    } while (codigo_existe(cod) || cod <= 0);

    std::cout << "Ingrese el nombre del producto: ";
    std::getline(std::cin, nombre);

    do {
        std::cout << "Ingrese el stock del producto: ";
        stock = verificar_numero_entero(std::cin);
        if (stock <= 0) {
            std::cout << "WARNING: El stock no puede ser un numero negativo o 0." << std::endl;
        }
    } while (stock < 0);

    do {
        std::cout << "Ingrese el precio del producto: ";
        precio = verificar_numero_flotante(std::cin);
        if (precio <= 0) {
            std::cout << "WARNING: El precio no puede ser un número negativo o 0." << std::endl;
        }
    } while (precio < 0);

    do {
        std::cout << "Ingrese el codigo de la ubicacion del producto (ej. A-01): ";
        std::cin >> ubicacion;
        if (ubicacion_existe(ubicacion)) {
            std::cout << "WARNING: Ese codigo de ubicacion ya existe" << std::endl;
        }
    } while (ubicacion_existe(ubicacion));

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    prod_codigos[productos_existentes] = cod;
    prod_nombres[productos_existentes] = nombre;
    precios_unitarios[productos_existentes] = precio;
    stocks[productos_existentes] = stock;
    cod_ubicacion[productos_existentes] = ubicacion;
    productos_existentes++;

    std::cout << "\nSu producto: " + nombre + " se ha agregado correctamente." << std::endl;
}

void informacion_producto(int indice) {
    std::cout << "\n--- Informacion del Producto ---" << std::endl;
    std::cout << "Codigo del producto: " << prod_codigos[indice] << std::endl;
    std::cout << "Nombre del producto: " << prod_nombres[indice] << std::endl;
    std::cout << "Cantidad en stock: " << stocks[indice] << std::endl;
    std::cout << "Precio Unitario: " << precios_unitarios[indice] << std::endl;
    std::cout << "Codigo de Sucursal: " <<cod_ubicacion[indice] << std::endl;
}

void consultar_produto_por_ubicacion() {
    std::string cod;
    std::cout << "\nIngrese el codigo de la ubicacion que desea consultar: ";
    std::cin >> cod;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!ubicacion_existe(cod)) {
        std::cout << "\nNo existe una ubicacion con ese codigo." << std::endl;
        return;
    }

    std::cout << "\nCodigo\t | Nombre\t\t\t | Stock | Precio | Ubicacion" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    for (int i = 0 ; i < productos_existentes; i++) {
        if (cod == cod_ubicacion[i]) {
            std::cout << prod_codigos[i] << "\t| " + prod_nombres[i] + "\t\t\t | " << stocks[i] << " | $" << precios_unitarios[i] << " | " + cod_ubicacion[i] << std::endl;
        }
    }
    std::cout << "--------------------------------------------------------" << std::endl;
}

void consultar_producto_por_codigo() {
    std::cout << "\nIngrese el codigo del producto que desea consultar: ";
    int cod = verificar_numero_entero(std::cin);
    int indice = encontrar_indice_por_codigo_producto(cod);
    if (indice == -1) {
        std::cout << "\nNo existe un producto con ese codigo." << std::endl;
    } else {
        informacion_producto(indice);
    }
}

void reporte_inventario() {
    std::cout << "\n--- Reporte de Inventario ---" << std::endl;
    std::cout << "Codigo\t | Nombre\t\t\t | Stock | Precio | Ubicacion" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    for (int i = 0 ; i < productos_existentes; i++) {
        std::cout << prod_codigos[i] << "\t| " + prod_nombres[i] + "\t\t\t | " << stocks[i] << " | $" << precios_unitarios[i] << " | " + cod_ubicacion[i] << std::endl;
    }
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "\n--- Fin del Reporte ---" << std::endl;
}

void consultar_producto() {
    bool loop = true;
    do {
        std::cout << "\n[1] Consultar por codigo" << std::endl;
        std::cout << "[2] Consultar por codigo de ubicacion" << std::endl;
        std::cout << "[3] Consultar Todos" << std::endl;
        std::cout << "[0] Regresar al menu principal" << std::endl;
        std::cout << "\nOpcion Seleccionada: ";
        int caso = verificar_numero_entero(std::cin);
        switch (caso) {
            case 1:
                consultar_producto_por_codigo();
                break;
            case 2:
                consultar_produto_por_ubicacion();
                break;
            case 3:
                reporte_inventario();
                break;
            case 0:
                std::cout << "\nRegresando..." << std::endl;
                loop = false;
                break;
            default:
                std::cout << "WARNING: La Opcion " << caso << " no esta dentro del menu." << std::endl;
                break;
        }
    } while (loop);

}

void encontrar_producto_mas_barato() {
    int indice = 0;
    for (int i=0; i < productos_existentes; i++) {
        if (precios_unitarios[i] <= precios_unitarios[indice]) indice = i;
    }

    std::cout << "\nEl producto mas barato es " + prod_nombres[indice] + " con un precio de $" << precios_unitarios[indice] << std::endl;
}

void reporte_bajo_stock() {
    int umbral;
    std::cout << "Ingrese el umbral: ";
    umbral = verificar_numero_entero(std::cin);
    std::cout << "\n--- Reporte de Productos con Bajo Stock ---" << std::endl;
    for (int i = 0 ; i < productos_existentes; i++) {
        if (stocks[i] <= umbral) {
            std::cout << "Nombre: " + prod_nombres[i] + ", Stock: " << stocks[i] << std::endl;
        }
    }
    std::cout << "\n--- Fin del Reporte ---" << std::endl;
}

void actualizar_stock() {
    int indice, num;
    std::string cod;

    std::cout << "Ingrese el codigo de la ubicación del producto a actualizar: ";
    std::cin >> cod;

    indice = encontrar_indice_por_ubicacion(cod);
    if (indice == -1) {
        std::cout << "ERROR: No existe una ubicacion con ese codigo." << std::endl;
        return;
    }

    std::cout << "Ingrese la cantidad a agregar/quitar (+/-): ";
    num = verificar_numero_entero(std::cin);
    actualiza_inventario(indice, num);
}
