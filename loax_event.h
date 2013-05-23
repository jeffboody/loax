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

#ifndef loax_event_H
#define loax_event_H

// type
#define LOAX_EVENT_KEYDOWN    0
#define LOAX_EVENT_KEYUP      1
#define LOAX_EVENT_BUTTONDOWN 2
#define LOAX_EVENT_BUTTONUP   3
#define LOAX_EVENT_RESIZE     4
#define LOAX_EVENT_TOUCHDOWN  5
#define LOAX_EVENT_TOUCHUP    6
#define LOAX_EVENT_TOUCHMOVE  7

// meta key mask
#define LOAX_KEY_ALT     0x00000032
#define LOAX_KEY_ALT_L   0x00000010
#define LOAX_KEY_ALT_R   0x00000020
#define LOAX_KEY_CTRL    0x00007000
#define LOAX_KEY_CTRL_L  0x00002000
#define LOAX_KEY_CTRL_R  0x00004000
#define LOAX_KEY_SHIFT   0x000000c1
#define LOAX_KEY_SHIFT_L 0x00000040
#define LOAX_KEY_SHIFT_R 0x00000080
#define LOAX_KEY_CAPS    0x00100000

// button ids
#define LOAX_BUTTON_1      0xbc
#define LOAX_BUTTON_2      0xbd
#define LOAX_BUTTON_3      0xbe
#define LOAX_BUTTON_4      0xbf
#define LOAX_BUTTON_5      0xc0
#define LOAX_BUTTON_6      0xc1
#define LOAX_BUTTON_7      0xc2
#define LOAX_BUTTON_8      0xc3
#define LOAX_BUTTON_9      0xc4
#define LOAX_BUTTON_10     0xc5
#define LOAX_BUTTON_11     0xc6
#define LOAX_BUTTON_12     0xc7
#define LOAX_BUTTON_13     0xc8
#define LOAX_BUTTON_14     0xc9
#define LOAX_BUTTON_15     0xca
#define LOAX_BUTTON_16     0xcb
#define LOAX_BUTTON_A      0x60
#define LOAX_BUTTON_B      0x61
#define LOAX_BUTTON_C      0x62
#define LOAX_BUTTON_L1     0x66
#define LOAX_BUTTON_R1     0x67
#define LOAX_BUTTON_L2     0x68
#define LOAX_BUTTON_R2     0x69
#define LOAX_BUTTON_MODE   0x6e
#define LOAX_BUTTON_SELECT 0x6d
#define LOAX_BUTTON_START  0x6c
#define LOAX_BUTTON_THUMBL 0x6a
#define LOAX_BUTTON_THUMBR 0x6b
#define LOAX_BUTTON_X      0x63
#define LOAX_BUTTON_Y      0x64
#define LOAX_BUTTON_Z      0x65
#define LOAX_BUTTON_UP     0x13
#define LOAX_BUTTON_DOWN   0x14
#define LOAX_BUTTON_LEFT   0x15
#define LOAX_BUTTON_RIGHT  0x16
#define LOAX_BUTTON_CENTER 0x17

// max touch events supported
#define LOAX_EVENT_TOUCHMAX 4

typedef struct
{
	int keycode;
	int meta;
} loax_eventkey_t;

typedef struct
{
	int keycode;
	int id;
} loax_eventbutton_t;

typedef struct
{
	int w;
	int h;
} loax_eventresize_t;

typedef struct
{
	float x;
	float y;
} loax_eventcoord_t;

typedef struct
{
	int count;
	loax_eventcoord_t coord[LOAX_EVENT_TOUCHMAX];
} loax_eventtouch_t;

typedef struct
{
	int type;
	union
	{
		loax_eventkey_t    event_key;
		loax_eventbutton_t event_button;
		loax_eventresize_t event_resize;
		loax_eventtouch_t  event_touch;
	};
} loax_event_t;

const char* loax_eventbutton_keystring(loax_eventbutton_t* self);

#endif
