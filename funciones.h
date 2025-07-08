/*
 * PREGUNTA DE PROFESOR: ¿Cuál es el propósito de este archivo de cabecera (.h)?
 *
 * RESPUESTA:
 * 1. ¿QUÉ HACE?: Este archivo centraliza las definiciones y declaraciones que son compartidas por varios archivos fuente (.c).
 *    - Define las ESTRUCTURAS DE DATOS (`RegistroDia`, `Zona`) que son el esqueleto de nuestro programa.
 *    - Define CONSTANTES (`MAX_ZONAS`, etc.) para facilitar el mantenimiento y evitar "números mágicos" en el código.
 *    - Declara los PROTOTIPOS DE FUNCIONES, que le dicen al compilador qué funciones existen, qué parámetros reciben y qué devuelven.
 *      Esto permite que `main.c` pueda llamar a funciones que están implementadas en `funciones.c`.
 *
 * 2. ¿QUÉ PASA SI SE BORRA?: Sin este archivo, `main.c` no sabría de la existencia de la estructura `Zona` ni de funciones como `mostrar_menu()`.
 *    El compilador daría errores por todos lados porque no podría encontrar las definiciones. Sería necesario duplicar las definiciones
 *    en cada archivo .c, lo cual sería una pésima práctica de programación y dificultaría enormemente el mantenimiento.
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

// --- CONSTANTES GLOBALES ---
/*
 * PREGUNTA DE PROFESOR: ¿Por qué usar constantes definidas con #define en lugar de números directamente en el código?
 * RESPUESTA: Usar constantes mejora la legibilidad y el mantenimiento. Si quisiéramos cambiar el número máximo de zonas de 10 a 20,
 * solo necesitamos cambiarlo en esta línea. Si usáramos el número 10 directamente en el código (un "número mágico"),
 * tendríamos que buscar y reemplazar cada instancia, con el riesgo de cometer errores.
 */
#define MAX_ZONAS 10
#define DIAS_HISTORIAL 7
#define NOMBRE_ZONA 50

// --- ESTRUCTURAS DE DATOS ---

// Representa el registro de contaminación y clima para un solo día.
// Agrupa todos los datos de una medición en una sola entidad.
typedef struct {
    char fecha[11]; // Formato YYYY-MM-DD
    float pm25, pm10, co2, so2, no2;
    float temperatura, humedad, velocidad_viento;
} RegistroDia;

// Representa una zona de monitoreo.
// Contiene el nombre de la zona y un array de registros diarios que funciona como su historial.
typedef struct {
    char nombre[NOMBRE_ZONA];
    int dias_registrados;
    RegistroDia historial[DIAS_HISTORIAL];
} Zona;

// --- PROTOTIPOS DE FUNCIONES ---
int cargar_zonas(Zona zonas[], int *num_zonas);
int guardar_zonas(Zona zonas[], int num_zonas);
void mostrar_menu();
void mostrar_estado_actual(Zona zonas[], int num_zonas);
void mostrar_predicciones(Zona zonas[], int num_zonas);
void ingresar_datos_actuales(Zona zonas[], int num_zonas);
void mostrar_info_zonas(Zona zonas[], int num_zonas);
void generar_alertas_y_recomendaciones(Zona zonas[], int num_zonas);
void generar_reporte(Zona zonas[], int num_zonas);
void exportar_respaldo(Zona zonas[], int num_zonas);
void anadir_zona(Zona zonas[], int *num_zonas);
void editar_zona(Zona zonas[], int num_zonas);
void eliminar_zona(Zona zonas[], int *num_zonas);
void reiniciar_programa();
int validar_float(float valor, float min, float max);
int leer_float(const char *mensaje, float min, float max, float *valor);
int leer_int(const char *mensaje, int min, int max, int *valor);
void limpiar_buffer();

#endif
