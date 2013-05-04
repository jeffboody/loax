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

#ifndef loax_server_H
#define loax_server_H

#include <net/net_socket.h>

typedef struct
{
	int w;
	int h;
	net_socket_t* socket_render;
	net_socket_t* socket_event;
} loax_server_t;

loax_server_t* loax_server_new(void);
void           loax_server_delete(loax_server_t** _self);
void           loax_server_resize(loax_server_t* self, int w, int h);
void           loax_server_keydown(loax_server_t* self, int keycode, int meta);
void           loax_server_keyup(loax_server_t* self, int keycode, int meta);
void           loax_server_touch(loax_server_t* self, int action, int count,
                                 float* coord);
// TODO return value?
int            loax_server_draw(loax_server_t* self);

#endif
