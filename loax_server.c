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
#include "gl2.h"
#include "loax_event.h"
#include "loax_gl2.h"
#include "loax_server.h"
#include "loax_server_gl2.h"

#define LOG_TAG "loax"
#include "loax_log.h"

/***********************************************************
* globals                                                  *
***********************************************************/

loax_server_t* g_server = NULL;

/***********************************************************
* private - function map                                   *
***********************************************************/

typedef void (*loax_function_cb)(void);

static loax_function_cb loax_function_map[] =
{
	loaxSwapBuffers,
	loaxSize,
	loaxActiveTexture,
	loaxAttachShader,
	loaxBindAttribLocation,
	loaxBindBuffer,
	loaxBindFramebuffer,
	loaxBindRenderbuffer,
	loaxBindTexture,
	loaxBlendColor,
	loaxBlendEquation,
	loaxBlendEquationSeparate,
	loaxBlendFunc,
	loaxBlendFuncSeparate,
	loaxBufferData,
	loaxBufferSubData,
	loaxCheckFramebufferStatus,
	loaxClear,
	loaxClearColor,
	loaxClearDepthf,
	loaxClearStencil,
	loaxColorMask,
	loaxCompileShader,
	loaxCompressedTexImage2D,
	loaxCompressedTexSubImage2D,
	loaxCopyTexImage2D,
	loaxCopyTexSubImage2D,
	loaxCreateProgram,
	loaxCreateShader,
	loaxCullFace,
	loaxDeleteBuffers,
	loaxDeleteFramebuffers,
	loaxDeleteProgram,
	loaxDeleteRenderbuffers,
	loaxDeleteShader,
	loaxDeleteTextures,
	loaxDepthFunc,
	loaxDepthMask,
	loaxDepthRangef,
	loaxDetachShader,
	loaxDisable,
	loaxDisableVertexAttribArray,
	loaxDrawArrays,
	loaxDrawElements,
	loaxEnable,
	loaxEnableVertexAttribArray,
	loaxFinish,
	loaxFlush,
	loaxFramebufferRenderbuffer,
	loaxFramebufferTexture2D,
	loaxFrontFace,
	loaxGenBuffers,
	loaxGenerateMipmap,
	loaxGenFramebuffers,
	loaxGenRenderbuffers,
	loaxGenTextures,
	loaxGetActiveAttrib,
	loaxGetActiveUniform,
	loaxGetAttachedShaders,
	loaxGetAttribLocation,
	loaxGetBooleanv,
	loaxGetBufferParameteriv,
	loaxGetError,
	loaxGetFloatv,
	loaxGetFramebufferAttachmentParameteriv,
	loaxGetIntegerv,
	loaxGetProgramiv,
	loaxGetProgramInfoLog,
	loaxGetRenderbufferParameteriv,
	loaxGetShaderiv,
	loaxGetShaderInfoLog,
	loaxGetShaderPrecisionFormat,
	loaxGetShaderSource,
	loaxGetString,
	loaxGetTexParameterfv,
	loaxGetTexParameteriv,
	loaxGetUniformfv,
	loaxGetUniformiv,
	loaxGetUniformLocation,
	loaxGetVertexAttribfv,
	loaxGetVertexAttribiv,
	loaxGetVertexAttribPointerv,
	loaxHint,
	loaxIsBuffer,
	loaxIsEnabled,
	loaxIsFramebuffer,
	loaxIsProgram,
	loaxIsRenderbuffer,
	loaxIsShader,
	loaxIsTexture,
	loaxLineWidth,
	loaxLinkProgram,
	loaxPixelStorei,
	loaxPolygonOffset,
	loaxReadPixels,
	loaxReleaseShaderCompiler,
	loaxRenderbufferStorage,
	loaxSampleCoverage,
	loaxScissor,
	loaxShaderBinary,
	loaxShaderSource,
	loaxStencilFunc,
	loaxStencilFuncSeparate,
	loaxStencilMask,
	loaxStencilMaskSeparate,
	loaxStencilOp,
	loaxStencilOpSeparate,
	loaxTexImage2D,
	loaxTexParameterf,
	loaxTexParameterfv,
	loaxTexParameteri,
	loaxTexParameteriv,
	loaxTexSubImage2D,
	loaxUniform1f,
	loaxUniform1fv,
	loaxUniform1i,
	loaxUniform1iv,
	loaxUniform2f,
	loaxUniform2fv,
	loaxUniform2i,
	loaxUniform2iv,
	loaxUniform3f,
	loaxUniform3fv,
	loaxUniform3i,
	loaxUniform3iv,
	loaxUniform4f,
	loaxUniform4fv,
	loaxUniform4i,
	loaxUniform4iv,
	loaxUniformMatrix2fv,
	loaxUniformMatrix3fv,
	loaxUniformMatrix4fv,
	loaxUseProgram,
	loaxValidateProgram,
	loaxVertexAttrib1f,
	loaxVertexAttrib1fv,
	loaxVertexAttrib2f,
	loaxVertexAttrib2fv,
	loaxVertexAttrib3f,
	loaxVertexAttrib3fv,
	loaxVertexAttrib4f,
	loaxVertexAttrib4fv,
	loaxVertexAttribPointer,
	loaxViewport,
};

