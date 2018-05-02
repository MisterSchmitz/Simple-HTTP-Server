#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <pthread.h>
#include <iostream>
#include "httpd.hpp"

#define MAXPENDING 5    /* Maximum outstanding connection requests */
#define MAXVERSION 1.1

using namespace std;

string doc_root;

struct ThreadArgs {
    int clntSock;
};

void start_httpd(unsigned short port, string root_arg)
{
	cerr << "Starting server (port: " << port <<
		", doc_root: " << root_arg << ")" << endl;

	doc_root = root_arg;

    int servSock;                    /* Socket descriptor for server */
    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    struct sockaddr_in echoServAddr; /* Local address */

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
	// printf("bind() success\n");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");
	printf("Server listening on port %i\n", port);

    int clntSock;  /* Socket descriptor for client */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int clntLen;            /* Length of client address data structure */
    /* Set the size of the in-out parameter */
    clntLen = sizeof(echoClntAddr);
    for (;;) /* Run forever */
    {
        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
                               &clntLen)) < 0) {
            DieWithError("accept() failed");
        }

        /* clntSock is connected to a client! */
        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        /* Create separate memory for client argument */
        struct ThreadArgs *threadArgs = (struct ThreadArgs *) malloc(
                sizeof(struct ThreadArgs));
        if (threadArgs == NULL) {
            DieWithError("malloc() failed");
        }
        threadArgs->clntSock = clntSock;

        /* Create client thread */
        pthread_t threadId;
        int returnValue = pthread_create(&threadId, NULL, ThreadMain, threadArgs);
        if (returnValue != 0) {
            DieWithError("pthread_create() failed");
        }
        printf("with thread %lu\n", (unsigned long int) threadId);

//        HandleTCPClient(clntSock, doc_root);
    }
    /* NOT REACHED */
}

void *ThreadMain(void *threadArgs) {
    pthread_detach(pthread_self());

    int clntSock = ((struct ThreadArgs *) threadArgs)->clntSock;
    free(threadArgs);

    HandleTCPClient(clntSock, doc_root);

    return (NULL);
}