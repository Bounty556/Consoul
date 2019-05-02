#pragma once

#include <string>

namespace Soul {
	// Returns true if src starts with all the characters in find
	// in the same order
	bool StartsWith(const char* src, const char* find);

	// Returns the string contained between the first 2 instances 
	// of bound in src
	std::string GetSurroundedString(const char* src, const char bound);
	
	// Returns the string contained between the first instance of
	// bound1 and the first instance of bound2
	std::string GetSurroundedString(const char* src, const char bound1, const char bound2);

	// Returns the first unsigned integer found in a string
	unsigned int GetIntInLine(const char* src);
}