#include "./Include/spidev.h"

#include <stdlib.h>

HAL_StatusTypeDef SPI_WaitOnFlagUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
	uint32_t tickstart = 0;

	/* Get tick */
	tickstart = HAL_GetTick();

	/* Wait until flag is set */
	if (Status == RESET)
	{
		while (__HAL_SPI_GET_FLAG(hspi, Flag) == RESET)
		{
			if (Timeout != HAL_MAX_DELAY)
			{
				if ((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
				{
					/* Disable the SPI and reset the CRC: the CRC value should be cleared
					on both master and slave sides in order to resynchronize the master
					and slave for their respective CRC calculation */

					/* Disable TXE, RXNE and ERR interrupts for the interrupt process */
					__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

					/* Disable SPI peripheral */
					__HAL_SPI_DISABLE(hspi);

					/* Reset CRC Calculation */
					if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
					{
						SPI_RESET_CRC(hspi);
					}

					hspi->State = HAL_SPI_STATE_READY;

					/* Process Unlocked */
					__HAL_UNLOCK(hspi);

					return HAL_TIMEOUT;
				}
			}
		}
	}
	else
	{
		while (__HAL_SPI_GET_FLAG(hspi, Flag) != RESET)
		{
			if (Timeout != HAL_MAX_DELAY)
			{
				if ((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
				{
					/* Disable the SPI and reset the CRC: the CRC value should be cleared
					on both master and slave sides in order to resynchronize the master
					and slave for their respective CRC calculation */

					/* Disable TXE, RXNE and ERR interrupts for the interrupt process */
					__HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

					/* Disable SPI peripheral */
					__HAL_SPI_DISABLE(hspi);

					/* Reset CRC Calculation */
					if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
					{
						SPI_RESET_CRC(hspi);
					}

					hspi->State = HAL_SPI_STATE_READY;

					/* Process Unlocked */
					__HAL_UNLOCK(hspi);

					return HAL_TIMEOUT;
				}
			}
		}
	}
	return HAL_OK;
}

HAL_StatusTypeDef MOD_HAL_SPI_Transmit_Same8(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Count, uint32_t Timeout)
{

	if (hspi->State == HAL_SPI_STATE_READY)
	{
		if ((pData == NULL) || (Count == 0))
		{
			return  HAL_ERROR;
		}

		/* Check the parameters */
		assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));

		/* Process Locked */
		__HAL_LOCK(hspi);

		/* Configure communication */
		hspi->State = HAL_SPI_STATE_BUSY_TX;
		hspi->ErrorCode = HAL_SPI_ERROR_NONE;

		hspi->pTxBuffPtr = pData;
		hspi->TxXferSize = Count;
		hspi->TxXferCount = Count;

		/*Init field not used in handle to zero */
		hspi->TxISR = 0;
		hspi->RxISR = 0;
		hspi->pRxBuffPtr = NULL;
		hspi->RxXferSize = 0;
		hspi->RxXferCount = 0;

		/* Reset CRC Calculation */
		if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
		{
			SPI_RESET_CRC(hspi);
		}

		if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
		{
			/* Configure communication direction : 1Line */
			SPI_1LINE_TX(hspi);
		}

		/* Check if the SPI is already enabled */
		if ((hspi->Instance->CR1 &SPI_CR1_SPE) != SPI_CR1_SPE)
		{
			/* Enable SPI peripheral */
			__HAL_SPI_ENABLE(hspi);
		}

		/* Transmit data in 8 Bit mode */
		if (hspi->Init.DataSize == SPI_DATASIZE_8BIT)
		{
			while (hspi->TxXferCount > 0)
			{
				/* Wait until TXE flag is set to send data */
				if (SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_TXE, RESET, Timeout) != HAL_OK)
				{
					return HAL_TIMEOUT;
				}
				hspi->Instance->DR = (*hspi->pTxBuffPtr);
				hspi->TxXferCount--;
			}
			/* Enable CRC Transmission */
			if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
			{
				SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
			}
		}

		/* Wait until TXE flag is set to send data */
		if (SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_TXE, RESET, Timeout) != HAL_OK)
		{
			SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
			return HAL_TIMEOUT;
		}

		/* Wait until Busy flag is reset before disabling SPI */
		if (SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_BSY, SET, Timeout) != HAL_OK)
		{
			SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
			return HAL_TIMEOUT;
		}

		/* Clear OVERUN flag in 2 Lines communication mode because received is not read */
		if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
		{
			__HAL_SPI_CLEAR_OVRFLAG(hspi);
		}

		hspi->State = HAL_SPI_STATE_READY;

		/* Process Unlocked */
		__HAL_UNLOCK(hspi);

		return HAL_OK;
	}
	else
	{
		return HAL_BUSY;
	}
}

