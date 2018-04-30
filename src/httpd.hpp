#ifndef HTTPD_H
#define HTTPD_H

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

using namespace std;

void start_httpd(unsigned short port, string doc_root);
void DieWithError(const char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket, string doc_root);   /* TCP client handling function */
int CreateTCPServerSocket(unsigned short port); /* Create TCP server socket */
int AcceptTCPConnection(int servSock);  /* Accept TCP connection request */

#endif // HTTPD_H
