#include "sendpost.h"
#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
extern char name[20];
typedef struct email
{
	char topic[50];
	char kind[10];
	char sender[20];
	char reciever[200];
	char time[20];
	char text[2048];//
	char info[50];
}Email;

void sendpost(char *_topic, char *_kind , char *_sender, char *_reciever, char *_time, char *_text, char *_info)
{
	int regfd,n;
 	char ch[1024];
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
 	Email *myemail=(Email*)malloc(sizeof(Email));
 	strcpy(myemail->topic,_topic);
 	strcpy(myemail->kind,_kind);
	strcpy(myemail->sender,name);
	strcpy(myemail->reciever,_reciever);
	strcpy(myemail->time,_time);
	strcpy(myemail->text,_text);
	strcpy(myemail->info,_info);
 	int size_Email=sizeof(Email);
        send(regfd,"ss",2,0);
 	char *buf=(char*)malloc(size_Email);
 	memcpy(buf,myemail,size_Email);
 	int len=0;
 	int pos=0;
  	while(pos<size_Email) 
  	{
  		len=send(regfd,buf+pos,size_Email,0);  
  		if(len<0)
  		{perror("data  send error \n");break;}
  		pos+=len;
  	}
 //judge it is to check
  //
 // receive success register warning 
  	free(buf);
  	free(myemail);
 	close(regfd);
}

