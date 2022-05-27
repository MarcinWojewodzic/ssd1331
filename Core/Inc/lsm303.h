/*
 * lsm303.h
 *
 *  Created on: Dec 24, 2020
 *      Author: Marcin
 */

#ifndef INC_LSM303_H_
#define INC_LSM303_H_
#define lsm_TIMEOUT 1000
#define CTRL_REG_1 0x20
#define CTRL_REG_5 0x24
#define CTRL_REG_6 0x25
#define CTRL_REG_7 0x26
#define READ_ADDRES_ACC (0x28|0x80)
#define READ_ADDRES_MAG (0x08|0x80)
typedef struct
{
	I2C_HandleTypeDef *i2c;
	uint8_t addres;
	int32_t Xoffsetacc;
	int32_t Yoffsetacc;
	int32_t Zoffsetacc;

}LSM_t;

void Lsm303_Init(LSM_t *lsm,I2C_HandleTypeDef* i2c,uint8_t addr);
void Lsm_ReadAcc(LSM_t *lsm,int16_t *X,int16_t *Y,int16_t *Z);
void Lsm_cal(LSM_t *lsm);
void Lsm_ReadMag(LSM_t *lsm,int16_t *X,int16_t *Y,int16_t *Z);
#endif /* INC_LSM303_H_ */
