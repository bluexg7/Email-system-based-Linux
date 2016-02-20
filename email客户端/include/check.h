#ifndef __CHECk_H__
#define __CHECK_H__


#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int clientlogin(char *username, char *password,int *flag);
#endif

