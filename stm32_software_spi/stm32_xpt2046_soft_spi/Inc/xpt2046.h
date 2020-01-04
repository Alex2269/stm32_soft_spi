#ifndef __xpt2046_H
#define __xpt2046_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stdlib.h"
#include "stdbool.h"
#include "stdint.h"
#include "pins_ext.h"
#include "soft_spi.h"

typedef struct TS_Point
{
  uint8_t str_x[8];
  uint8_t str_y[8];
  uint8_t str_z[8];

  int16_t x, y, z;
}TS_Point;

void pin_pen_on(void);
void get_point(TS_Point *p);

#ifdef __cplusplus
}
#endif

#endif /* __xpt2046_H */
