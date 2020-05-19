#include "header.h"
#include <string>
	
#define BYTE unsigned char 

enum CMD//命令的类型
{
	DIR = 1,
	FTP
};
struct DataHeader {//数据报的头节点
	int length;
	short cmd;
};

struct CMD_DIR : public CMD_HEADER//数据报的身子
{
	struct dirent drent;
	CMD_DIR()
	{
		length = sizeof(CMD_DIR);
		cmd = DIR;
	}
};

class myftp
{
public:
	void initStartSock();//初始化sock数据

	void sendFile(const char *filepath, long filelength);//发送文件

	void sendFileCatalogue();//发送文件的目录
	void dealWithOrder();//处理客户端的消息
	void runftp();//服务器开始运转


private:
	std::string SERVER_IP "127.0.0.1"
	int SERVER_PORT 6666
	int fdclient, fdserver;
	char readbuf[512];//单次读取文件到缓冲区的大小为512
	char *dirName = "/home/beanguohui/"
}