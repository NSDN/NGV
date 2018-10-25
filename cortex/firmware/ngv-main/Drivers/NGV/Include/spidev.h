#ifndef __SPIDEV_H_
#define __SPIDEV_H_


#include "halinc.h"

typedef struct {
	SPI_HandleTypeDef* spi;

	GPIO_TypeDef* SDAPortGroup;
		uint16_t SDAPortIndex;
	GPIO_TypeDef* SCLPortGroup;
		uint16_t SCLPortIndex;

	GPIO_TypeDef* DCPortGroup;
	uint16_t DCPortIndex;
	GPIO_TypeDef* CSPortGroup;
	uint16_t CSPortIndex;
} pSPIDevice;

typedef struct {
	pSPIDevice* p;
	void (*start)(pSPIDevice* p);
	void (*stop)(pSPIDevice* p);
	void (*dcs)(pSPIDevice* p, uint8_t data);
	void (*send)(pSPIDevice* p, uint8_t byte);
	void (*write)(pSPIDevice* p, uint16_t* word, uint32_t length);
	void (*sends)(pSPIDevice* p, uint8_t byte, uint32_t count);
	void (*writes)(pSPIDevice* p, uint16_t word, uint32_t count);
	uint8_t (*read)(pSPIDevice* p);
} SPIDevice;

SPIDevice* SPIDeviceInit(
		SPI_HandleTypeDef* pspi,
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex,
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex);

SPIDevice* SoftSPIInit(
		GPIO_TypeDef* pSDAPortGroup, uint16_t pSDAPortIndex,
		GPIO_TypeDef* pSCLPortGroup, uint16_t pSCLPortIndex,
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex,
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex);


#endif
