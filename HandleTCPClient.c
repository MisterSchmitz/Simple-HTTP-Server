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
			break;
		}
		
		// Pass received data to framer
		recv_buffer[recv_count] = '\0';	// add a null terminator
		f.append(recv_buffer);
		
		// Clear receive buffer
		memset(&recv_buffer[0], 0, sizeof(recv_buffer));
		
		// Parse received messages
		while (f.hasMessage()) {
			string m = f.topMessage();
			f.popMessage();
			cout << m << "\n";
			
			// Pass message to parser... 			
			// Parse ClientRequest
			HTTPRequest req = p.parse(m);
			string request = req.first_line.method+" "+req.first_line.path+" "+req.first_line.HTTPversion+"\n";
			
			// Generate ServerResponse
			HTTPResponse resp = p.respond(req);
			string server_response = "";
			string initial_line = resp.first_line.HTTPVersion+" "+to_string(resp.first_line.status_code)+" "+resp.first_line.status_code_description+"\r\n";
			server_response.append(initial_line);
			server_response.append(resp.header.server);
			server_response.append(resp.header.last_modified);
			server_response.append(resp.header.content_type);
			server_response.append(resp.header.content_length);
			server_response.append("\r\n");
			
			// Send response to client
			int send_count = send(clntSocket, server_response.c_str(), server_response.length(), 0);
			if (send_count < 0) {
				DieWithError("Send response to client failed.\n");
			}
			// If client sent message to close socket, close.
		}		
	}
    close(clntSocket);    /* Close client socket */
}

int CalcExecute(HTTPRequest req) {
	if (req.first_line.method == "END") {
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