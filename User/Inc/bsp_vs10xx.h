#ifndef __VS10XX_H__
#define __VS10XX_H__
#include "simple_gpio.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//VS10XX ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved													    								  
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
//���ⲿ�Ľӿ�
#define VS_SPI			hspi2
#define VS_DQ       PAin(8)  		//DREQ 
#define VS_RST      PBout(10) 		//RST
#define VS_XCS      PBout(12)  		//XCS
#define VS_XDCS     PBout(11)  		//XDCS 
//////////////////////////////////////////////////////////////

__packed typedef struct 
{							  
	uint8_t mvol;		//������,��Χ:0~254
	uint8_t bflimit;		//����Ƶ���޶�,��Χ:2~15(��λ:10Hz)
	uint8_t bass;		//����,��Χ:0~15.0��ʾ�ر�.(��λ:1dB)
	uint8_t tflimit;		//����Ƶ���޶�,��Χ:1~15(��λ:Khz)
	uint8_t treble;		//����,��Χ:0~15(��λ:1.5dB)(ԭ����Χ��:-8~7,ͨ�������޸���);
	uint8_t effect;		//�ռ�Ч������.0,�ر�;1,��С;2,�е�;3,���.

	uint8_t saveflag; 	//�����־,0X0A,�������;����,����δ����	   
}_vs10xx_obj;


extern _vs10xx_obj vsset;		//VS10XX����

#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03
//VS10XX�Ĵ�������
#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09 
  
#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f   
#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 		 



uint16_t  VS_RD_Reg(uint8_t address);				//���Ĵ���
uint16_t  VS_WRAM_Read(uint16_t addr);	    	//��RAM
void VS_WR_Data(uint8_t data);				//д����
void VS_WR_Cmd(uint8_t address,uint16_t data);	//д����
uint8_t   VS_HD_Reset(void);			    	//Ӳ��λ
void VS_Soft_Reset(void);           	//��λ
uint16_t VS_Ram_Test(void);             		//RAM����	    
void VS_Sine_Test(void);            	//���Ҳ���
													 
uint8_t 	 VS_SPI_ReadWriteByte(uint8_t data);
void VS_SPI_SpeedLow(void);
void VS_SPI_SpeedHigh(void);
void VS_Set_Speed(uint8_t t);				//���ò����ٶ�
uint16_t  VS_Get_HeadInfo(void);     		//�õ�������
uint32_t VS_Get_ByteRate(void);				//�õ��ֽ�����
uint16_t VS_Get_EndFillByte(void);			//�õ�����ֽ�
uint8_t 	 VS_Send_MusicData(uint8_t* buf);		//��VS10XX����32�ֽ�
void VS_Restart_Play(void);				//���¿�ʼ��һ�׸貥��	  
void VS_Reset_DecodeTime(void);			//�������ʱ��
uint16_t  VS_Get_DecodeTime(void);   		//�õ�����ʱ��

void VS_Load_Patch(uint16_t *patch,uint16_t len);	//�����û�patch
uint8_t 	 VS_Get_Spec(uint16_t *p);       		//�õ���������	   
void VS_Set_Bands(uint16_t *buf,uint8_t bands);	//��������Ƶ��
void VS_Set_Vol(uint8_t volx);				//����������   
void VS_Set_Bass(uint8_t bfreq,uint8_t bass,uint8_t tfreq,uint8_t treble);//���øߵ���
void VS_Set_Effect(uint8_t eft);				//������Ч
void VS_Set_All(void);

void vs10xx_read_para(_vs10xx_obj * vs10xxdev);
void vs10xx_save_para(_vs10xx_obj * vs10xxdev);

#endif

















