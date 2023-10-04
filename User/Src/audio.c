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

__WaveHeader global_wavheader;//很显然是函数栈空间不足。要定义在函数外面
uint8_t global_buff512[512] = {0};
///////////////////////////////移植映射修改区////////////////////////////////////////////
static FIL *file = (FIL *) &SDFile;        //为file申请内存
static FIL *ftemp = (FIL *) &SDFile;        //为ftemp申请内存
////////////////////////////////////////////////////////////////////////////////////////		


//VS1053的WAV录音有bug,这个plugin可以修正这个问题 							    
const uint16_t wav_plugin[40] =/* Compressed plugin */
        {
                0x0007, 0x0001, 0x8010, 0x0006, 0x001c, 0x3e12, 0xb817, 0x3e14, /* 0 */
                0xf812, 0x3e01, 0xb811, 0x0007, 0x9717, 0x0020, 0xffd2, 0x0030, /* 8 */
                0x11d1, 0x3111, 0x8024, 0x3704, 0xc024, 0x3b81, 0x8024, 0x3101, /* 10 */
                0x8024, 0x3b81, 0x8024, 0x3f04, 0xc024, 0x2808, 0x4800, 0x36f1, /* 18 */
                0x9811, 0x0007, 0x0001, 0x8028, 0x0006, 0x0002, 0x2a00, 0x040e,
        };

//激活PCM 录音模式
//agc:0,自动增益.1024相当于1倍,512相当于0.5倍,最大值65535=64倍		  
void recoder_enter_rec_mode(uint16_t agc) {
    //如果是IMA ADPCM,采样率计算公式如下:
    //采样率=CLKI/256*d;
    //假设d=0,并2倍频,外部晶振为12.288M.那么Fc=(2*12288000)/256*6=16Khz
    //如果是线性PCM,采样率直接就写采样值
    VS_WR_Cmd(SPI_BASS, 0x0000);
    VS_WR_Cmd(SPI_AICTRL0, 8000);    //设置采样率,设置为8Khz
    VS_WR_Cmd(SPI_AICTRL1, agc);        //设置增益,0,自动增益.1024相当于1倍,512相当于0.5倍,最大值65535=64倍
    VS_WR_Cmd(SPI_AICTRL2, 0);        //设置增益最大值,0,代表最大值65536=64X
    VS_WR_Cmd(SPI_AICTRL3, 6);        //左通道(MIC单声道输入)
    VS_WR_Cmd(SPI_CLOCKF, 0X2000);    //设置VS10XX的时钟,MULT:2倍频;ADD:不允许;CLK:12.288Mhz
    VS_WR_Cmd(SPI_MODE, 0x1804);        //MIC,录音激活
    delay_ms(5);                    //等待至少1.35ms
    VS_Load_Patch((uint16_t *) wav_plugin, 40);//VS1053的WAV录音需要patch
}

//初始化WAV头.
void recoder_wav_init(__WaveHeader *wavhead) //初始化WAV头
{
    wavhead->riff.ChunkID = 0X46464952;    //"RIFF"
    wavhead->riff.ChunkSize = 0;            //还未确定,最后需要计算
    wavhead->riff.Format = 0X45564157;    //"WAVE"
    wavhead->fmt.ChunkID = 0X20746D66;    //"fmt "
    wavhead->fmt.ChunkSize = 16;            //大小为16个字节
    wavhead->fmt.AudioFormat = 0X01;        //0X01,表示PCM;0X11,表示IMA ADPCM
    wavhead->fmt.NumOfChannels = 1;        //单声道
    wavhead->fmt.SampleRate = 8000;        //8Khz采样率 采样速率
    wavhead->fmt.ByteRate = wavhead->fmt.SampleRate * 2;//16位,即2个字节
    wavhead->fmt.BlockAlign = 2;            //块大小,2个字节为一个块
    wavhead->fmt.BitsPerSample = 16;        //16位PCM
    wavhead->data.ChunkID = 0X61746164;    //"data"
    wavhead->data.ChunkSize = 0;            //数据大小,还需要计算
}

//通过时间获取文件名
//仅限在SD卡保存,不支持FLASH DISK保存
//组合成:形如"0:RECORDER/REC20120321210633.wav"的文件名
void recoder_new_pathname(uint8_t *pname) {
    uint8_t res;
    uint16_t index = 0;
    while (index < 0XFFFF) {
        sprintf((char *) pname, "0:/Records/%d.wav", index);
        res = f_open(ftemp, (const TCHAR *) pname, FA_READ);//尝试打开这个文件
        if (res == FR_NO_FILE) {
            f_close(ftemp);
            break;        //该文件名不存在=正是我们需要的.
        }
        index++;
    }
}

static uint8_t data_buff[4096];//开辟4096字节的内存区域//这个一定要大一点，不然播放不了！！！！！！！！
uint16_t decode_time = 0;
uint8_t audio_play_current_state = AUDIO_NONE;

