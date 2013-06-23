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

#include <stdlib.h>
#include "loax/loax_client.h"
#include "loax/loax_gl2.h"
#include "loax/gl2.h"

#define LOG_TAG "loax"
#include "loax/loax_log.h"

int main(int argc, char** argv)
{
	int w = 0;
	int h = 0;

	while(1)
	{
		loax_client_t* c = loax_client_new();
		if(c == NULL)
		{
			return EXIT_FAILURE;
		}

		do
		{
			loax_event_t e;
			while(loax_client_poll(c, &e))
			{
				if(e.type == LOAX_EVENT_KEYUP)
				{
					LOGI("KEY: keycode=0x%X, meta=0x%X, key=%c",
					     e.event_key.keycode,
					     e.event_key.meta,
					     (char) e.event_key.keycode);
				}
				else if(e.type == LOAX_EVENT_BUTTONUP)
				{
					const char* keystring = loax_eventbutton_keystring(&e.event_button);
					LOGI("BUTTON: id=%i, keycode=0x%X, keystring=%s",
					     e.event_button.id, e.event_button.keycode, keystring);
				}
				else if(e.type == LOAX_EVENT_AXISMOVE)
				{
					const char* axisstring = loax_eventaxis_axisstring(&e.event_axis);
					LOGI("AXIS: id=%i, axis=%s, value=%f",
					     e.event_axis.id, axisstring, e.event_axis.value);
				}
			}

			loax_client_size(c, &w, &h);
			glViewport(0, 0, w, h);
			glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		} while(loax_client_swapbuffers(c));

		loax_client_delete(&c);
	}
}
