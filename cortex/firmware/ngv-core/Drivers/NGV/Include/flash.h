/*
* Some code from WarMonkey (luoshumymail@gmail.com)
* for more information, please visit bbs.kechuang.org
**/

#ifndef __FLASH_H_
#define __FLASH_H_


#include "halinc.h"

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

Flash* FlashInit(SPI_HandleTypeDef* hspi, GPIO_TypeDef* CSGroup, uint16_t CSIndex, partNumber partnum);


#endif
