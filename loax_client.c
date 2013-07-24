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
#include "loax_client.h"
#include "loax_gl2.h"
#include "loax_cmd.h"
#include "gl2.h"

#define LOG_TAG "loax"
#include "loax_log.h"

/***********************************************************
* globals                                                  *
***********************************************************/

loax_client_t* g_client = NULL;

/***********************************************************
* public                                                   *
***********************************************************/

loax_client_t* loax_client_new(void)
{
	LOGD("debug");

	loax_client_t* self = (loax_client_t*) malloc(sizeof(loax_client_t));
	if(self == NULL)
	{
		LOGE("malloc failed");
		return NULL;
	}

	net_socket_t* listen = net_socket_listen("6120", NET_SOCKET_TCP_BUFFERED, 1);
	if(listen == NULL)
	{
		goto fail_listen;
	}

	self->socket_render = net_socket_accept(listen);
	if(self->socket_render == NULL)
	{
		goto fail_accept;
	}
	net_socket_close(&listen);

	self->listener = loax_listener_new();
	if(self->listener == NULL)
	{
		goto fail_listener;
	}

	self->errno      = GL_NO_ERROR;
	self->get_string = NULL;

	// success
	g_client = self;   // TODO
	return self;

	// failure
	fail_listener:
		net_socket_close(&self->socket_render);
	fail_accept:
		net_socket_close(&listen);
	fail_listen:
		free(self);
	return NULL;
}

void loax_client_delete(loax_client_t** _self)
{
	assert(_self);

	loax_client_t* self = *_self;
	if(self)
	{
		LOGD("debug");
		free(self->get_string);
		loax_listener_delete(&self->listener);
		net_socket_close(&self->socket_render);
		free(self);
		*_self = NULL;
		g_client = NULL;   // TODO
	}
}

int loax_client_size(loax_client_t* self, int* w, int* h)
{
	assert(self);
	assert(w);
	assert(h);
	LOGD("debug");

	int cmd   = LOAX_Size;
	int recvd = 0;
	net_socket_sendall(self->socket_render, (const void*) &cmd, sizeof(int));
	net_socket_flush(self->socket_render);
	net_socket_recvall(self->socket_render, (void*) w, sizeof(int), &recvd);
	net_socket_recvall(self->socket_render, (void*) h, sizeof(int), &recvd);
	return 1;
}

int loax_client_swapbuffers(loax_client_t* self)
{
	assert(self);
	LOGD("debug");

	int cmd   = LOAX_SwapBuffers;
	int ret   = 0;
	int recvd = 0;
	net_socket_sendall(self->socket_render, (const void*) &cmd, sizeof(int));
	net_socket_flush(self->socket_render);
	net_socket_recvall(self->socket_render, (void*) &ret, sizeof(int), &recvd);
	return ret;
}

int loax_client_poll(loax_client_t* self, loax_event_t* e)
{
	assert(self);
	LOGD("debug");
	return loax_listener_poll(self->listener, e);
}

int loax_client_cmd(loax_client_t* self, int cmd)
{
	assert(self);
	LOGD("debug cmd=%i", cmd);

	int ret   = 0;
	int recvd = 0;
	net_socket_sendall(self->socket_render, (const void*) &cmd, sizeof(int));
	net_socket_flush(self->socket_render);
	net_socket_recvall(self->socket_render, (void*) &ret, sizeof(int), &recvd);
	return ret;
}

void loax_client_seterror(loax_client_t* self, int errno)
{
	assert(self);
	LOGD("debug errno=0x%X", errno);

	if(self->errno == GL_NO_ERROR)
	{
		self->errno = errno;
	}
}
