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
#define _XOPEN_SOURCE 700
#include "commands.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
static struct command_entry commands[] =
{
  {
    "pwd",
    do_pwd,
    err_pwd
  },
  {
    "cd",
    do_cd,
    err_cd
  },
  {
    "kill",
    do_kill,
    err_kill
  }
};

struct command_entry* fetch_command(const char* command_name)
{
  //struct command_entry*  cmd1 = &(commands[0]);
  //struct command_entry*  cmd2 = &(commands[1]);
  if(strcmp(command_name,"pwd")==0){
    return &(commands[0]);
  }
  else if(strcmp(command_name,"cd")==0){
    return &(commands[1]);
  }
  else if(strcmp(command_name,"kill")==0){
    return &(commands[2]);
  }
  return NULL;
}

int do_pwd(int argc, char** argv)
{
  char buf[100];
  if(argc ==1){
    getcwd(buf,100);
    fprintf(stdout,"%s\n",buf);
    return 0;
  }

  return -1;
}

void err_pwd(int err_code)
{
  // TODO: Fill it.
}

int do_cd(int argc, char** argv)    
{
  if(chdir(argv[1]) ==0){
    return 0;
  }
  else if(chdir(argv[1])== -1){
    if(errno == ENOENT){
      return 1;
    }
    else if(errno == ENOTDIR){
      return 2;
    }
  }
  return -1;
}

void err_cd(int err_code)
{
  if(err_code ==1){
    fprintf(stderr, "cd: no such file or directory\n");
  }
  else if(err_code ==2){
    fprintf(stderr, "cd: not a directory\n");
  }
}
int do_kill(int argc, char** argv)    
{
  pid_t a;
  a = atoi(argv[1]);
  kill(a,SIGKILL);
}
void err_kill(int err_code){

}