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
#include "fs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void resolution_path(char * command){
  char * var;
  var = getenv("PATH");
  char ** path;
  path =  malloc(sizeof(char*)*20);
  for(int x=0;x<20;x++){
    path[x] = malloc(sizeof(char*));
  } 
  int index =0;
  strcpy(path[index],strtok(var,":")); index++;
  while(path[index]!=NULL){
    strcpy(path[index],strtok(NULL,":"));
    index++;
  }
  if(strcmp("ls",command)==0){
   char * tmp = strcat(path[5],"/");
   command = strcat(tmp,command);
  }
  else if(strcmp("mkdir",command)==0){
    char * tmp = strcat(path[5],"/");
   command = strcat(tmp,command);
  }
  else if(strcmp("rm",command)==0){
    char * tmp = strcat(path[5],"/");
   command = strcat(tmp,command);
  }
  else if(strcmp("touch",command)==0){
    char * tmp = strcat(path[3],"/");
   command = strcat(tmp,command);
  }
  for (int i = 0; i < 20; i++) {
    free(path[(i)]);
  }
  free(path);
}
int does_exefile_exists(char* path)
{
  struct stat flinf;
  if(stat(path,&flinf)==-1){
    if(strcmp(path,"ls")==0||strcmp(path,"touch")==0||strcmp(path,"rm")==0||strcmp(path,"mkdir")==0||strcmp(path,"grep")==0||strcmp(path,"wget")==0){
      return 1;
    }
    else {
       return 0;
    }
  }
  stat(path,&flinf);
  if(flinf.st_mode & S_IXUSR){
    return 1;
  }
  else{
    return 0;
  }
  
}
