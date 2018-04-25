#include <assert.h>
#include <string.h>
#include "Framer.hpp"

using namespace std;

void Framer::append(string chars)
{	
	int recv_count = chars.length();
	// printf("%d bytes passed to? framer\n", recv_count);

	// // consider framing using string methods!!!
	// string delim = "\r\n";
	// int curr_pos = 0;
	// int delim_idx = chars.find(delim, curr_pos);
	
	// // if delim not found before reaching end of receive buffer
	// if (delim_idx == recv_count( {
		// delim_check=0;
		// curr_message.append(
	// }
	
	// // append to curr_message
	
	// // cout << chars.substr(curr_pos,delim_idx+1)<< '\t' << delim_idx << '\n';
	
	
	char delims[2];
	delims[0] = '\r';
	delims[1] = '\n';
	
	// int end_seq_check;
	int msg_ready=0;
	
	int count=0;
	// extract messages from received string
	while (count < recv_count) {
		// find the delimiter
		if (chars[count] == delims[0]) {
			delim_check=1;
		}
		else if (chars[count] == delims[1]) {
			if (delim_check == 1) {
				msg_ready=1;
				delim_check=0;
				msg_buffer[msg_count]='\0';
			}
		}
		else if (delim_check==1) {
			msg_buffer[msg_count]=chars[count-1];
			msg_count++;
			delim_check=0;
		}
		// add character to message buffer
		else {
			msg_buffer[msg_count]=chars[count];
			msg_count++;
		}

		// push message if we have a complete message
		if (msg_ready) {
			messages.push(msg_buffer);
			memset(&msg_buffer[0], 0, sizeof(msg_buffer));  // clear message buffer
			msg_count=0;
			msg_ready=0;
		}

		count++;
	}
}

bool Framer::hasMessage() const
{
	return !(messages.empty());
}

string Framer::topMessage() const
{
	return messages.front();
}

void Framer::popMessage()
{
	messages.pop();
}

// void Framer::printToStream(ostream& stream) const
// {
	// // PUT YOUR CODE HERE
// }