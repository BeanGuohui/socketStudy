#include "myftp.h"
#include "header.h"
#include <string>
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
#include <sys/types.h>

#define BYTE unsigned char

// std::string SERVER_IP "127.0.0.1"
// 	int SERVER_PORT 6666
// 	int fdclient, fdserver;

void myftp::initStartSock()//初始化sock数据
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
}

void myftp::sendFile(const char *filepath, long filelength)//发送文件
{
	FILE *pixmap = fopen(filepath,"rb");
	fseek(pixmap,0,SEEK_SET);
	struct stat filestat;
	stat(filepath,&filestat);

	write(fdclient, &filestat, sizeof(filestat));

	
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
void myftp::sendFileCatalogue()
{
	//发送指定的文件目录到客户端
	struct  CMD_DIR sendPage;//发送的包
	DIR *dir = opendir(dirName);
	struct dirent drent;

	while((&dirent = readdir(dir)) != NULL)
	{
		sendPage.drent = drent;
		write(fdclient,&sendPage,sizeof(sendPage));
	}

}
void myftp::dealWithOrder()
{
	struct CMD_DIR recvheader;
	read(fdclient,recvheader,sizeof(recvheader));
	switch(recvheader.CMD)
	{
		case DIR:
		{
			sendFileCatalogue();
			break;
		}
		default:
		{
			break;
		}
	}

}
void myftp::runftp()
{
	 initStartSock();
	 dealWithOrder();



}
