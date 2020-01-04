#include "main.h"
#include "max7219.h"
#include "pins_ext.h"
#include "soft_spi.h"

char dg=8;

void Send_7219 (uint8_t rg, uint8_t dt)
{
  soft_spi_begin();
  soft_spi_transfer(rg);
  soft_spi_transfer(dt);
  pin_pulse_low_hi(CS_GPIO_Port, CS_Pin, SPI_CLOCK_DIV128);
  soft_spi_end();
}

void Clear_7219 (void)
{
  uint8_t i=dg;
  do
  {
    Send_7219(i,0xF); // символ пустоты
  }
  while (--i);
}

void Number_7219 (volatile long n)
{
  uint8_t ng=0; // переменная для минуса
  if(n<0)
  {
    ng=1;
    n*=-1;
  }
  uint8_t i=0;
  do
  {
    Send_7219(++i,n%10); // символ цифры
    n/=10;
  }
  while(n);
  if(ng)
  {
    Send_7219(i+1,0x0A); // символ -
  }
}

void Init_7219 (void)
{
  Send_7219(0x09,0xFF); // включим режим декодирования
  Send_7219(0x0B,dg-1); // кол-во используемых разрядов
  Send_7219(0x0A,0x02); // интенсивность свечения
  Send_7219(0x0C,0x01); // включим индикатор
  Clear_7219();
}
