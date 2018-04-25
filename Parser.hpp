#ifndef Parser_HPP
#define Parser_HPP

#include <string>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

struct HTTPRequestFirstLine_t {
	int status_code;
	string method;
	string path;
	string HTTPversion;
} ;

// struct HTTPRequestHeader_t {
	
// } ;

class HTTPRequest {
public:
	HTTPRequestFirstLine_t first_line;
	// HTTPRequestHeader_t header;
};

// typedef struct HTTPResponseFirstLine_t {
	
// } HTTPResponseFirstLine;

// typedef struct HTTPResponseHeader_t {
	
// } HTTPResponseHeader;

struct HTTPResponseFirstLine_t {
	string HTTPVersion;
	int status_code;
	string status_code_description;
} ;

class HTTPResponse {
public:
	HTTPResponseFirstLine_t first_line;
};

class Parser {
public:
	static HTTPRequest parse(string request);
	static HTTPResponse respond(HTTPRequest request);
};

#endif // Parser_HPP
