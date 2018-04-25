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
	
	if (method_delim_idx==-1) {
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
	
	// Get all Header Key/Value pairs
	
	/* Get optional Body
	Messages without a body section still have the trailing CRLF 
	(a blank line) present so that the server knows that it should 
	not expect additional headers. HTTP requests and responses can be 
	of arbitrary size.*/
	
	// Valid request, set status_code
	req.first_line.status_code = 200;
	
	// Parse Header
	
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