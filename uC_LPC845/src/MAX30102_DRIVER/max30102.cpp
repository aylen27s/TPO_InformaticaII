/*
 * driver max30102
 *
 *  Created on: 1 nov 2025
 *      Author: unknown
 */

#include "Defines.h"
#include <stdio.h>
#include <string.h>

extern volatile uint32_t g_milisegundos; 

extern UART0 Uart0;
uint8_t data[192], CantDatos;
uint8_t buffer[100];

static uint32_t bpm_buffer[BPM_BUFFER_SIZE] = {0};
static uint8_t bpm_buffer_index = 0;
static uint8_t buffer_full = 0; // un flag para saber si se llenó el buffer

volatile uint8_t g_flagLecturaMAX30102 = 0;

void SetFlagLecturaMAX30102( uint8_t valor )
{
	g_flagLecturaMAX30102 = valor;
}

uint8_t GetFlagLecturaMAX30102( void )
{
	return g_flagLecturaMAX30102;
}

void MAX30102( void )
{
	static uint8_t estado = 0;
	uint8_t rd, wr;				//	, ov;
	int32_t samples;

	switch( estado )
	{
	case 0:
		//Init
		if( MAX30102InitialConfig() == 0 )
		{
			//SetDemora_IIC(IIC_DELAY_ms);
			estado = 1;
		}

		break;

	case 1:
		//if( !GetDemora_IIC() )
		if( GetFlagLecturaMAX30102() )
		{
			SetFlagLecturaMAX30102(0); // se limpia el flag 


			wr = MAX30102_Read( MAX30102_FIFO_WR_PTR ) & 0x1F;


			//ov = MAX30102_Read( MAX30102_FIFO_OV_PTR ) & 0x1F;


			rd = MAX30102_Read( MAX30102_FIFO_RD_PTR ) & 0x1F;


			samples = rd - wr;
			if(samples < 0)
			{
				samples += 32; //MAX30102_SAMPLES_LEN_MAX ES 32
			}
			uint8_t samplesToRead = (samples == 0) ? 1: (uint8_t)samples;
			if (samplesToRead > 32) {
				samplesToRead = 32;
			}
			MAX30102_Read_FIFO (MAX30102_FIFO_DATA, data, samplesToRead);
			uint32_t bpm_del_ciclo = 0;
			bool beat_detectado_en_el_ciclo = false;

			// iterar y procesar cada muestra individualmente
			for(uint8_t i = 0; i < samplesToRead; i++)
			{
				// reconstruir la muestra individual
				uint32_t red_sample = (data[(i*MAX30102_BYTES_PER_SAMPLE)]<<16) | (data[(i*MAX30102_BYTES_PER_SAMPLE)+1]<<8) | data[(i*MAX30102_BYTES_PER_SAMPLE)+2];
				uint32_t ir_sample = (data[(i*MAX30102_BYTES_PER_SAMPLE)+3]<<16) | (data[(i*MAX30102_BYTES_PER_SAMPLE)+4]<<8) | data[(i*MAX30102_BYTES_PER_SAMPLE)+5];
				red_sample &= 0x03FFFF;
				ir_sample &= 0x03FFFF;

				bool beatDetected = checkForBeat((int32_t) ir_sample);
				if(beatDetected)
				{
					static uint32_t lastBeatTime_ms = 0;
					uint32_t currentBeatTime_ms = g_milisegundos;
					if(lastBeatTime_ms != 0)
					{
						uint32_t delta_ms = currentBeatTime_ms - lastBeatTime_ms;
						if(delta_ms > 545 && delta_ms < 2000){

							bpm_del_ciclo = 60000 / delta_ms;
							beat_detectado_en_el_ciclo = true;

						}
					}
					lastBeatTime_ms = currentBeatTime_ms;
				}
			}
			if(beat_detectado_en_el_ciclo)
			{
				bpm_buffer[bpm_buffer_index] = bpm_del_ciclo;
				bpm_buffer_index++;
				if(bpm_buffer_index >= BPM_BUFFER_SIZE){
					bpm_buffer_index = 0;
					buffer_full = 1; // marcamos que el buffer se lleno una vez
				}

				// si el buffer está lleno, se calcula el promedio
				if(buffer_full){
					uint32_t bpm_sum = 0;
					for(uint8_t i = 0; i < BPM_BUFFER_SIZE; i++)
					{
						bpm_sum += bpm_buffer[i];
					}
					uint32_t bpm_avg = bpm_sum / BPM_BUFFER_SIZE;
					sprintf((char *)buffer, "BPM: %u\n",bpm_avg);
					Uart0.Send(buffer,strlen((char *)buffer));
				}

			}
		}
		break;

	case 2:

		break;
	case 3:

		break;
	default:
		break;
	}
}

