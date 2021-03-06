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

/*/#include "pcre.h"*/

#include "latino.h"

#define LIB_CADENA_NAME "cadena"

static int oct(int octalNumber) {
    int decimalNumber = 0, i = 0;
    while (octalNumber != 0) {
        decimalNumber += (octalNumber % 10) * pow(8, i);
        ++i;
        octalNumber /= 10;
    }
    i = 1;
    return decimalNumber;
}

char *analizar_fmt(const char *s, size_t len) {
    char *ret = malloc(len + 1);
    int i = 0;
    int j = 0;
    int c = '@';
    int num, let = 0;
    for (i = 0; i < ((int)len); i++) {
        switch (s[i]) {
            case '\\': {
                switch (s[i + 1]) {
                    case '/':
                        c = '/';
                        i++;
                        goto save;
                    case '"':
                        c = '\"';
                        i++;
                        goto save;
                    case 'a':
                        c = '\a';
                        i++;
                        goto save;
                    case 'b':
                        c = '\b';
                        i++;
                        goto save;
                    case 'f':
                        c = '\f';
                        i++;
                        goto save;
                    case 'n':
                        c = '\n';
                        i++;
                        goto save;
                    case 'r':
                        c = '\r';
                        i++;
                        goto save;
                    case 't':
                        c = '\t';
                        i++;
                        goto save;
                    case 'v':
                        c = '\v';
                        i++;
                        goto save;
                    case '0':
                        if (isdigit(s[i + 2]) && isdigit(s[i + 3])) {
                            for (num = i + 1; num < strlen(s) && num < i + 4;
                                 num++) {
                                let = (10 * let) + ((int)s[num] - 48);
                            }
                            c = oct(let);
                            let = 0;
                            i += 3;
                            goto save;
                        };
                    case '1':
                        if (isdigit(s[i + 2]) && isdigit(s[i + 3])) {
                            for (num = i + 1; num < strlen(s) && num < i + 4;
                                 num++) {
                                let = (10 * let) + ((int)s[num] - 48);
                            }
                            c = oct(let);
                            let = 0;
                            i += 3;
                            goto save;
                        };
                    case '\\':
                        c = '\\';
                        i++;
                        goto save;
                    case 'u':
                        c = s[i];
                        ret[j] = c;
                        j++;
                        i++;
                        int k;
                        for (k = 0; k <= 4; k++) {
                            c = s[i];
                            ret[j] = c;
                            j++;
                            i++;
                        }
                    default:
                        c = s[i];
                        break;
                }
            } break;
            default:
                c = s[i];
                break;
        }
    save:
        ret[j] = c;
        j++;
    }
    ret[j] = '\0';
    return ret;
}

char *analizar(const char *s, size_t len) {
    char *ret = malloc(len + 1);
    int i = 0;
    int j = 0;
    int c = '@';
    for (i = 0; i < ((int)len); i++) {
        switch (s[i]) {
            case '\\': {
                switch (s[i + 1]) {
                    case '\\':
                        c = '\\';
                        i++;
                        ret[j] = c;
                        j++;
                        c = '\\';
                        i++;
                        ret[j] = c;
                        j++;
                    case 'u':
                        c = s[i];
                        ret[j] = c;
                        j++;
                        i++;
                        int k;
                        for (k = 0; k <= 4; k++) {
                            c = s[i];
                            ret[j] = c;
                            j++;
                            i++;
                        }
                    default:
                        c = s[i];
                        break;
                }
            } break;
            default:
                c = s[i];
                break;
        }
        ret[j] = c;
        j++;
    }
    ret[j] = '\0';
    // printf("ret: %s\n", ret);
    return ret;
}

char *decimal_acadena(double d) {
    char *buffer = calloc(1, 64);
    snprintf(buffer, 64, LAT_NUMERIC_FMT, d);
    return buffer;
}