HAL_StatusTypeDef MOD_HAL_SPI_Transmit_Same16(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Count, uint32_t Timeout)
{

	if (hspi->State == HAL_SPI_STATE_READY)
	{
		if ((pData == NULL) || (Count == 0))
		{
			return  HAL_ERROR;
		}

		/* Check the parameters */
		assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE(hspi->Init.Direction));

		/* Process Locked */
		__HAL_LOCK(hspi);

		/* Configure communication */
		hspi->State = HAL_SPI_STATE_BUSY_TX;
		hspi->ErrorCode = HAL_SPI_ERROR_NONE;

		hspi->pTxBuffPtr = pData;
		hspi->TxXferSize = Count;
		hspi->TxXferCount = Count;

		/*Init field not used in handle to zero */
		hspi->TxISR = 0;
		hspi->RxISR = 0;
		hspi->pRxBuffPtr = NULL;
		hspi->RxXferSize = 0;
		hspi->RxXferCount = 0;

		/* Reset CRC Calculation */
		if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
		{
			SPI_RESET_CRC(hspi);
		}

		if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
		{
			/* Configure communication direction : 1Line */
			SPI_1LINE_TX(hspi);
		}

		/* Check if the SPI is already enabled */
		if ((hspi->Instance->CR1 &SPI_CR1_SPE) != SPI_CR1_SPE)
		{
			/* Enable SPI peripheral */
			__HAL_SPI_ENABLE(hspi);
		}

		/* Transmit data in 8 Bit mode */
		if (hspi->Init.DataSize == SPI_DATASIZE_8BIT)
		{
			while (hspi->TxXferCount > 0)
			{
				/* Wait until TXE flag is set to send data */
				if (SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_TXE, RESET, Timeout) != HAL_OK)
				{
					return HAL_TIMEOUT;
				}
				hspi->Instance->DR = (*hspi->pTxBuffPtr++);
				if (hspi->pTxBuffPtr - pData > 1) {
					hspi->pTxBuffPtr = pData;
					hspi->TxXferCount--;
				}
			}
			/* Enable CRC Transmission */
			if (hspi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
			{
				SET_BIT(hspi->Instance->CR1, SPI_CR1_CRCNEXT);
			}
		}

		/* Wait until TXE flag is set to send data */
		if (SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_TXE, RESET, Timeout) != HAL_OK)
		{
			SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
			return HAL_TIMEOUT;
		}

		/* Wait until Busy flag is reset before disabling SPI */
		if (SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_BSY, SET, Timeout) != HAL_OK)
		{
			SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
			return HAL_TIMEOUT;
		}

		/* Clear OVERUN flag in 2 Lines communication mode because received is not read */
		if (hspi->Init.Direction == SPI_DIRECTION_2LINES)
		{
			__HAL_SPI_CLEAR_OVRFLAG(hspi);
		}

		hspi->State = HAL_SPI_STATE_READY;

		/* Process Unlocked */
		__HAL_UNLOCK(hspi);

		return HAL_OK;
	}
	else
	{
		return HAL_BUSY;
	}
}

void _spi_dev_start(pSPIDevice* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_RESET);
}

