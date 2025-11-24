/*
 * max30102.h
 *
 *  Created on: 1 nov 2025
 *      Author: guido
 */

#ifndef MAX30102_DRIVER_MAX30102_H_
#define MAX30102_DRIVER_MAX30102_H_

//#include "MyIncludes.h"

#define IIC_DELAY_ms				10
#define IIC_DELAY_30ms				255
#define IIC_DELAY_500ms				500
#define IIC_DELAY_1s				1000
#define MAX30102_PHY_ADDRESS		0xAE
#define MAX30102_INT_STATUS_1		0x00
#define MAX30102_INT_STATUS_2		0x01
#define MAX30102_INT_EN_1			0x02
#define MAX30102_INT_EN_2			0x03
#define MAX30102_FIFO_WR_PTR		0x04
#define MAX30102_FIFO_OV_PTR		0x05
#define MAX30102_FIFO_RD_PTR		0x06
#define MAX30102_FIFO_DATA			0x07
#define MAX30102_FIFO_CONFIG		0x08
#define MAX30102_MODE_CONFIG		0x09
#define MAX30102_SPO2_CONFIG 		0x0A
#define MAX30102_LED1_PA 			0x0C
#define MAX30102_LED2_PA 			0x0D
#define MAX30102_PILOT_PA 			0x10

#define MAX30102_BYTES_PER_SAMPLE 	6
#define MAX30102_SAMPLE_LEN_MAX 	32

#define	MAX30102_LED_PA_7_6mA		0x20
#define MAX30102_LED_PA_10mA	 	0x32
#define MAX30102_LED_PA_25mA	 	0x7F
#define MAX30102_LED_PA_50mA 		0xFF

#define DC_REMOVER_ALPHA           	95//0.95

#define BPM_BUFFER_SIZE 			5 // TAMAÑO DEL PROMEDIADOR 5 MUESTRAS
#define OUTLIER_THRESHOLD_I 30
#define ALPHA_MULTIPLIER_X10 5

void MAX30102( void );
void MAX30102ResetPointers( void );
uint8_t MAX30102InitialConfig( void );
void MAX30102_Write( uint8_t, uint8_t );
uint8_t MAX30102_Read( uint8_t );
void MAX30102_Read_Sample( uint8_t, uint8_t *);
void MAX30102_Read_FIFO (uint8_t reg, uint8_t *buff, uint8_t samples);
//void MAX30102_Read_FIFO( uint8_t, uint8_t *, uint8_t, uint32_t *, uint32_t *);
//float DCRemoval(float, float *, float);
int32_t DCRemoval(int32_t, int32_t *, int32_t);
void SetFlagLecturaMAX30102(uint8_t valor);
uint8_t GetFlagLecturaMAX30102(void);

/*Agrego método par poder recupar la lectura del sensor en la Maquina de Estados desde el Main*/
int32_t MAX30102_GetLectura(void);
void MAX30102_CleanBuffer();


#endif /* MAX30102_DRIVER_MAX30102_H_ */
