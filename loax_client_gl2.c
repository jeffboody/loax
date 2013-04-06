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
#include "loax_client_gl2.h"
#include "loax_gl2.h"
#include "loax_serialize.h"

#define LOG_TAG "loax"
#include "loax_log.h"

/***********************************************************
* globals                                                  *
***********************************************************/

extern loax_client_t* g_client;

/***********************************************************
* private                                                  *
***********************************************************/

#define SEND_CHAR(d)             loax_serialize_sendchar(g_client->socket_render, d)
#define SEND_ENUM(d)             loax_serialize_senduint(g_client->socket_render, d)
#define SEND_BOOLEAN(d)          loax_serialize_senduchar(g_client->socket_render, d)
#define SEND_BITFIELD(d)         loax_serialize_senduint(g_client->socket_render, d)
#define SEND_BYTE(d)             loax_serialize_sendchar(g_client->socket_render, d)
#define SEND_SHORT(d)            loax_serialize_sendshort(g_client->socket_render, d)
#define SEND_INT(d)              loax_serialize_sendint(g_client->socket_render, d)
#define SEND_SIZEI(d)            loax_serialize_sendint(g_client->socket_render, d)
#define SEND_UBYTE(d)            loax_serialize_senduchar(g_client->socket_render, d)
#define SEND_USHORT(d)           loax_serialize_sendushort(g_client->socket_render, d)
#define SEND_UINT(d)             loax_serialize_senduint(g_client->socket_render, d)
#define SEND_FLOAT(d)            loax_serialize_sendfloat(g_client->socket_render, d)
#define SEND_CLAMPF(d)           loax_serialize_sendfloat(g_client->socket_render, d)
#define SEND_FIXED(d)            loax_serialize_sendint(g_client->socket_render, d)
#define SEND_INTPTR(d)           loax_serialize_sendintptr(g_client->socket_render, d)
#define SEND_VOIDPTR(d)          loax_serialize_sendvoidptr(g_client->socket_render, d)
#define SEND_SIZEIPTR(d)         loax_serialize_senduint(g_client->socket_render, d)
#define SEND_ARRAY_BOOLEAN(s, d) loax_serialize_sendarrayuchar(g_client->socket_render, s, d)
#define SEND_ARRAY_CHAR(s, d)    loax_serialize_sendarraychar(g_client->socket_render, s, d)
#define SEND_ARRAY_VOID(s, d)    loax_serialize_sendarrayvoid(g_client->socket_render, s, d)
#define SEND_ARRAY_INT(s, d)     loax_serialize_sendarrayint(g_client->socket_render, s, d)
#define SEND_ARRAY_UINT(s, d)    loax_serialize_sendarrayuint(g_client->socket_render, s, d)
#define SEND_ARRAY_FLOAT(s, d)   loax_serialize_sendarrayfloat(g_client->socket_render, s, d)
#define SEND_STRING(d)           loax_serialize_sendstring(g_client->socket_render, d)
#define FLUSH()                  net_socket_flush(g_client->socket_render)
#define RECV_CHAR(d)             loax_serialize_recvchar(g_client->socket_render, d)
#define RECV_ENUM(d)             loax_serialize_recvuint(g_client->socket_render, d)
#define RECV_BOOLEAN(d)          loax_serialize_recvuchar(g_client->socket_render, d)
#define RECV_BITFIELD(d)         loax_serialize_recvuint(g_client->socket_render, d)
#define RECV_BYTE(d)             loax_serialize_recvchar(g_client->socket_render, d)
#define RECV_SHORT(d)            loax_serialize_recvshort(g_client->socket_render, d)
#define RECV_INT(d)              loax_serialize_recvint(g_client->socket_render, d)
#define RECV_SIZEI(d)            loax_serialize_recvint(g_client->socket_render, d)
#define RECV_UBYTE(d)            loax_serialize_recvuchar(g_client->socket_render, d)
#define RECV_USHORT(d)           loax_serialize_recvushort(g_client->socket_render, d)
#define RECV_UINT(d)             loax_serialize_recvuint(g_client->socket_render, d)
#define RECV_FLOAT(d)            loax_serialize_recvfloat(g_client->socket_render, d)
#define RECV_CLAMPF(d)           loax_serialize_recvfloat(g_client->socket_render, d)
#define RECV_FIXED(d)            loax_serialize_recvint(g_client->socket_render, d)
#define RECV_INTPTR(d)           loax_serialize_recvintptr(g_client->socket_render, d)
#define RECV_VOIDPTR(d)          loax_serialize_recvvoidptr(g_client->socket_render, d)
#define RECV_SIZEIPTR(d)         loax_serialize_recvuint(g_client->socket_render, d)
#define RECV_ARRAY_BOOLEAN(s, d) loax_serialize_recvarrayuchar(g_client->socket_render, s, d)
#define RECV_ARRAY_CHAR(s, d)    loax_serialize_recvarraychar(g_client->socket_render, s, d)
#define RECV_ARRAY_VOID(s, d)    loax_serialize_recvarrayvoid(g_client->socket_render, s, d)
#define RECV_ARRAY_INT(s, d)     loax_serialize_recvarrayint(g_client->socket_render, s, d)
#define RECV_ARRAY_UINT(s, d)    loax_serialize_recvarrayuint(g_client->socket_render, s, d)
#define RECV_ARRAY_FLOAT(s, d)   loax_serialize_recvarrayfloat(g_client->socket_render, s, d)
#define RECV_STRING(d)           loax_serialize_recvstring(g_client->socket_render, d)
#define SET_ERROR(errno)         loax_client_seterror(g_client, errno)

