#include "rtc.h"
#include "screen.h"


i2c_device_t rtc=
{
    .address=0x6F,
    .baudRate_kbps=100
};

void RTC_init()
{
    I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &rtc_state);
}

void RTC_deinit()
{
    I2C_DRV_MasterDeinit(BOARD_I2C_INSTANCE);
}


void setRtc(uint8_t hours,uint8_t minutes, uint8_t second, uint8_t month, uint8_t day, uint8_t year, uint8_t dow)
{
    hours = ConvertToBCD(hours);
    minutes = ConvertToBCD(minutes);
    second = ConvertToBCD(second);
    month = ConvertToBCD(month);
    day = ConvertToBCD(day);
    year = ConvertToBCD(year);

    uint8_t cmd[64] = {0};
    cmd[0]=0x07;
    rtcTx[0]=0x10;

    //enable write access to RTC
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);

    //set Seconds
    cmd[0]=0x00;
    rtcTx[0]=second;
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);

    //set Minutes
    cmd[0]=0x01;
    rtcTx[0]=minutes;
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);

    //set Hour
    cmd[0]=0x02;
    rtcTx[0]=hours;
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);

    //set Day
    cmd[0]=0x03;
    rtcTx[0]=day;
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);

    //set Month
    cmd[0]=0x04;
    rtcTx[0]=month;
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);

    //set Year
    cmd[0]=0x05;
    rtcTx[0]=year;
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);

    //set DOW
    cmd[0]=0x06;
    rtcTx[0]=dow;
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcTx, 1, 1000);
}


uint8_t ConvertToBCD(uint8_t integer)
{
    uint8_t tens = (integer/ 10) << 4;
    uint8_t ones = integer- ((integer/ 10) * 10);
    return ones | tens;
}

void printTime(uint8_t hour, uint8_t minute, uint8_t second)
{

    //if out put it 0x10, seperate the 1 and the 0 and convert to char
    char time[8] = {(hour>>4)+48,(hour&0xF)+48,'\0',(minute>>4)+48,(minute&0xF)+48,'\0',(second>>4)+48,(second&0xF)+48};

    bool timeDots[8] = {0,1,0,0,1,0,0,0};
    multiplex(time,timeDots,8);
}


void printDate()
{
    uint8_t month;
    uint8_t day;
    uint8_t year;
    ReadDate(&month,&day,&year);

    char date[8] = {(month>>4)+48,(month&0xF)+48,'\0',(day>>4)+48,(day&0xF)+48,'\0',(year>>4)+48,(year&0xF)+48};

    bool timeDots[8] = {0,1,0,0,1,0,0,0};
    multiplex(date,timeDots,8);
}


uint8_t ReadSecond()
{
    uint8_t second;
    uint8_t cmd[1]={0x00};
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    second=rtcRxBuff[0];
    return second;
}

void ReadHourMinute(uint8_t *minute,uint8_t *hour)
{
    //read minute
    uint8_t cmd[1]= {0x01};
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    *minute =rtcRxBuff[0];
    //read hour
    cmd[0]=0x02;
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    *hour =rtcRxBuff[0];
}

void ReadDate(uint8_t *month, uint8_t *day, uint8_t *year)
{
    //Get Month
    uint8_t cmd[1]= {0x04};
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    *month= rtcRxBuff[0];

    //Get day
    cmd[0]= 0x03;
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    *day = rtcRxBuff[0];

    //Get Year
    cmd[0]= 0x05;
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    *year = rtcRxBuff[0];
}

uint8_t AdjustUTCHour(uint8_t hour, int8_t offset)
{
    if ((hour + offset) < 0)
    {
        //corrections add or sub a day
        switch(hour + offset)
        {
        //test
        case -1:
            hour=23;
            break;
        case -2:
            hour=22;
            break;
        case -3:
            hour=21;
            break;
        case -4:
            hour=20;
            break;
        case -5:
            hour=19;
            break;
        case -6:
            hour=18;
            break;
        case -7:
            hour=17;
            break;
        case -8:
            hour=16;
        case -9:
            hour=15;
            break;
        case -10:
            hour=14;
            break;
        case -11:
            hour=13;
            break;
        }
    }
    else if ((hour+offset) > 23)
    {
        //eastern hemesphere
        switch(hour+offset)
        {
        case 24:
            hour=0;
            break;
        case 25:
            hour=1;
            break;
        case 26:
            hour=2;
            break;
        case 27:
            hour=3;
            break;
        case 28:
            hour=4;
            break;
        case 29:
            hour=5;
            break;
        case 30:
            hour=6;
            break;
        case 31:
            hour=7;
            break;
        case 32:
            hour=8;
            break;
        case 33:
            hour=9;
            break;
        case 34:
            hour=10;
            break;
        }
    }
    else
    {
        hour=hour+offset;
    }

    return hour;
}



void adjustDate(uint8_t *day,uint8_t *month,uint8_t * year,int8_t offset,uint8_t hour)
{
    //check to see if needed
    if ((hour+offset) >23 || (hour+offset) < 0)
    {
        //date needs to change
        if ((hour+offset) > 23)
            *day=+1;
        else
            *day-=1;
        calChange(day,month,year);
    }
}

void calChange(uint8_t *day,uint8_t *month,uint8_t *year)
{
    //
    bool alreadyCorrected=false;
    //31 day months
    if (*month==1 || *month==3 || *month==5 || *month==7 || *month==8 || *month==10 || *month==12)
    {
        alreadyCorrected=true;
        //going forward
        if (*day > 31)
        {
            *month=*month+1;
            *day=1;
            if (*month > 12)
            {
                *month=1;
                *year+=1;
            }
        }

        //going back
        if (*day < 1)
        {
            *month-=1;
            *day=rewindDays(*month,*year);
            //if was jan, now dec
            if (*month < 1)
            {
                *month=12;
                *day=31;
                *year-=1;
            }
        }
    }

    //30 day months
    if (*month==4 || *month==6 || *month==9 || *month==11)
    {
        if (*day > 30)
        {
            *month+=1;
            *day=1;
        }
        else
        {
            if(*day < 1)
            {
                //going back
                *month-=1;
                *day=rewindDays(*month,*year);
            }
        }
    }



    //Feb...include leap years
    if (*month== 2 && alreadyCorrected==false)
    {
        if(isLeapYear(*year) ==true)
        {
            if (*day >= 29)
            {
                if (*day != 29)
                {
                    *day=1;
                    *month=3;
                }
            }
            else{
                if (*day < 1)
                {
                    *day=31;
                    *month=1;
                }
            }
        }
        else{
            if (*day >=28)
            {
                if (*day != 28)
                {
                    *day=1;
                    *month=3;
                }
            }
            else{
                if (*day < 1)
                {
                    *day=31;
                    *month=1;
                }
            }
        }
    }
}

uint8_t rewindDays(uint8_t month,uint8_t year)
{
    uint8_t day;
    switch(month)
    {
    case 4:
        day=30;
        break;
    case 6:
        day=30;
        break;
    case 9:
    day=30;
    break;
    case 11:
        day=30;
        break;
    case 2:
        if(isLeapYear(year) ==true) 
            day=29;
        else
            day=28;
    default:
        day=31;

    }
    return day;
}

bool isLeapYear(uint8_t year)
{
    if(year%4 ==0)
    return true;
  else
    return false;
}

uint8_t dayofweek(uint32_t y, uint8_t m,uint8_t  d)  /* 1 <= m <= 12,  y > 1752 (in the U.K.) */
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}