#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_


#ifdef __cplusplus

extern "C" {

#endif



#if _WIN64




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



void tcpsocket_init();
void tcpscoket_send(uint8_t* buf,uint32_t len);
void tcpsocket_handle();
#endif


#ifdef __cplusplus

}

#endif


#endif
