#ifndef parser_h
#define parser_h
#include <stdio.h>
#include <stdlib.h>

int skocz_do(FILE* wp, char *tab);

void tekst_pomiedzy(FILE *wp, FILE *zap, char *t);

int nast_new(FILE * wp, FILE * zap);

int parse (char* zapis);
#endif
