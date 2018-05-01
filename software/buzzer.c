#include "fsl_os_abstraction.h"
#include "fsl_gpio_driver.h"
#include "buzzer.h"
#include "switch.h"


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
