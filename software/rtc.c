#include "rtc.h"
#include "screen.h"

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

void printTime()
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


void ReadSecond()
{
    uint8_t cmd[1]={0x00};
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    second=rtcRxBuff[0];
}

void ReadHourMinute()
{
    //read minute
    uint8_t cmd[1]= {0x01};
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    minute =rtcRxBuff[0];
    //read hour
    cmd[0]=0x02;
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &rtc, cmd, 1, rtcRxBuff, 1, 1000);
    hour =rtcRxBuff[0];
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
