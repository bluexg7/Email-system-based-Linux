#include "check.h"

#define MYPORT  6666
#define BUFFER_SIZE 1024

typedef struct info
{
	char email[20];
	char pwd[20];
	char tel[20];
}Info;

int clientlogin(char *username, char *password,int *flag)
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

	//send check infomation
	send(regfd,"hl",2,0);//judge it is to check,h:host,l:login
	Info *info=(Info*)malloc(sizeof(Info));
	strcpy(info->email,username);
	strcpy(info->pwd,password);
	strcpy(info->tel,"");

	int size_Info=sizeof(Info);

	char *buf=(char*)malloc(size_Info);
	memcpy(buf,info,size_Info);

	int len=0;
	int pos=0;

	while(pos<size_Info) 
	{
		len=send(regfd,buf+pos,size_Info,0);  
		if(len<0)
		{
			perror("data  send error \n");break;
		}
		pos+=len;
	}
	free(buf);
	free(info);

	// receive success register warning 
	n=recv(regfd,ch,25,0);
	ch[n]='\0';
	if(strcmp(ch,"password wrong!")==0)
		*flag=-2;//password wrong!

	if(strcmp(ch,"no user name!")==0)
		*flag=-3;//no user name!

	printf("%s\n",ch);
	close(regfd);
	return 0;
}

