#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <sys/types.h>	/* for stat type */
#include <sys/stat.h>	/* for fstat() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <fstream>
#include <assert.h>
#include "httpd.hpp"
#include "Framer.hpp"
#include "Parser.hpp"

using namespace std;

vector<char> ReadAllBytes(char const* filename);

void HandleTCPClient(int clntSocket, string doc_root)
{
	Framer f;
	Parser p;

	char recv_buffer[1024];

	// Read data from client
	int recv_count = recv(clntSocket, recv_buffer, sizeof(recv_buffer)-1, 0);

	if (recv_count < 0 && errno == EWOULDBLOCK) {
		printf("Closing socket due to timeout\n");
		close(clntSocket);
		return;
	}
	if (recv_count < 0) {
		DieWithError("Receive from client failed.\n");
	}

	while (recv_count > 0) {
		if (recv_count < 0 && errno == EWOULDBLOCK) {
			printf("Closing socket due to timeout\n");
			break;
		}
		if (recv_count < 0) {
			DieWithError("Receive from client failed.\n");
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

			// Generate Server Response 
			// Header
			p.clearBuffers();
			HTTPResponse resp = p.respond(req, doc_root);
			string server_response = "";
			string initial_line = resp.first_line.HTTPVersion+" "+to_string(resp.first_line.status_code)+" "+resp.first_line.status_code_description+"\r\n";
			server_response.append(initial_line);
			server_response.append(resp.header.server);
			server_response.append(resp.header.last_modified);
			server_response.append(resp.header.content_type);
			server_response.append(resp.header.content_length);
			server_response.append("\r\n");

			// Send response headers to client
			int send_header_count = send(clntSocket, server_response.c_str(), server_response.length(), 0);
			if (send_header_count < 0) {
				DieWithError("Send header response to client failed.\n");
			}

			// If applicable, send response body to client
			if (resp.first_line.status_code == 200) {
				vector<char> content = ReadAllBytes(p.contentPath.c_str());
				int send_body_count = send(clntSocket, &content[0], content.size(), 0);
				if (send_body_count < 0) {
					DieWithError("Send body response to client failed.\n");
				}
			}

			// If client sent message to close socket, close.
            if (req.header.count("Connection")) {
			    string value = req.header.at(("Connection"));
			    if (value=="close") {
					f.reset();
					close(clntSocket);
					return;
			    }
			}
		}
		recv_count = recv(clntSocket, recv_buffer, sizeof(recv_buffer)-1, 0);
	}
	close(clntSocket);    /* Close client socket */
}

/* Read all bytes from a file into a vector
 * https://codereview.stackexchange.com/questions/22901/reading-all-bytes-from-a-file
 */
vector<char> ReadAllBytes(char const* filename) {
	ifstream ifs(filename, ios::binary|ios::ate);
	ifstream::pos_type pos = ifs.tellg();
	std::vector<char>  result(pos);
	ifs.seekg(0, ios::beg);
	ifs.read(&result[0], pos);
	return result;
}