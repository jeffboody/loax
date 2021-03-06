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
#include "loax_event.h"

#define LOG_TAG "loax"
#include "loax_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static const char* LOAX_STRING_UNKNOWN = "UNKNOWN";

// key strings
static const char* LOAX_KEYSTRING[128] =
{
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"BACKSPACE",
	"TAB",
	"ENTER",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"ESC",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"UNKNOWN",
	"SPACE",
	"!",
	"\"",
	"#",
	"$",
	"%",
	"&",
	"'",
	"(",
	")",
	"*",
	"+",
	",",
	"-",
	".",
	"/",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	":",
	";",
	"<",
	"=",
	">",
	"?",
	"@",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"[",
	"\\",
	"]",
	"^",
	"_",
	"`",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z",
	"{",
	"|",
	"}",
	"~",
	"DELETE",
};

// button strings
static const char* LOAX_BUTTONSTRING_1       = "1";
static const char* LOAX_BUTTONSTRING_2       = "2";
static const char* LOAX_BUTTONSTRING_3       = "3";
static const char* LOAX_BUTTONSTRING_4       = "4";
static const char* LOAX_BUTTONSTRING_5       = "5";
static const char* LOAX_BUTTONSTRING_6       = "6";
static const char* LOAX_BUTTONSTRING_7       = "7";
static const char* LOAX_BUTTONSTRING_8       = "8";
static const char* LOAX_BUTTONSTRING_9       = "9";
static const char* LOAX_BUTTONSTRING_10      = "10";
static const char* LOAX_BUTTONSTRING_11      = "11";
static const char* LOAX_BUTTONSTRING_12      = "12";
static const char* LOAX_BUTTONSTRING_13      = "13";
static const char* LOAX_BUTTONSTRING_14      = "14";
static const char* LOAX_BUTTONSTRING_15      = "15";
static const char* LOAX_BUTTONSTRING_16      = "16";
static const char* LOAX_BUTTONSTRING_A       = "A";
static const char* LOAX_BUTTONSTRING_B       = "B";
static const char* LOAX_BUTTONSTRING_C       = "C";
static const char* LOAX_BUTTONSTRING_L1      = "L1";
static const char* LOAX_BUTTONSTRING_R1      = "R1";
static const char* LOAX_BUTTONSTRING_L2      = "L2";
static const char* LOAX_BUTTONSTRING_R2      = "R2";
static const char* LOAX_BUTTONSTRING_MODE    = "MODE";
static const char* LOAX_BUTTONSTRING_SELECT  = "SELECT";
static const char* LOAX_BUTTONSTRING_START   = "START";
static const char* LOAX_BUTTONSTRING_THUMBL  = "THUMBL";
static const char* LOAX_BUTTONSTRING_THUMBR  = "THUMBR";
static const char* LOAX_BUTTONSTRING_X       = "X";
static const char* LOAX_BUTTONSTRING_Y       = "Y";
static const char* LOAX_BUTTONSTRING_Z       = "Z";
static const char* LOAX_BUTTONSTRING_UP      = "UP";
static const char* LOAX_BUTTONSTRING_DOWN    = "DOWN";
static const char* LOAX_BUTTONSTRING_LEFT    = "LEFT";
static const char* LOAX_BUTTONSTRING_RIGHT   = "RIGHT";
static const char* LOAX_BUTTONSTRING_CENTER  = "CENTER";

// axis strings
static const char* LOAX_AXISSTRING_X1 = "AX1";
static const char* LOAX_AXISSTRING_Y1 = "AY1";
static const char* LOAX_AXISSTRING_X2 = "AX2";
static const char* LOAX_AXISSTRING_Y2 = "AY2";
static const char* LOAX_AXISSTRING_HX = "AHX";
static const char* LOAX_AXISSTRING_HY = "AHY";

/***********************************************************
* private                                                  *
***********************************************************/

const char* loax_eventkey_keystring(loax_eventkey_t* self)
{
	int k = self->keycode;
	if((k >= 0) && (k < 128))
	{
		return LOAX_KEYSTRING[k];
	}
	else if(k == LOAX_KEY_UP)
	{
		return "UP";
	}
	else if(k == LOAX_KEY_DOWN)
	{
		return "DOWN";
	}
	else if(k == LOAX_KEY_LEFT)
	{
		return "LEFT";
	}
	else if(k == LOAX_KEY_RIGHT)
	{
		return "RIGHT";
	}
	else if(k == LOAX_KEY_HOME)
	{
		return "HOME";
	}
	else if(k == LOAX_KEY_END)
	{
		return "END";
	}
	else if(k == LOAX_KEY_PGUP)
	{
		return "PGUP";
	}
	else if(k == LOAX_KEY_PGDOWN)
	{
		return "PGDOWN";
	}
	else if(k == LOAX_KEY_INSERT)
	{
		return "INSERT";
	}
	return LOAX_STRING_UNKNOWN;
}

