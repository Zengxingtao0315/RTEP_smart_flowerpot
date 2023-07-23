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
void Paint::NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Rotate, UWORD Color)
{
    paint.Image = NULL;
    paint.Image = image;

    paint.WidthMemory = Width;
    paint.HeightMemory = Height;
    paint.Color = Color;    
	paint.Scale = 2;
		
    paint.WidthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
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
function: Select Image Rotate
parameter:
    Rotate : 0,90,180,270
******************************************************************************/

void Paint::SetScale(UBYTE scale)
{
    if(scale == 2){
        paint.Scale = scale;
        paint.WidthByte = (paint.WidthMemory % 8 == 0)? (paint.WidthMemory / 8 ): (paint.WidthMemory / 8 + 1);
    }else if(scale == 4){
        paint.Scale = scale;
        paint.WidthByte = (paint.WidthMemory % 4 == 0)? (paint.WidthMemory / 4 ): (paint.WidthMemory / 4 + 1);
    }else if(scale ==16) {
        paint.Scale = scale;
        paint.WidthByte = (paint.WidthMemory%2==0) ? (paint.WidthMemory/2) : (paint.WidthMemory/2+1); 
    }else if(scale ==65) {
        paint.Scale = scale;
       paint.WidthByte = paint.WidthMemory*2; 
    }else{
        Debug("Set Scale Input parameter error\r\n");
        Debug("Scale Only support: 2 4 16 65\r\n");
    }
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
    if(Xpoint > paint.Width || Ypoint > paint.Height){
        Debug("Exceeding display boundaries\r\n");
        return;
    }      
    UWORD X, Y;

	X = Xpoint;
	Y = Ypoint;  

    


    if(X > paint.WidthMemory || Y > paint.HeightMemory){
        Debug("Exceeding display boundaries\r\n");
        return;
    }
    
    if(paint.Scale == 2){
        UDOUBLE Addr = X / 8 + Y * paint.WidthByte;
        UBYTE Rdata = paint.Image[Addr];
        if(Color == BLACK)
            paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        else
            paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
    }else if(paint.Scale == 4){
        UDOUBLE Addr = X / 4 + Y * paint.WidthByte;
        Color = Color % 4;//Guaranteed color scale is 4  --- 0~3
        UBYTE Rdata = paint.Image[Addr];
        
        Rdata = Rdata & (~(0xC0 >> ((X % 4)*2)));
        paint.Image[Addr] = Rdata | ((Color << 6) >> ((X % 4)*2));
    }else if(paint.Scale == 16) {
        UDOUBLE Addr = X / 2 + Y * paint.WidthByte;
        UBYTE Rdata = paint.Image[Addr];
        Color = Color % 16;
        Rdata = Rdata & (~(0xf0 >> ((X % 2)*4)));
        paint.Image[Addr] = Rdata | ((Color << 4) >> ((X % 2)*4));
    }else if(paint.Scale == 65) {
        UDOUBLE Addr = X*2 + Y*paint.WidthByte;
        paint.Image[Addr] = 0xff & (Color>>8);
        paint.Image[Addr+1] = 0xff & Color;
    }

}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void Paint::Clear(UWORD Color)
{
    if(paint.Scale == 2 || paint.Scale == 4) {
        for (UWORD Y = 0; Y < paint.HeightByte; Y++) {
            for (UWORD X = 0; X < paint.WidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X + Y*paint.WidthByte;
                paint.Image[Addr] = Color;
            }
        }
    }else if(paint.Scale == 16) {
        for (UWORD Y = 0; Y < paint.HeightByte; Y++) {
            for (UWORD X = 0; X < paint.WidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X + Y*paint.WidthByte;
                Color = Color & 0x0f;
                paint.Image[Addr] = (Color<<4) | Color;
            }
        }
    }else if(paint.Scale == 65) {
        for (UWORD Y = 0; Y < paint.HeightByte; Y++) {
            for (UWORD X = 0; X < paint.WidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X*2 + Y*paint.WidthByte;
                paint.Image[Addr] = 0x0f & (Color>>8);
                paint.Image[Addr+1] = 0x0f & Color;
            }
        }
    }
}

/******************************************************************************
function: Clear the color of a window
parameter:
    Xstart : x starting point
    Ystart : Y starting point
    Xend   : x end point
    Yend   : y end point
    Color  : Painted colors
******************************************************************************/
void Paint::ClearWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color)
{
    UWORD X, Y;
    for (Y = Ystart; Y < Yend; Y++) {
        for (X = Xstart; X < Xend; X++) {//8 pixel =  1 byte
            SetPixel(X, Y, Color);
        }
    }
}

/******************************************************************************
function: Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		: The Xpoint coordinate of the point
    Ypoint		: The Ypoint coordinate of the point
    Color		: Painted color
    Dot_Pixel	: point size
    Dot_Style	: point Style
******************************************************************************/
void Paint::DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color,
                     DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style)
{
    if (Xpoint > paint.Width || Ypoint > paint.Height) {
        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
				std::cout << "Xpoint = " << Xpoint << " , paint.Width = " << paint.Width << std::endl;
				std::cout << "Ypoint = " << Ypoint << " , paint.Height = " << paint.Height << std::endl;
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (Dot_Style == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
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
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
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
void Paint::DrawBitMap(const unsigned char* image_buffer)
{
    UWORD x, y;
    UDOUBLE Addr = 0;

    for (y = 0; y < paint.HeightByte; y++) {
        for (x = 0; x < paint.WidthByte; x++) {//8 pixel =  1 byte
            Addr = x + y * paint.WidthByte;
            paint.Image[Addr] = (unsigned char)image_buffer[Addr];
        }
    }
}

void Paint::DrawBitMap_Block(const unsigned char* image_buffer, UBYTE Region)
{
    UWORD x, y;
    UDOUBLE Addr = 0;
		for (y = 0; y < paint.HeightByte; y++) {
				for (x = 0; x < paint.WidthByte; x++) {//8 pixel =  1 byte
						Addr = x + y * paint.WidthByte ;
						paint.Image[Addr] = \
						(unsigned char)image_buffer[Addr+ (paint.HeightByte)*paint.WidthByte*(Region - 1)];
				}
		}
}

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