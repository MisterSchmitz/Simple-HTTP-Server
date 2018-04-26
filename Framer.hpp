#ifndef Framer_HPP
#define Framer_HPP

#include <iostream>
#include <unistd.h>
#include <vector>
#include <queue>

class Framer {
public:
	void append(std::string chars);

	// Does this buffer contain at least one complete message?
	bool hasMessage() const;

	// Returns the first instruction
	std::string topMessage() const;

	// Removes the first instruction
	void popMessage();

	// // prints the string to an ostream (useful for debugging)
	// void printToStream(std::ostream& stream) const;

protected:
	std::queue<std::string> messages;
	std::string curr_message;
	int delim_check=0;
	int msg_count=0;
};

#endif // Framer_HPP
