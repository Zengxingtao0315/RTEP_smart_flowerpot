/*****************************************************************************
* | File      	:   GUI_BMPfile.h
* | Author      :   Xingtao Zeng
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability

******************************************************************************/
#ifndef __GUI_BMPFILE_H
#define __GUI_BMPFILE_H

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>

extern "C" {
    #include "DEV_Config.h"
	#include "Debug.h"
}

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
class BMP {
	public:
		void BMP::open(const char *path);
		bool BMP::is_65K_color();
		void BMP::readImage();
		void BMP::refresh(UWORD Xstart, UWORD Ystart);
	private:
		FILE *fp;                     //Define a file pointer
		BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
		BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure
		UWORD Image_Width_Byte;
		UWORD Bmp_Width_Byte;
		UBYTE *Image;
		Paint Paint;

};

#endif
