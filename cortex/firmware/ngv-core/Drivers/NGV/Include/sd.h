#ifndef __SD_H_
#define __SD_H_


#include "halinc.h"

enum {
	CMD_RESET = 0x00,
	CMD_INIT = 0x01,
	CMD_SECTOR = 0x10,
	CMD_ACMD = 0x37,
	CMD_AINIT = 0x29,
	CMD_READ = 0x11,
	CMD_READS = 0x12,
	CMD_APE = 0x17,
	CMD_WRITE = 0x18,
	CMD_WRITES = 0x19,
	CMD_VER = 0x08,
	CMD_NCRC = 0x3B,
	CMD_OCR = 0x3A,
	CMD_CID = 0x0A,
	CMD_CSD = 0x09,
	CMD_RSE = 0x0C
};

enum {
	STATE_READY = 0X00,
	STATE_IDLE = 0X01,
};

enum {
	VER_1 = 0x05,
	VER_2 = 0x01
};

enum SDType {
	TYPE_MMC = 0,
	TYPE_V1 = 1,
	TYPE_V2 = 2,
	TYPE_V2_SDHC = 4
};

typedef struct {
	enum SDType			type;
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
    uint8_t		(*cid)				(pSD* p, uint8_t* cid);
	uint8_t		(*csd)				(pSD* p, uint8_t* csd);
    uint8_t		(*cmd)				(pSD* p, uint8_t cmd, uint32_t arg);
    uint32_t	(*size)				(pSD* p);
    uint8_t		(*read)				(pSD* p, uint32_t sector, uint8_t* buf);
    uint8_t		(*write)			(pSD* p, uint32_t sector, uint8_t* buf);
    uint8_t		(*reads)				(pSD* p, uint32_t sector, uint8_t* buf, uint8_t count);
	uint8_t		(*writes)			(pSD* p, uint32_t sector, uint8_t* buf, uint8_t count);
} SD;

SD* SDInit(SPI_HandleTypeDef* hspi, GPIO_TypeDef* CSGroup, uint16_t CSIndex);


#endif
