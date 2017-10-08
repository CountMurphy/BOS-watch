#include "pins.h"


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
