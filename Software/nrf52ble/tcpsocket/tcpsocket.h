#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_


#ifdef __cplusplus

extern "C" {

#endif

#include "stdint.h"

#if _WIN64

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6000"



#endif

#if __unix__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>




#endif

void tcpsocket_init();
void tcpscoket_send(uint8_t* buf,uint32_t len);
void tcpsocket_handle();

#ifdef __cplusplus

}

#endif


#endif
