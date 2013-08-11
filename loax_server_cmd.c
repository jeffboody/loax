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
#include "loax_cmd.h"
#include "loax_server_cmd.h"
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
#define CALL_CMD(cmd)            (*g_server->cmd_fn)(cmd)

/***********************************************************
* public CMD API                                           *
***********************************************************/

void loaxCmdOrientationEnable (void)
{
	LOGD("debug");

	int ret = CALL_CMD(LOAX_CMD_ORIENTATION_ENABLE);
	SEND_INT(ret);
}

void loaxCmdOrientationDisable (void)
{
	LOGD("debug");

	int ret = CALL_CMD(LOAX_CMD_ORIENTATION_DISABLE);
	SEND_INT(ret);
}

void loaxCmdGpsEnable (void)
{
	LOGD("debug");

	int ret = CALL_CMD(LOAX_CMD_GPS_ENABLE);
	SEND_INT(ret);
}

void loaxCmdGpsDisable (void)
{
	LOGD("debug");

	int ret = CALL_CMD(LOAX_CMD_GPS_DISABLE);
	SEND_INT(ret);
}