char *logico_acadena(int i) {
    char *buffer = malloc(10);
    if (i)
        snprintf(buffer, 10, "%s", "verdadero");
    else
        snprintf(buffer, 10, "%s", "falso");
    buffer[9] = '\0';
    return buffer;
}

bool inicia_con(const char *base, const char *str) {
    return (strstr(base, str) - base) == 0;
}

bool termina_con(char *base, char *str) {
    int blen = strlen(base);
    int slen = strlen(str);
    return (blen >= slen) && (!strcmp(base + blen - slen, str));
}

int intercambiar_pos(char *base, char *str, int startIndex) {
    int result;
    int baselen = strlen(base);
    if ((int)strlen(str) > baselen || startIndex > baselen) {
        result = -1;
    } else {
        if (startIndex < 0) {
            startIndex = 0;
        }
        char *pos = strstr(base + startIndex, str);
        if (pos == NULL) {
            result = -1;
        } else {
            result = pos - base;
        }
    }
    return result;
}

int pos(char *base, char *str) { return intercambiar_pos(base, str, 0); }

int ultima_pos(char *base, char *str) {
    int result;
    if (strlen(str) > strlen(base)) {
        result = -1;
    } else {
        int start = 0;
        int endinit = strlen(base) - strlen(str);
        int end = endinit;
        int endtmp = endinit;
        while (start != end) {
            start = intercambiar_pos(base, str, start);
            end = intercambiar_pos(base, str, end);
            if (start == -1) {
                end = -1;
            } else if (end == -1) {
                if (endtmp == (start + 1)) {
                    end = start;
                } else {
                    end = endtmp - (endtmp - start) / 2;
                    if (end <= start) {
                        end = start + 1;
                    }
                    endtmp = end;
                }
            } else {
                start = end;
                end = endinit;
            }
        }
        result = start;
    }
    return result;
}

char *insertar(char *dest, char *src, int pos) {
    int srclen = strlen(src);
    int dstlen = strlen(dest);
    if (pos < 0) {
        pos = dstlen + pos;
    }
    if (pos > dstlen) {
        pos = dstlen;
    }
    char *m = malloc(srclen + dstlen + 1);
    memcpy(m, dest, pos);
    memcpy(m + pos, src, srclen);
    memcpy(m + pos + srclen, dest + pos, dstlen - pos + 1);
    return m;
}

char *rellenar_izquierda(char *base, char *c, int n) {
    char *ret = malloc(MAX_STR_LENGTH);
    int len = strlen(base);
    int i, final = len - 1;
    for (i = 0; i < (n - final); i++) {
        ret = strcat(ret, c);
    }
    ret = strcat(ret, base);
    return ret;
}

char *rellenar_derecha(char *base, char *c, int n) {
    char *ret = malloc(MAX_STR_LENGTH);
    int len = strlen(base);
    strcpy(ret, base);
    int i, final = len - 1;
    for (i = 0; i < (n - final); i++) {
        ret = strcat(ret, c);
    }
    return ret;
}

char *reemplazar(char *o_string, char *s_string, char *r_string) {
	char *buffer = malloc(MAX_STR_LENGTH);
    char *ch;
    if (!(ch = strstr(o_string, s_string))) {
        strcpy(buffer, o_string);
        return buffer;
    }
    strncpy(buffer, o_string, ch - o_string);
    buffer[ch - o_string] = 0;
    sprintf(buffer + (ch - o_string), "%s%s", r_string, ch + strlen(s_string));
    // printf("REEMPLAZAR: %s\n", buffer);
    return buffer;
}

char *subcadena(const char *str, int beg, int n) {
    char *ret = malloc(n + 1);
    strncpy(ret, (str + beg), n);
    *(ret + n) = 0;
    return ret;
}

char *minusculas(const char *str) {
    int i = 0;
    int len = strlen(str);
    char *ret = malloc(len + 1);
    for (i = 0; i < len; i++) {
        ret[i] = tolower(str[i]);
    }
    ret[len] = 0;
    return ret;
}

