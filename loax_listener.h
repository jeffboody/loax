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

#ifndef loax_listener_H
#define loax_listener_H

#include <pthread.h>
#include "net/net_socket.h"
#include "loax_event.h"

#define LOAX_LISTENER_BUFSIZE 256

typedef struct
{
	// event state
	pthread_mutex_t event_mutex;
	pthread_cond_t  event_cond;
	int             event_head;
	int             event_tail;
	loax_event_t    event_buffer[LOAX_LISTENER_BUFSIZE];

	// listener state
	net_socket_t*  socket_event;
	net_socket_t*  socket_listen;
	int            thread_cancel;
	pthread_t      thread;
} loax_listener_t;

loax_listener_t* loax_listener_new(void);
void             loax_listener_delete(loax_listener_t** _self);
int              loax_listener_poll(loax_listener_t* self, loax_event_t* e);

#endif
