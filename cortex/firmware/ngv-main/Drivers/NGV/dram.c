#include "./Include/dram.h"

void SDRAM_Init(SDRAM_HandleTypeDef *hsdram) {
	__IO uint32_t tmpmrd =0;
	FMC_SDRAM_CommandTypeDef Command;
	/* Step 3:  Configure a clock configuration enable command */
	Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &Command, ((uint32_t)0xFFFF));

	/* Step 4: Insert 1 ms minimum delay */
	/* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
	HAL_Delay(1);

	/* Step 5: Configure a PALL (precharge all) command */
	Command.CommandMode = FMC_SDRAM_CMD_PALL;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &Command, ((uint32_t)0xFFFF));

	/* Step 6 : Configure a Auto-Refresh command */
	Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 4;
	Command.ModeRegisterDefinition = 0;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &Command, ((uint32_t)0xFFFF));

	/* Step 7: Program the external memory mode register */
	tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2        |
					 SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
					 SDRAM_MODEREG_CAS_LATENCY_2           |
					 SDRAM_MODEREG_OPERATING_MODE_STANDARD |
					 SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	Command.AutoRefreshNumber = 1;
	Command.ModeRegisterDefinition = tmpmrd;

	/* Send the command */
	HAL_SDRAM_SendCommand(hsdram, &Command, ((uint32_t)0xFFFF));

	/* Step 8: Set the refresh rate counter */
	/* (7.81 us x (HCLK / 2)) - 20 */
	/* Set the device refresh counter */
	hsdram->Instance->SDRTR |= ((uint32_t)((448)<< 1));

}
