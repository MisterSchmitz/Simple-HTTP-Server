#include <assert.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
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
	
	// Find method
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
	
	// Find path
	int path_delim_idx = request.find(path_delim, pos);
	string path = request.substr(pos, path_delim_idx-pos);
	if (path == "/") {
		path = "/index.html";
	}
	req.first_line.path = path;
	pos = path_delim_idx+1;
	
	// Find HTTPversion
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

HTTPResponse Parser::respond(HTTPRequest req, string doc_root) {
	clearBuffers();
	HTTPResponse resp;

	resp.first_line.HTTPVersion = "";
	resp.first_line.status_code = -1;
	resp.first_line.status_code_description = "";

	// Highest HTTP version that the server supports
	resp.first_line.HTTPVersion = "HTTP/1.1";

	// Status of the request
	resp.first_line.status_code = req.first_line.status_code;

	// Headers
	// Server name
	resp.header.server = "Server: mjs\r\n";

	if (resp.first_line.status_code != 200) {
		resp.setStatusCodeDescription();
		return resp;
	}

//	string full_path = doc_root.substr(0,doc_root.length()-1)+req.first_line.path;
	string full_path = doc_root+req.first_line.path;
	const char * req_path = full_path.c_str();
	printf("Requested path: %s\n", req_path);

	char pathbuf[1024];
	char *res_path = realpath(req_path, pathbuf);
	if (res_path) {
		printf("This source is at %s.\n", pathbuf);
	} else {
		perror("realpath");
		resp.first_line.status_code = 404;
		resp.setStatusCodeDescription();
		return resp; // If the file does not exist, a file not found error (error code 404) is returned.
	}

	// Is realpath inside of root directory
	int root_idx = string(res_path).find(doc_root, 0);
	if (root_idx != 0) {
		resp.first_line.status_code = 404;
		resp.setStatusCodeDescription();
		return resp;
	}

	// Verify user has permissions to file, and set file statistics
	int stat_status = getFileStatistics(res_path);
	if (stat_status) {
		resp.first_line.status_code = stat_status;
		resp.setStatusCodeDescription();
		return resp;
	}
	getContentType(res_path);

	contentPath = res_path;

	resp.header.last_modified = "Last-Modified: "+lastModified+"\r\n";
	resp.header.content_type = "Content-Type: "+contentType+"\r\n";
	resp.header.content_length = "Content-Length: "+to_string(contentLength)+"\r\n";

	return resp;
}

void Parser::clearBuffers() {
	canRead=false;
	lastModified="";
	contentType="";
	contentLength=0;
	contentPath="";
}

int Parser::getFileStatistics(const char * file_path) {
	struct stat sb;
	if (stat(file_path, &sb) == -1) {
		perror("stat");
		return 404; // If the file does not exist, a file not found error (error code 404) is returned.
	}

	// Verify read permissions
	mode_t file_permission = sb.st_mode;
	canRead = (file_permission & S_IROTH);
	printf("Can read: %i\n", canRead);
	if (!canRead) {
		return 403; // If a file is present but proper permissions are not set, a permission denied error is returned.
	}

	// Last-Modified: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
	char datetime[30];
	const char * format = "%a, %d %b %Y %H:%M:%S GMT";
	strftime(datetime, sizeof(datetime), format, gmtime(&sb.st_mtime));
	lastModified = string(datetime);

	// Content-length
	printf("File size: %lld bytes\n",
		   (long long) sb.st_size);
	contentLength = sb.st_size;

	return 0;
}

/* The Content-Type for .jpg files should be “image/jpeg”,
 * for .png files it should be “image/png”,
 * and for html it should be “text/html”
 */
void Parser::getContentType(const char * file_path) {
	string s = string(file_path);
	string delimiter = ".";
	string fileExtension = s.substr(s.find(delimiter));

	if (fileExtension == ".jpg" || fileExtension == ".jpeg") {
		contentType = "image/jpeg";
	} else if (fileExtension == ".png") {
		contentType = "image/png";
	} else if (fileExtension == ".html") {
		contentType = "text/html";
	}
}

void HTTPResponse::setStatusCodeDescription() {
	// Status code description
	switch (first_line.status_code) {
		case 200:
			first_line.status_code_description = "OK";
			break;
		case 400:
			first_line.status_code_description = "Client Error";
			break;
		case 403:
			first_line.status_code_description = "Forbidden";
			break;
		case 404:
			first_line.status_code_description = "Not Found";
			break;
		default:
			perror("Invalid status code in request.");
	}
}