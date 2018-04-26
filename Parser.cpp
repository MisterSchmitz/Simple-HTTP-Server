#include <assert.h>
#include <iostream>
#include <string>
#include "Parser.hpp"

/*HTTP request and response messages are plain-text, consisting of a 
header section and an optional body section. The header section is 
separated from the body section with a blank line. The header 
consists of an initial line (which is different between requests and 
responses), followed by zero or more key-value pairs. Every line is 
terminated by a CRLF (carriage-return followed by a line feed). */
HTTPRequest Parser::parse(std::string request){
	HTTPRequest req;
	int request_len = request.length();
	
	// Parse First Line
	req.first_line.status_code = -1;
	req.first_line.method = "";
	req.first_line.path = "";
	req.first_line.HTTPversion = "";
	
	char method_delim = ' ';
	char path_delim = ' ';
	string version_delim = "\r\n";
	int pos = 0;
	
	// Find method delimiter
	int method_delim_idx = request.find(method_delim, pos);
	
	if (method_delim_idx == -1) {
		req.first_line.method = "END";
		return req;
	}

	req.first_line.method = request.substr(pos, method_delim_idx);
	pos = method_delim_idx+1;
	
	if (req.first_line.method != "GET") {
		req.first_line.status_code = 400;	 // We only support GET
		return req;
	}
	
	// Find path delimiter
	int path_delim_idx = request.find(path_delim, pos);
	string path = request.substr(pos, path_delim_idx-pos);
	req.first_line.path = path;
	pos = path_delim_idx+1;
	
	// Find HTTPversion delimiter
	int version_delim_idx = request.find(version_delim, pos);
	string HTTPversion = request.substr(pos, version_delim_idx-pos);
	req.first_line.HTTPversion = HTTPversion;
	pos = version_delim_idx+version_delim.length();
	
	// Valid request, set status_code
	req.first_line.status_code = 200;
	
	// Get Key/Value pairs
	string key_delim = ": ";
	string value_delim = "\r\n";
	while (pos < request_len) {
		string value;
		
		int key_delim_idx = request.find(key_delim, pos);
		string key = request.substr(pos, key_delim_idx-pos);
		pos = key_delim_idx+key_delim.length();
			
		int value_delim_idx = request.find(value_delim, pos);
		if (value_delim_idx == -1) {
			value = request.substr(pos, request_len-pos);
		}
		else {
			value = request.substr(pos, value_delim_idx-pos);
		}
		
		req.keys.insert(pair<string, string>(key, value));

		if (value_delim_idx == -1) {
			break;
		}
		else {
			pos = value_delim_idx+value_delim.length();
		}
	}
	
	// Verify host key present
	if(req.keys.count("Host") == 0){
		req.first_line.status_code = 400;
	};
	
	return req;
}

HTTPResponse Parser::respond(HTTPRequest request) {
	HTTPResponse resp;
	
	resp.first_line.HTTPVersion = "";
	resp.first_line.status_code = -1;
	resp.first_line.status_code_description = "";
	
	// TODO: Pull supported HTTPVersion from Server
	resp.first_line.HTTPVersion = "HTTP/1.1";
	
	// Get Status Code from request
	resp.first_line.status_code = request.first_line.status_code;
	
	// Get status code description from status code
	switch (resp.first_line.status_code) {
		case 200:
			resp.first_line.status_code_description = "OK";
			break;
		case 400:
			resp.first_line.status_code_description = "Client Error";
			break;
		case 403:
			resp.first_line.status_code_description = "Forbidden";
			break;
		case 404:
			resp.first_line.status_code_description = "Not Found";
			break;
		default:
			perror("Invalid status code in request.");
	}
	return resp;
}