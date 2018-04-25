#ifndef CALCPARSER_HPP
#define CALCPARSER_HPP

#include <string>
#include <stdint.h>
#include <inttypes.h>

using namespace std;

// typedef struct CalcInstruction_t {
	// // DEFINE YOUR DATA STRUCTURE HERE
// } CalcInstruction;


class CalcInstruction {
public:
	string operation;
	uint64_t value;
};

class CalcParser {
public:
	static CalcInstruction parse(string insstr);
};

#endif // CALCPARSER_HPP