/***********************************************************
* private - Android conversions                            *
***********************************************************/

#define LOAX_ANDROID_MOTION_ACTION_DOWN 0
#define LOAX_ANDROID_MOTION_ACTION_UP   1
#define LOAX_ANDROID_MOTION_ACTION_MOVE 2

/***********************************************************
* public                                                   *
***********************************************************/

loax_server_t* loax_server_new(void)
{
	LOGD("debug");

	loax_server_t* self = (loax_server_t*) malloc(sizeof(loax_server_t));
	if(self == NULL)
	{
		LOGE("malloc failed");
		return NULL;
	}

	self->w = 0;
	self->h = 0;

	self->socket_render = net_socket_connect("localhost", "6120", NET_SOCKET_TCP_NODELAY);
	if(self->socket_render == NULL)
	{
		goto fail_render;
	}

	self->socket_event = net_socket_connect("localhost", "6121", NET_SOCKET_TCP_NODELAY);
	if(self->socket_event == NULL)
	{
		goto fail_event;
	}

	// success
	g_server = self;   // TODO
	return self;

	// failure
	fail_event:
		net_socket_close(&self->socket_render);
	fail_render:
		free(self);
	return NULL;
}

void loax_server_delete(loax_server_t** _self)
{
	assert(_self);

	loax_server_t* self = *_self;
	if(self)
	{
		LOGD("debug");
		net_socket_close(&self->socket_event);
		net_socket_close(&self->socket_render);
		free(self);
		*_self = NULL;
		g_server = NULL;
	}
}

void loax_server_resize(loax_server_t* self, int w, int h)
{
	assert(self);
	LOGI("%ix%i", w, h);

	self->w = w;
	self->h = h;

	loax_event_t e =
	{
		.type         = LOAX_EVENT_RESIZE,
		.event_resize =
		{
			.w = w,
			.h = h,
		}
	};

	int size = sizeof(int) + sizeof(loax_eventresize_t);
	net_socket_sendall(self->socket_event, (const void*) &e, size);
}

void loax_server_keydown(loax_server_t* self, int keycode, int meta)
{
	assert(self);
	LOGD("debug keycode=0x%X, meta=0x%X", keycode, meta);

	loax_event_t e =
	{
		.type      = LOAX_EVENT_KEYDOWN,
		.event_key =
		{
			.keycode = keycode,
			.meta    = meta,
		}
	};

	int size = sizeof(int) + sizeof(loax_eventkey_t);
	net_socket_sendall(self->socket_event, (const void*) &e, size);
}

void loax_server_keyup(loax_server_t* self, int keycode, int meta)
{
	assert(self);
	LOGD("debug keycode=0x%X, meta=0x%X", keycode, meta);

	loax_event_t e =
	{
		.type      = LOAX_EVENT_KEYUP,
		.event_key =
		{
			.keycode = keycode,
			.meta    = meta,
		}
	};

	int size = sizeof(int) + sizeof(loax_eventkey_t);
	net_socket_sendall(self->socket_event, (const void*) &e, size);
}

