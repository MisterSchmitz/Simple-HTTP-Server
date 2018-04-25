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

struct HTTPRequestHeaderKeyValues_t {
    string Host;		// (required, 400 client error if not present)
    string Connection;	//(optional, if set to “close” then server should close connection with the client after sending response for this request)
	string Other;		// Should gracefully handle any other valid request headers that the client sends. Any request headers not in the proper form (e.g., missing a colon), should signal a 400 error.
} ;

class HTTPRequest {
public:
	HTTPRequestFirstLine_t first_line;
	HTTPRequestHeaderKeyValues_t headerkvs;
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
