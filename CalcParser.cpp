#include <assert.h>
#include <iostream>
#include <string>
#include "CalcParser.hpp"

CalcInstruction CalcParser::parse(std::string insstr)
{
	CalcInstruction ret;
	
	// Find operation delimiter
	char oper_delim = ' ';
	int oper_delim_idx = insstr.find(oper_delim);
	
	if (oper_delim_idx==-1) {
		ret.operation = "END";
		ret.value = 0;
		return ret;
	}
	
	ret.operation = insstr.substr(0,oper_delim_idx);
	
	// Find value delimiter
	string line_delim = "/r/n/";
	int line_delim_idx = insstr.find(line_delim);
	
	// Convert string to numeric
	uint64_t ins_value = stoull(insstr.substr(oper_delim_idx+1,line_delim_idx));
	ret.value = ins_value;
	
	return ret;
}