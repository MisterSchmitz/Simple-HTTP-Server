#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <sys/types.h>	/* for stat type */
#include <sys/stat.h>	/* for fstat() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <assert.h>
#include "httpd.h"
#include "Framer.hpp"
#include "Parser.hpp"

using namespace std;

int CalcExecute(HTTPRequest req);

void HandleTCPClient(int clntSocket)
{
	Framer f;
	Parser p;
	
	char recv_buffer[1024];
	
	while (1) {
		// Read data from client
		int recv_count = recv(clntSocket, recv_buffer, sizeof(recv_buffer)-1, 0);
		if (recv_count < 0) {
			DieWithError("Receive from client failed.\n");
		}
		if (recv_count == 0){
			// printf("Client closed connection.\n");
			break;
		}
		// printf("recv()'d %d bytes of data in buf\n", recv_count);
		
		// Send received data to framer
		recv_buffer[recv_count] = '\0';	// add a null terminator
		f.append(recv_buffer);
		
		// Clear receive buffer
		memset(&recv_buffer[0], 0, sizeof(recv_buffer));
		
		while (f.hasMessage()) {
			string m = f.topMessage();
			f.popMessage();
			HTTPRequest req = p.parse(m);
			string request = req.response_code+" "+req.method+" "+req.path+" "+req.HTTPversion;
			// Send result to client
			string result = request.append("\r\n");
			// cout << result;
			cout << req.response_code+"\n"+req.method+"\n"+req.path+"\n"+req.HTTPversion+"\n";
			int send_count = send(clntSocket, result.c_str(), result.length(), 0);
			if (send_count < 0) {
				DieWithError("Send result to client failed.\n");
			}
		}
	}
	
    close(clntSocket);    /* Close client socket */
}

int CalcExecute(HTTPRequest req) {
	if (req.method == "END") {
		return 1;
	}
	// else if (req.method == "SET") {
		// acc = req.value;
	// }
	// else if (req.method == "ADD") {
		// acc += req.value;
	// }
	// else if (req.method == "SUB") {
		// acc -= req.value;
	// }
	return 0;
}