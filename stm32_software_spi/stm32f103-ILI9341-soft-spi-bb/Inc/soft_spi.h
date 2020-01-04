#ifndef _SOFT_SPI_H
#define _SOFT_SPI_H

#include "main.h"

#define LOW 0
#define HIGH 1

#define SPI_MSB_FIRST 12
#define SPI_LSB_FIRST 13

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_CLOCK_DIV2 2
#define SPI_CLOCK_DIV4 4
#define SPI_CLOCK_DIV8 8
#define SPI_CLOCK_DIV16 16
#define SPI_CLOCK_DIV32 32
#define SPI_CLOCK_DIV64 64
#define SPI_CLOCK_DIV128 128
#define SPI_CLOCK_DIV256 256

uint8_t clock_polar;
uint8_t clock_phase;
    
uint32_t _delay;
uint8_t _order;

//void delay_tics(uint32_t tics);
void soft_spi_init(void);
void soft_spi_begin(void);
void soft_spi_end(void);
void soft_spi_set_bit_order(uint8_t order);
void soft_spi_set_data_mode(uint8_t mode);
void soft_spi_set_clock_divider(uint32_t div);
uint8_t soft_spi_transfer(uint8_t val);
uint16_t soft_spi_transfer16(uint16_t val);
void soft_spi_write(uint16_t val);
void soft_spi_write16(uint16_t val);
void soft_spi_wr(uint16_t val);
void soft_spi_wr16(uint16_t val);

#endif // soft_spi.h
