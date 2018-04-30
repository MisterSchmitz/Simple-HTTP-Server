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
	req.first_line.status_code = 200; // Assume valid, fail later
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
		req.first_line.status_code = 400;
		return req;
	}

	req.first_line.method = request.substr(pos, method_delim_idx);
	pos = method_delim_idx+1;
	
	if (req.first_line.method != "GET") {
		req.first_line.status_code = 400;
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

	if (req.first_line.HTTPversion != "HTTP/1.1") {
		req.first_line.status_code = 400;
		return req;
	}
	
	// Get Key/Value pairs
	string key_delim = ": ";
	string value_delim = "\r\n";
	
	// TODO: Any request headers not in the proper form (e.g., missing a colon), should signal a 400 error.
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
		
		req.header.insert(pair<string, string>(key, value));

		if (value_delim_idx == -1) {
			break;
		}
		else {
			pos = value_delim_idx+value_delim.length();
		}
	}
	
	// Verify host key present
	if(req.header.count("Host") == 0){
		req.first_line.status_code = 400;
	};
	
	// TODO: Connection (optional, if set to “close” then server should close connection with the client after sending response for this request)
	
	return req;
}

HTTPResponse Parser::respond(HTTPRequest req) {
	HTTPResponse resp;
	
	resp.first_line.HTTPVersion = "";
	resp.first_line.status_code = -1;
	resp.first_line.status_code_description = "";
	
	// Highest HTTP version that the server supports
	resp.first_line.HTTPVersion = "HTTP/1.1";
	
	// Status of the request
	resp.first_line.status_code = req.first_line.status_code;
	
	// Status code description
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
	
	// Headers
	// Server name
	resp.header.server = "Server: mjs\r\n";

	if (resp.first_line.status_code != 200){
		return resp;
	}

	// Last-Modified (required only if return type is 200)
	// <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
	resp.header.last_modified = "Last-Modified: TODO\r\n";
	
	// Content-Type (required if return type is 200; otherwise if you create a custom error page, you can set this to ‘text/html’)
	// The Content-Type for .jpg files should be “image/jpeg”, for .png files it should be “image/png”, and for html it should be “text/html”
	resp.header.content_type = "Content-Type: TODO\r\n";
	
	// Content-Length (required if return type is 200; otherwise if you create a custom error page, you can set this to the length of that response)
	resp.header.content_length = "Content-Length: TODO\r\n";
	
	return resp;
}