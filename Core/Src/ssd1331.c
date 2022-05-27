/*
 * ssd1331.c
 *
 *  Created on: Dec 17, 2020
 *      Author: Marcin
 */

#include "main.h"
#include "ssd1331.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
SPI_HandleTypeDef *oled_spi;
uint16_t buffor[OLED_HEIGHT*OLED_WIDTH];
uint8_t dataUart[20];
uint8_t Uartlength;
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void ssd1331_command(uint8_t command)
{
	ssd1331_cs_low;
	ssd1331_dc_low;
	HAL_SPI_Transmit(oled_spi, &command, 1, ssd1331_TIMEOUT);
	ssd1331_cs_high;
}
void setPixel(uint8_t x,uint8_t y,uint16_t color)
{
	if (x > OLED_WIDTH || y > OLED_HEIGHT)
		{
			return;
		}

	    buffor[y*96+x]=color;

}

void ssd1331_clear()
{
	memset(buffor,0x00,2*6144);

}
void invert()
{
	for(int i=0;i<6144;i++)
	{
		buffor[i]=~buffor[i];
	}
}
void ssd1331_display(uint8_t invertflag)
{
	if(invertflag)
	{
		invert();
	}
	ssd1331_command(SET_COLUMN_ADDRESS);
	ssd1331_command(0);
	ssd1331_command(OLED_WIDTH - 1);
	    //set row point
	ssd1331_command(SET_ROW_ADDRESS);
	ssd1331_command(0);
	ssd1331_command(OLED_HEIGHT - 1);

	//uint8_t Datafirst;
	//uint8_t Datasecond;
	ssd1331_cs_low;
	ssd1331_dc_high;
	uint32_t Length = OLED_WIDTH * OLED_HEIGHT,i=0;
	while (Length > 0U)
	    {
	      /* Wait until TXE flag is set to send data */
	      if(__HAL_SPI_GET_FLAG(oled_spi, SPI_FLAG_TXE))
	      {
	    	  // Write higher byte of color to DR
	        *((__IO uint8_t *)&oled_spi->Instance->DR) = (buffor[i] >> 8);
	        // Wait for transfer
	        while(__HAL_SPI_GET_FLAG(oled_spi, SPI_FLAG_TXE) != SET)
	        {}
	        // Write lower byt of color to DR
	        *((__IO uint8_t *)&oled_spi->Instance->DR) = (buffor[i] & 0xFF);
	        // Decrease Lenght
	        Length--;
	        i++;
	      }
	    }
		while(__HAL_SPI_GET_FLAG(oled_spi, SPI_FLAG_BSY) != RESET)
			{

			}

	ssd1331_cs_high;
}




void ssd1331_init(SPI_HandleTypeDef* spi)
{
	ssd1331_rst_low;
	HAL_Delay(15);
	ssd1331_rst_high;
	HAL_Delay(150);
	oled_spi=spi;
	__HAL_SPI_ENABLE(oled_spi);
	ssd1331_command(DISPLAY_OFF);          //Display Off
	    ssd1331_command(SET_CONTRAST_A);       //Set contrast for color A
	    ssd1331_command(0xFF);                     //145 0x91
	    ssd1331_command(SET_CONTRAST_B);       //Set contrast for color B
	    ssd1331_command(0xFF);                     //80 0x50
	    ssd1331_command(SET_CONTRAST_C);       //Set contrast for color C
	    ssd1331_command(0xFF);                     //125 0x7D
	    ssd1331_command(MASTER_CURRENT_CONTROL);//master current control
	    ssd1331_command(0x06);                     //6
	    ssd1331_command(SET_PRECHARGE_SPEED_A);//Set Second Pre-change Speed For ColorA
	    ssd1331_command(0x99);                     //100
	    ssd1331_command(SET_PRECHARGE_SPEED_B);//Set Second Pre-change Speed For ColorB
	    ssd1331_command(0x99);                     //120
	    ssd1331_command(SET_PRECHARGE_SPEED_C);//Set Second Pre-change Speed For ColorC
	    ssd1331_command(0x99);                     //100
	    ssd1331_command(SET_REMAP);            //set remap & data format
	    ssd1331_command(0x72);                     //0x72
	    ssd1331_command(SET_DISPLAY_START_LINE);//Set display Start Line
	    ssd1331_command(0x0);
	    ssd1331_command(SET_DISPLAY_OFFSET);   //Set display offset
	    ssd1331_command(0x0);
	    ssd1331_command(NORMAL_DISPLAY);       //Set display mode
	    ssd1331_command(SET_MULTIPLEX_RATIO);  //Set multiplex ratio
	    ssd1331_command(0x3F);
	    ssd1331_command(SET_MASTER_CONFIGURE); //Set master configuration
	    ssd1331_command(0x8E);
	    ssd1331_command(POWER_SAVE_MODE);      //Set Power Save Mode
	    ssd1331_command(0x00);                     //0x00
	    ssd1331_command(PHASE_PERIOD_ADJUSTMENT);//phase 1 and 2 period adjustment
	    ssd1331_command(0x31);                     //0x31
	    ssd1331_command(DISPLAY_CLOCK_DIV);    //display clock divider/oscillator frequency
	    ssd1331_command(0xF0);
	    ssd1331_command(SET_PRECHARGE_VOLTAGE);//Set Pre-Change Level
	    ssd1331_command(0x3A);
	    ssd1331_command(SET_V_VOLTAGE);        //Set vcomH
	    ssd1331_command(0x3E);
	    ssd1331_command(DEACTIVE_SCROLLING);   //disable scrolling
	    ssd1331_command(NORMAL_BRIGHTNESS_DISPLAY_ON);//set display on
	    Uartlength=sprintf((char*)dataUart,"Udalo sie! \n \r");
	    HAL_UART_Transmit(&huart2, dataUart, Uartlength, ssd1331_TIMEOUT);
	    HAL_Delay(1000);
	    ssd1331_clear();
	    ssd1331_display(0);
}
