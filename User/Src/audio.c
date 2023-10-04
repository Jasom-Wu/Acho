#include "audio.h"
#include "bsp_delay.h"
#include "bsp_usart.h"
#include "bsp_vs10xx.h"
#include "ff.h"
#include "fops.h"
#include "flac.h"
#include "fatfs.h"
#include "sdio.h"
#include "page_audio.h"

__WaveHeader global_wavheader;//����Ȼ�Ǻ���ջ�ռ䲻�㡣Ҫ�����ں�������
uint8_t global_buff512[512] = {0};
///////////////////////////////��ֲӳ���޸���////////////////////////////////////////////
static FIL *file = (FIL *) &SDFile;        //Ϊfile�����ڴ�
static FIL *ftemp = (FIL *) &SDFile;        //Ϊftemp�����ڴ�
////////////////////////////////////////////////////////////////////////////////////////		


//VS1053��WAV¼����bug,���plugin��������������� 							    
const uint16_t wav_plugin[40] =/* Compressed plugin */
        {
                0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /* 0 */
                0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /* 8 */
                0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /* 10 */
                0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /* 18 */
                0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e,
        };

//����PCM ¼��ģʽ
//agc:0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��		  
void recoder_enter_rec_mode(uint16_t agc) {
    //�����IMA ADPCM,�����ʼ��㹫ʽ����:
    //������=CLKI/256*d;
    //����d=0,��2��Ƶ,�ⲿ����Ϊ12.288M.��ôFc=(2*12288000)/256*6=16Khz
    //���������PCM,������ֱ�Ӿ�д����ֵ
    VS_WR_Cmd(SPI_BASS, 0x0000);
    VS_WR_Cmd(SPI_AICTRL0, 8000);    //���ò�����,����Ϊ8Khz
    VS_WR_Cmd(SPI_AICTRL1, agc);        //��������,0,�Զ�����.1024�൱��1��,512�൱��0.5��,���ֵ65535=64��
    VS_WR_Cmd(SPI_AICTRL2, 0);        //�����������ֵ,0,�������ֵ65536=64X
    VS_WR_Cmd(SPI_AICTRL3, 6);        //��ͨ��(MIC����������)
    VS_WR_Cmd(SPI_CLOCKF, 0X2000);    //����VS10XX��ʱ��,MULT:2��Ƶ;ADD:������;CLK:12.288Mhz
    VS_WR_Cmd(SPI_MODE, 0x1804);        //MIC,¼������
    delay_ms(5);                    //�ȴ�����1.35ms
    VS_Load_Patch((uint16_t *) wav_plugin, 40);//VS1053��WAV¼����Ҫpatch
}

//��ʼ��WAVͷ.
void recoder_wav_init(__WaveHeader *wavhead) //��ʼ��WAVͷ
{
    wavhead->riff.ChunkID = 0X46464952;    //"RIFF"
    wavhead->riff.ChunkSize = 0;            //��δȷ��,�����Ҫ����
    wavhead->riff.Format = 0X45564157;    //"WAVE"
    wavhead->fmt.ChunkID = 0X20746D66;    //"fmt "
    wavhead->fmt.ChunkSize = 16;            //��СΪ16���ֽ�
    wavhead->fmt.AudioFormat = 0X01;        //0X01,��ʾPCM;0X11,��ʾIMA ADPCM
    wavhead->fmt.NumOfChannels = 1;        //������
    wavhead->fmt.SampleRate = 8000;        //8Khz������ ��������
    wavhead->fmt.ByteRate = wavhead->fmt.SampleRate * 2;//16λ,��2���ֽ�
    wavhead->fmt.BlockAlign = 2;            //���С,2���ֽ�Ϊһ����
    wavhead->fmt.BitsPerSample = 16;        //16λPCM
    wavhead->data.ChunkID = 0X61746164;    //"data"
    wavhead->data.ChunkSize = 0;            //���ݴ�С,����Ҫ����
}

//ͨ��ʱ���ȡ�ļ���
//������SD������,��֧��FLASH DISK����
//��ϳ�:����"0:RECORDER/REC20120321210633.wav"���ļ���
void recoder_new_pathname(uint8_t *pname) {
    uint8_t res;
    uint16_t index = 0;
    while (index < 0XFFFF) {
        sprintf((char *) pname, "0:/Records/%d.wav", index);
        res = f_open(ftemp, (const TCHAR *) pname, FA_READ);//���Դ�����ļ�
        if (res == FR_NO_FILE) {
            f_close(ftemp);
            break;        //���ļ���������=����������Ҫ��.
        }
        index++;
    }
}

static uint8_t data_buff[4096];//����4096�ֽڵ��ڴ�����//���һ��Ҫ��һ�㣬��Ȼ���Ų��ˣ���������������
uint16_t decode_time = 0;
uint8_t audio_play_current_state = AUDIO_NONE;

