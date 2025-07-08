#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

int main() {
    Zona zonas[MAX_ZONAS];
    int num_zonas = 0, opcion;

    /*
     * PREGUNTA DE PROFESOR: ¿Cuál es el propósito de este bloque if-else inicial? ¿Qué pasaría si lo borramos?
     *
     * RESPUESTA:
     * 1. ¿QUÉ HACE?: Esta es la rutina de inicialización robusta del programa. Intenta cargar los datos desde "datos_zonas.txt".
     *    Si el archivo no existe o su contenido es inválido (la función cargar_zonas devuelve 0), en lugar de fallar,
     *    procede a crear un conjunto de datos de ejemplo con 5 zonas y 7 días de historial para cada una.
     *    Esto asegura que el programa siempre tenga datos válidos para funcionar desde el primer momento.
     *
     * 2. ¿QUÉ PASA SI SE BORRA?: Si se elimina este bloque, el programa no sería capaz de autogenerar datos.
     *    Si el archivo "datos_zonas.txt" no existiera, num_zonas sería 0 y el programa se ejecutaría con todas las zonas vacías,
     *    lo cual no permitiría al usuario probar la mayoría de las funcionalidades sin antes añadir zonas y datos manualmente.
     *    Perdería su capacidad de "funcionar desde el cero" de forma amigable.
     */
    if (!cargar_zonas(zonas, &num_zonas)) {
        printf("No se encontro archivo de datos o el formato es incorrecto. Creando uno nuevo...\n");
        char *nombres[] = {"UDLA Park", "Parque La Carolina", "Mitad del Mundo", "El Panecillo", "Centro Historico"};
        num_zonas = 5;

        for (int i = 0; i < num_zonas; i++) {
            strncpy(zonas[i].nombre, nombres[i], NOMBRE_ZONA - 1);
            zonas[i].nombre[NOMBRE_ZONA - 1] = '\0';
            zonas[i].dias_registrados = DIAS_HISTORIAL;
            for (int j = 0; j < DIAS_HISTORIAL; j++) {
                RegistroDia *r = &zonas[i].historial[j];
                sprintf(r->fecha, "2025-07-%02d", j + 1);
                r->pm25 = 15.0f + (rand() % 200) / 10.0f;
                r->pm10 = 25.0f + (rand() % 300) / 10.0f;
                r->co2 = 400.0f + (rand() % 2000) / 10.0f;
                r->so2 = 5.0f + (rand() % 150) / 10.0f;
                r->no2 = 10.0f + (rand() % 300) / 10.0f;
                r->temperatura = 10.0f + (rand() % 150) / 10.0f;
                r->humedad = 50.0f + (rand() % 300) / 10.0f;
                r->velocidad_viento = 5.0f + (rand() % 150) / 10.0f;
            }
        }
        guardar_zonas(zonas, num_zonas);
        printf("Archivo de datos inicial creado con 5 zonas y 7 dias de historial.\n");
    }

    /*
     * PREGUNTA DE PROFESOR: Explique el funcionamiento de este bucle y el switch. ¿Y por qué es vital la función limpiar_buffer() aquí?
     *
     * RESPUESTA:
     * 1. ¿QUÉ HACE?: Este es el bucle principal del programa (game loop o event loop). Se ejecuta indefinidamente hasta que el usuario elige la opción 0.
     *    Dentro del bucle, primero muestra el menú, luego usa scanf() para leer la opción del usuario. El `switch` dirige el flujo del programa
     *    a la función correspondiente según la opción elegida. Es el centro de control de la interacción con el usuario.
     *
     * 2. ¿QUÉ PASA SI SE BORRA EL SWITCH?: El programa mostraría el menú repetidamente, pero no haría nada al seleccionar una opción. Sería inútil.
     *
     * 3. ¿POR QUÉ ES VITAL limpiar_buffer()?: La función scanf("%d", &opcion) lee solo el número, pero deja el carácter de nueva línea ('\n')
     *    (que se genera al presionar "Enter") en el búfer de entrada. Si no se limpiara, la siguiente función que intente leer texto
     *    (como fgets() en anadir_zona o ingresar_datos) leería inmediatamente este '\n' y asumiría que el usuario no introdujo nada,
     *    causando que el programa se salte pasos y no funcione correctamente. limpiar_buffer() vacía estos residuos del búfer.
     */
    do {
        mostrar_menu();
        if (scanf("%d", &opcion) != 1) {
            opcion = -1; // Opción inválida para que el bucle continúe
        }
        limpiar_buffer(); // Limpiar el buffer después de cada entrada

        switch (opcion) {
            case 1: mostrar_estado_actual(zonas, num_zonas); break;
            case 2: mostrar_predicciones(zonas, num_zonas); break;
            case 3: ingresar_datos_actuales(zonas, num_zonas); break;
            case 4: mostrar_info_zonas(zonas, num_zonas); break;
            case 5: generar_alertas_y_recomendaciones(zonas, num_zonas); break;
            case 6: generar_reporte(zonas, num_zonas); break;
            case 7: exportar_respaldo(zonas, num_zonas); break;
            case 8: anadir_zona(zonas, &num_zonas); break;
            case 9: editar_zona(zonas, num_zonas); break;
            case 10: eliminar_zona(zonas, &num_zonas); break;
            case 1000:
                reiniciar_programa();
                num_zonas = 0; // Reinicia el contador de zonas
                printf("Programa reiniciado. Por favor, reinicie la aplicacion para generar nuevos datos de ejemplo.\n");
                opcion = 0; // Forzar salida para evitar operar con datos vacíos
                break;
            case 0:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida. Por favor, intente de nuevo.\n");
        }
    } while (opcion != 0);

    return 0;
}
