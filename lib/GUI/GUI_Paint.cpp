/******************************************************************************
* | File      	:   GUI_mc
* | Author      :   Xingtao Zeng
* | Function    :	Achieve drawing: draw points, lines, boxes, circles and
*                   their size, solid dotted line, solid rectangle hollow
*                   rectangle, solid circle hollow circle.
* | Info        :

******************************************************************************/
#include "GUI_Paint.h"
extern "C" {
    #include "DEV_Config.h"
	#include "Debug.h"
}
#include <cstdint>
#include <cstdlib>
#include <cstring> //memset()
#include <cmath>
#include <ctime>

Paint::Paint() {
	mImage = paint.Image;
    mWidthMemory = paint.WidthMemory;
    mHeightMemory = paint.HeightMemory;
    mColor = BLACK;
    mScale = 2;
    mWidth = paint.Width;
    mHeight = paint.Height;
    mRotate = ROTATE_0;
    mMirror = MIRROR_NONE;
    mWidthByte = paint.WidthByte;
	mHeightByte = paint.HeightByte;
}

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
    mImage = NULL;
    mImage = image;

    mWidthMemory = Width;
    mHeightMemory = Height;
    mColor = Color;    
	mScale = 2;
		
    mWidthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
    mHeightByte = Height;    
//    printf("WidthByte = %d, HeightByte = %d\r\n", mWidthByte, mHeightByte);
//    printf(" EPD_WIDTH / 8 = %d\r\n",  122 / 8);
   
    mRotate = Rotate;
    mMirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        mWidth = Width;
        mHeight = Height;
    } else {
        mWidth = Height;
        mHeight = Width;
    }
}

/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void Paint::SelectImage(UBYTE *image)
{
    mImage = image;
}

/******************************************************************************
function: Select Image Rotate
parameter:
    Rotate : 0,90,180,270
******************************************************************************/
void Paint::SetRotate(UWORD Rotate)
{
    if(Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
        Debug("Set image Rotate %d\r\n", Rotate);
        mRotate = Rotate;
    } else {
        Debug("rotate = 0, 90, 180, 270\r\n");
    }
}

void Paint::SetScale(UBYTE scale)
{
    if(scale == 2){
        mScale = scale;
        mWidthByte = (mWidthMemory % 8 == 0)? (mWidthMemory / 8 ): (mWidthMemory / 8 + 1);
    }else if(scale == 4){
        mScale = scale;
        mWidthByte = (mWidthMemory % 4 == 0)? (mWidthMemory / 4 ): (mWidthMemory / 4 + 1);
    }else if(scale ==16) {
        mScale = scale;
        mWidthByte = (mWidthMemory%2==0) ? (mWidthMemory/2) : (mWidthMemory/2+1); 
    }else if(scale ==65) {
        mScale = scale;
        mWidthByte = mWidthMemory*2; 
    }else{
        Debug("Set Scale Input parameter error\r\n");
        Debug("Scale Only support: 2 4 16 65\r\n");
    }
}

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void Paint::SetMirroring(UBYTE mirror)
{
    if(mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || 
        mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
        Debug("mirror image x:%s, y:%s\r\n",(mirror & 0x01)? "mirror":"none", ((mirror >> 1) & 0x01)? "mirror":"none");
        mMirror = mirror;
    } else {
        Debug("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, \
        MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
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
    if(Xpoint > mWidth || Ypoint > mHeight){
        Debug("Exceeding display boundaries\r\n");
        return;
    }      
    UWORD X, Y;

    switch(mRotate) {
    case 0:
        X = Xpoint;
        Y = Ypoint;  
        break;
    case 90:
        X = mWidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = mWidthMemory - Xpoint - 1;
        Y = mHeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = mHeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }
    
    switch(mMirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = mWidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = mHeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = mWidthMemory - X - 1;
        Y = mHeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > mWidthMemory || Y > mHeightMemory){
        Debug("Exceeding display boundaries\r\n");
        return;
    }
    
    if(mScale == 2){
        UDOUBLE Addr = X / 8 + Y * mWidthByte;
        UBYTE Rdata = mImage[Addr];
        if(Color == BLACK)
            mImage[Addr] = Rdata & ~(0x80 >> (X % 8));
        else
            mImage[Addr] = Rdata | (0x80 >> (X % 8));
    }else if(mScale == 4){
        UDOUBLE Addr = X / 4 + Y * mWidthByte;
        Color = Color % 4;//Guaranteed color scale is 4  --- 0~3
        UBYTE Rdata = mImage[Addr];
        
        Rdata = Rdata & (~(0xC0 >> ((X % 4)*2)));
        mImage[Addr] = Rdata | ((Color << 6) >> ((X % 4)*2));
    }else if(mScale == 16) {
        UDOUBLE Addr = X / 2 + Y * mWidthByte;
        UBYTE Rdata = mImage[Addr];
        Color = Color % 16;
        Rdata = Rdata & (~(0xf0 >> ((X % 2)*4)));
        mImage[Addr] = Rdata | ((Color << 4) >> ((X % 2)*4));
    }else if(mScale == 65) {
        UDOUBLE Addr = X*2 + Y*mWidthByte;
        mImage[Addr] = 0xff & (Color>>8);
        mImage[Addr+1] = 0xff & Color;
    }

}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void Paint::Clear(UWORD Color)
{
    if(mScale == 2 || mScale == 4) {
        for (UWORD Y = 0; Y < mHeightByte; Y++) {
            for (UWORD X = 0; X < mWidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X + Y*mWidthByte;
                mImage[Addr] = Color;
            }
        }
    }else if(mScale == 16) {
        for (UWORD Y = 0; Y < mHeightByte; Y++) {
            for (UWORD X = 0; X < mWidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X + Y*mWidthByte;
                Color = Color & 0x0f;
                mImage[Addr] = (Color<<4) | Color;
            }
        }
    }else if(mScale == 65) {
        for (UWORD Y = 0; Y < mHeightByte; Y++) {
            for (UWORD X = 0; X < mWidthByte; X++ ) {//8 pixel =  1 byte
                UDOUBLE Addr = X*2 + Y*mWidthByte;
                mImage[Addr] = 0x0f & (Color>>8);
                mImage[Addr+1] = 0x0f & Color;
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

    if (Xpoint > mWidth || Ypoint > mHeight) {
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
void Paint::DrawString(UWORD Xstart, UWORD Ystart, const char * pString,
                         sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    if (Xstart > mWidth || Ystart > mHeight) {
        Debug("DrawString Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > mWidth ) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height ) > mHeight ) {
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
    if (Xpoint > mWidth || Ypoint > mHeight) {
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
    DrawString(Xpoint, Ypoint, (const char*)pStr, Font, Color_Background, Color_Foreground);
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

    for (y = 0; y < mHeightByte; y++) {
        for (x = 0; x < mWidthByte; x++) {//8 pixel =  1 byte
            Addr = x + y * mWidthByte;
            mImage[Addr] = (unsigned char)image_buffer[Addr];
        }
    }
}

void Paint_DrawBitMap_Block(const unsigned char* image_buffer, UBYTE Region)
{
    UWORD x, y;
    UDOUBLE Addr = 0;
		for (y = 0; y < mHeightByte; y++) {
				for (x = 0; x < mWidthByte; x++) {//8 pixel =  1 byte
						Addr = x + y * mWidthByte ;
						mImage[Addr] = \
						(unsigned char)image_buffer[Addr+ (mHeightByte)*mWidthByte*(Region - 1)];
				}
		}
}

