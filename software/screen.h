#include "fsl_spi_master_driver.h"


uint8_t dictionary(char);
void spiLatch();
uint8_t addDot(char);
static spi_status_t SPI_Transfer(const uint8_t*, uint8_t*, size_t);
//SPI
#define SPI_MASTER_INSTANCE          1
#define TRANSFER_BAUDRATE           (500000U)           /*! Transfer baudrate - 500k */



