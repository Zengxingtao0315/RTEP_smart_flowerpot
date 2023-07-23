/******************************************************************************
* | File      	:   GUI_mc
* | Author      :   Xingtao Zeng
* | Function    :	Achieve drawing: draw points, lines, boxes, circles and
*                   their size, solid dotted line, solid rectangle hollow
*                   rectangle, solid circle hollow circle.
* | Info        :

******************************************************************************/
#include "GUI_Paint.hpp"

#include "DEV_Config.hpp"
#include "Debug.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring> //memset()
#include <cmath>
#include <ctime>
#include <iostream>

PAINT paint;
using namespace std;
/******************************************************************************
function: Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void Paint::NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Color)
{
    paint.Image = NULL;
    paint.Image = image;

    paint.WidthMemory = Width;
    paint.HeightMemory = Height;
    paint.Color = Color;    
		
    paint.WidthByte = paint.WidthMemory*2; 
    paint.HeightByte = Height;    
//    printf("WidthByte = %d, HeightByte = %d\r\n", Paint.WidthByte, Paint.HeightByte);
//    printf(" EPD_WIDTH / 8 = %d\r\n",  122 / 8);
   
    
    paint.Width = Width;
    paint.Height = Height;
    
}

/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void Paint::SelectImage(UBYTE *image)
{
    paint.Image = image;
}


/******************************************************************************
function: Draw Pixels
parameter:
    Xpoint : At point X
    Ypoint : At point Y
    Color  : Painted colors
******************************************************************************/
void Paint::SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
{
    if (Xpoint >= paint.Width || Ypoint >= paint.Height) {
        Debug("Exceeding display boundaries\r\n");
        return;
    }

    UDOUBLE Addr = Xpoint * 2 + Ypoint * paint.WidthByte;
    paint.Image[Addr] = 0xFF & (Color >> 8);
    paint.Image[Addr + 1] = 0xFF & Color;
}


/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void Paint::Clear(UWORD Color)
{
    UBYTE lowByte = 0x0F & Color;
    UBYTE highByte = 0x0F & (Color >> 8);

    UBYTE* image = paint.Image;
    UWORD imageByteSize = paint.WidthByte * paint.HeightByte * 2;

    std::memset(image, lowByte, imageByteSize);
    std::memset(image + 1, highByte, imageByteSize);
}


