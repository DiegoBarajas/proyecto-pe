#include <stdio.h>

int menu_principal() {
    clear();

    printTitle("MENU PRINCIPAL");
    printf("Ingrese la opcion que desea realizar:\n");
    printf(" 1. Pasajeros.\n");
    printf(" 2. Viajes.\n");
    printf(" 0. Salir.\n");

    return leerEntero("\n> ", 0, 3);
}

/* MENUS PASAJERO */
int menu_pasajeros() {
    nl();

    printTitle("PASAJEROS");
    printf("Ingrese la opcion que desea realizar:\n");
    printf(" 1. Buscar.\n");
    printf(" 2. Capturar.\n");
    printf(" 3. Modificar.\n");
    printf(" 4. Eliminar.\n");
    printf(" 0. Volver atras.\n");

    return leerEntero("\n> ", 0, 4);
}

int menu_buscar_pasajero() {
    nl();

    printTitle("BUSCAR PASAJERO");
    printf("Ingrese la opcion que desea realizar:\n");
    printf(" 1. Por asiento.\n");
    printf(" 2. Por nombre.\n");
    printf(" 3. Por telefono.\n");
    printf(" 0. Volver atras.\n");

    return leerEntero("\n> ", 0, 3);
}

/* MENUS VIAJE */
int menu_viajes() {
    nl();

    printTitle("VIAJES");
    printf("Ingrese la opcion que desea realizar:\n");
    printf(" 1. Buscar.\n");
    printf(" 2. Listado de pasajeros.\n");
    printf(" 3. Lista de viajes incompletos .\n");
    printf(" 0. Volver atras.\n");

    return leerEntero("\n> ", 0, 3);
}

int menu_listado_pasajeros() {
    nl();

    printTitle("LISTADO DE PASAJEROS");
    printf("Ingrese la opcion que desea realizar:\n");
    printf(" 1. Listado por viaje.\n");
    printf(" 2. Todos los viajes.\n");
    printf(" 0. Volver atras.\n");

    return leerEntero("\n> ", 0, 2);
}

int menu_viajes_incompletos() {
    nl();

    printTitle("LISTA DE VIAJES INCOMPLETOS");
    printf("Ingrese la opcion que desea realizar:\n");
    printf(" 1. 30%%\n");
    printf(" 2. 50%%\n");
    printf(" 3. 80%%\n");
    printf(" 4. 100%%\n");
    printf(" 0. Volver atras.\n");

    return leerEntero("\n> ", 0, 4);
}

/* MENU DE VIAJES */
int listar_viajes(Viaje *viajes) {
    int indx;
    printf("\nSelecciona el viaje deseado:\n");
    for(int i=0;i<8;i++){
        printf(" [ %d ] %-20s ( %0.2d/%0.2d/%0.4d ) - $%.2f p/P\n", i+1, viajes[i].destino, viajes[i].fecha.dia, viajes[i].fecha.mes, viajes[i].fecha.anio, viajes[i].precio);
    }
    indx = leerEntero("\n> ", 1, 8);
    indx--;

    return indx;
}
