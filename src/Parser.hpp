#ifndef Parser_HPP
#define Parser_HPP

#include <unordered_map>
#include <stdint.h>
#include <inttypes.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

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
    bool canRead;
	string lastModified;
	long long contentLength;

public:
	HTTPRequest parse(string request);
	HTTPResponse respond(HTTPRequest request, string doc_root);
	void getFileStatistics(const char * file_path);
	void clearBuffers();
};

#endif // Parser_HPP