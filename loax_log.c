/*
 * Copyright (c) 2013 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "loax_log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

void loax_log(const char* func, int line, int type, const char* tag, const char* fmt, ...)
{
	assert(func);
	assert(tag);
	assert(fmt);

	char buf[256];
	#ifdef ANDROID
		snprintf(buf, 256, "%s@%i ", func, line);
	#else
		if(type == ANDROID_LOG_DEBUG)
			snprintf(buf, 256, "D/%s: %s@%i ", tag, func, line);
		else if(type == ANDROID_LOG_INFO)
			snprintf(buf, 256, "I/%s: %s@%i ", tag, func, line);
		else if(type == ANDROID_LOG_WARN)
			snprintf(buf, 256, "W/%s: %s@%i ", tag, func, line);
		else if(type == ANDROID_LOG_ERROR)
			snprintf(buf, 256, "E/%s: %s@%i ", tag, func, line);
	#endif

	int size = (int) strlen(buf);
	if(size < 256)
	{
		va_list argptr;
		va_start(argptr, fmt);
		vsnprintf(&buf[size], 256 - size, fmt, argptr);
		va_end(argptr);
	}
	#ifdef ANDROID
		__android_log_print(type, tag, "%s", buf);
	#else
		printf("%s\n", buf);
	#endif
}
