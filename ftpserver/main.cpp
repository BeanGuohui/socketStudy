#include <iostream>
#include "myftp.h"#include <stdlib.h>
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
int main()
{

    myftp ftp;
    ftp.runftp();


}