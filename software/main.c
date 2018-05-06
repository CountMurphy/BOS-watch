// Standard C Included Files
#include <stdio.h>
#include <string.h>
// SDK Included Files
#include "fsl_os_abstraction.h"


#include "pins.h"
#include "screen.h"
#include "buzzer.h"
#include "rtc.h"
#include "gps.h"
#include "switch.h"


#define DATA_LENGTH             64
#define BOARD_I2C_INSTANCE      0

// Buffer store data to send to slave
uint8_t txBuff[DATA_LENGTH] = {0};
// Buffer store data to receive from slave
uint8_t rxBuff[DATA_LENGTH] = {0};
void RunClock(void);
void RunCompass();
void RunPewPew();
void RunGPS();
void RunBuzzer();
void RunStopWatch();
void RunStandBy();


int main (void)
{
    // Initialize
    hardware_init();
    OSA_Init();

    ScreenInit();
    SwitchInit();
    //lights init
    GPIO_DRV_OutputPinInit(&LED_North);
    GPIO_DRV_OutputPinInit(&LED_South);
    GPIO_DRV_OutputPinInit(&LED_West);
    GPIO_DRV_OutputPinInit(&LED_East);
    
    //prompt for dst
    bool dst=false;
    while(GetFirstRun())
    {
        bool dots[8]={false,false,false,false,false,false,false,false};
        if(GetMode()>0)
        {
            multiplex("is\0dst\0y",dots,8);
            dst=true;
        }else{
            multiplex("is\0dst\0n",dots,8);
            dst=false;
        }
    }

    //turn on gps
    GPSPower(true);


    WaitOnSatFix();
    //turn on UART
    GPSInit();
    uint8_t minute;
    uint8_t second;
    uint8_t hour;
    uint8_t month;
    uint8_t day;
    uint8_t year;
    uint32_t lon;
    uint8_t dow;
    char direction;
    ParseNMEA(&minute,&hour,&second,&day,&month,&year,&lon,&direction);
    GPSPower(false);

    //set clocks
    int8_t offset=GetLocalTimeZoneOffset(lon,direction);

    if(dst==true)
    {
        offset+=1;
    }

    RTC_init();
    adjustDate(&day,&month,&year,offset,hour);
    hour = AdjustUTCHour(hour,offset);
    dow=dayofweek((2000+year),month,day);
    setRtc(hour,minute,second,month,day,year,dow);

    PlayTheme();
    powerDisplay(true);

    // Master sends 1 bytes CMD and data to slave
    txBuff[0]=0x50;

    // OSA_TimeDelay(500);
    /*
    I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &compass_state);
    i2c_status_t retVal;
    retVal= I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, txBuff, 1, 1000);

    // Delay to wait slave received data
    OSA_TimeDelay(1);


    // Master receives count byte data from slave
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, rxBuff, 6, 1000);
*/

    // setRtc(21,15,59,11,12,18,1);

    for(;;)
    {
        while(!InterruptTriggered())
        {
            uint8_t mode=GetMainMode();
            switch(mode)
            {
            case 0:
            {
                RunClock();
                break;
            }
            case 1:
            {
                RunCompass();
                break;
            }
            case 2:
            {
                RunPewPew();
                break;
            }
            case 3:
            {
                RunGPS();
                break;
            }
            case 4:
            {
                RunBuzzer();
                break;
            }
            case 5:
            {
                RunStopWatch();
                break;
            }
            case 6:
            {
                RunStandBy();
                break;
            }
            }
        }
    }
    

    return 0;
}

void RunClock(void)
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;

    ReadHourMinute(&minute,&hour);
    second = ReadSecond();
    int rtcCounter=0;

    while(!InterruptTriggered())
    {
        switch(GetSubMode())
        {
        case 0:
            //Roughly 500 mils, dumb effort to save power by slowing down polling
            if(rtcCounter>=10)
            {
                rtcCounter=0;
                second = ReadSecond();
                if(second==0x00)
                {
                    ReadHourMinute(&minute,&hour);
                }
            }
            printTime(hour,minute,second);
            rtcCounter++;
            break;
        case 1:
            printDate();
            break;
        case 2:
            PrintDOW();
            break;
        }
    }
}

void RunBuzzer()
{
    bool dots[5]={false,false,false,false,false};
    while(!InterruptTriggered())
    {
        switch(GetSubMode())
        {
        case 0:
            multiplex("audio",dots,5);
            break;
        case 1:
            PlayTheme();
            break;
        case 2:
            Gieger();
            break;
        }
    }
}

void RunCompass()
{
    while(!InterruptTriggered())
    {

    }
}

void RunGPS()
{
    bool dots[3]={false,false,false};
    bool latDots[11]={false,false,false,false,false,false,false,false,false,false,false};
    char lat[9];
    char lon[10];
    double gLat;
    double gLon;
    char N_S;
    char E_W;

    while(!InterruptTriggered())
    {
        switch(GetSubMode())
        {
        case 0:
            multiplex("gps",dots,3);
            break;
        case 1:
            //print raw
            #if RELEASE
                GPSPower(true);
            #endif
            WaitOnSatFix();
            GetCurrentLocation(lat,&N_S,lon,&E_W);
            char fullLat[10];
            strcpy(fullLat,lat);
            fullLat[9]=N_S;
            Scroll(fullLat,latDots,9);
            char fullLon[11];
            strcpy(fullLon,lon);
            fullLon[10]=E_W;
            Scroll(fullLon,latDots,10);
            break;
        case 2:
            //print google ready
            #if RELEASE
                GPSPower(true);
            #endif
            WaitOnSatFix();
            GetCurrentLocation(lat,&N_S,lon,&E_W);
            GetGoogleReadyLocation(lat,N_S,lon,E_W,&gLat,&gLon);
            char chargLat[9];
            char chargLon[10];
            int ret =snprintf(chargLat, 9, "%f", 30.356145);
            if(ret<0){
                ret=2;
            }
            Scroll(chargLat,latDots,9);
            snprintf(chargLon, 10, "%f", gLon);
            Scroll(chargLon,latDots,10);
            break;
        }
    }
    GPSPower(false);
}

void RunPewPew()
{
    while(!InterruptTriggered())
    {

    }
}


void RunStandBy()
{
    while(!InterruptTriggered())
    {

    }
}

void RunStopWatch()
{
    while(!InterruptTriggered())
    {

    }
}
