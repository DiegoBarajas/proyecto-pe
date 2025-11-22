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
    leerString(p.nombre, "Ingrese el nombre del pasajero: ", 1, 50);
    leerString(apellidoP, "Ingrese el apellido paterno del pasajero: ", 1, 25);
    leerString(apellidoM, "Ingrese el apellido materno del pasajero: ", 0, 25);

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
        if (p.edad < 18 || p.edad > 60) { // Menor de edad o mayor de 60
            p.descuento = 0.10; // 10% de descuento
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
    generarRFC(p.nombre, apellidoP, apellidoM, p.fecha_nac, p.rfc);

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
    printf("\nLo sentimos. El asiento %d ya está ocupado.\n", asiento_seleccionado);
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

    printf("Nombre completo: %s\n", p->nombre);
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
Pasajero* buscarPasajeroPorNombre(Pasajero *pasajeros, int total) {
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
        return NULL;
    }

    // UNA coincidencia = retornar de inmediato
    if (encontrados == 1) {
        return &pasajeros[indicesEncontrados[0]];
    }

    // MULTIPLES coincidencias = mostrar lista y permitir elegir
    printf("\nSe encontraron %d coincidencias:\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        int idx = indicesEncontrados[i];
        printf(" [%d] %s (Destino: %s)\n",
               i + 1,
               pasajeros[idx].nombre,
               pasajeros[idx].destino ? pasajeros[idx].destino->destino : "N/A");
    }

    int opcion = leerEntero("\nSeleccione el pasajero: ", 1, encontrados);

    return &pasajeros[indicesEncontrados[opcion - 1]];
}

Pasajero* buscarPasajeroPorTelefono(Pasajero *pasajeros, int total) {
    char busqueda[20];
    leerString(busqueda, "Ingrese el telefono a buscar: ", 1, 20);

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
        printf("\nNo se encontraron pasajeros con ese teléfono.\n");
        return NULL;
    }

    // UNA coincidencia = retornar de inmediato
    if (encontrados == 1) {
        return &pasajeros[indicesEncontrados[0]];
    }

    // Varias coincidencias = mostrar lista
    printf("\nSe encontraron %d coincidencias:\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        int idx = indicesEncontrados[i];
        printf(" [%d] %s — Tel: %s — Destino: %s\n",
               i + 1,
               pasajeros[idx].nombre,
               pasajeros[idx].telefono,
               pasajeros[idx].destino ? pasajeros[idx].destino->destino : "N/A");
    }

    int opcion = leerEntero("\nSeleccione el pasajero: ", 1, encontrados);
    return &pasajeros[indicesEncontrados[opcion - 1]];
}

Pasajero* buscarPasajeroPorAsiento(Pasajero *pasajeros, int total, Viaje *viajes) {

    if (total == 0) {
        printf("\nNo hay pasajeros registrados.\n");
        return NULL;
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

            // Encontrado
            printf("\nPasajero encontrado:\n");
            printf("Nombre: %s\n", pasajeros[i].nombre);
            printf("Asiento: %d\n", pasajeros[i].num_asiento);
            printf("Destino: %s\n", viajes[idViaje].destino);

            return &pasajeros[i];
        }
    }

    //No encontrado
    printf("\nNo se encontro ningun pasajero en el asiento %d para el viaje %s\n",
           asiento, viajes[idViaje].destino);

    return NULL;
}

