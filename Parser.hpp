#ifndef Parser_HPP
#define Parser_HPP

#include <string>
#include <unordered_map>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

struct HTTPRequestFirstLine_t {
	int status_code;
	string method;
	string path;
	string HTTPversion;
};

class HTTPRequest {
public:
	HTTPRequestFirstLine_t first_line;
	unordered_map<string, string> header;
};

// typedef struct HTTPResponseFirstLine_t {
	
// } HTTPResponseFirstLine;

// typedef struct HTTPResponseHeader_t {
	
// } HTTPResponseHeader;

struct HTTPResponseFirstLine_t {
	string HTTPVersion;
	int status_code;
	string status_code_description;
};

struct HTTPResponseHeader_t {
    string server;
    string last_modified;
    string content_type;
    string content_length;
};

class HTTPResponse {
public:
	HTTPResponseFirstLine_t first_line;
	HTTPResponseHeader_t header;
	string body; // TODO: Change type?
};

class Parser {
public:
	static HTTPRequest parse(string request);
	static HTTPResponse respond(HTTPRequest request);
};

#endif // Parser_HPP
