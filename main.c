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

    switch(opc) {
        // Buscar
        case 1:
                // Comprobar si hay pasajeros
                if(actual_pass == 0){
                    nl(); nl();
                    printf("[ ATENCION ] No hay pasajeros registrados.");
                    nl(); nl();
                    pause();
                    break;
                }

                opc = menu_buscar_pasajero();
                Pasajero *p = NULL;
                switch (opc) {
                    // Por asiento
                    case 1:
                        p = buscarPasajeroPorAsiento(pasajeros, actual_pass, viajes);

                        if(p != NULL){
                            mostrarPasajero(p);
                        }
                        nl(); pause();
                        break;

                    // Por nombre
                    case 2:
                        p = buscarPasajeroPorNombre(pasajeros, actual_pass);

                        if(p != NULL){
                            mostrarPasajero(p);
                        }
                        nl(); pause();
                        break;

                    // Por telefono
                    case 3:
                        p = buscarPasajeroPorTelefono(pasajeros, actual_pass);

                        if(p != NULL){
                            mostrarPasajero(p);
                        }
                        nl(); pause();
                        break;

                    // Volver atras
                    case 0: break;

                    default:
                        printf("\n[ ATENCION ] %d NO es una opcion valida\n", opc);
                        break;
                }
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

                nl(); nl();
                printTitle("Pasajero guardado con exito!");
                nl(); nl();
                pause();

            break;

        // Modificar
        case 3:
            break;

        // Eliminar
        case 4:
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
            break;

        // Listado de pasajeros
        case 2:
            break;

        // Lista de viajes incompletos
        case 3:
            break;

        // Volver atras
        case 0: break;

        default:
            printf("\n[ ATENCION ] %d NO es una opcion valida\n", opc);
            break;
    }
}
