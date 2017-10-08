 // Standard C Included Files
#include <stdio.h>
// SDK Included Files
#include "fsl_os_abstraction.h"
#include "fsl_i2c_hal.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_port_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_gpio_driver.h"
#include "fsl_debug_console.h"

#define DATA_LENGTH             64
#define BOARD_I2C_INSTANCE      0

// Buffer store data to send to slave
uint8_t txBuff[DATA_LENGTH] = {0};
// Buffer store data to receive from slave
uint8_t rxBuff[DATA_LENGTH] = {0};

i2c_device_t compass=
{
	.address=0x32>>1,
	.baudRate_kbps=100
};

i2c_master_state_t compass_state;

 static const gpio_output_pin_user_config_t wiggle={
     .pinName=GPIO_MAKE_PIN(GPIOD_IDX,3),
     .config.outputLogic=0
 };

 static const gpio_output_pin_user_config_t wiggle2={
     .pinName=GPIO_MAKE_PIN(GPIOD_IDX,5),
     .config.outputLogic=0
 };

 static const gpio_output_pin_user_config_t wiggle3 ={
     .pinName=GPIO_MAKE_PIN(GPIOE_IDX,0),
     .config.outputLogic=0
 };

  static const gpio_output_pin_user_config_t wiggle4 ={
     .pinName=GPIO_MAKE_PIN(GPIOD_IDX,1),
     .config.outputLogic=0
 };



 static const gpio_output_pin_user_config_t test={
     .pinName=GPIO_MAKE_PIN(GPIOA_IDX,1),
     .config.outputLogic=0
 };



int main (void)
{
    // Initialize i2c master
    hardware_init();
    OSA_Init();
GPIO_DRV_InputPinInit(&test);
GPIO_DRV_OutputPinInit(&wiggle);
GPIO_DRV_OutputPinInit(&wiggle2);
GPIO_DRV_OutputPinInit(&wiggle3);
GPIO_DRV_OutputPinInit(&wiggle4);
while(1)
{
    GPIO_DRV_WritePinOutput(wiggle.pinName,1);
    OSA_TimeDelay(500);
    GPIO_DRV_WritePinOutput(wiggle.pinName,0);
    GPIO_DRV_WritePinOutput(wiggle2.pinName,1);
    OSA_TimeDelay(500);
    GPIO_DRV_WritePinOutput(wiggle2.pinName,0);
    GPIO_DRV_WritePinOutput(wiggle3.pinName,1);
    OSA_TimeDelay(500);
    GPIO_DRV_WritePinOutput(wiggle3.pinName,0);
    GPIO_DRV_WritePinOutput(wiggle4.pinName,1);
    OSA_TimeDelay(500);
    GPIO_DRV_WritePinOutput(wiggle4.pinName,0);
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
void hardware_init(void) 
{

    CLOCK_SYS_EnablePortClock(PORTA_IDX);
    CLOCK_SYS_EnablePortClock(PORTC_IDX);
    CLOCK_SYS_EnablePortClock(PORTD_IDX);
    CLOCK_SYS_EnablePortClock(PORTE_IDX);

    PORT_HAL_SetMuxMode(PORTA,4u,kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(PORTA,1u,kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(PORTE,25u,kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(PORTE,24u,kPortMuxAsGpio);

    PORT_HAL_SetMuxMode(PORTD,3u,kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(PORTD,5u,kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(PORTE,0u,kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(PORTD,1u,kPortMuxAsGpio);
    //I2C0
    /* Affects PORTC_PCR8 register  */
    PORT_HAL_SetMuxMode(PORTC,8u,kPortMuxAlt2);
    /* Affects PORTB_PCR1 register */
    PORT_HAL_SetMuxMode(PORTC,9u,kPortMuxAlt2);
    //PORT_HAL_SetMuxMode(PORTC,9u,kPortMuxAsGpio);

}
