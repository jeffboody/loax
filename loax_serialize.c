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

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "gl2.h"
#include "loax_gl2.h"
#include "loax_serialize.h"

#define LOG_TAG "loax"
#include "loax_log.h"

int loax_serialize_sendchar(net_socket_t* s, char data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(char));
}

int loax_serialize_senduchar(net_socket_t* s, unsigned char data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(unsigned char));
}

int loax_serialize_sendshort(net_socket_t* s, short data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(short));
}

int loax_serialize_sendushort(net_socket_t* s, unsigned short data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(unsigned short));
}

int loax_serialize_sendint(net_socket_t* s, int data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(int));
}

int loax_serialize_senduint(net_socket_t* s, unsigned int data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(unsigned int));
}

int loax_serialize_sendfloat(net_socket_t* s, float data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(float));
}

int loax_serialize_sendintptr(net_socket_t* s, const int* data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(int*));
}

int loax_serialize_sendvoidptr(net_socket_t* s, const void* data)
{
	return net_socket_sendall(s, (const void*) &data, sizeof(void*));
}

int loax_serialize_sendarraychar(net_socket_t* s, unsigned int size, const char* data)
{
	return net_socket_sendall(s, (const void*) data, size*sizeof(char));
}

int loax_serialize_sendarrayuchar(net_socket_t* s, unsigned int size, const unsigned char* data)
{
	return net_socket_sendall(s, (const void*) data, size*sizeof(unsigned char));
}

int loax_serialize_sendarrayvoid(net_socket_t* s, unsigned int size, const void* data)
{
	return net_socket_sendall(s, (const void*) data, size*sizeof(char));
}

int loax_serialize_sendarrayint(net_socket_t* s, unsigned int size, const int* data)
{
	return net_socket_sendall(s, (const void*) data, size*sizeof(int));
}

int loax_serialize_sendarrayuint(net_socket_t* s, unsigned int size, const unsigned int* data)
{
	return net_socket_sendall(s, (const void*) data, size*sizeof(unsigned int));
}

int loax_serialize_sendarrayfloat(net_socket_t* s, unsigned int size, const float* data)
{
	return net_socket_sendall(s, (const void*) data, size*sizeof(float));
}

int loax_serialize_sendstring(net_socket_t* s, const char* data)
{
	unsigned int len = strlen(data) + 1;
	loax_serialize_senduint(s, len);
	return net_socket_sendall(s, (const void*) data, len*sizeof(char));
}

int loax_serialize_recvchar(net_socket_t* s, char* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(char), &recvd);
}

int loax_serialize_recvuchar(net_socket_t* s, unsigned char* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(unsigned char), &recvd);
}

int loax_serialize_recvshort(net_socket_t* s, short* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(short), &recvd);
}

int loax_serialize_recvushort(net_socket_t* s, unsigned short* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(unsigned short), &recvd);
}

int loax_serialize_recvint(net_socket_t* s, int* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(int), &recvd);
}

int loax_serialize_recvuint(net_socket_t* s, unsigned int* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(unsigned int), &recvd);
}

int loax_serialize_recvfloat(net_socket_t* s, float* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(float), &recvd);
}

int loax_serialize_recvintptr(net_socket_t* s, int** data)
{
	// TODO - this one is weird
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(int*), &recvd);
}

int loax_serialize_recvvoidptr(net_socket_t* s, void** data)
{
	// TODO - this one is weird
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, sizeof(void*), &recvd);
}

int loax_serialize_recvarraychar(net_socket_t* s, unsigned int size, char* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, size*sizeof(char), &recvd);
}

int loax_serialize_recvarrayuchar(net_socket_t* s, unsigned int size, unsigned char* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, size*sizeof(unsigned char), &recvd);
}

int loax_serialize_recvarrayvoid(net_socket_t* s, unsigned int size, void* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, size*sizeof(char), &recvd);
}

int loax_serialize_recvarrayint(net_socket_t* s, unsigned int size, int* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, size*sizeof(int), &recvd);
}

int loax_serialize_recvarrayuint(net_socket_t* s, unsigned int size, unsigned int* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, size*sizeof(unsigned int), &recvd);
}

int loax_serialize_recvarrayfloat(net_socket_t* s, unsigned int size, float* data)
{
	int recvd = 0;
	return net_socket_recvall(s, (void*) data, size*sizeof(float), &recvd);
}

