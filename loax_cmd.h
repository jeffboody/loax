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

#ifndef loax_cmd_H
#define loax_cmd_H

#define LOAX_CMD_ACCELEROMETER_ENABLE  0x00010000
#define LOAX_CMD_ACCELEROMETER_DISABLE 0x00010001
#define LOAX_CMD_MAGNETOMETER_ENABLE   0x00010002
#define LOAX_CMD_MAGNETOMETER_DISABLE  0x00010003
#define LOAX_CMD_GPS_ENABLE            0x00010004
#define LOAX_CMD_GPS_DISABLE           0x00010005
#define LOAX_CMD_GYROSCOPE_ENABLE      0x00010006
#define LOAX_CMD_GYROSCOPE_DISABLE     0x00010007
#define LOAX_CMD_KEEPSCREENON_ENABLE   0x00010008
#define LOAX_CMD_KEEPSCREENON_DISABLE  0x00010009
#define LOAX_CMD_PLAY_CLICK            0x0001000A
#define LOAX_CMD_EXIT                  0x0001000B
#define LOAX_CMD_MAX                   0x0001000C

#endif
