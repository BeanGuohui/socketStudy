#include "myftp.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
using namespace std;
myftp::myftp():port(6666),sockCli(0),sockSer(0),clientOrder(0)
{
    memset(&addrCli,0,sizeof(addrCli));
    memset(&addrSer,0,sizeof(addrSer));
    memset(readFileBuf,0,sizeof(readFileBuf));
    memset(conetServerIp,0,sizeof(conetServerIp));
}

void myftp::initSocket()
{
    sockSer = socket(AF_INET,SOCK_STREAM,0);
    this->addrSer.sin_port = htons(port);
    this->addrSer.sin_family = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&this->addrSer.sin_addr.s_addr);
    bind(sockSer,(sockaddr*)&addrSer,sizeof(addrSer));

    listen(sockSer, 3);
    int len = sizeof(addrCli);
    sockCli = accept(sockSer,(struct sockaddr*)&addrCli,(socklen_t*)&len);
    //输出客户端链接的信息
    cout << "hello" << "accept value : " << sockCli <<  endl;
    cout << "new client connect, IP: "
         << inet_ntop(AF_INET,&addrCli.sin_addr.s_addr,conetServerIp,sizeof(conetServerIp))
         << "\tPORT: " << ntohs(addrCli.sin_port) << endl;
    //接下来就是处理消息了
}

void myftp::sendCatalogue(char *filePath)//发送文件目录
{

    system("ls /home/beanguohui/ -l > /home/beanguohui/catalogue.txt");
    int fd = open("/home/beanguohui/catalogue.txt",O_RDWR);

    int readByte = 0;
    while((readByte= read(fd,readFileBuf,sizeof(readFileBuf))) > 0)
    {
           write(sockCli,readFileBuf,readByte);
    }
    cout << "write sucesss" << endl;

}

void myftp::dealOrder()
{
    while(1)
    {
        cout << "wait for client message..." << endl;
        read(sockCli,&clientOrder,sizeof(clientOrder));
        cout << "recv order:" << clientOrder << endl;
        short int orderOfint = (short int)clientOrder;
        cout << "short int orderOfint" << orderOfint << endl;
        switch (orderOfint) {
        case 49:
        {
            sendCatalogue("/home/beaguohui/");
            break;
        }
        default:
            break;
        }
    }
}

void myftp::runFtp()
{
    initSocket();
    dealOrder();

}

