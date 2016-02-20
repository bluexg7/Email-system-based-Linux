#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "changemail.h"
extern char name[20];
void changemail(char *_mailnum ,char *task,char *_kind)
{
	printf("kind%s\n",_kind);	
	int regfd,len;
 	struct sockaddr_in serve_addr;
 	memset(&serve_addr,0,sizeof(serve_addr));
 	serve_addr.sin_family=AF_INET;
 	serve_addr.sin_addr.s_addr=htonl(INADDR_ANY);
 	serve_addr.sin_port=htons(6666);
 	if((regfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
 		perror("socket"); 
 		return ; 
	}
 	if(connect(regfd,(struct sockaddr *)&serve_addr,sizeof(struct   sockaddr))<0)
 	{
  		perror("connect"); 
  		return ;
 	}
	char  buf[4096];//char *buf=(char*)malloc(size_Info) ,error
        len=recv(regfd,buf,21,0);
        buf[len]='\0';
        printf("%s\n",buf);
	send(regfd,"de",2,0);
        send(regfd,name,20,0);
	send(regfd,_mailnum,20,0);
	send(regfd,task,10,0);
	send(regfd,_kind,10,0);
	close(regfd);
	
}

