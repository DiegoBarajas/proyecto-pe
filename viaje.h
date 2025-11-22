#include <stdio.h>
#include <string.h>


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
