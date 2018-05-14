#include "pewpew.h"

static const gpio_output_pin_user_config_t laser={
    .pinName=GPIO_MAKE_PIN(GPIOE_IDX,1),
    .config.outputLogic=0
};

void laserInit()
{
    GPIO_DRV_OutputPinInit(&laser);
}

void FirnMahLaser(bool enable)
{
    if(enable)
    {
        GPIO_DRV_WritePinOutput(laser.pinName,1);
    }
    else
    {
        GPIO_DRV_WritePinOutput(laser.pinName,0);
    }
}

