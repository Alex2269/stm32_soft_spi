#ifndef LEDMATRIXDRIVER_H_
#define LEDMATRIXDRIVER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "main.h"
#include "pins_ext.h"
#include "soft_spi.h"

#define scrollUp 1
#define scrollDown 2
#define scrollRight 3
#define scrollLeft 4

//commands as defined in the datasheet
const static uint16_t MATRIX_ON = 0x0C00;
const static uint16_t TEST = 0x0F00;
const static uint16_t INTENSITY = 0x0A00;
const static uint16_t SCAN_LIMIT = 0x0B00;
const static uint16_t DECODE = 0x0900;

uint8_t N;
uint8_t* frameBuffer;

bool selfAllocated;

void LEDMatrixDriver(uint8_t Nx);
void setPixel(uint16_t x, uint16_t y, bool enabled);
void setColumn(uint16_t x, uint8_t value);
void setEnabled(bool enabled);
void setIntensity(uint8_t level);
void _sendCommand(uint16_t command);
void _displayRow(uint8_t row);
void display(void);
void drawString(char* text, int len, int x, int y );
void drawSprite( uint8_t* sprite, int x, int y, int width, int height );

#endif /* LEDMATRIXDRIVER_H_ */
