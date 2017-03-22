#include <stdlib.h>
#include "stm32f7xx_hal.h"

/*
* Some code from WarMonkey (luoshumymail@gmail.com)
* for more information, please visit bbs.kechuang.org
**/

#ifndef __FLASH_H_
#define __FLASH_H_


#define W_EN 	0x06	//write enable
#define W_DE	0x04	//write disable
#define R_SR1	0x05	//read status reg 1
#define R_SR2	0x35	//read status reg 2
#define W_SR	0x01	//write status reg
#define PAGE_PGM	0x02	//page program
#define QPAGE_PGM	0x32	//quad input page program
#define BLK_E_64K	0xD8	//block erase 64KB
#define BLK_E_32K	0x52	//block erase 32KB
#define SECTOR_E	0x20	//sector erase 4KB
#define CHIP_ERASE	0xc7	//chip erase
#define CHIP_ERASE2	0x60	//=CHIP_ERASE
#define E_SUSPEND	0x75	//erase suspend
#define E_RESUME	0x7a	//erase resume
#define PDWN		0xb9	//power down
#define HIGH_PERF_M	0xa3	//high performance mode
#define CONT_R_RST	0xff	//continuous read mode reset
#define RELEASE		0xab	//release power down or HPM/Dev ID (deprecated)
#define R_MANUF_ID	0x90	//read Manufacturer and Dev ID (deprecated)
#define R_UNIQUE_ID	0x4b	//read unique ID (suggested)
#define R_JEDEC_ID	0x9f	//read JEDEC ID = Manuf+ID (suggested)
#define READ		0x03
#define FAST_READ	0x0b

#define SR1_BUSY_MASK	0x01
#define SR1_WEN_MASK	0x02

#define WINBOND_MANUF	0xef

#define DEFAULT_TIMEOUT 200

#define DUMMY_BYTE 0xFF

typedef enum {
    custom = -1,
    autoDetect = 0,
    W25Q80 = 1,
    W25Q16 = 2,
    W25Q32 = 4,
    W25Q64 = 8,
    W25Q128 = 16
} partNumber;

typedef struct {
    partNumber pn;
    uint16_t id;
    uint32_t bytes;
    uint32_t pages;
    uint16_t sectors;
    uint16_t blocks;
} pnListType;

const static pnListType pnList[] = {
    { W25Q80, 0x4014,1048576, 4096, 256, 16  },
    { W25Q16, 0x4015,2097152, 8192, 512, 32  },
    { W25Q32, 0x4016,4194304, 16384,1024,64  },
    { W25Q64, 0x4017,8388608, 32768,2048,128 },
    { W25Q128,0x4018,16777216,65536,4096,256 }
};

typedef struct {
    SPI_HandleTypeDef*  spi;
	GPIO_TypeDef*       CSPortGroup;
	uint16_t            CSPortIndex;
    partNumber          partno;
} pFlash;

typedef struct {
    pFlash* p;

    uint8_t     (*begin)            (pFlash* p);
    void        (*end)              (pFlash* p);
#ifdef FLASH_USE_PRIVATE
    void        (*select)           (pFlash* p);
    void        (*deselect)         (pFlash* p);
    uint8_t     (*transfer)         (pFlash* p, uint8_t x);
    uint8_t     (*checkPartNo)      (pFlash* p, partNumber _partno);
#endif
    long        (*bytes)            (pFlash* p);
    uint16_t    (*pages)            (pFlash* p);
    uint16_t    (*sectors)          (pFlash* p);
    uint16_t    (*blocks)           (pFlash* p);

    uint16_t    (*read)             (pFlash* p, uint32_t addr, uint8_t *buf, uint16_t n);

    //WE() every time before write or erase
    void        (*setWriteEnable)   (pFlash* p, uint8_t cmd);

    //write a page, sizeof(buf) is 256 bytes
    //addr is 8bit-aligned, 0x00ffff00
    void        (*writePage)        (pFlash* p, uint32_t addr_start, uint8_t *buf);

    //erase a sector ( 4096bytes ), return false if error
    //addr is 12bit-aligned, 0x00fff000
    void        (*eraseSector)      (pFlash* p, uint32_t addr);

    //erase a 32k block ( 32768b )
    //addr is 15bit-aligned, 0x00ff8000
    void        (*erase32kBlock)    (pFlash* p, uint32_t addr);

    //erase a 64k block ( 65536b )
    //addr is 16bit-aligned, 0x00ff0000
    void        (*erase64kBlock)    (pFlash* p, uint32_t addr);

    //chip erase, return true if successfully started, busy()==false -> erase complete
    void        (*eraseAll)         (pFlash* p);
    void        (*eraseSuspend)     (pFlash* p);
    void        (*eraseResume)      (pFlash* p);
    uint8_t     (*busy)             (pFlash* p);

    uint8_t     (*readManufacturer) (pFlash* p);
    uint16_t    (*readPartID)       (pFlash* p);
    uint64_t    (*readUniqueID)     (pFlash* p);
    uint16_t    (*readSR)           (pFlash* p);
} Flash;

