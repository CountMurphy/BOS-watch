#include "fsl_os_abstraction.h"
#include "fsl_i2c_hal.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_port_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_gpio_driver.h"
#include "fsl_debug_console.h"

i2c_device_t compass=
{
        .address=0x32>>1,
        .baudRate_kbps=100
};

i2c_master_state_t compass_state;

 static const gpio_output_pin_user_config_t LED_East={
     .pinName=GPIO_MAKE_PIN(GPIOD_IDX,3),
     .config.outputLogic=0
 };

 static const gpio_output_pin_user_config_t LED_North={
     .pinName=GPIO_MAKE_PIN(GPIOD_IDX,5),
     .config.outputLogic=0
 };

 static const gpio_output_pin_user_config_t LED_South={
     .pinName=GPIO_MAKE_PIN(GPIOE_IDX,0),
     .config.outputLogic=0
 };

  static const gpio_output_pin_user_config_t LED_West={
     .pinName=GPIO_MAKE_PIN(GPIOD_IDX,1),
     .config.outputLogic=0
 };



 static const gpio_input_pin_user_config_t swtch_exec={
     .pinName=GPIO_MAKE_PIN(GPIOA_IDX,1)
 };


void hardware_init(void)
{

    CLOCK_SYS_EnablePortClock(PORTA_IDX);
    CLOCK_SYS_EnablePortClock(PORTC_IDX);
    CLOCK_SYS_EnablePortClock(PORTD_IDX);
    CLOCK_SYS_EnablePortClock(PORTE_IDX);

    //input switches
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
