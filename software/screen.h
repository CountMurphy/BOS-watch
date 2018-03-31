#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "fsl_spi_master_driver.h"


uint8_t dictionary(char);
void spiLatch();
uint8_t addDot(char);
uint8_t center(int count);
uint8_t reverseBin(uint8_t data);
void multiplex(char[], bool dots[],int count);
void rawWriteToScreen(uint8_t[2]);
void powerDisplay(bool);
void ScreenInit();
spi_master_state_t spiMasterState;
uint32_t calculatedBaudRate;
spi_master_user_config_t userConfig;

#define SPI_MASTER_INSTANCE          1
#define TRANSFER_BAUDRATE           (500000U)           /*! Transfer baudrate - 500k */

#endif
