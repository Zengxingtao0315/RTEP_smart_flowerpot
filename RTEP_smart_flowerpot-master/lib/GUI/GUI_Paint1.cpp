/******************************************************************************
* | File      	:   GUI
* | Author      :   reference from 
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
   
    paint.Width = Height;
    paint.Height = Width;
}

void Paint::SelectImage(UBYTE *image)
{
    paint.Image = image;
}



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


void Paint::DrawChar(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char,
                     sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    // Ensure the character is within the font's ASCII range
    if (Acsii_Char < Font->StartChar || Acsii_Char > Font->EndChar) {
        Debug("Paint_DrawChar Character is out of font range\r\n");
        return;
    }

    // Get the character index in the font table
    uint16_t Char_Index = Acsii_Char - Font->StartChar;

    // Calculate the starting address of the character's bitmap in the font table
    const uint8_t* Char_Bitmap = &Font->table[Char_Index * Font->CharBytes];

    for (UWORD Page = 0; Page < Font->Height; Page++) {
        for (UWORD Column = 0; Column < Font->Width; Column++) {
            // Get the byte containing the pixel data for the current column
            uint8_t pixelByte = Char_Bitmap[Column + (Page * ((Font->Width + 7) / 8))];

            // Check each bit of the byte to determine the pixel color
            for (int8_t bit = 7; bit >= 0; bit--) {
                UWORD X = Xpoint + Column * 8 + (7 - bit);
                UWORD Y = Ypoint + Page;

                if ((pixelByte >> bit) & 0x01) {
                    SetPixel(X, Y, Color_Foreground);
                } else {
                    SetPixel(X, Y, Color_Background);
                }
            }
        }
    }
}


void Paint::DrawString(UWORD Xstart, UWORD Ystart, const char * pString,
                         sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    if (Xstart > paint.Width || Ystart > paint.Height) {
        Debug("Paint_DrawString Input exceeds the normal display range\r\n");
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



#define  ARRAY_LEN 255



void Paint::DrawNum(UWORD Xpoint, UWORD Ypoint, double Number,
                    sFONT* Font, UWORD Digit, UWORD Color_Foreground, UWORD Color_Background)
{
    if (Xpoint > paint.Width || Ypoint > paint.Height) {
        Debug("Paint_DisNum Input exceeds the normal display range\r\n");
        return;
    }

    std::string strNum = std::to_string(Number);

    // Find the decimal point position
    size_t decimalPos = strNum.find(".");
    if (decimalPos == std::string::npos) {
        decimalPos = strNum.length();
    }

    // Limit the number of digits after the decimal point
    size_t numDigits = strNum.length() - decimalPos - 1;
    if (Digit < numDigits) {
        strNum = strNum.substr(0, decimalPos + Digit + 1);
    }

    // Show the formatted number
    DrawString(Xpoint, Ypoint, strNum.c_str(), Font, Color_Background, Color_Foreground);
}


void Paint::DrawTime(UWORD Xstart, UWORD Ystart, PAINT_TIME* pTime, sFONT* Font,
                     UWORD Color_Foreground, UWORD Color_Background)
{
    if (Xstart > paint.Width || Ystart > paint.Height) {
        Debug("Paint_DrawTime Input exceeds the normal display range\r\n");
        return;
    }

    // Format the time into a string with leading zeros
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << pTime->Hour << ":"
        << std::setfill('0') << std::setw(2) << pTime->Min << ":"
        << std::setfill('0') << std::setw(2) << pTime->Sec;

    std::string timeStr = oss.str();

    // Initialize variables for X and Y coordinates and font width
    UWORD Dx = Font->Width;
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    // Loop through each character in the time string
    for (size_t i = 0; i < timeStr.length(); i++) {
        char currentChar = timeStr[i];

        switch (currentChar) {
            case ':':
                // Add extra spacing for the colon character ':'
                Xpoint += Dx / 4 + Dx / 2;
                break;

            default:
                // Draw the character using the DrawChar function
                DrawChar(Xpoint, Ypoint, currentChar, Font, Color_Background, Color_Foreground);
                Xpoint += Dx;
                break;
        }
    }
}



UBYTE Paint::GUI_ReadBmp(const char* path, UWORD Xstart, UWORD Ystart)
{
    std::ifstream file(path, std::ios::binary); // Define an input file stream

    if (!file.is_open()) {
        Debug("Cannot open the file!\n");
        return 1;
    }

    BMPFILEHEADER bmpFileHeader; // Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader; // Define a bmp info header structure

    // Read the bmp file header and info header
    file.read(reinterpret_cast<char*>(&bmpFileHeader), sizeof(BMPFILEHEADER));
    file.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(BMPINFOHEADER));

    std::cout << "pixel = " << bmpInfoHeader.biWidth << " * " << bmpInfoHeader.biHeight << std::endl;

    if (bmpInfoHeader.biBitCount != 16) {
        Debug("Bmp image is not a 65K-color bitmap!\n");
        return 2;
    }

    UWORD Image_Width_Byte = bmpInfoHeader.biWidth * 2;
    UWORD Bmp_Width_Byte = bmpInfoHeader.biWidth * 2;
    std::vector<UBYTE> Image(Image_Width_Byte * bmpInfoHeader.biHeight, 0xFF);

    // Read image data into the cache
    file.seekg(bmpFileHeader.bOffset, std::ios::beg);

    for (UWORD y = 0; y < bmpInfoHeader.biHeight; y++) { // Total display column
        file.read(reinterpret_cast<char*>(Image.data() + (bmpInfoHeader.biHeight - 1 - y) * Image_Width_Byte), Bmp_Width_Byte);
    }

    // Refresh the image to the display buffer based on the displayed orientation
    UWORD color;
    for (UWORD y = 0; y < bmpInfoHeader.biHeight; y++) {
        for (UWORD x = 0; x < bmpInfoHeader.biWidth; x++) {
            if (x > paint.Width || y > paint.Height) {
                break;
            }

            color = 0;
            color |= Image[x * 2 + y * bmpInfoHeader.biWidth * 2];
            color |= Image[x * 2 + y * bmpInfoHeader.biWidth * 2 + 1] << 8;
            SetPixel(Xstart + x, Ystart + y, color);
        }
    }

    return 0;
}
