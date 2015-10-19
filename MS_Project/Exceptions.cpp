#include "Exceptions.h"

TerminalException::TerminalException(std::string msg)
{
	error_msg = msg.c_str();
}