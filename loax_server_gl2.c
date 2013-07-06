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

// TODO malloc(0)

#include <stdlib.h>
#include <assert.h>
#include "gl2.h"
#include "loax_server.h"
#include "loax_server_gl2.h"
#include "loax_serialize.h"

#define LOG_TAG "loax"
#include "loax_log.h"

/***********************************************************
* globals                                                  *
***********************************************************/

extern loax_server_t* g_server;

/***********************************************************
* private                                                  *
***********************************************************/

#define SEND_CHAR(d)             loax_serialize_sendchar(g_server->socket_render, d)
#define SEND_ENUM(d)             loax_serialize_senduint(g_server->socket_render, d)
#define SEND_BOOLEAN(d)          loax_serialize_senduchar(g_server->socket_render, d)
#define SEND_BITFIELD(d)         loax_serialize_senduint(g_server->socket_render, d)
#define SEND_BYTE(d)             loax_serialize_sendchar(g_server->socket_render, d)
#define SEND_SHORT(d)            loax_serialize_sendshort(g_server->socket_render, d)
#define SEND_INT(d)              loax_serialize_sendint(g_server->socket_render, d)
#define SEND_SIZEI(d)            loax_serialize_sendint(g_server->socket_render, d)
#define SEND_UBYTE(d)            loax_serialize_senduchar(g_server->socket_render, d)
#define SEND_USHORT(d)           loax_serialize_sendushort(g_server->socket_render, d)
#define SEND_UINT(d)             loax_serialize_senduint(g_server->socket_render, d)
#define SEND_FLOAT(d)            loax_serialize_sendfloat(g_server->socket_render, d)
#define SEND_CLAMPF(d)           loax_serialize_sendfloat(g_server->socket_render, d)
#define SEND_FIXED(d)            loax_serialize_sendint(g_server->socket_render, d)
#define SEND_INTPTR(d)           loax_serialize_sendintptr(g_server->socket_render, d)
#define SEND_VOIDPTR(d)          loax_serialize_sendvoidptr(g_server->socket_render, d)
#define SEND_SIZEIPTR(d)         loax_serialize_senduint(g_server->socket_render, d)
#define SEND_ARRAY_BOOLEAN(s, d) loax_serialize_sendarrayuchar(g_server->socket_render, s, d)
#define SEND_ARRAY_CHAR(s, d)    loax_serialize_sendarraychar(g_server->socket_render, s, d)
#define SEND_ARRAY_VOID(s, d)    loax_serialize_sendarrayvoid(g_server->socket_render, s, d)
#define SEND_ARRAY_INT(s, d)     loax_serialize_sendarrayint(g_server->socket_render, s, d)
#define SEND_ARRAY_UINT(s, d)    loax_serialize_sendarrayuint(g_server->socket_render, s, d)
#define SEND_ARRAY_FLOAT(s, d)   loax_serialize_sendarrayfloat(g_server->socket_render, s, d)
#define SEND_STRING(d)           loax_serialize_sendstring(g_server->socket_render, d)
#define RECV_CHAR(d)             loax_serialize_recvchar(g_server->socket_render, d)
#define RECV_ENUM(d)             loax_serialize_recvuint(g_server->socket_render, d)
#define RECV_BOOLEAN(d)          loax_serialize_recvuchar(g_server->socket_render, d)
#define RECV_BITFIELD(d)         loax_serialize_recvuint(g_server->socket_render, d)
#define RECV_BYTE(d)             loax_serialize_recvchar(g_server->socket_render, d)
#define RECV_SHORT(d)            loax_serialize_recvshort(g_server->socket_render, d)
#define RECV_INT(d)              loax_serialize_recvint(g_server->socket_render, d)
#define RECV_SIZEI(d)            loax_serialize_recvint(g_server->socket_render, d)
#define RECV_UBYTE(d)            loax_serialize_recvuchar(g_server->socket_render, d)
#define RECV_USHORT(d)           loax_serialize_recvushort(g_server->socket_render, d)
#define RECV_UINT(d)             loax_serialize_recvuint(g_server->socket_render, d)
#define RECV_FLOAT(d)            loax_serialize_recvfloat(g_server->socket_render, d)
#define RECV_CLAMPF(d)           loax_serialize_recvfloat(g_server->socket_render, d)
#define RECV_FIXED(d)            loax_serialize_recvint(g_server->socket_render, d)
#define RECV_INTPTR(d)           loax_serialize_recvintptr(g_server->socket_render, d)
#define RECV_VOIDPTR(d)          loax_serialize_recvvoidptr(g_server->socket_render, d)
#define RECV_SIZEIPTR(d)         loax_serialize_recvuint(g_server->socket_render, d)
#define RECV_ARRAY_BOOLEAN(s, d) loax_serialize_recvarrayuchar(g_server->socket_render, s, d)
#define RECV_ARRAY_CHAR(s, d)    loax_serialize_recvarraychar(g_server->socket_render, s, d)
#define RECV_ARRAY_VOID(s, d)    loax_serialize_recvarrayvoid(g_server->socket_render, s, d)
#define RECV_ARRAY_INT(s, d)     loax_serialize_recvarrayint(g_server->socket_render, s, d)
#define RECV_ARRAY_UINT(s, d)    loax_serialize_recvarrayuint(g_server->socket_render, s, d)
#define RECV_ARRAY_FLOAT(s, d)   loax_serialize_recvarrayfloat(g_server->socket_render, s, d)
#define RECV_STRING(d)           loax_serialize_recvstring(g_server->socket_render, d)
#define SHUTDOWN()               net_socket_shutdown(g_server->socket_render, NET_SOCKET_SHUT_RDWR)

/***********************************************************
* public OPENGL ES2 API                                    *
***********************************************************/

void loaxSwapBuffers (void)
{
	LOGD("debug");
	// TODO
}

void loaxSize (void)
{
	LOGD("debug");
	// TODO
}

