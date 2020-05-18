#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#define SERVER_PORT 6666
#define SERVER_IP "127.0.0.1"
int main()
{
	int fdclient, fdserver;
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
	int length = sizeof(addrserver);

	while(1)
	{
		fdclient = accept(fdserver,(struct sockaddr *)&addrclient, &length);
		printf("客户端链接：IP: %s, PORT: %d\n",
						inet_ntop(AF_INET,&addrclient.sin_addr.s_addr,CLIENTIP,sizeof(CLIENTIP)),
						ntohs(addrclient.sin_port));
		pid_t pid = fork();
		if(pid == -1)
		{
			perror("fork error:");
			exit(-1);
		}
		//signal(SIG_CHILD,delchildren);处理子进程死亡信号
		// else if(pid > 0)
		// {
		// 	fdclient = accept(fdserver,(struct sockaddr *)&addrclient, &length);
		// 	printf("客户端链接：IP: %s, PORT: %d\n",
		// 				inet_ntop(AF_INET,&addrclient.sin_addr.s_addr,CLIENTIP,sizeof(CLIENTIP)),
		// 				ntohs(addrclient.sin_port));
		// pid_t pid = fork();
		// }
		else if(pid == 0)
		{
			break;
		}
	}
	while(1)
			{
				char readbuf[100];
				int n = read(fdclient,readbuf,sizeof(readbuf));
				if( n > 0)
				{
					for(int i = 0; i < n; i++)
					{
						readbuf[i] = toupper(readbuf[i]);
					}
				}
				write(fdclient, readbuf,strlen(readbuf));
				printf("%s\n",readbuf);
			}
}
