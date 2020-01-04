#include "xpt2046.h"

#define CMD_X 0xD1 // inquiry of coordinate X
#define CMD_Y 0x91 // inquiry of coordinate Y
#define CMD_Z 0xB1 // inquiry Z information on pressing

void pin_pen_on(void)
{
  pin_mode(T_IRQ_GPIO_Port, T_IRQ_Pin, GPIO_MODE_INPUT);
}

void get_point(TS_Point *p)
{
  soft_spi_begin();
  pin_write(CS_GPIO_Port, CS_Pin, LOW);

  soft_spi_write16(CMD_X);
  p->x = soft_spi_transfer16(0) >> 3;

  soft_spi_write16(CMD_Y);
  p->y = soft_spi_transfer16(0) >> 3;

  soft_spi_write16(CMD_Z);
  p->z = soft_spi_transfer16(0) >> 3;

  pin_write(CS_GPIO_Port, CS_Pin, HIGH);
  soft_spi_end();
}
