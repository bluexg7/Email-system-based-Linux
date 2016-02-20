#include "save.h"
#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
typedef struct info
{
char email[20];
char pwd[20];
char tel[20];
}Info;
void reginfo(char *email,char *pwd,char *tel,int *flag)
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
//enter serve
 n=recv(regfd,ch,21,0);
 ch[n]='\0';
 printf("%s\n",ch);
 send(regfd,"hr",2,0);//judge it is to register,h:host,r:register
//send register infomation
 Info *info=(Info*)malloc(sizeof(Info));
 strcpy(info->email,email);
 strcpy(info->pwd,pwd);
 strcpy(info->tel,tel);
 int size_Info=sizeof(Info);
 char *buf=(char*)malloc(size_Info);
 memcpy(buf,info,size_Info);
 int len=0;
 int pos=0;
  while(pos<size_Info) 
  {
  len=send(regfd,buf+pos,size_Info,0);  
  if(len<0)
  {perror("data  send error \n");break;}
  pos+=len;
  }
  free(buf);
  free(info);
  //
 // receive success register warning 
  n=recv(regfd,ch,BUFSIZ,0);
  ch[n]='\0';
  printf("%s\n",ch);
  if(strcmp(ch,"insert success!")!=0)
  *flag=-1;
 close(regfd);
}
