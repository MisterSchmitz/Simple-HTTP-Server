#ifndef Framer_HPP
#define Framer_HPP

#include <iostream>
#include <unistd.h>
#include <vector>
#include <queue>

class Framer {
public:
	void append(std::string chars);
	bool hasMessage() const;
	std::string topMessage() const;
	void popMessage();
	void reset();

protected:
	std::queue<std::string> messages;
	std::string curr_message;
	int delim_check=0;
	int msg_count=0;
};

#endif // Framer_HPP