void loaxActiveTexture (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum texture;
	recv &= RECV_ENUM(&texture);
	if(recv)
	{
		glActiveTexture(texture);
	}
}

void loaxAttachShader (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	GLuint shader;
	recv &= RECV_UINT(&program);
	recv &= RECV_UINT(&shader);
	if(recv)
	{
		glAttachShader(program, shader);
	}
}

void loaxBindAttribLocation (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	GLuint index;
	GLchar* name = NULL;
	recv &= RECV_UINT(&program);
	recv &= RECV_UINT(&index);
	recv &= RECV_STRING(&name);
	if(recv)
	{
		glBindAttribLocation(program, index, name);
	}
	free(name);
}

void loaxBindBuffer (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLuint buffer;
	recv &= RECV_ENUM(&target);
	recv &= RECV_UINT(&buffer);
	if(recv)
	{
		glBindBuffer(target, buffer);
	}
}

void loaxBindFramebuffer (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLuint framebuffer;
	recv &= RECV_ENUM(&target);
	recv &= RECV_UINT(&framebuffer);
	if(recv)
	{
		glBindFramebuffer(target, framebuffer);
	}
}

void loaxBindRenderbuffer (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLuint renderbuffer;
	recv &= RECV_ENUM(&target);
	recv &= RECV_UINT(&renderbuffer);
	if(recv)
	{
		glBindRenderbuffer(target, renderbuffer);
	}
}

void loaxBindTexture (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLuint texture;
	recv &= RECV_ENUM(&target);
	recv &= RECV_UINT(&texture);
	if(recv)
	{
		glBindTexture(target, texture);
	}
}

void loaxBlendColor (void)
{
	LOGD("debug");
	int recv = 1;
	GLclampf red;
	GLclampf green;
	GLclampf blue;
	GLclampf alpha;
	recv &= RECV_CLAMPF(&red);
	recv &= RECV_CLAMPF(&green);
	recv &= RECV_CLAMPF(&blue);
	recv &= RECV_CLAMPF(&alpha);
	if(recv)
	{
		glBlendColor(red, green, blue, alpha);
	}
}

void loaxBlendEquation (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum mode;
	recv &= RECV_ENUM(&mode);
	if(recv)
	{
		glBlendEquation(mode);
	}
}

void loaxBlendEquationSeparate (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum modeRGB;
	GLenum modeAlpha;
	recv &= RECV_ENUM(&modeRGB);
	recv &= RECV_ENUM(&modeAlpha);
	if(recv)
	{
		glBlendEquationSeparate(modeRGB, modeAlpha);
	}
}

void loaxBlendFunc (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum sfactor;
	GLenum dfactor;
	recv &= RECV_ENUM(&sfactor);
	recv &= RECV_ENUM(&dfactor);
	if(recv)
	{
		glBlendFunc(sfactor, dfactor);
	}
}

