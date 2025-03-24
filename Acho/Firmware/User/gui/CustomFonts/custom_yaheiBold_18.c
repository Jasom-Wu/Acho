/*******************************************************************************
 * Size: 18 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef CUSTOM_YAHEIBOLD_18
#define CUSTOM_YAHEIBOLD_18 1
#endif

#if CUSTOM_YAHEIBOLD_18

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xff, 0xe0, 0x7f, 0xc0,

    /* U+0022 "\"" */
    0xcf, 0x3c, 0xf3,

    /* U+0023 "#" */
    0xc, 0xc1, 0x98, 0x33, 0xc, 0x47, 0xfe, 0xff,
    0xc6, 0x60, 0xcc, 0x7f, 0xdf, 0xf8, 0xcc, 0x19,
    0x83, 0x30, 0x44, 0x0,

    /* U+0024 "$" */
    0x8, 0x1f, 0x9f, 0xdd, 0x6c, 0x86, 0x43, 0xe0,
    0xf8, 0x3f, 0x7, 0xc2, 0xe1, 0x7c, 0xbf, 0xf9,
    0xf8, 0x10, 0x8, 0x0,

    /* U+0025 "%" */
    0x3c, 0x38, 0xfc, 0x63, 0x19, 0x86, 0x37, 0xc,
    0x6c, 0x1f, 0xb8, 0x1e, 0x60, 0x1, 0x9e, 0x7,
    0x7e, 0xd, 0x8c, 0x3b, 0x18, 0x66, 0x31, 0x8f,
    0xc7, 0xf, 0x0,

    /* U+0026 "&" */
    0x1f, 0x0, 0x3f, 0x80, 0x71, 0xc0, 0x71, 0xce,
    0x7b, 0xce, 0x3f, 0x8e, 0x3f, 0xe, 0x77, 0x8c,
    0xe1, 0xdc, 0xe0, 0xf8, 0xe0, 0x70, 0xf1, 0xf8,
    0x7f, 0xff, 0x3f, 0xf,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x1c, 0xe3, 0x1c, 0x73, 0x8e, 0x38, 0xe3, 0x8e,
    0x38, 0x71, 0xc3, 0xe, 0x1c,

    /* U+0029 ")" */
    0xe1, 0xc3, 0xe, 0x38, 0x71, 0xc7, 0x1c, 0x71,
    0xc7, 0x38, 0xe3, 0x1c, 0xe0,

    /* U+002A "*" */
    0x30, 0x23, 0x5f, 0xf3, 0x8d, 0x93, 0x0,

    /* U+002B "+" */
    0xc, 0x6, 0x3, 0x1f, 0xff, 0xf8, 0x60, 0x30,
    0x18, 0xc, 0x0,

    /* U+002C "," */
    0x77, 0x66, 0xe0,

    /* U+002D "-" */
    0xff, 0xc0,

    /* U+002E "." */
    0xff, 0x80,

    /* U+002F "/" */
    0x3, 0x81, 0x81, 0xc0, 0xc0, 0xe0, 0x60, 0x70,
    0x38, 0x18, 0x1c, 0xc, 0xe, 0x7, 0x3, 0x3,
    0x81, 0x80,

    /* U+0030 "0" */
    0x1e, 0xf, 0xe7, 0x3b, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0x87, 0xe1, 0xf8, 0x7e, 0x19, 0xce, 0x7f,
    0x7, 0x80,

    /* U+0031 "1" */
    0x1d, 0xff, 0xff, 0x1c, 0x71, 0xc7, 0x1c, 0x71,
    0xc7, 0x1c, 0x70,

    /* U+0032 "2" */
    0x3e, 0x3f, 0x91, 0xe0, 0x70, 0x38, 0x1c, 0x1c,
    0x3e, 0x3e, 0x3e, 0x1c, 0x1c, 0xf, 0xff, 0xfc,

    /* U+0033 "3" */
    0x7e, 0x3f, 0x91, 0xe0, 0x70, 0x38, 0x38, 0xf8,
    0x7e, 0x7, 0x81, 0xc0, 0xf0, 0xff, 0xf3, 0xf0,

    /* U+0034 "4" */
    0x3, 0x80, 0xf0, 0x1e, 0x7, 0xc1, 0xf8, 0x37,
    0xc, 0xe3, 0x9c, 0xe3, 0x9f, 0xff, 0xff, 0x81,
    0xc0, 0x38, 0x7, 0x0,

    /* U+0035 "5" */
    0x7f, 0x3f, 0x9c, 0xe, 0x7, 0x3, 0xf1, 0xfc,
    0xf, 0x3, 0x81, 0xc0, 0xf0, 0xef, 0xf3, 0xe0,

    /* U+0036 "6" */
    0xf, 0x8f, 0xe7, 0x81, 0xc0, 0xe0, 0x3b, 0xcf,
    0xfb, 0xcf, 0xe1, 0xf8, 0x7e, 0x1d, 0xce, 0x7f,
    0x87, 0x80,

    /* U+0037 "7" */
    0xff, 0xff, 0xf0, 0x18, 0xe, 0x7, 0x1, 0xc0,
    0xe0, 0x38, 0x1e, 0x7, 0x1, 0xc0, 0xf0, 0x38,
    0xe, 0x0,

    /* U+0038 "8" */
    0x3e, 0x1f, 0xcf, 0x7b, 0x8e, 0xe3, 0x9d, 0xe3,
    0xf0, 0xfc, 0x73, 0xb8, 0x7e, 0x1f, 0xcf, 0x7f,
    0x8f, 0xc0,

    /* U+0039 "9" */
    0x1e, 0x1f, 0xe7, 0x3b, 0x87, 0xe1, 0xf8, 0x7f,
    0x3d, 0xff, 0x3d, 0xc0, 0x70, 0x38, 0x1e, 0x7f,
    0x1f, 0x0,

    /* U+003A ":" */
    0xff, 0x80, 0x7, 0xfc,

    /* U+003B ";" */
    0x77, 0x70, 0x0, 0x7, 0x66, 0x66,

    /* U+003C "<" */
    0x1, 0x83, 0xc7, 0x8f, 0xe, 0x7, 0xc0, 0x78,
    0xf, 0x1, 0x80,

    /* U+003D "=" */
    0xff, 0xff, 0xc0, 0x0, 0xf, 0xff, 0xfc,

    /* U+003E ">" */
    0x0, 0x60, 0x3c, 0x7, 0x80, 0xf8, 0x1c, 0x3c,
    0xf8, 0xf0, 0x60, 0x0,

    /* U+003F "?" */
    0x7c, 0xfe, 0xc7, 0x7, 0x7, 0xe, 0x1e, 0x3c,
    0x38, 0x38, 0x0, 0x38, 0x38, 0x38,

    /* U+0040 "@" */
    0x7, 0xe0, 0x1f, 0xf8, 0x3c, 0x1c, 0x30, 0x6,
    0x63, 0xe7, 0x67, 0xe3, 0xce, 0x63, 0xcc, 0x63,
    0xcc, 0x63, 0xcc, 0x63, 0xcc, 0xe6, 0xcf, 0xfe,
    0x67, 0x38, 0x70, 0x0, 0x38, 0x10, 0x1f, 0xf0,
    0x7, 0xe0,

    /* U+0041 "A" */
    0x7, 0x80, 0x3c, 0x3, 0xe0, 0x1f, 0x81, 0xdc,
    0xe, 0xe0, 0x73, 0x87, 0x1c, 0x38, 0xe1, 0xff,
    0x9f, 0xfc, 0xe0, 0xf7, 0x3, 0xf0, 0x1c,

    /* U+0042 "B" */
    0xfe, 0x3f, 0xce, 0x3b, 0x8e, 0xe3, 0xb9, 0xcf,
    0xe3, 0xfe, 0xe3, 0xf8, 0x7e, 0x1f, 0x8f, 0xff,
    0xbf, 0xc0,

    /* U+0043 "C" */
    0xf, 0xe3, 0xfc, 0xe0, 0xb8, 0xe, 0x1, 0xc0,
    0x38, 0x7, 0x0, 0xe0, 0x1c, 0x1, 0xc0, 0x3c,
    0x13, 0xfe, 0x1f, 0x80,

    /* U+0044 "D" */
    0xff, 0xf, 0xfc, 0xe1, 0xee, 0xe, 0xe0, 0x7e,
    0x7, 0xe0, 0x7e, 0x7, 0xe0, 0x7e, 0xf, 0xe0,
    0xee, 0x1c, 0xff, 0x8f, 0xf0,

    /* U+0045 "E" */
    0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xe0, 0xff, 0xff,
    0xe0, 0xe0, 0xe0, 0xe0, 0xff, 0xff,

    /* U+0046 "F" */
    0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xe0, 0xff, 0xff,
    0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,

    /* U+0047 "G" */
    0xf, 0xe1, 0xfe, 0x38, 0x27, 0x0, 0xf0, 0xe,
    0x0, 0xe3, 0xfe, 0x3f, 0xe0, 0x7e, 0x7, 0x70,
    0x77, 0x87, 0x3f, 0xf0, 0xfe,

    /* U+0048 "H" */
    0xe0, 0x7e, 0x7, 0xe0, 0x7e, 0x7, 0xe0, 0x7e,
    0x7, 0xff, 0xff, 0xff, 0xe0, 0x7e, 0x7, 0xe0,
    0x7e, 0x7, 0xe0, 0x7e, 0x7,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,

    /* U+004A "J" */
    0xe, 0x1c, 0x38, 0x70, 0xe1, 0xc3, 0x87, 0xe,
    0x1c, 0x38, 0xef, 0xde, 0x0,

    /* U+004B "K" */
    0xe1, 0xdc, 0x7b, 0x8e, 0x73, 0x8e, 0xf1, 0xdc,
    0x3f, 0x7, 0xf0, 0xee, 0x1d, 0xe3, 0x9e, 0x71,
    0xce, 0x3d, 0xc3, 0xc0,

    /* U+004C "L" */
    0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
    0xe0, 0xe0, 0xe0, 0xe0, 0xff, 0xff,

    /* U+004D "M" */
    0xf8, 0x1f, 0xf0, 0x7f, 0xe0, 0xff, 0xe1, 0xff,
    0xc7, 0xfd, 0x8d, 0xfb, 0x9b, 0xf7, 0x77, 0xe6,
    0xcf, 0xcd, 0x9f, 0x9f, 0x3f, 0x1c, 0x7e, 0x38,
    0xfc, 0x71, 0xc0,

    /* U+004E "N" */
    0xf0, 0x7f, 0x7, 0xf8, 0x7f, 0x87, 0xfc, 0x7e,
    0xe7, 0xee, 0x7e, 0x77, 0xe3, 0x7e, 0x3f, 0xe1,
    0xfe, 0x1f, 0xe0, 0xfe, 0x7,

    /* U+004F "O" */
    0xf, 0x81, 0xff, 0x1e, 0x3c, 0xe0, 0xee, 0x3,
    0xf0, 0x1f, 0x80, 0xfc, 0x7, 0xe0, 0x3f, 0x1,
    0xdc, 0x1c, 0xf1, 0xe3, 0xfe, 0x7, 0xc0,

    /* U+0050 "P" */
    0xff, 0x3f, 0xee, 0x3f, 0x87, 0xe1, 0xf8, 0x7e,
    0x3b, 0xfe, 0xfe, 0x38, 0xe, 0x3, 0x80, 0xe0,
    0x38, 0x0,

    /* U+0051 "Q" */
    0xf, 0x80, 0xff, 0x87, 0x8f, 0x1c, 0x1c, 0xe0,
    0x3b, 0x80, 0xee, 0x3, 0xb8, 0xe, 0xe0, 0x3b,
    0x80, 0xe7, 0x7, 0x1e, 0x3c, 0x3f, 0xe0, 0x3f,
    0xc0, 0x7, 0x80, 0xf,

    /* U+0052 "R" */
    0xff, 0xf, 0xf8, 0xe3, 0xce, 0x1c, 0xe1, 0xce,
    0x1c, 0xe3, 0x8f, 0xf0, 0xfe, 0xe, 0x78, 0xe3,
    0x8e, 0x1c, 0xe1, 0xee, 0xe,

    /* U+0053 "S" */
    0x3f, 0x3f, 0xbc, 0x1c, 0xe, 0x7, 0xc1, 0xf8,
    0x7e, 0xf, 0x81, 0xc0, 0xf0, 0xff, 0xf7, 0xf0,

    /* U+0054 "T" */
    0xff, 0xff, 0xfc, 0x38, 0x7, 0x0, 0xe0, 0x1c,
    0x3, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x0,

    /* U+0055 "U" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xb8,
    0xe7, 0xfc, 0x3e, 0x0,

    /* U+0056 "V" */
    0xe0, 0x3b, 0x83, 0x9c, 0x1c, 0xe0, 0xe3, 0x8e,
    0x1c, 0x70, 0xe3, 0x83, 0x38, 0x1d, 0xc0, 0xee,
    0x3, 0x60, 0x1f, 0x0, 0xf8, 0x3, 0x80,

    /* U+0057 "W" */
    0xe0, 0xe0, 0xfc, 0x1e, 0x1f, 0x87, 0xc7, 0x38,
    0xf8, 0xe7, 0x1b, 0x1c, 0xe7, 0x73, 0x1c, 0xee,
    0x61, 0x99, 0xdc, 0x3b, 0x1b, 0x87, 0xe3, 0x60,
    0xfc, 0x7c, 0xf, 0xf, 0x81, 0xe0, 0xf0, 0x3c,
    0x1c, 0x0,

    /* U+0058 "X" */
    0x70, 0x73, 0xc7, 0x8e, 0x38, 0x7b, 0x81, 0xdc,
    0x7, 0xc0, 0x3e, 0x1, 0xf0, 0x1f, 0x80, 0xee,
    0xe, 0x70, 0x71, 0xc7, 0xf, 0x78, 0x38,

    /* U+0059 "Y" */
    0xe0, 0xfe, 0x1d, 0xc7, 0x3c, 0xe3, 0xb8, 0x77,
    0x7, 0xc0, 0xf8, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x0,

    /* U+005A "Z" */
    0x7f, 0xef, 0xfc, 0x7, 0x1, 0xe0, 0x38, 0xe,
    0x3, 0xc0, 0x70, 0x1c, 0x7, 0x81, 0xe0, 0x3c,
    0xf, 0xff, 0xff, 0xc0,

    /* U+005B "[" */
    0xff, 0xf9, 0xce, 0x73, 0x9c, 0xe7, 0x39, 0xce,
    0x73, 0x9f, 0xf8,

    /* U+005C "\\" */
    0xe0, 0x30, 0x1c, 0xe, 0x3, 0x1, 0xc0, 0x60,
    0x38, 0x1c, 0x6, 0x3, 0x80, 0xc0, 0x70, 0x38,
    0xc, 0x7,

    /* U+005D "]" */
    0xff, 0xce, 0x73, 0x9c, 0xe7, 0x39, 0xce, 0x73,
    0x9c, 0xff, 0xf8,

    /* U+005E "^" */
    0xc, 0x7, 0x1, 0xe0, 0xd8, 0x33, 0x18, 0xc6,
    0x1b, 0x7,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0xe3, 0xc,

    /* U+0061 "a" */
    0x3e, 0x3f, 0x90, 0xe0, 0x73, 0xff, 0x9f, 0x8f,
    0xcf, 0xff, 0xbd, 0xc0,

    /* U+0062 "b" */
    0xe0, 0x38, 0xe, 0x3, 0x80, 0xef, 0x3f, 0xef,
    0x3f, 0x87, 0xe1, 0xf8, 0x7e, 0x1f, 0xce, 0xff,
    0xbb, 0xc0,

    /* U+0063 "c" */
    0x1f, 0x7f, 0x70, 0xe0, 0xe0, 0xe0, 0xe0, 0xf0,
    0x7f, 0x1f,

    /* U+0064 "d" */
    0x1, 0xc0, 0x70, 0x1c, 0x7, 0x3d, 0xdf, 0xf7,
    0x3f, 0x87, 0xe1, 0xf8, 0x7e, 0x1f, 0xcf, 0x7f,
    0xcf, 0x70,

    /* U+0065 "e" */
    0x1e, 0x3f, 0x9c, 0xfc, 0x7f, 0xff, 0xff, 0x81,
    0xe0, 0x7f, 0x1f, 0x80,

    /* U+0066 "f" */
    0x1e, 0x7c, 0xe1, 0xcf, 0xff, 0xce, 0x1c, 0x38,
    0x70, 0xe1, 0xc3, 0x87, 0x0,

    /* U+0067 "g" */
    0x3d, 0xdf, 0xf7, 0x3f, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0xcf, 0x7f, 0xcf, 0x70, 0x1d, 0xe, 0x7f,
    0x9f, 0x80,

    /* U+0068 "h" */
    0xe0, 0x70, 0x38, 0x1c, 0xe, 0xf7, 0xff, 0xcf,
    0xc7, 0xe3, 0xf1, 0xf8, 0xfc, 0x7e, 0x3f, 0x1c,

    /* U+0069 "i" */
    0xff, 0x8f, 0xff, 0xff, 0xff, 0xc0,

    /* U+006A "j" */
    0x1c, 0x71, 0xc0, 0x1c, 0x71, 0xc7, 0x1c, 0x71,
    0xc7, 0x1c, 0x71, 0xc7, 0xfb, 0xc0,

    /* U+006B "k" */
    0xe0, 0x38, 0xe, 0x3, 0x80, 0xe3, 0xb9, 0xce,
    0xf3, 0xb8, 0xfc, 0x3f, 0xe, 0xe3, 0x9c, 0xe7,
    0xb8, 0xe0,

    /* U+006C "l" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,

    /* U+006D "m" */
    0xef, 0x3d, 0xff, 0xff, 0xcf, 0x3f, 0x1c, 0x7e,
    0x38, 0xfc, 0x71, 0xf8, 0xe3, 0xf1, 0xc7, 0xe3,
    0x8f, 0xc7, 0x1c,

    /* U+006E "n" */
    0xef, 0x7f, 0xfc, 0xfc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xc7, 0xe3, 0xf1, 0xc0,

    /* U+006F "o" */
    0x1e, 0x1f, 0xe7, 0x3b, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0xce, 0x7f, 0x87, 0x80,

    /* U+0070 "p" */
    0xef, 0x3f, 0xef, 0x3f, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0xce, 0xff, 0xbb, 0xce, 0x3, 0x80, 0xe0,
    0x38, 0x0,

    /* U+0071 "q" */
    0x3d, 0xdf, 0xf7, 0x3f, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0xcf, 0x7f, 0xcf, 0x70, 0x1c, 0x7, 0x1,
    0xc0, 0x70,

    /* U+0072 "r" */
    0xef, 0xff, 0x38, 0xe3, 0x8e, 0x38, 0xe3, 0x80,

    /* U+0073 "s" */
    0x3d, 0xfb, 0x87, 0xf, 0x87, 0xc3, 0xc7, 0xff,
    0xf0,

    /* U+0074 "t" */
    0x18, 0x70, 0xe7, 0xff, 0xe7, 0xe, 0x1c, 0x38,
    0x70, 0xe1, 0xf1, 0xe0,

    /* U+0075 "u" */
    0xe3, 0xf1, 0xf8, 0xfc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xcf, 0xff, 0xbd, 0xc0,

    /* U+0076 "v" */
    0xe1, 0xf8, 0x77, 0x39, 0xce, 0x73, 0x8c, 0xc3,
    0xf0, 0xf8, 0x1e, 0x7, 0x80,

    /* U+0077 "w" */
    0xe3, 0x8e, 0xc7, 0x9d, 0xcf, 0x33, 0x9e, 0x67,
    0x6c, 0xc6, 0xdf, 0x8d, 0xbe, 0x1f, 0x3c, 0x1c,
    0x78, 0x38, 0xe0,

    /* U+0078 "x" */
    0x71, 0xdc, 0x63, 0xb8, 0xec, 0x1e, 0x7, 0xc3,
    0xf0, 0xee, 0x73, 0xbc, 0x70,

    /* U+0079 "y" */
    0x70, 0xee, 0x18, 0xc7, 0x1c, 0xe3, 0x98, 0x37,
    0x7, 0xc0, 0x78, 0xf, 0x1, 0xc0, 0x38, 0xe,
    0x7, 0xc0, 0xf0, 0x0,

    /* U+007A "z" */
    0x7f, 0xbf, 0xc1, 0xc1, 0xc1, 0xe0, 0xe0, 0xe0,
    0xf0, 0xff, 0xff, 0xc0,

    /* U+007B "{" */
    0x1c, 0xf3, 0x8e, 0x38, 0xe3, 0x8e, 0xf3, 0xc3,
    0x8e, 0x38, 0xe3, 0x8f, 0x1c,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,

    /* U+007D "}" */
    0xe3, 0xc3, 0x8e, 0x38, 0xe3, 0x8e, 0x1c, 0x73,
    0x8e, 0x38, 0xe3, 0xbe, 0xf0,

    /* U+007E "~" */
    0x78, 0xff, 0xfc, 0x78,

    /* U+007F "" */
    0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 86, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 100, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 7, .adv_w = 150, .box_w = 6, .box_h = 4, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 10, .adv_w = 184, .box_w = 11, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 30, .adv_w = 178, .box_w = 9, .box_h = 17, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 50, .adv_w = 268, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 262, .box_w = 16, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 89, .box_w = 2, .box_h = 4, .ofs_x = 2, .ofs_y = 10},
    {.bitmap_index = 106, .adv_w = 112, .box_w = 6, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 119, .adv_w = 112, .box_w = 6, .box_h = 17, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 132, .adv_w = 140, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 139, .adv_w = 219, .box_w = 9, .box_h = 9, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 150, .adv_w = 82, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 153, .adv_w = 126, .box_w = 5, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 155, .adv_w = 82, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 136, .box_w = 9, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 175, .adv_w = 178, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 178, .box_w = 6, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 178, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 178, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 178, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 256, .adv_w = 178, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 272, .adv_w = 178, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 290, .adv_w = 178, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 178, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 178, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 82, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 82, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 354, .adv_w = 219, .box_w = 9, .box_h = 9, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 365, .adv_w = 219, .box_w = 9, .box_h = 6, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 372, .adv_w = 219, .box_w = 9, .box_h = 10, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 384, .adv_w = 137, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 398, .adv_w = 297, .box_w = 16, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 432, .adv_w = 217, .box_w = 13, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 455, .adv_w = 197, .box_w = 10, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 473, .adv_w = 194, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 493, .adv_w = 228, .box_w = 12, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 514, .adv_w = 165, .box_w = 8, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 528, .adv_w = 161, .box_w = 8, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 542, .adv_w = 220, .box_w = 12, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 563, .adv_w = 237, .box_w = 12, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 584, .adv_w = 97, .box_w = 3, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 135, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 603, .adv_w = 200, .box_w = 11, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 623, .adv_w = 158, .box_w = 8, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 637, .adv_w = 296, .box_w = 15, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 664, .adv_w = 244, .box_w = 12, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 236, .box_w = 13, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 708, .adv_w = 189, .box_w = 10, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 726, .adv_w = 236, .box_w = 14, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 754, .adv_w = 201, .box_w = 12, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 173, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 791, .adv_w = 180, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 811, .adv_w = 224, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 831, .adv_w = 206, .box_w = 13, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 854, .adv_w = 310, .box_w = 19, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 888, .adv_w = 202, .box_w = 13, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 911, .adv_w = 187, .box_w = 11, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 931, .adv_w = 187, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 951, .adv_w = 112, .box_w = 5, .box_h = 17, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 962, .adv_w = 134, .box_w = 9, .box_h = 16, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 980, .adv_w = 112, .box_w = 5, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 991, .adv_w = 219, .box_w = 10, .box_h = 8, .ofs_x = 2, .ofs_y = 6},
    {.bitmap_index = 1001, .adv_w = 129, .box_w = 8, .box_h = 1, .ofs_x = 0, .ofs_y = -5},
    {.bitmap_index = 1002, .adv_w = 96, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 1004, .adv_w = 166, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1016, .adv_w = 192, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1034, .adv_w = 149, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1044, .adv_w = 191, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1062, .adv_w = 168, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1074, .adv_w = 117, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1087, .adv_w = 191, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1105, .adv_w = 186, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1121, .adv_w = 85, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1127, .adv_w = 87, .box_w = 6, .box_h = 18, .ofs_x = -2, .ofs_y = -4},
    {.bitmap_index = 1141, .adv_w = 172, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1159, .adv_w = 85, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1165, .adv_w = 283, .box_w = 15, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1184, .adv_w = 187, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1196, .adv_w = 189, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1209, .adv_w = 192, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1227, .adv_w = 191, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1245, .adv_w = 122, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1253, .adv_w = 142, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1262, .adv_w = 119, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1274, .adv_w = 187, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1286, .adv_w = 166, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1299, .adv_w = 245, .box_w = 15, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1318, .adv_w = 168, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1331, .adv_w = 165, .box_w = 11, .box_h = 14, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 1351, .adv_w = 148, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1363, .adv_w = 112, .box_w = 6, .box_h = 17, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1376, .adv_w = 98, .box_w = 3, .box_h = 19, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 1384, .adv_w = 112, .box_w = 6, .box_h = 17, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1397, .adv_w = 219, .box_w = 10, .box_h = 3, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 1401, .adv_w = 86, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 96, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    3, 83,
    3, 84,
    8, 83,
    8, 84,
    9, 75,
    11, 34,
    11, 43,
    11, 68,
    11, 69,
    11, 70,
    11, 72,
    11, 80,
    11, 82,
    34, 11,
    34, 13,
    34, 28,
    34, 36,
    34, 40,
    34, 43,
    34, 48,
    34, 53,
    34, 54,
    34, 55,
    34, 56,
    34, 58,
    34, 59,
    34, 85,
    34, 87,
    34, 88,
    34, 90,
    35, 53,
    35, 58,
    36, 32,
    36, 36,
    36, 40,
    36, 48,
    36, 50,
    37, 13,
    37, 15,
    37, 34,
    37, 53,
    37, 57,
    37, 59,
    38, 34,
    38, 43,
    38, 53,
    38, 55,
    38, 56,
    38, 57,
    39, 13,
    39, 15,
    39, 34,
    39, 43,
    39, 52,
    39, 53,
    39, 66,
    39, 71,
    40, 53,
    40, 55,
    40, 90,
    43, 13,
    43, 15,
    43, 34,
    43, 43,
    43, 66,
    44, 13,
    44, 28,
    44, 32,
    44, 36,
    44, 40,
    44, 43,
    44, 48,
    44, 50,
    44, 53,
    44, 57,
    44, 59,
    44, 68,
    44, 69,
    44, 70,
    44, 72,
    44, 80,
    44, 82,
    44, 85,
    44, 87,
    44, 88,
    44, 90,
    45, 11,
    45, 32,
    45, 34,
    45, 36,
    45, 40,
    45, 43,
    45, 48,
    45, 50,
    45, 53,
    45, 54,
    45, 55,
    45, 56,
    45, 58,
    45, 59,
    45, 85,
    45, 87,
    45, 88,
    45, 90,
    48, 13,
    48, 15,
    48, 34,
    48, 43,
    48, 53,
    48, 57,
    48, 58,
    48, 59,
    49, 13,
    49, 15,
    49, 34,
    49, 40,
    49, 43,
    49, 56,
    49, 57,
    49, 66,
    49, 68,
    49, 69,
    49, 70,
    49, 72,
    49, 80,
    49, 82,
    50, 13,
    50, 15,
    50, 34,
    50, 53,
    50, 57,
    50, 58,
    50, 59,
    51, 28,
    51, 36,
    51, 40,
    51, 43,
    51, 48,
    51, 50,
    51, 53,
    51, 58,
    51, 68,
    51, 69,
    51, 70,
    51, 72,
    51, 80,
    51, 82,
    52, 85,
    52, 87,
    52, 88,
    52, 90,
    53, 13,
    53, 15,
    53, 27,
    53, 28,
    53, 34,
    53, 36,
    53, 40,
    53, 43,
    53, 48,
    53, 50,
    53, 53,
    53, 55,
    53, 56,
    53, 57,
    53, 58,
    53, 66,
    53, 68,
    53, 69,
    53, 70,
    53, 71,
    53, 72,
    53, 78,
    53, 79,
    53, 80,
    53, 81,
    53, 82,
    53, 83,
    53, 84,
    53, 86,
    53, 87,
    53, 88,
    53, 89,
    53, 90,
    53, 91,
    54, 34,
    54, 43,
    55, 13,
    55, 15,
    55, 27,
    55, 28,
    55, 32,
    55, 34,
    55, 36,
    55, 40,
    55, 43,
    55, 48,
    55, 50,
    55, 52,
    55, 53,
    55, 66,
    55, 68,
    55, 69,
    55, 70,
    55, 72,
    55, 78,
    55, 79,
    55, 80,
    55, 81,
    55, 82,
    55, 83,
    55, 84,
    55, 86,
    56, 13,
    56, 15,
    56, 27,
    56, 28,
    56, 34,
    56, 53,
    56, 66,
    56, 68,
    56, 69,
    56, 70,
    56, 72,
    56, 80,
    56, 82,
    57, 13,
    57, 15,
    57, 28,
    57, 36,
    57, 40,
    57, 43,
    57, 48,
    57, 50,
    57, 53,
    58, 13,
    58, 15,
    58, 34,
    58, 36,
    58, 40,
    58, 43,
    58, 48,
    58, 50,
    58, 52,
    58, 53,
    58, 66,
    58, 68,
    58, 69,
    58, 70,
    58, 71,
    58, 72,
    58, 78,
    58, 79,
    58, 80,
    58, 81,
    58, 82,
    58, 83,
    58, 84,
    58, 86,
    59, 43,
    59, 53,
    59, 90,
    60, 75,
    67, 66,
    67, 71,
    67, 89,
    68, 43,
    68, 53,
    68, 58,
    70, 3,
    70, 8,
    71, 10,
    71, 11,
    71, 13,
    71, 14,
    71, 15,
    71, 27,
    71, 28,
    71, 32,
    71, 62,
    71, 67,
    71, 73,
    71, 76,
    71, 77,
    71, 85,
    71, 87,
    71, 88,
    71, 89,
    71, 90,
    71, 94,
    72, 75,
    75, 75,
    76, 13,
    76, 14,
    76, 15,
    76, 27,
    76, 28,
    76, 68,
    76, 69,
    76, 70,
    76, 72,
    76, 80,
    76, 82,
    76, 85,
    76, 91,
    79, 3,
    79, 8,
    80, 3,
    80, 8,
    80, 66,
    80, 71,
    80, 75,
    80, 89,
    81, 66,
    81, 71,
    81, 89,
    82, 75,
    83, 13,
    83, 14,
    83, 15,
    83, 27,
    83, 28,
    83, 68,
    83, 69,
    83, 70,
    83, 71,
    83, 72,
    83, 73,
    83, 74,
    83, 78,
    83, 79,
    83, 80,
    83, 82,
    83, 84,
    83, 85,
    83, 86,
    83, 87,
    83, 88,
    83, 89,
    83, 90,
    83, 91,
    85, 14,
    85, 32,
    85, 68,
    85, 69,
    85, 70,
    85, 72,
    85, 80,
    85, 82,
    85, 89,
    86, 3,
    86, 8,
    87, 13,
    87, 15,
    87, 66,
    87, 68,
    87, 69,
    87, 70,
    87, 72,
    87, 80,
    87, 82,
    88, 13,
    88, 15,
    88, 68,
    88, 69,
    88, 70,
    88, 72,
    88, 80,
    88, 82,
    89, 68,
    89, 69,
    89, 70,
    89, 72,
    89, 80,
    89, 82,
    90, 3,
    90, 8,
    90, 13,
    90, 15,
    90, 32,
    90, 68,
    90, 69,
    90, 70,
    90, 71,
    90, 72,
    90, 80,
    90, 82,
    90, 85,
    92, 75
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -6, -8, -9, -15, 30, -21, -19, -13,
    -13, -13, -13, -13, -13, -19, 9, 9,
    -5, -3, 12, -5, -22, -5, -17, -10,
    -23, 4, -6, -7, -5, -6, -8, -8,
    3, -9, -9, -4, -7, -16, -16, -5,
    -11, -9, -6, 4, 8, 2, 1, 6,
    5, -22, -22, -17, -8, -3, 4, -10,
    2, -6, -3, -3, -13, -13, -9, -8,
    -3, 9, 9, 3, -9, -9, 9, -9,
    -9, 1, 6, 6, -3, -3, -3, -3,
    -3, -3, -8, -11, -8, -13, -31, -13,
    7, -8, -8, 9, -8, -8, -20, -6,
    -18, -11, -22, 9, -3, -14, -9, -11,
    -15, -12, -5, -3, -13, -7, -6, -6,
    -53, -47, -19, 1, -20, 5, -7, -8,
    -10, -10, -10, -10, -10, -10, -12, -13,
    -3, -13, -6, -4, -6, 12, -3, -3,
    8, -3, -3, -6, -4, -8, -8, -8,
    -8, -8, -8, -8, -6, -3, -8, -22,
    -28, -3, -3, -22, -11, -11, -20, -12,
    -12, 6, 9, 6, -1, 6, -27, -28,
    -28, -28, -13, -28, -22, -22, -28, -21,
    -28, -24, -23, -22, -13, -14, -22, -14,
    -13, -7, -5, -31, -31, -5, -5, 2,
    -16, -6, -6, -17, -1, -4, -3, 6,
    -23, -20, -20, -20, -19, -11, -10, -20,
    -11, -20, -11, -12, -9, -19, -19, -3,
    -3, -11, 5, -12, -8, -8, -8, -8,
    -8, -6, 9, 9, 9, -4, -4, 11,
    -4, -4, 6, -34, -34, -23, -8, -8,
    -17, -8, -8, -3, 6, -28, -28, -28,
    -28, -5, -28, -20, -20, -28, -21, -28,
    -20, -18, -20, 8, 6, -8, 27, -3,
    -1, -6, 11, -13, -10, -13, -21, 13,
    6, -16, -13, -16, 12, 12, 9, 13,
    4, 3, 1, 1, 6, 6, 6, 3,
    6, 9, 3, 5, 12, -18, 12, 12,
    12, -4, -3, -4, -4, -4, -3, -2,
    2, -13, -18, -18, -24, -3, -5, -1,
    -6, -3, -5, -6, 14, -25, -16, -25,
    12, 12, -1, -1, -1, 8, -1, 1,
    1, 1, 1, -1, -3, 2, 9, 1,
    12, 11, 8, 12, 6, -14, -12, -1,
    -1, -1, -1, -1, -1, 4, -8, -12,
    -19, -19, -5, -2, -2, -3, -3, -3,
    -3, -13, -13, -1, -1, -1, -1, -1,
    -1, -5, -5, -5, -5, -5, -5, 4,
    6, -17, -18, -1, -3, -3, -3, 2,
    -3, -3, -3, 1, 25
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 389,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t custom_yaheiBold_18 = {
#else
lv_font_t custom_yaheiBold_18 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 20,          /*The maximum line height required by the font*/
    .base_line = 5,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -4,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if CUSTOM_YAHEIBOLD_18*/

