#ifndef _RTC_H_
#define _RTC_H_

#include "fsl_i2c_master_driver.h"

#define DATA_LENGTH             64
#define BOARD_I2C_INSTANCE      0

uint8_t rtcRxBuff[DATA_LENGTH] = {0};
uint8_t rtcTx[DATA_LENGTH] = {0};

i2c_master_state_t rtc_state;


void printTime(uint8_t hour, uint8_t minute, uint8_t second);
void printDate();
void setRtc(uint8_t hours,uint8_t minutes, uint8_t second, uint8_t month, uint8_t day, uint8_t year, uint8_t dow);
uint8_t ConvertToBCD(uint8_t integer);
uint8_t ReadSecond();
void ReadHourMinute(uint8_t *minute,uint8_t *hour);
void PrintDOW();
uint8_t ReadDOW();
void RTC_init();
void RTC_deinit();
void ReadDate(uint8_t *month, uint8_t *day, uint8_t *year);
uint8_t AdjustUTCHour(uint8_t hour, int8_t offset);
void adjustDate(uint8_t *day,uint8_t *month,uint8_t * year,int8_t offset,uint8_t hour);
void calChange(uint8_t *day,uint8_t *month,uint8_t *year);
uint8_t rewindDays(uint8_t month,uint8_t year);
bool isLeapYear(uint8_t year);
uint8_t dayofweek(uint32_t y, uint8_t m,uint8_t  d);
#endif
