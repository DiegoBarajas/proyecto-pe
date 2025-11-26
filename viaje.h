#include <stdio.h>
#include <string.h>

void listar_viajes_incompletos(Viaje *viajes, float porcentaje);

Viaje crear_viaje(Fecha fecha, const char *destino, float precio, int max_pass) {
    Viaje nuevo;
    nuevo.fecha = fecha;
    strcpy(nuevo.destino, destino);
    nuevo.precio = precio;
    nuevo.max_pas = max_pass;
    nuevo.pasajeros = 0;

    return nuevo;
}


void inicializar_viajes(Viaje *viajes) {
    Fecha fechas[] = {
        {26, 1, 2026}, {12, 2, 2026},
        {19, 2, 2026}, {25, 2, 2026},
        {26, 2, 2026}, {26, 3, 2026},
        {26, 3, 2026}, {14, 5, 2026}
    };

    char destinos[][50] = {
        "Playa platanitos",
        "Pelicano borregon",
        "Santa Maria del Oro",
        "Mariposa monarca",
        "Tequila",
        "El manto",
        "Los ayala",
        "Feria de la pitaya"
    };
    float precios[] = {
        720, 580,
        700, 950,
        500, 780,
        720, 550
    };

    for (int i = 0; i < NUM_VIAJES && i < 8; i++) {
        viajes[i] = crear_viaje(fechas[i], destinos[i], precios[i], 40);
    }
}

// Mostrar pasajeros por viaje
void listado_pasajero_viaje(Pasajero *pasajeros, Viaje *viajes, int total_pass) {
    clear();

    if(total_pass <= 0){
        printTitle("No hay pasajeros registrados");
        pause();
        return;
    }

    int viaje_index = listar_viajes(viajes);
    Viaje v = viajes[viaje_index];   // v es un struct, NO un puntero

    int cont = 0;

    for (int i = 0; i < total_pass; i++) {
        Pasajero p = pasajeros[i];  // p también es un struct

        // Comparación correcta de destino de viaje con destino del pasajero
        if (strcmp(p.destino->destino, v.destino) == 0) {

            float precio_final = p.destino->precio * (1.0f - p.descuento);
            float monto_restante = precio_final - p.monto_pagado;

            printf("=========================\n");
            printf("Nombre: %s %s %s\n", p.nombre, p.apellidoP, p.apellidoM);
            printf("Asiento: %d\n", p.num_asiento);
            printf("Monto total: $%.2f\n", precio_final);
            printf("Monto pagado: $%.2f\n", p.monto_pagado);

            // Evita mostrar negativo si ya pagó de más
            if (monto_restante < 0) monto_restante = 0;

            printf("Monto restante: $%.2f\n\n", monto_restante);

            cont++;
        }
    }

    // Verificar si se mostró
    if (cont == 0) {
        printTitle("No hay pasajeros registrados en ese viaje");
    }
    pause();
}

// Buscar viaje por str
int buscarViajePorNombre(Viaje *viajes) {
    char busqueda[100];
    leerString(busqueda, "Ingrese el viaje a buscar: ", 1, 100);

    // Convertir a minusculas para la comparacion
    char busqLower[100];
    strcpy(busqLower, busqueda);
    for (int i = 0; busqLower[i]; i++) busqLower[i] = tolower(busqLower[i]);

    int indicesEncontrados[MAX_PASAJEROS];
    int encontrados = 0;

    // Buscar coincidencias parciales
    for (int i = 0; i < NUM_VIAJES; i++) {
        char nombreLower[100];
        strcpy(nombreLower, viajes[i].destino);

        for (int j = 0; nombreLower[j]; j++) nombreLower[j] = tolower(nombreLower[j]);

        if (strstr(nombreLower, busqLower) != NULL) {
            indicesEncontrados[encontrados++] = i;
        }
    }

    // SIN coincidencias
    if (encontrados == 0) {
        printf("\nNo se encontraron viajes con ese nombre.\n");
        return -1;
    }

    // UNA coincidencia = retornar de inmediato
    if (encontrados == 1) {
        return indicesEncontrados[0];
    }

    // MULTIPLES coincidencias = mostrar lista y permitir elegir
    printf("\nSe encontraron %d coincidencias:\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        int idx = indicesEncontrados[i];
        printf(" [%d] %-20s - Fecha: ( %0.2d/%0.2d/%0.4d )\n",
               i + 1,
               viajes[idx].destino,
               viajes[idx].fecha.dia, viajes[idx].fecha.mes, viajes[idx].fecha.anio);
    }

    int opcion = leerEntero("\nSeleccione el viaje: ", 1, encontrados);

    return indicesEncontrados[opcion - 1];
}

// Mostrar lista de viajes que no han completado un porcentaje de pasajeros (30%, 50%, 80% y 100%)
void viajes_incompletos(Viaje *viajes, int total_pass) {
    clear();

    int opt = menu_viajes_incompletos();
    switch(opt) {
        case 1: listar_viajes_incompletos(viajes, 30);
        break;

        case 2: listar_viajes_incompletos(viajes, 50);
        break;

        case 3: listar_viajes_incompletos(viajes, 80);
        break;

        case 4: listar_viajes_incompletos(viajes, 100);
        break;

        case 0:
        break;

        default:
            printf("Opcion no valida.\n");
            pause();
    }

    if(opt != 0) pause();
}

// Void listado porcentajes
void listar_viajes_incompletos(Viaje *viajes, float porcentaje) {
    int count = 1;
    for (int i = 0; i < NUM_VIAJES; i++) {

        // Cálculo correcto del porcentaje de ocupación y de libertad
        float ocupados = (viajes[i].pasajeros * 100.0f) / viajes[i].max_pas;
        float libres = 100.0f - ocupados;

        // Mostrar solo los viajes cuyo porcentaje libre sea MAYOR al solicitado
        if (libres <= porcentaje) {
            printf("\n====================================\n");
            printf("%d.- %s\n", count, viajes[i].destino);
            printf("====================================\n");
            printf("  Fecha:                 ( %0.2d/%0.2d/%0.4d )\n", viajes[i].fecha.dia, viajes[i].fecha.mes, viajes[i].fecha.anio);
            printf("  Costo base:            $%.2f p/P\n", viajes[i].precio);
            printf("  Capacidad total:       %d\n", viajes[i].max_pas);
            printf("  Pasajeros registrados: %d\n", viajes[i].pasajeros);
            printf("  %% Ocupados:            %.2f%%\n", ocupados);
            printf("  %% Libres:              %.2f%%\n", libres);
            count++;
        }
    }
}


