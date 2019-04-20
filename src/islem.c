#include "islem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jrb.h>
#include <jval.h>
#include <fields.h>

JRB tmpTree;
char *tmp;

int komut_calistir(char** args){
  tmp = malloc(sizeof(char) * 1024);
  memset(tmp,'\0',1024);
  
  if(strcmp(args[0],"tarsau") == 0){
    if(strcmp(args[1],"b") == 0){
      printf("tarsau b komutu\n");
    } 
    else if(strcmp(args[1],"a") == 0){
      printf("tarsau a komutu\n");
    }
  }
  else{
    printf("Komut geçerli değil\n");
  }
  free(tmp);
  return 1;
}
