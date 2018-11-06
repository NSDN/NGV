#include "./Include/flash.h"

#include <stdlib.h>
#include <string.h>

void _flash_select(pFlashR* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_RESET);
}

void _flash_deselect(pFlashR* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_SET);
}

uint8_t _flash_transfer(pFlashR* p, uint8_t x) {
	uint8_t data = 0;
	HAL_SPI_TransmitReceive(p->spi, &x, &data, 1, 10);
	return data;
}

uint16_t _flash_readSR(pFlashR* p) {
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

uint8_t _flash_readManufacturer(pFlashR* p) {
    uint8_t c;
    _flash_select(p);
    _flash_transfer(p, R_JEDEC_ID);
    c = _flash_transfer(p, DUMMY_BYTE);
    _flash_transfer(p, 0x00);
    _flash_transfer(p, 0x00);
    _flash_deselect(p);
    return c;
}

uint64_t _flash_readUniqueID(pFlashR* p) {
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

uint16_t _flash_readPartID(pFlashR* p) {
    uint8_t a,b;
    _flash_select(p);
    _flash_transfer(p, R_JEDEC_ID);
    _flash_transfer(p, 0x00);
    a = _flash_transfer(p, DUMMY_BYTE);
    b = _flash_transfer(p, DUMMY_BYTE);
    _flash_deselect(p);
    return (a << 8) | b;
}

uint8_t _flash_checkPartNo(pFlashR* p, partNumber _partno) {
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

uint8_t _flash_busy(pFlashR* p) {
    uint8_t r1 = 0;
    _flash_select(p);
    _flash_transfer(p, R_SR1);
    r1 = _flash_transfer(p, DUMMY_BYTE);
    _flash_deselect(p);
    if(r1 & SR1_BUSY_MASK)
        return 1;
    return 0;
}

void _flash_setWriteEnable(pFlashR* p, uint8_t cmd) {
    _flash_select(p);
    _flash_transfer(p, cmd ? W_EN : W_DE);
    _flash_deselect(p);
}

long _flash_bytes(pFlashR* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].bytes;
    }
    return 0;
}

uint16_t _flash_pages(pFlashR* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].pages;
    }
    return 0;
}

uint16_t _flash_sectors(pFlashR* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].sectors;
    }
    return 0;
}

uint16_t _flash_blocks(pFlashR* p) {
    for(int i = 0; i < sizeof(pnList) / sizeof(pnList[0]); i++) {
        if(p->partno == pnList[i].pn)
            return pnList[i].blocks;
    }
    return 0;
}

uint8_t _flash_begin(pFlashR* p) {
    _flash_select(p);
    _flash_transfer(p, RELEASE);
    _flash_deselect(p);
    HAL_Delay(1); //>3us

    if(!_flash_checkPartNo(p, p->partno)) return 0;
    return 1;
}

void _flash_end(pFlashR* p) {
    _flash_select(p);
    _flash_transfer(p, PDWN);
    _flash_deselect(p);
    HAL_Delay(1); //>3us
}