/***********************************************************
* public OPENGL ES2 API                                    *
***********************************************************/

GL_APICALL void GL_APIENTRY glActiveTexture (GLenum texture)
{
	LOGD("debug");
	SEND_INT(LOAX_ActiveTexture);
	SEND_ENUM(texture);
}

GL_APICALL void GL_APIENTRY glAttachShader (GLuint program, GLuint shader)
{
	LOGD("debug");
	SEND_INT(LOAX_AttachShader);
	SEND_UINT(program);
	SEND_UINT(shader);
}

GL_APICALL void GL_APIENTRY glBindAttribLocation (GLuint program, GLuint index, const GLchar* name)
{
	LOGD("debug");
	SEND_INT(LOAX_BindAttribLocation);
	SEND_UINT(program);
	SEND_UINT(index);
	SEND_STRING(name);
}

GL_APICALL void GL_APIENTRY glBindBuffer (GLenum target, GLuint buffer)
{
	LOGD("debug");
	SEND_INT(LOAX_BindBuffer);
	SEND_ENUM(target);
	SEND_UINT(buffer);
}

GL_APICALL void GL_APIENTRY glBindFramebuffer (GLenum target, GLuint framebuffer)
{
	LOGD("debug");
	SEND_INT(LOAX_BindFramebuffer);
	SEND_ENUM(target);
	SEND_UINT(framebuffer);
}

GL_APICALL void GL_APIENTRY glBindRenderbuffer (GLenum target, GLuint renderbuffer)
{
	LOGD("debug");
	SEND_INT(LOAX_BindRenderbuffer);
	SEND_ENUM(target);
	SEND_UINT(renderbuffer);
}

GL_APICALL void GL_APIENTRY glBindTexture (GLenum target, GLuint texture)
{
	LOGD("debug");
	SEND_INT(LOAX_BindTexture);
	SEND_ENUM(target);
	SEND_UINT(texture);
}

GL_APICALL void GL_APIENTRY glBlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	LOGD("debug");
	SEND_INT(LOAX_BlendColor);
	SEND_CLAMPF(red);
	SEND_CLAMPF(green);
	SEND_CLAMPF(blue);
	SEND_CLAMPF(alpha);
}

GL_APICALL void GL_APIENTRY glBlendEquation ( GLenum mode )
{
	LOGD("debug");
	SEND_INT(LOAX_BlendEquation);
	SEND_ENUM(mode);
}

GL_APICALL void GL_APIENTRY glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha)
{
	LOGD("debug");
	SEND_INT(LOAX_BlendEquationSeparate);
	SEND_ENUM(modeRGB);
	SEND_ENUM(modeAlpha);
}

GL_APICALL void GL_APIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor)
{
	LOGD("debug");
	SEND_INT(LOAX_BlendFunc);
	SEND_ENUM(sfactor);
	SEND_ENUM(dfactor);
}

GL_APICALL void GL_APIENTRY glBlendFuncSeparate (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	LOGD("debug");
	SEND_INT(LOAX_BlendFuncSeparate);
	SEND_ENUM(srcRGB);
	SEND_ENUM(dstRGB);
	SEND_ENUM(srcAlpha);
	SEND_ENUM(dstAlpha);
}

GL_APICALL void GL_APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
	LOGD("debug");
	SEND_INT(LOAX_BufferData);
	SEND_ENUM(target);
	SEND_SIZEIPTR(size);
	SEND_ARRAY_VOID(size, data);
	SEND_ENUM(usage);
}

GL_APICALL void GL_APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
	LOGD("debug");
	SEND_INT(LOAX_BufferSubData);
	SEND_ENUM(target);
	SEND_INTPTR(offset);
	SEND_SIZEIPTR(size);
	SEND_ARRAY_VOID(size, data);
}

GL_APICALL GLenum GL_APIENTRY glCheckFramebufferStatus (GLenum target)
{
	LOGD("debug");
	GLenum status;
	SEND_INT(LOAX_CheckFramebufferStatus);
	SEND_ENUM(target);
	FLUSH();
	return RECV_ENUM(&status) ? status : GL_FRAMEBUFFER_UNSUPPORTED;
}

GL_APICALL void GL_APIENTRY glClear (GLbitfield mask)
{
	LOGD("debug");
	SEND_INT(LOAX_Clear);
	SEND_BITFIELD(mask);
}

GL_APICALL void GL_APIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	LOGD("debug");
	SEND_INT(LOAX_ClearColor);
	SEND_CLAMPF(red);
	SEND_CLAMPF(green);
	SEND_CLAMPF(blue);
	SEND_CLAMPF(alpha);
}

GL_APICALL void GL_APIENTRY glClearDepthf (GLclampf depth)
{
	LOGD("debug");
	SEND_INT(LOAX_ClearDepthf);
	SEND_CLAMPF(depth);
}

GL_APICALL void GL_APIENTRY glClearStencil (GLint s)
{
	LOGD("debug");
	SEND_INT(LOAX_ClearStencil);
	SEND_INT(s);
}

GL_APICALL void GL_APIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	LOGD("debug");
	SEND_INT(LOAX_ColorMask);
	SEND_BOOLEAN(red);
	SEND_BOOLEAN(green);
	SEND_BOOLEAN(blue);
	SEND_BOOLEAN(alpha);
}

GL_APICALL void GL_APIENTRY glCompileShader (GLuint shader)
{
	LOGD("debug");
	SEND_INT(LOAX_CompileShader);
	SEND_UINT(shader);
}

GL_APICALL void GL_APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data)
{
	LOGD("debug");
	// TODO
}

GL_APICALL void GL_APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data)
{
	LOGD("debug");
	// TODO
}