void _flash_select(pFlash* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_RESET);
}

void _flash_deselect(pFlash* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_SET);
}

uint8_t _flash_transfer(pFlash* p, uint8_t x) {
	uint8_t data = 0;
	HAL_SPI_TransmitReceive(p->spi, &x, &data, 1, 1);
	return data;
}

uint16_t _flash_readSR(pFlash* p) {
    uint8_t r1,r2;
    _flash_select(p);
    _flash_transfer(p, R_SR1);
    r1 = _flash_transfer(p, DUMMY_BYTE);
    _flash_deselect(p);
    _flash_deselect(p); //some delay
    _flash_select(p);
    _flash_transfer(p, R_SR2);
    r2 = _flash_transfer(p, DUMMY_BYTE);
    _flash_deselect(p);
    return (((uint16_t)r2) << 8) | r1;
}

uint8_t _flash_readManufacturer(pFlash* p) {
    uint8_t c;
    _flash_select(p);
    _flash_transfer(p, R_JEDEC_ID);
    c = _flash_transfer(p, DUMMY_BYTE);
    _flash_transfer(p, 0x00);
    _flash_transfer(p, 0x00);
    _flash_deselect(p);
    return c;
}

uint64_t _flash_readUniqueID(pFlash* p) {
    uint64_t uid;
    uint8_t *arr;
    arr = (uint8_t*)&uid;
    _flash_select(p);
    _flash_transfer(p, R_UNIQUE_ID);
    _flash_transfer(p, 0x00);
    _flash_transfer(p, 0x00);
    _flash_transfer(p, 0x00);
    _flash_transfer(p, 0x00);
    //for little endian machine only
    for(int i = 7; i >= 0; i--)
    {
        arr[i] = _flash_transfer(p, DUMMY_BYTE);
    }
    _flash_deselect(p);
    return uid;
}

uint16_t _flash_readPartID(pFlash* p) {
    uint8_t a,b;
    _flash_select(p);
    _flash_transfer(p, R_JEDEC_ID);
    _flash_transfer(p, 0x00);
    a = _flash_transfer(p, DUMMY_BYTE);
    b = _flash_transfer(p, DUMMY_BYTE);
    _flash_deselect(p);
    return (a << 8) | b;
}

uint8_t _flash_checkPartNo(pFlash* p, partNumber _partno) {
    uint8_t manuf;
    uint16_t id;

    _flash_select(p);
    _flash_transfer(p, R_JEDEC_ID);
    manuf = _flash_transfer(p, DUMMY_BYTE);
    id = _flash_transfer(p, DUMMY_BYTE) << 8;
    id |= _flash_transfer(p, DUMMY_BYTE);
    _flash_deselect(p);

    if(manuf != WINBOND_MANUF)
        return 0;

    if(_partno == custom)
        return 1;

    if(_partno == autoDetect) {
        for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
            if(id == pnList[i].id) {
                _partno = pnList[i].pn;
                return 1;
            }
        }
        if(_partno == autoDetect) return 0;
    }

    //test chip id and partNo
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(_partno == pnList[i].pn) {
            if(id == pnList[i].id) //id equal
                return 1;
            else
                return 0;
        }
    }
    return 0; //partNo not found
}

uint8_t _flash_busy(pFlash* p) {
    uint8_t r1;
    _flash_select(p);
    _flash_transfer(p, R_SR1);
    r1 = _flash_transfer(p, DUMMY_BYTE);
    _flash_deselect(p);
    if(r1 & SR1_BUSY_MASK)
        return 1;
    return 0;
}

void _flash_setWriteEnable(pFlash* p, uint8_t cmd) {
    _flash_select(p);
    _flash_transfer(p, cmd ? W_EN : W_DE);
    _flash_deselect(p);
}

long _flash_bytes(pFlash* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].bytes;
    }
    return 0;
}

uint16_t _flash_pages(pFlash* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].pages;
    }
    return 0;
}

uint16_t _flash_sectors(pFlash* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].sectors;
    }
    return 0;
}

uint16_t _flash_blocks(pFlash* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].blocks;
    }
    return 0;
}

uint8_t _flash_begin(pFlash* p) {
    _flash_select(p);
    _flash_transfer(p, RELEASE);
    _flash_deselect(p);
    HAL_Delay(1); //>3us

    if(!_flash_checkPartNo(p, p->partno)) return 0;
    return 1;
}

