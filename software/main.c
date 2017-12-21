// Standard C Included Files
#include <stdio.h>
// SDK Included Files
#include "fsl_os_abstraction.h"
#include "fsl_gpio_driver.h"
#include "fsl_debug_console.h"

#include "fsl_spi_master_driver.h"

#include "pins.c"

#define DATA_LENGTH             64
#define BOARD_I2C_INSTANCE      0
//for headers
uint8_t dictionary(char);
void spiLatch();
uint8_t addDot(char);

// Buffer store data to send to slave
uint8_t txBuff[DATA_LENGTH] = {0};
// Buffer store data to receive from slave
uint8_t rxBuff[DATA_LENGTH] = {0};


//SPI
#define SPI_MASTER_INSTANCE          1
#define TRANSFER_BAUDRATE           (500000U)           /*! Transfer baudrate - 500k */


static spi_status_t SPI_Transfer(const uint8_t *tx, uint8_t *rx, size_t count)
{
    spi_status_t rc;

    rc = SPI_DRV_MasterTransfer(1, NULL, tx, rx, count);
    if (rc != kStatus_SPI_Success) {
        return rc;
    }
    int i, timeout = (count + 127) / 128 + 1;
    for (i = 0; i < timeout; ++i) {
        rc = SPI_DRV_MasterGetTransferStatus(1, NULL);
        if (rc == kStatus_SPI_Success) {
           return rc;
        }
    }
    return rc;
}


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
    digit=addDot(digit);
    uint8_t spiData[2] = {0x00,digit};


    //           SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE, NULL, spiData,NULL, TRANSFER_SIZE, MASTER_TRANSFER_TIMEOUT);
    uint8_t  rx[2];
    GPIO_DRV_OutputPinInit(&spiAccept);
    GPIO_DRV_WritePinOutput(spiAccept.pinName,0);

    GPIO_DRV_OutputPinInit(&displayOutput);
    GPIO_DRV_WritePinOutput(displayOutput.pinName,0);

    SPI_Transfer(spiData,rx,2);

    spiLatch();


    while(1)
    {
        GPIO_DRV_WritePinOutput(displayOutput.pinName,1);
        OSA_TimeDelay(1000);
        GPIO_DRV_WritePinOutput(displayOutput.pinName,0);
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


                //http://www.nxp.com/assets/documents/data/en/supporting-information/Inter-Integrated-Circuit-Training.pdf
void spiLatch()
{
                //have shift registers accept current values
            GPIO_DRV_WritePinOutput(spiAccept.pinName,1);
            OSA_TimeDelay(100);
            GPIO_DRV_WritePinOutput(spiAccept.pinName,0);
}

uint8_t dictionary(char digit)
{
    uint8_t retVal;
    switch(digit)
    {
    case '0':
    case 'O':
        retVal=0b00111111;
        break;
    case '1':
    case 'I':
        retVal=0b00000110;
        break;
    case '2':
        retVal=0b01011011;
        break;
    case '3':
        retVal=0b01001111;
        break;
    case '4':
        retVal=0b01100110;
        break;
    case '5':
    case 's':
    case 'S':
        retVal=0b01101101;
        break;
    case '6':
    case 'g':
    case 'G':
        retVal=0b01111101;
        break;
    case '7':
        retVal=0b00000111;
        break;
    case '8':
        retVal=0b01111111;
        break;
    case '9':
    case 'q':
    case 'Q':
        retVal=0b01100111;
        break;
    case 'A':
    case 'a':
        retVal=0b01110111;
        break;
    case 'b':
    case 'B':
        retVal=0b01111100;
        break;
    case 'c':
        retVal=0b01011000;
        break;
    case 'C':
        retVal=0b00111001;
        break;
    case 'D':
    case 'd':
        retVal=0b01011110;
        break;
    case 'e':
    case 'E':
        retVal=0b01111001;
        break;
    case 'f':
    case 'F':
        retVal=0b01110001;
        break;
    case 'H':
        retVal=0b01110110;
        break;
    case 'h':
        retVal=0b01110100;
        break;
    case 'i':
        retVal=0b00000100;
        break;
    case 'j':
    case 'J':
        retVal=0b00001110;
        break;
    case 'k':
    case 'K':
        retVal=0b01111010;
        break;
    case 'L':
        retVal=0b00111000;
        break;
        //m an M.....just do a double 'n'
    case 'l':
        retVal=0b00110000;
        break;
    case 'n':
        retVal=0b01010100;
        break;
    case 'o':
        retVal=0b01011100;
        break;
    case 'P':
    case 'p':
        retVal=0b01110011;
        break;
    case 'r':
        retVal=0b01010000;
        break;
    case 'R':
        retVal=0b00110011;
        break;
    case 't':
    case 'T':
        retVal=0b01111000;
        break;
    case 'U':
        retVal=0b00111110;
        break;
    case 'v':
    case 'V':
    case 'u':
        retVal=0b00011100;
        break;
    }
    return retVal;
}

uint8_t addDot(char digit)
{
    uint8_t dot= 0x80;
    return  digit^dot;
}
