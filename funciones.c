#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"

#define ARCHIVO_DATOS "datos_zonas.txt"
#define ARCHIVO_RESPALDO "respaldo_zonas.txt"

// Carga los datos desde un archivo de texto
int cargar_zonas(Zona zonas[], int *num_zonas) {
    FILE *f = fopen(ARCHIVO_DATOS, "r");
    if (!f) return 0;
    // Leer número de zonas
    if (fscanf(f, "%d\n", num_zonas) != 1) {
        fclose(f);
        return 0;
    }
    for (int i = 0; i < *num_zonas; i++) {
        // Leer nombre de zona
        if (!fgets(zonas[i].nombre, NOMBRE_ZONA, f)) {
            fclose(f);
            return 0;
        }
        zonas[i].nombre[strcspn(zonas[i].nombre, "\n")] = '\0';
        // Leer días registrados
        int dias;
        if (fscanf(f, "%d\n", &dias) != 1) {
            fclose(f);
            return 0;
        }
        zonas[i].dias_registrados = dias;
        // Leer historial de días
        for (int j = 0; j < dias; j++) {
            RegistroDia *r = &zonas[i].historial[j];
            if (fscanf(f, "%10s %f %f %f %f %f %f %f %f\n",
                       r->fecha,
                       &r->pm25, &r->pm10, &r->co2,
                       &r->so2, &r->no2,
                       &r->temperatura, &r->humedad,
                       &r->velocidad_viento) != 9) {
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return 1;
}

// Guarda los datos en un archivo de texto
int guardar_zonas(Zona zonas[], int num_zonas) {
    FILE *f = fopen(ARCHIVO_DATOS, "w");
    if (!f) return 0;
    fprintf(f, "%d\n", num_zonas);
    for (int i = 0; i < num_zonas; i++) {
        fprintf(f, "%s\n", zonas[i].nombre);
        fprintf(f, "%d\n", zonas[i].dias_registrados);
        for (int j = 0; j < zonas[i].dias_registrados; j++) {
            RegistroDia *r = &zonas[i].historial[j];
            fprintf(f, "%s %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f\n",
                    r->fecha,
                    r->pm25, r->pm10, r->co2,
                    r->so2, r->no2,
                    r->temperatura, r->humedad,
                    r->velocidad_viento);
        }
    }
    fclose(f);
    return 1;
}

void mostrar_menu() {
    printf("\n============================================================\n");
    printf("    SISTEMA INTEGRAL DE GESTION DE CONTAMINACION DEL AIRE\n");
    printf("                      CIUDAD DE QUITO\n");
    printf("============================================================\n");
    printf("1. Mostrar estado actual de todas las zonas\n");
    printf("2. Mostrar predicciones para 24 horas\n");
    printf("3. Ingresar datos actuales de una zona\n");
    printf("4. Mostrar informacion de zonas monitoreadas\n");
    printf("5. Generar alertas del sistema\n");
    printf("6. Generar recomendaciones para una zona\n");
    printf("7. Generar reporte completo\n");
    printf("8. Exportar copia de respaldo\n");
    printf("9. Anadir nueva zona de monitoreo\n");
    printf("10. Editar datos de una zona existente\n");
    printf("11. Eliminar una zona del sistema\n");
    printf("0. Salir del sistema\n");
    printf("1000. Reiniciar programa (eliminar todos los datos)\n");
    printf("============================================================\n");
    printf("NOTA: Todos los datos se gestionan automaticamente en\n");
    printf("      formato de texto para mayor portabilidad.\n");
    printf("      El historial ahora almacena los ultimos 7 dias.\n");
    printf("============================================================\n");
    printf("Seleccione una opcion: ");
}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validar_float(float valor, float min, float max) {
    return valor >= min && valor <= max;
}

int leer_float(const char *mensaje, float min, float max, float *valor) {
    char buffer[50];
    char *endptr;
    float temp;
    int valido = 0;
    do {
        printf("%s", mensaje);
        if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
        temp = strtof(buffer, &endptr);
        if (endptr == buffer || *endptr != '\n') {
            printf("Entrada invalida. Intente de nuevo.\n");
            continue;
        }
        if (!validar_float(temp, min, max)) {
            printf("Valor fuera de rango (%.2f - %.2f).\n", min, max);
            continue;
        }
        *valor = temp;
        valido = 1;
    } while (!valido);
    return 1;
}

int leer_int(const char *mensaje, int min, int max, int *valor) {
    char buffer[50];
    char *endptr;
    int temp, valido = 0;
    do {
        printf("%s", mensaje);
        if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
        temp = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Entrada invalida. Intente de nuevo.\n");
            continue;
        }
        if (temp < min || temp > max) {
            printf("Valor fuera de rango (%d - %d).\n", min, max);
            continue;
        }
        *valor = temp;
        valido = 1;
    } while (!valido);
    return 1;
}

void mostrar_estado_actual(Zona zonas[], int num_zonas) {
    printf("\nESTADO ACTUAL DE LAS ZONAS (ULTIMOS 7 DIAS):\n");
    for (int i = 0; i < num_zonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("------------------------------------------------------------\n");
        printf("Fecha      | PM2.5 | PM10 | CO2  | SO2  | NO2  | Temp | Hum | V.Viento\n");
        printf("-----------|-------|------|------|------|------|------|-----|----------\n");
        if (zonas[i].dias_registrados > 0) {
            for (int j = 0; j < zonas[i].dias_registrados; j++) {
                RegistroDia *r = &zonas[i].historial[j];
                printf("%-10s | %5.1f | %4.1f | %4.1f | %4.1f | %4.1f | %4.1f | %3.1f | %7.1f\n",
                    r->fecha, r->pm25, r->pm10, r->co2, r->so2, r->no2, r->temperatura, r->humedad, r->velocidad_viento);
            }
        } else {
            printf("No hay datos registrados.\n");
        }
    }
}

void mostrar_predicciones(Zona zonas[], int num_zonas) {
    printf("\nPREDICCIONES PARA LAS PROXIMAS 24 HORAS:\n");
    for (int i = 0; i < num_zonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("------------------------------------------------------------\n");
        printf("PM2.5 | PM10 | CO2  | SO2  | NO2  | Temp | Hum | V.Viento\n");
        float sumas[8] = {0};
        float pesos[3] = {0.6, 0.3, 0.1};
        int dias = zonas[i].dias_registrados;
        if (dias < 3) {
            printf("No hay suficientes datos para predecir.\n");
            continue;
        }
        for (int j = 0; j < 3; j++) {
            RegistroDia *r = &zonas[i].historial[dias-1-j];
            sumas[0] += r->pm25 * pesos[j];
            sumas[1] += r->pm10 * pesos[j];
            sumas[2] += r->co2 * pesos[j];
            sumas[3] += r->so2 * pesos[j];
            sumas[4] += r->no2 * pesos[j];
            sumas[5] += r->temperatura * pesos[j];
            sumas[6] += r->humedad * pesos[j];
            sumas[7] += r->velocidad_viento * pesos[j];
        }
        printf("%5.1f | %4.1f | %4.1f | %4.1f | %4.1f | %4.1f | %3.1f | %7.1f\n",
            sumas[0], sumas[1], sumas[2], sumas[3], sumas[4], sumas[5], sumas[6], sumas[7]);
    }
}

void ingresar_datos_actuales(Zona zonas[], int num_zonas) {
    int op;
    printf("\nSeleccione la zona para ingresar datos:\n");
    for (int i = 0; i < num_zonas; i++)
        printf("%d. %s\n", i+1, zonas[i].nombre);
    if (!leer_int("Opcion: ", 1, num_zonas, &op)) return;
    Zona *z = &zonas[op-1];
    if (z->dias_registrados == DIAS_HISTORIAL) {
        for (int i = 1; i < DIAS_HISTORIAL; i++)
            z->historial[i-1] = z->historial[i];
        z->dias_registrados--;
    }
    RegistroDia *r = &z->historial[z->dias_registrados];
    printf("Ingrese la fecha (YYYY-MM-DD): ");
    fgets(r->fecha, sizeof(r->fecha), stdin);
    r->fecha[strcspn(r->fecha, "\n")] = 0;
    leer_float("PM2.5: ", 0, 99999, &r->pm25);
    leer_float("PM10: ", 0, 99999, &r->pm10);
    leer_float("CO2: ", 0, 99999, &r->co2);
    leer_float("SO2: ", 0, 99999, &r->so2);
    leer_float("NO2: ", 0, 99999, &r->no2);
    leer_float("Temperatura (C): ", -50, 60, &r->temperatura);
    leer_float("Humedad (%%): ", 0, 100, &r->humedad);
    leer_float("Velocidad viento (km/h): ", 0, 500, &r->velocidad_viento);
    z->dias_registrados++;
    guardar_zonas(zonas, num_zonas);
    printf("Datos ingresados correctamente.\n");
}

void mostrar_info_zonas(Zona zonas[], int num_zonas) {
    printf("\nINFORMACION DE ZONAS MONITOREADAS:\n");
    for (int i = 0; i < num_zonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("------------------------------------------------------------\n");
        printf("Fecha      | PM2.5 | PM10 | CO2  | SO2  | NO2  | Temp | Hum | V.Viento\n");
        printf("-----------|-------|------|------|------|------|------|-----|----------\n");
        for (int j = 0; j < zonas[i].dias_registrados; j++) {
            RegistroDia *r = &zonas[i].historial[j];
            printf("%-10s | %5.1f | %4.1f | %4.1f | %4.1f | %4.1f | %4.1f | %3.1f | %7.1f\n",
                r->fecha, r->pm25, r->pm10, r->co2, r->so2, r->no2, r->temperatura, r->humedad, r->velocidad_viento);
        }
    }
}

void generar_alertas(Zona zonas[], int num_zonas) {
    printf("\nALERTAS DEL SISTEMA:\n");
    for (int i = 0; i < num_zonas; i++) {
        int alerta = 0;
        if (zonas[i].dias_registrados == 0) continue;
        RegistroDia *r = &zonas[i].historial[zonas[i].dias_registrados-1];
        if (r->pm25 > 25 || r->pm10 > 50 || r->co2 > 1000 || r->so2 > 20 || r->no2 > 40) alerta = 1;
        if (alerta) {
            printf("ALERTA: %s supera los limites OMS. Revise los datos.\n", zonas[i].nombre);
        }
    }
}

void generar_recomendaciones(Zona zonas[], int num_zonas) {
    int op;
    printf("\nSeleccione la zona para recomendaciones:\n");
    for (int i = 0; i < num_zonas; i++)
        printf("%d. %s\n", i+1, zonas[i].nombre);
    if (!leer_int("Opcion: ", 1, num_zonas, &op)) return;
    Zona *z = &zonas[op-1];
    if (z->dias_registrados == 0) {
        printf("No hay datos para esta zona.\n");
        return;
    }
    RegistroDia *r = &z->historial[z->dias_registrados-1];
    printf("\nRecomendaciones para %s:\n", z->nombre);
    if (r->pm25 > 25 || r->pm10 > 50 || r->co2 > 1000 || r->so2 > 20 || r->no2 > 40) {
        printf("- Reducir el trafico vehicular.\n");
        printf("- Cierre temporal de industrias.\n");
        printf("- Suspender actividades al aire libre.\n");
    } else {
        printf("- Niveles dentro de lo aceptable. Mantener monitoreo.\n");
    }
}

void generar_reporte(Zona zonas[], int num_zonas) {
    FILE *f = fopen("reporte.txt", "w");
    if (!f) {
        printf("No se pudo crear el reporte.\n");
        return;
    }
    for (int i = 0; i < num_zonas; i++) {
        fprintf(f, "Zona: %s\n", zonas[i].nombre);
        fprintf(f, "Fecha      | PM2.5 | PM10 | CO2  | SO2  | NO2  | Temp | Hum | V.Viento\n");
        for (int j = 0; j < zonas[i].dias_registrados; j++) {
            RegistroDia *r = &zonas[i].historial[j];
            fprintf(f, "%-10s | %5.1f | %4.1f | %4.1f | %4.1f | %4.1f | %4.1f | %3.1f | %7.1f\n",
                r->fecha, r->pm25, r->pm10, r->co2, r->so2, r->no2, r->temperatura, r->humedad, r->velocidad_viento);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Reporte generado en reporte.txt\n");
}

void exportar_respaldo(Zona zonas[], int num_zonas) {
    FILE *f = fopen(ARCHIVO_RESPALDO, "wb");
    if (!f) {
        printf("No se pudo exportar el respaldo.\n");
        return;
    }
    fwrite(&num_zonas, sizeof(int), 1, f);
    fwrite(zonas, sizeof(Zona), num_zonas, f);
    fclose(f);
    printf("Respaldo exportado en %s\n", ARCHIVO_RESPALDO);
}

void anadir_zona(Zona zonas[], int *num_zonas) {
    if (*num_zonas >= MAX_ZONAS) {
        printf("No se pueden agregar mas zonas.\n");
        return;
    }
    printf("Nombre de la nueva zona: ");
    fgets(zonas[*num_zonas].nombre, NOMBRE_ZONA, stdin);
    zonas[*num_zonas].nombre[strcspn(zonas[*num_zonas].nombre, "\n")] = 0;
    zonas[*num_zonas].dias_registrados = 0;
    (*num_zonas)++;
    guardar_zonas(zonas, *num_zonas);
    printf("Zona agregada correctamente.\n");
}

void editar_zona(Zona zonas[], int num_zonas) {
    int op;
    printf("\nSeleccione la zona a editar:\n");
    for (int i = 0; i < num_zonas; i++)
        printf("%d. %s\n", i+1, zonas[i].nombre);
    if (!leer_int("Opcion: ", 1, num_zonas, &op)) return;
    Zona *z = &zonas[op-1];
    printf("Nuevo nombre (deje vacio para no cambiar): ");
    char buffer[NOMBRE_ZONA];
    fgets(buffer, NOMBRE_ZONA, stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strncpy(z->nombre, buffer, NOMBRE_ZONA);
    }
    guardar_zonas(zonas, num_zonas);
    printf("Zona editada correctamente.\n");
}

void eliminar_zona(Zona zonas[], int *num_zonas) {
    int op;
    printf("\nSeleccione la zona a eliminar:\n");
    for (int i = 0; i < *num_zonas; i++)
        printf("%d. %s\n", i+1, zonas[i].nombre);
    if (!leer_int("Opcion: ", 1, *num_zonas, &op)) return;
    for (int i = op-1; i < *num_zonas-1; i++)
        zonas[i] = zonas[i+1];
    (*num_zonas)--;
    guardar_zonas(zonas, *num_zonas);
    printf("Zona eliminada correctamente.\n");
}

void reiniciar_programa() {
    remove(ARCHIVO_DATOS);
    printf("Todos los datos han sido eliminados.\n");
}