void _flash_end(pFlash* p) {
    _flash_select(p);
    _flash_transfer(p, PDWN);
    _flash_deselect(p);
    HAL_Delay(1); //>3us
}

uint16_t _flash_read(pFlash* p, uint32_t addr, uint8_t *buf, uint16_t n) {
    if(_flash_busy(p)) return 0;

    _flash_select(p);
    _flash_transfer(p, READ);
    _flash_transfer(p, addr >> 16);
    _flash_transfer(p, addr >> 8);
    _flash_transfer(p, addr);
    for(uint16_t i = 0; i < n; i++) {
        buf[i] = _flash_transfer(p, DUMMY_BYTE);
    }
    _flash_deselect(p);

    return n;
}

void _flash_writePage(pFlash* p, uint32_t addr_start, uint8_t *buf) {
	_flash_setWriteEnable(p, 1);
    _flash_select(p);
    _flash_transfer(p, PAGE_PGM);
    _flash_transfer(p, addr_start >> 16);
    _flash_transfer(p, addr_start >> 8);
    _flash_transfer(p, 0x00);
    uint8_t i = 0;
    do {
        _flash_transfer(p, buf[i]);
        i++;
    } while (i != 0);
    _flash_deselect(p);
	while (_flash_busy(p));
	_flash_setWriteEnable(p, 0);
}

void _flash_eraseSector(pFlash* p, uint32_t addr_start) {
	_flash_setWriteEnable(p, 1);
    _flash_select(p);
    _flash_transfer(p, SECTOR_E);
    _flash_transfer(p, addr_start >> 16);
    _flash_transfer(p, addr_start >> 8);
    _flash_transfer(p, addr_start);
    _flash_deselect(p);
	while (_flash_busy(p));
	_flash_setWriteEnable(p, 0);
}

void _flash_erase32kBlock(pFlash* p, uint32_t addr_start) {
    _flash_select(p);
    _flash_transfer(p, BLK_E_32K);
    _flash_transfer(p, addr_start >> 16);
    _flash_transfer(p, addr_start >> 8);
    _flash_transfer(p, addr_start);
    _flash_deselect(p);
}

void _flash_erase64kBlock(pFlash* p, uint32_t addr_start) {
    _flash_select(p);
    _flash_transfer(p, BLK_E_64K);
    _flash_transfer(p, addr_start >> 16);
    _flash_transfer(p, addr_start >> 8);
    _flash_transfer(p, addr_start);
    _flash_deselect(p);
}

void _flash_eraseAll(pFlash* p) {
    _flash_select(p);
    _flash_transfer(p, CHIP_ERASE);
    _flash_deselect(p);
}

void _flash_eraseSuspend(pFlash* p) {
    _flash_select(p);
    _flash_transfer(p, E_SUSPEND);
    _flash_deselect(p);
}

void _flash_eraseResume(pFlash* p) {
    _flash_select(p);
    _flash_transfer(p, E_RESUME);
    _flash_deselect(p);
}

Flash* FlashInit(SPI_HandleTypeDef* hspi, GPIO_TypeDef* CSGroup, uint16_t CSIndex, partNumber partnum) {
    pFlash* p = malloc(sizeof(pFlash));
    p->spi = hspi;
    p->CSPortGroup = CSGroup;
    p->CSPortIndex = CSIndex;
    p->partno = partnum;

    Flash* c = malloc(sizeof(Flash));
    c->p = p;

    c->begin = &_flash_begin;
    c->end = &_flash_end;
#ifdef FLASH_USE_PRIVATE
    c->select = &_flash_select;
    c->deselect = &_flash_deselect;
    c->transfer = &_flash_transfer;
    c->checkPartNo = &_flash_checkPartNo;
#endif
    c->bytes = &_flash_bytes;
    c->pages = &_flash_pages;
    c->sectors = &_flash_sectors;
    c->blocks = &_flash_blocks;
    c->read = &_flash_read;
    c->setWriteEnable = &_flash_setWriteEnable;
    c->writePage = &_flash_writePage;
    c->eraseSector = &_flash_eraseSector;
    c->erase32kBlock = &_flash_erase32kBlock;
    c->erase64kBlock = &_flash_erase64kBlock;
    c->eraseAll = &_flash_eraseAll;
    c->eraseSuspend = &_flash_eraseSuspend;
    c->eraseResume = &_flash_eraseResume;
    c->busy = &_flash_busy;
    c->readManufacturer = &_flash_readManufacturer;
    c->readPartID = &_flash_readPartID;
    c->readUniqueID = &_flash_readUniqueID;
    c->readSR = &_flash_readSR;

    return c;
}


#endif
