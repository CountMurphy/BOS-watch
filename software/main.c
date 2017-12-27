// Standard C Included Files
#include <stdio.h>
// SDK Included Files
#include "fsl_os_abstraction.h"
#include "fsl_gpio_driver.h"
#include "fsl_debug_console.h"


#include "pins.c"
#include "screen.c"

#define DATA_LENGTH             64
#define BOARD_I2C_INSTANCE      0

// Buffer store data to send to slave
uint8_t txBuff[DATA_LENGTH] = {0};
// Buffer store data to receive from slave
uint8_t rxBuff[DATA_LENGTH] = {0};



int main (void)
{
    // Initialize i2c master
    hardware_init();
    OSA_Init();

    //SPI

    spi_master_state_t spiMasterState;
    spi_master_user_config_t userConfig =
    {
            .bitsPerSec = 2000000, /* 2 MHz */
            .polarity = kSpiClockPolarity_ActiveHigh,
            .phase = kSpiClockPhase_FirstEdge,
            .direction = kSpiMsbFirst,
            .bitCount = kSpi8BitMode,
    };

    uint32_t calculatedBaudRate;

    SPI_DRV_MasterInit(SPI_MASTER_INSTANCE, &spiMasterState);

    SPI_DRV_MasterConfigureBus(SPI_MASTER_INSTANCE,&userConfig,&calculatedBaudRate);

    uint8_t digit = dictionary('8');
//    digit=addDot(digit);
    uint8_t spiData[2] = {center(2),digit};


    //           SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE, NULL, spiData,NULL, TRANSFER_SIZE, MASTER_TRANSFER_TIMEOUT);
    uint8_t  rx[2];
    GPIO_DRV_OutputPinInit(&spiAccept);

    GPIO_DRV_OutputPinInit(&displayOutput);

    SPI_Transfer(spiData,rx,2);

    spiLatch();


    while(1)
    {
        GPIO_DRV_WritePinOutput(displayOutput.pinName,0);
        OSA_TimeDelay(1000);
        GPIO_DRV_WritePinOutput(displayOutput.pinName,1);
        OSA_TimeDelay(1000);
    }

    return 0;
    I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &compass_state);

    // Master sends 1 bytes CMD and data to slave
    txBuff[0]=0x50;

    // OSA_TimeDelay(500);

    i2c_status_t retVal;
    retVal= I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, txBuff, 1, 1000);

    // Delay to wait slave received data
    OSA_TimeDelay(1);


    // Master receives count byte data from slave
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, rxBuff, 6, 1000);


    return 0;
}
