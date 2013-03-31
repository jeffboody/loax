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

#include <stdio.h>
#include <stdlib.h>
#include "loax/loax_client.h"
#include "loax/loax_gl2.h"
#include "loax/gl2.h"

#define LOG_TAG "loax"
#include "loax/loax_log.h"

int main(int argc, char** argv)
{
	int w;
	int h;
	loax_client_t* c = loax_client_new();
	if(c == NULL)
	{
		return EXIT_FAILURE;
	}

	int idx = 0;
	float mode = 1.0f;
	float color[3] = { 0.0f, 0.0f, 0.0f };
	do
	{
		glClearColor(color[0], color[1], color[2], 1.0f);

		// update next color
		color[idx] += mode * 0.1f;
		if(color[idx] > 1.0f)
		{
			color[idx] = 1.0f;
			++idx;
		}
		else if(color[idx] < 0.0f)
		{
			color[idx] = 0.0f;
			++idx;
		}

		// check for rollover
		if(idx == 3)
		{
			idx = 0;
			mode *= -1.0f;
		}

		loax_client_size(c, &w, &h);
		glViewport(0, 0, w, h);
		glClear(GL_COLOR_BUFFER_BIT);
	} while(loax_client_swapbuffers(c));

	loax_client_delete(&c);

	return EXIT_SUCCESS;
}
