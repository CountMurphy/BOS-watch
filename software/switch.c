#include "fsl_gpio_driver.h"
#include "fsl_pit_driver.h"
#include "switch.h"
#include "buzzer.h"
#include "pins.h"
#include "screen.h"

static const gpio_input_pin_user_config_t switch_exec={
    .pinName=GPIO_MAKE_PIN(GPIOA_IDX,4),
    .config.interrupt = kPortIntEitherEdge
};

static const gpio_input_pin_user_config_t switch_one={
    .pinName=GPIO_MAKE_PIN(GPIOE_IDX,24),
};

static const gpio_input_pin_user_config_t switch_two={
    .pinName=GPIO_MAKE_PIN(GPIOE_IDX,25),
};

static const gpio_input_pin_user_config_t switch_four={
    .pinName=GPIO_MAKE_PIN(GPIOA_IDX,1),
    .config.interrupt = kPortIntEitherEdge
};

static const pit_user_config_t g_pitChan0 = {
    .periodUs = 3000000u,
    .isInterruptEnabled = true,
};

static bool isSubOn=false;
static bool isMainOn=false;
uint8_t mainMode=0;
uint8_t subMode=0;

void PIT_IRQHandler(void)
{
    if (PIT_HAL_IsIntPending(g_pitBase[0], 0))
    {
        PIT_HAL_ClearIntFlag(g_pitBase[0], 0);
        CommandSetAck();
    }
    if (PIT_HAL_IsIntPending(g_pitBase[0], 1))
    {
        PIT_HAL_ClearIntFlag(g_pitBase[0], 1);
    }
}

void PORTA_IRQHandler(void)
{
    PIT_DRV_StopTimer(0, 0);


    /* Clear interrupt flag.*/
    PORT_HAL_ClearPortIntFlag(PORTA_BASE_PTR);
    //test
    if(isMainOn && GetExecStatus()==false)
    {
        powerDisplay(true);
        //Main command entered
        mainMode=GetMode();
        subMode=0;
        isMainOn=false;

    }else if(isSubOn && GetSubStatus()==false)
    {
        //Sub command entered
        powerDisplay(true);
        subMode=GetMode();
        isSubOn=false;
        
    }else if(!isSubOn && !isMainOn)
    {
        //entered into command set mode
        if(GetExecStatus())
        {
            powerDisplay(false);
            CommandSetAck();
            isMainOn=true;
            PIT_DRV_StartTimer(0, 0);
        }else if(GetSubStatus())
        {
            powerDisplay(false);
            CommandSetAck();
            isSubOn=true;
            PIT_DRV_StartTimer(0, 0);
        }
    }

    OSA_TimeDelay(10);//debounce

}
void SwitchInit()
{
    GPIO_DRV_InputPinInit(&switch_exec);
    GPIO_DRV_InputPinInit(&switch_one);
    GPIO_DRV_InputPinInit(&switch_two);
    GPIO_DRV_InputPinInit(&switch_four);

    GPIO_DRV_OutputPinInit(&LED_North);
    GPIO_DRV_OutputPinInit(&LED_South);
    GPIO_DRV_OutputPinInit(&LED_East);
    GPIO_DRV_OutputPinInit(&LED_West);
     PIT_DRV_Init(0, false);
    PIT_DRV_InitChannel(0, 0, &g_pitChan0);
}

uint8_t GetMode()
{
    uint8_t mode=0;
    if(GPIO_DRV_ReadPinInput(switch_one.pinName))
    {
        mode+=1;
    }
    if(GPIO_DRV_ReadPinInput(switch_two.pinName))
    {
        mode+=2;
    }
    if(GPIO_DRV_ReadPinInput(switch_four.pinName))
    {
        mode+=4;
    }

    return mode;
}

bool GetExecStatus()
{
    return GPIO_DRV_ReadPinInput(switch_exec.pinName);
}

bool GetSubStatus()
{
    return GPIO_DRV_ReadPinInput(switch_four.pinName);
}

void CommandSetAck()
{
    OSA_TimeDelay(125);
    GPIO_DRV_WritePinOutput(LED_East.pinName,1);
    GPIO_DRV_WritePinOutput(LED_West.pinName,1);
    if(!GetSubStatus() && GetExecStatus())
    {
        GPIO_DRV_WritePinOutput(LED_North.pinName,1);
        GPIO_DRV_WritePinOutput(LED_South.pinName,1);
    }
    Click();
    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
}

bool InterruptTriggered()
{
    return isSubOn || isMainOn;
}

uint8_t GetMainMode()
{
    return mainMode;
}

uint8_t GetSubMode()
{
    return subMode;
}