char *mayusculas(const char *str) {
    int i = 0;
    int len = strlen(str);
    char *ret = malloc(len + 1);
    for (i = 0; i < len; i++) {
        ret[i] = toupper(str[i]);
    }
    ret[len] = 0;
    return ret;
}

char *quitar_espacios(const char *str) {
    char *start;
    char *end;
    for (start = (char *)str; *start; start++) {
        if (!isspace((unsigned char)start[0]))
            break;
    }
    for (end = start + strlen(start); end > start + 1; end--) {
        if (!isspace((unsigned char)end[-1]))
            break;
    }
    char *ret = malloc((end - start));
    *end = 0;
    if (start > str) {
        memcpy(ret, start, (end - start) + 1);
    } else {
        memcpy(ret, str, strlen(str));
    }
    return ret;
}

lat_cadena *latO_cadenaNueva(lat_mv *mv, const char *str, size_t l);

void str_concatenar(lat_mv *mv) {
    // printf("%s\n", "str_concatenar");
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *s1 = latC_astring(mv, a);
    // printf("s1: %s\n", s1);
    char *s2 = latC_astring(mv, b);
    // printf("s2: %s\n", s2);
    int len = strlen(s1) + strlen(s2) + 1;
    char *s3 = calloc(1, len);
    strcat(s3, s1);
    strcat(s3, s2);
    s3[len - 1] = '\0';
    latC_apilar_string(mv, s3);
    free(s1);
    free(s2);
    free(s3);
}

void str_comparar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    lat_objeto *tmp = latC_crear_numerico(
        mv, strcmp(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b)));
    latC_apilar(mv, tmp);
}

void str_contiene(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *result = strstr(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b));
    if (result != NULL) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

void str_termina_con(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    if (termina_con(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b))) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

void str_es_igual(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    if (!strcmp(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b))) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

void str_indice(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *cb = latC_checar_cadena(mv, b);
    char *ca = latC_checar_cadena(mv, a);
    lat_objeto *r = NULL;
    if (a->tipo == T_STR && b->tipo == T_STR) {
        char *cpos = strstr(ca, cb);
        int pos;
        if (cpos == NULL) {
            pos = -1;
        } else {
            pos = cpos - ca;
        }
        r = latC_crear_numerico(mv, pos);
    }
    latC_apilar(mv, r);
    return;
}

void str_insertar(lat_mv *mv) {
    lat_objeto *c = latC_desapilar(mv);
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *buf = insertar(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b),
                         latC_checar_numerico(mv, c));
    lat_objeto *tmp = latC_crear_cadena(mv, buf);
    latC_apilar(mv, tmp);
}

void str_ultimo_indice(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    lat_objeto *tmp = latC_crear_numerico(
        mv, ultima_pos(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b)));
    latC_apilar(mv, tmp);
}

void str_rellenar_izquierda(lat_mv *mv) {
    lat_objeto *c = latC_desapilar(mv);
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *buf =
        rellenar_izquierda(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b),
                           latC_checar_numerico(mv, c));
    lat_objeto *tmp = latC_crear_cadena(mv, buf);
    latC_apilar(mv, tmp);
}

void str_rellenar_derecha(lat_mv *mv) {
    lat_objeto *c = latC_desapilar(mv);
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *buf =
        rellenar_derecha(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b),
                         latC_checar_numerico(mv, c));
    lat_objeto *tmp = latC_crear_cadena(mv, buf);
    latC_apilar(mv, tmp);
}

void str_eliminar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *buf = latC_checar_cadena(mv, a);
    char *res = reemplazar(buf, latC_checar_cadena(mv, b), "");
    lat_objeto *tmp = latC_crear_cadena(mv, res);
    latC_apilar(mv, tmp);
}

