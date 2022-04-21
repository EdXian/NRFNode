#include "tcpsocket.h"
#include <unistd.h>

static  int listenfd = 0;

static  int  connfd = 0;

static struct sockaddr_in serv_addr;

static char sendBuff[1025];
static time_t ticks;


#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void wait( int seconds )
{   // Pretty crossplatform, both ALL POSIX compliant systems AND Windows
    #ifdef _WIN32
        Sleep( 1000 * seconds );
    #else
        sleep( seconds );
    #endif
}

void tcpsocket_init(){

       listenfd = socket(AF_INET, SOCK_STREAM, 0);
       memset(&serv_addr, '0', sizeof(serv_addr));
       memset(sendBuff, '0', sizeof(sendBuff));

       serv_addr.sin_family = AF_INET;
       serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
       serv_addr.sin_port = htons(6000);

       bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

       listen(listenfd, 10);
       printf("sockest\n");
       connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
}


void tcpscoket_send(uint8_t* buf,uint32_t len){
    write(connfd, (uint8_t*)buf, len);
}


void tcpsocket_handle(){


        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "today is %.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));
        wait(1);

        //    close(connfd);
        //    sleep(1);
}
