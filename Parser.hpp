#ifndef Parser_HPP
#define Parser_HPP

#include <string>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

// typedef struct HTTPRequest_t {
	// // DEFINE YOUR DATA STRUCTURE HERE
// } HTTPRequest;


class HTTPRequest {
public:
	string response_code;
	string method;
	string path;
	string HTTPversion;
};

class Parser {
public:
	static HTTPRequest parse(string request);
};

#endif // Parser_HPP
