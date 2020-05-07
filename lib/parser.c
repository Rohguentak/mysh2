/**********************************************************************
 * Copyright (C) Jaewon Choi <jaewon.james.choi@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 *********************************************************************/
#define Com_length 20
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int countargc(const char * input){
  int a =strlen(input); 
  int cnt=0;
  char* tmp = malloc(sizeof(char)*a+1);
  strcpy(tmp,input);
  tmp[a]=' ';
  for(int i=0;i<a;i++){
    if(tmp[i]=='|'||tmp[i]=='&'){
      i++;
    }
    if(tmp[i]=='"'&&tmp[i+1]!=' '){
      while(tmp[i+1]!='"'){
          i++;
      }
    }
    if(tmp[i]!=' ' && tmp[i+1] == ' '){
      cnt++;
    }
    
  }
  free(tmp);
  return cnt;
} 

void parse_command(const char* input, int* argc, char*** argv)
{
  
  *argc = countargc(input); 
  int targc = *argc;
  char tmp[strlen(input)+1]; 
  char * tmpo = NULL;
  strcpy(tmp,input);
  int a=0;
  int aa = 0;
  *argv =  malloc(sizeof(char*)*((targc)+1)); 
  for(int x=0;x<(*argc);x++){
    (*argv)[x]=malloc(sizeof(char*));
  }
  while(aa<strlen(tmp)){
    if(tmp[aa]=='"'){
      tmpo= strtok(tmp,"\"\n");
      tmpo = strtok(NULL,"\"\n");
      strcpy((*argv)[(*argc)-1],tmpo);
      (targc) = (*argc)-1;
      break;
    }
    aa++;
  }
   strcpy((*argv)[a],strtok(tmp," \n|&"));
   //printf("%s, %d\n",(*argv)[a],a);
   while(1){
    a++;
    if(a==targc){
      break;
    }
    
    strcpy((*argv)[a],strtok(NULL," \n|&"));
    //printf("%s, %d\n",(*argv)[a],a);
  }
  (*argv)[*argc] = NULL;
  
}