void _spi_dev_dcs(pSPIDevice* p, uint8_t data) {
	HAL_GPIO_WritePin(p->DCPortGroup, p->DCPortIndex, (data > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void _spi_dev_send(pSPIDevice* p, uint8_t byte) {
	//HAL_SPI_Transmit_DMA(p->spi, &byte, 1);
	HAL_SPI_Transmit(p->spi, &byte, 1, 1);
}

void _spi_dev_write(pSPIDevice* p, uint16_t* word, uint32_t length) {
	//HAL_SPI_Transmit_DMA(p->spi, &byte, length);
	HAL_SPI_Transmit(p->spi, (uint8_t*)word, length, 1);
}

uint8_t _spi_dev_read(pSPIDevice* p) {
	uint8_t byte = 0;
	//HAL_SPI_Receive_DMA(p->spi, &byte, 1);
	HAL_SPI_Receive(p->spi, &byte, 1, 1);
	return byte;
}

void _spi_dev_stop(pSPIDevice* p) {
	HAL_GPIO_WritePin(p->CSPortGroup, p->CSPortIndex, GPIO_PIN_SET);
}

void _spi_dev_sends(pSPIDevice* p, uint8_t byte, uint32_t count) {
	MOD_HAL_SPI_Transmit_Same8(p->spi, (&byte), count, 1);
}

void _spi_dev_writes(pSPIDevice* p, uint16_t word, uint32_t count) {
	MOD_HAL_SPI_Transmit_Same16(p->spi, (uint8_t*)(&word), count, 1);
}

SPIDevice* SPIDeviceInit(
		SPI_HandleTypeDef* pspi,
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex,
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex) {
	pSPIDevice* p = malloc(sizeof(pSPIDevice));
	p->spi = pspi;
	p->DCPortGroup = pDCPortGroup;
	p->DCPortIndex = pDCPortIndex;
	p->CSPortGroup = pCSPortGroup;
	p->CSPortIndex = pCSPortIndex;

	SPIDevice* c = malloc(sizeof(SPIDevice));
	c->p = p;
	c->start = &_spi_dev_start;
	c->stop = &_spi_dev_stop;
	c->dcs = &_spi_dev_dcs;
	c->send = &_spi_dev_send;
	c->write = &_spi_dev_write;
	c->sends = &_spi_dev_sends;
	c->writes = &_spi_dev_writes;
	c->read = &_spi_dev_read;

	return c;
}

void _soft_spi_send(pSPIDevice* p, uint8_t byte) {
#define __setpin(port, pin, state) (port)->BSRR = (state) ? (pin) : (pin << 16)
	for (uint8_t i = 0; i < 8; i++) {
		__setpin(p->SCLPortGroup, p->SCLPortIndex, 0);
		__setpin(p->SDAPortGroup, p->SDAPortIndex, byte & (0x80 >> i));
		__setpin(p->SCLPortGroup, p->SCLPortIndex, 1);
	}
#undef __setpin
}

void _soft_spi_write(pSPIDevice* p, uint16_t* word, uint32_t length) {
	// here length is byte count
	for (uint32_t i = 0; i < length / 2; i++) {
		_soft_spi_send(p, (*(word + i) >> 8) & 0xFF);
		_soft_spi_send(p, *(word + i) & 0xFF);
	}
}

uint8_t _soft_spi_read(pSPIDevice* p) {
	return 0x32;
}

void _soft_spi_sends(pSPIDevice* p, uint8_t byte, uint32_t count) {
	for (uint32_t i = 0; i < count; i++)
		_soft_spi_send(p, byte);
}

void _soft_spi_writes(pSPIDevice* p, uint16_t word, uint32_t count) {
	for (uint32_t i = 0; i < count; i++) {
		_soft_spi_send(p, (word >> 8) & 0xFF);
		_soft_spi_send(p, word & 0xFF);
	}

}

SPIDevice* SoftSPIInit(
		GPIO_TypeDef* pSDAPortGroup, uint16_t pSDAPortIndex,
		GPIO_TypeDef* pSCLPortGroup, uint16_t pSCLPortIndex,
		GPIO_TypeDef* pDCPortGroup, uint16_t pDCPortIndex,
		GPIO_TypeDef* pCSPortGroup, uint16_t pCSPortIndex) {
	pSPIDevice* p = malloc(sizeof(pSPIDevice));
	p->SDAPortGroup = pSDAPortGroup;
	p->SDAPortIndex = pSDAPortIndex;
	p->SCLPortGroup = pSCLPortGroup;
	p->SCLPortIndex = pSCLPortIndex;
	p->DCPortGroup = pDCPortGroup;
	p->DCPortIndex = pDCPortIndex;
	p->CSPortGroup = pCSPortGroup;
	p->CSPortIndex = pCSPortIndex;

	SPIDevice* c = malloc(sizeof(SPIDevice));
	c->p = p;
	c->start = &_spi_dev_start;
	c->stop = &_spi_dev_stop;
	c->dcs = &_spi_dev_dcs;
	c->send = &_soft_spi_send;
	c->write = &_soft_spi_write;
	c->sends = &_soft_spi_sends;
	c->writes = &_soft_spi_writes;
	c->read = &_soft_spi_read;

	return c;
}