void MAX30102ResetPointers( void )
{
	MAX30102_Write( MAX30102_FIFO_WR_PTR, 0 );
	MAX30102_Write( MAX30102_FIFO_OV_PTR, 0 );
	MAX30102_Write( MAX30102_FIFO_RD_PTR, 0 );
}

uint8_t MAX30102InitialConfig( void )
{
	static uint8_t estado = 0;


	switch( estado )
	{
	case 0:
		MAX30102_Write( MAX30102_MODE_CONFIG, 0x40);		//Reset
		SetDemora_IIC(IIC_DELAY_30ms);
		estado = 1;
		break;

	case 1:
		if( !GetDemora_IIC() )
		{

			//MAX30102_Write( MAX30102_INT_EN_1, 0x80);		//Almost full enable
			MAX30102_Write( MAX30102_INT_EN_1, 0x00);		//
			MAX30102_Write( MAX30102_INT_EN_2, 0x00);		//
			MAX30102ResetPointers( );
			SetDemora_IIC(IIC_DELAY_ms);
			estado = 2;
		}
		break;

	case 2:
		if( !GetDemora_IIC() )
		{
			MAX30102_Write( MAX30102_MODE_CONFIG, 0x03);		//SpO2 Red and IR
			SetDemora_IIC(IIC_DELAY_ms);
			estado = 3;
		}
		break;


	case 3:
		if( !GetDemora_IIC() )
		{
			//MAX30102_Write( MAX30102_SPO2_CONFIG, 0x41);		//ADC resolution 15 bits, 50 samples per second, pulse 69us, fullscale 8192
			MAX30102_Write( MAX30102_SPO2_CONFIG, 0x07);		//ADC resolution 18 bits, 100 samples per second, pulse 411us, fullscale 2048
			//MAX30102_Write( MAX30102_SPO2_CONFIG, 0x0F);		//ADC resolution 18 bits, 400 samples per second, pulse 411us
			SetDemora_IIC(IIC_DELAY_ms);
			estado = 4;
		}
		break;

	case 4:
		if( !GetDemora_IIC() )
			{
				MAX30102_Write( MAX30102_LED1_PA, 0x15);		//
				SetDemora_IIC(IIC_DELAY_ms);
				estado = 5;
			}
			break;



	case 5:
		if( !GetDemora_IIC() )
		{
			MAX30102_Write( MAX30102_LED2_PA, 0x0F);		//
			SetDemora_IIC(IIC_DELAY_ms);
			estado = 6;
		}
		break;

	case 6:
		if( !GetDemora_IIC() )
		{
			//MAX30102_Write( MAX30102_FIFO_CONFIG, 0xE0);		//32 muestras promedio, rollover on
			MAX30102_Write( MAX30102_FIFO_CONFIG, 0x00);		//
			SetDemora_IIC(IIC_DELAY_ms);
			estado = 7;
		}
		break;

	case 7:
		estado = 0;
		break;


	default:
		estado = 0;
		break;


	}

	return estado;
}

void MAX30102_Write( uint8_t reg, uint8_t value )
{
	IIC_Start();
	IIC_Write(MAX30102_PHY_ADDRESS);
	IIC_Write(reg);
	IIC_Write(value);
	IIC_Stop();
}

uint8_t MAX30102_Read( uint8_t reg )
{
	uint8_t value;

	IIC_Start();
	IIC_Write(MAX30102_PHY_ADDRESS);
	IIC_Write(reg);
	IIC_Start();
	IIC_Write(MAX30102_PHY_ADDRESS | 1);
	value = IIC_Read(NO_ACK);
	IIC_Stop();
	return value;
}

void MAX30102_Read_Sample( uint8_t reg, uint8_t * buff)
{
	uint8_t i;
	IIC_Start();
	IIC_Write(MAX30102_PHY_ADDRESS);
	IIC_Write(reg);
	IIC_Start();
	IIC_Write(MAX30102_PHY_ADDRESS | 1);
	for(i = 0; i < 6; i ++ )	//cada muestra es de 6 bytes
	{
		buff[i] = IIC_Read(ACK);
	}

	IIC_Stop();
}


void MAX30102_Read_FIFO (uint8_t reg, uint8_t *buff, uint8_t samples)
{
	uint8_t i;
	IIC_Start();
	IIC_Write(MAX30102_PHY_ADDRESS);
	IIC_Write(reg);
	IIC_Start();
	IIC_Write(MAX30102_PHY_ADDRESS | 1);

	for (i = 0; i < samples * MAX30102_BYTES_PER_SAMPLE; i++)
	{
		buff[i] = IIC_Read(ACK);
	}
	IIC_Stop();
	CantDatos = samples*MAX30102_BYTES_PER_SAMPLE;

}