void str_esta_vacia(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    if (!strcmp(latC_checar_cadena(mv, a), "")) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

void str_longitud(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    lat_objeto *tmp = NULL;
    if (a->tipo == T_STR) {
        tmp = latC_crear_numerico(mv, strlen(latC_checar_cadena(mv, a)));
    } else if (a->tipo == T_DIC) {
        tmp = latC_crear_numerico(mv, latH_longitud(latC_checar_dic(mv, a)));
    } else {
        tmp = latC_crear_numerico(mv, 0);
    }
    latC_apilar(mv, tmp);
}

char *reemplazar_lat(char *orig, char *rep, char *with) {
    char *result, *ins, *tmp;
    int len_rep, len_with, len_front, count;
    if (!orig && !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL;
    if (!with)
        with = "";
    len_with = strlen(with);
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)) != NULL; ++count) {
        ins = tmp + len_rep;
    }
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);
    if (!result)
        return NULL;
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
    free(result);
    free(tmp);
}

void str_reemplazar(lat_mv *mv) {
    lat_objeto *c = latC_desapilar(mv);
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *cadena_final =
        reemplazar_lat(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b),
                       latC_checar_cadena(mv, c));
    lat_objeto *r = latC_crear_cadena(mv, cadena_final);
    latC_apilar(mv, r);
}

