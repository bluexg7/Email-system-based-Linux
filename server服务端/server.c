#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include "insert_db.h"
#include "select_db.h"
#include <pthread.h> 
#include "mailin_db.h"
#include "infoout_db.h"
#include "textout_db.h"
#include "change_db.h"
typedef struct info
{
char email[20];
char pwd[20];
char tel[20];
}Info;
typedef struct email
{
	char topic[50];
	char kind[10];
	char sender[20];
	char reciever[200];
	char time[20];
	char text[2048];
	char info[50];
}Email;
//
typedef struct e_rec
{
	char topic[50];
	char kind[10];
	char sender[20];
	char reciever[200];
	char time[20];
	char textnum[20];
	char info[50];
}E_rec;
E_rec part[200];
char bigbuf[4096];//text;
void *callback(void *fd);
int main(int argc, char *argv[])
{
int server_sockfd;
int *client_sockfd;
pthread_t id;//creat thread
struct sockaddr_in my_addr;
int sin_size; 
memset(&my_addr,0,sizeof(my_addr));
my_addr.sin_family=AF_INET;
my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
my_addr.sin_port=htons(6666);
if((server_sockfd=socket(AF_INET,SOCK_STREAM,0))<0) 
 {
  perror("socket"); 
  return 1; 
 }
if (bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
 {
 perror("bind");  
 return 1; 
 }
 listen(server_sockfd,5); 
 while(1)
 {
  struct sockaddr_in remote_addr;
  client_sockfd = (int *)malloc(sizeof(int));
  sin_size=sizeof(struct sockaddr_in); 
  
  if((*client_sockfd=accept(server_sockfd,(struct sockaddr *) &remote_addr,&sin_size))<0) 
  {
  perror("accept"); 
  return 1;
  }
  if(pthread_create(&id,NULL,callback,client_sockfd)==0)
  {
  printf("accept client %s\n",inet_ntoa(remote_addr.sin_addr));
  
  }
 
 } 
  close(*client_sockfd);
  close(server_sockfd);  
 
 return 0;
}
 void *callback(void *fd)
{
   int client_sockfd;
   client_sockfd=*((int*)fd); 
   send(client_sockfd,"Welcome to my server/n",21,0);//send 1th to client

   char check[10]={0};
   int len=0;
   len=recv(client_sockfd,check,2,0);//receive 1th from client
   check[len]='\0';

  if(check[0]=='c')//receive text 
{
 char returnwarn[50]={0};
 char textnum[20];
 recv(client_sockfd,textnum,20,0);
 textout_db(textnum, returnwarn);
 len=send(client_sockfd,bigbuf,4000,0);//send text;
}
  if(check[0]=='d')
{	
	char _name[20] = {0}; 
	recv(client_sockfd,_name,20,0);
	char _mailnum[20] = {0};
	recv(client_sockfd,_mailnum,20,0);
	char _task[10]= {0};
	recv(client_sockfd,_task,10,0);
	char _kind[10]= {0};
	recv(client_sockfd,_kind,10,0);
	char rtnresult[20]={0};
	change_db(_name, _mailnum ,_task ,_kind,rtnresult);
}

  if(check[0]=='g')//receive preview information
  { 
   char name[20];
   len=recv(client_sockfd,name,20,0);//receive name 
   check[len]='\0';
   char box[10];
   len=recv(client_sockfd,box,10,0);//receive box
   box[len]='\0';
   //connect database
   
   int numemail = 0;
   char getwarn[50] = {0};
  
   db_infoout(name,box,getwarn,&numemail);
   
   int  size_rec=sizeof(E_rec);
   char *buf=(char *)malloc(size_rec);
   int i;
   for(i=0;i<numemail;i++)
 {
   memcpy(buf,&part[i],size_rec);
   int lenth=0;
   int pos=0;
   while(pos<size_rec) 
  	{
  		lenth=send(client_sockfd,buf+pos,size_rec,0);  
  		if(lenth<0)
  		{perror("data  send error \n");break;}
  		pos+=lenth;
  	}
}

 }// check kuo hao
   


   if(check[0]=='h')//register,login,
  {
  //receive register infomation
  Info *info=(Info*)malloc(sizeof(Info));
  int size_Info=sizeof(Info);
  char  buf[1024];//char *buf=(char*)malloc(size_Info) ,error
  int pos=0;
//---------------------
   len=recv(client_sockfd,buf,size_Info,0); 
//----------------------------
   memcpy(info,buf,size_Info);
   printf("usrname: %s\npassword: %s\ntel: %s\n",info->email,info->pwd,info->tel);
   char rtnresult[50]={0};
   if(check[1]=='r')
   {
   db_insert(info->email,info->pwd,info->tel,rtnresult);//
   }
   if(check[1]=='l')
   {
   db_select(info->email,info->pwd,rtnresult);//
   }
   len=send(client_sockfd,rtnresult,25,0);
  }
  if(check[0]=='s')//s:send
  {
    Email *myemail=(Email *)malloc(sizeof(Email));
   int size_email=sizeof(Email);
   char  buf[4000];//char *buf=(char*)malloc(size_Info) ,error
   int pos=0;
   int len=0;
   len=recv(client_sockfd,buf,size_email,0); 
   memcpy(myemail,buf,size_email);
   printf("topic:%s\nkind:%s\nsender:%s\nreciever:%s\ntime:%s\ntext:%s\ninfo:%s\n",myemail->topic,myemail->kind,myemail->sender,myemail->reciever,myemail->time,myemail->text,myemail->info);
   char rtnresult[200]={0}; 
   
   mailin_db(myemail->topic,myemail->kind,myemail->sender,myemail->reciever,myemail->time,myemail->text,myemail->info,rtnresult);//
  }
   free(fd);  
   close(client_sockfd);  
   pthread_exit(NULL);  

}

