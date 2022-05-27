/*
 * lsm303.c
 *
 *  Created on: Dec 24, 2020
 *      Author: Marcin
 */
#include "main.h"
#include "lsm303.h"
uint8_t ctrl1_data=0b01100111;
uint8_t ctrl5_data=0b00010000;
uint8_t ctrl6_data=0b01100000;
uint8_t ctrl7_data=0b00000000;
void lsm_write(LSM_t *lsm,uint8_t reg,uint8_t data)
{
	HAL_I2C_Mem_Write(lsm->i2c, ((lsm->addres)<<1), reg, 1, &data, 1, lsm_TIMEOUT);
}
void Lsm_ReadAcc(LSM_t *lsm,int16_t *X,int16_t *Y,int16_t *Z)
{
	uint8_t data[6];
	HAL_I2C_Mem_Read(lsm->i2c, ((lsm->addres)<<1), READ_ADDRES_ACC, 1, data, 6, lsm_TIMEOUT);
	*X=((data[1]<<8|data[0])-lsm->Xoffsetacc);
	*Y=((data[3]<<8|data[2])-lsm->Yoffsetacc);
	*Z=((data[5]<<8|data[4])-lsm->Zoffsetacc);
}
void Lsm_ReadMag(LSM_t *lsm,int16_t *X,int16_t *Y,int16_t *Z)
{
	uint8_t data2[]={0,0,0,0,0,0};
	HAL_I2C_Mem_Read(lsm->i2c, ((lsm->addres)<<1), READ_ADDRES_MAG, 1, data2, 6, lsm_TIMEOUT);
	*X=(data2[1]<<8|data2[0]);
	*Y=(data2[3]<<8|data2[2]);
	*Z=(data2[5]<<8|data2[4]);
}
void Lsm_cal(LSM_t *lsm)
{
	uint8_t data[6];
	for(int i=0;i<1000;i++)
	{
		HAL_I2C_Mem_Read(lsm->i2c, ((lsm->addres)<<1), READ_ADDRES_ACC, 1, data, 6, lsm_TIMEOUT);
		lsm->Xoffsetacc+=(data[1]<<8|data[0]);
		lsm->Yoffsetacc+=(data[3]<<8|data[2]);
		lsm->Zoffsetacc+=(data[5]<<8|data[4]);
		HAL_Delay(10);
	}
	lsm->Xoffsetacc/=1000;
	lsm->Yoffsetacc/=1000;
	lsm->Zoffsetacc/=1000;
}
void Lsm303_Init(LSM_t *lsm,I2C_HandleTypeDef* i2c,uint8_t addr)
{
	lsm->i2c=i2c;
	lsm->addres=addr;
	lsm_write(lsm,CTRL_REG_1,ctrl1_data);
	lsm_write(lsm, CTRL_REG_5, ctrl5_data);
	lsm_write(lsm, CTRL_REG_6, ctrl6_data);
	lsm_write(lsm, CTRL_REG_7, ctrl7_data);
	lsm->Xoffsetacc=0;
	lsm->Yoffsetacc=0;
	lsm->Zoffsetacc=0;
}

