#ifndef Parser_HPP
#define Parser_HPP

#include <string>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

// typedef struct HTTPRequestFirstLine_t {
	
// } HTTPRequestFirstLine;

// typedef struct HTTPRequestHeader_t {
	
// } HTTPRequestHeader;

class HTTPRequest {
public:
	// HTTPRequestFirstLine_t HTTPRequestFirstLine;
	int status_code;
	string method;
	string path;
	string HTTPversion;
};

typedef struct HTTPResponseFirstLine_t {
	
} HTTPResponseFirstLine;

typedef struct HTTPResponseHeader_t {
	
} HTTPResponseHeader;

class HTTPResponse {
public:
	string HTTPVersion;
	int status_code;
	string status_code_description;
};

class Parser {
public:
	static HTTPRequest parse(string request);
	static HTTPResponse respond(HTTPRequest request);
};

#endif // Parser_HPP
