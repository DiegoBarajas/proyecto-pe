#define MAX_PASAJEROS (8*40)
#define NUM_VIAJES 8

typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

typedef struct {
    char calle[50];
    int  num;
    int  cp;
    char colonia[30];
    char estado[30];
} Ubicacion;

typedef struct {
    Fecha        fecha;
    char         destino[50];
    float        precio;
    unsigned int max_pas;   // Cantidad maxima de pasajeros
    unsigned int pasajeros; // Numero actual de pasajeros
} Viaje;

typedef struct {
    char         nombre[25];
    char         apellidoP[25];
    char         apellidoM[25];

    Fecha        fecha_nac;
    char         rfc[14];
    char         telefono[11];
    Ubicacion    direccion;
    int          edad;
    Viaje        *destino;
    float        descuento;  // En decimal; Ej. 0.1=10%, 0.5=50%
    float        monto_pagado;
    Fecha        fecha_reg;
    unsigned int num_asiento; // UNICO
} Pasajero;
