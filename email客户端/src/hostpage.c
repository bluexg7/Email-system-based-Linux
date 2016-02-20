#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "hostpage.h"
typedef struct e_rec
{
	char topic[50];
	char kind[10];//
	char sender[20];
	char reciever[200];
	char time[20];
	char textnum[20];
	char info[50];
}E_rec;
extern E_rec pioe[200]; //part information of email 
extern char name[20];
extern int total;
void hostpage(char *box)
{
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
        send(regfd,"gs",2,0);// 1,get receive box emails
        send(regfd,name,20,0);//2,username
        send(regfd,box,10,0);//3,which box

        int size_pioe=sizeof(E_rec);
        int count=0;
	int count_unread = 0;
        for(;;count++)
       {
        len=recv(regfd,buf,size_pioe,0);//pioe:part information of email
        if(len==0)
        {printf("receive finished !\n");break;}
        memcpy(&pioe[count],buf,size_pioe);
	if (strcmp(pioe[count].kind,"recu") == 0)
	count_unread ++;
        printf("topic: %s\nkind: %s\nsender: %s\n",pioe[count].topic,pioe[count].kind,pioe[count].sender);
       }
	if (count_unread != 0)
	create_error_window( 8);
        total=count;
        close(regfd);
}