uint16_t _flash_read(pFlashR* p, uint32_t addr, uint8_t *buf, uint16_t n) {
    uint32_t time = HAL_GetTick();
	while (_flash_busy(p))
		if (HAL_GetTick() - time > 1000)
			return 0;

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

void _flash_writePage(pFlashR* p, uint32_t addr_start, uint8_t *buf) {
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
    uint32_t time = HAL_GetTick();
	while (_flash_busy(p))
		if (HAL_GetTick() - time > 1000)
			break;
	_flash_setWriteEnable(p, 0);
}

void _flash_eraseSector(pFlashR* p, uint32_t addr_start) {
	_flash_setWriteEnable(p, 1);
    _flash_select(p);
    _flash_transfer(p, SECTOR_E);
    _flash_transfer(p, addr_start >> 16);
    _flash_transfer(p, addr_start >> 8);
    _flash_transfer(p, addr_start);
    _flash_deselect(p);
    uint32_t time = HAL_GetTick();
	while (_flash_busy(p))
		if (HAL_GetTick() - time > 1000)
			break;
	_flash_setWriteEnable(p, 0);
}

void _flash_erase32kBlock(pFlashR* p, uint32_t addr_start) {
    _flash_select(p);
    _flash_transfer(p, BLK_E_32K);
    _flash_transfer(p, addr_start >> 16);
    _flash_transfer(p, addr_start >> 8);
    _flash_transfer(p, addr_start);
    _flash_deselect(p);
}

void _flash_erase64kBlock(pFlashR* p, uint32_t addr_start) {
    _flash_select(p);
    _flash_transfer(p, BLK_E_64K);
    _flash_transfer(p, addr_start >> 16);
    _flash_transfer(p, addr_start >> 8);
    _flash_transfer(p, addr_start);
    _flash_deselect(p);
}

uint8_t __check(uint8_t* a, uint8_t* b, uint16_t len) {
	uint16_t sum = 0;
	for (uint16_t i = 0; i < len; i++)
		sum += (a[i] == b[i]);
	return sum == len;
}

#ifdef USE_512BYTE_IO
void _flash_read512byte(pFlashR* p, uint32_t addr, uint8_t *buf) {
	_flash_read(p, addr & 0x00FFFE00, buf, 0x200);
}

void _flash_write512byte(pFlashR* p, uint32_t addr, uint8_t *buf) {
	_flash_read(p, addr & 0x00FFF000, p->buffer, FLASH_SECTOR_SIZ);
	memcpy(p->buffer + ((addr & 0x00FFFE00) - (addr & 0x00FFF000)), buf, 0x200);
	memset(p->check, 0x00, FLASH_SECTOR_SIZ);
	uint8_t cnt = 16;
	while (1) {
		_flash_eraseSector(p, addr & 0x00FFF000);
		for (uint8_t i = 0; i < FLASH_SECTOR_SIZ / FLASH_PAGE_SIZ; i++)
			_flash_writePage(p, (addr & 0x00FFF000) + FLASH_PAGE_SIZ * i, p->buffer + FLASH_PAGE_SIZ * i);
		_flash_read(p, addr & 0x00FFF000, p->check, FLASH_SECTOR_SIZ);
		if (__check(p->buffer, p->check, FLASH_SECTOR_SIZ)) break;
		cnt -= 1; if (cnt == 0) break;
	}
}
#endif

void _flash_readSector(pFlashR* p, uint32_t addr, uint8_t *buf) {
	_flash_read(p, addr & 0x00FFF000, buf, FLASH_SECTOR_SIZ);
}

void _flash_writeSector(pFlashR* p, uint32_t addr, uint8_t *buf) {
	uint8_t cnt = 16; memset(p->check, 0x00, FLASH_SECTOR_SIZ);
	while (1) {
		_flash_eraseSector(p, addr & 0x00FFF000);
		for (uint8_t i = 0; i < FLASH_SECTOR_SIZ / FLASH_PAGE_SIZ; i++)
			_flash_writePage(p, (addr & 0x00FFF000) + FLASH_PAGE_SIZ * i, buf + FLASH_PAGE_SIZ * i);
		_flash_read(p, addr & 0x00FFF000, p->check, FLASH_SECTOR_SIZ);
		if (__check(buf, p->check, FLASH_SECTOR_SIZ)) break;
		cnt -= 1; if (cnt == 0) break;
	}
}

void _flash_eraseAll(pFlashR* p) {
    _flash_select(p);
    _flash_transfer(p, CHIP_ERASE);
    _flash_deselect(p);
}

void _flash_eraseSuspend(pFlashR* p) {
    _flash_select(p);
    _flash_transfer(p, E_SUSPEND);
    _flash_deselect(p);
}

void _flash_eraseResume(pFlashR* p) {
    _flash_select(p);
    _flash_transfer(p, E_RESUME);
    _flash_deselect(p);
}

Flash* FlashInit(SPI_HandleTypeDef* hspi, GPIO_TypeDef* CSGroup, uint16_t CSIndex, partNumber partnum) {
    pFlashR* p = malloc(sizeof(pFlashR));
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
#ifdef USE_512BYTE_IO
    c->read512byte = &_flash_read512byte;
    c->write512byte = &_flash_write512byte;
#endif
    c->readSector = &_flash_readSector;
	c->writeSector = &_flash_writeSector;
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

