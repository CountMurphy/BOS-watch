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
