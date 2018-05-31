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
#include "pewpew.h"

static void RunClock(void)
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

static void RunBuzzer()
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

static void toggleDirectionLight(uint16_t heading)
{
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
}

static void RunCompass()
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
                multiplex("compass",wordDots,8);
                break;
            case 1:
                sprintf (charHead, "%u", heading);
                multiplex(charHead,dots,4);
                break;
            case 2:
                sprintf (charHead, "%u", heading);
                multiplex(charHead,dots,4);
                heading=heading/10;
                toggleDirectionLight(heading);
                break;
        }
    }
    GPIO_DRV_WritePinOutput(LED_North.pinName,0);
    GPIO_DRV_WritePinOutput(LED_East.pinName,0);
    GPIO_DRV_WritePinOutput(LED_West.pinName,0);
    GPIO_DRV_WritePinOutput(LED_South.pinName,0);
}

static void PrintRawGPS()
{
    bool latDots[11]={false,false,false,false,false,false,false,false,false,false,false};
    char lat[9];
    char lon[10];
    char N_S;
    char E_W;

#if RELEASE
    GPSPower(true);
    WaitOnSatFix();
#endif
    GetCurrentLocation(lat,&N_S,lon,&E_W);
    while(!InterruptTriggered())
    {

        char fullLat[11];
        strcpy(fullLat,lat);
        fullLat[9]=N_S;
        Scroll(fullLat,latDots,9);
        char fullLon[11];
        strcpy(fullLon,lon);
        fullLon[10]=E_W;
        Scroll(fullLon,latDots,10);
    }
}

static void PrintGoogleGPS()
{
    bool latDots[11]={false,false,false,false,false,false,false,false,false,false,false};
    char lat[9];
    char lon[10];
    double gLat;
    double gLon;
    char N_S;
    char E_W;

#if RELEASE
    GPSPower(true);
    WaitOnSatFix();
#endif
    GetCurrentLocation(lat,&N_S,lon,&E_W);
    GetGoogleReadyLocation(lat,N_S,lon,E_W,&gLat,&gLon);
    char chargLat[9];
    char chargLon[10];
    int ret =snprintf(chargLat, 9, "%f", 30.356145);
    if(ret<0)
    {
        ret=2;
    }
    while(!InterruptTriggered())
    {
        Scroll(chargLat,latDots,9);
        snprintf(chargLon, 10, "%f", gLon);
        Scroll(chargLon,latDots,10);
    }
}

static void RunGPS()
{
    bool dots[3]={false,false,false};

    while(!InterruptTriggered())
    {
        switch(GetSubMode())
        {
        case 0:
            multiplex("gps",dots,3);
            break;
        case 1:
            PrintRawGPS();
            break;
        case 2:
            //print google ready
            PrintGoogleGPS();
            break;
        }
    }
    GPSPower(false);
}

static void RunPewPew()
{
    bool dots[5]={false,false,false,false,false};
    while(!InterruptTriggered())
    {
        switch(GetSubMode())
        {
            case 0:
                multiplex("Armed",dots,5);
                FirnMahLaser(false);
                break;
            case 1:
                FirnMahLaser(true);
                break;
        }
    }
    FirnMahLaser(false);
}


static void RunStandBy()
{
    while(!InterruptTriggered())
    {

    }
}


static void CreateStopWatchString(char countedTime[],int8_t hour,int8_t minute,int8_t second)
{
    char minChar[2],hourChar[2],secChar[2];
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
}

static void SetCountDownTime(int8_t *hour,int8_t *minute,int8_t *second)
{
    *hour=0;
    *minute=0;
    *second=0;
    uint8_t mode=GetMode();
    char digToPrint[2];
    bool dots[2]={false,false};
    //Set hour
    for(int i=0;i<3;i++)
    {
        uint16_t timeDelay=0;
        while(mode==2)
        {
            mode=GetMode();
        }

        while(mode!=2)
        {
            mode=GetMode();
            switch(i)
            {
            case 0:
                digToPrint[0]=(*hour>>4)+48;
                digToPrint[1]=(*hour&0xF)+48;
                multiplex(digToPrint,dots,2);
                break;
            case 1:
                digToPrint[0]=(*minute>>4)+48;
                digToPrint[1]=(*minute&0xF)+48;
                multiplex(digToPrint,dots,2);
                break;
            case 2:
                digToPrint[0]=(*second>>4)+48;
                digToPrint[1]=(*second&0xF)+48;
                multiplex(digToPrint,dots,2);
                break;
            }
            if(mode==1 && timeDelay>=25)
            {
                timeDelay=0;
                switch(i)
                {
                case 0:
                    *hour+=1;
                    if(*hour==10)
                    {
                        *hour =0;
                    }
                    break;
                case 1:
                    *minute+=1;
                    if(*minute>59)
                    {
                        *minute=0;
                    }
                    break;
                case 2:
                    *second+=1;
                    if(*second>59)
                    {
                        *second=0;
                    }
                    break;
                }


            }
            timeDelay+=1;
        }
    }
}



static void RunStopWatch()
{
    int8_t minute=0;
    int8_t hour=0;
    int8_t second=0;
    int8_t milisecond=0;
    bool fuckingDots[8]={false,false,false,false,false,false,false,false};//for those grepping for swear words. You're welcome
    char countedTime[8];
    while(!InterruptTriggered())
    {
        
        switch(GetSubMode())
        {
        case 0:
            //stopwatch

            CreateStopWatchString(countedTime,hour,minute,second);
            //multiplex has a milisecond delay already
            multiplex(countedTime,fuckingDots,8);
            if(GetMode()==1)
            {
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
        case 1:
            //countdown
            milisecond=59;
            SetCountDownTime(&hour,&minute,&second);
            while(!InterruptTriggered())
            {
                CreateStopWatchString(countedTime,hour,minute,second);
                multiplex(countedTime,fuckingDots,8);
                milisecond--;
                if(second==0 && minute==0 && hour==0)
                {
                    while(!InterruptTriggered())
                    {
                        PlayAlarm();
                    }
                }else if(milisecond<0)
                {
                    milisecond=59;
                    second--;
                    if(second<0)
                    {
                        minute--;
                        second=59;
                        if(minute<0)
                        {
                            minute=59;
                            hour--;
                        }
                    }
                }
            }
            break;
        }
    }
}



int main (void)
{
    // Initialize
    hardware_init();
    OSA_Init();

    ScreenInit();
    SwitchInit();
    laserInit();
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

