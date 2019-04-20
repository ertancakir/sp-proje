#include "komut.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BOYUT 1024

char *term_satir_oku(void){
  int boyut = BOYUT;
  int konum = 0;
  char *karakterDizi = malloc(sizeof(char) * boyut);
  int c;
  if (!karakterDizi) {
    fprintf(stderr, "bellek ayirma hatasi\n");
    exit(EXIT_FAILURE);
  }
  while (1) {
    if(c == EOF){
        exit(EXIT_SUCCESS);
    }
    if (c == '\n') {
      karakterDizi[konum] = '\0';
      return karakterDizi;
    } else {
      karakterDizi[konum] = c;
    }
    konum++;
    if (konum >= boyut) {
      boyut += BOYUT;
      karakterDizi = realloc(karakterDizi, boyut);
      if (!karakterDizi) {
        fprintf(stderr, "bellek ayirma hatasi\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **term_satir_parcala(char *satir){
  int boyut = isaret_boyut, konum = 0;
  char **tokens = malloc(boyut * sizeof(char*));
  char *token, **tokens_backup;
  printf("7\n");
  if (!tokens) {
    fprintf(stderr, "Hata\n");
    exit(EXIT_FAILURE);
  }
  char buf[64];
  strcpy(buf,satir);
  token = strtok(buf, " -");
  printf("8\n");
  while (token != NULL) {
    tokens[konum] = token;
    konum++;

    if (konum >= boyut) {
      boyut += isaret_boyut;
      tokens_backup = tokens;
      tokens = realloc(tokens, boyut * sizeof(char*));
      if (!tokens) {
		    free(tokens_backup);
        fprintf(stderr, "Hata\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, " -");
  }
  tokens[konum] = NULL;
  return tokens;
}
