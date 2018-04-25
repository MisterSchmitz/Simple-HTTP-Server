#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <sys/types.h>	/* for stat type */
#include <sys/stat.h>	/* for fstat() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <assert.h>
#include "httpd.h"
#include "CalcFramer.hpp"
#include "CalcParser.hpp"

using namespace std;

uint64_t accumulator=0;

int CalcExecute(uint64_t &acc, CalcInstruction ins);

void HandleTCPClient(int clntSocket)
{
	CalcFramer f;
	CalcParser p;
	
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
			CalcInstruction ins = p.parse(m);
			if(CalcExecute(accumulator, ins)) {
				// Send result to client
				string calc_result = to_string(accumulator).append("\r\n");
				int send_count = send(clntSocket, calc_result.c_str(), calc_result.length(), 0);
				if (send_count < 0) {
					DieWithError("Send result to client failed.\n");
				}
				accumulator = 0; // Reset accumulator
			}
		}
	}
	
    close(clntSocket);    /* Close client socket */
}

int CalcExecute(uint64_t &acc, CalcInstruction ins) {
	if (ins.operation == "END") {
		return 1;
	}
	else if (ins.operation == "SET") {
		acc = ins.value;
	}
	else if (ins.operation == "ADD") {
		acc += ins.value;
	}
	else if (ins.operation == "SUB") {
		acc -= ins.value;
	}
	return 0;
}