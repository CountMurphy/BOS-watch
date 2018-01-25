#include "pins.h"


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


    //SPI
    //SPI bubble screen
      PORT_HAL_SetMuxMode(PORTC,3u,kPortMuxAlt2);
      PORT_HAL_SetMuxMode(PORTB,16u,kPortMuxAlt2);

      PORT_HAL_SetMuxMode(PORTB,18u,kPortMuxAsGpio);
      PORT_HAL_SetMuxMode(PORTD,4u,kPortMuxAsGpio);

      //buzzer
      PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 30, kPortMuxAlt3);
}
