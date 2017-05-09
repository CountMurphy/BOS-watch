 // Standard C Included Files
#include <stdio.h>
// SDK Included Files
#include "fsl_os_abstraction.h"
#include "fsl_i2c_hal.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"

#define DATA_LENGTH             64
#define BOARD_I2C_INSTANCE      1

// Buffer store data to send to slave
uint8_t txBuff[DATA_LENGTH] = {0};
// Buffer store data to receive from slave
uint8_t rxBuff[DATA_LENGTH] = {0};

i2c_device_t compass=
{
	.address=0x32,
	.baudRate_kbps=400
};

i2c_master_state_t compass_state;


int main (void)
{
    // Initialize i2c master
    I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &compass_state);

	// Master sends 1 bytes CMD and data to slave
    uint32_t cmd=0x50;

	// hardware_init();

    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, (const uint8_t*)&cmd, 1, (const uint8_t*)txBuff, cmd, 1000);

    int red=5;
    red =6;
    // Delay to wait slave received data
    // OSA_TimeDelay(25);

    // Master receives count byte data from slave
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, rxBuff, cmd, 1000);

    rxBuff;
    return 0;
}


		//http://www.nxp.com/assets/documents/data/en/supporting-information/Inter-Integrated-Circuit-Training.pdf
void hardware_init(void) {

  /* enable clock for PORTs */
  CLOCK_SYS_EnablePortClock(PORTA_IDX);
  CLOCK_SYS_EnablePortClock(PORTC_IDX);
  CLOCK_SYS_EnablePortClock(PORTD_IDX);

  configure_i2c_pins(1U);
  /* Init board clock */
  BOARD_ClockInit();
  dbg_uart_init();
}