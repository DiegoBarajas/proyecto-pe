#include <stdio.h>

/* Prototipos de funciones */
int asiento_disponible(int asiento_seleccionado, Viaje *dest, Pasajero *pasajeros, int totalPasajeros);
void generarRFC(char *nombre, char *apellidoP, char *apellidoM, Fecha fecha, char *rfc);
int calcularEdad(int diaNac, int mesNac, int anioNac);
void sugerir_destinos(Viaje *viajes);

/* Funcion para capturar pasajero */
Pasajero capturarPasajero(Viaje *viajes, Pasajero *pasajeros, int total_pass) {
    clear();
    printTitle(" CAPTURAR PASAJERO ");

    Pasajero p;
    char apellidoP[50];
    char apellidoM[50];
    int indx_viaje;

    time_t t;
    struct tm *fecha;
    t = time(NULL);
    fecha = localtime(&t);

    /* Captura de datos */
    // Nombre
    leerString(p.nombre, "Ingrese el nombre del pasajero: ", 1, 25);
    leerString(p.apellidoP, "Ingrese el apellido paterno del pasajero: ", 1, 25);
    leerString(p.apellidoM, "Ingrese el apellido materno del pasajero: ", 0, 25);

    // Fecha de nacimiento
    nl();
    p.fecha_nac.dia  = leerEntero("Ingrese el dia de nacimiento del pasajero: ", 1, 31);
    p.fecha_nac.mes  = leerEntero("Ingrese el mes de nacimiento del pasajero: ", 1, 12);
    p.fecha_nac.anio = leerEntero("Ingrese el anio de nacimiento del pasajero: ", 1900, 2025);

    // Telefono
    nl();
    leerString(p.telefono, "Ingrese el telefono del pasajero: ", 10, 10);

    // Direccion
    nl();
    leerString(p.direccion.calle, "Ingrese la calle del pasajero: ", 3, 50);
    p.direccion.num = leerEntero("Ingrese el numero de la casa del pasajero: ", 1, 9999);
    p.direccion.cp  = leerEntero("Ingrese el Codigo Postal del pasajero: ", 9999, 99999);
    leerString(p.direccion.colonia, "Ingrese la colonia del pasajero: ", 3, 30);
    leerString(p.direccion.estado,  "Ingrese el estado del pasajero: ", 3, 30);

    // Obtener edad
    p.edad = calcularEdad(p.fecha_nac.dia, p.fecha_nac.mes, p.fecha_nac.anio);

    // Seleccionar viaje
    do {
        indx_viaje = listar_viajes(viajes);

        // Comprobar si hay cupos en el viaje
        if(viajes[indx_viaje].pasajeros >= viajes[indx_viaje].max_pas){
            sugerir_destinos(viajes);
        }

    } while(viajes[indx_viaje].pasajeros >= viajes[indx_viaje].max_pas);

    // Aumentar contador de numero de pasajeros
    viajes[indx_viaje].pasajeros++;

    // Mandar por referencia el viaje
    p.destino = &viajes[indx_viaje];

    // Calcular descuento
    int limite10 = p.destino->max_pas / 10;

    if (p.destino->pasajeros <= limite10) {   // Si hay menos del 10%
        if (p.edad < 18 || p.edad > 60) {     // Menor de edad o mayor de 60
            p.descuento = 0.10;               // 10% de descuento
        } else {
            p.descuento = 0;
        }
    } else {
        p.descuento = 0;
    }

    int numero_asiento;
    // Numero de asiento
    do {
        numero_asiento = leerEntero("Ingrese el numero de asiento: ", 1, p.destino->max_pas);
    }while(asiento_disponible(numero_asiento, p.destino, pasajeros, total_pass));
    p.num_asiento = numero_asiento;

    // Anticipo
    float total_a_pagar = (viajes[indx_viaje].precio - (viajes[indx_viaje].precio * p.descuento));
    p.monto_pagado = leerFloat("\nIngrese el monto pagado (0 si no dio anticipo): ", 0, total_a_pagar);

    // Fecha de registro
    p.fecha_reg.dia  = fecha->tm_mday;
    p.fecha_reg.mes  = fecha->tm_mon + 1;
    p.fecha_reg.anio = fecha->tm_year + 1900;

    // RFC
    generarRFC(p.nombre, p.apellidoP, p.apellidoM, p.fecha_nac, p.rfc);

    return p;
}