const char* loax_eventbutton_keystring(loax_eventbutton_t* self)
{
	assert(self);
	LOGD("debug");

	int k = self->keycode;
	if     (k == LOAX_BUTTON_1)      return LOAX_BUTTONSTRING_1;
	else if(k == LOAX_BUTTON_2)      return LOAX_BUTTONSTRING_2;
	else if(k == LOAX_BUTTON_3)      return LOAX_BUTTONSTRING_3;
	else if(k == LOAX_BUTTON_4)      return LOAX_BUTTONSTRING_4;
	else if(k == LOAX_BUTTON_5)      return LOAX_BUTTONSTRING_5;
	else if(k == LOAX_BUTTON_6)      return LOAX_BUTTONSTRING_6;
	else if(k == LOAX_BUTTON_7)      return LOAX_BUTTONSTRING_7;
	else if(k == LOAX_BUTTON_8)      return LOAX_BUTTONSTRING_8;
	else if(k == LOAX_BUTTON_9)      return LOAX_BUTTONSTRING_9;
	else if(k == LOAX_BUTTON_10)     return LOAX_BUTTONSTRING_10;
	else if(k == LOAX_BUTTON_11)     return LOAX_BUTTONSTRING_11;
	else if(k == LOAX_BUTTON_12)     return LOAX_BUTTONSTRING_12;
	else if(k == LOAX_BUTTON_13)     return LOAX_BUTTONSTRING_13;
	else if(k == LOAX_BUTTON_14)     return LOAX_BUTTONSTRING_14;
	else if(k == LOAX_BUTTON_15)     return LOAX_BUTTONSTRING_15;
	else if(k == LOAX_BUTTON_16)     return LOAX_BUTTONSTRING_16;
	else if(k == LOAX_BUTTON_A)      return LOAX_BUTTONSTRING_A;
	else if(k == LOAX_BUTTON_B)      return LOAX_BUTTONSTRING_B;
	else if(k == LOAX_BUTTON_C)      return LOAX_BUTTONSTRING_C;
	else if(k == LOAX_BUTTON_L1)     return LOAX_BUTTONSTRING_L1;
	else if(k == LOAX_BUTTON_R1)     return LOAX_BUTTONSTRING_R1;
	else if(k == LOAX_BUTTON_L2)     return LOAX_BUTTONSTRING_L2;
	else if(k == LOAX_BUTTON_R2)     return LOAX_BUTTONSTRING_R2;
	else if(k == LOAX_BUTTON_MODE)   return LOAX_BUTTONSTRING_MODE;
	else if(k == LOAX_BUTTON_SELECT) return LOAX_BUTTONSTRING_SELECT;
	else if(k == LOAX_BUTTON_START)  return LOAX_BUTTONSTRING_START;
	else if(k == LOAX_BUTTON_THUMBL) return LOAX_BUTTONSTRING_THUMBL;
	else if(k == LOAX_BUTTON_THUMBR) return LOAX_BUTTONSTRING_THUMBR;
	else if(k == LOAX_BUTTON_X)      return LOAX_BUTTONSTRING_X;
	else if(k == LOAX_BUTTON_Y)      return LOAX_BUTTONSTRING_Y;
	else if(k == LOAX_BUTTON_Z)      return LOAX_BUTTONSTRING_Z;
	else if(k == LOAX_BUTTON_UP)     return LOAX_BUTTONSTRING_UP;
	else if(k == LOAX_BUTTON_DOWN)   return LOAX_BUTTONSTRING_DOWN;
	else if(k == LOAX_BUTTON_LEFT)   return LOAX_BUTTONSTRING_LEFT;
	else if(k == LOAX_BUTTON_RIGHT)  return LOAX_BUTTONSTRING_RIGHT;
	else if(k == LOAX_BUTTON_CENTER) return LOAX_BUTTONSTRING_CENTER;
	else                             return LOAX_STRING_UNKNOWN;
}

const char* loax_eventaxis_axisstring(loax_eventaxis_t* self)
{
	assert(self);
	LOGD("debug");

	int a = self->axis;
	if     (a == LOAX_AXIS_X1) return LOAX_AXISSTRING_X1;
	else if(a == LOAX_AXIS_Y1) return LOAX_AXISSTRING_Y1;
	else if(a == LOAX_AXIS_X2) return LOAX_AXISSTRING_X2;
	else if(a == LOAX_AXIS_Y2) return LOAX_AXISSTRING_Y2;
	else if(a == LOAX_AXIS_HX) return LOAX_AXISSTRING_HX;
	else if(a == LOAX_AXIS_HY) return LOAX_AXISSTRING_HY;
	else                       return LOAX_STRING_UNKNOWN;
}