GL_APICALL void GL_APIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	LOGD("debug");
	SEND_INT(LOAX_CopyTexImage2D);
	SEND_ENUM(target);
	SEND_INT(level);
	SEND_ENUM(internalformat);
	SEND_INT(x);
	SEND_INT(y);
	SEND_SIZEI(width);
	SEND_SIZEI(height);
	SEND_INT(border);
}

GL_APICALL void GL_APIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	LOGD("debug");
	SEND_INT(LOAX_CopyTexSubImage2D);
	SEND_ENUM(target);
	SEND_INT(level);
	SEND_INT(xoffset);
	SEND_INT(yoffset);
	SEND_INT(x);
	SEND_INT(y);
	SEND_SIZEI(width);
	SEND_SIZEI(height);
}

GL_APICALL GLuint GL_APIENTRY glCreateProgram (void)
{
	LOGD("debug");
	GLuint p;
	SEND_INT(LOAX_CreateProgram);
	FLUSH();
	return RECV_UINT(&p) ? p : 0;
}

GL_APICALL GLuint GL_APIENTRY glCreateShader (GLenum type)
{
	LOGD("debug");
	GLuint s;
	SEND_INT(LOAX_CreateShader);
	SEND_ENUM(type);
	FLUSH();
	return RECV_UINT(&s) ? s : 0;
}

GL_APICALL void GL_APIENTRY glCullFace (GLenum mode)
{
	LOGD("debug");
	SEND_INT(LOAX_CullFace);
	SEND_ENUM(mode);
}

GL_APICALL void GL_APIENTRY glDeleteBuffers (GLsizei n, const GLuint* buffers)
{
	LOGD("debug");
	SEND_INT(LOAX_DeleteBuffers);
	SEND_SIZEI(n);
	SEND_ARRAY_UINT(n, buffers);
}

GL_APICALL void GL_APIENTRY glDeleteFramebuffers (GLsizei n, const GLuint* framebuffers)
{
	LOGD("debug");
	SEND_INT(LOAX_DeleteFramebuffers);
	SEND_SIZEI(n);
	SEND_ARRAY_UINT(n, framebuffers);
}

GL_APICALL void GL_APIENTRY glDeleteProgram (GLuint program)
{
	LOGD("debug");
	SEND_INT(LOAX_DeleteProgram);
	SEND_UINT(program);
}

GL_APICALL void GL_APIENTRY glDeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers)
{
	LOGD("debug");
	SEND_INT(LOAX_DeleteRenderbuffers);
	SEND_SIZEI(n);
	SEND_ARRAY_UINT(n, renderbuffers);
}

GL_APICALL void GL_APIENTRY glDeleteShader (GLuint shader)
{
	LOGD("debug");
	SEND_INT(LOAX_DeleteShader);
	SEND_UINT(shader);
}

GL_APICALL void GL_APIENTRY glDeleteTextures (GLsizei n, const GLuint* textures)
{
	LOGD("debug");
	SEND_INT(LOAX_DeleteTextures);
	SEND_SIZEI(n);
	SEND_ARRAY_UINT(n, textures);
}

GL_APICALL void GL_APIENTRY glDepthFunc (GLenum func)
{
	LOGD("debug");
	SEND_INT(LOAX_DepthFunc);
	SEND_ENUM(func);
}

GL_APICALL void GL_APIENTRY glDepthMask (GLboolean flag)
{
	LOGD("debug");
	SEND_INT(LOAX_DepthMask);
	SEND_BOOLEAN(flag);
}

GL_APICALL void GL_APIENTRY glDepthRangef (GLclampf zNear, GLclampf zFar)
{
	LOGD("debug");
	SEND_INT(LOAX_DepthRangef);
	SEND_CLAMPF(zNear);
	SEND_CLAMPF(zFar);
}

GL_APICALL void GL_APIENTRY glDetachShader (GLuint program, GLuint shader)
{
	LOGD("debug");
	SEND_INT(LOAX_DetachShader);
	SEND_UINT(program);
	SEND_UINT(shader);
}

GL_APICALL void GL_APIENTRY glDisable (GLenum cap)
{
	LOGD("debug");
	SEND_INT(LOAX_Disable);
	SEND_ENUM(cap);
}

GL_APICALL void GL_APIENTRY glDisableVertexAttribArray (GLuint index)
{
	LOGD("debug");
	SEND_INT(LOAX_DisableVertexAttribArray);
	SEND_UINT(index);
}

GL_APICALL void GL_APIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
	LOGD("debug");
	SEND_INT(LOAX_DrawArrays);
	SEND_ENUM(mode);
	SEND_INT(first);
	SEND_SIZEI(count);
}

GL_APICALL void GL_APIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
	LOGD("debug");
	unsigned int b = loax_serialize_bytesDrawElements(type);
	if(b)
	{
		SEND_INT(LOAX_DrawElements);
		SEND_ENUM(mode);
		SEND_SIZEI(count);
		SEND_ENUM(type);
		SEND_ARRAY_VOID(count*b, indices);
	}
	else
	{
		SET_ERROR(GL_INVALID_VALUE);
	}
}

GL_APICALL void GL_APIENTRY glEnable (GLenum cap)
{
	LOGD("debug");
	SEND_INT(LOAX_Enable);
	SEND_ENUM(cap);
}

GL_APICALL void GL_APIENTRY glEnableVertexAttribArray (GLuint index)
{
	LOGD("debug");
	SEND_INT(LOAX_EnableVertexAttribArray);
	SEND_UINT(index);
}

GL_APICALL void GL_APIENTRY glFinish (void)
{
	LOGD("debug");
	SEND_INT(LOAX_Finish);
}

