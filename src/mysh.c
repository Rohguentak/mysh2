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
#include "parser.h"
#include "utils.h"
#include "fs.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#define BUF_SIZE 1024
#define FILE_SERVER "./uds_server"
int fd[2];
void *server(void * arg){
  int server_socket; int client_socket; int client_addrsize;
  struct sockaddr_un serveradd;
  struct sockaddr_un clientadd;
  char buff_rcv[BUF_SIZE+10]; char buff_snd[BUF_SIZE+10];
  if(access(FILE_SERVER,F_OK)==0){
    unlink(FILE_SERVER);
  }
  fd[0] = socket(PF_FILE,SOCK_STREAM,0); //fd[0] = server_socket;
  memset(&serveradd,0,sizeof(serveradd));
  serveradd.sun_family = AF_UNIX;
  strcpy(serveradd.sun_path,FILE_SERVER);
  bind(fd[0],(struct sockaddr*)&serveradd,sizeof(serveradd));
  while(1){
    listen(server_socket,10);
    client_addrsize = sizeof(clientadd);
    client_socket = accept(server_socket,(struct sockaddr*)&clientadd,&client_addrsize);
    while(1){
      //read(fd[1],buff_rcv,BUF_SIZE);
      //sprintf();
      write(client_socket,&fd[0],strlen(buff_snd)+1);
    }
    close(client_socket);
  }
}
void *client(void * arg){
int client_socket;
struct sockaddr_un server_addr;
char buf[BUF_SIZE+10];
fd[1] = socket(PF_FILE,SOCK_STREAM,0);// fd[1]= client_socket;
memset(&server_addr,0,sizeof(server_addr));
server_addr.sun_family = AF_UNIX;
strcpy(server_addr.sun_path,FILE_SERVER);
connect(fd[1],(struct sockaddr*)&server_addr,sizeof(server_addr));
while(1){
  read(fd[1],&fd[1],BUF_SIZE);
}
close(fd[1]);
}

void sig_handler(int signum){
  if(signum == SIGCHLD){
    if(wait(NULL)!=-1){
      fprintf(stderr,"Zombie alert!!!\n");
    }
  }
}
int main()
{
  signal(SIGINT,SIG_IGN);
  signal(SIGTSTP,SIG_IGN);
  signal(SIGCHLD,sig_handler);

  char command_buffer[4096] = { 0, };
   while (fgets(command_buffer, 4096, stdin) != NULL) {
     //printf("%c",command_buffer[1]);
    int bgindex=0;
    for(int a=0;a<4096;a++){
      if(command_buffer[a]=='|'){
        bgindex=1; 
      }
      else if(command_buffer[a]=='&'){
        bgindex = 2;
      }
    }
    
    int argc = -1;
    char** argv = NULL;
    parse_command(command_buffer, &argc, &argv);
    assert(argv != NULL); 
    if (strcmp(argv[0], "exit") == 0) {
      FREE_2D_ARRAY(argc, argv);
      break;
    }
    struct command_entry* comm_entry = fetch_command(argv[0]);
    int a = does_exefile_exists(argv[0]);
    
    if (comm_entry != NULL) {
      int ret = comm_entry->command_func(argc, argv);
      if (ret != 0) {
        comm_entry->err(ret);
      }
    }
    else if(a==1&&bgindex==2){
       pid_t pid; 
        pid = fork();
        if(pid ==0){
          setpgid(0,0);
          execvp(argv[0],argv);
          exit(1);
        }
        else{
          
        }
    }
    else if(a==1&&bgindex==1){
        pid_t pid; 
        pid = fork();
        if(pid ==0){
         //pthread_t ser;
         pthread_t cli;
        //pthread_create(&ser,NULL,server,NULL);  
         pthread_create(&cli,NULL,server,NULL);
          close(fd[0]);
          dup2(fd[1],STDIN_FILENO);
         execvp(argv[0],argv);

        }
        else{
          pthread_t ser;pthread_create(&ser,NULL,server,NULL); 
          close(fd[1]); 
       
          dup2(fd[0],STDOUT_FILENO);
          waitpid(pid,NULL,0);
      }
    }
    else if (a&&bgindex!=2) {
      
      pid_t pid;
      pid = fork();
      if(pid == 0){
        execvp(argv[0],argv);
      }
      else if(pid != -1){
        waitpid(pid,NULL,0);
      }
    } 
    else{
      assert(comm_entry == NULL);
      fprintf(stderr, "%s: command not found.\n", argv[0]);
    }

    FREE_2D_ARRAY(argc, argv);
  }

  return 0;
}
