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
    if (Xpoint >= paint.Width || Ypoint >= paint.Height) {
        Debug("Paint_DrawChar: 输入超出正常显示范围\r\n");
        return;
    }

    // Calculate the character offset in the font table
    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char* ptr = &Font->table[Char_Offset];

    // Draw characters line by line
    for (UWORD Page = 0; Page < Font->Height; Page++) {
        // Pixel-by-pixel data processing
        for (UWORD Column = 0; Column < Font->Width; Column++) {
            // Determine if the current pixel is a foreground color pixel
            bool drawForeground = *ptr & (0x80 >> (Column % 8));
            UWORD color = drawForeground ? Color_Foreground : Color_Background;
            SetPixel(Xpoint + Column, Ypoint + Page, color);

            // Each pixel occupies 1 bit, so one byte is processed per 8 columns
            if (Column % 8 == 7)
                ptr++;
        }

        // Consider the case where each pixel occupies 1 bit and is not an integer multiple of 8
        if (Font->Width % 8 != 0)
            ptr++;
    }
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
void Paint::DrawString(UWORD Xstart, UWORD Ystart, const char *pString,
                         sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    for (; *pString != '\0'; pString++) {
        if (Xpoint + Font->Width > paint.Width) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }
        if (Ypoint + Font->Height > paint.Height) {
            break; // if the showearea is full, so break
        }
        DrawChar(Xpoint, Ypoint, *pString, Font, Color_Background, Color_Foreground);
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

void Paint::DrawNum(UWORD Xpoint, UWORD Ypoint, double Number,
                   sFONT* Font, UWORD Digit, UWORD Color_Foreground, UWORD Color_Background)
{
    char str[ARRAY_LEN];
    snprintf(str, ARRAY_LEN, "%.*f", Digit, Number);
    DrawString(Xpoint, Ypoint, str, Font, Color_Background, Color_Foreground);
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
void Paint::DrawTime(UWORD Xstart, UWORD Ystart, PAINT_TIME* pTime, sFONT* Font,
                     UWORD Color_Foreground, UWORD Color_Background)
{
    char timeStr[9]; // 8 characters + null-terminator

    // Format the time string in "HH:MM:SS" format using snprintf
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", pTime->Hour, pTime->Min, pTime->Sec);

    // Draw the time string using DrawString function
    DrawString(Xstart, Ystart, timeStr, Font, Color_Foreground, Color_Background);
}


/******************************************************************************
function:	Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/

UBYTE Paint::GUI_ReadBmp(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp = fopen(path, "rb");  
    if (!fp) {
        Debug("Cann't open the file!\n");
        return 1;
    }

    BMPFILEHEADER bmpFileHeader;
    BMPINFOHEADER bmpInfoHeader;
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);  // Read BMP file header
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);  // Read the BMP message header

    if (bmpInfoHeader.biBitCount != 16) {
        Debug("Bmp image is not a 65K-color bitmap!\n");
        fclose(fp);
        return 2;
    }

    UWORD Image_Width_Byte = bmpInfoHeader.biWidth * 2;
    UBYTE Image[Image_Width_Byte * bmpInfoHeader.biHeight];
    memset(Image, 0xFF, Image_Width_Byte * bmpInfoHeader.biHeight);

    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);

    // Read image data line by line starting from the bottom line
    for (int y = bmpInfoHeader.biHeight - 1; y >= 0; y--) {
        fread(Image + (bmpInfoHeader.biHeight - 1 - y) * Image_Width_Byte, sizeof(UBYTE), Image_Width_Byte, fp);
    }

    fclose(fp);

    UWORD color;
    for (UWORD y = 0; y < bmpInfoHeader.biHeight; y++) {
        for (UWORD x = 0; x < bmpInfoHeader.biWidth; x++) {
            if (x > paint.Width || y > paint.Height) {
                break;
            }
            color = Image[x * 2 + (bmpInfoHeader.biHeight - 1 - y) * Image_Width_Byte];
            color |= Image[x * 2 + (bmpInfoHeader.biHeight - 1 - y) * Image_Width_Byte + 1] << 8;
            SetPixel(Xstart + x, Ystart + y, color);  // Refresh image to display buffer
        }
    }
    return 0;
}