void str_inicia_con(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    bool emp = inicia_con(latC_checar_cadena(mv, a), latC_checar_cadena(mv, b));
    if (emp) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

void str_subcadena(lat_mv *mv) {
    lat_objeto *c = latC_desapilar(mv);
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    lat_objeto *tmp = latC_crear_cadena(
        mv, subcadena(latC_checar_cadena(mv, a), latC_checar_numerico(mv, b),
                      latC_checar_numerico(mv, c)));
    latC_apilar(mv, tmp);
}

void str_minusculas(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    lat_objeto *tmp =
        latC_crear_cadena(mv, minusculas(latC_checar_cadena(mv, a)));
    latC_apilar(mv, tmp);
}

void str_mayusculas(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    lat_objeto *tmp =
        latC_crear_cadena(mv, mayusculas(latC_checar_cadena(mv, a)));
    latC_apilar(mv, tmp);
}

void str_recortar(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    lat_objeto *tmp =
        latC_crear_cadena(mv, quitar_espacios(latC_checar_cadena(mv, a)));
    latC_apilar(mv, tmp);
}

void str_es_numerico(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    char *ptr;
    strtod(latC_checar_cadena(mv, a), &ptr);
    if (!strcmp(ptr, "")) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

void str_esalfa(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    char *cad = latC_checar_cadena(mv, a);
    bool res = true;
    for (int i = 0; i < strlen(cad); i++) {
        if (!isalnum(cad[i])) {
            res = false;
            break;
        }
    }
    if (res) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

void str_separar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *sep = " "; // separador default
    if (b->tipo != T_NULL) {
        sep = latC_checar_cadena(mv, b);
        if (strlen(sep) == 0) {
            sep = " ";
        }
    }
    char *str = latC_checar_cadena(mv, a);
    lista *lst = latL_crear(mv);
    char *tok = strtok(str, sep);
    if (tok) {
        latL_agregar(mv, lst, latC_crear_cadena(mv, tok));
    }
    while (tok != NULL) {
        tok = strtok(NULL, sep);
        if (tok) {
            latL_agregar(mv, lst, latC_crear_cadena(mv, tok));
        }
    }
    lat_objeto *tmp = latC_crear_lista(mv, lst);
    latC_apilar(mv, tmp);
}

void str_invertir(lat_mv *mv) {
    // TODO: Pendiente implementacion para multibyte
    // printf("%s\n", "latM_asignar");
    lat_objeto *a = latC_desapilar(mv);
    char *orig = latC_checar_cadena(mv, a);
    int i = strlen(orig) - 1;
    char *dest = malloc(strlen(orig) + 1);
    int j = 0;
    for (; i >= 0; i--) {
        dest[j] = orig[i];
        j++;
    }
    dest[strlen(orig)] = '\0';
    lat_objeto *tmp = latC_crear_cadena(mv, dest);
    latC_apilar(mv, tmp);
}

void str_ejecutar(lat_mv *mv) {
    int status;
    lat_objeto *o = latC_desapilar(mv);
    char *codigo = latC_checar_cadena(mv, o);
    char *tmp_name = mv->nombre_archivo;
    mv->nombre_archivo = "CADENA";
    ast *nodo = latA_analizar_exp(codigo, &status);
    lat_objeto *func = latC_analizar(mv, nodo);
    if (status == 0 && nodo != NULL) {
        status = latC_llamar_funcion(mv, func);
        latA_destruir(nodo);
    } else {
        latC_error(mv, "Error al ejecutar cadena");
    }
    mv->nombre_archivo = tmp_name;
}

void str_regex(lat_mv *mv) {
    lat_objeto *cadena_regex = latC_desapilar(mv);
    lat_objeto *cad = latC_desapilar(mv);
    char *tmp = NULL;
    if (cad->tipo == T_NUMERIC) {
        tmp = malloc(64);
        sprintf(tmp, "%.16g", latC_checar_numerico(mv, cad));
    } else if (cad->tipo == T_STR) {
        tmp = malloc(strlen(latC_checar_cadena(mv, cad)) + 1);
        strcpy(tmp, latC_checar_cadena(mv, cad));
    } else {
        latC_error(mv, "Error se esperaba una cadena ó númerico.");
    };
    regex_t regex;
    int reti;
    reti = regcomp(&regex, latC_checar_cadena(mv, cadena_regex), REG_EXTENDED);
    if (reti) {
        latC_error(mv, "Error al compilar regex.");
    }
    reti = regexec(&regex, tmp, 0, NULL, 0);
    if (!reti) {
        latC_apilar(mv, latO_verdadero);
    } else if (reti == REG_NOMATCH) {
        latC_apilar(mv, latO_falso);
    } else {
        latC_error(mv, "Error en el match regex.");
    }
    regfree(&regex);
    free(tmp);
}

void str_match(lat_mv *mv) {
    lat_objeto *regexString = latC_desapilar(mv);
    lat_objeto *source = latC_desapilar(mv);
    char maxMatches = 50;
    int maxGroups = 50;
    regex_t regexCompiled;
    regmatch_t groupArray[50];
    char *cursor;
    if (regcomp(&regexCompiled, latC_checar_cadena(mv, regexString),
                REG_EXTENDED)) {
        latC_error(mv, "Error en el match regex");
    };
    cursor = latC_checar_cadena(mv, source);
    lat_objeto *l_matches = latC_crear_lista(mv, latL_crear(mv));
    for (int m = 0; m < maxMatches; m++) {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
            break; // No more matches
        unsigned int offset = 0;
        lat_objeto *l_groups = latC_crear_lista(mv, latL_crear(mv));
        for (int g = 0; g < maxGroups; g++) {
            if (groupArray[g].rm_so == (size_t)-1)
                break; // No more groups
            if (g == 0)
                offset = groupArray[g].rm_eo;
            char *cursorCopy = malloc(strlen(cursor) + 1);
            strcpy(cursorCopy, cursor);
            cursorCopy[groupArray[g].rm_eo] = 0;
            int len = groupArray[g].rm_eo - groupArray[g].rm_so;
            char *str = malloc(len + 1);
            strcpy(str, cursorCopy + groupArray[g].rm_so);
            str[len] = '\0';
            if (strcmp("", str)) {
                latL_agregar(mv, latC_checar_lista(mv, l_groups),
                             latC_crear_cadena(mv, str));
            }
            free(cursorCopy);
        }
        cursor += offset;
        if (latL_longitud(getLista(l_groups)) > 0) {
            latL_agregar(mv, latC_checar_lista(mv, l_matches), l_groups);
        }
    }
    latC_apilar(mv, l_matches);
    regfree(&regexCompiled);
}

void imprimir_pila(lat_mv *mv);

void str_formato(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    double top = latC_checar_numerico(mv, o);
    int arg = 1;
    int i = 0;
    lista *params = latL_crear(mv);
    while (i < top) {
        latL_insertar_inicio(mv, params, latO_clonar(mv, latC_desapilar(mv)));
        i++;
    }
    lat_objeto *ofmt = latL_extraer_inicio(mv, params);
    if (ofmt == NULL) {
        latC_apilar(mv, latO_nulo);
        return;
    }
    char *strfrmt = latC_checar_cadena(mv, ofmt);
    char *strfrmt_end = strfrmt + strlen(strfrmt);
    char *b = calloc(1, 1024);
    while (strfrmt < strfrmt_end) {
        if (*strfrmt != '%') {
            sprintf(b, "%s%c", b, *strfrmt++);
        } else if (*++strfrmt == '%') {
            sprintf(b, "%s%c", b, *strfrmt++);
        } else {
            char buff[1024];
            if (++arg > top) {
                latC_error(mv, "Numero de argumentos invalido para el formato");
            }
            switch (*strfrmt++) {
                case 'c': {
                    lat_objeto *cr = latL_extraer_inicio(mv, params);
                    sprintf(buff, "%c", (int)latC_adouble(mv, cr));
                } break;
                case 'i': {
                    lat_objeto *ent = latL_extraer_inicio(mv, params);
                    sprintf(buff, "%i", (int)latC_adouble(mv, ent));
                } break;
                case 'f': {
                    lat_objeto *dec = latL_extraer_inicio(mv, params);
                    sprintf(buff, "%f", (float)latC_adouble(mv, dec));
                } break;
                case 'd': {
                    lat_objeto *dec = latL_extraer_inicio(mv, params);
                    sprintf(buff, LAT_NUMERIC_FMT, latC_adouble(mv, dec));
                } break;
                case 's': {
                    lat_objeto *str = latL_extraer_inicio(mv, params);
                    sprintf(buff, "%s", latC_astring(mv, str));
                } break;
                default: { latC_error(mv, "Opcion de formato invalida"); }
            }
            strcat(b, buff);
        }
    }
    latC_apilar_string(mv, b);
    latL_limpiar_destruir(mv, params);
    free(b);
}

static const lat_CReg libstr[] = {
    {"esta_vacia", str_esta_vacia, 1},
    {"longitud", str_longitud, 1},
    {"minusculas", str_minusculas, 1},
    {"mayusculas", str_mayusculas, 1},
    {"recortar", str_recortar, 1},
    {"es_numerico", str_es_numerico, 1},
    {"es_numero", str_es_numerico, 1},
    {"es_alfa", str_esalfa, 1},
    {"invertir", str_invertir, 1},
    {"ejecutar", str_ejecutar, 1},
    {"concatenar", str_concatenar, 2},
    {"comparar", str_comparar, 2},
    {"contiene", str_contiene, 2},
    {"termina_con", str_termina_con, 2},
    {"es_igual", str_es_igual, 2},
    {"indice", str_indice, 2},
    {"encontrar", str_indice, 2},
    {"ultimo_indice", str_ultimo_indice, 2},
    {"eliminar", str_eliminar, 2},
    {"separar", str_separar, 2},
    {"inicia_con", str_inicia_con, 2},
    {"regex", str_regex, 2},
    {"match", str_match, 2},
    {"insertar", str_insertar, 3},
    {"rellenar_izquierda", str_rellenar_izquierda, 3},
    {"rellenar_derecha", str_rellenar_derecha, 3},
    {"reemplazar", str_reemplazar, 3},
    {"subcadena", str_subcadena, 3},
    {"formato", str_formato, FUNCION_VAR_ARGS}, // para funciones var_arg
    {NULL, NULL}};

void latC_abrir_liblatino_strlib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_CADENA_NAME, libstr);
}
