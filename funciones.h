#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX_ZONAS 20
#define DIAS_HISTORIAL 7
#define NOMBRE_ZONA 40

typedef struct {
    char fecha[11]; // "YYYY-MM-DD"
    float pm25, pm10, co2, so2, no2;
    float temperatura, humedad, velocidad_viento;
} RegistroDia;

typedef struct {
    char nombre[NOMBRE_ZONA];
    int dias_registrados;
    RegistroDia historial[DIAS_HISTORIAL];
} Zona;

// Carga los datos de las zonas desde el archivo de texto.
// Si se borra o modifica, el programa no podrá leer los datos guardados y podría empezar vacío o con errores.
int cargar_zonas(Zona zonas[], int *num_zonas);

// Guarda los datos de las zonas en el archivo de texto.
// Si se borra o modifica, los cambios hechos durante la sesión no se guardarán al salir.
int guardar_zonas(Zona zonas[], int num_zonas);

// Muestra el menú principal de opciones.
// Si se borra o modifica, el usuario no verá las opciones para interactuar con el programa.
void mostrar_menu();

// Muestra una tabla con el estado actual (último registro) de todas las zonas.
// Si se borra, se pierde la capacidad de ver un resumen rápido del estado general.
void mostrar_estado_actual(Zona zonas[], int num_zonas);

// Calcula y muestra una predicción a 24 horas basada en los últimos días.
// Si se borra o modifica, se pierde la funcionalidad de predicción del sistema.
void mostrar_predicciones(Zona zonas[], int num_zonas);

// Permite ingresar un nuevo registro de datos para una zona específica.
// Si se borra, no se podrán añadir nuevos datos de monitoreo al sistema.
void ingresar_datos_actuales(Zona zonas[], int num_zonas);

// Muestra el historial completo de una zona seleccionada.
// Si se borra, el usuario no podrá consultar los datos históricos de una zona en particular.
void mostrar_info_zonas(Zona zonas[], int num_zonas);

// Evalúa los datos y genera alertas si se superan los límites, con recomendaciones.
// Si se borra o modifica, el sistema pierde su capacidad de alertar sobre niveles peligrosos.
void generar_alertas_y_recomendaciones(Zona zonas[], int num_zonas);

// Crea un archivo de texto "reporte_integral.txt" con toda la información detallada.
// Si se borra, no se podrán generar reportes en formato de texto.
void generar_reporte(Zona zonas[], int num_zonas);

// Crea una copia de seguridad binaria de los datos.
// Si se borra, no se podrán crear respaldos de seguridad.
void exportar_respaldo(Zona zonas[], int num_zonas);

// Añade una nueva zona de monitoreo al sistema.
// Si se borra, no se podrán agregar más zonas al sistema.
void anadir_zona(Zona zonas[], int *num_zonas);

// Permite editar el nombre o los registros de una zona existente.
// Si se borra, no se podrán corregir errores en los datos ya ingresados.
void editar_zona(Zona zonas[], int num_zonas);

// Elimina una zona de monitoreo del sistema.
// Si se borra, no se podrán quitar zonas que ya no se necesiten.
void eliminar_zona(Zona zonas[], int *num_zonas);

// Borra el archivo de datos para reiniciar el programa desde cero.
// Si se borra, se pierde la funcionalidad de reseteo rápido.
void reiniciar_programa();

// Valida que un número flotante esté dentro de un rango (min-max).
// Es una función de ayuda. Si se borra, las funciones que leen datos podrían aceptar valores inválidos.
int validar_float(float valor, float min, float max);

// Lee de forma segura un número flotante desde el teclado.
// Es una función de ayuda crítica. Si se borra, la entrada de datos numéricos fallará.
int leer_float(const char *mensaje, float min, float max, float *valor);

// Lee de forma segura un número entero desde el teclado.
// Es una función de ayuda crítica. Si se borra, la selección de menús y otras entradas fallarán.
int leer_int(const char *mensaje, int min, int max, int *valor);

// Limpia el buffer de entrada para evitar errores en la lectura de datos.
// Es una función de ayuda muy importante. Si se borra, la lectura de datos puede volverse impredecible.
void limpiar_buffer();

// Lee y valida una fecha (día, mes, año) de forma numérica y la formatea a YYYY-MM-DD.
// Si se borra, se pierde la capacidad de ingresar fechas de forma validada y segura.
int leer_fecha(const char *mensaje, char *fecha_str);

#endif