//����pname���wav�ļ���Ҳ������MP3�ȣ�
uint8_t audio_play(uint8_t *pname, uint8_t *play_state) {
    if (*play_state != AUDIO_PLAY){
        audio_play_current_state = AUDIO_NONE;
        return 0xff;
    }
    audio_play_current_state = AUDIO_PLAY;
    FIL *fmp3 = file;
    uint8_t res, rval = 0;
    uint16_t i;
    UINT count;
    VS_Restart_Play();                    //��������
    VS_Set_All();                            //������������Ϣ
    VS_Set_Vol(190);        //��������
    VS_Reset_DecodeTime();    //��λ����ʱ��
    res = f_typetell(pname);                    //�õ��ļ���׺
    if (res == 0x4c)//�����flac,����patch
    {
        VS_Load_Patch((uint16_t *) vs1053b_patch, VS1053B_PATCHLEN);
    }
    res = f_open(fmp3, (const TCHAR *) pname, FA_READ);//���ļ�
    if (res == 0)//�򿪳ɹ�.
    {
        VS_SPI_SpeedHigh();    //����
        decode_time = 0;
        while (*play_state != AUDIO_CANCEL) {
            if (*play_state == AUDIO_PLAY) {
                if(audio_play_current_state==AUDIO_HALT){
                    audio_play_current_state = AUDIO_PLAY;
                }
                while (HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY);
                res = f_read(fmp3, data_buff, 4096, (UINT *) &count);//����4096���ֽ�
                i = 0;
                do//������ѭ��
                {
                    if (VS_Send_MusicData(data_buff + i) == 0) { i += 32; }//��VS10XX������Ƶ����
                    else {
                        decode_time = VS_Get_DecodeTime();
//                        printf("%d\n", decode_time);//��ʾ����ʱ��
                    }
                    if (count != 4096 && i >= count)break;
                } while (i < 4096);//ѭ������4096���ֽ�
                if (count != 4096 || res != 0) {
                    break;//������.
                }
            } else if (*play_state == AUDIO_HALT && audio_play_current_state != AUDIO_HALT)
                audio_play_current_state = AUDIO_HALT;
        }
        f_close(fmp3);
    } else rval = 0XFF;//���ִ���
    *play_state = audio_play_current_state = AUDIO_NONE;
    return rval;
}

/**
	���ܣ�¼������
	������
				agc ¼�����汶��
*/


uint8_t audio_recorde(float agc, uint32_t sec) {
    FIL *f_rec = file;//���ܽ�fil�ļ��������������ջ���������mallocջ�ռ���㣩��Ӧ�ö����ں�����ߡ�
    uint8_t rval = 0;
    uint8_t res = 0;
    uint32_t sectorsize = 0;
    uint32_t recsec = 0;
    uint16_t w;
    uint16_t idx = 0;
    UINT count;
    uint8_t pname[30] = {0};//����30���ֽ��ڴ�,����"0:RECORDER/REC00001.wav"

    recoder_enter_rec_mode(512 * agc);
    while (VS_RD_Reg(SPI_HDAT1) >> 8);            //�ȵ�buf ��Ϊ�����ٿ�ʼ
    pname[0] = 0;                                //pnameû���κ��ļ���
    recoder_new_pathname(pname);            //�õ��µ�����
    recoder_wav_init(&global_wavheader);                //��ʼ��wav����
    res = f_open(f_rec, (const TCHAR *) pname, FA_CREATE_ALWAYS | FA_WRITE);
    if (res)            //�ļ�����ʧ��
    {
        printf("failed1\n");
        rval = 0XFE;    //��ʾ�Ƿ����SD��
    } else res = f_write(f_rec, (const void *) &global_wavheader, sizeof(__WaveHeader), &count);//д��ͷ����
    if (rval == 0) {
        while (recsec < sec) {
            w = VS_RD_Reg(SPI_HDAT1);
            if ((w >= 256) && (w < 896)) {
                idx = 0;
                while (idx < 512)    //һ�ζ�ȡ512�ֽ�
                {
                    w = VS_RD_Reg(SPI_HDAT0);
                    global_buff512[idx++] = w & 0XFF;
                    global_buff512[idx++] = w >> 8;
                }
                res = f_write(f_rec, global_buff512, 512, &count);//д���ļ�
                if (res) {
                    printf("err\r\n");
                    break;//д�����.
                }
                sectorsize++;//����������1,ԼΪ32ms
            }
            if (recsec != (sectorsize * 4 / 125))//¼��ʱ����ʾ
            {
                recsec = sectorsize * 4 / 125;
                printf("%d\n", recsec);
            }
        }
        if (res == 0) {
            global_wavheader.riff.ChunkSize = sectorsize * 512 + 36;   //�����ļ��Ĵ�С-8;    sizeof(__WaveHeader)-8=36 bytes
            global_wavheader.data.ChunkSize = sectorsize * 512;        //���ݴ�С
            f_lseek(f_rec, 0);                                  //ƫ�Ƶ��ļ�ͷ.
            f_write(f_rec, (const void *) &global_wavheader, sizeof(__WaveHeader), &count);//д��ͷ����
            f_close(f_rec);
        }
        uint16_t temp;
        temp = VS_RD_Reg(SPI_MODE);    //��ȡSPI_MODE������
        temp &= ~(1 << 12);
        VS_WR_Cmd(SPI_MODE, temp);
    }
    return rval;
}

















