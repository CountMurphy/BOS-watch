 #include "fsl_i2c_master_driver.h"

#define DATA_LENGTH             64
#define BOARD_I2C_INSTANCE      0

uint8_t second;
uint8_t minute;
uint8_t hour;
uint8_t rtcRxBuff[DATA_LENGTH] = {0};
uint8_t rtcTx[DATA_LENGTH] = {0};


i2c_device_t rtc=
{
        .address=0x6F,
        .baudRate_kbps=100
};

i2c_master_state_t rtc_state;


void printTime();
void printDate();
void setRtc(uint8_t hours,uint8_t minutes, uint8_t second, uint8_t month, uint8_t day, uint8_t year, uint8_t dow);
uint8_t ConvertToBCD(uint8_t integer);
void ReadSecond();
void ReadHourMinute();
void RTC_init();
void RTC_deinit();
void ReadDate(uint8_t *month, uint8_t *day, uint8_t *year);