int loax_serialize_recvstring(net_socket_t* s, char** data)
{
	// TODO improve loax_serialize_recvstring
	int recvd = 0;
	unsigned int size;
	if((loax_serialize_recvuint(s, &size) == 0) || (size == 0))
	{
		LOGE("size=%u", size);
        *data = NULL;
		return 0;
	}

	*data = (char*) malloc(size*sizeof(char));
	if(*data == NULL)
	{
		LOGE("malloc failed");
		return 0;
	}

	return net_socket_recvall(s, (void*) *data, size*sizeof(char), &recvd);
}

unsigned int loax_serialize_bytesDrawElements(unsigned int type)
{
	if(type == GL_UNSIGNED_BYTE)       { return 1; }
	else if(type == GL_UNSIGNED_SHORT) { return 2; }
	else
	{
		LOGE("invalid type=0x%X", type);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetBooleanv(unsigned int pname)
{
	if((pname == GL_ACTIVE_TEXTURE) ||
	   (pname == GL_ALPHA_BITS) ||
	   (pname == GL_ARRAY_BUFFER_BINDING) ||
	   (pname == GL_BLEND) ||
	   (pname == GL_BLEND_DST_ALPHA) ||
	   (pname == GL_BLEND_DST_RGB) ||
	   (pname == GL_BLEND_EQUATION_ALPHA) ||
	   (pname == GL_BLEND_EQUATION_RGB) ||
	   (pname == GL_BLEND_SRC_ALPHA) ||
	   (pname == GL_BLEND_SRC_RGB) ||
	   (pname == GL_BLUE_BITS) ||
	   (pname == GL_CULL_FACE) ||
	   (pname == GL_CULL_FACE_MODE) ||
	   (pname == GL_CURRENT_PROGRAM) ||
	   (pname == GL_DEPTH_BITS) ||
	   (pname == GL_DEPTH_CLEAR_VALUE) ||
	   (pname == GL_DEPTH_FUNC) ||
	   (pname == GL_DEPTH_TEST) ||
	   (pname == GL_DEPTH_WRITEMASK) ||
	   (pname == GL_DITHER) ||
	   (pname == GL_ELEMENT_ARRAY_BUFFER_BINDING) ||
	   (pname == GL_FRAMEBUFFER_BINDING) ||
	   (pname == GL_FRONT_FACE) ||
	   (pname == GL_GENERATE_MIPMAP_HINT) ||
	   (pname == GL_GREEN_BITS) ||
	   (pname == GL_IMPLEMENTATION_COLOR_READ_FORMAT) ||
	   (pname == GL_IMPLEMENTATION_COLOR_READ_TYPE) ||
	   (pname == GL_LINE_WIDTH) ||
	   (pname == GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) ||
	   (pname == GL_MAX_CUBE_MAP_TEXTURE_SIZE) ||
	   (pname == GL_MAX_FRAGMENT_UNIFORM_VECTORS) ||
	   (pname == GL_MAX_RENDERBUFFER_SIZE) ||
	   (pname == GL_MAX_TEXTURE_IMAGE_UNITS) ||
	   (pname == GL_MAX_TEXTURE_SIZE) ||
	   (pname == GL_MAX_VARYING_VECTORS) ||
	   (pname == GL_MAX_VERTEX_ATTRIBS) ||
	   (pname == GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS) ||
	   (pname == GL_MAX_VERTEX_UNIFORM_VECTORS) ||
	   (pname == GL_NUM_COMPRESSED_TEXTURE_FORMATS) ||
	   (pname == GL_NUM_SHADER_BINARY_FORMATS) ||
	   (pname == GL_PACK_ALIGNMENT) ||
	   (pname == GL_POLYGON_OFFSET_FACTOR) ||
	   (pname == GL_POLYGON_OFFSET_FILL) ||
	   (pname == GL_POLYGON_OFFSET_UNITS) ||
	   (pname == GL_RED_BITS) ||
	   (pname == GL_RENDERBUFFER_BINDING) ||
	   (pname == GL_SAMPLE_ALPHA_TO_COVERAGE) ||
	   (pname == GL_SAMPLE_BUFFERS) ||
	   (pname == GL_SAMPLE_COVERAGE) ||
	   (pname == GL_SAMPLE_COVERAGE_INVERT) ||
	   (pname == GL_SAMPLE_COVERAGE_VALUE) ||
	   (pname == GL_SAMPLES) ||
	   (pname == GL_SCISSOR_TEST) ||
	   (pname == GL_SHADER_COMPILER) ||
	   (pname == GL_STENCIL_BACK_FAIL) ||
	   (pname == GL_STENCIL_BACK_FUNC) ||
	   (pname == GL_STENCIL_BACK_PASS_DEPTH_FAIL) ||
	   (pname == GL_STENCIL_BACK_PASS_DEPTH_PASS) ||
	   (pname == GL_STENCIL_BACK_REF) ||
	   (pname == GL_STENCIL_BACK_VALUE_MASK) ||
	   (pname == GL_STENCIL_BACK_WRITEMASK) ||
	   (pname == GL_STENCIL_BITS) ||
	   (pname == GL_STENCIL_CLEAR_VALUE) ||
	   (pname == GL_STENCIL_FAIL) ||
	   (pname == GL_STENCIL_FUNC) ||
	   (pname == GL_STENCIL_PASS_DEPTH_FAIL) ||
	   (pname == GL_STENCIL_PASS_DEPTH_PASS) ||
	   (pname == GL_STENCIL_REF) ||
	   (pname == GL_STENCIL_TEST) ||
	   (pname == GL_STENCIL_VALUE_MASK) ||
	   (pname == GL_STENCIL_WRITEMASK) ||
	   (pname == GL_SUBPIXEL_BITS) ||
	   (pname == GL_TEXTURE_BINDING_2D) ||
	   (pname == GL_TEXTURE_BINDING_CUBE_MAP) ||
	   (pname == GL_UNPACK_ALIGNMENT))
	{
		return 1;
	}
	else if((pname == GL_ALIASED_LINE_WIDTH_RANGE) ||
	        (pname == GL_ALIASED_POINT_SIZE_RANGE) ||
	        (pname == GL_DEPTH_RANGE) ||
	        (pname == GL_MAX_VIEWPORT_DIMS))
	{
		return 2;
	}
	else if((pname == GL_BLEND_COLOR) ||
	        (pname == GL_COLOR_CLEAR_VALUE) ||
	        (pname == GL_COLOR_WRITEMASK) ||
	        (pname == GL_SCISSOR_BOX) ||
	        (pname == GL_VIEWPORT))
	{
		return 4;
	}
	else
	{
		// TODO
		// GL_COMPRESSED_TEXTURE_FORMATS,
		// GL_SHADER_BINARY_FORMATS
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetBufferParameteriv(unsigned int pname)
{
	if((pname == GL_BUFFER_SIZE) ||
	   (pname == GL_BUFFER_USAGE))
	{
		return 1;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetFramebufferAttachmentParameteriv(unsigned int pname)
{
	if((pname == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE) ||
	   (pname == GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME) ||
	   (pname == GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL) ||
	   (pname == GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE))
	{
		return 1;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetProgramiv(unsigned int pname)
{
	if((pname == GL_DELETE_STATUS) ||
	   (pname == GL_LINK_STATUS) ||
	   (pname == GL_VALIDATE_STATUS) ||
	   (pname == GL_INFO_LOG_LENGTH) ||
	   (pname == GL_ATTACHED_SHADERS) ||
	   (pname == GL_ACTIVE_ATTRIBUTES) ||
	   (pname == GL_ACTIVE_ATTRIBUTE_MAX_LENGTH) ||
	   (pname == GL_ACTIVE_UNIFORMS) ||
	   (pname == GL_ACTIVE_UNIFORM_MAX_LENGTH))
	{
		return 1;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetRenderbufferParameteriv(unsigned int pname)
{
	if((pname == GL_RENDERBUFFER_WIDTH) ||
	   (pname == GL_RENDERBUFFER_HEIGHT) ||
	   (pname == GL_RENDERBUFFER_INTERNAL_FORMAT) ||
	   (pname == GL_RENDERBUFFER_RED_SIZE) ||
	   (pname == GL_RENDERBUFFER_GREEN_SIZE) ||
	   (pname == GL_RENDERBUFFER_BLUE_SIZE) ||
	   (pname == GL_RENDERBUFFER_ALPHA_SIZE) ||
	   (pname == GL_RENDERBUFFER_DEPTH_SIZE) ||
	   (pname == GL_RENDERBUFFER_STENCIL_SIZE))
	{
		return 1;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetShaderiv(unsigned int pname)
{
	if((pname == GL_SHADER_TYPE) ||
	   (pname == GL_DELETE_STATUS) ||
	   (pname == GL_COMPILE_STATUS) ||
	   (pname == GL_INFO_LOG_LENGTH) ||
	   (pname == GL_SHADER_SOURCE_LENGTH))
	{
		return 1;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetTexParameter(unsigned int pname)
{
	if((pname == GL_TEXTURE_MAG_FILTER) ||
	   (pname == GL_TEXTURE_MIN_FILTER) ||
	   (pname == GL_TEXTURE_WRAP_S) ||
	   (pname == GL_TEXTURE_WRAP_T))
	{
		return 1;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countGetVertexAttrib(unsigned int pname)
{
	if((pname == GL_TEXTURE_MAG_FILTER) ||
	   (pname == GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING) ||
	   (pname == GL_VERTEX_ATTRIB_ARRAY_ENABLED) ||
	   (pname == GL_VERTEX_ATTRIB_ARRAY_SIZE) ||
	   (pname == GL_VERTEX_ATTRIB_ARRAY_STRIDE) ||
	   (pname == GL_VERTEX_ATTRIB_ARRAY_TYPE) ||
	   (pname == GL_VERTEX_ATTRIB_ARRAY_NORMALIZED))
	{
		return 1;
	}
	else if(pname == GL_CURRENT_VERTEX_ATTRIB)
	{
		return 4;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_bppReadPixels(unsigned int format, unsigned int type)
{
	if((format == GL_ALPHA) && (type == GL_UNSIGNED_BYTE))
	{
		return 1;
	}
	else if((format == GL_RGB) && (type == GL_UNSIGNED_BYTE))
	{
		return 3;
	}
	else if((format == GL_RGB) && (type == GL_UNSIGNED_SHORT_5_6_5))
	{
		return 2;
	}
	else if((format == GL_RGBA) && (type == GL_UNSIGNED_BYTE))
	{
		return 4;
	}
	else if((format == GL_RGBA) && (type == GL_UNSIGNED_SHORT_4_4_4_4))
	{
		return 2;
	}
	else if((format == GL_RGBA) && (type == GL_UNSIGNED_SHORT_5_6_5))
	{
		return 2;
	}
	else
	{
		LOGE("invalid format=0x%X, type=0x%X", format, type);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_bppTexImage(unsigned int format, unsigned int type)
{
	if((format == GL_ALPHA) && (type == GL_UNSIGNED_BYTE))
	{
		return 1;
	}
	else if((format == GL_LUMINANCE) && (type == GL_UNSIGNED_BYTE))
	{
		return 1;
	}
	else if((format == GL_LUMINANCE_ALPHA) && (type == GL_UNSIGNED_BYTE))
	{
		return 2;
	}
	else if((format == GL_RGB) && (type == GL_UNSIGNED_BYTE))
	{
		return 3;
	}
	else if((format == GL_RGB) && (type == GL_UNSIGNED_SHORT_5_6_5))
	{
		return 2;
	}
	else if((format == GL_RGBA) && (type == GL_UNSIGNED_BYTE))
	{
		return 4;
	}
	else if((format == GL_RGBA) && (type == GL_UNSIGNED_SHORT_4_4_4_4))
	{
		return 2;
	}
	else if((format == GL_RGBA) && (type == GL_UNSIGNED_SHORT_5_6_5))
	{
		return 2;
	}
	else
	{
		LOGE("invalid format=0x%X, type=0x%X", format, type);
		assert(0);
		return 0;
	}
}

unsigned int loax_serialize_countTexParameter(unsigned int pname)
{
	if((pname == GL_TEXTURE_MIN_FILTER) ||
	   (pname == GL_TEXTURE_MAG_FILTER) ||
	   (pname == GL_TEXTURE_WRAP_S) ||
	   (pname == GL_TEXTURE_WRAP_T))
	{
		return 1;
	}
	else
	{
		LOGE("invalid pname=0x%X", pname);
		assert(0);
		return 0;
	}
}
