/******************************************************************************
* | File      	:   GUI_Paint.h
* | Author      :   Xingtao Zeng
* | Function    :	Achieve drawing: draw points, lines, boxes, circles and
*                   their size, solid dotted line, solid rectangle hollow
*                   rectangle, solid circle hollow circle.
* | Info        :

******************************************************************************/
#ifndef __GUI_PAINT_HPP_
#define __GUI_PAINT_HPP_


#include "../Config/DEV_Config.hpp"
#include "../Fonts/fonts.hpp"

/*Bitmap file header   14bit*/
typedef struct BMP_FILE_HEADER {
    UWORD bType;        //File identifier
    UDOUBLE bSize;      //The size of the file
    UWORD bReserved1;   //Reserved value, must be set to 0
    UWORD bReserved2;   //Reserved value, must be set to 0
    UDOUBLE bOffset;    //The offset from the beginning of the file header to the beginning of the image data bit
} __attribute__ ((packed)) BMPFILEHEADER;    // 14bit

/*Bitmap information header  40bit*/
typedef struct BMP_INFO {
    UDOUBLE biInfoSize;      //The size of the header
    UDOUBLE biWidth;         //The width of the image
    UDOUBLE biHeight;        //The height of the image
    UWORD biPlanes;          //The number of planes in the image
    UWORD biBitCount;        //The number of bits per pixel
    UDOUBLE biCompression;   //Compression type
    UDOUBLE bimpImageSize;   //The size of the image, in bytes
    UDOUBLE biXPelsPerMeter; //Horizontal resolution
    UDOUBLE biYPelsPerMeter; //Vertical resolution
    UDOUBLE biClrUsed;       //The number of colors used
    UDOUBLE biClrImportant;  //The number of important colors
} __attribute__ ((packed)) BMPINFOHEADER;

/*Color table: palette */
typedef struct RGB_QUAD {
    UBYTE rgbBlue;               //Blue intensity
    UBYTE rgbGreen;              //Green strength
    UBYTE rgbRed;                //Red intensity
    UBYTE rgbReversed;           //Reserved value
} __attribute__ ((packed)) BMPRGBQUAD;
/**************************************** end ***********************************************/

/**
 * Image attributes
**/
typedef struct {
    UBYTE *Image;
    UWORD Width;
    UWORD Height;
    UWORD WidthMemory;
    UWORD HeightMemory;
    UWORD Color;
    UWORD WidthByte;
    UWORD HeightByte;
} PAINT;
extern PAINT paint;

/**
 * image color
**/
#define TIME_COLOR     0x1D34
#define WHITE          0xFFFF
#define BLACK          0x0000


#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE



/**
 * Whether the graphic is filled
**/
typedef enum {
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
} DRAW_FILL;

/**
 * Custom structure of a time attribute
**/
typedef struct {
    UWORD	Year;  //0000
    UBYTE Month; //1 - 12
    UBYTE Day;   //1 - 30
    UBYTE Hour;  //0 - 23
    UBYTE Min;   //0 - 59
    UBYTE Sec;   //0 - 59
} PAINT_TIME;
extern PAINT_TIME sPaint_time;

class Paint {
public:

	void NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Color);
	void SelectImage(UBYTE *image);
	void SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color);


	void Clear(UWORD Color);
	//Display string
	void DrawChar(UWORD Xstart, UWORD Ystart, const char Acsii_Char, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);
	void DrawString(UWORD Xstart, UWORD Ystart, const char * pString, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);
	void DrawNum(UWORD Xpoint, UWORD Ypoint, double Nummber, sFONT* Font, UWORD Digit,UWORD Color_Foreground, UWORD Color_Background);
	void DrawTime(UWORD Xstart, UWORD Ystart, PAINT_TIME *pTime, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background);

	//pic
	UBYTE GUI_ReadBmp(const char *path, UWORD Xstart, UWORD Ystart);

};



#endif





