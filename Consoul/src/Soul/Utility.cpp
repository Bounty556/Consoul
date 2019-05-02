#include "pch.h"
#include "Utility.h"

namespace Soul {
	bool StartsWith(const char* src, const char* find)
	{
		unsigned int srcLength = strlen(src);
		unsigned int findLength = strlen(find);

		if (findLength > srcLength)
			return false;

		for (int i = 0; i < findLength; i++)
			if (find[i] != src[i])
				return false;

		return true;
	}

	std::string GetSurroundedString(const char* src, const char bound)
	{
		return GetSurroundedString(src, bound, bound);
	}

	std::string GetSurroundedString(const char* src, const char bound1, const char bound2)
	{
		std::string result;

		unsigned int srcLength = strlen(src);
		bool inBounds = false;

		for (int i = 0; i < srcLength; ++i)
		{
			if (inBounds)
				if (src[i] == bound2)
					inBounds = false;
				else
					result += src[i];
			else
				if (src[i] == bound1)
					inBounds = true;
		}

		return result;
	}

	bool IsInt(const char src)
	{
		return src - 48 > 0 && src - 58 < 0;
	}

	unsigned int GetIntInLine(const char* src)
	{
		unsigned int srcLength = strlen(src);
		bool isInInt = false;
		std::string toCast;

		for (int i = 0; i < srcLength; ++i)
		{
			if (isInInt)
			{
				if (IsInt(src[i]))
					toCast += src[i];
				else
					break;
			}
			else
			{
				if (IsInt(src[i]))
					isInInt = true;
			}
		}

		return (unsigned int) std::atoi(toCast.c_str());
	}
}