GL_APICALL void GL_APIENTRY glFlush (void)
{
	LOGD("debug");
	SEND_INT(LOAX_Flush);
}

GL_APICALL void GL_APIENTRY glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	LOGD("debug");
	SEND_INT(LOAX_FramebufferRenderbuffer);
	SEND_ENUM(target);
	SEND_ENUM(attachment);
	SEND_ENUM(renderbuffertarget);
	SEND_UINT(renderbuffer);
}

GL_APICALL void GL_APIENTRY glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	LOGD("debug");
	SEND_INT(LOAX_FramebufferTexture2D);
	SEND_ENUM(target);
	SEND_ENUM(attachment);
	SEND_ENUM(textarget);
	SEND_UINT(texture);
	SEND_INT(level);
}

GL_APICALL void GL_APIENTRY glFrontFace (GLenum mode)
{
	LOGD("debug");
	SEND_INT(LOAX_FrontFace);
	SEND_ENUM(mode);
}

GL_APICALL void GL_APIENTRY glGenBuffers (GLsizei n, GLuint* buffers)
{
	LOGD("debug");
	SEND_INT(LOAX_GenBuffers);
	SEND_SIZEI(n);
	FLUSH();
	RECV_ARRAY_UINT(n, buffers);
}

GL_APICALL void GL_APIENTRY glGenerateMipmap (GLenum target)
{
	LOGD("debug");
	SEND_INT(LOAX_GenerateMipmap);
	SEND_ENUM(target);
}

GL_APICALL void GL_APIENTRY glGenFramebuffers (GLsizei n, GLuint* framebuffers)
{
	LOGD("debug");
	SEND_INT(LOAX_GenFramebuffers);
	SEND_SIZEI(n);
	FLUSH();
	RECV_ARRAY_UINT(n, framebuffers);
}

GL_APICALL void GL_APIENTRY glGenRenderbuffers (GLsizei n, GLuint* renderbuffers)
{
	LOGD("debug");
	SEND_INT(LOAX_GenRenderbuffers);
	SEND_SIZEI(n);
	FLUSH();
	RECV_ARRAY_UINT(n, renderbuffers);
}

GL_APICALL void GL_APIENTRY glGenTextures (GLsizei n, GLuint* textures)
{
	LOGD("debug");
	SEND_INT(LOAX_GenTextures);
	SEND_SIZEI(n);
	FLUSH();
	RECV_ARRAY_UINT(n, textures);
}

GL_APICALL void GL_APIENTRY glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
	LOGD("debug");
	// bufsize is len of name including '\0'
	// length is len of returned name excluding '\0'
	// length may be NULL
	SEND_INT(LOAX_GetActiveAttrib);
	SEND_UINT(program);
	SEND_UINT(index);
	SEND_SIZEI(bufsize);
	GLsizei tmp_length = 0;
	FLUSH();
	RECV_SIZEI(&tmp_length);
	if(tmp_length + 1 > bufsize)
	{
		tmp_length = bufsize - 1;
	}
	if(length)
	{
		*length = tmp_length;
	}
	RECV_INT(size);
	RECV_ENUM(type);
	RECV_ARRAY_CHAR(tmp_length + 1, name);
	name[tmp_length] = '\0';
}

GL_APICALL void GL_APIENTRY glGetActiveUniform (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name)
{
	LOGD("debug");
	// bufsize is len of name including '\0'
	// length is len of returned name excluding '\0'
	// length may be NULL
	SEND_INT(LOAX_GetActiveUniform);
	SEND_UINT(program);
	SEND_UINT(index);
	SEND_SIZEI(bufsize);
	GLsizei tmp_length = 0;
	FLUSH();
	RECV_SIZEI(&tmp_length);
	if(tmp_length + 1 > bufsize)
	{
		tmp_length = bufsize - 1;
	}
	if(length)
	{
		*length = tmp_length;
	}
	RECV_INT(size);
	RECV_ENUM(type);
	RECV_ARRAY_CHAR(tmp_length + 1, name);
	name[tmp_length] = '\0';
}

GL_APICALL void GL_APIENTRY glGetAttachedShaders (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
	LOGD("debug");
	// count may be NULL
	SEND_INT(LOAX_GetAttachedShaders);
	SEND_UINT(program);
	SEND_SIZEI(maxcount);
	GLsizei tmp_count = 0;
	FLUSH();
	RECV_SIZEI(&tmp_count);
	if(tmp_count > maxcount)
	{
		tmp_count = maxcount;
	}
	if(count)
	{
		*count = tmp_count;
	}
	RECV_ARRAY_UINT(tmp_count, shaders);
}

GL_APICALL int GL_APIENTRY glGetAttribLocation (GLuint program, const GLchar* name)
{
	LOGD("debug");
	SEND_INT(LOAX_GetAttribLocation);
	SEND_UINT(program);
	SEND_STRING(name);
	int a = -1;
	FLUSH();
	return RECV_INT(&a) ? a : -1;
}

GL_APICALL void GL_APIENTRY glGetBooleanv (GLenum pname, GLboolean* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetBooleanv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetBooleanv);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_BOOLEAN(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetBufferParameteriv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetBufferParameteriv);
		SEND_ENUM(target);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL GLenum GL_APIENTRY glGetError (void)
{
	assert(g_client);
	LOGD("debug");

	SEND_INT(LOAX_GetError);
	GLenum error = GL_OUT_OF_MEMORY;
	FLUSH();
	if((RECV_ENUM(&error) == 0) || (error == GL_NO_ERROR))
	{
		error = g_client->errno;
	}
	g_client->errno = GL_NO_ERROR;
	return error;
}