void sugerir_destinos(Viaje *viajes) {
    int indices[8] = {0,1,2,3,4,5,6,7};
    int recomendados = 3;

    for (int i = 7; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    printf("\n\nLo sentimos, ya no hay asientos disponibles, pero te recomendamos los siguientes destinos:\n");
    for (int i = 0; i < 8 && recomendados > 0; i++) {
        int indx = indices[i];

        if (viajes[indx].pasajeros < viajes[indx].max_pas) {
            printf(" [ %d ] %s\n", indx + 1, viajes[indx].destino);
            recomendados--;
        }
    }

    if (recomendados == 3)
        printf("\nLo sentimos, no tenemos viajes disponibles.\n");

    nl();
}

int asiento_disponible(int asiento_seleccionado, Viaje *dest, Pasajero *pasajeros, int totalPasajeros) {
    int ocupado = 0;

    // Verificar si el asiento ya lo tiene alguien en este viaje
    for (int i = 0; i < totalPasajeros; i++) {
        if (pasajeros[i].destino == dest &&
            pasajeros[i].num_asiento == asiento_seleccionado) {
            ocupado = 1;
            break;
        }
    }

    if (!ocupado) {
        return 0; // disponible
    }

    // Si está ocupado → mostrar mensaje y listar los libres
    printf("\nLo sentimos. El asiento %d ya esta ocupado.\n", asiento_seleccionado);
    printf("Asientos disponibles en el viaje a %s:\n", dest->destino);

    int libres = 0;
    for (int a = 1; a <= dest->max_pas; a++) {

        int esta_ocupado = 0;

        for (int i = 0; i < totalPasajeros; i++) {
            if (pasajeros[i].destino == dest &&
                pasajeros[i].num_asiento == a) {
                esta_ocupado = 1;
                break;
            }
        }

        if (!esta_ocupado) {
            printf(" %d", a);
            libres++;
        }
    }

    if (libres == 0) {
        printf("\n[ ATENCION ] No quedan asientos disponibles.\n");
    } else {
        printf("\nTotal disponibles: %d\n", libres);
    }

    return 1; // ocupado
}


int calcularEdad(int diaNac, int mesNac, int anioNac) {
    time_t t = time(NULL);
    struct tm *fechaActual = localtime(&t);

    int diaHoy = fechaActual->tm_mday;
    int mesHoy = fechaActual->tm_mon + 1;
    int anioHoy = fechaActual->tm_year + 1900;

    int edad = anioHoy - anioNac;

    if ((mesHoy < mesNac) || ((mesHoy == mesNac) && (diaHoy < diaNac))) {
        edad--;
    }

    return edad;
}

void generarRFC(char *nombre, char *apellidoP, char *apellidoM, Fecha fecha, char *rfc) {
    char apPat[50], apMat[50], nom[50];
    strcpy(apPat, apellidoP);
    strcpy(apMat, apellidoM);
    strcpy(nom, nombre);

    // Hacer mayusculas
    for (int i = 0; apPat[i]; i++) apPat[i] = toupper(apPat[i]);
    for (int i = 0; apMat[i]; i++) apMat[i] = toupper(apMat[i]);
    for (int i = 0; nom[i]; i++) nom[i] = toupper(nom[i]);

    // Ignorar "JOSE" o "MARIA"
    if (strncmp(nom, "JOSE", 4) == 0 || strncmp(nom, "MARIA", 5) == 0) {
        char *espacio = strchr(nom, ' ');
        if (espacio) strcpy(nom, espacio + 1);
    }

    // Primera letra + vocal interna del apellido paterno
    char primeraLetra = apPat[0];
    char primeraVocal = 'X';
    for (int i = 1; apPat[i]; i++) {
        if (strchr("AEIOU", apPat[i])) {
            primeraVocal = apPat[i];
            break;
        }
    }

    char letraMat = apMat[0] ? apMat[0] : 'X';
    char letraNom = nom[0];

    sprintf(rfc, "%c%c%c%c%02d%02d%02d",
        primeraLetra, primeraVocal, letraMat, letraNom,
        fecha.anio % 100, fecha.mes, fecha.dia);
}


void mostrarPasajero(Pasajero *p) {
    if (p == NULL) {
        printf("\nError: pasajero NULL.\n");
        return;
    }

    clear();
    printTitle("INFORMACION DE PASAJERO");

    printf("Nombre completo: %s %s %s\n", p->nombre, p->apellidoP, p->apellidoM);
    printf("RFC:             %s\n", p->rfc);
    printf("Edad:            %d anios\n", p->edad);
    printf("Telefono:        %s\n", p->telefono);

    printf("\n=== Direccion ---\n");
    printf("      %s %d, %s, %d, %s.\n", p->direccion.calle, p->direccion.num, p->direccion.colonia, p->direccion.cp, p->direccion.estado);

    printf("\n=== Fecha de nacimiento ===\n");
    printf("      [%02d/%02d/%04d]\n", p->fecha_nac.dia, p->fecha_nac.mes, p->fecha_nac.anio);

    printf("\n=== Fecha de registro ===\n");
    printf("      [%02d/%02d/%04d]\n", p->fecha_reg.dia, p->fecha_reg.mes, p->fecha_reg.anio);


    printf("\n=== Viaje ===\n");
    printf("%s [%0.2d/%0.2d/%0.4d] - $%.2f\n", p->destino->destino,  p->destino->fecha.dia, p->destino->fecha.mes, p->destino->fecha.anio,  p->destino->precio);
    printf("Asiento asignado:  %u\n", p->num_asiento);

    printf("\n=== Pago ===\n");
    printf("Descuento:         %.0f%%\n", p->descuento * 100);
    printf("Monto pagado:      $%.2f\n", p->monto_pagado);
    printf("Monto pendiente:   $%.2f\n", ((p->destino->precio - (p->destino->precio * p->descuento)) - p->monto_pagado));

    printf("============================================================\n");
}


/* Busquedas */
// POR NOMBRE
int buscarPasajeroPorNombre(Pasajero *pasajeros, int total) {
    char busqueda[100];
    leerString(busqueda, "Ingrese el nombre a buscar: ", 1, 100);

    // Convertir a minusculas para la comparacion
    char busqLower[100];
    strcpy(busqLower, busqueda);
    for (int i = 0; busqLower[i]; i++) busqLower[i] = tolower(busqLower[i]);

    int indicesEncontrados[MAX_PASAJEROS];
    int encontrados = 0;

    // Buscar coincidencias parciales
    for (int i = 0; i < total; i++) {
        char nombreLower[100];
        strcpy(nombreLower, pasajeros[i].nombre);

        for (int j = 0; nombreLower[j]; j++) nombreLower[j] = tolower(nombreLower[j]);

        if (strstr(nombreLower, busqLower) != NULL) {
            indicesEncontrados[encontrados++] = i;
        }
    }

    // SIN coincidencias
    if (encontrados == 0) {
        printf("\nNo se encontraron pasajeros con ese nombre.\n");
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
        printf(" [%d] %s %s %s - Tel: %s - Destino: %s\n",
               i + 1,
               pasajeros[idx].nombre, pasajeros[idx].apellidoP, pasajeros[idx].apellidoM,
               pasajeros[idx].telefono,
               pasajeros[idx].destino->destino );
    }

    int opcion = leerEntero("\nSeleccione el pasajero: ", 1, encontrados);

    return indicesEncontrados[opcion - 1];
}

int buscarPasajeroPorTelefono(Pasajero *pasajeros, int total) {
    char busqueda[20];
    leerString(busqueda, "Ingrese el telefono a buscar: ", 1, 10);

    int indicesEncontrados[MAX_PASAJEROS];
    int encontrados = 0;

    // Buscar coincidencias parciales
    for (int i = 0; i < total; i++) {
        if (strstr(pasajeros[i].telefono, busqueda) != NULL) {
            indicesEncontrados[encontrados++] = i;
        }
    }

    // Sin coincidencias
    if (encontrados == 0) {
        printf("\nNo se encontraron pasajeros con ese telefono.\n");
        return -1;
    }

    // UNA coincidencia = retornar de inmediato
    if (encontrados == 1) {
        return indicesEncontrados[0];
    }

    // Varias coincidencias = mostrar lista
    printf("\nSe encontraron %d coincidencias:\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        int idx = indicesEncontrados[i];
        printf(" [%d] %s %s %s - Tel: %s - Destino: %s\n",
               i + 1,
               pasajeros[idx].nombre, pasajeros[idx].apellidoP, pasajeros[idx].apellidoM,
               pasajeros[idx].telefono,
               pasajeros[idx].destino->destino );
    }

    int opcion = leerEntero("\nSeleccione el pasajero: ", 1, encontrados);
    return indicesEncontrados[opcion - 1];
}

int buscarPasajeroPorAsiento(Pasajero *pasajeros, int total, Viaje *viajes) {

    if (total == 0) {
        printf("\nNo hay pasajeros registrados.\n");
        return -1;
    }

    printf("\nBUSCAR PASAJERO POR ASIENTO\n");

    // Seleccionar el viaje
    int idViaje = listar_viajes(viajes);

    printf("\nSeleccionaste: %s\n", viajes[idViaje].destino);

    // Pedir el numero de asiento
    int asiento = leerEntero("Ingrese el numero de asiento a buscar: ", 1, viajes[idViaje].max_pas);

    // Buscar pasajero que coincida con viaje + asiento
    for (int i = 0; i < total; i++) {
        if (pasajeros[i].destino == &viajes[idViaje] &&
            pasajeros[i].num_asiento == asiento) {

            return i;
        }
    }

    //No encontrado
    printf("\nNo se encontro ningun pasajero en el asiento %d para el viaje %s\n", asiento, viajes[idViaje].destino);

    return -1;
}

// Buscador de pasajeros
/*
    indice == -1 -> No encontrado
    indice == -2 -> Cancelado
    indice >= 0 -> Indice del elemento
*/
int buscarPasajero(Pasajero *pasajeros, Viaje *viajes, int actual_pass) {
    // Comprobar si hay pasajeros
    if(actual_pass == 0){
        nl(); nl();
        printf("[ ATENCION ] No hay pasajeros registrados.");
        nl(); nl();

        return -1;
    }

    int opc = menu_buscar_pasajero(), pass_index = -1;
    Pasajero *p = NULL;
        switch (opc) {
            // Por asiento
            case 1: return buscarPasajeroPorAsiento(pasajeros, actual_pass, viajes);
                break;

            // Por nombre
            case 2: return buscarPasajeroPorNombre(pasajeros, actual_pass);
                break;

            // Por telefono
            case 3: return buscarPasajeroPorTelefono(pasajeros, actual_pass);
                break;

            // Volver atras
            case 0: return -1;
                break;

            default: printf("\n[ ATENCION ] %d NO es una opcion valida\n", opc);
                return -1;
        }
}

// Eliminar pasajero
int eliminar_pasajero(Pasajero *pasajeros, int indx_pass, int *actual_pass) {

    if (indx_pass < 0 || indx_pass >= *actual_pass) {
        nl(); nl();
        printf("[ ATENCION ] No se encontro al pasajero.\n");
        nl(); nl();
        pause();
        return -1;
    }

    Pasajero p = pasajeros[indx_pass];

    clear();
    printTitle("SE ELIMINARA EL SIGUIENTE PASAJERO");

    mostrarPasajero(&p);

    printf("\nDesea eliminarlo? [1] Si. [0] NO. ");
    int opt = leerEntero("> ", 0, 1);

    if (opt == 1) {

        /* -- Restar contador de pasajeros del viaje -- */
        if (p.destino != NULL)
            p.destino->pasajeros--;

        /* -- Compactar el array (ELIMINAR) -- */
        for (int i = indx_pass; i < *actual_pass - 1; i++) {
            pasajeros[i] = pasajeros[i + 1];
        }

        (*actual_pass)--;  // ↓ disminuir total

        clear();
        printTitle("Se elimino el pasajero con exito");
        return 1;

    } else {

        clear();
        printTitle("No se elimino el pasajero");
        return 0;
    }
}

int actualizarPasajero(Pasajero *pasajeros, Viaje *viajes, int indx_pass,  int total_pass) {

    if (indx_pass < 0) {
        printf("\n[ ATENCION ] Pasajero no encontrado.\n");
        pause();
        return -1;
    }

    Pasajero *p = &pasajeros[indx_pass];
    int opcion;
    int indx_viaje;
    int numero_asiento;


    do {
        clear();
        printTitle(" ACTUALIZAR PASAJERO ");
        mostrarPasajero(p);

        printf("\nSeleccione el dato que desea actualizar:\n");
        printf("[1] Nombre\n");
        printf("[2] Fecha de nacimiento\n");
        printf("[3] Telefono\n");
        printf("[4] Direccion\n");
        printf("[5] Cambiar viaje\n");
        printf("[6] Cambiar asiento\n");
        printf("[7] Actualizar monto pagado\n");
        printf("[0] Volver atras\n");

        opcion = leerEntero("> ", 0, 7);

        switch(opcion) {
            // Nombre
            case 1:
                leerString(p->nombre, "Ingrese el nombre del pasajero: ", 1, 25);
                leerString(p->apellidoP, "Ingrese el apellido paterno del pasajero: ", 1, 25);
                leerString(p->apellidoM, "Ingrese el apellido materno del pasajero: ", 0, 25);

                // RFC
                generarRFC(p->nombre, p->apellidoP, p->apellidoM, p->fecha_nac, p->rfc);
            break;

            // Fecha de nacimiento
            case 2:
                p->fecha_nac.dia  = leerEntero("Ingrese el dia de nacimiento del pasajero: ", 1, 31);
                p->fecha_nac.mes  = leerEntero("Ingrese el mes de nacimiento del pasajero: ", 1, 12);
                p->fecha_nac.anio = leerEntero("Ingrese el anio de nacimiento del pasajero: ", 1900, 2025);

                // Regenerar RFC y edad
                generarRFC(p->nombre, p->apellidoP, p->apellidoM, p->fecha_nac, p->rfc);
                p->edad = calcularEdad(p->fecha_nac.dia, p->fecha_nac.mes, p->fecha_nac.anio);
            break;

            // Telefono
            case 3:
                leerString(p->telefono, "Ingrese el telefono del pasajero: ", 10, 10);
            break;

            // Direccion
            case 4:
                leerString(p->direccion.calle, "Ingrese la calle del pasajero: ", 3, 50);
                p->direccion.num = leerEntero("Ingrese el numero de la casa del pasajero: ", 1, 9999);
                p->direccion.cp  = leerEntero("Ingrese el Codigo Postal del pasajero: ", 9999, 99999);
                leerString(p->direccion.colonia, "Ingrese la colonia del pasajero: ", 3, 30);
                leerString(p->direccion.estado,  "Ingrese el estado del pasajero: ", 3, 30);
            break;


            // Seleccionar viaje
            case 5:
                p->destino->pasajeros--;
                do {
                    indx_viaje = listar_viajes(viajes);

                    // Comprobar si hay cupos en el viaje
                    if(viajes[indx_viaje].pasajeros >= viajes[indx_viaje].max_pas){
                        sugerir_destinos(viajes);
                    }

                } while(viajes[indx_viaje].pasajeros >= viajes[indx_viaje].max_pas);

                // Aumentar contador de numero de pasajeros
                viajes[indx_viaje].pasajeros++;

                // Mandar por referencia el viaje
                p->destino = &viajes[indx_viaje];

                // Calcular descuento
                int limite10 = p->destino->max_pas / 10;
                if (p->destino->pasajeros <= limite10) {   // Si hay menos del 10%
                    if (p->edad < 18 || p->edad > 60) {     // Menor de edad o mayor de 60
                        p->descuento = 0.10;               // 10% de descuento
                    } else {
                        p->descuento = 0;
                    }
                } else {
                    p->descuento = 0;
                }

                // Numero de asiento
                do {
                    numero_asiento = leerEntero("Ingrese el numero de asiento: ", 1, p->destino->max_pas);
                }while(asiento_disponible(numero_asiento, p->destino, pasajeros, total_pass));
                p->num_asiento = numero_asiento;
            break;

            // Numero de asiento
            case 6:
                p->num_asiento = -1;
                // Numero de asiento
                do {
                    numero_asiento = leerEntero("Ingrese el numero de asiento: ", 1, p->destino->max_pas);
                }while(asiento_disponible(numero_asiento, p->destino, pasajeros, total_pass));
                p->num_asiento = numero_asiento;
            break;

            // Monto pagado
            case 7:
                float total_a_pagar = (viajes[indx_viaje].precio - (viajes[indx_viaje].precio * p->descuento));
                printf("Monto pagado actual: $%.2f", p->monto_pagado);
                p->monto_pagado = leerFloat("\nIngrese el nuevo monto pagado: ", 0, total_a_pagar);
            break;

            case 0:
                clear();
                printTitle(" ACTUALIZACION COMPLETA ");
                break;

            default:
                printf("Opcion no valida.\n");
                pause();
        }

    } while (opcion != 0);

    return 1;
}

