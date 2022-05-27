/*
 * l3gd20.h
 *
 *  Created on: Dec 26, 2020
 *      Author: Marcin
 */

#ifndef INC_L3GD20_H_
#define INC_L3GD20_H_
#define READ 0x80
#define MULTIPLE_READ 0x40
#define CTRL_REG_1 0x20
#define READ_REG_L3 0x28
#define L3GD20_TIMEOUT 1000
typedef struct
{
	SPI_HandleTypeDef *l3gd_spi;
}L3GD_t;
void L3GD_Init(L3GD_t *l3,SPI_HandleTypeDef *spi);
void L3GD20_Read(L3GD_t *l3,int16_t *X,int16_t *Y,int16_t *Z);
#endif /* INC_L3GD20_H_ */