GL_APICALL void GL_APIENTRY glGetFloatv (GLenum pname, GLfloat* params)
{
	LOGD("debug");
	// same pnames as GetBooleanv
	unsigned int c = loax_serialize_countGetBooleanv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetFloatv);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_FLOAT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetFramebufferAttachmentParameteriv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetFramebufferAttachmentParameteriv);
		SEND_ENUM(target);
		SEND_ENUM(attachment);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetIntegerv (GLenum pname, GLint* params)
{
	LOGD("debug");
	// same pnames as GetBooleanv
	unsigned int c = loax_serialize_countGetBooleanv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetIntegerv);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetProgramiv (GLuint program, GLenum pname, GLint* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetProgramiv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetProgramiv);
		SEND_UINT(program);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetProgramInfoLog (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog)
{
	LOGD("debug");
	// bufsize is len of infolog including '\0'
	// length is len of returned infolog excluding '\0'
	// length may be NULL
	SEND_INT(LOAX_GetProgramInfoLog);
	SEND_UINT(program);
	SEND_SIZEI(bufsize);
	GLsizei tmp_length = 0;
	FLUSH();
	RECV_SIZEI(&tmp_length);
	if(tmp_length + 1 > bufsize)
	{
		tmp_length = bufsize - 1;
	}
	if(length)
	{
		*length = tmp_length;
	}
	RECV_ARRAY_CHAR(tmp_length + 1, infolog);
	infolog[tmp_length] = '\0';
}