void loaxBlendFuncSeparate (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum srcRGB;
	GLenum dstRGB;
	GLenum srcAlpha;
	GLenum dstAlpha;
	recv &= RECV_ENUM(&srcRGB);
	recv &= RECV_ENUM(&dstRGB);
	recv &= RECV_ENUM(&srcAlpha);
	recv &= RECV_ENUM(&dstAlpha);
	if(recv)
	{
		glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
}

void loaxBufferData (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLsizeiptr size;
	GLenum usage;
	recv &= RECV_ENUM(&target);
	recv &= RECV_SIZEIPTR(&size);

	GLvoid* data = (GLvoid*) malloc(size*sizeof(char));
	if(data == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_VOID(size, data);
	recv &= RECV_ENUM(&usage);
	if(recv)
	{
		glBufferData(target, size, data, usage);
	}
	free(data);
}

void loaxBufferSubData (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLintptr offset;
	GLsizeiptr size;
	recv &= RECV_ENUM(&target);
	recv &= RECV_INTPTR(&offset);
	recv &= RECV_SIZEIPTR(&size);

	GLvoid* data = (GLvoid*) malloc(size*sizeof(char));
	if(data == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_VOID(size, data);
	if(recv)
	{
		glBufferSubData(target, offset, size, data);
	}
	free(data);
}

void loaxCheckFramebufferStatus (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum status = GL_FRAMEBUFFER_UNSUPPORTED;
	GLenum target;
	recv &= RECV_ENUM(&target);
	if(recv)
	{
		status = glCheckFramebufferStatus(target);
		SEND_ENUM(status);
	}
}

void loaxClear (void)
{
	LOGD("debug");
	int recv = 1;
	GLbitfield mask;
	recv &= RECV_BITFIELD(&mask);
	if(recv)
	{
		glClear(mask);
	}
}

void loaxClearColor (void)
{
	LOGD("debug");
	int recv = 1;
	GLclampf red;
	GLclampf green;
	GLclampf blue;
	GLclampf alpha;
	recv &= RECV_CLAMPF(&red);
	recv &= RECV_CLAMPF(&green);
	recv &= RECV_CLAMPF(&blue);
	recv &= RECV_CLAMPF(&alpha);
	if(recv)
	{
		glClearColor(red, green, blue, alpha);
	}
}

void loaxClearDepthf (void)
{
	LOGD("debug");
	int recv = 1;
	GLclampf depth;
	recv &= RECV_CLAMPF(&depth);
	if(recv)
	{
		glClearDepthf(depth);
	}
}

void loaxClearStencil (void)
{
	LOGD("debug");
	int recv = 1;
	GLint s;
	recv &= RECV_INT(&s);
	if(recv)
	{
		glClearStencil(s);
	}
}

void loaxColorMask (void)
{
	LOGD("debug");
	int recv = 1;
	GLboolean red;
	GLboolean green;
	GLboolean blue;
	GLboolean alpha;
	recv &= RECV_BOOLEAN(&red);
	recv &= RECV_BOOLEAN(&green);
	recv &= RECV_BOOLEAN(&blue);
	recv &= RECV_BOOLEAN(&alpha);
	if(recv)
	{
		glColorMask(red, green, blue, alpha);
	}
}

void loaxCompileShader (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint shader;
	recv &= RECV_UINT(&shader);
	if(recv)
	{
		glCompileShader(shader);
	}
}

void loaxCompressedTexImage2D (void)
{
	LOGD("debug");
	// TODO
}

void loaxCompressedTexSubImage2D (void)
{
	LOGD("debug");
	// TODO
}

void loaxCopyTexImage2D (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLint level;
	GLenum internalformat;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	GLint border;
	recv &= RECV_ENUM(&target);
	recv &= RECV_INT(&level);
	recv &= RECV_ENUM(&internalformat);
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	recv &= RECV_INT(&border);
	if(recv)
	{
		glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	}
}

void loaxCopyTexSubImage2D (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	recv &= RECV_ENUM(&target);
	recv &= RECV_INT(&level);
	recv &= RECV_INT(&xoffset);
	recv &= RECV_INT(&yoffset);
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	if(recv)
	{
		glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	}
}

void loaxCreateProgram (void)
{
	LOGD("debug");
	GLuint program = glCreateProgram();
	SEND_UINT(program);
}

void loaxCreateShader (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint shader = 0;
	GLenum type;
	recv &= RECV_ENUM(&type);
	if(recv)
	{
		shader = glCreateShader(type);
	}
	SEND_UINT(shader);
}

void loaxCullFace (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum mode;
	recv &= RECV_ENUM(&mode);
	if(recv)
	{
		glCullFace(mode);
	}
}

void loaxDeleteBuffers (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n = 0;
	recv &= RECV_SIZEI(&n);

	GLuint* buffers = (GLuint*) malloc(n*sizeof(GLuint));
	if(buffers == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_UINT(n, buffers);
	if(recv)
	{
		glDeleteBuffers(n, buffers);
	}
	free(buffers);
}

void loaxDeleteFramebuffers (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n = 0;
	recv &= RECV_SIZEI(&n);

	GLuint* framebuffers = (GLuint*) malloc(n*sizeof(GLuint));
	if(framebuffers == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_UINT(n, framebuffers);
	if(recv)
	{
		glDeleteFramebuffers(n, framebuffers);
	}
	free(framebuffers);
}

void loaxDeleteProgram (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	recv &= RECV_UINT(&program);
	if(recv)
	{
		glDeleteProgram(program);
	}
}

void loaxDeleteRenderbuffers (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n = 0;
	recv &= RECV_SIZEI(&n);

	GLuint* renderbuffers = (GLuint*) malloc(n*sizeof(GLuint));
	if(renderbuffers == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_UINT(n, renderbuffers);
	if(recv)
	{
		glDeleteRenderbuffers(n, renderbuffers);
	}
	free(renderbuffers);
}

void loaxDeleteShader (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint shader;
	recv &= RECV_UINT(&shader);
	if(recv)
	{
		glDeleteShader(shader);
	}
}

void loaxDeleteTextures (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n = 0;
	recv &= RECV_SIZEI(&n);

	GLuint* textures = (GLuint*) malloc(n*sizeof(GLuint));
	if(textures == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_UINT(n, textures);
	if(recv)
	{
		glDeleteTextures(n, textures);
	}
	free(textures);
}

void loaxDepthFunc (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum func;
	recv &= RECV_ENUM(&func);
	if(recv)
	{
		glDepthFunc(func);
	}
}

void loaxDepthMask (void)
{
	LOGD("debug");
	int recv = 1;
	GLboolean flag;
	recv &= RECV_BOOLEAN(&flag);
	if(recv)
	{
		glDepthMask(flag);
	}
}

void loaxDepthRangef (void)
{
	LOGD("debug");
	int recv = 1;
	GLclampf zNear;
	GLclampf zFar;
	recv &= RECV_CLAMPF(&zNear);
	recv &= RECV_CLAMPF(&zFar);
	if(recv)
	{
		glDepthRangef(zNear, zFar);
	}
}

void loaxDetachShader (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	GLuint shader;
	recv &= RECV_UINT(&program);
	recv &= RECV_UINT(&shader);
	if(recv)
	{
		glDetachShader(program, shader);
	}
}

void loaxDisable (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum cap;
	recv &= RECV_ENUM(&cap);
	if(recv)
	{
		glDisable(cap);
	}
}

void loaxDisableVertexAttribArray (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint index;
	recv &= RECV_UINT(&index);
	if(recv)
	{
		glDisableVertexAttribArray(index);
	}
}

void loaxDrawArrays (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum mode;
	GLint first;
	GLsizei count;
	recv &= RECV_ENUM(&mode);
	recv &= RECV_INT(&first);
	recv &= RECV_SIZEI(&count);
	if(recv)
	{
		glDrawArrays(mode, first, count);
	}
}

void loaxDrawElements (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum mode;
	GLsizei count = 0;
	GLenum type;
	GLvoid* indices;
	recv &= RECV_ENUM(&mode);
	recv &= RECV_SIZEI(&count);
	recv &= RECV_ENUM(&type);
	recv &= RECV_VOIDPTR(&indices);
	if(recv)
	{
		if(indices != NULL)
		{
			LOGE("VBOs are required");
			SHUTDOWN();
			return;
		}
		glDrawElements(mode, count, type, indices);
	}
}

void loaxEnable (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum cap;
	recv &= RECV_ENUM(&cap);
	if(recv)
	{
		glEnable(cap);
	}
}

void loaxEnableVertexAttribArray (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint index;
	recv &= RECV_UINT(&index);
	if(recv)
	{
		glEnableVertexAttribArray(index);
	}
}

void loaxFinish (void)
{
	LOGD("debug");
	glFinish();
}

void loaxFlush (void)
{
	LOGD("debug");
	glFlush();
}

void loaxFramebufferRenderbuffer (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum attachment;
	GLenum renderbuffertarget;
	GLuint renderbuffer;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&attachment);
	recv &= RECV_ENUM(&renderbuffertarget);
	recv &= RECV_UINT(&renderbuffer);
	if(recv)
	{
		glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	}
}

void loaxFramebufferTexture2D (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum attachment;
	GLenum textarget;
	GLuint texture;
	GLint level;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&attachment);
	recv &= RECV_ENUM(&textarget);
	recv &= RECV_UINT(&texture);
	recv &= RECV_INT(&level);
	if(recv)
	{
		glFramebufferTexture2D(target, attachment, textarget, texture, level);
	}
}

void loaxFrontFace (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum mode;
	recv &= RECV_ENUM(&mode);
	if(recv)
	{
		glFrontFace(mode);
	}
}

void loaxGenBuffers (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n;
	recv &= RECV_SIZEI(&n);

	GLuint* buffers = (GLuint*) malloc(n*sizeof(GLuint));
	if(buffers == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGenBuffers(n, buffers);
		SEND_ARRAY_UINT(n, buffers);

		int i;
		for(i = 0; i < n; ++i)
		{
			LOGD("%i: %u", i, buffers[i]);
		}
	}
	free(buffers);
}

void loaxGenerateMipmap (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	recv &= RECV_ENUM(&target);
	if(recv)
	{
		glGenerateMipmap(target);
	}
}

void loaxGenFramebuffers (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n;
	recv &= RECV_SIZEI(&n);

	GLuint* framebuffers = (GLuint*) malloc(n*sizeof(GLuint));
	if(framebuffers == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGenFramebuffers(n, framebuffers);
		SEND_ARRAY_UINT(n, framebuffers);
	}
	free(framebuffers);
}

void loaxGenRenderbuffers (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n;
	recv &= RECV_SIZEI(&n);

	GLuint* renderbuffers = (GLuint*) malloc(n*sizeof(GLuint));
	if(renderbuffers == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGenRenderbuffers(n, renderbuffers);
		SEND_ARRAY_UINT(n, renderbuffers);
	}
	free(renderbuffers);
}

void loaxGenTextures (void)
{
	LOGD("debug");
	int recv = 1;
	GLsizei n;
	recv &= RECV_SIZEI(&n);

	GLuint* textures = (GLuint*) malloc(n*sizeof(GLuint));
	if(textures == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGenTextures(n, textures);
		SEND_ARRAY_UINT(n, textures);
	}
	free(textures);
}

void loaxGetActiveAttrib (void)
{
	LOGD("debug");
	// bufsize is len of name including '\0'
	// length is len of returned name excluding '\0'
	// length may be NULL
	int recv = 1;
	GLuint program;
	GLuint index;
	GLsizei bufsize;
	GLsizei length;
	GLint size;
	GLenum type;
	recv &= RECV_UINT(&program);
	recv &= RECV_UINT(&index);
	recv &= RECV_SIZEI(&bufsize);

	GLchar* name = (GLchar*) malloc(bufsize*sizeof(GLchar));
	if(name == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGetActiveAttrib(program, index, bufsize, &length, &size, &type, name);
		SEND_INT(length);
		SEND_INT(size);
		SEND_ENUM(type);
		SEND_ARRAY_CHAR(length + 1, name);
	}
	free(name);
}

void loaxGetActiveUniform (void)
{
	LOGD("debug");
	// bufsize is len of name including '\0'
	// length is len of returned name excluding '\0'
	// length may be NULL
	int recv = 1;
	GLuint program;
	GLuint index;
	GLsizei bufsize;
	GLsizei length;
	GLint size;
	GLenum type;
	recv &= RECV_UINT(&program);
	recv &= RECV_UINT(&index);
	recv &= RECV_SIZEI(&bufsize);

	GLchar* name = (GLchar*) malloc(bufsize*sizeof(GLchar));
	if(name == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGetActiveUniform(program, index, bufsize, &length, &size, &type, name);
		SEND_INT(length);
		SEND_INT(size);
		SEND_ENUM(type);
		SEND_ARRAY_CHAR(length + 1, name);
	}
	free(name);
}

void loaxGetAttachedShaders (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	GLsizei maxcount;
	GLsizei count;
	recv &= RECV_UINT(&program);
	recv &= RECV_SIZEI(&maxcount);

	GLuint* shaders = (GLuint*) malloc(maxcount*sizeof(GLuint));
	if(shaders == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGetAttachedShaders(program, maxcount, &count, shaders);
		SEND_SIZEI(count);
		SEND_ARRAY_UINT(count, shaders);
	}
	free(shaders);
}

void loaxGetAttribLocation (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	GLchar* name;
	recv &= RECV_UINT(&program);
	recv &= RECV_STRING(&name);

	if(recv)
	{
		int location = glGetAttribLocation (program, name);
		SEND_INT(location);
	}
	free(name);
}

void loaxGetBooleanv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum pname;
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetBooleanv(pname);
	if(c*sizeof(GLboolean))
	{
		GLboolean* params = (GLboolean*) malloc(c*sizeof(GLboolean));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetBooleanv(pname, params);
			SEND_ARRAY_BOOLEAN(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetBufferParameteriv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetBufferParameteriv(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetBufferParameteriv(target, pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetError (void)
{
	LOGD("debug");
	GLenum error = glGetError();
	SEND_ENUM(error);
}

void loaxGetFloatv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum pname;
	recv &= RECV_ENUM(&pname);

	// same pnames as GetBooleanv
	unsigned int c = loax_serialize_countGetBooleanv(pname);
	if(c*sizeof(GLfloat))
	{
		GLfloat* params = (GLfloat*) malloc(c*sizeof(GLfloat));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetFloatv(pname, params);
			SEND_ARRAY_FLOAT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetFramebufferAttachmentParameteriv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum attachment;
	GLenum pname;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&attachment);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetFramebufferAttachmentParameteriv(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetIntegerv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum pname;
	recv &= RECV_ENUM(&pname);

	// same pnames as GetBooleanv
	unsigned int c = loax_serialize_countGetBooleanv(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetIntegerv(pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetProgramiv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	GLenum pname;
	recv &= RECV_UINT(&program);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetProgramiv(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetProgramiv(program, pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetProgramInfoLog (void)
{
	LOGD("debug");
	// bufsize is len of infolog including '\0'
	// length is len of returned infolog excluding '\0'
	// length may be NULL
	int recv = 1;
	GLuint program;
	GLsizei bufsize;
	GLsizei length;
	recv &= RECV_UINT(&program);
	recv &= RECV_SIZEI(&bufsize);

	GLchar* infolog = (GLchar*) malloc(bufsize*sizeof(GLchar));
	if(infolog == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGetProgramInfoLog(program, bufsize, &length, infolog);
		SEND_SIZEI(length);
		SEND_ARRAY_CHAR(length + 1, infolog);
	}
	free(infolog);
}

void loaxGetRenderbufferParameteriv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetRenderbufferParameteriv(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetRenderbufferParameteriv(target, pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetShaderiv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint shader;
	GLenum pname;
	recv &= RECV_UINT(&shader);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetShaderiv(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetShaderiv(shader, pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetShaderInfoLog (void)
{
	LOGD("debug");
	// bufsize is len of infolog including '\0'
	// length is len of returned infolog excluding '\0'
	// length may be NULL
	int recv = 1;
	GLuint shader;
	GLsizei bufsize;
	GLsizei length;
	recv &= RECV_UINT(&shader);
	recv &= RECV_SIZEI(&bufsize);

	GLchar* infolog = (GLchar*) malloc(bufsize*sizeof(GLchar));
	if(infolog == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGetShaderInfoLog(shader, bufsize, &length, infolog);
		SEND_SIZEI(length);
		SEND_ARRAY_CHAR(length + 1, infolog);
	}
	free(infolog);
}

void loaxGetShaderPrecisionFormat (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum shadertype;
	GLenum precisiontype;
	GLint range[2];
	GLint precision[1];
	recv &= RECV_ENUM(&shadertype);
	recv &= RECV_ENUM(&precisiontype);
	if(recv)
	{
		glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
		SEND_ARRAY_INT(2, range);
		SEND_ARRAY_INT(1, precision);
	}
}

void loaxGetShaderSource (void)
{
	LOGD("debug");
	// bufsize is len of source including '\0'
	// length is len of returned source excluding '\0'
	// length may be NULL
	int recv = 1;
	GLuint shader;
	GLsizei bufsize;
	GLsizei length;
	recv &= RECV_UINT(&shader);
	recv &= RECV_SIZEI(&bufsize);

	GLchar* source = (GLchar*) malloc(bufsize*sizeof(GLchar));
	if(source == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	if(recv)
	{
		glGetShaderSource(shader, bufsize, &length, source);
		SEND_SIZEI(length);
		SEND_ARRAY_CHAR(length + 1, source);
	}
	free(source);
}

void loaxGetString (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum name;
	recv &= RECV_ENUM(&name);
	if(recv)
	{
		const GLubyte* s = glGetString(name);
		SEND_STRING((const GLchar*) s);
	}
}

void loaxGetTexParameterfv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetTexParameter(pname);
	if(c*sizeof(GLfloat))
	{
		GLfloat* params = (GLfloat*) malloc(c*sizeof(GLfloat));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetTexParameterfv(target, pname, params);
			SEND_ARRAY_FLOAT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetTexParameteriv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetTexParameter(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetTexParameteriv(target, pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetUniformfv (void)
{
	LOGD("debug");
	// TODO
}

void loaxGetUniformiv (void)
{
	LOGD("debug");
	// TODO
}

void loaxGetUniformLocation (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	GLchar* name = NULL;
	recv &= RECV_UINT(&program);
	recv &= RECV_STRING(&name);
	if(recv)
	{
		int location = glGetUniformLocation(program, name);
		SEND_INT(location);
	}
	free(name);
}

void loaxGetVertexAttribfv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint index;
	GLenum pname;
	recv &= RECV_UINT(&index);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetVertexAttrib(pname);
	if(c*sizeof(GLfloat))
	{
		GLfloat* params = (GLfloat*) malloc(c*sizeof(GLfloat));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetVertexAttribfv(index, pname, params);
			SEND_ARRAY_FLOAT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetVertexAttribiv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint index;
	GLenum pname;
	recv &= RECV_UINT(&index);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countGetVertexAttrib(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glGetVertexAttribiv(index, pname, params);
			SEND_ARRAY_INT(c, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxGetVertexAttribPointerv (void)
{
	LOGD("debug");
	// TODO
}

void loaxHint (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum mode;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&mode);
	if(recv)
	{
		glHint(target, mode);
	}
}

void loaxIsBuffer (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint buffer;
	recv &= RECV_UINT(&buffer);
	if(recv)
	{
		GLboolean b = glIsBuffer(buffer);
		SEND_BOOLEAN(b);
	}
}

void loaxIsEnabled (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum cap;
	recv &= RECV_ENUM(&cap);
	if(recv)
	{
		GLboolean b = glIsEnabled(cap);
		SEND_BOOLEAN(b);
	}
}

void loaxIsFramebuffer (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint framebuffer;
	recv &= RECV_UINT(&framebuffer);
	if(recv)
	{
		GLboolean b = glIsFramebuffer(framebuffer);
		SEND_BOOLEAN(b);
	}
}

void loaxIsProgram (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	recv &= RECV_UINT(&program);
	if(recv)
	{
		GLboolean b = glIsProgram(program);
		SEND_BOOLEAN(b);
	}
}

void loaxIsRenderbuffer (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint renderbuffer;
	recv &= RECV_UINT(&renderbuffer);
	if(recv)
	{
		GLboolean b = glIsRenderbuffer(renderbuffer);
		SEND_BOOLEAN(b);
	}
}

void loaxIsShader (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint shader;
	recv &= RECV_UINT(&shader);
	if(recv)
	{
		GLboolean b = glIsShader(shader);
		SEND_BOOLEAN(b);
	}
}

void loaxIsTexture (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint texture;
	recv &= RECV_UINT(&texture);
	if(recv)
	{
		GLboolean b = glIsTexture(texture);
		SEND_BOOLEAN(b);
	}
}

void loaxLineWidth (void)
{
	LOGD("debug");
	int recv = 1;
	GLfloat width;
	recv &= RECV_FLOAT(&width);
	if(recv)
	{
		glLineWidth(width);
	}
}

void loaxLinkProgram (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	recv &= RECV_UINT(&program);
	if(recv)
	{
		glLinkProgram(program);
	}
}

void loaxPixelStorei (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum pname;
	GLint param;
	recv &= RECV_ENUM(&pname);
	recv &= RECV_INT(&param);
	if(recv)
	{
		glPixelStorei(pname, param);
	}
}

void loaxPolygonOffset (void)
{
	LOGD("debug");
	int recv = 1;
	GLfloat factor;
	GLfloat units;
	recv &= RECV_FLOAT(&factor);
	recv &= RECV_FLOAT(&units);
	if(recv)
	{
		glPolygonOffset(factor, units);
	}
}

void loaxReadPixels (void)
{
	LOGD("debug");
	int recv = 1;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	recv &= RECV_ENUM(&format);
	recv &= RECV_ENUM(&type);

	unsigned int bpp = loax_serialize_bppReadPixels(format, type);
	unsigned int size = bpp*width*height;
	if(size)
	{
		GLvoid* pixels = (GLvoid*) malloc(size*sizeof(char));
		if(pixels == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		if(recv)
		{
			glReadPixels(x, y, width, height, format, type, pixels);
			SEND_ARRAY_VOID(size, pixels);
		}
		free(pixels);
	}
	else
	{
		LOGE("invalid bpp=%u, width=%u, height=%u", bpp, width, height);
		SHUTDOWN();
	}
}

void loaxReleaseShaderCompiler (void)
{
	LOGD("debug");
	glReleaseShaderCompiler();
}

void loaxRenderbufferStorage (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum internalformat;
	GLsizei width;
	GLsizei height;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&internalformat);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	if(recv)
	{
		glRenderbufferStorage(target, internalformat, width, height);
	}
}

void loaxSampleCoverage (void)
{
	LOGD("debug");
	int recv = 1;
	GLclampf value;
	GLboolean invert;
	recv &= RECV_CLAMPF(&value);
	recv &= RECV_BOOLEAN(&invert);
	if(recv)
	{
		glSampleCoverage(value, invert);
	}
}

void loaxScissor (void)
{
	LOGD("debug");
	int recv = 1;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	if(recv)
	{
		glScissor(x, y, width, height);
	}
}

void loaxShaderBinary (void)
{
	LOGD("debug");
	// TODO
}

void loaxShaderSource (void)
{
	LOGD("debug");
	// TODO count, length and cannot assume strings are terminated?
	int recv = 1;
	GLuint shader;
	GLsizei count;
	GLchar* string = NULL;
	recv &= RECV_UINT(&shader);
	recv &= RECV_SIZEI(&count);
	recv &= RECV_STRING(&string);

	if(recv)
	{
		glShaderSource(shader, count, (const GLchar**) &string, NULL);
	}
	free(string);
}

void loaxStencilFunc (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum func;
	GLint ref;
	GLuint mask;
	recv &= RECV_ENUM(&func);
	recv &= RECV_INT(&ref);
	recv &= RECV_UINT(&mask);
	if(recv)
	{
		glStencilFunc(func, ref, mask);
	}
}

void loaxStencilFuncSeparate (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum face;
	GLenum func;
	GLint ref;
	GLuint mask;
	recv &= RECV_ENUM(&face);
	recv &= RECV_ENUM(&func);
	recv &= RECV_INT(&ref);
	recv &= RECV_UINT(&mask);
	if(recv)
	{
		glStencilFuncSeparate(face, func, ref, mask);
	}
}

void loaxStencilMask (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint mask;
	recv &= RECV_UINT(&mask);
	if(recv)
	{
		glStencilMask(mask);
	}
}

void loaxStencilMaskSeparate (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum face;
	GLuint mask;
	recv &= RECV_ENUM(&face);
	recv &= RECV_UINT(&mask);
	if(recv)
	{
		glStencilMaskSeparate(face, mask);
	}
}

void loaxStencilOp (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum fail;
	GLenum zfail;
	GLenum zpass;
	recv &= RECV_ENUM(&fail);
	recv &= RECV_ENUM(&zfail);
	recv &= RECV_ENUM(&zpass);
	if(recv)
	{
		glStencilOp(fail, zfail, zpass);
	}
}

void loaxStencilOpSeparate (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum face;
	GLenum fail;
	GLenum zfail;
	GLenum zpass;
	recv &= RECV_ENUM(&face);
	recv &= RECV_ENUM(&fail);
	recv &= RECV_ENUM(&zfail);
	recv &= RECV_ENUM(&zpass);
	if(recv)
	{
		glStencilOpSeparate(face, fail, zfail, zpass);
	}
}

void loaxTexImage2D (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLint level;
	GLint internalformat;
	GLsizei width;
	GLsizei height;
	GLint border;
	GLenum format;
	GLenum type;
	recv &= RECV_ENUM(&target);
	recv &= RECV_INT(&level);
	recv &= RECV_INT(&internalformat);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	recv &= RECV_INT(&border);
	recv &= RECV_ENUM(&format);
	recv &= RECV_ENUM(&type);

	unsigned int bpp = loax_serialize_bppTexImage(format, type);
	unsigned int size = bpp*width*height;
	if(size)
	{
		GLvoid* pixels = (GLvoid*) malloc(size*sizeof(char));
		if(pixels == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_VOID(size, pixels);
		if(recv)
		{
			glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
		}
		free(pixels);
	}
	else
	{
		LOGE("invalid bpp=%u, width=%u, height=%u", bpp, width, height);
		SHUTDOWN();
	}
}

void loaxTexParameterf (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	GLfloat param;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);
	recv &= RECV_FLOAT(&param);
	if(recv)
	{
		glTexParameterf(target, pname, param);
	}
}

void loaxTexParameterfv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countTexParameter(pname);
	if(c*sizeof(GLfloat))
	{
		GLfloat* params = (GLfloat*) malloc(c*sizeof(GLfloat));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(c, params);
		if(recv)
		{
			glTexParameterfv(target, pname, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxTexParameteri (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	GLint param;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);
	recv &= RECV_INT(&param);
	if(recv)
	{
		glTexParameteri(target, pname, param);
	}
}

void loaxTexParameteriv (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLenum pname;
	recv &= RECV_ENUM(&target);
	recv &= RECV_ENUM(&pname);

	unsigned int c = loax_serialize_countTexParameter(pname);
	if(c*sizeof(GLint))
	{
		GLint* params = (GLint*) malloc(c*sizeof(GLint));
		if(params == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_INT(c, params);
		if(recv)
		{
			glTexParameteriv(target, pname, params);
		}
		free(params);
	}
	else
	{
		LOGE("invalid c=%u", c);
		SHUTDOWN();
	}
}

void loaxTexSubImage2D (void)
{
	LOGD("debug");
	int recv = 1;
	GLenum target;
	GLint level;
	GLint xoffset;
	GLint yoffset;
	GLsizei width;
	GLsizei height;
	GLenum format;
	GLenum type;
	recv &= RECV_ENUM(&target);
	recv &= RECV_INT(&level);
	recv &= RECV_INT(&xoffset);
	recv &= RECV_INT(&yoffset);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	recv &= RECV_ENUM(&format);
	recv &= RECV_ENUM(&type);

	unsigned int bpp = loax_serialize_bppTexImage(format, type);
	unsigned int size = bpp*width*height;
	if(size)
	{
		GLvoid* pixels = (GLvoid*) malloc(size*sizeof(char));
		if(pixels == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_VOID(size, pixels);
		if(recv)
		{
			glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
		}
		free(pixels);
	}
	else
	{
		LOGE("invalid bpp=%u, width=%u, height=%u", bpp, width, height);
		SHUTDOWN();
	}
}

void loaxUniform1f (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLfloat x;
	recv &= RECV_INT(&location);
	recv &= RECV_FLOAT(&x);
	if(recv)
	{
		glUniform1f(location, x);
	}
}

void loaxUniform1fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(count*sizeof(GLfloat))
	{
		GLfloat* v = (GLfloat*) malloc(count*sizeof(GLfloat));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(count, v);
		if(recv)
		{
			glUniform1fv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniform1i (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLint x;
	recv &= RECV_INT(&location);
	recv &= RECV_INT(&x);
	if(recv)
	{
		glUniform1i(location, x);
	}
}

void loaxUniform1iv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(count*sizeof(GLint))
	{
		GLint* v = (GLint*) malloc(count*sizeof(GLint));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_INT(count, v);
		if(recv)
		{
			glUniform1iv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniform2f (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLfloat x;
	GLfloat y;
	recv &= RECV_INT(&location);
	recv &= RECV_FLOAT(&x);
	recv &= RECV_FLOAT(&y);
	if(recv)
	{
		glUniform2f(location, x, y);
	}
}

void loaxUniform2fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(2*count*sizeof(GLfloat))
	{
		GLfloat* v = (GLfloat*) malloc(2*count*sizeof(GLfloat));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(2*count, v);
		if(recv)
		{
			glUniform2fv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniform2i (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLint x;
	GLint y;
	recv &= RECV_INT(&location);
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	if(recv)
	{
		glUniform2i(location, x, y);
	}
}

void loaxUniform2iv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(2*count*sizeof(GLint))
	{
		GLint* v = (GLint*) malloc(2*count*sizeof(GLint));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_INT(2*count, v);
		if(recv)
		{
			glUniform2iv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniform3f (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	recv &= RECV_INT(&location);
	recv &= RECV_FLOAT(&x);
	recv &= RECV_FLOAT(&y);
	recv &= RECV_FLOAT(&z);
	if(recv)
	{
		glUniform3f(location, x, y, z);
	}
}

void loaxUniform3fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(3*count*sizeof(GLfloat))
	{
		GLfloat* v = (GLfloat*) malloc(3*count*sizeof(GLfloat));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(3*count, v);
		if(recv)
		{
			glUniform3fv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniform3i (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLint x;
	GLint y;
	GLint z;
	recv &= RECV_INT(&location);
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	recv &= RECV_INT(&z);
	if(recv)
	{
		glUniform3i(location, x, y, z);
	}
}

void loaxUniform3iv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(3*count*sizeof(GLint))
	{
		GLint* v = (GLint*) malloc(3*count*sizeof(GLint));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_INT(3*count, v);
		if(recv)
		{
			glUniform3iv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniform4f (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
	recv &= RECV_INT(&location);
	recv &= RECV_FLOAT(&x);
	recv &= RECV_FLOAT(&y);
	recv &= RECV_FLOAT(&z);
	recv &= RECV_FLOAT(&w);
	if(recv)
	{
		glUniform4f(location, x, y, z, w);
	}
}

void loaxUniform4fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(4*count*sizeof(GLfloat))
	{
		GLfloat* v = (GLfloat*) malloc(4*count*sizeof(GLfloat));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(4*count, v);
		if(recv)
		{
			glUniform4fv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniform4i (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLint x;
	GLint y;
	GLint z;
	GLint w;
	recv &= RECV_INT(&location);
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	recv &= RECV_INT(&z);
	recv &= RECV_INT(&w);
	if(recv)
	{
		glUniform4i(location, x, y, z, w);
	}
}

void loaxUniform4iv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);

	if(4*count*sizeof(GLint))
	{
		GLint* v = (GLint*) malloc(4*count*sizeof(GLint));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_INT(4*count, v);
		if(recv)
		{
			glUniform4iv(location, count, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniformMatrix2fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	GLboolean transpose;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);
	recv &= RECV_BOOLEAN(&transpose);

	if(2*2*count*sizeof(GLfloat))
	{
		GLfloat* v = (GLfloat*) malloc(2*2*count*sizeof(GLfloat));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(2*2*count, v);
		if(recv)
		{
			glUniformMatrix2fv(location, count, transpose, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniformMatrix3fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	GLboolean transpose;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);
	recv &= RECV_BOOLEAN(&transpose);

	if(3*3*count*sizeof(GLfloat))
	{
		GLfloat* v = (GLfloat*) malloc(3*3*count*sizeof(GLfloat));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(3*3*count, v);
		if(recv)
		{
			glUniformMatrix3fv(location, count, transpose, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUniformMatrix4fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLint location;
	GLsizei count = 0;
	GLboolean transpose;
	recv &= RECV_INT(&location);
	recv &= RECV_SIZEI(&count);
	recv &= RECV_BOOLEAN(&transpose);

	if(4*4*count*sizeof(GLfloat))
	{
		GLfloat* v = (GLfloat*) malloc(4*4*count*sizeof(GLfloat));
		if(v == NULL)
		{
			LOGE("malloc failed");
			SHUTDOWN();
			return;
		}

		recv &= RECV_ARRAY_FLOAT(4*4*count, v);
		if(recv)
		{
			glUniformMatrix4fv(location, count, transpose, v);
		}
		free(v);
	}
	else
	{
		LOGE("invalid count=%u", count);
		SHUTDOWN();
	}
}

void loaxUseProgram (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	recv &= RECV_UINT(&program);
	if(recv)
	{
		glUseProgram(program);
	}
}

void loaxValidateProgram (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint program;
	recv &= RECV_UINT(&program);
	if(recv)
	{
		glValidateProgram(program);
	}
}

void loaxVertexAttrib1f (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	GLfloat x;
	recv &= RECV_UINT(&indx);
	recv &= RECV_FLOAT(&x);
	if(recv)
	{
		glVertexAttrib1f(indx, x);
	}
}

void loaxVertexAttrib1fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	recv &= RECV_UINT(&indx);

	GLfloat* values = (GLfloat*) malloc(sizeof(GLfloat));
	if(values == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_FLOAT(1, values);
	if(recv)
	{
		glVertexAttrib1fv(indx, values);
	}
	free(values);
}

void loaxVertexAttrib2f (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	GLfloat x;
	GLfloat y;
	recv &= RECV_UINT(&indx);
	recv &= RECV_FLOAT(&x);
	recv &= RECV_FLOAT(&y);
	if(recv)
	{
		glVertexAttrib2f(indx, x, y);
	}
}

void loaxVertexAttrib2fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	recv &= RECV_UINT(&indx);

	GLfloat* values = (GLfloat*) malloc(2*sizeof(GLfloat));
	if(values == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_FLOAT(2, values);
	if(recv)
	{
		glVertexAttrib2fv(indx, values);
	}
	free(values);
}

void loaxVertexAttrib3f (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	recv &= RECV_UINT(&indx);
	recv &= RECV_FLOAT(&x);
	recv &= RECV_FLOAT(&y);
	recv &= RECV_FLOAT(&z);
	if(recv)
	{
		glVertexAttrib3f(indx, x, y, z);
	}
}

void loaxVertexAttrib3fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	recv &= RECV_UINT(&indx);

	GLfloat* values = (GLfloat*) malloc(3*sizeof(GLfloat));
	if(values == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_FLOAT(3, values);
	if(recv)
	{
		glVertexAttrib3fv(indx, values);
	}
	free(values);
}

void loaxVertexAttrib4f (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
	recv &= RECV_UINT(&indx);
	recv &= RECV_FLOAT(&x);
	recv &= RECV_FLOAT(&y);
	recv &= RECV_FLOAT(&z);
	recv &= RECV_FLOAT(&w);
	if(recv)
	{
		glVertexAttrib4f(indx, x, y, z, w);
	}
}

void loaxVertexAttrib4fv (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	recv &= RECV_UINT(&indx);

	GLfloat* values = (GLfloat*) malloc(4*sizeof(GLfloat));
	if(values == NULL)
	{
		LOGE("malloc failed");
		SHUTDOWN();
		return;
	}

	recv &= RECV_ARRAY_FLOAT(4, values);
	if(recv)
	{
		glVertexAttrib4fv(indx, values);
	}
	free(values);
}

void loaxVertexAttribPointer (void)
{
	LOGD("debug");
	int recv = 1;
	GLuint indx;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	GLvoid* ptr;
	recv &= RECV_UINT(&indx);
	recv &= RECV_INT(&size);
	recv &= RECV_ENUM(&type);
	recv &= RECV_BOOLEAN(&normalized);
	recv &= RECV_SIZEI(&stride);
	recv &= RECV_VOIDPTR(&ptr);
	if(recv)
	{
		if(ptr != NULL)
		{
			LOGE("VBOs are required");
			SHUTDOWN();
			return;
		}
		glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
	}
}

void loaxViewport (void)
{
	LOGD("debug");
	int recv = 1;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	recv &= RECV_INT(&x);
	recv &= RECV_INT(&y);
	recv &= RECV_SIZEI(&width);
	recv &= RECV_SIZEI(&height);
	if(recv)
	{
		glViewport(x, y, width, height);
	}
}
