#include "fsl_spi_master_driver.h"


uint8_t dictionary(char);
void spiLatch();
uint8_t addDot(char);
uint8_t center(int count);
uint8_t reverseBin(uint8_t data);
void multiplex(char[], bool dots[],int count);
//SPI
#define SPI_MASTER_INSTANCE          1
#define TRANSFER_BAUDRATE           (500000U)           /*! Transfer baudrate - 500k */



