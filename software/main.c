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
#include "compass.h"



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
        uint16_t heading = GetHeading();
        bool dots[4]={false,false,true,false};
        bool wordDots[8]={false,false,false,false,false,false,false,false};
        char charHead[4];
        switch(GetSubMode())
        {
            case 0:
                multiplex("connpass",wordDots,8);
                break;
            case 1:
                sprintf (charHead, "%u", heading);
                multiplex(charHead,dots,4);
                break;
            case 2:
                sprintf (charHead, "%u", heading);
                multiplex(charHead,dots,4);
                heading=heading/10;
                if((heading>=340 && heading <=360)|| (heading >=0 && heading<=20))
                {
                    //North
                    GPIO_DRV_WritePinOutput(LED_North.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
                }else if(heading >20 && heading <=40)
                {
                    //North East
                    GPIO_DRV_WritePinOutput(LED_North.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
                }else if(heading >40 && heading <=120)
                {
                    //East
                    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
                }else if(heading >120 && heading <=150)
                {
                    //South East
                    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,1);
                }else if(heading >150 && heading <=220)
                {
                    //South
                    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,1);
                }else if(heading >200 && heading <=230)
                {
                    //South West
                    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,1);
                }else if(heading > 230 && heading <=300)
                {
                    //West
                    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
                }else if(heading >300 && heading <340)
                {
                    //North West
                    GPIO_DRV_WritePinOutput(LED_North.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
                    GPIO_DRV_WritePinOutput(LED_West.pinName,1);
                    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
                }
                break;
        }
    }
    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
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
    uint8_t minute=0;
    uint8_t hour=0;
    uint8_t second=0;
    uint8_t milisecond=0;
    char minChar[2],hourChar[2],secChar[2];
    bool fuckingDots[8]={false,false,false,false,false,false,false,false};//for those grepping for swear words. You're welcome
    char countedTime[8];
    while(!InterruptTriggered())
    {
        switch(GetSubMode())
        {
        case 0:
            sprintf (hourChar, "%u", hour);
            sprintf (minChar, "%u", minute);
            sprintf (secChar, "%u", second);
            countedTime[0]=hourChar[0];
            countedTime[1]=hourChar[1];
            countedTime[2]='\0';
            countedTime[3]=minChar[0];
            countedTime[4]=minChar[1];
            countedTime[5]='\0';
            countedTime[6]=secChar[0];
            countedTime[7]=secChar[1];
            //multiplex has a milisecond delay already
            multiplex(countedTime,fuckingDots,8);
            if(GetMode()==1)
            {
                // OSA_TimeDelay(1000);
                milisecond++;
                if(milisecond==60)
                {
                    milisecond=0;
                    second++;
                    if(second==60)
                    {
                        second=0;
                        minute++;
                    }
                }
            }else if(GetMode()>1)
            {
                hour=minute=second=0;
            }
            break;
        }
    }
}
