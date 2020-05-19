#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/stat.h>

#define SERVER_PORT 6666
#define BYTE unsigned char

#define SERVER_IP "127.0.0.1"
int fdclient, fdserver;

long filelength(const char *filepath)//获取文件的大小
{
	FILE *pixmap = fopen(filepath,"rb");
	fseek(pixmap, 0, SEEK_END);//把光标设置到文件的开头
	long length = ftell(pixmap);
	fclose(pixmap);
	return length;
}
void sendFile(const char *filepath, long filelength)//发送文件
{
	FILE *pixmap = fopen(filepath,"rb");
	fseek(pixmap,0,SEEK_SET);
	struct stat filestat;
	stat(filepath,&filestat);

	write(fdclient, &filestat, sizeof(filestat));
	printf("first write!\n");
	char readbuf[512];
	
	while(!feof(pixmap))
	{
		int n = fread(readbuf,sizeof(BYTE),sizeof(readbuf),pixmap);
		if(n < 0)
		{
			fclose(pixmap);
			perror("read error:");
			exit(-1);
		}
		write(fdclient, readbuf,n);
		printf("i have send %d bytes!\n",n);			
	}
	fclose(pixmap);
	printf("good news, file send has finished!\n");
}

int main()
{
	
	struct sockaddr_in addrclient, addrserver;
	char CLIENTIP[20];

	fdserver = socket(AF_INET, SOCK_STREAM,0);
	if(fdserver == -1)
	{
		perror("socket error:");
		exit(-1);
	}
	bzero(&addrserver, sizeof(addrserver));
	addrserver.sin_family = AF_INET;
	addrserver.sin_port = htons(6666);
	//addrserver.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, SERVER_IP, &addrserver.sin_addr.s_addr);
	bzero(&addrclient,sizeof(addrclient));
	int rec = bind(fdserver,(struct sockaddr *)&addrserver,sizeof(addrserver));
	if(rec == -1)
	{
		perror("bind error:");
		exit(-1);
	}
	listen(fdserver,3);
	int length1 = sizeof(addrserver);
	fdclient = accept(fdserver,(struct sockaddr *)&addrclient, (socklen_t*)&length1);
		printf("客户端链接：IP: %s, PORT: %d\n",
						inet_ntop(AF_INET,&addrclient.sin_addr.s_addr,CLIENTIP,sizeof(CLIENTIP)),
						ntohs(addrclient.sin_port));
		FILE *pixmap = fopen("./temp.png","rb");

		fseek(pixmap, 0, SEEK_SET);//把光标设置到文件的开头

	while(1)
	{
		char readbuf[330];
		int n = read(fdclient,readbuf,sizeof(readbuf));
		if(n < 1)
		{
			printf("error:\n");
		}
		printf("this is test: ");
		printf("read: %s\n",readbuf);
		sendFile("./file.c",filelength("./file.c"));
		
	}
}