#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

/*
* Exception which requires the program to terminate.
*/
class TerminalException : public std::exception
{
public:
	TerminalException(char* msg);
	/* Return the error message when caught */
	virtual const char* what() const throw() { return error_msg; };
private:
	/* Error message for this exception*/
	char* error_msg;
};

#endif