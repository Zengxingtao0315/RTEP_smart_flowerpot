/*****************************************************************************
This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/tree/master/RaspberryPi/user_dev/c/lib/Config
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
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
#include <mutex>
#include <thread>

using namespace std;

/**
 * Function: Initialize a new image for painting.
 * 
 * param image - Pointer to the image data.
 * param Width - Width of the image.
 * param Height - Height of the image.
 * param Color - Color information.
 */
void Paint::NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Color)
{
    paint.Image = NULL;
    paint.Image = image;

    paint.WidthMemory = Width;
    paint.HeightMemory = Height;
    paint.Color = Color;    
		
    paint.WidthByte = paint.WidthMemory*2; 
    paint.HeightByte = Height;    
   
    paint.Width = Width;
    paint.Height = Height;
    
}

/**
 * Function: set image from cache

 */
void Paint::SelectImage(UBYTE *image)
{
    paint.Image = image;
}


/**
 * Function: Set a pixel at specified coordinates with the specified color.
 * 
 * param Xpoint - X coordinate of the pixel.
 * param Ypoint - Y coordinate of the pixel.
 * param Color - Color information.
 */
void Paint::SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
{
    
    // Check if the specified coordinates are within the display boundaries
    if (Xpoint >= paint.Width || Ypoint >= paint.Height) {
        Debug("Exceeding display boundaries\r\n");
        return;
    }

    // Calculate the memory address (index) in the display buffer for the specified pixel
    UDOUBLE Addr = Xpoint * 2 + Ypoint * paint.WidthByte;

    
    paint.Image[Addr] = 0xFF & (Color >> 8);   // Store the high byte of the color
    paint.Image[Addr + 1] = 0xFF & Color;      // Store the low byte of the color
    
}



/**
 * Function: clear the color
 * 

 */
void Paint::Clear(UWORD Color)
{
    // Extract the low byte and high byte of the specified color
    UBYTE lowByte = 0x0F & Color;
    UBYTE highByte = 0x0F & (Color >> 8);

    // Get a pointer to the start of the display buffer (paint.Image)
    UBYTE* image = paint.Image;

    // Calculate the size of the display buffer in bytes
    UWORD imageByteSize = paint.WidthByte * paint.HeightByte * 2;



    // Step 1: Fill the low byte of the color into the buffer
    std::memset(image, lowByte, imageByteSize);

    // Step 2: Fill the high byte of the color into the buffer
    std::memset(image + 1, highByte, imageByteSize);
}



/**
 * Function: Draw a character on the display.
 * 
 * param Xpoint - X coordinate of the starting point for the character.
 * param Ypoint - Y coordinate of the starting point for the character.
 * param Acsii_Char - ASCII character to be displayed.
 * param Font - Pointer to the font information.
 * param Color_Foreground - Color for the character.
 * param Color_Background - Background color.
 */