/******************************************************************************
function: Show English characters
parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::DrawChar(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char,
                    sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Page, Column;

    if (Xpoint > paint.Width || Ypoint > paint.Height) {
        Debug("Paint_DrawChar Input exceeds the normal display range\r\n");
        return;
    }

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> (Column % 8)))
                    SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                   
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    
                } else {
                    SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
               
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
    }// Write all
}

/******************************************************************************
function:	Display the string
parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::DrawString_EN(UWORD Xstart, UWORD Ystart, const char * pString,
                         sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    if (Xstart > paint.Width || Ystart > paint.Height) {
        Debug("Paint_DrawString_EN Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > paint.Width ) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height ) > paint.Height ) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        DrawChar(Xpoint, Ypoint, * pString, Font, Color_Background, Color_Foreground);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}


/******************************************************************************
function:	Display nummber
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
	Digit						 : Fractional width
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
#define  ARRAY_LEN 255
void Paint::DrawNum(UWORD Xpoint, UWORD Ypoint, double Nummber,
                   sFONT* Font, UWORD Digit,UWORD Color_Foreground, UWORD Color_Background)
{
    int16_t Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;
	int temp = Nummber;
	float decimals;
	uint8_t i;
    if (Xpoint > paint.Width || Ypoint > paint.Height) {
        Debug("Paint_DisNum Input exceeds the normal display range\r\n");
        return;
    }

	if(Digit > 0) {		
		decimals = Nummber - temp;
		for(i=Digit; i > 0; i--) {
			decimals*=10;
		}
		temp = decimals;
		//Converts a number to a string
		for(i=Digit; i>0; i--) {
			Num_Array[Num_Bit] = temp % 10 + '0';
			Num_Bit++;
			temp /= 10;						
		}	
		Num_Array[Num_Bit] = '.';
		Num_Bit++;
	}

	temp = Nummber;
    //Converts a number to a string
    while (temp) {
        Num_Array[Num_Bit] = temp % 10 + '0';
        Num_Bit++;
        temp /= 10;
    }
		
    //The string is inverted
    while (Num_Bit > 0) {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit ++;
        Num_Bit --;
    }

    //show
    DrawString_EN(Xpoint, Ypoint, (const char*)pStr, Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display time
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    pTime            : Time-related structures
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::DrawTime(UWORD Xstart, UWORD Ystart, PAINT_TIME *pTime, sFONT* Font,
                    UWORD Color_Foreground, UWORD Color_Background)
{
    uint8_t value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    UWORD Dx = Font->Width;

    //Write data into the cache
    DrawChar(Xstart                           , Ystart, value[pTime->Hour / 10], Font, Color_Background, Color_Foreground);
    DrawChar(Xstart + Dx                      , Ystart, value[pTime->Hour % 10], Font, Color_Background, Color_Foreground);
    DrawChar(Xstart + Dx  + Dx / 4 + Dx / 2   , Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    DrawChar(Xstart + Dx * 2 + Dx / 2         , Ystart, value[pTime->Min / 10] , Font, Color_Background, Color_Foreground);
    DrawChar(Xstart + Dx * 3 + Dx / 2         , Ystart, value[pTime->Min % 10] , Font, Color_Background, Color_Foreground);
    DrawChar(Xstart + Dx * 4 + Dx / 2 - Dx / 4, Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    DrawChar(Xstart + Dx * 5                  , Ystart, value[pTime->Sec / 10] , Font, Color_Background, Color_Foreground);
    DrawChar(Xstart + Dx * 6                  , Ystart, value[pTime->Sec % 10] , Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/

UBYTE Paint::GUI_ReadBmp_65K(const char *path, UWORD Xstart, UWORD Ystart)
{
	FILE *fp;                     //Define a file pointer
	BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
	BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure
	
	// Binary file open
	if((fp = fopen(path, "rb")) == NULL) {
		Debug("Cann't open the file!\n");
		std::exit(0);
	}

	// Set the file pointer from the beginning
	fseek(fp, 0, SEEK_SET);
	fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
	fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
	std::cout << "pixel = " << bmpInfoHeader.biWidth << " * " << bmpInfoHeader.biHeight << std::endl;

	UWORD Image_Width_Byte = bmpInfoHeader.biWidth * 2;
	UWORD Bmp_Width_Byte = bmpInfoHeader.biWidth * 2;
	UBYTE Image[Image_Width_Byte * bmpInfoHeader.biHeight];
	memset(Image, 0xFF, Image_Width_Byte * bmpInfoHeader.biHeight);

	// Determine if it is a monochrome bitmap
	int readbyte = bmpInfoHeader.biBitCount;
	std::cout << "biBitCount = " << readbyte << std::endl;
	if(readbyte != 16){
		Debug("Bmp image is not a 65K-color bitmap!\n");
		std::exit(0);
	}
	// Read image data into the cache
	UWORD x, y;
	UBYTE Rdata;
	fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
	
	for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
		for(x = 0; x < Bmp_Width_Byte; x++) {//Show a line in the line
			if(fread((char *)&Rdata, 1, 1, fp) != 1) {
				std::cerr << "get bmpdata:" << std::endl;
				break;
			}
			Image[x + (bmpInfoHeader.biHeight-1 - y)*Image_Width_Byte] =  Rdata;
		}
	}
	fclose(fp);
	
	// Refresh the image to the display buffer based on the displayed orientation
	UWORD color;
	std::cout << "bmpInfoHeader.biWidth  = " << bmpInfoHeader.biWidth << std::endl;
	std::cout << "bmpInfoHeader.biHeight  = " << bmpInfoHeader.biHeight << std::endl;
	for(y = 0; y < bmpInfoHeader.biHeight; y++) {
		for(x = 0; x < bmpInfoHeader.biWidth; x++) {
			if(x > paint.Width || y > paint.Height) {
				break;
			}
			color = 0;
			color |= Image[x*2 + y*bmpInfoHeader.biWidth*2];
			color |= Image[x*2 + y*bmpInfoHeader.biWidth*2 + 1] << 8;
			SetPixel(Xstart + x, Ystart + y, color);
		}
	}
	return 0;
}