GL_APICALL void GL_APIENTRY glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetRenderbufferParameteriv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetRenderbufferParameteriv);
		SEND_ENUM(target);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetShaderiv (GLuint shader, GLenum pname, GLint* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetShaderiv(pname);
	if(c)
	{
		SEND_INT(LOAX_GetShaderiv);
		SEND_UINT(shader);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetShaderInfoLog (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog)
{
	LOGD("debug");
	// bufsize is len of name including '\0'
	// length is len of returned name excluding '\0'
	// length may be NULL
	SEND_INT(LOAX_GetShaderInfoLog);
	SEND_UINT(shader);
	SEND_SIZEI(bufsize);
	GLsizei tmp_length = 0;
	FLUSH();
	RECV_SIZEI(&tmp_length);
	if(tmp_length + 1 > bufsize)
	{
		tmp_length = bufsize - 1;
	}
	if(length)
	{
		*length = tmp_length;
	}
	RECV_ARRAY_CHAR(tmp_length + 1, infolog);
	infolog[tmp_length] = '\0';
}

GL_APICALL void GL_APIENTRY glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
	LOGD("debug");
	SEND_INT(LOAX_GetShaderPrecisionFormat);
	SEND_ENUM(shadertype);
	SEND_ENUM(precisiontype);
	FLUSH();
	RECV_ARRAY_INT(2, range);
	RECV_ARRAY_INT(1, precision);
}

GL_APICALL void GL_APIENTRY glGetShaderSource (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source)
{
	LOGD("debug");
	// bufsize is len of name including '\0'
	// length is len of returned name excluding '\0'
	// length may be NULL
	SEND_INT(LOAX_GetShaderSource);
	SEND_UINT(shader);
	SEND_SIZEI(bufsize);
	GLsizei tmp_length = 0;
	FLUSH();
	RECV_SIZEI(&tmp_length);
	if(tmp_length + 1 > bufsize)
	{
		tmp_length = bufsize - 1;
	}
	if(length)
	{
		*length = tmp_length;
	}
	RECV_ARRAY_CHAR(tmp_length + 1, source);
	source[tmp_length] = '\0';
}

GL_APICALL const GLubyte* GL_APIENTRY glGetString (GLenum name)
{
	LOGD("debug");
// TODO
//	if(g_client->get_string == NULL)
//	{
//		SEND_INT(LOAX_GetString);
//		SEND_ENUM(name);
//		FLUSH();
//		RECV_STRING(&g_client->get_string);
//	}
//	return (const GLubyte*) g_client->get_string;
	return (const GLubyte*) "loax";
}

GL_APICALL void GL_APIENTRY glGetTexParameterfv (GLenum target, GLenum pname, GLfloat* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetTexParameter(pname);
	if(c)
	{
		SEND_INT(LOAX_GetTexParameterfv);
		SEND_ENUM(target);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_FLOAT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetTexParameteriv (GLenum target, GLenum pname, GLint* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetTexParameter(pname);
	if(c)
	{
		SEND_INT(LOAX_GetTexParameteriv);
		SEND_ENUM(target);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetUniformfv (GLuint program, GLint location, GLfloat* params)
{
	LOGD("debug");
	// TODO
}

GL_APICALL void GL_APIENTRY glGetUniformiv (GLuint program, GLint location, GLint* params)
{
	LOGD("debug");
	// TODO
}

GL_APICALL int GL_APIENTRY glGetUniformLocation (GLuint program, const GLchar* name)
{
	LOGD("debug");
	SEND_INT(LOAX_GetUniformLocation);
	SEND_UINT(program);
	SEND_STRING(name);
	int location = -1;
	FLUSH();
	return RECV_INT(&location) ? location : -1;
}

GL_APICALL void GL_APIENTRY glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetVertexAttrib(pname);
	if(c)
	{
		SEND_INT(LOAX_GetVertexAttribfv);
		SEND_UINT(index);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_FLOAT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetVertexAttribiv (GLuint index, GLenum pname, GLint* params)
{
	LOGD("debug");
	unsigned int c = loax_serialize_countGetVertexAttrib(pname);
	if(c)
	{
		SEND_INT(LOAX_GetVertexAttribiv);
		SEND_UINT(index);
		SEND_ENUM(pname);
		FLUSH();
		RECV_ARRAY_INT(c, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glGetVertexAttribPointerv (GLuint index, GLenum pname, GLvoid** pointer)
{
	LOGD("debug");
	// TODO
}

GL_APICALL void GL_APIENTRY glHint (GLenum target, GLenum mode)
{
	LOGD("debug");
	SEND_INT(LOAX_Hint);
	SEND_ENUM(target);
	SEND_ENUM(mode);
}

GL_APICALL GLboolean GL_APIENTRY glIsBuffer (GLuint buffer)
{
	LOGD("debug");
	SEND_INT(LOAX_IsBuffer);
	SEND_UINT(buffer);
	GLboolean b = GL_FALSE;
	FLUSH();
	return RECV_BOOLEAN(&b) ? b : GL_FALSE;
}

GL_APICALL GLboolean GL_APIENTRY glIsEnabled (GLenum cap)
{
	LOGD("debug");
	SEND_INT(LOAX_IsEnabled);
	SEND_ENUM(cap);
	GLboolean b = GL_FALSE;
	FLUSH();
	return RECV_BOOLEAN(&b) ? b : GL_FALSE;
}

GL_APICALL GLboolean GL_APIENTRY glIsFramebuffer (GLuint framebuffer)
{
	LOGD("debug");
	SEND_INT(LOAX_IsFramebuffer);
	SEND_UINT(framebuffer);
	GLboolean b = GL_FALSE;
	FLUSH();
	return RECV_BOOLEAN(&b) ? b : GL_FALSE;
}

GL_APICALL GLboolean GL_APIENTRY glIsProgram (GLuint program)
{
	LOGD("debug");
	SEND_INT(LOAX_IsProgram);
	SEND_UINT(program);
	GLboolean b = GL_FALSE;
	FLUSH();
	return RECV_BOOLEAN(&b) ? b : GL_FALSE;
}

GL_APICALL GLboolean GL_APIENTRY glIsRenderbuffer (GLuint renderbuffer)
{
	LOGD("debug");
	SEND_INT(LOAX_IsRenderbuffer);
	SEND_UINT(renderbuffer);
	GLboolean b = GL_FALSE;
	FLUSH();
	return RECV_BOOLEAN(&b) ? b : GL_FALSE;
}

GL_APICALL GLboolean GL_APIENTRY glIsShader (GLuint shader)
{
	LOGD("debug");
	SEND_INT(LOAX_IsShader);
	SEND_UINT(shader);
	GLboolean b = GL_FALSE;
	FLUSH();
	return RECV_BOOLEAN(&b) ? b : GL_FALSE;
}

GL_APICALL GLboolean GL_APIENTRY glIsTexture (GLuint texture)
{
	LOGD("debug");
	SEND_INT(LOAX_IsTexture);
	SEND_UINT(texture);
	GLboolean b = GL_FALSE;
	FLUSH();
	return RECV_BOOLEAN(&b) ? b : GL_FALSE;
}

GL_APICALL void GL_APIENTRY glLineWidth (GLfloat width)
{
	LOGD("debug");
	SEND_INT(LOAX_LineWidth);
	SEND_FLOAT(width);
}

GL_APICALL void GL_APIENTRY glLinkProgram (GLuint program)
{
	LOGD("debug");
	SEND_INT(LOAX_LinkProgram);
	SEND_UINT(program);
}

GL_APICALL void GL_APIENTRY glPixelStorei (GLenum pname, GLint param)
{
	LOGD("debug");
	SEND_INT(LOAX_PixelStorei);
	SEND_ENUM(pname);
	SEND_INT(param);
}

GL_APICALL void GL_APIENTRY glPolygonOffset (GLfloat factor, GLfloat units)
{
	LOGD("debug");
	SEND_INT(LOAX_PolygonOffset);
	SEND_FLOAT(factor);
	SEND_FLOAT(units);
}

GL_APICALL void GL_APIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels)
{
	LOGD("debug");
	unsigned int bpp = loax_serialize_bppReadPixels(format, type);
	if(bpp)
	{
		SEND_INT(LOAX_ReadPixels);
		SEND_INT(x);
		SEND_INT(y);
		SEND_SIZEI(width);
		SEND_SIZEI(height);
		SEND_ENUM(format);
		SEND_ENUM(type);
		FLUSH();
		RECV_ARRAY_VOID(bpp*width*height, pixels);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glReleaseShaderCompiler (void)
{
	LOGD("debug");
	SEND_INT(LOAX_ReleaseShaderCompiler);
}

GL_APICALL void GL_APIENTRY glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
	LOGD("debug");
	SEND_INT(LOAX_RenderbufferStorage);
	SEND_ENUM(target);
	SEND_ENUM(internalformat);
	SEND_SIZEI(width);
	SEND_SIZEI(height);
}

GL_APICALL void GL_APIENTRY glSampleCoverage (GLclampf value, GLboolean invert)
{
	LOGD("debug");
	SEND_INT(LOAX_SampleCoverage);
	SEND_CLAMPF(value);
	SEND_BOOLEAN(invert);
}

GL_APICALL void GL_APIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
	LOGD("debug");
	SEND_INT(LOAX_Scissor);
	SEND_INT(x);
	SEND_INT(y);
	SEND_SIZEI(width);
	SEND_SIZEI(height);
}

GL_APICALL void GL_APIENTRY glShaderBinary (GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length)
{
	LOGD("debug");
	// TODO
}

GL_APICALL void GL_APIENTRY glShaderSource (GLuint shader, GLsizei count, const GLchar** string, const GLint* length)
{
	assert(g_client);
	LOGD("debug");

	// TODO count, length and cannot assume strings are terminated?
	if(count != 1)
	{
		net_socket_shutdown(g_client->socket_render, NET_SOCKET_SHUT_RDWR);
		return;
	}

	SEND_INT(LOAX_ShaderSource);
	SEND_UINT(shader);
	SEND_SIZEI(count);
	SEND_STRING(string[0]);
}

GL_APICALL void GL_APIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask)
{
	LOGD("debug");
	SEND_INT(LOAX_StencilFunc);
	SEND_ENUM(func);
	SEND_INT(ref);
	SEND_UINT(mask);
}

GL_APICALL void GL_APIENTRY glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask)
{
	LOGD("debug");
	SEND_INT(LOAX_StencilFuncSeparate);
	SEND_ENUM(face);
	SEND_ENUM(func);
	SEND_INT(ref);
	SEND_UINT(mask);
}

GL_APICALL void GL_APIENTRY glStencilMask (GLuint mask)
{
	LOGD("debug");
	SEND_INT(LOAX_StencilMask);
	SEND_UINT(mask);
}

GL_APICALL void GL_APIENTRY glStencilMaskSeparate (GLenum face, GLuint mask)
{
	LOGD("debug");
	SEND_INT(LOAX_StencilMaskSeparate);
	SEND_ENUM(face);
	SEND_UINT(mask);
}

GL_APICALL void GL_APIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass)
{
	LOGD("debug");
	SEND_INT(LOAX_StencilOp);
	SEND_ENUM(fail);
	SEND_ENUM(zfail);
	SEND_ENUM(zpass);
}

GL_APICALL void GL_APIENTRY glStencilOpSeparate (GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
	LOGD("debug");
	SEND_INT(LOAX_StencilOpSeparate);
	SEND_ENUM(face);
	SEND_ENUM(fail);
	SEND_ENUM(zfail);
	SEND_ENUM(zpass);
}

GL_APICALL void GL_APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels)
{
	LOGD("debug");
	unsigned int bpp = loax_serialize_bppTexImage(format, type);
	if(bpp)
	{
		SEND_INT(LOAX_TexImage2D);
		SEND_ENUM(target);
		SEND_INT(level);
		SEND_INT(internalformat);
		SEND_SIZEI(width);
		SEND_SIZEI(height);
		SEND_INT(border);
		SEND_ENUM(format);
		SEND_ENUM(type);
		SEND_ARRAY_VOID(bpp*width*height, pixels);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
	LOGD("debug");
	SEND_INT(LOAX_TexParameterf);
	SEND_ENUM(target);
	SEND_ENUM(pname);
	SEND_FLOAT(param);
}

GL_APICALL void GL_APIENTRY glTexParameterfv (GLenum target, GLenum pname, const GLfloat* params)
{
	LOGD("debug");
	int count = loax_serialize_countTexParameter(pname);
	if(count)
	{
		SEND_INT(LOAX_TexParameterfv);
		SEND_ENUM(target);
		SEND_ENUM(pname);
		SEND_ARRAY_FLOAT(count, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glTexParameteri (GLenum target, GLenum pname, GLint param)
{
	LOGD("debug");
	SEND_INT(LOAX_TexParameteri);
	SEND_ENUM(target);
	SEND_ENUM(pname);
	SEND_INT(param);
}

GL_APICALL void GL_APIENTRY glTexParameteriv (GLenum target, GLenum pname, const GLint* params)
{
	LOGD("debug");
	int count = loax_serialize_countTexParameter(pname);
	if(count)
	{
		SEND_INT(LOAX_TexParameteriv);
		SEND_ENUM(target);
		SEND_ENUM(pname);
		SEND_ARRAY_INT(count, params);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels)
{
	LOGD("debug");
	unsigned int bpp = loax_serialize_bppTexImage(format, type);
	if(bpp)
	{
		SEND_INT(LOAX_TexSubImage2D);
		SEND_ENUM(target);
		SEND_INT(level);
		SEND_INT(xoffset);
		SEND_INT(yoffset);
		SEND_SIZEI(width);
		SEND_SIZEI(height);
		SEND_ENUM(format);
		SEND_ENUM(type);
		SEND_ARRAY_VOID(bpp*width*height, pixels);
	}
	else
	{
		SET_ERROR(GL_INVALID_ENUM);
	}
}

GL_APICALL void GL_APIENTRY glUniform1f (GLint location, GLfloat x)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform1f);
	SEND_INT(location);
	SEND_FLOAT(x);
}

GL_APICALL void GL_APIENTRY glUniform1fv (GLint location, GLsizei count, const GLfloat* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform1fv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_FLOAT(count, v);
}

GL_APICALL void GL_APIENTRY glUniform1i (GLint location, GLint x)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform1i);
	SEND_INT(location);
	SEND_INT(x);
}

GL_APICALL void GL_APIENTRY glUniform1iv (GLint location, GLsizei count, const GLint* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform1iv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_INT(count, v);
}

GL_APICALL void GL_APIENTRY glUniform2f (GLint location, GLfloat x, GLfloat y)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform2f);
	SEND_INT(location);
	SEND_FLOAT(x);
	SEND_FLOAT(y);
}

GL_APICALL void GL_APIENTRY glUniform2fv (GLint location, GLsizei count, const GLfloat* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform2fv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_FLOAT(2*count, v);
}

GL_APICALL void GL_APIENTRY glUniform2i (GLint location, GLint x, GLint y)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform2i);
	SEND_INT(location);
	SEND_INT(x);
	SEND_INT(y);
}

GL_APICALL void GL_APIENTRY glUniform2iv (GLint location, GLsizei count, const GLint* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform2iv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_INT(2*count, v);
}

GL_APICALL void GL_APIENTRY glUniform3f (GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform3f);
	SEND_INT(location);
	SEND_FLOAT(x);
	SEND_FLOAT(y);
	SEND_FLOAT(z);
}

