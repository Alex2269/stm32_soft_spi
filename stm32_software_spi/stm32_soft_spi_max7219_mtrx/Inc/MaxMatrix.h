#ifndef _MaxMatrix_H_
#define _MaxMatrix_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pins_ext.h"
#include "soft_spi.h"

#define max7219_reg_noop 0x00
#define max7219_reg_digit0 0x01
#define max7219_reg_digit1 0x02
#define max7219_reg_digit2 0x03
#define max7219_reg_digit3 0x04
#define max7219_reg_digit4 0x05
#define max7219_reg_digit5 0x06
#define max7219_reg_digit6 0x07
#define max7219_reg_digit7 0x08
#define max7219_reg_decodeMode 0x09
#define max7219_reg_intensity 0x0a
#define max7219_reg_scanLimit 0x0b
#define max7219_reg_shutdown 0x0c
#define max7219_reg_displayTest 0x0f

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

uint8_t num;
uint8_t buffer[80];
void reload(void);
void Max_Matrix(/*uint8_t _data, uint8_t _load, uint8_t _clock,*/ uint8_t _num);
void MAX_MATRIX_init(void);
void init(void);
void clear(void);
void setCommand(uint8_t command, uint8_t value);
void setIntensity(uint8_t intensity);
void setColumn(uint8_t col, uint8_t value);
void setColumnAll(uint8_t col, uint8_t value);
void setDot(uint8_t col, uint8_t row, uint8_t value);
void writeSprite(int x, int y, const uint8_t* sprite);
//
void shift_Left(bool rotate, bool fill_zero);
void shift_Right(bool rotate, bool fill_zero);
void shift_Up(bool rotate);
void shift_Down(bool rotate);
void printStringWithShift(char* s, int shift_speed);

#endif
