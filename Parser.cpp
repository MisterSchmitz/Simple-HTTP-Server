#include <assert.h>
#include <iostream>
#include <string>
#include "Parser.hpp"

HTTPRequest Parser::parse(std::string request){
	HTTPRequest req;
	// req.first_line.first_line.status_code = -1
	
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
	
	
	// Valid request, set status_code
	req.first_line.status_code = 200;
	
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