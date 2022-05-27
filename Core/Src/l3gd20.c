/*
 * l3gd20.c
 *
 *  Created on: Dec 26, 2020
 *      Author: Marcin
 */
#include "main.h"
#include "l3gd20.h"
uint8_t ctrl_reg_1_data = 0b11111111;
uint8_t read = (READ|READ_REG_L3|MULTIPLE_READ);
uint8_t dataTosend[]={0x28|0x80,0x29|0x80,0x2a|0x80,0x2b|0x80,0x2c|0x80,0x2d|0x80};
void L3_Write(L3GD_t *l3,uint8_t reg,uint8_t data)
{
	HAL_GPIO_WritePin(l3gd20_cs_GPIO_Port, l3gd20_cs_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(l3->l3gd_spi, &reg, 1, L3GD20_TIMEOUT);
	HAL_SPI_Transmit(l3->l3gd_spi, &data, 1, L3GD20_TIMEOUT);
	HAL_GPIO_WritePin(l3gd20_cs_GPIO_Port, l3gd20_cs_Pin, GPIO_PIN_SET);
}
void L3GD20_Read(L3GD_t *l3,int16_t *X,int16_t *Y,int16_t *Z)
{
	L3_Write(l3, CTRL_REG_1, ctrl_reg_1_data);
	HAL_GPIO_WritePin(l3gd20_cs_GPIO_Port, l3gd20_cs_Pin, GPIO_PIN_RESET);
	uint8_t data[]={0,0,0,0,0,0,0,0};
	HAL_SPI_TransmitReceive(l3->l3gd_spi, &read, data, 6, L3GD20_TIMEOUT);
	*X=((data[1]<<8|data[0]));
	*Y=((data[3]<<8|data[2]));
	*Z=((data[5]<<8|data[4]));

	HAL_GPIO_WritePin(l3gd20_cs_GPIO_Port, l3gd20_cs_Pin, GPIO_PIN_SET);
}
void L3GD_Init(L3GD_t *l3,SPI_HandleTypeDef *spi)
{
	l3->l3gd_spi=spi;
	L3_Write(l3, CTRL_REG_1, ctrl_reg_1_data);
}

