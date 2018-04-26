#include <assert.h>
#include <string.h>
#include "Framer.hpp"

using namespace std;

void Framer::append(string chars)
{	
	size_t recv_count = chars.length();
	string delim = "\r\n\r\n";
	size_t delim_len = delim.length();
	size_t curr_pos = 0;
	size_t prev_pos = 0;
	size_t delim_pos;
	
	while (curr_pos < recv_count) {
		prev_pos = curr_pos;
		curr_pos = chars.find(delim, prev_pos);
	
		// if delim not yet found, add what we have to curr_message
		if (curr_pos >= recv_count) {
			curr_message.append(chars);
			// search for delim in message buffer (in case it was split between buffers)
			delim_pos = curr_message.find(delim, 0);
			if (delim_pos != curr_message.npos) {
				messages.push(curr_message.substr(0, delim_pos));
				curr_message = curr_message.substr(delim_pos+delim_len,curr_message.npos);
			}
		}
		// otherwise, add from prev_pos up to delim
		else {
			curr_message.append(chars.substr(prev_pos, curr_pos));
			messages.push(curr_message);
			curr_message = "";
			curr_pos += delim_len;
		}
	}
}

bool Framer::hasMessage() const {
	return !(messages.empty());
}

string Framer::topMessage() const {
	return messages.front();
}

void Framer::popMessage() {
	messages.pop();
}