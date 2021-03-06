/*
The MIT License (MIT)

Copyright (c) Latino - Lenguaje de Programacion

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "latcompat.h"
#include "latgc.h"
#include "latino.h"
#include "latlist.h"
#include "latmem.h"
#include "latmv.h"

#define LIB_SISTEMA_NAME "sis"
volatile sig_atomic_t proceso_detenido;

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

static void latSO_ejecutar(lat_mv *mv) {
    lat_objeto *cmd = latC_desapilar(mv);
    system(latC_checar_cadena(mv, cmd));
}

static void latSO_dormir(lat_mv *mv) {
    lat_objeto *mili_segundos = latC_desapilar(mv);
    latC_sleep(latC_checar_numerico(mv, mili_segundos));
}

static void latSO_pipe(lat_mv *mv) {
    lat_objeto *cmd = latC_desapilar(mv);
    FILE *fp = latC_popen(mv, latC_checar_cadena(mv, cmd), "r");
    char *p = malloc(MAX_BUFFERSIZE);
    fread(p, sizeof(char), MAX_BUFFERSIZE, fp);
    int rlen = strlen(p);
    p[rlen - 1] = '\0'; // elimina el ultimo '\n'
    lat_objeto *tmp = latC_crear_cadena(mv, p);
    latC_apilar(mv, tmp);
    latC_pclose(mv, fp);
    latM_liberar(mv, p);
}

static void latSO_fecha(lat_mv *mv) {
    lat_objeto *tiempo = latC_desapilar(mv);
    char *num = latC_checar_cadena(mv, tiempo);
    time_t raw;
    struct tm *tipo;
    time(&raw);
    tipo = localtime(&raw);
    lat_objeto *tmp = latO_nulo;
    if (!strcmp(num, "seg")) {
        tmp = latC_crear_numerico(mv, tipo->tm_sec); // segundos
    } else if (!strcmp(num, "min")) {
        tmp = latC_crear_numerico(mv, tipo->tm_min);
    } else if (!strcmp(num, "hora")) {
        tmp = latC_crear_numerico(mv, tipo->tm_hour);
    } else if (!strcmp(num, "d_mes")) {
        tmp = latC_crear_numerico(mv, tipo->tm_mday); // dia del mes
    } else if (!strcmp(num, "mes")) {
        tmp = latC_crear_numerico(mv, tipo->tm_mon);
    } else if (!strcmp(num, "año")) {
        tmp = latC_crear_numerico(mv, tipo->tm_year + 1900);
    } else if (!strcmp(num, "d_sem")) {
        tmp = latC_crear_numerico(mv, tipo->tm_wday); // día de la sem.
    } else if (!strcmp(num, "d_año")) {
        tmp = latC_crear_numerico(mv, tipo->tm_yday); // día del año
    } else if (!strcmp(num, "estacion")) {
        tmp = latC_crear_numerico(mv, tipo->tm_isdst); // verano/inv
    } else {
        latC_error(mv, "El formato de tiempo indicado no existe");
    }
    latC_apilar(mv, tmp);
}

static void proceso_al_activarse(int sig) { proceso_detenido = 1; }

static void latSO_avisar(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    signal(latC_checar_numerico(mv, a), proceso_al_activarse);
    if (!proceso_detenido) {
        latC_apilar(mv, latO_falso);
        return;
    } else {
        latC_apilar(mv, latO_verdadero);
    };
}

static void latSO_salir(lat_mv *mv) { exit(0); }

static void latSO_cwd(lat_mv *mv) {
    char dir[1024];
    getcwd(dir, sizeof(dir));
    if (dir != NULL) {
        latC_apilar(mv, latC_crear_cadena(mv, dir));
    } else {
        latC_apilar(mv, latO_nulo);
    };
}

static void latSO_iraxy(lat_mv *mv) {
    lat_objeto *x = latC_desapilar(mv);
    lat_objeto *y = latC_desapilar(mv);
    int yx = latC_checar_numerico(mv, y), xx = latC_checar_numerico(mv, x);
    printf("%c[%d;%df", 0x1B, yx, xx);
    fflush(stdout);
}

static void latSO_usuario(lat_mv *mv) {
    char *user = getenv("USER");
    lat_objeto *tmp;
    if (user != NULL || user) {
        tmp = latC_crear_cadena(mv, user);
    } else {
        tmp = latO_nulo;
    }
    latC_apilar(mv, tmp);
}

static const lat_CReg libsistema[] = {
    {"dormir", latSO_dormir, 1},   {"ejecutar", latSO_ejecutar, 1},
    {"pipe", latSO_pipe, 1},       {"fecha", latSO_fecha, 1},
    {"salir", latSO_salir, 0},     {"avisar", latSO_avisar, 1},
    {"cwd", latSO_cwd, 0},         {"iraxy", latSO_iraxy, 2},
    {"usuario", latSO_usuario, 0}, {NULL, NULL}};

void latC_abrir_liblatino_syslib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_SISTEMA_NAME, libsistema);
}