void Paint::DrawChar(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char,
                     sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    if (Xpoint >= paint.Width || Ypoint >= paint.Height) {
        Debug("Paint_DrawChar: Input is outside the normal display range\r\n");
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


/**
 * Function: Draw a string on the display.
 * 
 * param Xstart - X coordinate of the starting point for the string.
 * param Ystart - Y coordinate of the starting point for the string.
 * param pString - Pointer to the string to be displayed.
 * param Font - Pointer to the font information.
 * param Color_Foreground - Color for the characters.
 * param Color_Background - Background color.
 */

void Paint::DrawString(UWORD Xstart, UWORD Ystart, const char *pString,
                       sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    // Loop through the characters in the input string until the null terminator is reached
    for (; *pString != '\0'; pString++) {
        // Check if the current Xpoint + the character's width exceeds the screen width
        // If so, reset Xpoint to the starting X-coordinate and move Ypoint down by the character's height
        if (Xpoint + Font->Width > paint.Width) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // Check if the current Ypoint + the character's height exceeds the screen height
        // If so, break out of the loop as the screen area is full
        if (Ypoint + Font->Height > paint.Height) {
            break;
        }

        // Call the DrawChar functions
        DrawChar(Xpoint, Ypoint, *pString, Font, Color_Background, Color_Foreground);

        // Move the Xpoint to the right by the character's width to position the next character
        Xpoint += Font->Width;
    }
}




/**
 * Function: Draw a number on the display.
 * 
 * param Xpoint - X coordinate of the starting point for the number.
 * param Ypoint - Y coordinate of the starting point for the number.
 * param Number - The number to be displayed.
 * param Font - Pointer to the font information.
 * param Digit - Number of digits after the decimal point.
 * param Color_Foreground - Color for the characters.
 * param Color_Background - Background color.
 */
const int  ARRAY_LEN = 255;

void Paint::DrawNum(UWORD Xpoint, UWORD Ypoint, double Number,
                    sFONT* Font, UWORD Digit, UWORD Color_Foreground, UWORD Color_Background)
{
    // Create a character array to store the formatted number string
    char str[ARRAY_LEN];

    
    snprintf(str, ARRAY_LEN, "%.*f", Digit, Number);

    // Call the DrawString function
    DrawString(Xpoint, Ypoint, str, Font, Color_Background, Color_Foreground);
}

/**
 * Function: Draw a time 
 *
 * param Xstart            X coordinate of the starting point
 * param Ystart            Y coordinate of the starting point
 * param pTime             Pointer to the PAINT_TIME structure containing Hour, Min, and Sec
 * param Font              Pointer to the font structure
 * param Color_Foreground  Foreground color
 * param Color_Background  Background color
 */

void Paint::DrawTime(UWORD Xstart, UWORD Ystart, PAINT_TIME* pTime, sFONT* Font,
                     UWORD Color_Foreground, UWORD Color_Background)
{
    char timeStr[9]; // 8 characters + null-terminator

    // Format the time string in "HH:MM:SS" format using snprintf
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", pTime->Hour, pTime->Min, pTime->Sec);

    // Draw the time string using DrawString function
    DrawString(Xstart, Ystart, timeStr, Font, Color_Foreground, Color_Background);
}


/**
 * Function: Read and display a 65K-color BMP image.
 *
 * param path       Path to the BMP file
 * param Xstart     X coordinate for starting point
 * param Ystart     Y coordinate for starting point
 * return           0 if successful, 1 if unable to open file, 2 if not a 65K-color bitmap
 */

UBYTE Paint::GUI_ReadBmp(const char *path, UWORD Xstart, UWORD Ystart)
{
    
    // Open the BMP file in binary read mode
    
    FILE *fp = fopen(path, "rb");
     
    if (!fp) {
        Debug("Error: Unable to open file for reading.\n");
        return 1;
    }

    // Read the BMP file header and info header
    BMPFILEHEADER bmpFileHeader;
    BMPINFOHEADER bmpInfoHeader;
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);  // Read BMP file header
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);  // Read the BMP info header

    // Check if the BMP image is a 65K-color bitmap (16-bit color depth)
    if (bmpInfoHeader.biBitCount != 16) {
        Debug("Error: Not a 65K-color bitmap.\n");
        fclose(fp);
        return 2;
    }

    // Calculate the number of bytes per row of the image
    UWORD Image_Width_Byte = bmpInfoHeader.biWidth * 2;

    // Allocate memory to store the image data in an array
    UBYTE Image[Image_Width_Byte * bmpInfoHeader.biHeight];
    memset(Image, 0xFF, Image_Width_Byte * bmpInfoHeader.biHeight);

    // Move the file pointer to the start of the image data
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);

    // Read the image data line by line, starting from the bottom line (due to BMP storage format)
    for (int y = bmpInfoHeader.biHeight - 1; y >= 0; y--) {
        fread(Image + (bmpInfoHeader.biHeight - 1 - y) * Image_Width_Byte, sizeof(UBYTE), Image_Width_Byte, fp);
    }

   
    fclose(fp);

    
    UWORD color;

    // Loop through the image and update the display buffer
    for (UWORD y = 0; y < bmpInfoHeader.biHeight; y++) {
        for (UWORD x = 0; x < bmpInfoHeader.biWidth; x++) {
            // If the current pixel is outside the screen size, break out of the loop
            if (x >= paint.Width || y >= paint.Height) {
                break;
            }
            // Extract the color value from the image data (16-bit color depth)
            color = Image[x * 2 + (bmpInfoHeader.biHeight - 1 - y) * Image_Width_Byte];
            color |= Image[x * 2 + (bmpInfoHeader.biHeight - 1 - y) * Image_Width_Byte + 1] << 8;

            // Update the pixel at the specified position (Xstart + x, Ystart + y) with the color value
            SetPixel(Xstart + x, Ystart + y, color);  // Refresh image to display buffer
        }
    }
    
   
    return 0;
}
