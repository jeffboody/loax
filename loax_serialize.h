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

#ifndef loax_serialize_H
#define loax_serialize_H

#include <net/net_socket.h>

int loax_serialize_sendchar(net_socket_t* s, char data);
int loax_serialize_senduchar(net_socket_t* s, unsigned char data);
int loax_serialize_sendshort(net_socket_t* s, short data);
int loax_serialize_sendushort(net_socket_t* s, unsigned short data);
int loax_serialize_sendint(net_socket_t* s, int data);
int loax_serialize_senduint(net_socket_t* s, unsigned int data);
int loax_serialize_sendfloat(net_socket_t* s, float data);
int loax_serialize_sendintptr(net_socket_t* s, const int* data);
int loax_serialize_sendvoidptr(net_socket_t* s, const void* data);
int loax_serialize_sendarraychar(net_socket_t* s, unsigned int size, const char* data);
int loax_serialize_sendarrayuchar(net_socket_t* s, unsigned int size, const unsigned char* data);
int loax_serialize_sendarrayvoid(net_socket_t* s, unsigned int size, const void* data);
int loax_serialize_sendarrayint(net_socket_t* s, unsigned int size, const int* data);
int loax_serialize_sendarrayuint(net_socket_t* s, unsigned int size, const unsigned int* data);
int loax_serialize_sendarrayfloat(net_socket_t* s, unsigned int size, const float* data);
int loax_serialize_sendstring(net_socket_t* s, const char* data);

int loax_serialize_recvchar(net_socket_t* s, char* data);
int loax_serialize_recvuchar(net_socket_t* s, unsigned char* data);
int loax_serialize_recvshort(net_socket_t* s, short* data);
int loax_serialize_recvushort(net_socket_t* s, unsigned short* data);
int loax_serialize_recvint(net_socket_t* s, int* data);
int loax_serialize_recvuint(net_socket_t* s, unsigned int* data);
int loax_serialize_recvfloat(net_socket_t* s, float* data);
int loax_serialize_recvintptr(net_socket_t* s, int** data);
int loax_serialize_recvvoidptr(net_socket_t* s, void** data);
int loax_serialize_recvarraychar(net_socket_t* s, unsigned int size, char* data);
int loax_serialize_recvarrayuchar(net_socket_t* s, unsigned int size, unsigned char* data);
int loax_serialize_recvarrayvoid(net_socket_t* s, unsigned int size, void* data);
int loax_serialize_recvarrayint(net_socket_t* s, unsigned int size, int* data);
int loax_serialize_recvarrayuint(net_socket_t* s, unsigned int size, unsigned int* data);
int loax_serialize_recvarrayfloat(net_socket_t* s, unsigned int size, float* data);
int loax_serialize_recvstring(net_socket_t* s, char** data);

unsigned int loax_serialize_bytesDrawElements(unsigned int type);
unsigned int loax_serialize_countGetBooleanv(unsigned int pname);
unsigned int loax_serialize_countGetBufferParameteriv(unsigned int pname);
unsigned int loax_serialize_countGetFramebufferAttachmentParameteriv(unsigned int pname);
unsigned int loax_serialize_countGetProgramiv(unsigned int pname);
unsigned int loax_serialize_countGetRenderbufferParameteriv(unsigned int pname);
unsigned int loax_serialize_countGetShaderiv(unsigned int pname);
unsigned int loax_serialize_countGetTexParameter(unsigned int pname);
unsigned int loax_serialize_countGetVertexAttrib(unsigned int pname);
unsigned int loax_serialize_bppReadPixels(unsigned int format, unsigned int type);
unsigned int loax_serialize_bppTexImage(unsigned int format, unsigned int type);
unsigned int loax_serialize_countTexParameter(unsigned int pname);

#endif
