#ifndef MYFTP_H
#define MYFTP_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

class myftp
{
public:
    myftp();
    void initSocket();//初始化网络连接
    void sendCatalogue(char *filePath);//发送目录给客户端
    void dealOrder();//处理客户端发来的命令
    void runFtp();//启动并运行ftp

private:
    int port;
    int sockSer;
    int sockCli;
    struct sockaddr_in addrSer;
    struct sockaddr_in addrCli;
    char readFileBuf[512];
    char clientOrder;
    char conetServerIp[12];
};

#endif // MYFTP_H
