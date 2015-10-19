#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

/*
* Exception which requires the program to terminate.
*/
class TerminalException : public std::exception
{
public:
	TerminalException(std::string msg);
	/* Return the error message when caught */
	virtual const char * what() const throw() { return error_msg.c_str(); };
private:
	/* Error message for this exception*/
	std::string error_msg;
};

#endif