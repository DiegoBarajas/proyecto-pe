#include <stdio.h>
#include <time.h>

#include "types.h"

#include "utils.h"
#include "console.h"
#include "menus.h"
#include "pasajeros.h"
#include "viaje.h"

/* Variables globales */
Pasajero pasajeros[MAX_PASAJEROS];
Viaje viajes[NUM_VIAJES];

unsigned int actual_pass = 0;

/* Prototipos de funciones */
void pasajero();
void viaje();

int main(void) {
    inicializar_viajes(viajes);
    int opt;

    do {
        opt = menu_principal();

        switch(opt){
            // Pasajeros
            case 1: pasajero();
                break;

            // Viajes
            case 2: viaje();
                break;
            // Salir
            case 0: break;

            default:
                printf("\n[ ATENCION ] %d NO es una opcion valida\n", opt);
                break;
        }


    } while(opt != 0);
}

void pasajero() {
    int opc = menu_pasajeros();
    int indx_pass = -1;
    Pasajero *p = NULL;

    switch(opc) {
        // Buscar
        case 1:
            indx_pass = buscarPasajero(pasajeros, viajes, actual_pass);
            if(indx_pass >= 0){
                p = pasajeros+indx_pass;
                mostrarPasajero(p);
            }

            nl(); pause();
        break;

        // Capturar
        case 2:
            // Evitar desborde de pasajeros
            if(actual_pass >= MAX_PASAJEROS){
                nl(); nl();
                printf("[ ATENCION ] La base de datos de pasajeros esta llena");
                nl(); nl();
                pause();
                break;
            }
            pasajeros[actual_pass] = capturarPasajero(viajes, pasajeros, actual_pass);
            actual_pass++;

            clear();
            printTitle("Pasajero guardado con exito!");
            nl(); nl();
            pause();
        break;

        // Modificar
        case 3:
            indx_pass = buscarPasajero(pasajeros, viajes, actual_pass);
            if(indx_pass >= 0){
                actualizarPasajero(pasajeros, viajes, indx_pass, actual_pass);
            }

            nl(); pause();
        break;

        // Eliminar
        case 4:
            indx_pass = buscarPasajero(pasajeros, viajes, actual_pass);
            if(indx_pass >= 0){
                eliminar_pasajero(pasajeros, indx_pass, &actual_pass);
            }

            nl(); pause();
        break;

        // Volver atras
        case 0: break;

        default:
            printf("\n[ ATENCION ] %d NO es una opcion valida\n", opc);
            break;
    }
}

void viaje() {
    int opc = menu_viajes();

    switch(opc) {
        // Buscar
        case 1:
            int viaje_index = buscarViajePorNombre(viajes);
            if(viaje_index >= 0){
                clear();
                Viaje v = viajes[viaje_index];

                printf("==========================\n");
                printf("Destino:     %s\n", v.destino);
                printf("Fecha:       ( %0.2d/%0.2d/%0.4d )\n", v.fecha.dia, v.fecha.mes, v.fecha.anio);
                printf("Precio base: %.2f\n\n", v.precio);

                printf("Num. asientos:        %d\n", v.max_pas);
                printf("Asientos vendidos:    %d\n", v.pasajeros);
                printf("Asientos disponibles: %d\n", (v.max_pas - v.pasajeros));
                printf("==========================\n");
            }else{
                clear();
                printTitle("No se encontraron coincidencias");
            }
            pause();
        break;

        // Listado de pasajeros por viaje
        case 2: listado_pasajero_viaje(pasajeros, viajes, actual_pass);
            break;

        // Lista de viajes incompletos
        case 3: viajes_incompletos(viajes, actual_pass);
            break;

        // Volver atras
        case 0: break;

        default:
            printf("\n[ ATENCION ] %d NO es una opcion valida\n", opc);
            break;
    }
}