void loax_server_buttondown(loax_server_t* self, int id, int keycode)
{
	assert(self);
	LOGD("debug id=%i, keycode=0x%X", id, keycode);

	loax_event_t e =
	{
		.type         = LOAX_EVENT_BUTTONDOWN,
		.event_button =
		{
			.keycode = keycode,
			.id      = id,
		}
	};

	int size = sizeof(int) + sizeof(loax_eventbutton_t);
	net_socket_sendall(self->socket_event, (const void*) &e, size);
}

void loax_server_buttonup(loax_server_t* self, int id, int keycode)
{
	assert(self);
	LOGD("debug id=%i, keycode=0x%X", id, keycode);

	loax_event_t e =
	{
		.type         = LOAX_EVENT_BUTTONUP,
		.event_button =
		{
			.keycode = keycode,
			.id      = id,
		}
	};

	int size = sizeof(int) + sizeof(loax_eventbutton_t);
	net_socket_sendall(self->socket_event, (const void*) &e, size);
}

void loax_server_axismove(loax_server_t* self, int id, int axis, float value)
{
	assert(self);
	LOGD("debug id=%i, axis=0x%X, value=%f", id, axis, value);

	loax_event_t e =
	{
		.type       = LOAX_EVENT_AXISMOVE,
		.event_axis =
		{
			.id    = id,
			.axis  = axis,
			.value = value,
		}
	};

	int size = sizeof(int) + sizeof(loax_eventaxis_t);
	net_socket_sendall(self->socket_event, (const void*) &e, size);
}

void loax_server_touch(loax_server_t* self, int action, int count, float* coord)
{
	assert(self);
	assert(coord);
	LOGD("debug action=%i, count=%i, x0=%0.1f, y0=%0.1f, x1=%0.1f, y1=%0.1f",
	     action, count, coord[0], coord[1], coord[2], coord[3]);

	int type;
	if(action == LOAX_ANDROID_MOTION_ACTION_DOWN)
	{
		type = LOAX_EVENT_TOUCHDOWN;
	}
	else if(action == LOAX_ANDROID_MOTION_ACTION_UP)
	{
		type = LOAX_EVENT_TOUCHUP;
	}
	else if(action == LOAX_ANDROID_MOTION_ACTION_MOVE)
	{
		type = LOAX_EVENT_TOUCHMOVE;
	}
	else
	{
		// ignore depreciated actions
		return;
	}

	if(count < 1)
	{
		LOGW("invalid count=%i", count);
		return;
	}

	if(count > LOAX_EVENT_TOUCHMAX)
	{
		// silently clamp to the max touch events
		count = LOAX_EVENT_TOUCHMAX;
	}

	loax_event_t e =
	{
		.type = type,
		.event_touch =
		{
			.count = count,
		}
	};

	int i;
	for(i = 0; i < count; ++i)
	{
		e.event_touch.coord[i].x = coord[2*i];
		e.event_touch.coord[i].y = coord[2*i + 1];
	}

	int size = 2*sizeof(int) + count*sizeof(loax_eventcoord_t);
	net_socket_sendall(self->socket_event, (const void*) &e, size);
}

int loax_server_draw(loax_server_t* self)
{
	int command;
	int recvd;

	while(net_socket_recvall(self->socket_render, (void*) &command, sizeof(int), &recvd) > 0)
	{
		if((command >= 0) && (command < LOAX_MAX))
		{
			loax_function_map[command]();
		}
		else
		{
			LOGE("invalid command=%i", command);
			break;
		}

		if(command == LOAX_SwapBuffers)
		{
			int one = 1;
			net_socket_sendall(self->socket_render, (void*) &one, sizeof(int));
			return 1;
		}
		else if(command == LOAX_Size)
		{
			net_socket_sendall(self->socket_render, (void*) &self->w, sizeof(int));
			net_socket_sendall(self->socket_render, (void*) &self->h, sizeof(int));
		}
	}

	return 0;
}