GL_APICALL void GL_APIENTRY glUniform3fv (GLint location, GLsizei count, const GLfloat* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform3fv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_FLOAT(3*count, v);
}

GL_APICALL void GL_APIENTRY glUniform3i (GLint location, GLint x, GLint y, GLint z)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform3i);
	SEND_INT(x);
	SEND_INT(y);
	SEND_INT(z);
}

GL_APICALL void GL_APIENTRY glUniform3iv (GLint location, GLsizei count, const GLint* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform3iv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_INT(3*count, v);
}

GL_APICALL void GL_APIENTRY glUniform4f (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform4f);
	SEND_FLOAT(x);
	SEND_FLOAT(y);
	SEND_FLOAT(z);
	SEND_FLOAT(w);
}

GL_APICALL void GL_APIENTRY glUniform4fv (GLint location, GLsizei count, const GLfloat* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform4fv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_FLOAT(4*count, v);
}

GL_APICALL void GL_APIENTRY glUniform4i (GLint location, GLint x, GLint y, GLint z, GLint w)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform4i);
	SEND_INT(x);
	SEND_INT(y);
	SEND_INT(z);
	SEND_INT(w);
}

GL_APICALL void GL_APIENTRY glUniform4iv (GLint location, GLsizei count, const GLint* v)
{
	LOGD("debug");
	SEND_INT(LOAX_Uniform4iv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_ARRAY_INT(4*count, v);
}

GL_APICALL void GL_APIENTRY glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	LOGD("debug");
	SEND_INT(LOAX_UniformMatrix2fv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_BOOLEAN(transpose);
	SEND_ARRAY_FLOAT(2*2*count, value);
}

GL_APICALL void GL_APIENTRY glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	LOGD("debug");
	SEND_INT(LOAX_UniformMatrix3fv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_BOOLEAN(transpose);
	SEND_ARRAY_FLOAT(3*3*count, value);
}

GL_APICALL void GL_APIENTRY glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	LOGD("debug");
	SEND_INT(LOAX_UniformMatrix4fv);
	SEND_INT(location);
	SEND_SIZEI(count);
	SEND_BOOLEAN(transpose);
	SEND_ARRAY_FLOAT(4*4*count, value);
}

