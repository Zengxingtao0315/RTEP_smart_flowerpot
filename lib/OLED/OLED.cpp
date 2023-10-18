/*****************************************************************************

// This code is adapted from the little-sun-gazer project by dr-mod, licensed under the GNU GENERAL PUBLIC LICENSE.
// Original source: https://github.com/dr-mod/little-sun-gazer/blob/main/GUI_Paint.c
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
* -----------------------------------------------------------------------------
*****************************************************************************/
#include "OLED.hpp"
#include <cstdio>
#include <cstdint> 
#include <cstdint>
#include <cstring>



OLED::OLED(DEV * devPtr): dev(devPtr){}


 OLED::~OLED(){
    delete dev;
 }

/*******************************************************************************
function:
            Hardware reset
*******************************************************************************/
using namespace std;

void OLED::Reset(void)
{
	dev->Digital_Write(OLED_RST,1);
    dev->Delay_ms(100);
	dev->Digital_Write(OLED_RST,0);
    dev->Delay_ms(100);
	dev->Digital_Write(OLED_RST,1);
    dev->Delay_ms(100);
}

/*******************************************************************************
function:
            Write register address and data
*******************************************************************************/
void OLED::WriteReg(uint8_t Reg)
{

    dev->Digital_Write(OLED_DC,0);
    dev->SPI_WriteByte(Reg);

}

void OLED::WriteData(uint8_t Data)
{   

    dev->Digital_Write(OLED_DC,1);
    dev->SPI_WriteByte(Data);
}

/*******************************************************************************
function:
        Common register initialization
*******************************************************************************/
void OLED::InitReg(void)
{
    WriteReg(0xfd);  // command lock
    WriteData(0x12);
    WriteReg(0xfd);  // command lock
    WriteData(0xB1);

    WriteReg(0xae);  // display off
    WriteReg(0xa4);  // Normal Display mode

    WriteReg(0x15);  //set column address
    WriteData(0x00);     //column address start 00
    WriteData(0x7f);     //column address end 95
    WriteReg(0x75);  //set row address
    WriteData(0x00);     //row address start 00
    WriteData(0x7f);     //row address end 63    

    WriteReg(0xB3);
    WriteData(0xF1);

    WriteReg(0xCA);  
    WriteData(0x7F);

    WriteReg(0xa0);  //set re-map & data format
    WriteData(0x74);     //Horizontal address increment

    WriteReg(0xa1);  //set display start line
    WriteData(0x00);     //start 00 line

    WriteReg(0xa2);  //set display offset
    WriteData(0x00);

    WriteReg(0xAB);  
    WriteReg(0x01);  

    WriteReg(0xB4);  
    WriteData(0xA0);   
    WriteData(0xB5);  
    WriteData(0x55);    

    WriteReg(0xC1);  
    WriteData(0xC8); 
    WriteData(0x80);
    WriteData(0xC0);

    WriteReg(0xC7);  
    WriteData(0x0F);

    WriteReg(0xB1);  
    WriteData(0x32);

    WriteReg(0xB2);  
    WriteData(0xA4);
    WriteData(0x00);
    WriteData(0x00);

    WriteReg(0xBB);  
    WriteData(0x17);

    WriteReg(0xB6);
    WriteData(0x01);

    WriteReg(0xBE);
    WriteData(0x05);

    WriteReg(0xA6);
}

/********************************************************************************
function:
            initialization
********************************************************************************/
void OLED::Init(void)
{
    //Hardware reset
    Reset();

    //Set the initialization register
    InitReg();
    dev->Delay_ms(200);

    //Turn on the OLED display
    WriteReg(0xAF);
    initSuccess = true;
    
}

/********************************************************************************
function:
            Clear screen
********************************************************************************/
void OLED::Clear()
{
    uint16_t i;
    WriteReg(0x15);
    WriteData(0);
    WriteData(OLED_WIDTH - 1);
    WriteReg(0x75);
    WriteData(0);
    WriteData(OLED_HEIGHT / 8 - 1);
    WriteReg(0x5C);

    for (i = 0; i < OLED_WIDTH * OLED_HEIGHT / 8; i++) {
        WriteData(0x00);
    }
}


/********************************************************************************
function:   Update all memory to OLED
********************************************************************************/
void OLED::Display(uint8_t *Image)
{
    uint16_t i, j;

    WriteReg(0x15);
    WriteData(0);
    WriteData(OLED_WIDTH - 1);
    WriteReg(0x75);
    WriteData(0);
    WriteData(OLED_HEIGHT - 1);
    WriteReg(0x5C);

    for (i = 0; i < OLED_HEIGHT; i++) {
        for (j = 0; j < OLED_WIDTH*2; j++) {
            WriteData(Image[j + i * 256]);
        }
    }
}



bool OLED::checkInit()
{
    int maxRetryCount = 3; // Maximum number of retries
    int currentRetryCount = 0;

    while (currentRetryCount < maxRetryCount) {
        Init();
        if (initSuccess) {
            break; // If the initialisation succeeds, jump out of the loop
        }

        currentRetryCount++;
    }
    return initSuccess;

}


