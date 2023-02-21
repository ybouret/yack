
#include "yack/rtld/soak.hpp"
#include <cstdarg>

namespace yack
{
	void soak::print(FILE* stream, const char* format, ...) noexcept
	{
		if (stream)
		{
			va_list args;
			va_start(args, format);
			vfprintf(stream, format, args);
			va_end(args);
		}
	}

	bool       soak::verbose = false;


	const char soak::message::fmt[] = "[[soak::app<%s> %s]]\n";
	const char soak::message::init[] = "init";
	const char soak::message::call[] = "call";
	const char soak::message::quit[] = "quit";
	const char soak::message::unknown[] = "unhandled exception";

	void soak::message::disp(const char* id, const char* msg) noexcept
	{
		assert(NULL!=id);
		assert(NULL!=msg);
		YACK_SOAK_VERBOSE(soak::print(stderr, fmt, id, msg));
	}

}
