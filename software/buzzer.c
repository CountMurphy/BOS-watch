#include "fsl_os_abstraction.h"
#include "fsl_gpio_driver.h"
#include "buzzer.h"
#include "switch.h"
#include "pins.h"


void Click()
{
    TPM_DRV_Init(0, &tmpConfig);
    TPM_DRV_SetClock(0, kTpmClockSourceModuleMCGIRCLK, kTpmDividedBy1);
    param.uFrequencyHZ=50;
    TPM_DRV_PwmStart(0,&param,3);
    OSA_TimeDelay(125);
    TPM_DRV_PwmStop(0,&param,3);
}

void Gieger()
{
    TPM_DRV_Init(0, &tmpConfig);
    TPM_DRV_SetClock(0, kTpmClockSourceModuleMCGIRCLK, kTpmDividedBy1);
    if((rand() %20) < 10)
    {
        OSA_TimeDelay(125);
    }
    else
    {
        param.uFrequencyHZ=0;
        TPM_DRV_PwmStart(0,&param,3);
        OSA_TimeDelay(92);
        TPM_DRV_PwmStop(0,&param,3);
    }
}

void PlayAlarm()
{
    TPM_DRV_Init(0, &tmpConfig);
    TPM_DRV_SetClock(0, kTpmClockSourceModuleMCGIRCLK, kTpmDividedBy1);
    param.uFrequencyHZ=20;
    TPM_DRV_PwmStart(0,&param,3);
    OSA_TimeDelay(125);
    TPM_DRV_PwmStop(0,&param,3);
}

void PlayTheme()
{
    /* Play some chip tunez */

    TPM_DRV_Init(0, &tmpConfig);
    TPM_DRV_SetClock(0, kTpmClockSourceModuleMCGIRCLK, kTpmDividedBy1);

    uint16_t notes[] = {
        0,   // rest
        2093, //C7 	1
        1047, //C6	2
        1568, //G6	3
        1865, //A#6	4
        2349, //D7	5
        1397, //F6	6
        1319, //E6	7
        523,	 //C5	8
        622,  //D#5	9
        659,  //E5	10
    };
    uint8_t beeps[] = {
        1, 2,3,4,0,5,4,0,6,6,7,0,8,9,10

    };
    for(int i = 0;  i <  sizeof(beeps);(i=i+1) ) {
        if(InterruptTriggered())
        {
            break;
        }
        param.uFrequencyHZ = notes[beeps[i]];
        if (param.uFrequencyHZ)
            TPM_DRV_PwmStart(0, &param, 3);
        else
            TPM_DRV_PwmStop(0, &param, 3);
        if(beeps[i]==9)
        {
            OSA_TimeDelay(1500);
        }else{
            OSA_TimeDelay(850);
        }
        if(i==sizeof(beeps)-1)
        {
            TPM_DRV_PwmStop(0, &param, 3);
            break;
        }
    }

}

void TakeOnMe()
{
    uint16_t notes[] = {
        0,   // rest
        494, // B4  1
        523, // C5  2
        554, // C#5 3
        587, // D5  4
        622, // D#5 5
        659, // E5  6
        698, // F5  7
        740, // F#5 8
        784, // G5  9
        830, // G#5 10
        880, // A5  11
        932, // A#5 12
        988, // B5  13
    };
    uint8_t beeps[] = {
         8,  8,  4,  1,
         0,  1,  0,  6,
         0,  6,  0,  6,
        10, 10, 11, 13,
        11, 11, 11,  6,
         0,  4,  0,  8,
         0,  8,  0,  8,
         6,  6,  8,  6,
    };

        for (int i = 0; ; i = (i + 1) % sizeof(beeps)) 
        {
            KillTakeOnMeLights();
            if(InterruptTriggered())
            {
                break;
            }
            param.uFrequencyHZ = notes[beeps[i]];
            if (param.uFrequencyHZ)
            {
                TPM_DRV_PwmStart(0, &param, 3);
                TakeOnMeFlashes(beeps[i]);
            }
            else
                TPM_DRV_PwmStop(0, &param, 3);
            OSA_TimeDelay(193);
    }
}

void TakeOnMeFlashes(int position)
{
    switch(position)
    {
        case 8:
        case 10:
            GPIO_DRV_WritePinOutput(LED_North.pinName,1);
            break;
        case 4:
        case 11:
            GPIO_DRV_WritePinOutput(LED_South.pinName,1);
            break;
        case 1:
        case 13:
            GPIO_DRV_WritePinOutput(LED_East.pinName,1);
            break;
        case 6:
            GPIO_DRV_WritePinOutput(LED_West.pinName,1);
    }
}

void KillTakeOnMeLights()
{
    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
}