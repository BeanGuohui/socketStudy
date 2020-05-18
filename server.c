#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#define SERVER_PORT 6666
int main(){
	int ser,cli;
	ser = socket(AF_INET,SOCK_STREAM,0);
	if(ser == -1)
	{
		perror("soket error:");
		exit(-1);
	}

	struct sockaddr_in sockaddrser;
	sockaddrser.sin_port = htons(SERVER_PORT);
	sockaddrser.sin_family = AF_INET;
	sockaddrser.sin_addr.s_addr = htonl(INADDR_ANY);
	if( -1 == bind(ser,(struct sockaddr *)&sockaddrser,sizeof(sockaddrser)))
	{
		perror("bind error:");
		exit(-1);
	}
	
	if( -1 == listen(ser, 3))//最后链接3个客户端
	{
		perror("listen error:");
		exit(-1);
	}
	struct sockaddr_in addrclient;
	int len  = sizeof(addrclient);
	printf("listening\n");

	cli = accept(ser,(struct sockaddr *)&addrclient,&len);
	char buf[1024];
	if(cli == -1)
	{
		perror("accept error:");
		exit(-1);
	}


	printf("this is accept\n");
	while(1)
	{
		int n = read(cli,buf,sizeof(buf));
		for(int i = 0;i < n; i++)
			buf[i] = toupper(buf[i]);
		write(cli,buf,n);
	}
	close(ser);
	close(cli);
}