//播放pname这个wav文件（也可以是MP3等）
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
    VS_Restart_Play();                    //重启播放
    VS_Set_All();                            //设置音量等信息
    VS_Set_Vol(190);        //设置音量
    VS_Reset_DecodeTime();    //复位解码时间
    res = f_typetell(pname);                    //得到文件后缀
    if (res == 0x4c)//如果是flac,加载patch
    {
        VS_Load_Patch((uint16_t *) vs1053b_patch, VS1053B_PATCHLEN);
    }
    res = f_open(fmp3, (const TCHAR *) pname, FA_READ);//打开文件
    if (res == 0)//打开成功.
    {
        VS_SPI_SpeedHigh();    //高速
        decode_time = 0;
        while (*play_state != AUDIO_CANCEL) {
            if (*play_state == AUDIO_PLAY) {
                if(audio_play_current_state==AUDIO_HALT){
                    audio_play_current_state = AUDIO_PLAY;
                }
                while (HAL_SD_GetState(&hsd) != HAL_SD_STATE_READY);
                res = f_read(fmp3, data_buff, 4096, (UINT *) &count);//读出4096个字节
                i = 0;
                do//主播放循环
                {
                    if (VS_Send_MusicData(data_buff + i) == 0) { i += 32; }//给VS10XX发送音频数据
                    else {
                        decode_time = VS_Get_DecodeTime();
//                        printf("%d\n", decode_time);//显示播放时间
                    }
                    if (count != 4096 && i >= count)break;
                } while (i < 4096);//循环发送4096个字节
                if (count != 4096 || res != 0) {
                    break;//读完了.
                }
            } else if (*play_state == AUDIO_HALT && audio_play_current_state != AUDIO_HALT)
                audio_play_current_state = AUDIO_HALT;
        }
        f_close(fmp3);
    } else rval = 0XFF;//出现错误
    *play_state = audio_play_current_state = AUDIO_NONE;
    return rval;
}

/**
	功能：录音函数
	变量：
				agc 录音增益倍数
*/


uint8_t audio_recorde(float agc, uint32_t sec) {
    FIL *f_rec = file;//不能将fil文件定义在这里，否则栈溢出（除非malloc栈空间很足），应该定义在函数外边。
    uint8_t rval = 0;
    uint8_t res = 0;
    uint32_t sectorsize = 0;
    uint32_t recsec = 0;
    uint16_t w;
    uint16_t idx = 0;
    UINT count;
    uint8_t pname[30] = {0};//申请30个字节内存,类似"0:RECORDER/REC00001.wav"

    recoder_enter_rec_mode(512 * agc);
    while (VS_RD_Reg(SPI_HDAT1) >> 8);            //等到buf 较为空闲再开始
    pname[0] = 0;                                //pname没有任何文件名
    recoder_new_pathname(pname);            //得到新的名字
    recoder_wav_init(&global_wavheader);                //初始化wav数据
    res = f_open(f_rec, (const TCHAR *) pname, FA_CREATE_ALWAYS | FA_WRITE);
    if (res)            //文件创建失败
    {
        printf("failed1\n");
        rval = 0XFE;    //提示是否存在SD卡
    } else res = f_write(f_rec, (const void *) &global_wavheader, sizeof(__WaveHeader), &count);//写入头数据
    if (rval == 0) {
        while (recsec < sec) {
            w = VS_RD_Reg(SPI_HDAT1);
            if ((w >= 256) && (w < 896)) {
                idx = 0;
                while (idx < 512)    //一次读取512字节
                {
                    w = VS_RD_Reg(SPI_HDAT0);
                    global_buff512[idx++] = w & 0XFF;
                    global_buff512[idx++] = w >> 8;
                }
                res = f_write(f_rec, global_buff512, 512, &count);//写入文件
                if (res) {
                    printf("err\r\n");
                    break;//写入出错.
                }
                sectorsize++;//扇区数增加1,约为32ms
            }
            if (recsec != (sectorsize * 4 / 125))//录音时间显示
            {
                recsec = sectorsize * 4 / 125;
                printf("%d\n", recsec);
            }
        }
        if (res == 0) {
            global_wavheader.riff.ChunkSize = sectorsize * 512 + 36;   //整个文件的大小-8;    sizeof(__WaveHeader)-8=36 bytes
            global_wavheader.data.ChunkSize = sectorsize * 512;        //数据大小
            f_lseek(f_rec, 0);                                  //偏移到文件头.
            f_write(f_rec, (const void *) &global_wavheader, sizeof(__WaveHeader), &count);//写入头数据
            f_close(f_rec);
        }
        uint16_t temp;
        temp = VS_RD_Reg(SPI_MODE);    //读取SPI_MODE的内容
        temp &= ~(1 << 12);
        VS_WR_Cmd(SPI_MODE, temp);
    }
    return rval;
}

















