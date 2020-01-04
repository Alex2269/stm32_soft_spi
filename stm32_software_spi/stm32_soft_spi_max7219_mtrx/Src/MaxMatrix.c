#include "MaxMatrix_font.h"
#include "MaxMatrix.h"

extern uint8_t MATRIX_COUNT; //how many MAX7219 are connected
uint8_t buff[10];

// Put extracted character on Display
void printCharWithShift(char c, int shift_speed)
{
  if (c < 32) return;
  c -= 32;
  // memcpy_P(buff, font_CH + 7*c, 7); // memcopy for PROGMEM
  // for(uint8_t i=0;i<8;i++)buff[i] = font_CH[i+7*c]; // mem_copy
  memcpy(buff, font_CH + 7*c, 7);
  writeSprite(MATRIX_COUNT*8, 0, buff);
  setColumn(MATRIX_COUNT*8 + buff[0], 0);
  for (int i=0; i<buff[0]+1; i++)
  {
    HAL_Delay(shift_speed);
    shift_Left(false, false);
  }
}

// Extract characters from Scrolling text
void printStringWithShift(char* s, int shift_speed)
{
  while (*s != 0)
  {
    printCharWithShift(*s, shift_speed);
    s++;
  }
}

void Max_Matrix(uint8_t _num)
{
  num = _num;
  for (int i=0; i<80; i++)
  buffer[i] = 0;
}

void MAX_MATRIX_init(void)
{
  setCommand(max7219_reg_scanLimit, 0x07);
  setCommand(max7219_reg_decodeMode, 0x00); // using an led matrix (not digits)
  setCommand(max7219_reg_shutdown, 0x01); // not in shutdown mode
  setCommand(max7219_reg_displayTest, 0x00); // no display test
  // empty registers, turn all LEDs off
  clear();
  setIntensity(0x0f); // the first 0x0f is the value you can set
}

void setIntensity(uint8_t intensity)
{
  setCommand(max7219_reg_intensity, intensity);
}

void clear(void)
{
  for (int i=0; i<8; i++)
  setColumnAll(i,0);
  for (int i=0; i<80; i++)
  buffer[i] = 0;
}

void setCommand(uint8_t command, uint8_t value)
{
  soft_spi_begin();
  for (int i=0; i<num; i++)
  {
    soft_spi_transfer(command);
    soft_spi_transfer(value);
  }
  pin_pulse_low_hi(CS_GPIO_Port, CS_Pin, SPI_CLOCK_DIV128);
  soft_spi_end();
}

void setColumn(uint8_t col, uint8_t value)
{
  int n = col / 8;
  int c = col % 8;
  soft_spi_begin();
  for (int i=0; i<num; i++)
  {
    if (i == n)
    {
      soft_spi_transfer(c + 1);
      soft_spi_transfer(value);
    }
    else
    {
      soft_spi_transfer(0);
      soft_spi_transfer(0);
    }
  }
  pin_pulse_low_hi(CS_GPIO_Port, CS_Pin, SPI_CLOCK_DIV128);
  soft_spi_end();
  buffer[col] = value;
}

void setColumnAll(uint8_t col, uint8_t value)
{
  soft_spi_begin();
  for (int i=0; i<num; i++)
  {
    soft_spi_transfer(col + 1);
    soft_spi_transfer(value);
    buffer[col * i] = value;
  }
  pin_pulse_low_hi(CS_GPIO_Port, CS_Pin, SPI_CLOCK_DIV128);
  soft_spi_end();
}

void setDot(uint8_t col, uint8_t row, uint8_t value)
{
  bitWrite(buffer[col], row, value);
  int n = col / 8;
  int c = col % 8;
  soft_spi_begin();
  for (int i=0; i<num; i++)
  {
    if (i == n)
    {
      soft_spi_transfer(c + 1);
      soft_spi_transfer(buffer[col]);
    }
    else
    {
      soft_spi_transfer(0);
      soft_spi_transfer(0);
    }
  }
  pin_pulse_low_hi(CS_GPIO_Port, CS_Pin, SPI_CLOCK_DIV128);
  soft_spi_end();
}

void writeSprite(int x, int y, const uint8_t* sprite)
{
  int w = sprite[0];
  int h = sprite[1];
  if (h == 8 && y == 0)
  for (int i=0; i<w; i++)
  {
    int c = x + i;
    if (c>=0 && c<80)
    setColumn(c, sprite[i+2]);
  }
  else
  for (int i=0; i<w; i++)
  for (int j=0; j<h; j++)
  {
    int c = x + i;
    int r = y + j;
    if (c>=0 && c<80 && r>=0 && r<8)
    setDot(c, r, bitRead(sprite[i+2], j));
  }
}

void reload(void)
{
  for (int i=0; i<8; i++)
  {
    int col = i;
    soft_spi_begin();
    for (int j=0; j<num; j++)
    {
      soft_spi_transfer(i + 1);
      soft_spi_transfer(buffer[col]);
      col += 8;
    }
    pin_pulse_low_hi(CS_GPIO_Port, CS_Pin, SPI_CLOCK_DIV128);
    soft_spi_end();
  }
}

void shift_Left(bool rotate, bool fill_zero)
{
  uint8_t old = buffer[0];
  for (uint8_t volatile i=0; i<80; i++)
  buffer[i] = buffer[i+1];
  if (rotate) buffer[num*8-1] = old;
  else if (fill_zero) buffer[num*8-1] = 0;
  reload();
}

void shift_Right(bool rotate, bool fill_zero)
{
  int last = num*8-1;
  uint8_t old = buffer[last];
  int i;
  for (i=79; i>0; i--)
  buffer[i] = buffer[i-1];
  if (rotate) buffer[0] = old;
  else if (fill_zero) buffer[0] = 0;
  reload();
}

void shift_Up(bool rotate)
{
  for (int i=0; i<num*8; i++)
  {
    bool b = buffer[i] & 1;
    buffer[i] >>= 1;
    if (rotate) bitWrite(buffer[i], 7, b);
  }
  reload();
}

void shift_Down(bool rotate)
{
  for (int i=0; i<num*8; i++)
  {
    bool b = buffer[i] & 128;
    buffer[i] <<= 1;
    if (rotate) bitWrite(buffer[i], 0, b);
  }
  reload();
}
