 // Standard C Included Files
#include <stdio.h>
// SDK Included Files
#include "fsl_os_abstraction.h"
#include "fsl_gpio_driver.h"
#include "fsl_debug_console.h"

#include "pins.c"

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
    GPIO_DRV_InputPinInit(&swtch_exec);
    GPIO_DRV_OutputPinInit(&LED_East);
    GPIO_DRV_OutputPinInit(&LED_North);
    GPIO_DRV_OutputPinInit(&LED_South);
    GPIO_DRV_OutputPinInit(&LED_West);
    while(1)
    {
        GPIO_DRV_WritePinOutput(LED_East.pinName,1);
        OSA_TimeDelay(500);
        GPIO_DRV_WritePinOutput(LED_East.pinName,0);
        GPIO_DRV_WritePinOutput(LED_North.pinName,1);
        OSA_TimeDelay(500);
        GPIO_DRV_WritePinOutput(LED_North.pinName,0);
        GPIO_DRV_WritePinOutput(LED_West.pinName,1);
        OSA_TimeDelay(500);
        GPIO_DRV_WritePinOutput(LED_West.pinName,0);
        GPIO_DRV_WritePinOutput(LED_South.pinName,1);
        OSA_TimeDelay(500);
        GPIO_DRV_WritePinOutput(LED_South.pinName,0);
    }
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
