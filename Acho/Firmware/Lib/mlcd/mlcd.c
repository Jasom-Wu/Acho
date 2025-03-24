#include <memory.h>
#include "mlcd.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;       //SPI interface for display

MLCD mlcd;
static uint8_t mlcd_buff[LCD_WIDTH * LCD_HEIGHT * 3 / 8]={0};
void mlcd_Init(void) {
  mlcd_Clear();
  HAL_Delay(1000);
  HAL_GPIO_WritePin(DISP_DISPLAY_GPIO_Port, DISP_DISPLAY_Pin, GPIO_PIN_SET);
  mlcd.CurrentX = 0;
  mlcd.CurrentY = 0;
  mlcd.Inverted = FALSE;
  mlcd.Initialized = TRUE;

  mlcd.BufferSize = LCD_WIDTH * LCD_HEIGHT * 3 / 8;
  mlcd.Buffer = mlcd_buff;
}

void mlcd_Clear(void) {
  uint8_t clear_data[2] = {BIT_CLEAR, 0x00};

  HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_SET);
  HAL_SPI_Transmit(&hspi1, &clear_data[0], 1, HAL_MAX_DELAY);
  HAL_SPI_Transmit(&hspi1, &clear_data[1], 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_RESET);

  mlcd.CurrentX = 0;
  mlcd.CurrentY = 0;

  memset(mlcd.Buffer, 0xFF, mlcd.BufferSize);
//  mlcd_Test(0);
}
void mlcd_DrawPixel(uint8_t x, uint8_t y, LCD_COLOR color) {
  if (x >= LCD_WIDTH || y >= LCD_HEIGHT) {
    return;
  }
  // x : 0~7 is a group, 8~15 is a group, etc...
  // a group fill 3 bytes: [R0 G0 B0 R1 G1 B1 R2 G2][B2 R3 G3 B3 R4 G4 B4 R5][G5 B5 R6 G6 B6 R7 G7 B7]
  // so a line have 0~15 group (128*3/8 = 48 bytes a line, 48 / 3 = 16)
  uint16_t y_offset = (LCD_WIDTH * 3 / 8) * y;
  uint8_t group = x / 8, group_offset = x - 8 * group;

//  if (group_offset < 0 || group_offset > 7) return;

  switch (group_offset) {
    case 0:mlcd.Buffer[y_offset + group * 3] &= 0b11111000;
      mlcd.Buffer[y_offset + group * 3] |= (color << 0);
      break;
    case 1:mlcd.Buffer[y_offset + group * 3] &= 0b11000111;
      mlcd.Buffer[y_offset + group * 3] |= (color << 3);
      break;
    case 2:mlcd.Buffer[y_offset + group * 3] &= 0b00111111;
      mlcd.Buffer[y_offset + group * 3 + 1] &= 0b11111110;
      mlcd.Buffer[y_offset + group * 3] |= (color << 6);
      mlcd.Buffer[y_offset + group * 3 + 1] |= (color >> 2);
      break;
    case 3:mlcd.Buffer[y_offset + group * 3 + 1] &= 0b11110001;
      mlcd.Buffer[y_offset + group * 3 + 1] |= (color << 1);
      break;
    case 4:mlcd.Buffer[y_offset + group * 3 + 1] &= 0b10001111;
      mlcd.Buffer[y_offset + group * 3 + 1] |= (color << 4);
      break;
    case 5:mlcd.Buffer[y_offset + group * 3 + 1] &= 0b01111111;
      mlcd.Buffer[y_offset + group * 3 + 2] &= 0b11111100;
      mlcd.Buffer[y_offset + group * 3 + 1] |= (color << 7);
      mlcd.Buffer[y_offset + group * 3 + 2] |= (color >> 1);
      break;
    case 6:mlcd.Buffer[y_offset + group * 3 + 2] &= 0b11100011;
      mlcd.Buffer[y_offset + group * 3 + 2] |= (color << 2);
      break;
    case 7:mlcd.Buffer[y_offset + group * 3 + 2] &= 0b00011111;
      mlcd.Buffer[y_offset + group * 3 + 2] |= (color << 5);
      break;
    default:break;
  }
}
void mlcd_Refresh(void) {
  uint8_t cmd_data = BIT_WRITECMD;
  uint8_t adr_data = 0x01;
  uint8_t dmy_data = 0x00;

  HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_SET);

  HAL_SPI_Transmit(&hspi1, &cmd_data, 1, HAL_MAX_DELAY);

  for (uint16_t s_add = 1; s_add <= LCD_HEIGHT; s_add++) {
    adr_data = s_add;
    HAL_SPI_Transmit(&hspi1, &adr_data, 1, HAL_MAX_DELAY);
    uint16_t step = LCD_WIDTH * 3 / 8;
    for (uint16_t s_data = 0; s_data < LCD_WIDTH * 3 / 8; s_data++) {
      uint16_t index = s_data + (s_add - 1) * step;
      HAL_SPI_Transmit(&hspi1, &mlcd.Buffer[index], 1, HAL_MAX_DELAY);
    }
    HAL_SPI_Transmit(&hspi1, &dmy_data, 1, HAL_MAX_DELAY);
  }
  HAL_SPI_Transmit(&hspi1, &dmy_data, 1, HAL_MAX_DELAY);
  HAL_Delay(1);
  HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_RESET);
}

void mlcd_fill(uint8_t color) {
  uint8_t cmd = BIT_WRITECMD;
  uint8_t dummy_data = 0x00;
  HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_SET);
  HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
  for(uint8_t l=1;l<=128;l++){
    HAL_SPI_Transmit(&hspi1, &l, 1, HAL_MAX_DELAY);
    for(uint8_t p=1;p<=48;p++){
      HAL_SPI_Transmit(&hspi1, &color, 1, HAL_MAX_DELAY);
    }
    HAL_SPI_Transmit(&hspi1, &dummy_data, 1, HAL_MAX_DELAY);
  }
  HAL_SPI_Transmit(&hspi1, &dummy_data, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_RESET);
}