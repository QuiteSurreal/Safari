#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>

#include "Utils.hpp"

void logDebug(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "[LOG DEBUG]: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void logInfo(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "[LOG INFO]: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void logFatal(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, "[LOG FATAL]: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
	std::exit(EXIT_FAILURE);
}

float randf(float min, float max)
{
	const float unit = (float) rand() / (float) RAND_MAX;
	return unit * (max - min) + min;
}

std::string readTextFile(const std::string &path)
{
	std::ifstream file {path};
	if (!file)
	{
		logFatal("failed to open: %s", path.c_str());
	}
	return {
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>()
	};
}