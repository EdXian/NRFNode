#include "tcpsocket.h"
#include <unistd.h>

static  int listenfd = 0;
static  int  connfd = 0;
static struct sockaddr_in serv_addr;
static char sendBuff[1025];
static time_t ticks;


#ifdef _WIN32
    #include <windows.h>
SOCKET ListenSocket = INVALID_SOCKET;
  SOCKET ClientSocket = INVALID_SOCKET;
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

#if __WIN32
    WSADATA wsaData;
      int iResult;



      struct addrinfo *result = NULL;
      struct addrinfo hints;

      int iSendResult;
      char recvbuf[DEFAULT_BUFLEN];
      int recvbuflen = DEFAULT_BUFLEN;

      // Initialize Winsock
      iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
      if (iResult != 0) {
          printf("WSAStartup failed with error: %d\n", iResult);
          return;
      }else{
           printf("WSAStartup succeess.\n");
      }
      ZeroMemory(&hints, sizeof(hints));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;
      hints.ai_flags = AI_PASSIVE;

      // Resolve the server address and port
      iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
      if ( iResult != 0 ) {
          printf("getaddrinfo failed with error: %d\n", iResult);
          WSACleanup();
          return ;
      }

      ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
      if (ListenSocket == INVALID_SOCKET) {
          printf("socket failed with error: %ld\n", WSAGetLastError());
          freeaddrinfo(result);
          WSACleanup();
          return ;
      }

      iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
         if (iResult == SOCKET_ERROR) {
             printf("bind failed with error: %d\n", WSAGetLastError());
             freeaddrinfo(result);
             closesocket(ListenSocket);
             WSACleanup();
             return ;
         }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
            if (iResult == SOCKET_ERROR) {
                printf("listen failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                return ;
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return ;
        }
        printf("set up finished.\n");
#else


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


#endif

}


void tcpscoket_send(uint8_t* buf,uint32_t len){
#if _WIN32
   send( ClientSocket, (char*)buf, (int)len, 0 );
#else

    write(connfd, (uint8_t*)buf, len);
#endif
}


void tcpsocket_handle(){


        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "today is %.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff));
        wait(1);

        //    close(connfd);
        //    sleep(1);
}
