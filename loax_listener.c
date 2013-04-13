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
#include <assert.h>
#include "loax_listener.h"

#define LOG_TAG "loax"
#include "loax_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static void* loax_listener_thread(void* _self)
{
	assert(_self);
	LOGD("debug");

	loax_listener_t* self = (loax_listener_t*) _self;
	int   recvd = 0;
	int   ok    = 1;
	int*  type  = &self->event_buffer[self->event_tail].type;
	void* event = (void*) &self->event_buffer[self->event_tail].event_key;
	while(net_socket_recvall(self->socket_event, (void*) type,
	                         sizeof(int), &recvd))
	{
		if(*type == LOAX_EVENT_RESIZE)
		{
			ok &= net_socket_recvall(self->socket_event, event,
			                         sizeof(loax_eventresize_t),
			                         &recvd);
		}
		else if((*type == LOAX_EVENT_KEYDOWN) ||
		        (*type == LOAX_EVENT_KEYUP))
		{
			ok &= net_socket_recvall(self->socket_event, event,
			                         sizeof(loax_eventkey_t),
			                         &recvd);
		}
		else
		{
			net_socket_shutdown(self->socket_event, NET_SOCKET_SHUT_RDWR);
			LOGE("invalid type=%i", type);
			break;
		}

		if(ok == 0)
		{
			break;
		}

		pthread_mutex_lock(&self->event_mutex);

		self->event_tail = (self->event_tail + 1) % LOAX_LISTENER_BUFSIZE;
		type  = &self->event_buffer[self->event_tail].type;
		event = (void*) &self->event_buffer[self->event_tail].event_key;

		if(self->thread_cancel == 1)
		{
			pthread_mutex_unlock(&self->event_mutex);
			break;
		}

		if(self->event_tail == self->event_head)
		{
			// wait if the buffer is full
			pthread_cond_wait(&self->event_cond, &self->event_mutex);

			if(self->thread_cancel == 1)
			{
				pthread_mutex_unlock(&self->event_mutex);
				break;
			}
		}

		pthread_mutex_unlock(&self->event_mutex);
	}
	return NULL;
}

/***********************************************************
* public                                                   *
***********************************************************/

loax_listener_t* loax_listener_new(void)
{
	LOGD("debug");

	loax_listener_t* self = (loax_listener_t*) malloc(sizeof(loax_listener_t));
	if(self == NULL)
	{
		LOGE("malloc failed");
		return NULL;
	}

	// PTHREAD_MUTEX_DEFAULT is not re-entrant
	if(pthread_mutex_init(&self->event_mutex, NULL) != 0)
	{
		LOGE("pthread_mutex_init failed");
		goto fail_mutex_init;
	}

	if(pthread_cond_init(&self->event_cond, NULL) != 0)
	{
		LOGE("pthread_cond_init failed");
		goto fail_cond_init;
	}

	self->socket_listen = net_socket_listen("6121", NET_SOCKET_TCP_BUFFERED, 1);
	if(self->socket_listen == NULL)
	{
		goto fail_listen;
	}

	self->socket_event = net_socket_accept(self->socket_listen);
	if(self->socket_event == NULL)
	{
		goto fail_accept;
	}

	if(pthread_create(&self->thread, NULL,
	                  loax_listener_thread, (void*) self) != 0)
	{
		LOGE("pthread_create failed");
		goto fail_thread;
	}

	self->event_head    = 0;
	self->event_tail    = 0;
	self->thread_cancel = 0;

	// success
	return self;

	// failure
	fail_thread:
		net_socket_close(&self->socket_event);
	fail_accept:
		net_socket_close(&self->socket_listen);
	fail_listen:
		pthread_cond_destroy(&self->event_cond);
	fail_cond_init:
		pthread_mutex_destroy(&self->event_mutex);
	fail_mutex_init:
		free(self);
	return NULL;
}

void loax_listener_delete(loax_listener_t** _self)
{
	assert(_self);

	loax_listener_t* self = *_self;
	if(self)
	{
		LOGD("debug");
		net_socket_shutdown(self->socket_event, NET_SOCKET_SHUT_RDWR);
		pthread_mutex_lock(&self->event_mutex);
		self->thread_cancel = 1;
		pthread_cond_signal(&self->event_cond);
		pthread_mutex_unlock(&self->event_mutex);
		pthread_join(self->thread, NULL);
		net_socket_close(&self->socket_event);
		net_socket_close(&self->socket_listen);
		pthread_cond_destroy(&self->event_cond);
		pthread_mutex_destroy(&self->event_mutex);
		free(self);
		*_self = NULL;
	}
}

int loax_listener_poll(loax_listener_t* self, loax_event_t* e)
{
	assert(self);
	assert(e);
	LOGD("debug");

	pthread_mutex_lock(&self->event_mutex);

	int has_event = 0;
	if(self->event_head == self->event_tail)
	{
		// buffer is empty
	}
	else
	{
		*e = self->event_buffer[self->event_head];
		self->event_head = (self->event_head + 1) % LOAX_LISTENER_BUFSIZE;
		has_event = 1;
		pthread_cond_signal(&self->event_cond);
	}

	pthread_mutex_unlock(&self->event_mutex);

	return has_event;
}
