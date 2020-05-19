#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#define SERVER_PORT 6666
#define BYTE unsigned char
#define SERVER_IP "127.0.0.1"

int main()
{
	int client = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addclient, addserver;
	addserver.sin_port = htons(6666);
	addserver.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addserver.sin_addr.s_addr);
	connect(client,(struct sockaddr *)&addserver,sizeof(addserver));
	printf("绑定服务器成功！\n");
	while(1)
	{
		BYTE buf[400];
		scanf("%s",buf);
		write(client,buf,strlen(buf));
		FILE *fp = fopen("./t.png","wb");
		int length = 0 ;
		struct stat filestat;

		int n = read(client,&filestat,sizeof(filestat));
		while(1)
		{
			
			int n = read(client,buf,400);
			if(n == 0)
			{
				break;
			}
			
			fwrite(buf,1,n,fp);
			length = length + n;
			printf("length :%d\n", length);
			printf("filestat.st_size:%d\n",(int)filestat.st_size);
			if(length >= (int)filestat.st_size)
				break;
			printf("n:%d\n",n);
		}

		printf("接收完成\n");
		//printf("%s\n",buf);
		printf("传输完成\n");
		
		
		fclose(fp);
	}


	return 0;
}




