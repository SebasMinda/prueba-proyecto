#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

int main() {
    Zona zonas[MAX_ZONAS];
    int num_zonas = 0, opcion;
    if (!cargar_zonas(zonas, &num_zonas)) {
        // Si no existe archivo, crear con 5 zonas y 7 dias de datos
        char *nombres[] = {"UDLA Park", "Parque La Carolina", "Mitad del Mundo", "El Panecillo", "Sangolqui"};
        num_zonas = 5;
        for (int i = 0; i < num_zonas; i++) {
            strncpy(zonas[i].nombre, nombres[i], NOMBRE_ZONA);
            zonas[i].dias_registrados = DIAS_HISTORIAL;
            for (int j = 0; j < DIAS_HISTORIAL; j++) {
                RegistroDia *r = &zonas[i].historial[j];
                sprintf(r->fecha, "2025-07-%02d", j + 1);
                r->pm25 = 15.0f + (rand() % 200) / 10.0f; // 15.0 - 34.9
                r->pm10 = 25.0f + (rand() % 300) / 10.0f; // 25.0 - 54.9
                r->co2 = 400.0f + (rand() % 2000) / 10.0f; // 400 - 599.9
                r->so2 = 5.0f + (rand() % 150) / 10.0f;  // 5.0 - 19.9
                r->no2 = 10.0f + (rand() % 300) / 10.0f; // 10.0 - 39.9
                r->temperatura = 10.0f + (rand() % 150) / 10.0f; // 10.0 - 24.9
                r->humedad = 50.0f + (rand() % 300) / 10.0f; // 50.0 - 79.9
                r->velocidad_viento = 5.0f + (rand() % 150) / 10.0f; // 5.0 - 19.9
            }
        }
        guardar_zonas(zonas, num_zonas);
        printf("Archivo de datos inicial creado con 7 dias de historial.\n");
    }
    do {
        mostrar_menu();
        if (!leer_int("", 0, 1000, &opcion)) continue;
        switch (opcion) {
            case 1: mostrar_estado_actual(zonas, num_zonas); break;
            case 2: mostrar_predicciones(zonas, num_zonas); break;
            case 3: ingresar_datos_actuales(zonas, num_zonas); break;
            case 4: mostrar_info_zonas(zonas, num_zonas); break;
            case 5: generar_alertas(zonas, num_zonas); break;
            case 6: generar_recomendaciones(zonas, num_zonas); break;
            case 7: generar_reporte(zonas, num_zonas); break;
            case 8: exportar_respaldo(zonas, num_zonas); break;
            case 9: anadir_zona(zonas, &num_zonas); break;
            case 10: editar_zona(zonas, num_zonas); break;
            case 11: eliminar_zona(zonas, &num_zonas); break;
            case 1000: reiniciar_programa(); num_zonas = 0; break;
            case 0: printf("Saliendo del sistema...\n"); break;
            default: printf("Opcion invalida.\n");
        }
    } while (opcion != 0);
    return 0;
}