GL_APICALL void GL_APIENTRY glUseProgram (GLuint program)
{
	LOGD("debug");
	SEND_INT(LOAX_UseProgram);
	SEND_UINT(program);
}

GL_APICALL void GL_APIENTRY glValidateProgram (GLuint program)
{
	LOGD("debug");
	SEND_INT(LOAX_ValidateProgram);
	SEND_UINT(program);
}

GL_APICALL void GL_APIENTRY glVertexAttrib1f (GLuint indx, GLfloat x)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib1f);
	SEND_UINT(indx);
	SEND_FLOAT(x);
}

GL_APICALL void GL_APIENTRY glVertexAttrib1fv (GLuint indx, const GLfloat* values)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib1fv);
	SEND_UINT(indx);
	SEND_ARRAY_FLOAT(1, values);
}

GL_APICALL void GL_APIENTRY glVertexAttrib2f (GLuint indx, GLfloat x, GLfloat y)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib2f);
	SEND_UINT(indx);
	SEND_FLOAT(x);
	SEND_FLOAT(y);
}

GL_APICALL void GL_APIENTRY glVertexAttrib2fv (GLuint indx, const GLfloat* values)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib2fv);
	SEND_UINT(indx);
	SEND_ARRAY_FLOAT(2, values);
}

GL_APICALL void GL_APIENTRY glVertexAttrib3f (GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib3f);
	SEND_UINT(indx);
	SEND_FLOAT(x);
	SEND_FLOAT(y);
	SEND_FLOAT(z);
}

GL_APICALL void GL_APIENTRY glVertexAttrib3fv (GLuint indx, const GLfloat* values)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib3fv);
	SEND_UINT(indx);
	SEND_ARRAY_FLOAT(3, values);
}

GL_APICALL void GL_APIENTRY glVertexAttrib4f (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib4f);
	SEND_UINT(indx);
	SEND_FLOAT(x);
	SEND_FLOAT(y);
	SEND_FLOAT(z);
	SEND_FLOAT(w);
}

GL_APICALL void GL_APIENTRY glVertexAttrib4fv (GLuint indx, const GLfloat* values)
{
	LOGD("debug");
	SEND_INT(LOAX_VertexAttrib4fv);
	SEND_UINT(indx);
	SEND_ARRAY_FLOAT(4, values);
}

GL_APICALL void GL_APIENTRY glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr)
{
	LOGD("debug");
	if(ptr == NULL)
	{
		SEND_INT(LOAX_VertexAttribPointer);
		SEND_UINT(indx);
		SEND_INT(size);
		SEND_ENUM(type);
		SEND_BOOLEAN(normalized);
		SEND_SIZEI(stride);
		SEND_VOIDPTR(ptr);
	}
	else
	{
		// TODO - glVertexAttribPointer
		SET_ERROR(GL_INVALID_VALUE);
	}
}

GL_APICALL void GL_APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
	LOGD("debug");
	SEND_INT(LOAX_Viewport);
	SEND_INT(x);
	SEND_INT(y);
	SEND_SIZEI(width);
	SEND_SIZEI(height);
}
