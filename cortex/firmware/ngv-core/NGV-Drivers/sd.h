#include <stdlib.h>
#include "stm32f7xx_hal.h"


#ifndef __SD_H_
#define __SD_H_


enum {
	CMD_RESET = 0x00,
	CMD_INIT = 0x01,
	CMD_ACMD = 0x37,
	CMD_AINIT = 0x29,
	CMD_READ = 0x11,
	CMD_WRITE = 0x18
};

enum {
	STATE_READY = 0X00,
	STATE_IDLE = 0X01,
};

typedef struct {
    SPI_HandleTypeDef*  spi;
	GPIO_TypeDef*       CSPortGroup;
	uint16_t            CSPortIndex;
} pSD;

typedef struct {
    pSD* p;

    uint8_t     (*init)            (pSD* p);
#ifdef SD_USE_PRIVATE
    void        (*select)           (pSD* p);
    void        (*deselect)         (pSD* p);
    uint8_t     (*transfer)         (pSD* p, uint8_t x);
#endif
    uint8_t		(*cmd)				(pSD* p, uint8_t cmd, uint32_t arg);
    uint8_t		(*read)				(pSD* p, uint32_t sector, uint8_t* buf);
    uint8_t		(*write)			(pSD* p, uint32_t sector, uint8_t* buf);
} SD;

void _sd_select(pSD* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_RESET);
}

void _sd_deselect(pSD* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_SET);
}

uint8_t _sd_transfer(pSD* p, uint8_t x) {
	uint8_t data = 0;
	HAL_SPI_TransmitReceive(p->spi, &x, &data, 1, 1);
	return data;
}

uint8_t _sd_cmd(pSD* p, uint8_t cmd, uint32_t arg) {
	uint8_t response = 0, retry = 0;
	_sd_select(p);
	_sd_transfer(p, cmd | 0x40);
	_sd_transfer(p, arg >> 24);
	_sd_transfer(p, arg >> 16);
	_sd_transfer(p, arg >> 8);
	_sd_transfer(p, arg);
	uint8_t crc = 0xFF;
	if (cmd == CMD_RESET) crc = 0x95;
	_sd_transfer(p, crc);

	do {
		response = _sd_transfer(p, 0xFF);
		retry += 1;
	}  while (((response & 0x80) != 0) && (retry < 200));
	_sd_deselect(p);
	_sd_transfer(p, 0xFF);
	return response;
}

uint8_t _sd_cmd_ncs_(pSD* p, uint8_t cmd, uint32_t arg) {
	uint8_t response = 0, retry = 0;
	_sd_transfer(p, cmd | 0x40);
	_sd_transfer(p, arg >> 24);
	_sd_transfer(p, arg >> 16);
	_sd_transfer(p, arg >> 8);
	_sd_transfer(p, arg);
	uint8_t crc = 0xFF;
	if (cmd == CMD_RESET) crc = 0x95;
	_sd_transfer(p, crc);

	do {
		response = _sd_transfer(p, 0xFF);
		retry += 1;
	}  while (((response & 0x80) != 0) && (retry < 200));
	return response;
}

#define SPI_BAUDRATEPRESCALER_CLR ~SPI_BAUDRATEPRESCALER_256
uint8_t _sd_init(pSD* p) {
	p->spi->Instance->CR1 &= SPI_BAUDRATEPRESCALER_CLR;
	p->spi->Instance->CR1 |= SPI_BAUDRATEPRESCALER_256;

	_sd_deselect(p);
	for (uint8_t i = 0; i < 10; i++) {
		_sd_transfer(p, 0xFF);
	}
	_sd_select(p);

	uint8_t response = 0;

	for (uint16_t i = 0; ; i++) {
		response = _sd_cmd(p, CMD_RESET, 0);
		if (response == STATE_IDLE) break;
		if (i == 0x01FF) {
			_sd_deselect(p);
			return 3;
		}
	}

	for (uint16_t i = 0; ; i++) {
		response = _sd_cmd(p, CMD_ACMD, 0);
		if (response == STATE_IDLE) break;
		if (i == 0x01FF) {
			_sd_deselect(p);
			return 2;
		}
	}

	for (uint16_t i = 0; ; i++) {
		response = _sd_cmd(p, CMD_AINIT, 0);
		if (response == STATE_READY) break;
		if (i == 0x01FF) {
			_sd_deselect(p);
			return 1;
		}
	}

	_sd_deselect(p);

	p->spi->Instance->CR1 &= SPI_BAUDRATEPRESCALER_CLR;
	p->spi->Instance->CR1 |= SPI_BAUDRATEPRESCALER_2;
	return 0;
}

uint8_t _sd_read(pSD* p, uint32_t sector, uint8_t* buf) {
	uint8_t response = 0; uint16_t retry = 0;
	_sd_select(p);

	response = _sd_cmd_ncs_(p, CMD_READ, sector << 9);
	if (response != STATE_READY) {
		_sd_deselect(p);
		_sd_transfer(p, 0xFF);
		return response;
	}

	while (_sd_transfer(p, 0xFF) != 0xFE) {
		if (++retry > 0xFFFE) {
			_sd_deselect(p);
			_sd_transfer(p, 0xFF);
			return 1;
		}
	}
	for (uint16_t i = 0; i < 512; i++) {
		*buf++ = _sd_transfer(p, 0xFF);
	}
	_sd_transfer(p, 0xFF); //CRC1
	_sd_transfer(p, 0xFF); //CRC2
	_sd_deselect(p);
	_sd_transfer(p, 0xFF);
	return 0;
}

uint8_t _sd_write(pSD* p, uint32_t sector, uint8_t* buf) {
	uint8_t response = 0; uint16_t retry = 0;
	_sd_select(p);

	response = _sd_cmd_ncs_(p, CMD_WRITE, sector << 9);
	if (response != STATE_READY) {
		_sd_deselect(p);
		_sd_transfer(p, 0xFF);
		return response;
	}

	_sd_transfer(p, 0xFF);
	_sd_transfer(p, 0xFF);
	_sd_transfer(p, 0xFF);
	_sd_transfer(p, 0xFE);
	for (uint16_t i = 0; i < 512; i++) {
		 _sd_transfer(p, *buf++);
	}
	_sd_transfer(p, 0xFF); //CRC1
	_sd_transfer(p, 0xFF); //CRC2

	response = _sd_transfer(p, 0xFF);
	if ((response & 0x1F) != 0x05) {
		_sd_deselect(p);
		_sd_transfer(p, 0xFF);
		return response;
	}
	while (_sd_transfer(p, 0xFF) != 0x00) {
		if (++retry > 0xFFFE) {
			_sd_deselect(p);
			_sd_transfer(p, 0xFF);
			return 1;
		}
	}
	_sd_deselect(p);
	_sd_transfer(p, 0xFF);
	return 0;
}

SD* SDInit(SPI_HandleTypeDef* hspi, GPIO_TypeDef* CSGroup, uint16_t CSIndex) {
    pSD* p = malloc(sizeof(pSD));
    p->spi = hspi;
    p->CSPortGroup = CSGroup;
    p->CSPortIndex = CSIndex;

    SD* c = malloc(sizeof(SD));
    c->p = p;

    c->init = &_sd_init;
#ifdef SD_USE_PRIVATE
    c->select = &_sd_select;
    c->deselect = &_sd_deselect;
    c->transfer = &_sd_transfer;
#endif
    c->cmd = &_sd_cmd;
    c->read = &_sd_read;
    c->write = &_sd_write;

    return c;
}


#endif
