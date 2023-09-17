#ifndef __RECORDER_H
#define __RECORDER_H
#include "simple_gpio.h"	
	   															 
 //RIFF��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"RIFF",��0X46464952
    uint32_t ChunkSize ;		   	//���ϴ�С;�ļ��ܴ�С-8
    uint32_t Format;	   			//��ʽ;WAVE,��0X45564157
}ChunkRIFF ;
//fmt��
typedef __packed struct
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fmt ",��0X20746D66
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:20.
    uint16_t AudioFormat;	  	//��Ƶ��ʽ;0X10,��ʾ����PCM;0X11��ʾIMA ADPCM
	uint16_t NumOfChannels;		//ͨ������;1,��ʾ������;2,��ʾ˫����;
	uint32_t SampleRate;			//������;0X1F40,��ʾ8Khz
	uint32_t ByteRate;			//�ֽ�����; 
	uint16_t BlockAlign;			//�����(�ֽ�); 
	uint16_t BitsPerSample;		//�����������ݴ�С;4λADPCM,����Ϊ4
//	uint16_t ByteExtraData;		//���ӵ������ֽ�;2��; ����PCM,û���������
//	uint16_t ExtraData;			//���ӵ�����,�����������ݿ��С;0X1F9:505�ֽ�  ����PCM,û���������
}ChunkFMT;	   
//fact�� 
typedef __packed struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"fact",��0X74636166;
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
    uint32_t NumOfSamples;	  	//����������; 
}ChunkFACT;
//data�� 
typedef __packed struct 
{
    uint32_t ChunkID;		   	//chunk id;����̶�Ϊ"data",��0X61746164
    uint32_t ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);�ļ���С-60.
}ChunkDATA;

//wavͷ
typedef __packed struct
{ 
	ChunkRIFF riff;	//riff��
	ChunkFMT fmt;  	//fmt��
	//ChunkFACT fact;	//fact�� ����PCM,û������ṹ��	 
	ChunkDATA data;	//data��		 
}__WaveHeader; 

extern __WaveHeader global_wavheader;//����Ȼ�Ǻ���ջ�ռ䲻�㡣Ҫ�����ں�������
extern uint8_t global_buff512[512];

void recoder_enter_rec_mode(uint16_t agc);
void recoder_wav_init(__WaveHeader* wavhead);//��ʼ��WAVͷ	
uint8_t audio_play(uint8_t *pname);
uint8_t audio_recorde(float agc,uint32_t recsec);
#endif
















